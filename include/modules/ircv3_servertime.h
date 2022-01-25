/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2018-2019 Sadie Powell <sadie@witchery.services>
 *   Copyright (C) 2018 Attila Molnar <attilamolnar@hush.com>
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


#pragma once

namespace IRCv3
{
	namespace ServerTime
	{
		class Manager;
		class API;

		/** Format a unix timestamp into the format used by server-time.
		 * @param secs UNIX timestamp to format.
		 * @params millisecs Number of milliseconds to format.
		 * @return Time in server-time format, as a string.
		 */
		inline std::string FormatTime(time_t secs, long millisecs = 0)
		{
			std::string timestr = InspIRCd::TimeString(secs, "%Y-%m-%dT%H:%M:%S.Z", true);
			timestr.insert(20, InspIRCd::Format("%03ld", millisecs));
			return timestr;
		}
	}
}

/** Implements manipulating the server time on messages.
 * A timestamp can be attached to outgoing client protocol messages to indicate the time when the message
 * was generated by us. If a message has server time attached then recipient clients who have negotiated
 * the appropriate protocol extension will receive it.
 */
class IRCv3::ServerTime::Manager
	: public DataProvider
{
protected:
	ClientProtocol::MessageTagProvider* tagprov;

public:
	/** Constructor.
	 * @param mod Module that owns the Manager.
	 */
	Manager(Module* mod)
		: DataProvider(mod, "servertimeapi")
	{
	}

	/** Set the server time on a message.
	 * @param msg Message to set the time on. No-op if the message already has server time set.
	 * @param t Unix timestamp to set.
	 */
	void Set(ClientProtocol::Message& msg, time_t t)
	{
		Set(msg, FormatTime(t));
	}

	/** Set the server time on a message.
	 * @param msg Message to set the time on. No-op if the message already has server time set.
	 * @param timestr Timestamp to set. Must be in server time format.
	 * The FormatTime() function can be used to convert unix timestamps into the required format.
	 */
	void Set(ClientProtocol::Message& msg, const std::string& timestr)
	{
		msg.AddTag("time", tagprov, timestr);
	}
};

/** Server time API. Use this to access the Manager.
 */
class IRCv3::ServerTime::API final
	: public dynamic_reference_nocheck<Manager>
{
public:
	API(Module* mod)
		: dynamic_reference_nocheck<Manager>(mod, "servertimeapi")
	{
	}
};
