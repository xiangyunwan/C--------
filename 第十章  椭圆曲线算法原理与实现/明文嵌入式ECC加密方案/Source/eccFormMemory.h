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
#ifndef eccFormMemoryH
#define eccFormMemoryH
//---------------------------------------------------------------------------

#include <IniFiles.hpp>
//---------------------------------------------------------------------------

/*	Note that the inifile remains unprotected as
	long as a TFormMemory object exists!

	-Write Example:
	{
		TFormMemory mem("C:\\Program Files\\MyApp\\mem.ini", true);

		mem.SetComponent(f_Main);
		mem.WriteProps("Top;Left;");
		mem.WriteProps(bt_Exit, "Visible");

		mem.SetComponent(fr_Settings);
		mem.WriteProps(fr_Settings->cb_Beep, "Checked");

		// You can read AND write!
		TIniFile* ini = mem.GetIniFile();
		int apprun = ini->ReadInteger("General", "RunCount", 0);
		ini->WriteInteger("General", "RunCount", apprun + 1);
	}

	-Read Example:
	{
		mem.ReadProps(f_Main);
		mem.ReadProps(fr_Settings);
	}
*/
//===========================================================================
namespace ecc {
//===========================================================================

class TFormMemory
{
	private:
		void ProtectFile(bool protect);
		void Scramble(char* buf, int flen);
		void Unscramble(char* buf, int flen);
		void WriteIniInfo(int fh, char* buf, int flen);

		TComponent*	FSectionComponent;
		TIniFile* FIniFile;
		String FFilePath;
		bool FProtected;

		bool FWritePropsError;	// When true all further WriteProps will be ignored.

	public:
		TFormMemory(const String ini_fpath, const bool protect = false);
		/*	Creates a TFormMemory object that reads its data from
			and saves its data to ini_fpath.
			If ini_fpath does not exist it is created, including the directories. */

		~TFormMemory(void);
		/*	Destroys the TFormMemory object after saving its data to FFilePath. */

		TIniFile* GetIniFile();
		/* Returns NULL if (IniFile->FileName) does not exist. */

		void SetComponent(TComponent* sect_comp);

		void WriteProps(String props);
		void WriteProps(TComponent* comp, String props);
		void ReadProps(TComponent* sect_comp);
};

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
