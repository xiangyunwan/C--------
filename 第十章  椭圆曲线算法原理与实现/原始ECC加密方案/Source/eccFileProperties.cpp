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

#include "eccFileProperties.h"
#include "eccFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

TFileProperties::TFileProperties()
{
	FInitialized = false;
}
//---------------------------------------------------------------------------

TFileProperties::TFileProperties(String fname)
{
	GetFileProps(fname);
}
//---------------------------------------------------------------------------

bool TFileProperties::GetFileProps(const String fname)
{
	FName = fname;
	FError = false;
	FErrorMessage = "";
	FInitialized = false;

	// Get file handle:
	WIN32_FIND_DATA fi;
	ZeroMemory(&fi, sizeof(fi));
	FHandle = FindFirstFile(fname.c_str(), &fi);
	if (FHandle == INVALID_HANDLE_VALUE)
	{
		FError = true;
		FErrorMessage = __FILE__ ": CreateFile() failed. Line: " + IntToStr(__LINE__) + ". Func: " __FUNC__;
		FInitialized = false;
		return false;
	}

	// Set file attributes:
	SetFileAttributes(fi);

	// Get file size:
	SetFileSize(fi);

	// Get file date/time:
	SetFileDateTime(fi);

	// Close file handle:
	FindClose(FHandle);
	FHandle = NULL;

	FInitialized = true;
	return true;
}
//---------------------------------------------------------------------------

String TFileProperties::GetAttributeStr() const
{
	String str;
	str += FArchive		? "A" : "";
	str += FCompressed	? "C" : "";
	str += FHidden		? "H" : "";
	str += FReadOnly	? "R" : "";
	str += FSystem	 	? "S" : "";
	str += FTemporary 	? "T" : "";
	return str;
}
//---------------------------------------------------------------------------

void TFileProperties::SetFileAttributes(const WIN32_FIND_DATA &fi)
{
	FArchive 	= fi.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
	FCompressed	= fi.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED;
	FHidden 	= fi.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
	FReadOnly	= fi.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
	FSystem 	= fi.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM;
	FTemporary 	= fi.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY;
	FDir 		= fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

	FdwFileAttributes = fi.dwFileAttributes;
}
//---------------------------------------------------------------------------

void TFileProperties::SetFileSize(const WIN32_FIND_DATA &fi)
{
	// Helper union:
	union
	{
		struct
		{
			unsigned int lo;
			unsigned int hi;
		};
		unsigned long size;
	} u_size;

	u_size.hi = fi.nFileSizeHigh;
	u_size.lo = fi.nFileSizeLow;
	
	FSize = u_size.size;
}
//---------------------------------------------------------------------------

void TFileProperties::SetFileDateTime(const WIN32_FIND_DATA &fi)
{
	// Get file times:
	FCreationTime 	= FILETIMEToTDateTime(fi.ftCreationTime);
	FLastAccessTime	= FILETIMEToTDateTime(fi.ftLastAccessTime);
	FLastWriteTime	= FILETIMEToTDateTime(fi.ftLastWriteTime);
}
//---------------------------------------------------------------------------

String TFileProperties::GetHumanSize()
{
	return FloatToHumanFileSize(FSize);
}
//---------------------------------------------------------------------------

String TFileProperties::GetCreationTimeStr() const
{
	return FCreationTime.DateTimeString();
}
//---------------------------------------------------------------------------

String TFileProperties::GetLastAccessTimeStr() const
{
	return FLastAccessTime.DateTimeString();
}
//---------------------------------------------------------------------------

String TFileProperties::GetLastWriteTimeStr() const
{
	return FLastWriteTime.DateTimeString();
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================