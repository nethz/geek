﻿/*
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
#include <boost/foreach.hpp>
#include <geek/toolkits/component/component_interface_query.hpp>
#include <geek/toolkits/component/component_interface.hpp>
#include <geek/toolkits/component/component.hpp>

#include <geek/toolkits/pool/stl_containter.hpp>

using namespace geek;







component_interface_query::component_interface_query(void)
{
}
component_interface_query::~component_interface_query(void)
{
}

void component_interface_query::add(component * component)
{
	components_.push_back(component);
}
void component_interface_query::add(component_interface * c_interface)
{
	interfaces_.push_back(c_interface);
}

component_interface * component_interface_query::query_interface(std::type_info const& info)
{
	
		BOOST_FOREACH(component_interface* i, interfaces_)
		{
			if(info  == typeid(*i))
				return (i);
		}
		

		BOOST_FOREACH(component* c, components_)
		{
			component_interface* c_interface  = c->_query_interface(info);
			if(c_interface)
				return c_interface;
		}
	
	return NULL;
}
