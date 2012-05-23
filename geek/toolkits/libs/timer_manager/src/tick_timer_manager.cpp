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

#include <geek/toolkits/timer_manager/tick_timer_manager.hpp>

using namespace geek;

template<> tick_timer_manager* geek::singleton<tick_timer_manager>::singleton_ = NULL;


tick_timer_manager::tick_timer_manager()
	: first_get_time_(true)
{
}

tick_timer_manager::~tick_timer_manager()
{
}

void tick_timer_manager::delay(timed_func_t const& func, time_point_type time)
{
	timed_callback callback;
	callback.time_point = time;
	callback.callback = func;
	
	queue_.push(callback);
}

void tick_timer_manager::delay(timed_func_t const& func, duration_type time)
{
	delay(func, get_time() + time);
}

bool tick_timer_manager::init(void)
{
	first_get_time_ = true;
	return true;
}

void tick_timer_manager::update(duration_type const& interval)
{
	geek_clock::time_point& now = get_time();
	now  += interval;
	while(!queue_.empty())
	{
		timed_callback const& cb = queue_.top();
		
		duration_type delta = cb.time_point - now;
		if (boost::chrono::nanoseconds(delta).count() > 0) 
		{
			break;
		}

		cb.callback();
		queue_.pop();
	}
	
}

void tick_timer_manager::shutdown(void)
{
	clear_queue();
}

void tick_timer_manager::clear_queue()
{
	queue_ = timed_callback_queue();
}
