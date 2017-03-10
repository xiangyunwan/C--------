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

#include "eccTimeLapse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

TTimeLapse::TTimeLapse()
{
	FdtStart = NULL; // Not initialized.
	FRunning = false;
}
//---------------------------------------------------------------------------

void TTimeLapse::Start()
{
	FdtStart = Now();
	FRunning = true;
}
//---------------------------------------------------------------------------

void TTimeLapse::Stop()
{
	FdtEnd = Now();
	FdtElapsed = FdtEnd - FdtStart;
	FRunning = false;
}
//---------------------------------------------------------------------------

void TTimeLapse::Pause()
{
	FdtPausedAt = Now();
	FRunning = false;
}
//---------------------------------------------------------------------------

void TTimeLapse::Resume()
{
	FdtStart = FdtStart + (Now() - FdtPausedAt);
	FRunning = true;
}
//---------------------------------------------------------------------------

TDateTime TTimeLapse::Elapsed()
{
	if (FRunning)
		return Now() - FdtStart;
	else
		return FdtEnd - FdtStart;
}
//---------------------------------------------------------------------------

String TTimeLapse::AsString(const bool ms, TDateTime dt)
{
	if ((int)dt == 0)
        dt = Elapsed();

	dt.DecodeTime(&FHour, &FMin, &FSec, &FMSec);

	String str;
	if (FHour != 0)
		str = IntToStr(FHour) + " Hours, ";
	str += dt.FormatString("n' Minutes, 's");
	if (ms)
		str +=  dt.FormatString("'.'z");
	str += " Seconds";
	return str;
}
//---------------------------------------------------------------------------

String TTimeLapse::AsStr(TDateTime dt)
{
	if ((int)dt == 0) dt = Elapsed();

	dt.DecodeTime(&FHour, &FMin, &FSec, &FMSec);

	String str;
	if (FHour != 0)
		str = dt.FormatString("h':'nn':'ss");
	else
		str = dt.FormatString("n':'ss");
	return str;
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
