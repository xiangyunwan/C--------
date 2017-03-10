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
#ifndef eccH
#define eccH
//---------------------------------------------------------------------------
/*
	What is 'ecc.h'?

	Include this file to make ALL of ecc's classes and functions
	available to your project. Use namespace ecc when necessary.
	
	Requires 'ecc.lib' to be added to your project.
	
*/
//---------------------------------------------------------------------------

#include <vcl.h>

/* ecc Functions: */
#include "eccRelease.h"
#include "eccError.h"
#include "eccFile.h"
#include "eccMisc.h"
#include "eccString.h"

/* ecc Classes: */
#include "eccBrowseForDirectory.h"
#include "eccCheckStringList.h"
#include "eccFileProperties.h"
#include "eccFileVersionInfo.h"
#include "eccFormMemory.h"
#include "eccPropertyHandler.h"
#include "eccRecentFiles.h"
#include "eccShortcutFile.h"
#include "eccTimeLapse.h"
//---------------------------------------------------------------------------
#endif

