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

#include <geek/view_lua/config.hpp>
#include <geek/view_lua/lua_script.hpp>
#include <geek/view_lua/lua_manager.hpp>
#include <geek/framework/game_interface.hpp>
#include <geek/view_lua/lua_component_framework.hpp>


using namespace geek;
lua_component_framework::lua_component_framework(std::string const& lua_game, duration_type step_time):lua_game_(lua_game),step_time_(step_time)
{
	
}


void lua_component_framework::on_destroy(component* object)
{
	assert(object != NULL);

	GEEK_LOG_NORMAL_MESSAGE("void lua_component_framework::on_destroy(component* object)");
	geek::game_interface * the_interface = object->query_interface<geek::game_interface>();
	if(the_interface)
	{
		/*
		interface_list::iterator end_itr(interface_list_.end());
		interface_list::iterator itr(
			std::find(
			interface_list_.begin(),
			end_itr, the_interface
			)
			);

		if (itr != end_itr)
		{

			geek::game_interface*& curr = *itr;
			// swap and pop back vector to effective erase element
			geek::game_interface*& back = interface_list_.back(); // can't be empty, so this always success
			if (back != curr)
			{
				using std::swap;
				swap(back, curr); // swap always success in c++ standard
			}

			interface_list_.pop_back(); // pop back to erase and destroy plugin that swapped
		}

		*/

		interface_list_.erase(std::remove(interface_list_.begin(), interface_list_.end(), the_interface), interface_list_.end());
		the_interface->shutdown();
		GEEK_LOG_NORMAL_MESSAGE("the_interface->shutdown()");
	}

}
void lua_component_framework::on_create(component* object)
{	
	assert(object != NULL);
	game_interface * the_interface = object->query_interface<game_interface>();
	if(the_interface)
	{
		the_interface->init();
		interface_list_.push_back(the_interface);
	}
}
bool lua_component_framework::init(void)
{
	lua_script_ = lua_manager::get_singleton().create_script();
	on_create_connection_ = geek::component_factories::get_instance().on_component_create.connect(boost::bind(&lua_component_framework::on_create, this, _1));
	on_destroy_connection_ = geek::component_factories::get_instance().on_component_destroy.connect(boost::bind(&lua_component_framework::on_destroy, this, _1));

	accumulator_ = boost::chrono::seconds(0);

	lua_script_->run_file(lua_game_);
	return true;
}
void lua_component_framework::update(duration_type const& interval)
{
	accumulator_ = accumulator_ + interval;

	unsigned int step = unsigned int(accumulator_ /step_time_);

	if(step > 0)
	{
		BOOST_FOREACH(game_interface * the_interface, interface_list_)
		{
			the_interface->update(1);
		}
		accumulator_ -= (step * step_time_);
	}

	
}
void lua_component_framework::shutdown(void)
{
	GEEK_LOG_NORMAL_MESSAGE("void lua_component_framework::shutdown(void)");
	lua_script_.reset();
// 	on_destroy_connection_.disconnect();
// 	on_create_connection_.disconnect();

}

lua_component_framework::~lua_component_framework(void)
{


}
		
		
	