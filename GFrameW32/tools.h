//-----------------------------------------------------------------------------
// File: Tools.h
// Desc: Declaration and definition of miscellaneous functions.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <malloc.h>

//------------------------------------------------------------------------------
// Name: ARRAYSIZE()
// Desc: Macro for calculating of array element count.
//------------------------------------------------------------------------------
#ifndef ARRAYSIZE
#define ARRAYSIZE(ar)     (sizeof(ar)/sizeof(ar[0]))
#endif

//------------------------------------------------------------------------------
// Name: strnzcpy()
// Desc: Copy characters from strSource to strDest while zero character not
//       encountered or character count-1 not reached. Then terminating zero 
//       character placed to strDest.
//------------------------------------------------------------------------------
inline
char* strnzcpy( char* strDest, const char* strSource, size_t count )
{
    char* start = strDest;
    while( --count && ( *strDest++ = *strSource++ ) != '\0' )
    {};
    *strDest = '\0';
    return start;
}

inline
wchar_t* strnzcpy( wchar_t* strDest, const wchar_t* strSource, size_t count )
{
    wchar_t* start = strDest;
    while( --count && ( *strDest++ = *strSource++ ) != L'\0' )
    {};
    *strDest = L'\0';
    return start;
}

//------------------------------------------------------------------------------
// Name: strtrim()
// Desc: Removes specified leading and trailing characters from a string.
//------------------------------------------------------------------------------
inline
char* strtrim( char* str, const char* trimChars )
{
    char* pd = str;
    const char* ps = str;
    char* pEnd = str;
    while( *ps != '\0' )
    {
        bool tc = (strchr(trimChars, *ps) != NULL);

        if( !tc )
            pEnd = pd + 1;

        if( !tc || pd != str )
        {
            *pd = *ps;
            ++pd;
        }
        ++ps;
    }
    *pEnd = '\0';
    return str;
}

inline
wchar_t* strtrim( wchar_t* str, const wchar_t* trimChars )
{
    wchar_t* pd = str;
    const wchar_t* ps = str;
    wchar_t* pEnd = str;
    while( *ps != L'\0' )
    {
        bool tc = (wcschr(trimChars, *ps) != NULL);

        if( !tc )
            pEnd = pd + 1;

        if( !tc || pd != str )
        {
            *pd = *ps;
            ++pd;
        }
        ++ps;
    }
    *pEnd = L'\0';
    return str;
}

//------------------------------------------------------------------------------
// Name: IsStrEmpty()
// Desc: Checks if passed string pointer is NULL or points to an empty string.
//------------------------------------------------------------------------------
inline
bool IsStrEmpty( const char* str )
{
    return (str == NULL || *str == '\0');
}

inline
bool IsStrEmpty( const wchar_t* str )
{
    return (str == NULL || *str == L'\0');
}

//------------------------------------------------------------------------------
// Name: StrToInt()
// Desc: Converts string to integer number.
//------------------------------------------------------------------------------
inline
int StringToInt( const char* str )
{
    return atoi(str);
}

inline
int StringToInt( const wchar_t* str )
{
    return _wtoi(str);
}

//------------------------------------------------------------------------------
// Name: StringToFloat()
// Desc: Converts string to floating point number.
//------------------------------------------------------------------------------
inline
double StringToFloat( const char* str )
{
    return atof(str);
}

inline
double StringToFloat( const wchar_t* str )
{
    if( !IsStrEmpty(str) )
    {
        size_t len = wcslen(str) + 1;
        char* buf = (char*)_alloca(len * sizeof(char));
        wcstombs(buf, str, len);
        return atof(buf);
    }
    return 0.0;
}

//------------------------------------------------------------------------------
// Name: _ttof()
// Desc: Converts string to floating point number.
//------------------------------------------------------------------------------
#ifndef _UNICODE
#define _ttof   atof
#else
inline
double _wtof( const wchar_t* str )
{
    return StringToFloat(str);
}

#define _ttof   _wtof
#endif

//------------------------------------------------------------------------------
// Name: Swap()
// Desc: Swaps two values
//------------------------------------------------------------------------------
template <class T> inline
const void Swap( T& a, T& b )
{
    T tmp(a);
    a = b;
    b = tmp;
}

//------------------------------------------------------------------------------
// Name: Min()
// Desc: Returns minimal value of two.
//------------------------------------------------------------------------------
template <class T> inline
T Min( T a, T b )
{
    return ( a < b ) ? a : b;
}

//------------------------------------------------------------------------------
// Name: Max()
// Desc: Returns maximal value of two.
//------------------------------------------------------------------------------
template <class T> inline
T Max( T a, T b )
{
    return ( a > b ) ? a : b;
}

//------------------------------------------------------------------------------
// Name: Bound()
// Desc: Limits specified value to specified range
//------------------------------------------------------------------------------
template <class T> inline
T Bound( T val, T valMin, T valMax )
{
    return Max( valMin, Min(valMax, val) );
}

//------------------------------------------------------------------------------
// Name: Lerp()
// Desc: Performs linear interpolation of two values
//------------------------------------------------------------------------------
template <class VALTYPE, class INTERPOLATORTYPE> inline
const VALTYPE Lerp( const VALTYPE& start, const VALTYPE& end, INTERPOLATORTYPE t )
{
    return start + (end - start) * t;
}

//------------------------------------------------------------------------------
// Name: IntRound()
// Desc: Performs rounding of a floating point value no nearest integer
//------------------------------------------------------------------------------
inline
int IntRound( float val )
{
    return (int)(val + ((val >= 0.0f) ? 0.5f : -0.5f));
}

inline
int IntRound( double val )
{
    return (int)(val + ((val >= 0.0) ? 0.5 : -0.5));
}

//------------------------------------------------------------------------------
// Name: Abs()
// Desc: Returns absolute of the specified value
//------------------------------------------------------------------------------
inline
int Abs( int val )
{
    return abs(val);
}

inline
float Abs( float val )
{
    return (float)fabs(val);
}

inline
double Abs( double val )
{
    return fabs(val);
}

#endif // TOOLS_H
