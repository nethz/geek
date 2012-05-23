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
#include "log_stream.hpp"
#include <geek/toolkits/log_system/log_system.hpp>
using namespace geek;

//-------------------------------------------------------------------------------------------------
log_system::~log_system(void)
{
}
//-------------------------------------------------------------------------------------------------
log_system& log_system::get_instance(void)
{
	return *(get_instance_ptr());
}
//-------------------------------------------------------------------------------------------------
log_system* log_system::get_instance_ptr(void)
{
	static log_system instance;
	return &instance;
}
//-------------------------------------------------------------------------------------------------
std::ostream& log_system::stream(void)
{
	create_tss_log_stream();
	return log_stream_->stream();
}
//-------------------------------------------------------------------------------------------------
void log_system::stream_endl(severity_level level)
{
	create_tss_log_stream();
	log_stream_->stream().flush();
	std::string& result = log_stream_->result();
	if(level > warning)
	{
		log_message( result, level);
		
	}else
	{
		log_message(result, level);
	}
	result = "";
}
//-------------------------------------------------------------------------------------------------
log_system::log_system(void)//stream_(new std::ostream()
{
	namespace logging = boost::log;
	namespace fmt = boost::log::formatters;
	namespace flt = boost::log::filters;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace src = boost::log::sources;
	namespace keywords = boost::log::keywords;

	using boost::shared_ptr;


	// For now we only create a text output sink:
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	shared_ptr<text_sink> sink(new text_sink);

	// Here synchronous_sink is a sink frontend that performs thread synchronization
	// before passing log records to the backend (the text_ostream_backend class).
	// The backend formats each record and outputs it to one or several streams.
	// This approach makes implementing backends a lot simplier, because you don't
	// need to worry about multithreading.

	{
		// The good thing about sink frontends is that they are provided out-of-box and
		// take away thread-safety burden from the sink backend implementors. Even if you
		// have to call a custom backend method, the frontend gives you a convenient way
		// to do it in a thread safe manner. All you need is to acquire a locking pointer
		// to the backend.
		text_sink::locked_backend_ptr backend = sink->locked_backend();

		// Now, as long as pBackend lives, you may work with the backend without
		// interference of other threads that might be trying to log.

		// Next we add streams to which logging records should be output
		shared_ptr<std::ostream> stream(&std::cerr, logging::empty_deleter());
		backend->add_stream(stream);

		// We can add more than one stream to the sink backend
		shared_ptr<std::ofstream> stream2(new std::ofstream("geek.log"));
		assert(stream2->is_open());
		backend->add_stream(stream2);
	}

	// Ok, we're ready to add the sink to the logging library
	logging::core::get()->add_sink(sink);

	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());

	// Also let's add some commonly used attributes, like timestamp and record counter.
//	boost::log::add_common_attributes();

	sink->locked_backend()->set_formatter(
		fmt::format("%1% <%2%> %3%")
		% fmt::date_time("TimeStamp", std::nothrow)
		% fmt::attr< severity_level >("Severity", std::nothrow)
		% fmt::message());

	sink->set_filter( flt::attr< severity_level >("Severity", std::nothrow) >= trace ); 
}
//-------------------------------------------------------------------------------------------------
void log_system::create_tss_log_stream()
{
	if (!log_stream_.get())
	{
		log_stream_.reset(new log_stream);
	}
}
//-------------------------------------------------------------------------------------------------
log_system::log_type& log_system::get_global_log(void)
{
	if (!global_log_.get())
	{
		global_log_.reset(new boost::log::sources::severity_logger<log_system::severity_level>());
	}

	return *global_log_;
}
//-------------------------------------------------------------------------------------------------
void log_system::log_message(std::string const& message, log_system::severity_level level, bool mask_debug)
{
#ifdef NDEBUG
	if(mask_debug)
		return;
#endif
	BOOST_LOG_SEV(get_global_log(), level) << message;
}
//-------------------------------------------------------------------------------------------------
void log_system::add_sink(sink_type_ptr const& sink)
{
	boost::log::core::get()->add_sink(sink);
}
//-------------------------------------------------------------------------------------------------
void log_system::remove_sink(sink_type_ptr const& sink)
{
	boost::log::core::get()->remove_sink(sink);
}
//-------------------------------------------------------------------------------------------------
