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
/// @file       basic_timer.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_BASIC_TIMER_HPP
#define GEEK_TOOLKITS_BASIC_TIMER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/timer_manager/config.hpp>
#include <geek/toolkits/exception.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      Timer 定时器，定期调用某个仿函数,
///
/// @details    
/// @date       2011/09/26
///////////////////////////////////////////////////////////////////////
template <typename timer_manager_t>
class basic_timer
{
public:
	typedef boost::function<void (int step)> callback_function;
public:

	/////////////////////////////////////////////////
	/// @brief     构造函数
	/// @details   
	/// @param callback  {英文翻译} 仿函数,里面的参数是经过几个
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	basic_timer(callback_function const& callback);

	~basic_timer(void);

	/////////////////////////////////////////////////
	/// @brief     {英文翻译} ; 计时器开始
	/// @details   
	/// @param interval {英文翻译} ; 两次调用间隔
	/// @param times {英文翻译} ; 调用次数，当为负数的时候循环
	/// @param begin {英文翻译} ; 第一次调用前的时间间隔
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void start(duration_type interval, int times = -1,  duration_type begin = duration_type(0));


	/////////////////////////////////////////////////
	/// @brief     stop basic_timer ; 关掉计时器
	/// @details   
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void stop(void);


	/////////////////////////////////////////////////
	/// @brief     is basic_timer enable ; 检查计时器是否在运行
	/// @details   
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	bool is_enable(void) const;

private:
	void callback();
	

private:
	bool enable_;
	timer_manager_t& timer_mgr_;
	callback_function callback_;
	int times_;
	duration_type interval_;
	time_point_type time_point_;
};
//-------------------------------------------------------------------------------------------------
// timer impl
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
basic_timer<timer_manager_t>::basic_timer(callback_function const& callback)
	: callback_(callback)
	, timer_mgr_(timer_manager_t::get_singleton())
{
}
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
basic_timer<timer_manager_t>::~basic_timer()
{
}
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
void basic_timer<timer_manager_t>::start(duration_type interval, int times, duration_type begin)
{
	interval_ = interval;
	times_ = times;
	time_point_ = timer_mgr_.get_time() + begin;
	timer_mgr_.delay(boost::bind(&basic_timer<timer_manager_t>::callback, this), time_point_);
	
	enable_ = true;
}
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
void basic_timer<timer_manager_t>::stop(void)
{
	enable_ = false;
	times_ = 0;
}
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
bool basic_timer<timer_manager_t>::is_enable(void) const
{
	return enable_;
}
//-------------------------------------------------------------------------------------------------
template <typename timer_manager_t>
void basic_timer<timer_manager_t>::callback()
{
	if (enable_)
	{
		time_point_type now = timer_mgr_.get_time();
		int step = 0;
		while (now > time_point_)
		{
			time_point_ += interval_;
			++ step;
		}

		callback_(step);

		if (times_ < 0)
		{
			timer_mgr_.delay(boost::bind(&basic_timer<timer_manager_t>::callback, this), interval_);
		}
		else if (times_ > 0)
		{
			--times_;
			timer_mgr_.delay(boost::bind(&basic_timer<timer_manager_t>::callback, this), interval_);
		}
	}
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_BASIC_TIMER_HPP
