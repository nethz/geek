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
/// @file       basic_task.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/14
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_BASIC_TASK_HPP
#define GEEK_TOOLKITS_BASIC_TASK_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/task_manager/config.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>
#include <geek/toolkits/exception.hpp>

namespace geek
{
template <typename task_manager_t>
class basic_task
{
	friend task_manager_t;

public:
	typedef task_manager_t task_mgr_t;
	typedef basic_task<task_mgr_t> self_type;
	typedef typename task_mgr_t::ref_count_t ref_count_t;
	typedef typename task_mgr_t::task_run_callback_t run_callback_t;
	typedef typename task_mgr_t::task_end_callback_t end_callback_t;

public:
	/////////////////////////////////////////////////
	/// @brief     构造函数
	/// @param task end callback ; 任务结束回调函数
	/////////////////////////////////////////////////
	basic_task(
		run_callback_t const& run_callback, 
		end_callback_t const& end_callback, 
		std::size_t reserve_dependency_num = 0,
		bool is_in_group = false
		);

	~basic_task();

public:
	/// d depend on this
	void add_dependency(self_type* d);
	void start();
	void reset();
	void clear();
	bool is_root() const;
	bool is_end() const;

private:
	struct dependency
	{
		dependency(ref_count_t const& ref_count, self_type* task) 
			: ref_count(ref_count)
			, task(task)
		{
		}

		ref_count_t ref_count;
		self_type* task;
	};

private:
	inline bool is_in_group() const
	{
		return is_in_group_;
	}

	inline void schedule()
	{
		task_mgr_->schedule_task(this);
	}

	inline ref_count_t share_ref()
	{
		if (ref_count_.empty())
		{
			ref_count_.reset(boost::bind(&self_type::schedule, this));
		}
		is_root_ = false;
		return ref_count_;
	}

	inline void release_src_ref()
	{
		ref_count_.release();
	}

	inline void end()
	{
		BOOST_FOREACH(dependency& d, dependency_list_)
		{
			d.task->release_src_ref();
			d.ref_count.release();
		}
	}
	
private:
	task_mgr_t* task_mgr_;
	ref_count_t ref_count_;
	bool is_root_;
	bool is_end_;
	bool is_in_group_;
	typename vector<dependency>::type dependency_list_;
	run_callback_t run_callback_;
	end_callback_t end_callback_;
};
//-------------------------------------------------------------------------------------------------
// basic_task impl
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline basic_task<task_manager_t>::basic_task(
	run_callback_t const& run_callback, 
	end_callback_t const& end_callback, 
	std::size_t reserve_dependency_num,
	bool is_in_group
	)
	: task_mgr_(&task_manager_t::get_singleton())
	, is_root_(true)
	, is_end_(false)
	, is_in_group_(is_in_group)
	, run_callback_(run_callback)
	, end_callback_(end_callback)
{
	dependency_list_.reserve(reserve_dependency_num);
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline basic_task<task_manager_t>::~basic_task()
{
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline void basic_task<task_manager_t>::add_dependency(self_type* d)
{
	dependency_list_.push_back(dependency(d->share_ref(), d));
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline void basic_task<task_manager_t>::start()
{
	if (!is_root_)
	{
		GEEK_THROW_EXCEPTION(err_invalid_state, "task is not root!");
	}

	task_mgr_->schedule_task(this);
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline void basic_task<task_manager_t>::reset()
{
	is_end_ = false;

	BOOST_FOREACH(dependency& d, dependency_list_)
	{
		d.task->reset();
		d.ref_count = d.task->share_ref();
	}
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline void basic_task<task_manager_t>::clear()
{
	is_root_ = true;
	is_end_ = false;
	ref_count_.release();

	BOOST_FOREACH(dependency& d, dependency_list_)
	{
		d.task->clear();
	}

	dependency_list_.clear();
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline bool basic_task<task_manager_t>::is_root() const
{
	return is_root_;
}
//-------------------------------------------------------------------------------------------------
template <typename task_manager_t>
inline bool basic_task<task_manager_t>::is_end() const
{
	return is_end_;
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_BASIC_TASK_HPP



