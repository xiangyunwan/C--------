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
#ifndef eccShortcutFileH
#define eccShortcutFileH
//---------------------------------------------------------------------------

/*	Example:
	{
		TShortcutFile sf;
		sf.Name 		= "My Application";
		sf.Path 		= GetSystemFolder(sfDesktop); // In EccMisc.h
		sf.Target 		= Application->ExeName;
		sf.Description 	= "This application is supposed to do...";
		sf.Arguments 	= "-arg";
		sf.WorkingDir 	= ExtractFileDir(Application->ExeName);
		sf.Create();
	}
*/
//===========================================================================
namespace ecc {
//===========================================================================

class TShortcutFile
{
public:
	String Name;			// Name of the shortcut (without '.lnk')
	String Path;			// Location of the shortcut
	String Target;			// Location where the shortcut points to
	String Description;
	String Arguments;		// Commandline arguments
	String WorkingDir;
//	TWindowState WindowState;	// Does not work!?

	bool Create();

private:
//	int MSWinState();

};

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
