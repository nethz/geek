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
/// @file       test.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_TOOLKITS_TEST_DYN_LIB_MANAGER_TEST_HPP
#define GEEK_TOOLKITS_TEST_DYN_LIB_MANAGER_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 


#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>


BOOST_AUTO_TEST_CASE(dyn_lib_manager_ut)
{
	using namespace geek;
	//typedef geek::singleton2<geek::plugins_manager> plugins_manager_singleton;
	boost::shared_ptr<geek::plugins_manager> plugins_mgr(new plugins_manager());

	std::string plugin_name("test_dyn_lib_manager_plugin");

#ifdef _DEBUG
	plugin_name.append(_DEBUG_POSTFIX);
#endif

	plugins_manager::get_singleton().load_plugin(plugin_name);
	plugins_manager::get_singleton_ptr()->init();
	plugins_mgr->shutdown();
	plugins_mgr.reset();
}


#endif// GEEK_TOOLKITS_TEST_DYN_LIB_MANAGER_TEST_HPP
