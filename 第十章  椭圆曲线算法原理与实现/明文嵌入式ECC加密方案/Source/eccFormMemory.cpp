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

#include "eccFormMemory.h"
#include "eccPropertyHandler.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include "eccError.h"
#include "eccString.h"

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

TFormMemory::TFormMemory(const String ini_fpath, const bool protect)
{
	FProtected	= protect;
	FFilePath 	= ini_fpath;
	FWritePropsError = false;

	if (FProtected)
		ProtectFile(false);  // Unscramble

	if(! DirectoryExists(ExtractFilePath(ini_fpath)) )
		ForceDirectories(ExtractFilePath(ini_fpath));
	FIniFile = new TIniFile(ini_fpath);
}
//---------------------------------------------------------------------------

TFormMemory::~TFormMemory(void)
{
	try
	{
		FIniFile->UpdateFile(); // Flush file to disk.
		delete FIniFile;

		if (FProtected)
			ProtectFile(true);   // Scramble
	}
	catch(...)
	{
		ECC_ERR_MSG("Failed saving data to file\n" + QuoteStr(FFilePath) + ".");
	}
}
//---------------------------------------------------------------------------

TIniFile* TFormMemory::GetIniFile()
{
	return (TIniFile*) FIniFile;
}
//---------------------------------------------------------------------------

void TFormMemory::SetComponent(TComponent* sect_comp)
{
	FSectionComponent = sect_comp;
}
//---------------------------------------------------------------------------

void TFormMemory::WriteProps(String props)
{
	if (FWritePropsError) return;

	TPropertyHandler ph(FSectionComponent);
	TStringList* prop_list = new TStringList();

	try
	{
		prop_list->Text = ph.GetNamesValues(props);
		for (int i = 0; i < prop_list->Count; i++)
		{
			FIniFile->WriteString(
				ph.GetName(),
				prop_list->Names[i],
				prop_list->Values[prop_list->Names[i]]);
		}
	}
	catch(...)
	{
		ECC_ERR_MSG("Writing the settings failed.\n"
			"Further attempts to write to\n"
			+ QuoteStr(FFilePath) + "\nwill be ignored.");
		FWritePropsError = true;
	}
	delete prop_list;
}
//---------------------------------------------------------------------------

void TFormMemory::WriteProps(TComponent* comp, String props)
{
	TPropertyHandler ph(FSectionComponent);
	TStringList* prop_list = new TStringList();

	try
	{
		prop_list->Text = ph.GetNamesValues(comp, props);
		for (int i = 0; i < prop_list->Count; i++)
		{
			FIniFile->WriteString(
				ph.GetName(FSectionComponent),
				ph.GetName(comp) + "." + prop_list->Names[i],
				prop_list->Values[prop_list->Names[i]]);
		}
	}
	__finally
	{
		delete prop_list;
	}
}
//---------------------------------------------------------------------------

void TFormMemory::ReadProps(TComponent* sect_comp)
{
	TPropertyHandler ph(sect_comp);
	TStringList* prop_list = new TStringList();

	try
	{
		FIniFile->ReadSectionValues(ph.GetName(), prop_list);
		ph.SetNamesValues(prop_list->Text);
	}
	__finally
	{
		delete prop_list;
	}
}
//---------------------------------------------------------------------------

void TFormMemory::ProtectFile(bool protect)
//	(Un)Scrambles the ini file.
//	Note that this is a lousy way to scramble the file, it is NOT encrypted.
//	The file becomes just a bit unreadable...
{
	if (!FileExists(FFilePath)) return;

	FileSetAttr(FFilePath, 0);

	try
	{
		// Read file:
		int fh = FileOpen(FFilePath, fmOpenRead);
		int flen = FileSeek(fh,0,2);
		FileSeek(fh,0,0);
		char* buf = new char[flen+1];
		flen = FileRead(fh, buf, flen);
		FileClose(fh);

		// FileWrite() inserts...does not overwrite! So empty file by deleting it:
		DeleteFile(FFilePath);

		if (protect) Scramble(buf, flen);
		else Unscramble(buf, flen);

		// Create file:
		fh = FileCreate(FFilePath);
		FileClose(fh);

		// Write file:
		fh = FileOpen(FFilePath, fmOpenWrite);
		FileSeek(fh,0,0);
		FileWrite(fh, buf, flen);
		FileClose(fh);

		delete[] buf;
	}
	catch(...)
	{
		//"FILE ERROR! "__FUNC__;
	}

	if (protect) FileSetAttr(FFilePath, faHidden | faReadOnly);
}
//---------------------------------------------------------------------------

void TFormMemory::Scramble(char* buf, int flen)
{
	for (int i = 0; i < flen; i++)
	{
		buf[i] = buf[i] ^ 0xff;
		buf[i] = buf[i] ^ i;
	}
}
//---------------------------------------------------------------------------

void TFormMemory::Unscramble(char* buf, int flen)
{
	for (int i = 0; i < flen; i++)
	{
		buf[i] = buf[i] ^ i;
		buf[i] = buf[i] ^ 0xff;
	}
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================

