/**********************************************************************
 *<
    FILE: DllEntry.cpp

    DESCRIPTION:Contains the Dll Entry stuff

    CREATED BY: 

    HISTORY: 

 *>    Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/
#include "StdAfx.h"

#include "N3DExp.h"

extern ClassDesc * GetCN3DExpDesc();

// This function is called by Windows when the DLL is loaded.  This
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) {
    g_hInst = hinstDLL; // Hang on to this DLL's instance handle.

    static BOOL bControlsInit = FALSE;
    if (!bControlsInit) {
        bControlsInit = TRUE;
        InitCustomControls(g_hInst); // Initialize MAX's custom controls
        InitCommonControls();        // Initialize Win95 controls
    }

    return (TRUE);
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
__declspec(dllexport) const TCHAR * LibDescription() {
    return GetString(IDS_LIBDESCRIPTION);
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
__declspec(dllexport) int LibNumberClasses() {
    return 1;
}

// This function returns the number of plug-in classes this DLL
__declspec(dllexport) ClassDesc * LibClassDesc(int i) {
    switch (i) {
    case 0:
        return GetCN3DExpDesc();
    default:
        return 0;
    }
}

// This function returns a pre-defined constant indicating the version of
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
__declspec(dllexport) ULONG LibVersion() {
    return VERSION_3DSMAX;
}

TCHAR * GetString(int id) {
    static TCHAR buf[256];

    if (g_hInst) {
        return LoadString(g_hInst, id, buf, sizeof(buf)) ? buf : NULL;
    }
    return NULL;
}
