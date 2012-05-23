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
/// @file       task_group.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/14
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_TASK_GROUP_HPP
#define GEEK_TOOLKITS_TASK_GROUP_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/task_manager/config.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>
#include <geek/toolkits/exception.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace geek
{
template <typename task_t>
class task_group : public boost::enable_shared_from_this<task_group<task_t> >
{
	typedef typename task_t::task_mgr_t task_manager_t;
	friend task_manager_t;

public:
	typedef boost::shared_ptr<task_group<task_t> > ptr_t;
	typedef typename task_t::run_callback_t run_callback_t;
	typedef typename task_t::end_callback_t end_callback_t;

	explicit task_group(std::size_t task_num);
	~task_group();

public:
	task_t* create_task(
		run_callback_t const& run_callback, 
		end_callback_t const& end_callback,
		std::size_t reserve_dependency_num = 0
		);
	void add_root_task(task_t* root);

	void start();
	void reset();
	void clear();

private:
	inline void set_running_list()
	{
		BOOST_FOREACH(task_t& task, task_list_)
		{
			running_task_list_.insert(&task);
		}
	}

private:
	task_manager_t* task_mgr_;
	typename vector<task_t>::type task_list_;
	typename vector<task_t*>::type root_list_;
	typename set<task_t*>::type running_task_list_;
};
//-------------------------------------------------------------------------------------------------
// task_group impl
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline task_group<task_t>::task_group(std::size_t task_num)
	: task_mgr_(&task_manager_t::get_singleton())
{
	task_list_.reserve(task_num);
	root_list_.reserve(task_num);
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline task_group<task_t>::~task_group()
{
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline task_t* task_group<task_t>::create_task(
	run_callback_t const& run_callback, 
	end_callback_t const& end_callback,
	std::size_t reserve_dependency_num
	)
{
	if (task_list_.size() == task_list_.capacity())
	{
		GEEK_THROW_EXCEPTION(err_invalid_state, "task num is max!")
	}

	task_list_.push_back(task_t(run_callback, end_callback, reserve_dependency_num, true));
	return &task_list_.back();
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline void task_group<task_t>::add_root_task(task_t* root)
{
	if (!root->is_root())
	{
		GEEK_THROW_EXCEPTION(err_invalid_state, "task is not root!");
	}

	root_list_.push_back(root);
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline void task_group<task_t>::start()
{
	set_running_list();
	task_mgr_->schedule_task(shared_from_this());
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline void task_group<task_t>::reset()
{
	running_task_list_.clear();
	BOOST_FOREACH(task_t* task, root_list_)
	{
		task->reset();
	}
}
//-------------------------------------------------------------------------------------------------
template <typename task_t>
inline void task_group<task_t>::clear()
{
	task_list_.clear();
	root_list_.clear();
	running_task_list_.clear();
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_TASK_GROUP_HPP



