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
/// @file       signal_task_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/14
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_SIGNAL_TASK_MANAGER_HPP
#define GEEK_TOOLKITS_SIGNAL_TASK_MANAGER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/task_manager/config.hpp>
#include <geek/toolkits/task_manager/task_group.hpp>
#include <geek/toolkits/task_manager/basic_task.hpp>
#include <geek/toolkits/concurrent/coroutine.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>
#include <geek/toolkits/pool/freelist_allocator.hpp>
#include <geek/toolkits/singleton.hpp>


namespace geek
{
class GEEK_TOOLKITS_TASK_MANAGER_DECL signal_task_manager : public singleton<signal_task_manager>, private boost::noncopyable
{
public:
	typedef basic_task<signal_task_manager> task_t;
	typedef task_group<task_t> task_group_t;
	typedef boost::function<bool (coroutine::ptr_t)> task_run_callback_t; // return true continue ; false end task
	typedef boost::function<void (duration_type const& interval)> task_end_callback_t;

public:
	class ref_count_t
	{
	public:
		typedef boost::function<void ()> callback_t;

		ref_count_t() {}
		~ref_count_t() {}

	public:
		void reset(callback_t const& callback)
		{
			null n; // nothing but just a tmp
			ref_count_.reset(&n, null_deleter(callback), freelist_allocator_st<null, 0, 32, ref_count_t>());
		}

		void release()
		{
			ref_count_.reset();
		}

		bool empty()
		{
			return ref_count_.get() == NULL;
		}

	private:
		struct null 
		{
			boost::uint32_t n;
		};

		struct null_deleter
		{
			null_deleter(callback_t const& callback) : callback(callback) {}

			void operator()(null*)
			{
				callback();
			}

			callback_t callback;
		};

		boost::shared_ptr<null> ref_count_;
	};

public:
	signal_task_manager();
	~signal_task_manager();

public:
	void update(duration_type const& interval);

	task_group_t::ptr_t create_task_group(std::size_t task_num);
	void schedule_task(task_t* task);
	void schedule_task(task_group_t::ptr_t task_group);

private:
	void update_task(duration_type const& interval, set<task_t*>::type& task_list);
	void run_task(coroutine::ptr_t self, task_t* task);

private:
	set<task_t*>::type task_list_;
	set<task_group_t::ptr_t>::type task_group_list_;
};
}
#endif // GEEK_TOOLKITS_SIGNAL_TASK_MANAGER_HPP



