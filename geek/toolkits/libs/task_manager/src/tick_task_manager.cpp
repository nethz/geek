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

#include <geek/toolkits/task_manager/tick_task_manager.hpp>
#include <boost/make_shared.hpp>

using namespace geek;

template<> tick_task_manager* geek::singleton<tick_task_manager>::singleton_ = NULL;


tick_task_manager::tick_task_manager()
{
}

tick_task_manager::~tick_task_manager()
{
}

void tick_task_manager::update(duration_type const& interval)
{
	update_task(interval, task_list_);

	if (!task_group_list_.empty())
	{
		set<task_group_t::ptr_t>::type::iterator itr(task_group_list_.begin());
		set<task_group_t::ptr_t>::type::iterator end_itr(task_group_list_.end());
		set<task_group_t::ptr_t>::type::iterator del_itr;
		task_group_t::ptr_t task_group;
		while (itr != end_itr)
		{
			task_group = *itr;
			
			update_task(interval, task_group->running_task_list_);
			if (task_group->running_task_list_.empty())
			{
				del_itr = itr;
				++itr;
				task_group_list_.erase(del_itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
tick_task_manager::task_group_t::ptr_t tick_task_manager::create_task_group(std::size_t task_num)
{
	typedef freelist_allocator_st<task_group_t, 0, 32, task_group_t> shared_alloc_t;
	return boost::allocate_shared<task_group_t, shared_alloc_t, std::size_t>(shared_alloc_t(), task_num);
}
//-------------------------------------------------------------------------------------------------
void tick_task_manager::schedule_task(task_t* task)
{
	if (!task->is_in_group())
	{
		std::pair<set<task_t*>::type::iterator, bool> pr = task_list_.insert(task);
		if (pr.second == false)
		{
			GEEK_THROW_EXCEPTION(err_duplicate_item, "task duplicate");
		}
	}
}
//-------------------------------------------------------------------------------------------------
void tick_task_manager::schedule_task(task_group_t::ptr_t task_group)
{
	std::pair<set<task_group_t::ptr_t>::type::iterator, bool> pr = task_group_list_.insert(task_group);
	if (pr.second == false)
	{
		GEEK_THROW_EXCEPTION(err_duplicate_item, "task group duplicate");
	}
}
//-------------------------------------------------------------------------------------------------
void tick_task_manager::update_task(duration_type const& interval, set<task_t*>::type& task_list)
{
	if (!task_list.empty())
	{
		set<task_t*>::type::iterator itr(task_list.begin());
		set<task_t*>::type::iterator end_itr(task_list.end());
		set<task_t*>::type::iterator del_itr;
		task_t* task = NULL;
		while (itr != end_itr)
		{
			task = *itr;
			if (!task->run_callback_(interval))
			{
				task->is_end_ = true;
				del_itr = itr;
				++itr;
				task->end_callback_(interval);
				task->end();
				task_list.erase(del_itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
