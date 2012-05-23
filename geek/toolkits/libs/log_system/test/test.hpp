﻿/*
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
/// @file       test.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/24
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_TEST_LOG_SYSTEM_TEST_HPP
#define GEEK_TOOLKITS_TEST_LOG_SYSTEM_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 


#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>


BOOST_AUTO_TEST_CASE(log_system_ut)
{
	using namespace geek;

	log_system::get_instance().log_message("my test");
	log_system::get_instance().log_message("my test 2" , log_system::normal);

	GEEK_LOG<<"this is a test: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_TRACE;
	GEEK_LOG<<GEEK_FILE_MESSAGE<<"this is a test with file message: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_ERROR;
}


#endif// GEEK_TOOLKITS_TEST_LOG_SYSTEM_TEST_HPP