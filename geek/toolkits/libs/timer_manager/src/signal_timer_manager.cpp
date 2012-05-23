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

#include <geek/toolkits/timer_manager/signal_timer_manager.hpp>
#include <geek/toolkits/concurrent/concurrent.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/placeholders.hpp>
using namespace geek;

template<> signal_timer_manager* geek::singleton<signal_timer_manager>::singleton_ = NULL;


signal_timer_manager::signal_timer_manager(std::size_t const timed_callback_pool_size, std::size_t const timed_callback_pool_grow_size)
	: timed_callback_pool_(timed_callback_pool_size, timed_callback_pool_grow_size)
	, timed_callback_freelist_(timed_callback_pool_)
	, io_service_(geek::concurrent::get_singleton().get_io_service())
	, first_get_time_(true)
{
}

signal_timer_manager::~signal_timer_manager()
{
}

void signal_timer_manager::delay(timed_func_t const& func, time_point_type time)
{
	timed_callback_ptr callback(timed_callback_freelist_.construct<boost::asio::io_service&>(io_service_), timed_callback_deleter(timed_callback_freelist_));
	callback->time_point = time;
	callback->callback = func;
	callback->is_end = false;

	boost::chrono::milliseconds d = boost::chrono::duration_cast<boost::chrono::milliseconds>(callback->time_point - get_time());
	callback->timer.expires_from_now(boost::posix_time::milliseconds(d.count()));

	queue_.push(callback);
	callback->timer.async_wait(boost::bind(&signal_timer_manager::handle_timeout, this, callback, boost::asio::placeholders::error));
}

void signal_timer_manager::delay(timed_func_t const& func, duration_type time)
{
	delay(func, get_time() + time);
}

bool signal_timer_manager::init(void)
{
	first_get_time_ = true;
	return true;
}

void signal_timer_manager::update(duration_type const& interval)
{
	geek_clock::time_point& now = get_time();
	now  += interval;
	while(!queue_.empty())
	{
		timed_callback_ptr const& cb = queue_.top();

		if (!cb->is_end)
		{
			break;
		}

		cb->callback();
		queue_.pop();
	}
}

void signal_timer_manager::shutdown(void)
{
	clear_queue();
}

void signal_timer_manager::clear_queue()
{
	queue_ = timed_callback_queue();
}

void signal_timer_manager::handle_timeout(timed_callback_ptr callback, boost::system::error_code const& )
{
	callback->is_end = true;
}
