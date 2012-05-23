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
class foo:public geek::singleton<foo>
{
public:
	virtual void print(void)
	{

		GEEK_LOG<<"class foo"<<GEEK_NORMAL;
	}
};

template<> foo* geek::singleton<foo>::singleton_ = NULL;

class foo_derived:public foo
{
public:
	virtual void print(void)
	{
		GEEK_LOG<<"class foo_derived"<<GEEK_NORMAL;
	}
	static foo_derived& get_singleton(void)
	{
		return static_cast<foo_derived&>(foo::get_singleton());
	}
	static foo_derived* get_singleton_ptr(void)
	{
		return static_cast<foo_derived*>(foo::get_singleton_ptr());
	}
};


int main(void)
{
	try{
		foo* o = new foo();
		{
			foo::get_singleton().print();
			assert(foo::get_singleton_ptr() == o);
		}
		delete o;



		o = new foo_derived();
		{
			foo_derived::get_singleton().print();
			assert(foo_derived::get_singleton_ptr() == o);
		}
		delete o;


		
	}
	catch(std::exception& e)
	{
		GEEK_LOG<<e.what()<<GEEK_ERROR;
	}

}
