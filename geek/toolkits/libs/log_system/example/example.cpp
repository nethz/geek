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
#include "stable_headers.hpp"
#include "example.hpp"
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

void add_stream_just_trace(boost::shared_ptr< std::ostream > stream)
{

	using namespace geek;
	namespace logging = boost::log;
	namespace fmt = boost::log::formatters;
	namespace flt = boost::log::filters;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace src = boost::log::sources;
	namespace keywords = boost::log::keywords;
	using boost::shared_ptr;




	// For now we only create a text output sink:
	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
	shared_ptr< text_sink > sink(new text_sink);

	text_sink::locked_backend_ptr backend = sink->locked_backend();
	backend->add_stream(stream);

	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());

	// Also let's add some commonly used attributes, like timestamp and record counter.
	//	boost::log::add_common_attributes();

	sink->locked_backend()->set_formatter(fmt::format("%1%")% fmt::message());


	log_system::get_instance().add_sink(sink);

	// just trace
	sink->set_filter( flt::attr< log_system::severity_level >("Severity", std::nothrow) == log_system::trace ); 

}

enum
{
    LOG_RECORDS_TO_WRITE = 20,
    THREAD_COUNT = 3
};

void thread_fun(boost::barrier& bar)
{
	// Wait until all threads are created
	bar.wait();

	// Now, do some logging
	for (unsigned int i = 0; i < LOG_RECORDS_TO_WRITE; ++i)
	{
		using namespace geek;
		log_system::get_instance().log_message("my test 2" , log_system::normal);

		GEEK_LOG<<"this is a test: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_TRACE;
		GEEK_LOG<<GEEK_FILE_MESSAGE<<"this is a test with file message: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_ERROR;
	}
}

//以下代码用于关掉所有宏日志
/*
#ifdef GEEK_LOG
	#undef GEEK_LOG
	#define GEEK_LOG /##/
#endif
	*/
int main(void)
{
	try
	{
		using namespace geek;
		log_system::get_instance().log_message("my test");
		log_system::get_instance().log_message("my test 2" , log_system::normal);
		log_system::get_instance().stream() << "log use stream"<<log_level<log_system::normal>::endl;

		GEEK_LOG<<"this is a test with stream: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_TRACE;

		GEEK_LOG<<GEEK_FILE_MESSAGE<<"this is a test with file message: int,"<<1<<"; float, "<<3.14159f<<";double, "<<1.23456<<"."<<GEEK_ERROR;

		// Create logging threads
		boost::barrier bar(THREAD_COUNT);
		boost::thread_group threads;
		for (unsigned int i = 0; i < THREAD_COUNT; ++i)
		{
			threads.create_thread(boost::bind(&thread_fun, boost::ref(bar)));
		}

		// Wait until all action ends
		threads.join_all();

		//把日志文件中的trace储存在一个字符串流里面、
		boost::shared_ptr< std::stringstream > stream(new std::stringstream());
		add_stream_just_trace(stream);
		GEEK_LOG<<"just trace"<<GEEK_TRACE;
		GEEK_LOG<<"no warning"<<GEEK_WARNING;
		
		std::cout<<"stream :"<< stream->str() <<std::endl;
	}
	catch(std::exception& e)
	{

		GEEK_LOG<<e.what()<<GEEK_ERROR;
	}

}
