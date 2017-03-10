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
#include <vcl.h>
#pragma hdrstop

#include "eccMisc.h"
#include "Registry.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

#include "NewAPIs_Fixed.h"
/*	Including "NewAPIs_Fixed.h" prevents compatibility errors on older systems
	by wrapping some system-calls. */

//===========================================================================
namespace ecc {
//===========================================================================

String GetSystemFolder(const TEccSystemFolder sf)
{

	if (sf == sfTemp)
	{
		// The temporary folder is an environment variable:
		char buf[MAX_PATH];
		if (!GetEnvironmentVariable("TEMP", buf, MAX_PATH))
			return "";

		if (!GetLongPathName(buf, buf, MAX_PATH))
			return "";
		else
			return IncludeTrailingBackslash(String(buf));
	}

	String key_name;

	switch (sf)
	{
		case sfDesktop:
			key_name = "Desktop";
			break;
		case sfFavorites:
			key_name = "Favorites";
			break;
		case sfPersonal:
			key_name = "Personal";
			break;
		case sfPrograms:
			key_name = "Programs";
			break;
		case sfQuickLaunch:
			key_name = "Quick Launch";
			break;
		case sfRecent:
			key_name = "Recent";
			break;
		case sfStartMenu:
			key_name = "Start Menu";
			break;
		case sfStartup:
			key_name = "Startup";
			break;
		case sfAppData:
			key_name = "AppData";
			break;
		default:
			return "";
	}

	String key_str = "\\Software\\Microsoft\\Windows\\CurrentVersion\\";
	if (sf == sfQuickLaunch)
		key_str += "GrpConv\\MapGroups";
	else
		key_str += "Explorer\\Shell Folders";

	const String c_key_str = key_str;
	String folder;

	TRegistry *reg = new TRegistry();
	try
	{
        /*  Abakt0.9.5-beta2: changed OpenKey() to OpenKeyReadOnly()
            Fixes a bug mentioned by Michael (15-sep-2006). */
		if (reg->OpenKeyReadOnly(c_key_str))
		{
			folder = reg->ReadString(key_name);
			reg->CloseKey();
		}
	}
	__finally
	{
		delete reg;
	}

	return IncludeTrailingBackslash(folder);
}
//---------------------------------------------------------------------------

//===========================================================================

static int EccHourGlassCount;
/* 	Increases with every HourGlass(true);
	Decreases with every HourGlass(false);
	When EccHourGlassCount is not 0 the Screen->Cursor displays an hourglass. */
//---------------------------------------------------------------------------

int GetHourGlassCount()
{
	return EccHourGlassCount;
}
//---------------------------------------------------------------------------

void ResetHourGlass()
{
	EccHourGlassCount = 0;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void HourGlass(const bool state)
{
	if (state) ++EccHourGlassCount;
	else --EccHourGlassCount;
	Screen->Cursor = (EccHourGlassCount) ? crHourGlass : crDefault;
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------

//===========================================================================

bool ConfirmBox(const String msg, const String caption)
{
	return (
		Application->MessageBox( msg.c_str(), caption.c_str(), MB_YESNO )
		== IDYES );
}
//---------------------------------------------------------------------------

String GetSystemUserName()
{
	#define USERNAME_BUFSIZE 1024
	LPTSTR lpszSystemInfo;      			// pointer to system information string
	DWORD cchBuff = USERNAME_BUFSIZE - 1;   // size of computer or user name
	char tchBuffer[USERNAME_BUFSIZE];   	// buffer for string
	lpszSystemInfo = tchBuffer;

	// Get the user name:
	String uname = "unknown_user";
	if( GetUserName(lpszSystemInfo, &cchBuff) )
		uname = String(lpszSystemInfo);
	return uname;
}
//---------------------------------------------------------------------------

void InitRandomizer()
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(Now(), Hour, Min, Sec, MSec);
	srand((MSec+Hour)*(Sec+Min));
}
//---------------------------------------------------------------------------

bool DriveIsRemovable(String drive)
{
	drive = ExtractFileDrive(drive);
	unsigned int dtype = GetDriveType( drive.c_str() );

	return (dtype == DRIVE_REMOVABLE);
}
//---------------------------------------------------------------------------

/* 	See:
	http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/getdiskfreespaceex.asp
	(The code on that page contained some flaws on April 2004.)

    Note: This function can be replaced with Borland's DiskFree function. */

bool GetDriveFreeSpace(String dir, String &freeBytesStr, __int64 &freeBytes)
{
	__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
	bool fResult;
    dir = IncludeTrailingPathDelimiter(ExtractFileDrive(dir));

	fResult = GetDiskFreeSpaceEx(
			dir.c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes
		);

	freeBytes = i64FreeBytes;	/* TODO : Use i64FreeBytesToCaller? */
	freeBytesStr.printf("%I64d", freeBytes);

	// fResult is zero on failure.
	return !fResult;
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
