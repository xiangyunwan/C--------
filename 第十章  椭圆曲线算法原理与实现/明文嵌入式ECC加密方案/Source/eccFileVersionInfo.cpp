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

#include "eccFileVersionInfo.h"
#include "eccError.h"
#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <StrUtils.hpp>	// AnsiReplaceStr
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

TFileVersionInfo::TFileVersionInfo()
{
	ResetValues();
}
//---------------------------------------------------------------------------

TFileVersionInfo::TFileVersionInfo(String fname)
{
	ReadValues(fname);
}
//---------------------------------------------------------------------------

void TFileVersionInfo::ResetValues()
{
	FFileName = "";
	FMajor = FMinor = FRelease = FBuild = "-1";
	FError = false;
	FErrorMsg = "";

	for (int i = 0; i < eccFVIKeyStringsCount; i++)
	{
		Values[i] = "";
	}
}
//---------------------------------------------------------------------------

void TFileVersionInfo::ReadValues(String fname)
{
	ResetValues();

	DWORD zeroval;
	int size = GetFileVersionInfoSize(fname.c_str(), (LPDWORD)&zeroval);
	if (!size)
	{
		FError = true;
		FErrorMsg = "GetFileVersionInfoSize() failed. " + ECC_DEBUGINFO;
		return;
	}

	char* buf = new char[size+1];

	try
	{
		if (GetFileVersionInfo(fname.c_str(), 0, size, (LPVOID)buf) == 0)
			ECC_THROW("GetFileVersionInfo() failed");

		/* Get language/codepage: */
		struct TeccLangAndCP
		{
			WORD wLanguage;
			WORD wCodePage;
		} *PeccLangAndCP;
		UINT LangLen;

		if (VerQueryValue(buf, "\\VarFileInfo\\Translation", (LPVOID*)&PeccLangAndCP, &LangLen) == 0)
			ECC_THROW("VerQueryValue() failed while retrieving language/codepage");

		/* Get a value: */
		String sub_blk = Format("\\StringFileInfo\\%.4x%.4x\\",
			ARRAYOFCONST((PeccLangAndCP->wLanguage, PeccLangAndCP->wCodePage)));

		void* val;
		unsigned int len;

		for (int i = 0; i < eccFVIKeyStringsCount; i++)
		{
			if (VerQueryValue(buf, String(sub_blk + TEccFVIKeyStrings[i]).c_str(), &val, &len))
				Values[i] = String((char*)val, len).TrimRight();
				//ECC_THROW("VerQueryValue() failed while retrieving value");
			else
				Values[i] = "Error retrieving value of '" + TEccFVIKeyStrings[i] + "'";
		}

		/* Ok: */
		delete[] buf;
		SplitVersionNumber();
	}
	catch(Exception &ex)
	{
		FError = true;
		FErrorMsg = ex.Message;
		delete[] buf;
	}
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetValue(TEccFVIKeyId fvi)
{
	if (fvi >= eccFVIKeyStringsCount) return ("");
	return Values[fvi];
}
//---------------------------------------------------------------------------

void TFileVersionInfo::SplitVersionNumber()
{
	String version = Values[fviFileVersion];
	if (version.IsEmpty()) return;

	version = AnsiReplaceStr(version, ",", "."); // Seperator can be ',' or '.'.

	int pos1 = 1;
	int pos2 = version.Pos(".");
	if (!pos2) return;
	FMajor = version.SubString(pos1, pos2-pos1);
	version = version.Delete(pos1, (pos2-pos1 +1));

	pos2 = version.Pos(".");
	if (!pos2) return;
	FMinor = version.SubString(pos1, pos2-pos1);
	version = version.Delete(pos1, (pos2-pos1 +1));

	pos2 = version.Pos(".");
	if (!pos2) return;
	FRelease = version.SubString(pos1, pos2-pos1);
	version = version.Delete(pos1, (pos2-pos1 +1));

	FBuild = version;
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetMajor()
{
	return FMajor;
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetMinor()
{
	return FMinor;
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetRelease()
{
	return FRelease;
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetBuild()
{
	return FBuild;
}
//---------------------------------------------------------------------------

String TFileVersionInfo::GetVersion()
{
	return FMajor + "." + FMinor + "." + FRelease;
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================

