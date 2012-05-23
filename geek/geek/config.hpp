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
/// @file       config.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_CONFIG_HPP
#define GEEK_CONFIG_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <geek/platform.hpp>
#include <geek/std_header.hpp>


#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#	ifndef WINVER
#		define WINVER 0x0501
#	endif
#	if GEEK_ARCH_TYPE == GEEK_ARCHITECTURE_64
#		ifndef _WIN64_WINNT
#			define _WIN64_WINNT WINVER
#		endif
#	else
#		ifndef _WIN32_WINNT
#			define _WIN32_WINNT WINVER
#		endif
#	endif
#endif

#include <boost/asio/io_service.hpp> // for avoid "winsock already defined balbal.." error

#define GEEK_SYMBOL_EXPORT BOOST_SYMBOL_EXPORT
#define GEEK_SYMBOL_IMPORT BOOST_SYMBOL_IMPORT

#ifndef GEEK_STATIC_LINK
#	ifndef GEEK_DYN_LINK
#		define GEEK_DYN_LINK 1
#	endif// GEEK_DYN_LINK
#endif// GEEK_STATIC_LINK



#ifdef BOOST_MSVC
#     define GEEK_MSVC BOOST_MSVC
#endif

#ifdef GEEK_MSVC
// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#pragma warning( disable: 4661)
#pragma warning( disable: 4251)
#pragma warning( disable: 4996)
//for boost::variant
#pragma warning( disable: 4503)
//for boost's pdb
#pragma warning (disable: 4244)
//for boost's Unicode
#pragma warning (disable: 4819)



#pragma warning (disable: 4005)
#endif
//#endif

#include <geek/prerequisites.hpp>

#endif // GEEK_CONFIG_HPP
