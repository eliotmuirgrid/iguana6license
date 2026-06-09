#ifndef __COL_MINIMUM_INCLUDE_H__
#define __COL_MINIMUM_INCLUDE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLminimumInclude.h 
//
// Author: Eliot Muir
//
// Description:
//
// Minimal include file used by exported DLL interface classes
//
//
//---------------------------------------------------------------------------

class COLostream;

#include <string.h>

#if defined(__BORLANDC__)  || defined (CHM_UNIX)
   #define COLputenv putenv
#else
   #define strdup   _strdup
   #define COLputenv _putenv
#endif

#if defined(WIN32)
   #define strcasecmp _stricmp
   #define access   _access
#endif

#define COL_CHAR_SIZE sizeof(char)

#ifdef WIN32
// This little warning gets rid of all those *&^*&^ warnings relating to the use of supposedly
// unsafe API functions.  It makes life miserable driving a car (compiler) that you cannot see through the windscreen because of
// the snow (warnings) - Eliot
#pragma warning(disable: 4996)
// Don't just warn if you don't know how to call a destructor.
#pragma warning (error : 4150)
// to disable the "unreferenced local variable" informational warning in VC++.
#pragma warning (disable : 4101)
#ifndef NDEBUG
#  define COL_ENABLE_CONDITIONAL_LOGGING 1
#endif
#endif

#if defined(_WIN32) || defined(WIN32)
#   ifndef CHM_WIN
#      define CHM_WIN
#   endif
#endif

#ifdef _WIN64
#   ifndef CHM_WIN
#      define CHM_WIN
#   endif
#   ifndef CHM_64
#      define CHM_64
#   endif
#endif

typedef double DATE;

#include <stddef.h>

// I think we have to use size_t here for a bit - maybe if we got COLstring changed
// over to use COLindex as well and had more extensive unit tests in place we could change
// this over - Eliot
#ifndef __NPOS_DEFINED__
const size_t npos = 0xFFFFFFFF;
#define __NPOS_DEFINED__
#endif

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the COL_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// COL_DLL_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
// For more information look up __declspec(dllexport) and __declspec(dllimport) in MSDN. 
// This is a very standard 'trick' when setting up C++ libraries as DLLs in Windows.
#ifdef COL_DLL_EXPORTS
#   ifdef __BORLANDC__
#      define COL_DLL_FUNC(X) X _export
#      define COL_DLL __declspec(dllexport) 
#   else  // __BORLANDC__
#      define COL_DLL __declspec(dllexport) 
#      define COL_DLL_FUNC(X) COL_DLL X
#   endif  // __BORLANDC__
#else
#ifdef COL_DLL_IMPORTS
#   ifdef __BORLANDC__
#      define COL_DLL __declspec(dllimport) 
#      define COL_DLL_FUNC(X)  X __import
#   else  // __BORLANDC__
#      define COL_DLL __declspec(dllimport) 
#      define COL_DLL_FUNC(X)  COL_DLL X
#   endif  // __BORLANDC__
#else
#   define COL_DLL
#   define COL_DLL_FUNC(X)  X
#endif
#endif

#ifndef NULL
#   define NULL 0
#endif

#ifdef WIN32
#   define COL_STD_CALL __stdcall
#else
#   define COL_STD_CALL
#endif

#define COLint8 char
#define COLint16 short
#define COLint32 int
#ifdef CHM_UNIX
#   define COLint64 long long
#else
#   define COLint64 __int64
#endif
#define COLuint8 unsigned COLint8
#define COLuint16 unsigned COLint16
#define COLuint32 unsigned COLint32

#define COLuint64 unsigned COLint64

#define COLfloat32 float
#define COLfloat64 double

#define COLindex size_t

#define COL_HI32(l)   ((COLuint16) (((COLuint32) (l) >> 16) & 0xFFFF))
#define COL_LO32(l)   ((COLuint16) (l))
#define COL_MAKE32(a, b) ((COLuint32) (((COLuint16) (a)) | ((COLuint32) ((COLuint16) (b))) << 16))

#ifdef CHM_UNIX
#   define COL_UINT_64_MAX 0xFFFFFFFFFFFFFFFF##ULL    // 18446744073709551615
#else
#   define COL_UINT_64_MAX 0xFFFFFFFFFFFFFFFF         // 18446744073709551615
#endif

#define COL_UINT_32_MAX 0xFFFFFFFF        // 4294967295
#define COL_UINT_16_MAX 0xFFFF            // 65535
#define COL_UINT_8_MAX  0xFF              // 255

#ifdef CHM_UNIX
#   define COL_INT_64_MAX  0x7FFFFFFFFFFFFFFF##LL   // 9223372036854775807
#else
#   define COL_INT_64_MAX  0x7FFFFFFFFFFFFFFF   // 9223372036854775807
#endif

#define COL_INT_32_MAX  0x7FFFFFFF           // 2147483647
#define COL_INT_16_MAX  0x7FFF               // 32767
#define COL_INT_8_MAX   0x7F                 // 127

// Henry: These were previously written as -0x80000000, etc., however, the compiler would
// warn "unary minus operator applied to unsigned type, result still unsigned".
#ifdef CHM_UNIX
#  define COL_INT_64_MIN  (COLint64)0x8000000000000000ll  // -9223372036854775808
#else
#  define COL_INT_64_MIN  (COLint64)0x8000000000000000  // -9223372036854775808
#endif
#define COL_INT_32_MIN  (COLint32)0x80000000          // -2147483648
#define COL_INT_16_MIN  (COLint16)0x8000              // -32768
#define COL_INT_8_MIN   (COLint8)0x80                // -128

#define COL_FLOAT_64_MAX 3.40282347e+38

//TODO mins and maxs for 32 floats etc.

#define COL_MIN(Num1,Num2) (((Num1) > (Num2)) ? (Num2) : (Num1))
#define COL_MAX(Num1,Num2) (((Num1) < (Num2)) ? (Num2) : (Num1))

#define COL_ABS(NUMBER) ((NUMBER) > 0) ? (NUMBER) : -(NUMBER)

#ifdef WIN32
#  define COL_UINT64_C_LITERAL(C) C##ui64
#else
#  define COL_UINT64_C_LITERAL(C) C##ULL
#endif

// TODO we could put global pragmas in here.

#endif // end of defensive include
