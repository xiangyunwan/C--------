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

#include "eccFile.h"
#include "eccString.h"
//#include "eccError.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <Registry.hpp>	// CreateFileAssociation()
#include "shlobj.h"		// SHChangeNotify()
/*	#include "shlobj.h" Requires conditional define NO_WIN32_LEAN_AND_MEAN */
#include <fstream>
#include <StrUtils.hpp>	// AnsiReplaceStr
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

bool MakeFileList(String path, TStringList* list, const bool include_path)
{
	HANDLE fhandle;
	String fitem;
	WIN32_FIND_DATA filedata;
	bool done = false;

	// Get the first item:
	path = IncludeTrailingBackslash(path);
	fhandle = FindFirstFile( String(path + "*").c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return false;

	while (!done)
	{
		fitem = String(filedata.cFileName);
		// Test if the filedata-object is a directory or a file:
		if (!(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// Filedata-object is a file:
			if (include_path)
				list->Append(path + fitem);
			else
				list->Append(fitem);
		}
		// Is this the last Filedata-object?
		if (!FindNextFile(fhandle, &filedata))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				done = true;
		}
	}

	FindClose(fhandle);
	return true;
}
//---------------------------------------------------------------------------

bool MakeDirList(String path, TStringList* list)
{
	HANDLE fhandle;
	String fitem;
	WIN32_FIND_DATA filedata;
	bool done = false;

	// Get the first item:
	path = IncludeTrailingBackslash(path);
	fhandle = FindFirstFile( String(path + "*").c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return false;

	while (!done)
	{
		fitem = String(filedata.cFileName);
		// Test if the filedata-object is a directory or a file:
		if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Filedata-object is a directory:
			if ((fitem != ".") && (fitem != ".."))
			{
				list->Append(path + fitem);
			}
		}
		// Is this the last Filedata-object?
		if (!FindNextFile(fhandle, &filedata))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				done = true;
		}
	}
	FindClose(fhandle);
	return true;
}
//---------------------------------------------------------------------------

bool MakeRelativeDirList(String path, TStringList* list)
{
	HANDLE fhandle;
	String fitem;
	WIN32_FIND_DATA filedata;
	bool done = false;

	// Get the first item:
	path = IncludeTrailingBackslash(path);
	fhandle = FindFirstFile( String(path + "*").c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return false;

	while (!done)
	{
		fitem = String(filedata.cFileName);
		// Test if the filedata-object is a directory or a file:
		if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Filedata-object is a directory:
			if ((fitem != ".") && (fitem != ".."))
			{
				list->Append(fitem);
			}
		}
		// Is this the last Filedata-object?
		if (!FindNextFile(fhandle, &filedata))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				done = true;
		}
	}

	FindClose(fhandle);
	return true;
}
//---------------------------------------------------------------------------

bool CopyDirectory(String src, String dest, const bool overwrite, const bool recurse)
{
	if (!ForceDirectories(dest))
		return false;

	HANDLE fhandle;
	String fitem;
	WIN32_FIND_DATA filedata;
	bool done = false;

	// Get the first item:
	src 	= IncludeTrailingBackslash(src);
	dest 	= IncludeTrailingBackslash(dest);
	fhandle = FindFirstFile( String(src + "*").c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return false;

	while (!done)
	{
		fitem = String(filedata.cFileName);
		// Test if the filedata-object is a directory or a file:
		if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Filedata-object is a directory:
			if (recurse)
			{
				if (CreateDirectory(String(dest + fitem).c_str(), NULL) == 0)
					return false;
				if (!CopyDirectory(src + fitem, dest + fitem, overwrite, recurse))
					return false;
			}
		}
		else
		{
			// Filedata-object is a file:
			if ( !(FileExists(dest + fitem) && !overwrite) )
			{
				if (CopyFile(
					String(src + fitem).c_str(),
					String(dest + fitem).c_str(),
					FALSE	// No need to fail if dest-file exists, covered by [overwrite].
					) == 0) return false;
			}
		}
		// Is this the last Filedata-object?
		if (!FindNextFile(fhandle, &filedata))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				done = true;
		}
	}

	FindClose(fhandle);
	return true;
}
//---------------------------------------------------------------------------

bool StringToFile(const String str, const String fname)
{
	bool error = false;
	TStringList *sl_text = new TStringList();

	try
	{
		sl_text->Text = str;
		sl_text->SaveToFile(fname);
	}
	catch(Exception &err)
	{
		ShowMessage("Could not write to file:\n'" + fname + "'\n\nError:\n" + err.Message);
		error = true;
	}

	delete sl_text;

	return error;
}
//---------------------------------------------------------------------------

