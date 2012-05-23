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
/// @file       tick_timer_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_TICK_TIMER_MANAGER_HPP
#define GEEK_TOOLKITS_TICK_TIMER_MANAGER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/timer_manager/config.hpp>
#include <geek/toolkits/singleton.hpp>

namespace geek
{
class concurrent;
///////////////////////////////////////////////////////////////////////
/// @brief      tick timer manager ;  计时器管理器, 负责计时器优先级队列
///
/// @details    
/// @date       2011/09/26
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_TIMER_MANAGER_DECL tick_timer_manager : public singleton<tick_timer_manager>
{
public:
	tick_timer_manager(void);
	~tick_timer_manager(void);

public:
	typedef boost::function<void (void)> timed_func_t;
 
	/**
	 * \brief Delayed execution of a callback
	 * \param func the callback to execute
	 * \param time to delay execution of callback
	 */
	void delay(timed_func_t const& func, duration_type time);

	void delay(timed_func_t const& func, time_point_type time);

	bool init(void);
	void update(duration_type const& interval);
	void shutdown(void);

	/**
	 * \brief Clears the queue
	 */
	void clear_queue();

	/**
	 * \return now
	 */
	
	inline geek_clock::time_point& get_time() 
	{ 
		if (first_get_time_)
		{
			first_get_time_ = false;
			now_ = geek_clock::now();
		}
		return now_; 
	}

private:
	struct timed_callback
	{
		geek_clock::time_point time_point;
		timed_func_t callback;
	};

	class prioritize_callbacks
	{
	public:
		bool operator() (timed_callback const& lhs, timed_callback const& rhs) const
		{
			return rhs.time_point < lhs.time_point;
		}
	};

private:
	typedef std::priority_queue<timed_callback, std::vector<timed_callback>, prioritize_callbacks> timed_callback_queue;

	geek_clock::time_point now_;
	timed_callback_queue queue_;
	bool first_get_time_;
};


}
#endif // GEEK_TOOLKITS_TICK_TIMER_MANAGER_HPP



