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
#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#include <windows.h>
#include <geek/view_lua/lua_component_framework.hpp>
#endif //GEEK_PLATFORM == GEEK_PLATFORM_WIN32

using namespace geek;




#ifdef _ENABLE_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(void)
#endif
{
	boost::scoped_ptr<system_by_functor<> > system(new system_by_functor<>());

	boost::shared_ptr<key_value_server> kvs_ptr(new key_value_server());
	system->push_obj(kvs_ptr);


	boost::shared_ptr<plugins_manager> pm_ptr(new plugins_manager());
	system->push_obj(pm_ptr)
		.add_init(boost::bind(&plugins_manager::init, pm_ptr.get()))
		.add_shutdown(boost::bind(&plugins_manager::shutdown, pm_ptr.get()));

	boost::shared_ptr<tick_timer_manager> tm_ptr(new tick_timer_manager());
	system->push_obj(tm_ptr)
		.add_update(boost::bind(&tick_timer_manager::update, tm_ptr.get(), _1));


	boost::shared_ptr<ogre_graphics_manager> ogre_ptr(new ogre_graphics_manager());
	system->push_obj(ogre_ptr)
		.add_update(boost::bind(&ogre_graphics_manager::update, ogre_ptr.get(), _1))
		.add_init(boost::bind(&ogre_graphics_manager::init, ogre_ptr.get()))
		.add_shutdown(boost::bind(&ogre_graphics_manager::shutdown, ogre_ptr.get()));


	boost::shared_ptr<ois_input_manager> ois_ptr(new ois_input_manager());
	system->push_obj(ois_ptr)
		.add_update(boost::bind(&ois_input_manager::update, ois_ptr.get(), _1))
		.add_init(boost::bind(&ois_input_manager::init, ois_ptr.get()))
		.add_shutdown(boost::bind(&ois_input_manager::shutdown, ois_ptr.get()));



	boost::shared_ptr<cegui_manager> cegui_ptr(new cegui_manager());
	system->push_obj(cegui_ptr)
		.add_update(boost::bind(&cegui_manager::update, cegui_ptr.get(), _1))
		.add_init(boost::bind(&cegui_manager::init, cegui_ptr.get()))
		.add_shutdown(boost::bind(&cegui_manager::shutdown, cegui_ptr.get()));


	
	boost::shared_ptr<lua_manager> lua_ptr(new lua_manager());
	system->push_obj(lua_ptr)
		.add_update(boost::bind(&lua_manager::update, lua_ptr.get(), _1))
 		.add_init(boost::bind(&lua_manager::init, lua_ptr.get()))
		.add_shutdown(boost::bind(&lua_manager::shutdown, lua_ptr.get()));


	boost::shared_ptr<lua_component_framework> lcf_ptr(new lua_component_framework("runme.lua"));
	system->push_obj(lcf_ptr)
		.add_update(boost::bind(&lua_component_framework::update, lcf_ptr.get(), _1))
		.add_init(boost::bind(&lua_component_framework::init, lcf_ptr.get()))
		.add_shutdown(boost::bind(&lua_component_framework::shutdown, lcf_ptr.get()));





	boost::shared_ptr<cegui_lua_module> clm_ptr(new cegui_lua_module());
	system->push_obj(clm_ptr)
 		.add_init(boost::bind(&cegui_lua_module::init, clm_ptr.get()))
 		.add_shutdown(boost::bind(&cegui_lua_module::shutdown, clm_ptr.get()));


	
	
#ifdef _DEBUG
	plugins_manager::get_singleton().load_plugin(std::string("geek_script_component") + _DEBUG_POSTFIX);
#else 
	plugins_manager::get_singleton().load_plugin("geek_script_component");
#endif
	
	
	tick_timer_manager::get_singleton().delay(boost::bind(&system_interface::exit, system_interface::get_singleton_ptr()), boost::chrono::seconds(30));
	if(system->init())
	{
		
		system->run();
		system->shutdown();
	}
	return 0;
}
