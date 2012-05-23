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
/// @author     NX
/// @date       2011/10/1
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_TOOLKITS_TEST_POOL_TEST_HPP
#define GEEK_TOOLKITS_TEST_POOL_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 

#include <geek/toolkits/log_system.hpp>

#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/thread/barrier.hpp>

typedef boost::chrono::steady_clock test_clock_t;

enum 
{
	TEST_COUNT = 100000,
	PRE_TEST_COUNT = 10,
	N_TEST_COUNT = TEST_COUNT/PRE_TEST_COUNT,
	TEST_THREAD_COUNT = GEEK_PROCESSOR_COUNT,
	TEST_PRE_THREAD_COUNT = TEST_COUNT / TEST_THREAD_COUNT
};

struct uintish
{
	unsigned int value;
	uintish() : value(5) { }
	uintish(const uintish &o) : value(o.value) { }
};

class my_object
{
public:
	my_object() : uint_(0), int_(-1), str_(NULL) {}
	explicit my_object(int i) : uint_(0), int_(i), str_(NULL) {}
	~my_object() {}

	void print()
	{
		GEEK_LOG << "my_object::print: " << uint_ << ", " << int_ << GEEK_NORMAL;
	}

private:
	unsigned int uint_;
	int int_;
	uintish uintish_;
	char* str_;
};

class my_obj
{
public:
	my_obj()
		: uint_(0)
		, int_(-1)
	{
		GEEK_LOG << "my_object::my_object 1" << GEEK_NORMAL;
	}

	my_obj(int i)
		: uint_(0)
		, int_(i)
	{
		GEEK_LOG << "my_object::my_object 2" << GEEK_NORMAL;
	}

	~my_obj()
	{
		GEEK_LOG << "my_object::~my_object" << GEEK_NORMAL;
	}

	void print()
	{
		GEEK_LOG << "my_object::print: " << uint_ << ", " << int_ << GEEK_NORMAL;
	}

	unsigned int uint_;
	int int_;
};

typedef my_object test_t;


BOOST_AUTO_TEST_CASE(pool_ut)
{
	using namespace geek;

	GEEK_LOG << "testing... ";

	test_t t;

	vector<test_t>::type v;
	v.reserve(20);
	for (std::size_t i=0; i<25; ++i)
	{
		v.push_back(t);
		t = v[i];
	}
	v.pop_back();

	list<test_t>::type l;
	l.push_back(t);

	deque<test_t>::type d;
	d.push_back(t);
	d.push_front(t);

	map<test_t*, test_t>::type m;
	m.insert(std::make_pair(&t, t));
	map<test_t*, test_t>::type::iterator itr = m.find(&t);
	itr->second;

	GEEK_LOG << "done.\n" << GEEK_NORMAL;
}

BOOST_AUTO_TEST_CASE(pool_freelist_ut)
{
	using namespace geek;

	freelist_policy_st<my_obj> alloc(1);
	freelist<my_obj, freelist_policy_st<my_obj> > freelist(alloc);

	my_obj* t1 = freelist.construct();
	BOOST_CHECK(t1->uint_ == 0);
	BOOST_CHECK(t1->int_ == -1);
	freelist.destruct(t1);

	my_obj* t2 = freelist.construct<int>(1);
	BOOST_CHECK(t2->uint_ == 0);
	BOOST_CHECK(t2->int_ == 1);
	freelist.destruct(t2);

	my_obj* t3 = new (freelist.allocate()) my_obj(2);
	BOOST_CHECK(t3->uint_ == 0);
	BOOST_CHECK(t3->int_ == 2);
	freelist.destruct(t3);

	// array test
	my_obj* ta1 = freelist.construct(4);
	for (std::size_t i=0; i<4; ++i)
	{
		BOOST_CHECK(ta1[i].uint_ == 0);
		BOOST_CHECK(ta1[i].int_ == -1);
	}
	freelist.destruct(ta1, 4);

	my_obj* ta2 = freelist.construct<int>(1, 3);
	for (std::size_t i=0; i<3; ++i)
	{
		BOOST_CHECK(ta2[i].uint_ == 0);
		BOOST_CHECK(ta2[i].int_ == 1);
	}
	freelist.destruct(ta2, 3);

	GEEK_LOG << "done.\n" << GEEK_NORMAL;
}

