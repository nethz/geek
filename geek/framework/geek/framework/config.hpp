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
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_FRAMEWORK_CONFIG_HPP
#define GEEK_FRAMEWORK_CONFIG_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/config.hpp>



#ifdef GEEK_DYN_LINK 
#	ifdef GEEK_FRAMEWORK_SOURCE
#		define GEEK_FRAMEWORK_DECL GEEK_SYMBOL_EXPORT
#	else
#		define GEEK_FRAMEWORK_DECL GEEK_SYMBOL_IMPORT
#	endif  // GEEK_FRAMEWORK_SOURCE
#else
#	define GEEK_FRAMEWORK_DECL
#endif


namespace geek{
	class system_interface;
	typedef boost::shared_ptr<system_interface> system_ptr;

	class window_interface;
	typedef boost::shared_ptr<window_interface> window_ptr;
	typedef boost::weak_ptr<window_interface> weak_window_ptr;
	static const int step_milliseconds = 17;
	static const float step_seconds = 0.017f;
}

#endif // GEEK_FRAMEWORK_CONFIG_HPP