String FileToString(const String fname)
{
	if (!FileExists(fname)) return "";

	String str;
	TStringList *sl_text = new TStringList();
	try
	{
		sl_text->LoadFromFile(fname);
		str = sl_text->Text;
	}
	__finally
	{
		delete sl_text;
	}
	return str;
}
//---------------------------------------------------------------------------

String ExtractFileNameNoExt(String fname)
{
	fname = ExtractFileName(fname);
	int ext_pos = fname.Pos(ExtractFileExt(fname)) -1;
	if (ext_pos > 0) fname = fname.SubString(1,  ext_pos);
	return fname;
}
//---------------------------------------------------------------------------

String ReplaceIllegalFileNameChars(String fname, const String replace)
{
	String ill_chars = "\\/:*?\"<>|";
	if (replace.LastDelimiter(ill_chars))
        throw Exception(__FUNC__ ": [replace] cannot contain illegal chars.");

	int pos;
	while (1)
	{
		pos = fname.LastDelimiter(ill_chars);
		if (!pos) break;
		if (replace.IsEmpty()) fname.Delete(pos, 1);
		else fname = AnsiReplaceStr(fname, fname[pos], replace);
	}
	return fname;
}
//---------------------------------------------------------------------------

long GetFileSize(const String fname)
{
	HANDLE fhandle;
	WIN32_FIND_DATA filedata;

	fhandle = FindFirstFile(fname.c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return -1;

	long size = (filedata.nFileSizeHigh * MAXDWORD) + filedata.nFileSizeLow;

	FindClose(fhandle);
	return size;
}
//---------------------------------------------------------------------------

String FloatToHumanFileSize(float fsize)
{
	if (fsize < 0) return "?";

	if (fsize < 10000)
		return FloatToStr(fsize) + " B";

	fsize = fsize/1024;
	if (fsize < 10000)
		return (FloatToStrF(fsize, ffFixed, 7, 0) + " kB");

	fsize = fsize/1024;
	if (fsize < 10000)
		return (FloatToStrF(fsize, ffFixed, 7, 1) + " MB");

	fsize = fsize/1024;
	return (FloatToStrF(fsize, ffFixed, 7, 1) + " GB");
}
//---------------------------------------------------------------------------

String FileAttributeStr(const String fname)
{
	HANDLE fhandle;
	WIN32_FIND_DATA filedata;

	fhandle = FindFirstFile(fname.c_str(), &filedata);
	if (fhandle == INVALID_HANDLE_VALUE)
		return "";
	if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		return "";

	String str;
	str += (filedata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)	? "A" : "";
	str += (filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)	? "H" : "";
	str += (filedata.dwFileAttributes & FILE_ATTRIBUTE_READONLY)? "R" : "";
	str += (filedata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)	? "S" : "";

	FindClose(fhandle);
	return str;
}
//---------------------------------------------------------------------------

void AddDoubleZeroToString(String &str)
{
	str.SetLength(str.Length() + 1);
	str[str.Length()] = '\0';
	str.SetLength(str.Length() + 1);
	str[str.Length()] = '\0';
}
//---------------------------------------------------------------------------

bool FileToRecycleBin(String fpath)
{
	AddDoubleZeroToString(fpath);

	SHFILEOPSTRUCT op;
	ZeroMemory(&op,sizeof(op));
	//op.hwnd 	= Handle;
	op.hwnd 	= 0;
	op.wFunc 	= FO_DELETE;
	op.pFrom 	= fpath.c_str();
	op.fFlags 	= FOF_ALLOWUNDO;
	return !SHFileOperation(&op);
}
//---------------------------------------------------------------------------

bool MoveFile(String src_fpath, String dest_fpath)
{
	AddDoubleZeroToString(src_fpath);
	AddDoubleZeroToString(dest_fpath);

	SHFILEOPSTRUCT op;
	ZeroMemory(&op,sizeof(op));
	//op.hwnd 	= Handle;
	op.hwnd   	= 0;
	op.wFunc 	= FO_MOVE;
	op.pFrom 	= src_fpath.c_str();
	op.pTo 		= dest_fpath.c_str();
	op.fFlags	= FOF_RENAMEONCOLLISION + FOF_NOCONFIRMATION;
	return !SHFileOperation(&op);
}
//---------------------------------------------------------------------------

TDateTime FILETIMEToTDateTime(const FILETIME ftime)
{
	// Create a union/struct for DosDateTime:
	union
	{
		struct
		{
			WORD time;
			WORD date;
		};
		unsigned int fat_dt;
	} u_date;

	// Convert Coordinated Universal Time (UTC) to a local file time:
	FILETIME local_ftime;
	TDateTime fdatetime;
	try
	{
		FileTimeToLocalFileTime(
			&ftime,			// lpFileTime, pointer to UTC file time to convert
			&local_ftime);	// lpLocalFileTime, pointer to converted file time
		// Convert to DosDateTime so BCB can deal with it:
		FileTimeToDosDateTime(
			&local_ftime,	// lpFileTime, pointer to 64-bit file time
			&u_date.date, 	// lpFatDate, pointer to variable for MS-DOS date
			&u_date.time);	// lpFatTime, pointer to variable for MS-DOS time
	}
	catch (...)
	{
		return TDateTime(1911, 11, 11);
	}

	// Convert Windows time as a BCB TDateTime:
	try
	{
		fdatetime = FileDateToDateTime(u_date.fat_dt);
	}
	catch (...)
	{
		fdatetime = TDateTime(1911, 11, 11);
	}
	return fdatetime;
}
//---------------------------------------------------------------------------

bool CreateFileAssociation(String ext, String fileTypeStr,
		String appName, String appPath, bool clickStart)
{
	TRegistry *reg = new TRegistry();
	bool ok = true;

	try
	{
		reg->RootKey = HKEY_CLASSES_ROOT;
		if (reg->OpenKey(ext, true))
		{
			reg->WriteString("", appName + ext);
			reg->CloseKey();
		}
		if (reg->OpenKey(appName + ext, true))
		{
			reg->WriteString("", fileTypeStr);
			reg->CloseKey();
		}
		if (clickStart)
		{
			// Clicking opens app_path:
			if (reg->OpenKey(appName + ext + "\\shell\\open\\command", true))
			{
				reg->WriteString("", "\"" + appPath + "\" \"%1\"");
				reg->CloseKey();
			}
			// Give this file app_name's icon:
			if (reg->OpenKey(appName + ext + "\\DefaultIcon", true))
			{
				reg->WriteString("", appPath + ",0");
				reg->CloseKey();
			}
		}
		else
		{
			// Give this file no icon:
			if (reg->OpenKey(appName + ext + "\\DefaultIcon", true))
			{
				//reg->WriteString("", appPath + ",0"); // app's icon
				reg->WriteString("", "");
				reg->CloseKey();
			}
		}

/*0.3.13: 		if (click_start)
		{
			// Clicking opens app_path:
			if (reg->OpenKey(app_name + ext + "\\shell\\open\\command", true))
			{
				reg->WriteString("", "\"" + app_path + "\" \"%1\"");
				reg->CloseKey();
			}
		}
		else
		{
			// Give this file app_name's icon:
			if (reg->OpenKey(app_name + ext + "\\DefaultIcon", true))
			{
				reg->WriteString("", app_path + ",0");
				reg->CloseKey();
			}
		} */

		// Inform the shell that file associations have changed:
		SHChangeNotify(
			SHCNE_ASSOCCHANGED,	// LONG wEventId
			SHCNF_IDLIST,		// UINT uFlags
			NULL, 				// LPCVOID dwItem1
			NULL				// LPCVOID dwItem2
		);
	}
	catch(...)
	{
		ok = false;
	}
	delete reg;
	return ok;
}
//---------------------------------------------------------------------------

void AppendStrToFile(String str, String fpath, bool datetime, bool endline)
{
	if (fpath.IsEmpty()) return;

	std::ofstream log(fpath.c_str(), std::ios::out|std::ios::app); // Append, and create if necessary.
	if (!log) return;

	if (datetime)
		log	<< "[" << Now().DateTimeString().c_str() << "] ";
	log	<< str.c_str();
	if (endline)
		log << std::endl;
}
//---------------------------------------------------------------------------

String ActualPathName(const String filedir_path)
{
	// FindFirstFile() doesn't like trailing backslashes for directories:
	String path = ExcludeTrailingBackslash(filedir_path);
	String base_path = ExtractFilePath(path);

	HANDLE fhandle;
	WIN32_FIND_DATA filedata;
	fhandle = FindFirstFile( String(path).c_str(), &filedata);
	FindClose(fhandle);
	if (fhandle != INVALID_HANDLE_VALUE)
	{
		// Return 'real' charactercase for last part of [filedir_path]:
		return base_path + String(filedata.cFileName);	// 'Real' charactercase.
	}
	else
	{
		// [filedir_path] is invalid, return it unchanged:
		return filedir_path;
	}
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================

