/*
-----------------------------------------------------------------------------
This source file is a part of geek
(Game Engine Extensible Kit)
For the latest info, see http://gdgeek.com/

GEEK (www.gdgeek.com) is made available under the MIT License.

Copyright (c) 2010-2011 http://gdgeek.com/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

////////////////////////////////////////////////////////////////////////
///  
/// @file       platform.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/24
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_PLATFORM_HPP
#define GEEK_PLATFORM_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



namespace geek {
/* Initial platform/compiler-related stuff to set.
*/
#define GEEK_PLATFORM_WIN32 1
#define GEEK_PLATFORM_LINUX 2
#define GEEK_PLATFORM_APPLE 3
#define GEEK_PLATFORM_SYMBIAN 4
#define GEEK_PLATFORM_IPHONE 5

#define GEEK_COMPILER_MSVC 1
#define GEEK_COMPILER_GNUC 2
#define GEEK_COMPILER_BORL 3
#define GEEK_COMPILER_WINSCW 4
#define GEEK_COMPILER_GCCE 5

#define GEEK_ENDIAN_LITTLE 1
#define GEEK_ENDIAN_BIG 2

#define GEEK_ARCHITECTURE_32 1
#define GEEK_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if defined( __GCCE__ )
#   define GEEK_COMPILER GEEK_COMPILER_GCCE
#   define GEEK_COMP_VER _MSC_VER
//#	include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE 
#elif defined( __WINSCW__ )
#   define GEEK_COMPILER GEEK_COMPILER_WINSCW
#   define GEEK_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define GEEK_COMPILER GEEK_COMPILER_MSVC
#   define GEEK_COMP_VER _MSC_VER
#elif defined( __GNUC__ )
#   define GEEK_COMPILER GEEK_COMPILER_GNUC
#   define GEEK_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define GEEK_COMPILER GEEK_COMPILER_BORL
#   define GEEK_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__ 
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead*/
#if GEEK_COMPILER == GEEK_COMPILER_MSVC
#   if GEEK_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* Finds the current platform*/

#if defined( __SYMBIAN32__ ) 
#   define GEEK_PLATFORM GEEK_PLATFORM_SYMBIAN
#elif defined( __WIN32__ ) || defined( _WIN32 )
#   define GEEK_PLATFORM GEEK_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
    // Device                                                     Simulator
    // Both requiring OS version 3.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 30000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#       define GEEK_PLATFORM GEEK_PLATFORM_IPHONE
#   else
#       define GEEK_PLATFORM GEEK_PLATFORM_APPLE
#   endif
#else
#   define GEEK_PLATFORM GEEK_PLATFORM_LINUX
#endif

    /* Find the arch type*/
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define GEEK_ARCH_TYPE GEEK_ARCHITECTURE_64
#else
#   define GEEK_ARCH_TYPE GEEK_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define GEEK_QUOTE_INPLACE(x) # x
#define GEEK_QUOTE(x) GEEK_QUOTE_INPLACE(x)
#define GEEK_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32

// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define GEEK_DEBUG_MODE 1
#   else
#       define GEEK_DEBUG_MODE 0
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if GEEK_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || GEEK_COMP_VER > 345
#    define GEEK_UNICODE_SUPPORT 1
#   else
#    define GEEK_UNICODE_SUPPORT 0
#   endif
#  else
#   define GEEK_UNICODE_SUPPORT 1
#  endif
# else
#  define GEEK_UNICODE_SUPPORT 1
# endif
#else
#  define GEEK_UNICODE_SUPPORT 1
#endif

#endif // GEEK_PLATFORM == GEEK_PLATFORM_WIN32

//----------------------------------------------------------------------------
// Symbian Settings
#if GEEK_PLATFORM == GEEK_PLATFORM_SYMBIAN
#	define GEEK_UNICODE_SUPPORT 1
#   define GEEK_DEBUG_MODE 0
#	define CLOCKS_PER_SEC  1000
// pragma def were found here: http://www.inf.pucrs.br/~eduardob/disciplinas/SistEmbarcados/Mobile/Nokia/Tools/Carbide_vs/WINSCW/Help/PDF/C_Compilers_Reference_3.2.pdf
#	pragma warn_unusedarg off
#	pragma warn_emptydecl off
#	pragma warn_possunwant off
#endif
//----------------------------------------------------------------------------
// Linux/Apple/Symbian Settings
#if GEEK_PLATFORM == GEEK_PLATFORM_LINUX || GEEK_PLATFORM == GEEK_PLATFORM_APPLE || GEEK_PLATFORM == GEEK_PLATFORM_IPHONE || GEEK_PLATFORM == GEEK_PLATFORM_SYMBIAN



// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   ifdef DEBUG
#       define GEEK_DEBUG_MODE 1
#   else
#       define GEEK_DEBUG_MODE 0
#   endif


// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define GEEK_UNICODE_SUPPORT 1

#endif

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set GEEK_ENDIAN correctly
#ifdef GEEK_CONFIG_BIG_ENDIAN
#    define GEEK_ENDIAN GEEK_ENDIAN_BIG
#else
#    define GEEK_ENDIAN GEEK_ENDIAN_LITTLE
#endif
/*
// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;
// define uint64 type
#if GEEK_COMPILER == GEEK_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#endif
*/

}

#endif
