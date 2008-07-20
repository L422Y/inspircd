/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2008 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"
#include "commands/cmd_info.h"

extern "C" DllExport Command* init_command(InspIRCd* Instance)
{
	return new CommandInfo(Instance);
}

/** Handle /INFO
 */
CmdResult CommandInfo::Handle (const std::vector<std::string>&, User *user)
{
	user->WriteNumeric(RPL_INFO, "%s :                   -/\\- \2InspIRCd\2 -\\/-", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :                 November 2002 - Present", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s : ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :\2Core Developers\2:", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Craig Edwards,          Brain,     <brain@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Craig McLure,           Craig,     <craig@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Robin Burchell,         w00t,      <w00t@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Oliver Lupton,          Om,        <om@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    John Brooks,            Special,   <special@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Pippijn van Steenhoven, pippijn,   <pippijn@one09.net>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Dennis Friis,           peavey,    <peavey@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Thomas Stagner,         aquanight, <aquanight@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Eric Dietz,             fez,       <fez@inspircd.org>", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s : ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :\2Regular Contributors\2:", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    satmd          nenolod         John2           Skip", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    jilles         Majic           eggy            Trystan", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    Quension       danieldg        praetorian", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :    GreenReaper    psychon         searchirc.com   irc-junkie.org", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s : ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :\2Other Contributors\2:", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :   dmb             Zaba            skenmy         LeaChim", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :   Thunderhacker   Dan             Jason          owine", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :   Adremelech      dz              BuildSmart     HiroP", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s :   ThaPrince       Bricker         AnMaster       djGrrr", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s : ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, " %s :\2Former Contributors\2:", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, " %s :   CC              jamie           typobox43      Burlex (win32)", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, " %s :   Stskeeps        ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, " %s : ", user->nick.c_str());
	user->WriteNumeric(RPL_INFO, "%s : Best experienced with: \2An IRC client\2", user->nick.c_str());
	FOREACH_MOD(I_OnInfo,OnInfo(user));
	user->WriteNumeric(RPL_ENDOFINFO, "%s :End of /INFO list", user->nick.c_str());
	return CMD_SUCCESS;
}
