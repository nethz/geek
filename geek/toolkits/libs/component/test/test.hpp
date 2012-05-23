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


#ifndef GEEK_TOOLKITS_TEST_COMPONENT_TEST_HPP
#define GEEK_TOOLKITS_TEST_COMPONENT_TEST_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif 
#include <boost/functional/factory.hpp>
#include <geek/toolkits/log_system.hpp>
#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>
class test_interface:public geek::component_interface
{
public:
	boost::function<void(void)> print;
};

class pure_virtual_function: public geek::component_interface
{
public:
	virtual void print(void) = 0;
};
class component_test:public geek::component, public pure_virtual_function
{
public:
	component_test(void)
	{
		interface_.reset(new test_interface());
		interface_->print = boost::bind(&component_test::print, this);
		query_.add(interface_.get());
	}
private:
	void print(void)
	{
		GEEK_LOG<<"component_test"<<GEEK_NORMAL;
	}
	
	component_interface * _query_interface(std::type_info const& info)
	{
		if(info == typeid(pure_virtual_function))
		{
			return this;
		}
		return component::_query_interface(info);
	}
	boost::scoped_ptr<test_interface> interface_;

};

BOOST_AUTO_TEST_CASE(component_ut)
{
	using namespace geek;
	component_factories::get_instance().add_factory("unit_test", boost::factory<component_test*>());
	component_ptr comp = component_factories::get_instance().create("unit_test");
	BOOST_CHECK(comp);

	test_interface * ti = comp->query_interface<test_interface>();
	BOOST_CHECK(ti);
	ti->print();

	pure_virtual_function * ti2 = comp->query_interface<pure_virtual_function>();
	BOOST_CHECK(ti2);
	ti2->print();

}


#endif// GEEK_TOOLKITS_TEST_COMPONENT_TEST_HPP