BOOST_AUTO_TEST_CASE(pool_freelist_mt_ut)
{
	using namespace geek;

	freelist_policy_mt<my_obj> alloc(1);
	freelist<my_obj, freelist_policy_mt<my_obj> > freelist(alloc);

	my_obj* t1 = freelist.construct();
	BOOST_CHECK(t1->uint_ == 0);
	BOOST_CHECK(t1->int_ == -1);
	freelist.destruct(t1);

	my_obj* t2 = freelist.construct<int>(1);
	BOOST_CHECK(t2->uint_ == 0);
	BOOST_CHECK(t2->int_ == 1);
	freelist.destruct(t2);

	my_obj* t3 = new (freelist.allocate()) my_obj(2);
	BOOST_CHECK(t3->uint_ == 0);
	BOOST_CHECK(t3->int_ == 2);
	freelist.destruct(t3);

	// array test
	my_obj* ta1 = freelist.construct(4);
	for (std::size_t i=0; i<4; ++i)
	{
		BOOST_CHECK(ta1[i].uint_ == 0);
		BOOST_CHECK(ta1[i].int_ == -1);
	}
	freelist.destruct(ta1, 4);

	my_obj* ta2 = freelist.construct<int>(1, 3);
	for (std::size_t i=0; i<3; ++i)
	{
		BOOST_CHECK(ta2[i].uint_ == 0);
		BOOST_CHECK(ta2[i].int_ == 1);
	}
	freelist.destruct(ta2, 3);

	GEEK_LOG << "done.\n" << GEEK_NORMAL;
}

BOOST_AUTO_TEST_CASE(pool_std_and_freelist_st_ut)
{
	using namespace geek;

	std::vector<test_t*> test_array(TEST_COUNT);

	test_clock_t::duration dt1;
	test_clock_t::duration dt2;

	// std
	{
		std::allocator<test_t> alloc;
		GEEK_LOG << "testing... " << GEEK_NORMAL;
		test_clock_t::time_point start = test_clock_t::now();
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = alloc.allocate(1);
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			alloc.deallocate(test_array[i], 1);
		}
		dt1 = test_clock_t::now() - start;
		GEEK_LOG << "std alloc time: " << dt1.count() << GEEK_NORMAL;
	}

	// freelist st
	{
		geek::freelist_policy_st<test_t> alloc(TEST_COUNT);
		GEEK_LOG << "testing... " << GEEK_NORMAL;
		test_clock_t::time_point start = test_clock_t::now();
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = alloc.allocate();
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			alloc.deallocate(test_array[i]);
		}
		dt2 = test_clock_t::now() - start;
		GEEK_LOG << "freelist st alloc time: " << dt2.count() << GEEK_NORMAL;
	}

	GEEK_LOG << "compare, dt1 / dt2: " << (double)dt1.count()/(double)dt2.count() << "\n" << GEEK_NORMAL;
}

BOOST_AUTO_TEST_CASE(pool_boost_pool_and_freelist_st_ut)
{
	using namespace geek;

	std::vector<test_t*> test_array(TEST_COUNT);

	test_clock_t::duration dt1;
	test_clock_t::duration dt2;

	// boost pool
	{
		boost::pool<> pool(sizeof(test_t));
		GEEK_LOG << "testing... " << GEEK_NORMAL;
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = (test_t*)pool.malloc();
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			pool.free(test_array[i]);
		}

		test_clock_t::time_point start = test_clock_t::now();
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = (test_t*)pool.malloc();
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			pool.free(test_array[i]);
		}
		dt1 = test_clock_t::now() - start;
		GEEK_LOG << "boost pool alloc time: " << dt1.count() << GEEK_NORMAL;
	}

	// freelist st
	{
		freelist_policy_st<test_t> alloc(TEST_COUNT);

		GEEK_LOG << "testing... " << GEEK_NORMAL;
		test_clock_t::time_point start = test_clock_t::now();
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = alloc.allocate();
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			alloc.deallocate(test_array[i]);
		}
		dt2 = test_clock_t::now() - start;
		GEEK_LOG << "freelist st alloc time: " << dt2.count() << GEEK_NORMAL;
	}

	GEEK_LOG << "compare, dt1 / dt2: " << (double)dt1.count()/(double)dt2.count() << "\n" << GEEK_NORMAL;
}


void test_std_mt(std::allocator<test_t>& alloc, boost::barrier& bar)
{
	std::vector<test_t*> test_array(TEST_COUNT);

	bar.wait();

	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		test_array[i] = (test_t*)alloc.allocate(1);
	}
	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		alloc.deallocate(test_array[i], 1);
	}
}


struct my_pool_tag {};

void test_boost_pool_mt(boost::barrier& bar)
{
	std::vector<test_t*> test_array(TEST_COUNT);
	
	typedef boost::singleton_pool<my_pool_tag, sizeof(test_t)> my_pool_t;

	bar.wait();

	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		test_array[i] = (test_t*)my_pool_t::malloc();
	}
	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		my_pool_t::free(test_array[i]);
	}
}

