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
/// @file       system_interface.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_FRAMEWORK_SYSTEM_HPP
#define GEEK_FRAMEWORK_SYSTEM_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/framework/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/prerequisites.hpp>
#include <geek/framework/system_interface.hpp>
#include <boost/foreach.hpp>
#include <geek/toolkits/key_value_server.hpp>
#include <geek/toolkits/dyn_lib_manager/plugins_manager.hpp>
#include <geek/toolkits/timer_manager/tick_timer_manager.hpp>

namespace geek{

	class watche
	{
	public:
		//得到两次调用的时间间隔
		inline geek::duration_type const& get_interval(void)
		{
			geek::time_point_type time = geek::geek_clock::now();
			interval_ = time - now_;
			now_ = time;
			return interval_;
		}
		///重置时间
		inline void reset(void)
		{
			now_ = geek::geek_clock::now();
		}
	private:


		geek::time_point_type now_;
		geek::duration_type interval_;
	};


	/**
	很多时候，当渲染速度较慢时，会导致fps不稳定，继而会导致比如旋转之类的动画不流畅，这时候就需要我们对帧速进行过滤，
	我们将对之前 FilterCount-1 个帧 和当前帧求平均值，作为过滤后的帧速。这个技术是来源于游戏精粹3的文章。
	**/
	template<unsigned int FilterCount = 10>
	class watche_filter
	{
	public:
		//得到两次调用的时间间隔
		geek::duration_type const& get_interval(void)
		{

			geek::time_point_type time = geek::geek_clock::now();
			interval_ = time - now_;
			now_ = time;
			get_exact_last_frame_duration();
			add_to_frame_history();
			get_predicted_frame_duration();


			return interval_;
		}
		///重置时间
		inline void reset(void)
		{
			now_ = geek::geek_clock::now();
		}
	private:

		inline void get_exact_last_frame_duration()
		{
			if (boost::chrono::nanoseconds(interval_).count() > 200000000 && !frame_duration_history_.empty())
			{
				interval_ = frame_duration_history_.back(); 
			}

		}


		inline void get_predicted_frame_duration ()
		{
			duration_type total_frame_time = boost::chrono::seconds(0);

			std::deque<duration_type>::const_iterator it;
			for (it = frame_duration_history_.begin(); it != frame_duration_history_.end(); ++it)
				total_frame_time += *it;
			interval_ =total_frame_time/frame_duration_history_.size();
		}

		void add_to_frame_history ()
		{
			frame_duration_history_.push_back (interval_);
			if (frame_duration_history_.size () > (unsigned int) FilterCount)
				frame_duration_history_.pop_front ();
		}


		geek::time_point_type now_;
		geek::duration_type interval_;

		std::deque<duration_type> frame_duration_history_;
	};


	template<class StopWatchs = watche_filter<10> >
	class system: public system_interface
	{
	public:

		//得到两次调用的时间间隔
		virtual duration_type const& get_interval(void)
		{
			return watche_.get_interval();
		}
		///重置时间
		virtual void reset(void)
		{
			watche_.reset();
		}

		///依次初始化每个子系统
		virtual bool init(void)
		{
			init_ = true; 

			exit_ = false;
			return true;
		}
		///依次关闭每个子系统
		virtual void shutdown(void)
		{
			init_ = false; 
		}
		///依次更新所有子系统，参数是两次调用的时间间隔
		virtual void update(duration_type const& interval)
		{

		}


		virtual bool is_init(void) const
		{
			return init_;
		}

		///启动系统循环
		virtual void run(void)
		{

			reset();
			while(is_running())
			{
				update(get_interval());
			}
		}
		///推出系统运行

		virtual void exit(void)
		{
			exit_ = true;
		}
		virtual  bool is_running(void) const
		{
			return !exit_;
		}

	private:
		StopWatchs watche_;
		bool init_;
		bool exit_;

	};

	template<class StopWatchs = watche_filter<10> >
	class system_by_functor: public geek::system<StopWatchs>
	{
	public:
		typedef boost::function<bool (void)> init_function;
		typedef boost::function<void (duration_type const&)> update_function;
		typedef boost::function<void (void)> shutdown_function;
	private:
	public:
		system_by_functor(void)
		{

		}
		virtual ~system_by_functor(void)
		{
			shutdown_list_.clear();
			update_list_.clear();
			init_list_.clear();
			obj_list_.clear();
		}

		inline system_by_functor& push_obj(boost::shared_ptr<void> obj)
		{	
			obj_list_.push_back(obj);
			return *this;
		}	

		inline system_by_functor& add_init(init_function init)
		{	
			init_list_.push_back(init);
			return *this;
		}

		inline system_by_functor& add_update(update_function update)
		{	
			update_list_.push_back(update);
			return *this;
		}

		inline system_by_functor& add_shutdown(shutdown_function shutdown)
		{
			shutdown_list_.push_back(shutdown);
			return *this;
		}
		virtual void shutdown(void)
		{
			geek::system<StopWatchs>::shutdown();
			BOOST_REVERSE_FOREACH(shutdown_function shutdown, shutdown_list_)
			{
				shutdown();
			}
		}
		virtual void update(duration_type const& interval)
		{

			bool ret = true;
			BOOST_FOREACH(update_function update, update_list_)
			{
				update(interval);
			}
			geek::system<StopWatchs>::update(interval);
		}

		virtual bool init(void)
		{
			bool ret = true;
			BOOST_FOREACH(init_function init, init_list_)
			{
				ret = ret && init();
			}
			return ret && geek::system<StopWatchs>::init();
		}

		inline void add_common_view(void)
		{
			boost::shared_ptr<key_value_server> kvs_ptr(new key_value_server());
			push_obj(kvs_ptr);

			boost::shared_ptr<plugins_manager> pm_ptr(new plugins_manager());
			push_obj(pm_ptr)
				.add_init(boost::bind(&plugins_manager::init, pm_ptr.get()))
				.add_shutdown(boost::bind(&plugins_manager::shutdown, pm_ptr.get()));

			boost::shared_ptr<tick_timer_manager> tm_ptr(new tick_timer_manager());
			push_obj(tm_ptr)
				.add_update(boost::bind(&tick_timer_manager::update, tm_ptr.get(), _1));

		}
	private:


		std::vector<boost::shared_ptr<void> > obj_list_;
		std::vector<init_function> init_list_;
		std::vector<update_function> update_list_;
		std::vector<shutdown_function> shutdown_list_;



	};

}
//#include <orz/Framework_Base/System/SystemTools.h>
#endif // GEEK_FRAMEWORK_SYSTEM_INTERFACE_HPP


