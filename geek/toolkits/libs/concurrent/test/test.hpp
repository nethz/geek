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
/// @author     NX
/// @date       2011/09/25
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_TOOLKITS_TEST_CONCURRENT_TEST_HPP
#define GEEK_TOOLKITS_TEST_CONCURRENT_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 

#include <geek/toolkits/log_system.hpp>
//#include <geek/toolkits/pool/freelist.hpp>
//#include <geek/toolkits/pool/freelist_policy_st.hpp>
#include <geek/toolkits/exception.hpp>
#include <boost/thread/barrier.hpp>

#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>


namespace ut
{
enum 
{
	THREAD_COUNT = GEEK_PROCESSOR_NUM - 1,
	PRINT_COUNT = 10,
	CC_THREAD_COUNT = 4
};

class printer
{
public:
	printer(geek::concurrent& cc)
		: cc_(cc)
	{
	}

	void print(std::string const& msg)
	{
		GEEK_LOG << msg << GEEK_NORMAL;
	}

	void print(geek::coroutine::ptr_t coro, std::string const& msg)
	{
		BOOST_ASSERT(coro != NULL);

		std::string status("begin!");
		//GEEK_LOG << "cosync print - " << status << GEEK_NORMAL;

		cc_.post(
			coro->get_strand(), 
			boost::bind(
				&printer::handle_print, 
				this, 
				coro, 
				boost::ref(status),
				boost::cref(msg)
				)
			);

		coro->yield();

		//GEEK_LOG << "cosync print - " << status << GEEK_NORMAL;
	}

private:
	void handle_print(
		geek::coroutine::ptr_t coro, 
		std::string& status, 
		std::string const& msg
		)
	{
		status.assign("end!");
		GEEK_LOG << msg << GEEK_NORMAL;
		coro->call();
	}

private:
	geek::concurrent& cc_;
};

void print_test_thread(printer& ptr, boost::barrier& bar)
{
	geek::concurrent& cc = geek::concurrent::get_singleton();

	for (std::size_t i=0; i<PRINT_COUNT; ++i)
	{
		cc.post(boost::bind(&printer::print, &ptr, "random post"));
		cc.post(0, boost::bind(&printer::print, &ptr, "post to queue 0"));
		cc.post(1, boost::bind(&printer::print, &ptr, "post to queue 1"));
		cc.post(4, boost::bind(&printer::print, &ptr, "post to queue 4"));
	}
	bar.wait();
}

void print_test_coroutine(geek::coroutine::ptr_t coro, printer& ptr, geek::channel<std::size_t>::ptr_t chan, std::size_t index)
{
 	for (std::size_t i=0; i<PRINT_COUNT; ++i)
 	{
 		ptr.print(coro, "coroutine print 1");
 		ptr.print(coro, "coroutine print 2");
 		ptr.print(coro, "coroutine print 3");
 		ptr.print(coro, "coroutine print 4");
 	}
 	chan->send(coro, index);
}

void print_test_coroutine_main(geek::coroutine::ptr_t coro)
{
	printer ptr(geek::concurrent::get_singleton());
	geek::channel<std::size_t>::ptr_t chan = geek::channel<std::size_t>::create<std::size_t, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32>();

 	for (std::size_t i=0; i<THREAD_COUNT; ++i)
 	{
 		geek::go<int, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32>(boost::bind(&print_test_coroutine, _1, boost::ref(ptr), chan, i));
 	}

 	for (std::size_t i=0; i<THREAD_COUNT; ++i)
 	{
 		std::size_t index = 0;
 		chan->recv(coro, index);
 		GEEK_LOG << "print_test_coroutine [" << index << "] finished!" << GEEK_NORMAL;
 	}

	GEEK_LOG << "please push enter button to quit ... " << GEEK_NORMAL;
}

}


BOOST_AUTO_TEST_CASE(concurrent_ut)
{
	
	using namespace geek;
	using namespace ut;
	
	BOOST_STATIC_ASSERT(THREAD_COUNT < CC_THREAD_COUNT);

	try
	{
		GEEK_LOG << "test" << GEEK_NORMAL;

		boost::scoped_ptr<concurrent> cc_ptr(new concurrent(8, CC_THREAD_COUNT));
		concurrent& cc = concurrent::get_singleton();

		printer ptr(cc);
		boost::barrier bar(THREAD_COUNT + 1);

		for (std::size_t i=0; i<THREAD_COUNT; ++i)
		{
			cc.post(boost::bind(&print_test_thread, boost::ref(ptr), boost::ref(bar)));
		}

		bar.wait();
	}
	catch(std::exception& ex)
	{
		GEEK_LOG << ex.what() << GEEK_ERROR;
	}
	 
}

BOOST_AUTO_TEST_CASE(concurrent_coroutine_ut)
{
	
	using namespace geek;
	using namespace ut;
	
	BOOST_STATIC_ASSERT(THREAD_COUNT < CC_THREAD_COUNT);

	try
	{
		GEEK_LOG << "test" << GEEK_NORMAL;

		freelist_policy_st<coroutine> coroutine_pool;
		freelist<coroutine, freelist_policy_st<coroutine> > coroutine_freelist(coroutine_pool);

		boost::scoped_ptr<concurrent> cc_ptr(new concurrent(8, CC_THREAD_COUNT));
		
		geek::go<short, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32>(boost::bind(&print_test_coroutine_main, _1));
	}
	catch(std::exception& ex)
	{
		GEEK_LOG << ex.what() << GEEK_ERROR;
	}
}

#endif// GEEK_TOOLKITS_TEST_CONCURRENT_TEST_HPP