void test_freelist_mt(geek::freelist_policy_mt<test_t>& alloc, boost::barrier& bar)
{ 
	std::vector<test_t*> test_array(TEST_COUNT);

	bar.wait();

	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		test_array[i] = alloc.allocate();
	}
	for (std::size_t i=0; i<TEST_PRE_THREAD_COUNT; ++i)
	{
		alloc.deallocate(test_array[i]);
	}
}

BOOST_AUTO_TEST_CASE(pool_boost_pool_and_freelist_mt_ut)
{
	using namespace geek;

	test_clock_t::duration dt1;
	test_clock_t::duration dt2;

	// boost pool
	{
		std::vector<test_t*> test_array(TEST_COUNT);

		GEEK_LOG << "testing... " << GEEK_NORMAL;
		typedef boost::singleton_pool<my_pool_tag, sizeof(test_t)> my_pool_t;
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			test_array[i] = (test_t*)my_pool_t::malloc();
		}
		for (std::size_t i=0; i<TEST_COUNT; ++i)
		{
			my_pool_t::free(test_array[i]);
		}

		boost::barrier bar(TEST_THREAD_COUNT + 1);
		boost::thread_group thread_pool;
		for (std::size_t i=0; i<TEST_THREAD_COUNT; ++i)
		{
			thread_pool.create_thread(boost::bind(&test_boost_pool_mt, boost::ref(bar)));
		}

		bar.wait();

		test_clock_t::time_point start = test_clock_t::now();
		thread_pool.join_all();
		dt1 = test_clock_t::now() - start;
		GEEK_LOG << "boost pool mt alloc time: " << dt1.count() << GEEK_NORMAL;
	}

	// freelist mt
	{
		freelist_policy_mt<test_t> alloc(TEST_THREAD_COUNT, 2, TEST_COUNT);
		
		boost::barrier bar(TEST_THREAD_COUNT + 1);
		boost::thread_group thread_pool;
		for (std::size_t i=0; i<TEST_THREAD_COUNT; ++i)
		{
			thread_pool.create_thread(boost::bind(&test_freelist_mt, boost::ref(alloc), boost::ref(bar)));
		}

		GEEK_LOG << "testing... " << GEEK_NORMAL;
		bar.wait();

		test_clock_t::time_point start = test_clock_t::now();
		thread_pool.join_all();
		dt2 = test_clock_t::now() - start;
		GEEK_LOG << "freelist mt alloc time: " << dt2.count() << GEEK_NORMAL;
	}

	GEEK_LOG << "compare, dt1 / dt2: " << (double)dt1.count()/(double)dt2.count() << "\n" << GEEK_NORMAL;
}


BOOST_AUTO_TEST_CASE(pool_std_and_freelist_mt_ut)
{
	using namespace geek;

	test_clock_t::duration dt1;
	test_clock_t::duration dt2;

	// boost pool
	{
		std::vector<test_t*> test_array(TEST_COUNT);

		GEEK_LOG << "testing... " << GEEK_NORMAL;
		
		std::allocator<test_t> alloc;
		
		boost::barrier bar(TEST_THREAD_COUNT + 1);
		boost::thread_group thread_pool;
		for (std::size_t i=0; i<TEST_THREAD_COUNT; ++i)
		{
			thread_pool.create_thread(boost::bind(&test_std_mt, boost::ref(alloc), boost::ref(bar)));
		}

		bar.wait();

		test_clock_t::time_point start = test_clock_t::now();
		thread_pool.join_all();
		dt1 = test_clock_t::now() - start;
		GEEK_LOG << "std mt alloc time: " << dt1.count() << GEEK_NORMAL;
	}

	// freelist mt
	{
		freelist_policy_mt<test_t> alloc(TEST_THREAD_COUNT, 2, TEST_COUNT);
		
		boost::barrier bar(TEST_THREAD_COUNT + 1);
		boost::thread_group thread_pool;
		for (std::size_t i=0; i<TEST_THREAD_COUNT; ++i)
		{
			thread_pool.create_thread(boost::bind(&test_freelist_mt, boost::ref(alloc), boost::ref(bar)));
		}

		GEEK_LOG << "testing... " << GEEK_NORMAL;
		bar.wait();

		test_clock_t::time_point start = test_clock_t::now();
		thread_pool.join_all();
		dt2 = test_clock_t::now() - start;
		GEEK_LOG << "freelist mt alloc time: " << dt2.count() << GEEK_NORMAL;
	}

	GEEK_LOG << "compare, dt1 / dt2: " << (double)dt1.count()/(double)dt2.count() << "\n" << GEEK_NORMAL;
}

#endif// GEEK_TOOLKITS_TEST_POOL_TEST_HPP
