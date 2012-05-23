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
#include <geek/toolkits/component/component.hpp>
#include <geek/toolkits/component/component_factories.hpp>

#include <geek/toolkits/log_system.hpp>

using namespace geek;

component_factories::~component_factories(void)
{

}
component_factories::component_factories(void)
{
}

void component_factories::destroy(component* object)
{
	GEEK_LOG_NOTIFICATION_MESSAGE("void component_factories::destroy(component* object)");
	on_component_destroy(object);
	delete object;
}
component_ptr component_factories::create(std::string const& type_name)
{
	factory_map::iterator it = map_.find(type_name);
	if(it != map_.end())
	{
		component* object=  (it->second)();
		on_component_create(object);
		return component_ptr(object, boost::bind(&component_factories::destroy, this, _1));
	}
	return component_ptr();
}
component_factories& component_factories::add_factory(std::string const& name, const factory_type & factory)
{
	map_[name] = factory;
	return *this;
}

component_factories& component_factories::remove_factory(std::string const& type_name)
{
	map_.erase(type_name);
	return *this;
}
component_factories& component_factories::get_instance(void)
{
	return *(get_instance_ptr());
}
component_factories* component_factories::get_instance_ptr(void)
{
	static component_factories instance;
	return &instance;
}


