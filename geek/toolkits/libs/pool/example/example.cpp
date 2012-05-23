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
#include "example.hpp"
#include <geek/toolkits/log_system.hpp>

struct uintish
{
	unsigned int value;
	uintish() : value(5) { }
	uintish(const uintish &o) : value(o.value) { }
};

class my_object
{
public:
	my_object() {}
	~my_object() {}

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

private:
	unsigned int uint_;
	int int_;
};

typedef my_object test_t;

int main(void)
{
	using namespace geek;

	try{
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

		freelist_policy_st<test_t> alloc(10);

		test_t* o = alloc.allocate();
		alloc.deallocate(o);

		freelist_policy_st<my_obj> alloc2(1);
		freelist<my_obj, freelist_policy_st<my_obj> > freelist(alloc2);

		my_obj* t1 = freelist.construct();
		t1->print();
		freelist.destruct(t1);

		int i = 1;
		my_obj* t2 = freelist.construct<int>(1);
		t2->print();
		freelist.destruct(t2);

		my_obj* t3 = new (freelist.allocate()) my_obj(2);
		t3->print();
		freelist.destruct(t3);
	}
	catch(std::exception & e)
	{
		std::cout<<e.what();
	}

}
