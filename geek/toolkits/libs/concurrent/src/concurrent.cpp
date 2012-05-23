
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
#include <geek/toolkits/concurrent/concurrent.hpp>
#include <geek/toolkits/exception.hpp>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>

using namespace geek;

//-------------------------------------------------------------------------------------------------
template<> concurrent* geek::singleton<concurrent>::singleton_ = NULL;
//-------------------------------------------------------------------------------------------------
concurrent::concurrent(
	std::size_t queue_count, 
	std::size_t thread_count,
	bool is_force_shutdown
	)
	: is_internal_(true)
	, is_force_shutdown_(is_force_shutdown)
	, io_service_(new boost::asio::io_service, io_service_deleter(is_internal_))
	, work_(new boost::asio::io_service::work(*io_service_))
	, queue_count_(queue_count)
	, thread_count_(thread_count)
	, queue_list_(queue_count_)
{
	initialize();
}
//-------------------------------------------------------------------------------------------------
concurrent::concurrent(
	boost::asio::io_service& io_service, 
	std::size_t queue_count, 
	std::size_t thread_count,
	bool is_force_shutdown
	)
	: is_internal_(false)
	, is_force_shutdown_(is_force_shutdown)
	, io_service_(&io_service, io_service_deleter(is_internal_))
	, work_(new boost::asio::io_service::work(*io_service_))
	, queue_count_(queue_count)
	, thread_count_(thread_count)
	, queue_list_(queue_count_)
{
	initialize();
}
//-------------------------------------------------------------------------------------------------
concurrent::~concurrent(void)
{
	work_.reset();

	try
	{
		if (is_force_shutdown_)
		{
			io_service_->stop();
		}

		thread_pool_.join_all();
	}
	catch(...)
	{
	//!!	GEEK_LOG << "unexpected exception" << GEEK_ERROR;
	}
}
//-------------------------------------------------------------------------------------------------
boost::asio::io_service& concurrent::get_io_service()
{
	BOOST_ASSERT(io_service_);

	return *io_service_;
}
//-------------------------------------------------------------------------------------------------
void concurrent::initialize()
{
	BOOST_ASSERT(thread_count_ > 0);

	BOOST_FOREACH(queue_ptr& queue, queue_list_)
	{
		queue.reset(new boost::asio::io_service::strand(*io_service_));
	}
	for (std::size_t i=0; i<thread_count_; ++i)
	{
		thread_pool_.create_thread(boost::bind(&concurrent::run, this));
	}
}
//-------------------------------------------------------------------------------------------------
void concurrent::run()
{
	for (;;)
	{
		try
		{
			io_service_->run();
			break;
		}
		catch(...)
		{
		//	GEEK_LOG << "unexpected exception" << GEEK_ERROR;
		}
	}
}
//-------------------------------------------------------------------------------------------------
// concurrent::io_service_deleter
//-------------------------------------------------------------------------------------------------
concurrent::io_service_deleter::io_service_deleter(bool is_delete)
	 : is_delete_(is_delete)
{
}
//-------------------------------------------------------------------------------------------------
void concurrent::io_service_deleter::operator()(boost::asio::io_service* io_serivce)
{
	if (is_delete_)
	{
		delete io_serivce;
	}
}
//-------------------------------------------------------------------------------------------------
