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
/// @file       test.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_TOOLKITS_TEST_TASK_MANAGER_TEST_HPP
#define GEEK_TOOLKITS_TEST_TASK_MANAGER_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 


#include <geek/toolkits/concurrent.hpp>
#include <geek/toolkits/log_system.hpp>

#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>


class signal_task_template
{
public:
	signal_task_template(int& index, bool& exit, int max_task_count) : index_(index), exit_(exit), max_task_count_(max_task_count) {}
	~signal_task_template() {}

public:
	bool run(geek::coroutine::ptr_t /*self*/, int id)
	{
		GEEK_LOG << "task[" << id << "] running..." << GEEK_NORMAL;
		return false;
	}

	void end(geek::duration_type const& /*interval*/, int id)
	{
		GEEK_LOG << "task[" << id << "] end." << GEEK_NORMAL;
		if (index_ == max_task_count_)
		{
			exit_ = true;
		}
		else
		{
			++index_;
		}
	}

private:
	int& index_;
	bool& exit_;
	int const max_task_count_;
};


BOOST_AUTO_TEST_CASE(signal_task_manager_ut)
{
	using namespace geek;

	GEEK_LOG << "test begin" << GEEK_NORMAL;

	boost::scoped_ptr<concurrent> cc_ptr(new concurrent(5));
	concurrent& cc = concurrent::get_singleton();

	boost::scoped_ptr<signal_task_manager> tm(new signal_task_manager());
	signal_task_manager& task_mgr = signal_task_manager::get_singleton();

	boost::chrono::steady_clock::time_point start = boost::chrono::steady_clock::now();
	
	boost::chrono::system_clock::time_point last = geek_clock::now();
	boost::chrono::system_clock::time_point now;

	task_group<signal_task>::ptr_t tg = task_mgr.create_task_group(6);

	// tt1
	bool exit1 = false;
	int index1 = 1;
	signal_task_template tt1(index1, exit1, 6);
	{
		signal_task* t1 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 1), boost::bind(&signal_task_template::end, &tt1, _1, 1));
		signal_task* t2 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 2), boost::bind(&signal_task_template::end, &tt1, _1, 2));
		signal_task* t3 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 3), boost::bind(&signal_task_template::end, &tt1, _1, 3));
		signal_task* t4 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 4), boost::bind(&signal_task_template::end, &tt1, _1, 4));
		signal_task* t5 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 5), boost::bind(&signal_task_template::end, &tt1, _1, 5));
		signal_task* t6 = tg->create_task(boost::bind(&signal_task_template::run, &tt1, _1, 6), boost::bind(&signal_task_template::end, &tt1, _1, 6));

		/*
			t4--->t3--->t1
			t5--->t3--->t2
			t5--------->t2
				  t6--->t2
		*/
		t1->add_dependency(t3);
		t3->add_dependency(t4);
		t2->add_dependency(t3);
		t3->add_dependency(t5);
		t2->add_dependency(t5);
		t2->add_dependency(t6);

		tg->add_root_task(t1);
		tg->add_root_task(t2);
	}

	// tt2
	bool exit2 = false;
	int index2 = 1;
	signal_task_template tt2(index2, exit2, 5);

	signal_task t1(boost::bind(&signal_task_template::run, &tt2, _1, 7), boost::bind(&signal_task_template::end, &tt2, _1, 7));
	signal_task t2(boost::bind(&signal_task_template::run, &tt2, _1, 8), boost::bind(&signal_task_template::end, &tt2, _1, 8));
	signal_task t3(boost::bind(&signal_task_template::run, &tt2, _1, 9), boost::bind(&signal_task_template::end, &tt2, _1, 9));
	signal_task t4(boost::bind(&signal_task_template::run, &tt2, _1, 10), boost::bind(&signal_task_template::end, &tt2, _1, 10));
	signal_task t5(boost::bind(&signal_task_template::run, &tt2, _1, 11), boost::bind(&signal_task_template::end, &tt2, _1, 11));
	
	/*
		t11--->t8--->t7
		t11--->t9--->t7
		t11--->t10--->t7
	*/
	t1.add_dependency(&t2);
	t1.add_dependency(&t3);
	t1.add_dependency(&t4);
	t2.add_dependency(&t5);
	t3.add_dependency(&t5);
	t4.add_dependency(&t5);


	tg->start();
	t1.start();


	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		task_mgr.update(now - last);
		
		last = now;
	}

	GEEK_LOG << "reset ---------------------------" << GEEK_NORMAL;

	exit1 = exit2 = false;
	index1 = index2 = 1;

	tg->reset();
	t1.reset();

	tg->start();
	t1.start();

	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		task_mgr.update(now - last);
		
		last = now;
	}

	boost::chrono::steady_clock::duration dt = boost::chrono::steady_clock::now() - start;
	GEEK_LOG << "test time: " << dt.count() << GEEK_NORMAL;
}


