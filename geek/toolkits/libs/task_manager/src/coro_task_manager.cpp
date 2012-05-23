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

#include <geek/toolkits/task_manager/coro_task_manager.hpp>
#include <geek/toolkits/exception.hpp>
#include <boost/make_shared.hpp>

namespace geek
{
struct coro_task_manager_coro_tag {};
}

using namespace geek;

//-------------------------------------------------------------------------------------------------
template<> coro_task_manager* geek::singleton<coro_task_manager>::singleton_ = NULL;
//-------------------------------------------------------------------------------------------------
coro_task_manager::coro_task_manager()
{
}
//-------------------------------------------------------------------------------------------------
coro_task_manager::~coro_task_manager()
{
}
//-------------------------------------------------------------------------------------------------
coro_task_manager::task_group_t::ptr_t coro_task_manager::create_task_group(std::size_t task_num)
{
	typedef freelist_allocator_st<task_group_t, 0, 32, task_group_t> shared_alloc_t;
	return boost::allocate_shared<task_group_t, shared_alloc_t, std::size_t>(shared_alloc_t(), task_num);
}
//-------------------------------------------------------------------------------------------------
void coro_task_manager::schedule_task(task_t* task)
{
	go<coro_task_manager_coro_tag>(boost::bind(&coro_task_manager::run_task, this, _1, task));
}
//-------------------------------------------------------------------------------------------------
void coro_task_manager::schedule_task(task_group_t::ptr_t task_group)
{
	BOOST_FOREACH(task_t* task, task_group->root_list_)
	{
		go<coro_task_manager_coro_tag>(boost::bind(&coro_task_manager::run_task_group, this, _1, task, task_group));
	}
}
//-------------------------------------------------------------------------------------------------
void coro_task_manager::run_task(coroutine::ptr_t self, task_t* task)
{
	if (!task->run_callback_(self))
	{
		task->is_end_ = true;
		task->end_callback_(self);
		task->end();
	}
	else
	{
		go<coro_task_manager_coro_tag>(boost::bind(&coro_task_manager::run_task, this, _1, task));
	}
}
//-------------------------------------------------------------------------------------------------
void coro_task_manager::run_task_group(coroutine::ptr_t self, task_t* task, task_group_t::ptr_t task_group)
{
	if (!task->run_callback_(self))
	{
		task->is_end_ = true;
		task->end_callback_(self);
		task->end();
	}
	else
	{
		go<coro_task_manager_coro_tag>(boost::bind(&coro_task_manager::run_task_group, this, _1, task, task_group));
	}
}
//-------------------------------------------------------------------------------------------------
