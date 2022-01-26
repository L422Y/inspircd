/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2013-2015 Attila Molnar <attilamolnar@hush.com>
 *   Copyright (C) 2012 Robby <robby@chatbelgie.be>
 *   Copyright (C) 2009-2010 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2008, 2010 Craig Edwards <brain@inspircd.org>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "inspircd.h"
#include "threadsocket.h"

#if __has_include(<sys/eventfd.h>)
#include <sys/eventfd.h>

class ThreadSignalSocket final
	: public EventHandler
{
	SocketThread* parent;
public:
	ThreadSignalSocket(SocketThread* p, int newfd) : parent(p)
	{
		SetFd(newfd);
		SocketEngine::AddFd(this, FD_WANT_FAST_READ | FD_WANT_NO_WRITE);
	}

	~ThreadSignalSocket() override
	{
		SocketEngine::Close(this);
	}

	void Notify()
	{
		eventfd_write(fd, 1);
	}

	void OnEventHandlerRead() override
	{
		eventfd_t dummy;
		eventfd_read(fd, &dummy);
		parent->OnNotify();
	}

	void OnEventHandlerWrite() override
	{
		ServerInstance->GlobalCulls.AddItem(this);
	}

	void OnEventHandlerError(int errcode) override
	{
		ThreadSignalSocket::OnEventHandlerWrite();
	}
};

SocketThread::SocketThread()
{
	socket = NULL;
	int fd = eventfd(0, EFD_NONBLOCK);
	if (fd < 0)
		throw CoreException("Could not create pipe " + std::string(strerror(errno)));
	socket = new ThreadSignalSocket(this, fd);
}
#else

class ThreadSignalSocket final
	: public EventHandler
{
	SocketThread* parent;
	int send_fd;
public:
	ThreadSignalSocket(SocketThread* p, int recvfd, int sendfd) :
		parent(p), send_fd(sendfd)
	{
		SetFd(recvfd);
		SocketEngine::NonBlocking(fd);
		SocketEngine::AddFd(this, FD_WANT_FAST_READ | FD_WANT_NO_WRITE);
	}

	~ThreadSignalSocket()
	{
		close(send_fd);
		SocketEngine::Close(this);
	}

	void Notify()
	{
		static constexpr char dummy = '*';
		write(send_fd, &dummy, 1);
	}

	void OnEventHandlerRead() override
	{
		char dummy[128];
		read(fd, dummy, 128);
		parent->OnNotify();
	}

	void OnEventHandlerWrite() override
	{
		ServerInstance->GlobalCulls.AddItem(this);
	}

	void OnEventHandlerError(int errcode) override
	{
		ThreadSignalSocket::OnEventHandlerWrite();
	}
};

SocketThread::SocketThread()
{
	socket = NULL;
	int fds[2];
	if (pipe(fds))
		throw CoreException("Could not create pipe " + std::string(strerror(errno)));
	socket = new ThreadSignalSocket(this, fds[0], fds[1]);
}
#endif

void SocketThread::NotifyParent()
{
	socket->Notify();
}

SocketThread::~SocketThread()
{
	if (socket)
	{
		socket->Cull();
		delete socket;
	}
}
