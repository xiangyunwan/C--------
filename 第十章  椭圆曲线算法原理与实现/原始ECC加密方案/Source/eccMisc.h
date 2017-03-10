/* ==========================================================================

	ecc - Erik's Code Collection
	Copyright (C) 2003-2005 - Erik Dienske

	This file is part of ecc.

	ecc is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	ecc is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ecc; if not, write to the Free Software Foundation, Inc.,
	59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

===========================================================================*/

//---------------------------------------------------------------------------
#ifndef eccMiscH
#define eccMiscH

#include <vcl.h>
//===========================================================================

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

enum TEccSystemFolder
{
	sfDesktop, sfFavorites,
	sfPersonal, // sfPersonal refers to 'My Documents'.
	sfPrograms, sfQuickLaunch, sfRecent, sfSendTo, sfStartMenu, sfStartup,
	sfAppData,	// '...\Documents and Settings\<user>\Application Data'.
	sfTemp
};
//---------------------------------------------------------------------------

String GetSystemFolder(const TEccSystemFolder sf);
/*	Returns the path of TEccSystemFolder, including the trailing backslash.
	Returns an empty string on failure. */

String AppPath()
{	return ExtractFilePath(Application->ExeName); }
/*	Includes trailing backslash or colon. */

String AppName()
{	return Application->Title; }

int GetHourGlassCount();
/*	Get the value of EccHourGlassCount, see .cpp file. */

void ResetHourGlass();
/*	Sets EccHourGlassCount to 0,
	and sets Screen->Cursor to crDefault. */

void HourGlass(const bool state);
/* 	Sets Screen->Cursor to [state].
	Calls to HourGlass() are cumulative,
	for every HourGlass(true) there must be a corresponding HourGlass(false).
	Note: The number of calls or stored in EccHourGlassCount, see .cpp file. */

bool ConfirmBox(const String msg, const String caption = AppPath());

String GetSystemUserName();
/*	Returns name of the system's current user.
	Returns "unknown_user" if an error occured. */

void InitRandomizer();
/* 	Initializes the random generator with a new start-value.
	Use it before calling random(n). */

bool DriveIsRemovable(String drive);
/* 	Returns true if (the rootdir of) [drive] is a removable drive. */

bool GetDriveFreeSpace(String dir, String &freeBytesStr, __int64 &freeBytes);
/*	[space_str] and [space] retrieve the free space of directory [dir].
	Returns false on failure. */

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
