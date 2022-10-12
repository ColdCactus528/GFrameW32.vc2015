// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Compile standard headers with level-3 warnings
#pragma warning (push, 3)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS

#define WINVER 0x0501

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commdlg.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <math.h>
#include <time.h>
#include <fstream>

#pragma warning(pop)

// Disable unnecessary warnings
#pragma warning(disable : 4699)     // creating precompiled header
#pragma warning(disable : 4200)     // Zero-length array item at end of structure, a VC-specific extension
#pragma warning(disable : 4514)     // unreferenced inline function has been removed
#pragma warning(disable : 4201)     // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4710)     // inline function not expanded
#pragma warning(disable : 4711)     // function selected for autmatic inlining
#pragma warning(disable : 4127)     // Conditional expression is constant
#pragma warning(disable : 4511)     // Copy constructor could not be generated
#pragma warning(disable : 4512)     // Assignment operator could not be generated
#pragma warning(disable : 4238)     // nonstandard extension used : class rvalue used as lvalue

// Define debug macros
#ifndef NDEBUG
    #define ASSERT(x)   _ASSERTE(x)
    #define VERIFY(x)   ASSERT(x)
#else
    #define ASSERT(x)
    #define VERIFY(x)   {x;}
#endif

#include "Tools.h"
#include "Array.h"
#include "Point.h"
#include "Rect.h"
#include "Vector.h"
#include "Matrix.h"
#include "Matrix2D.h"
#include "Box.h"
#include "RGBPixel.h"

