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
#ifndef eccFileH
#define eccFileH
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

bool MakeFileList(String path, TStringList* list, const bool include_path = true);
/*	Adds absolute paths of all files in [path] to [list].
	Or, if [include_path] is false: adds relative paths of all files in [path] to [list].
	Returns false if an error occured. */

bool MakeDirList(String path, TStringList* list);
/*	Adds absolute paths of all directories in [path] to [list].
	Returns false if an error occured. */

bool MakeRelativeDirList(String path, TStringList* list);
/*	Adds relative paths of all directories in [path] to [list].
	Returns false if an error occured. */

bool CopyDirectory(String src, String dest,
		 const bool overwrite = false, const bool recurse = false);
/*	Copies files in directory [src] to directory [dest].
	If [recurse] is true, all subdirectories and their contents of [src] are copied as well.
	This function aborts on the first error that occurs.
	Returns false if an error occured. */

bool StringToFile(const String str, const String fname);

String FileToString(const String fname);
/*	Returns the contents of file [fname] as a string. */

String ExtractFileNameNoExt(String fname);
/* 	Returns a filename without path and extension. */

String ReplaceIllegalFileNameChars(String fname, const String replace = "");
/* 	See source. */

long GetFileSize(const String fname);
/*	Returns -1 if size could not be read. */

String FloatToHumanFileSize(float fsize);
/* 	Returns human readable filesize. In bytes, kB, MB or GB.
	Returns "?" if size<0. */

String FileAttributeStr(const String fname);
/* 	Returns a string like: "AH".
	Returns an empty string if fname is a directory. */

void AddDoubleZeroToString(String &str);

bool FileToRecycleBin(String fpath);
/* 	Returns false on failure. */

bool MoveFile(String src_fname, String dest_fname);
/* 	Returns false on failure. */

TDateTime FILETIMEToTDateTime(const FILETIME ftime);
/*	Converts a windows FILETIME structure to a Borland TDateTime class.
	Drawback: TDateTime (==DosDateTime) is less accurate than FILETIME.
	On error: return value is TDateTime(1911, 11, 11). */

bool CreateFileAssociation(String ext, String fileTypeStr,
		String appName, String appPath, bool clickStart);
/*	Creates a file association for files with [ext] as extension.
	Returns false if an error occured.
	[ext]           Should include the dot.
	[fileTypeStr]	String that describes the file type.
	[appName]       Name of the application. Will be stored in the registry.
	[appPath]       Full path to the application.
	[clickStart]    If true, clicking file.ext will open appPath. */

void AppendStrToFile(String str, String fpath,
		bool datetime, bool endline = true);
/*  Appends [str] to file [fpath].
	If [datetime], [str] is prefixed with a date/time string like "[14-04-2004 10:19:33]".
	If [endline], [str] is followed by std::endl.
	Useful for adding strings to a log file. */

String ActualPathName(const String filedir_path);
/* 	Returns the file- or directoryname of [filedir_path] with
	the same charactercase as on disk, without trailing backslashes.
	- Note: Only the last part of [filedir_path] is modified.
	Returns an unchanged [filedir_path] if [filedir_path] is not
	a valid file or directory. */

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