class tick_task_template
{
public:
	tick_task_template(int& index, bool& exit, int max_task_count) : index_(index), exit_(exit), max_task_count_(max_task_count) {}
	~tick_task_template() {}

public:
	bool run(geek::duration_type const& /*interval*/, int id)
	{
		GEEK_LOG << "task[" << id << "] running..." << GEEK_NORMAL;
		return false;
	}

	void end(geek::duration_type const& /*interval*/, int id)
	{
		GEEK_LOG << "task[" << id << "] end." << GEEK_NORMAL;
		if (index_ == max_task_count_)
		{
			exit_ = true;
		}
		else
		{
			++index_;
		}
	}

private:
	int& index_;
	bool& exit_;
	int const max_task_count_;
};

BOOST_AUTO_TEST_CASE(tick_task_manager_ut)
{
	using namespace geek;

	GEEK_LOG << "test begin" << GEEK_NORMAL;

	boost::scoped_ptr<concurrent> cc_ptr(new concurrent(5));
	concurrent& cc = concurrent::get_singleton();

	boost::scoped_ptr<tick_task_manager> tm(new tick_task_manager());
	tick_task_manager& task_mgr = tick_task_manager::get_singleton();

	boost::chrono::steady_clock::time_point start = boost::chrono::steady_clock::now();
	
	boost::chrono::system_clock::time_point last = geek_clock::now();
	boost::chrono::system_clock::time_point now;

	task_group<tick_task>::ptr_t tg = task_mgr.create_task_group(6);

	// tt1
	bool exit1 = false;
	int index1 = 1;
	tick_task_template tt1(index1, exit1, 6);
	{
		tick_task* t1 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 1), boost::bind(&tick_task_template::end, &tt1, _1, 1));
		tick_task* t2 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 2), boost::bind(&tick_task_template::end, &tt1, _1, 2));
		tick_task* t3 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 3), boost::bind(&tick_task_template::end, &tt1, _1, 3));
		tick_task* t4 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 4), boost::bind(&tick_task_template::end, &tt1, _1, 4));
		tick_task* t5 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 5), boost::bind(&tick_task_template::end, &tt1, _1, 5));
		tick_task* t6 = tg->create_task(boost::bind(&tick_task_template::run, &tt1, _1, 6), boost::bind(&tick_task_template::end, &tt1, _1, 6));

		/*
			t4--->t3--->t1
			t5--->t3--->t2
			t5--------->t2
				  t6--->t2
		*/
		t1->add_dependency(t3);
		t3->add_dependency(t4);
		t2->add_dependency(t3);
		t3->add_dependency(t5);
		t2->add_dependency(t5);
		t2->add_dependency(t6);

		tg->add_root_task(t1);
		tg->add_root_task(t2);
	}

	// tt2
	bool exit2 = false;
	int index2 = 1;
	tick_task_template tt2(index2, exit2, 5);

	tick_task t1(boost::bind(&tick_task_template::run, &tt2, _1, 7), boost::bind(&tick_task_template::end, &tt2, _1, 7));
	tick_task t2(boost::bind(&tick_task_template::run, &tt2, _1, 8), boost::bind(&tick_task_template::end, &tt2, _1, 8));
	tick_task t3(boost::bind(&tick_task_template::run, &tt2, _1, 9), boost::bind(&tick_task_template::end, &tt2, _1, 9));
	tick_task t4(boost::bind(&tick_task_template::run, &tt2, _1, 10), boost::bind(&tick_task_template::end, &tt2, _1, 10));
	tick_task t5(boost::bind(&tick_task_template::run, &tt2, _1, 11), boost::bind(&tick_task_template::end, &tt2, _1, 11));
	
	/*
		t11--->t8--->t7
		t11--->t9--->t7
		t11--->t10--->t7
	*/
	t1.add_dependency(&t2);
	t1.add_dependency(&t3);
	t1.add_dependency(&t4);
	t2.add_dependency(&t5);
	t3.add_dependency(&t5);
	t4.add_dependency(&t5);


	tg->start();
	t1.start();


	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		task_mgr.update(now - last);
		
		last = now;
	}

	GEEK_LOG << "reset ---------------------------" << GEEK_NORMAL;

	exit1 = exit2 = false;
	index1 = index2 = 1;

	tg->reset();
	t1.reset();

	tg->start();
	t1.start();

	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		task_mgr.update(now - last);
		
		last = now;
	}

	boost::chrono::steady_clock::duration dt = boost::chrono::steady_clock::now() - start;
	GEEK_LOG << "test time: " << dt.count() << GEEK_NORMAL;
}


