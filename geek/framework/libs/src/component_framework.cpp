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
#include <geek/framework/component_framework.hpp>

using namespace geek;

component_framework::component_framework(std::string const& main_component_name, duration_type step_time): 
main_component_name_(main_component_name),
game_interface_(NULL),
accumulator_(boost::chrono::seconds(0)),
step_time_(step_time)
{
}
component_framework::~component_framework(void)
{

}
bool component_framework::init(void)	
{
	main_component_ = geek::component_factories::get_instance().create(main_component_name_);
	if(!main_component_)
	{
		GEEK_LOG_ERROR_MESSAGE("can't find component type :"+ main_component_name_);
		GEEK_THROW_EXCEPTION(err_item_not_found, "can't find component type :"+ main_component_name_);
		return false;
	}
	game_interface_ = main_component_->query_interface<game_interface>();

	if(!game_interface_)
	{
		GEEK_LOG_ERROR_MESSAGE("can't find query interface type : game_interface, in :"+ main_component_name_);
		GEEK_THROW_EXCEPTION(err_item_not_found, "can't find query interface type : game_interface, in :"+ main_component_name_);
		return false;
	}

	accumulator_ = boost::chrono::seconds(0);

	return game_interface_->init();
}
void component_framework::update(duration_type const& interval)
{
	accumulator_ = accumulator_ + interval;

	unsigned int step = unsigned int(accumulator_ /step_time_);

	if(step > 0)
	{
		game_interface_->update(step);
		accumulator_ -= (step * step_time_);
	}
}
void component_framework::shutdown(void)
{
	game_interface_->shutdown();
}