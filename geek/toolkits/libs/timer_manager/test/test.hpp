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


#ifndef GEEK_TOOLKITS_TEST_TIMER_MANAGER_TEST_HPP
#define GEEK_TOOLKITS_TEST_TIMER_MANAGER_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 

#include "boost/lambda/lambda.hpp"
#include <geek/toolkits/concurrent.hpp>
#include <geek/toolkits/log_system.hpp>
#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>


void fun(int seconds)
{
	GEEK_LOG<<"delay seconds: " << seconds << GEEK_NORMAL;
};

void fun2(int step)
{
	GEEK_LOG<<"using timer, step: "<<step<<GEEK_NORMAL;
};

BOOST_AUTO_TEST_CASE(tick_timer_manager_ut)
{
	using namespace geek;

	GEEK_LOG<<"test"<<GEEK_NORMAL;

	boost::scoped_ptr<tick_timer_manager> tm(new tick_timer_manager());
	tick_timer_manager& timer_mgr = tick_timer_manager::get_singleton();

	timer_mgr.init();

	timer_mgr.delay(boost::bind(&fun, 3), boost::chrono::seconds(3));
	timer_mgr.delay(boost::bind(&fun, 4), boost::chrono::seconds(4));
	timer_mgr.delay(boost::bind(&fun, 2), boost::chrono::seconds(2));
	timer_mgr.delay(boost::bind(&fun, 1), boost::chrono::seconds(1));
	timer_mgr.delay(boost::bind(&fun, 7), boost::chrono::seconds(7));

	tick_timer a(boost::bind(&fun2, _1));
	a.start(boost::chrono::seconds(3), -1, boost::chrono::seconds(5));
	bool exit = false;

	timer_mgr.delay((boost::lambda::var(exit) = true), boost::chrono::seconds(15));

	boost::chrono::system_clock::time_point last = geek_clock::now();
	boost::chrono::system_clock::time_point now;
	while(!exit)
	{	
		now = geek_clock::now();
		timer_mgr.update(now - last);
		
		last = now;
	}
}

BOOST_AUTO_TEST_CASE(signal_timer_manager_ut)
{
	using namespace geek;

	GEEK_LOG<<"test"<<GEEK_NORMAL;

	boost::scoped_ptr<concurrent> cc_ptr(new concurrent(5));
	concurrent& cc = concurrent::get_singleton();

	boost::scoped_ptr<signal_timer_manager> tm(new signal_timer_manager());
	signal_timer_manager& timer_mgr = signal_timer_manager::get_singleton();

	timer_mgr.init();

	timer_mgr.delay(boost::bind(&fun, 3), boost::chrono::seconds(3));
	timer_mgr.delay(boost::bind(&fun, 4), boost::chrono::seconds(4));
	timer_mgr.delay(boost::bind(&fun, 2), boost::chrono::seconds(2));
	timer_mgr.delay(boost::bind(&fun, 1), boost::chrono::seconds(1));
	timer_mgr.delay(boost::bind(&fun, 7), boost::chrono::seconds(7));

	signal_timer a(boost::bind(&fun2, _1));
	a.start(boost::chrono::seconds(3), -1, boost::chrono::seconds(5));
	bool exit = false;

	timer_mgr.delay((boost::lambda::var(exit) = true), boost::chrono::seconds(15));

	boost::chrono::system_clock::time_point last = geek_clock::now();
	boost::chrono::system_clock::time_point now;
	while(!exit)
	{	
		now = geek_clock::now();
		timer_mgr.update(now - last);
		
		last = now;
	}
}

#endif// GEEK_TOOLKITS_TEST_TIMER_MANAGER_TEST_HPP
