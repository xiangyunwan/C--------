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
#ifndef eccFileVersionInfoH
#define eccFileVersionInfoH
//---------------------------------------------------------------------------

/*	Example:
	{
		TFileVersionInfo fvi(Application->ExeName);
		if (fvi.Failed()) ShowMessage(fvi.GetErrorMsg());
		String version = fvi.GetMajor() + "." + fvi.GetMinor() + "." + fvi.GetRelease();
		String copyright = fvi.GetValue(fviLegalCopyright);
	}
*/

//===========================================================================
namespace ecc {
//===========================================================================

static enum TEccFVIKeyId
{
	fviCompanyName = 0,	fviFileDescription,	fviFileVersion, fviInternalName,
	fviLegalCopyright, fviLegalTradeMarks,	fviOriginalFilename,
	fviProductName, fviProductVersion, fviComments
} EccFVIKeyId;

static const String TEccFVIKeyStrings[] = {
	"CompanyName", "FileDescription", "FileVersion", "InternalName",
	"LegalCopyright", "LegalTradeMarks", "OriginalFilename", "ProductName",
	"ProductVersion", "Comments"
};
//#define eccFVIKeyStringsCount sizeof(TEccFVIKeyStrings)/sizeof(TEccFVIKeyStrings[0])
static const int eccFVIKeyStringsCount = sizeof(TEccFVIKeyStrings)/sizeof(TEccFVIKeyStrings[0]);

class TFileVersionInfo
{
	private:
		String FFileName;
		String Values[eccFVIKeyStringsCount];

		void SplitVersionNumber();
		String FMajor, FMinor, FRelease, FBuild;

		bool FError;
		String FErrorMsg;

		void ResetValues();

	public:
		TFileVersionInfo();
		TFileVersionInfo(String fname);

		void ReadValues(String fname);

		String GetValue(TEccFVIKeyId fvi);

		String GetMajor();
		String GetMinor();
		String GetRelease();
		String GetBuild();
		String GetVersion();

		bool Failed() { return FError; }
		String GetErrorMsg() { return FErrorMsg; }
};

//===========================================================================
} // namespace ecc
//===========================================================================
#endif