class coro_task_template
{
public:
	coro_task_template(int& index, bool& exit, int max_task_count) : index_(index), exit_(exit), max_task_count_(max_task_count) {}
	~coro_task_template() {}

public:
	bool run(geek::coroutine::ptr_t /*self*/, int id)
	{
		GEEK_LOG << "task[" << id << "] running..." << GEEK_NORMAL;
		return false;
	}

	void end(geek::coroutine::ptr_t /*self*/, int id)
	{
		GEEK_LOG << "task[" << id << "] end." << GEEK_NORMAL;
		if (index_ == max_task_count_)
		{
			exit_ = true;
		}
		else
		{
			++index_;
		}
	}

private:
	int& index_;
	bool& exit_;
	int const max_task_count_;
};

BOOST_AUTO_TEST_CASE(coro_task_manager_ut)
{
	using namespace geek;

	GEEK_LOG << "test begin" << GEEK_NORMAL;

	boost::scoped_ptr<concurrent> cc_ptr(new concurrent(5));
	concurrent& cc = concurrent::get_singleton();

	boost::scoped_ptr<coro_task_manager> tm(new coro_task_manager());
	coro_task_manager& task_mgr = coro_task_manager::get_singleton();

	boost::chrono::steady_clock::time_point start = boost::chrono::steady_clock::now();
	
	boost::chrono::system_clock::time_point last = geek_clock::now();
	boost::chrono::system_clock::time_point now;

	task_group<coro_task>::ptr_t tg = task_mgr.create_task_group(6);

	// tt1
	bool exit1 = false;
	int index1 = 1;
	coro_task_template tt1(index1, exit1, 6);
	{
		coro_task* t1 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 1), boost::bind(&coro_task_template::end, &tt1, _1, 1));
		coro_task* t2 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 2), boost::bind(&coro_task_template::end, &tt1, _1, 2));
		coro_task* t3 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 3), boost::bind(&coro_task_template::end, &tt1, _1, 3));
		coro_task* t4 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 4), boost::bind(&coro_task_template::end, &tt1, _1, 4));
		coro_task* t5 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 5), boost::bind(&coro_task_template::end, &tt1, _1, 5));
		coro_task* t6 = tg->create_task(boost::bind(&coro_task_template::run, &tt1, _1, 6), boost::bind(&coro_task_template::end, &tt1, _1, 6));

		/*
			t4--->t3--->t1
			t5--->t3--->t2
			t5--------->t2
				  t6--->t2
		*/
		t1->add_dependency(t3);
		t3->add_dependency(t4);
		t2->add_dependency(t3);
		t3->add_dependency(t5);
		t2->add_dependency(t5);
		t2->add_dependency(t6);

		tg->add_root_task(t1);
		tg->add_root_task(t2);
	}

	// tt2
	bool exit2 = false;
	int index2 = 1;
	coro_task_template tt2(index2, exit2, 5);

	coro_task t1(boost::bind(&coro_task_template::run, &tt2, _1, 7), boost::bind(&coro_task_template::end, &tt2, _1, 7));
	coro_task t2(boost::bind(&coro_task_template::run, &tt2, _1, 8), boost::bind(&coro_task_template::end, &tt2, _1, 8));
	coro_task t3(boost::bind(&coro_task_template::run, &tt2, _1, 9), boost::bind(&coro_task_template::end, &tt2, _1, 9));
	coro_task t4(boost::bind(&coro_task_template::run, &tt2, _1, 10), boost::bind(&coro_task_template::end, &tt2, _1, 10));
	coro_task t5(boost::bind(&coro_task_template::run, &tt2, _1, 11), boost::bind(&coro_task_template::end, &tt2, _1, 11));
	
	/*
		t11--->t8--->t7
		t11--->t9--->t7
		t11--->t10--->t7
	*/
	t1.add_dependency(&t2);
	t1.add_dependency(&t3);
	t1.add_dependency(&t4);
	t2.add_dependency(&t5);
	t3.add_dependency(&t5);
	t4.add_dependency(&t5);


	tg->start();
	t1.start();


	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		
		last = now;
	}

	GEEK_LOG << "reset ---------------------------" << GEEK_NORMAL;

	exit1 = exit2 = false;
	index1 = index2 = 1;

	tg->reset();
	t1.reset();

	tg->start();
	t1.start();

	while (!exit1 || !exit2)
	{	
		now = geek_clock::now();
		
		last = now;
	}

	boost::chrono::steady_clock::duration dt = boost::chrono::steady_clock::now() - start;
	GEEK_LOG << "test time: " << dt.count() << GEEK_NORMAL;
}

#endif// GEEK_TOOLKITS_TEST_TASK_MANAGER_TEST_HPP
