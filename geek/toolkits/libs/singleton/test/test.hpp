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


#ifndef GEEK_TOOLKITS_TEST_SINGLETON_TEST_HPP
#define GEEK_TOOLKITS_TEST_SINGLETON_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 


#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>
#include <geek/toolkits/log_system.hpp>
// 
// template<class T>
// class the_test2: public geek::singleton<the_test2<T> >
// {
// public:
// 	inline the_test2(T* obj):obj_(obj)
// 	{
// 
// 	}
// 	virtual ~the_test2(void)
// 	{
// 
// 	}
// 	T& obj(void) const
// 	{
// 		return *obj_ptr();
// 	}
// 
// 	T* obj_ptr(void) const
// 	{
// 		return obj_.get();
// 	}
// private:
// 	boost::scoped_ptr<T> obj_;
// };
class object
{
public:
	
	void print(void) const
	{
		GEEK_LOG<<"object"<<GEEK_NORMAL;

	}
private:


};


class the_test:public geek::singleton<the_test>
{
public:
	the_test(void);
	virtual ~the_test(void);
	void print(void) const;
};



BOOST_AUTO_TEST_CASE(singleton_ut)
{
	using namespace geek;
	the_test* t = new the_test();

	the_test::get_singleton().print();
	BOOST_CHECK_EQUAL(t , the_test::get_singleton_ptr());
	delete t;

	singleton_non_invasive<object>* t2 = new singleton_non_invasive<object>(new object());

	singleton_non_invasive<object>::get_singleton().obj().print();

	delete t2;
}


#endif// GEEK_TOOLKITS_TEST_SINGLETON_TEST_HPP
