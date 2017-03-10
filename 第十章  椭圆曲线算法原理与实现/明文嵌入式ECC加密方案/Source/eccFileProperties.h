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
#ifndef eccFilePropertiesH
#define eccFilePropertiesH
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------
/* TODO : This class does not handle directories, yet */

/* 	TFileProperties provides easy access to attributes, size and dates of a file.

	Example:
	{
	TFileProperties fp("c:\\file.txt");
	if (fp.HasError()) return;
	ShowMessage(
	"a:" + IntToStr(fp.IsArchive()) +
	" h:" + IntToStr(fp.IsHidden()) +
	" r:" + IntToStr(fp.IsReadOnly()) +
	" s:" + IntToStr(fp.IsSystem())
	);
	ShowMessage( fp.GetLastWriteTime().DateTimeString()	);

	}
*/
//---------------------------------------------------------------------------

class TFileProperties {

private:
	bool FInitialized;	// Is false if class properties contain no valid data.
	bool FError;
	String FErrorMessage;

	String FName;
	HANDLE FHandle;

	bool FArchive;
	bool FCompressed;
	bool FHidden;
	bool FReadOnly;
	bool FSystem;
	bool FTemporary;
	bool FDir;
	DWORD FdwFileAttributes;

	unsigned long FSize;

	TDateTime FCreationTime;    // Created
	TDateTime FLastAccessTime;  // Accessed
	TDateTime FLastWriteTime;	// Modified

	void SetFileAttributes(const WIN32_FIND_DATA &fi);
	void SetFileSize(const WIN32_FIND_DATA &fi);
	void SetFileDateTime(const WIN32_FIND_DATA &fi);

public:
	TFileProperties();
	TFileProperties(String fname);
	bool GetFileProps(String fname);

	bool HasError() 			{ return FError; }
	String GetErrorMessage() 	{ return FErrorMessage; }
	bool IsInitialized() 		{ return FInitialized; }

	String GetName() 			{ return FName; }
	HANDLE GetHandle() 			{ return FHandle; }

	bool IsArchive() 			{ return FArchive; }
	bool IsCompressed() 		{ return FCompressed; }
	bool IsHidden() 			{ return FHidden; }
	bool IsReadOnly() 			{ return FReadOnly; }
	bool IsSystem() 			{ return FSystem; }
	bool IsTemporary() 			{ return FTemporary; }
	String GetAttributeStr() const;
	DWORD GetdwFileAttributes()
	{	return FdwFileAttributes; }

	bool IsDir() { return FDir; }

	unsigned long GetSize() 	{ return FSize; }
	String GetHumanSize();

	TDateTime GetCreationTime() { return FCreationTime; }
	TDateTime GetLastAccessTime() { return FLastAccessTime; }
	TDateTime GetLastWriteTime() { return FLastWriteTime; }
	/* The ..Time() functions return NULL if the filedatetime could not be read. */

	String GetCreationTimeStr() const;
	String GetLastAccessTimeStr() const;
	String GetLastWriteTimeStr() const;
	/* The ..TimeStr() functions return "?" if the filedatetime could not be read. */
};
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
