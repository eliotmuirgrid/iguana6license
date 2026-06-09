#ifndef __DRG_IGUANA_ID_H__
#define __DRG_IGUANA_ID_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGiguanaId
//
// Description:
//
// Contains a function, DRGiguanaId(), which returns an Iguana ID.
// An Iguana ID constructed from a the machine's Machine ID (RGN library),
// and Iguana's installation directory (the current working directory).
//
// Author: Kevin Senn
// Date:   Wednesday, October 22nd, 2008 @ 02:59:29 PM
//
//---------------------------------------------------------------------------

#include <COL/COLminimumInclude.h>

// Returns the Iguana Id.
COLstring DRGiguanaId();

// Returns an Iguana ID constructed from a specified MachineID and
// working directory.
// Note: WorkingDir should end with a path separator.
COLstring DRGiguanaIdImplementation(const COLstring& MachineId, const COLstring& WorkingDir);

// Input: An N-character hex string
// Output: An (N/2)-character alpha-numeric string
// Public only for unit testing purposes.
COLstring DRGconvertFromHexToIguanaId(const COLstring& HexString);

#endif // end of defensive include
