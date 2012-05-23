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
/// @file       signal_timer_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/1
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_SIGNAL_TIMER_MANAGER_HPP
#define GEEK_TOOLKITS_SIGNAL_TIMER_MANAGER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/timer_manager/config.hpp>
#include <geek/toolkits/pool/freelist.hpp>
#include <geek/toolkits/pool/freelist_policy_st.hpp>
#include <geek/toolkits/singleton.hpp>
#include <boost/asio/deadline_timer.hpp>

// forward declare
namespace boost 
{
namespace system
{
class error_code;
}
namespace asio 
{ 
class io_service; 
} 
}

namespace geek
{
class concurrent;
///////////////////////////////////////////////////////////////////////
/// @brief      signal timer manager ;  使用信号方式实现的计时器管理器
///
/// @details    
/// @date       2011/10/1
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_TIMER_MANAGER_DECL signal_timer_manager : public singleton<signal_timer_manager>
{
public:
	signal_timer_manager(std::size_t const timed_callback_pool_size = 0, std::size_t const timed_callback_pool_grow_size = 32);
	~signal_timer_manager(void);

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
		timed_callback(boost::asio::io_service& io_service)
			: timer(io_service)
		{
		}

		geek_clock::time_point time_point;
		boost::asio::deadline_timer timer;
		timed_func_t callback;
		bool is_end;
	};

	typedef boost::shared_ptr<timed_callback> timed_callback_ptr;

	class prioritize_callbacks
	{
	public:
		bool operator() (timed_callback_ptr const& lhs, timed_callback_ptr const& rhs) const
		{
			return rhs->time_point < lhs->time_point;
		}
	};

	typedef boost::shared_ptr<boost::asio::deadline_timer> deadline_timer_ptr;
	void handle_timeout(timed_callback_ptr callback, boost::system::error_code const& error);

private:
	typedef freelist_policy_st<timed_callback> timed_callback_pool_t;
	typedef freelist<timed_callback, timed_callback_pool_t> timed_callback_freelist_t;

	struct timed_callback_deleter
	{
		timed_callback_deleter(timed_callback_freelist_t& timed_callback_freelist)
			: timed_callback_freelist(timed_callback_freelist)
		{
		}

		void operator()(timed_callback* ptr)
		{
			timed_callback_freelist.destruct(ptr);
		}

		timed_callback_freelist_t& timed_callback_freelist;
	};

	typedef std::priority_queue<timed_callback_ptr, std::vector<timed_callback_ptr>, prioritize_callbacks> timed_callback_queue;

	timed_callback_pool_t timed_callback_pool_;
	timed_callback_freelist_t timed_callback_freelist_;
	boost::asio::io_service& io_service_;
	geek_clock::time_point now_;
	timed_callback_queue queue_;
	bool first_get_time_;
};


}
#endif // GEEK_TOOLKITS_SIGNAL_TIMER_MANAGER_HPP



