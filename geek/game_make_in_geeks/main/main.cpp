a/*
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
#include <geek/framework/system.hpp>
#include <geek/toolkits/timer_manager/tick_timer_manager.hpp>
#include <geek/toolkits/dyn_lib_manager.hpp>
#include <geek/toolkits/concurrent/concurrent.hpp>
#include <geek/toolkits/key_value_server.hpp>
#include <geek/view_ogre/ogre_graphics_manager.hpp>
#include <geek/view_ois/ois_input_manager.hpp>
#include <geek/view_cegui/cegui_manager.hpp>
#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#include <windows.h>
#endif //GEEK_PLATFORM == GEEK_PLATFORM_WIN32

using namespace geek;

#ifdef _ENABLE_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(void)
#endif
{
	boost::scoped_ptr<system_by_functor<> > system(new system_by_functor<>());

//	boost::shared_ptr<concurrent> c
 	boost::shared_ptr<key_value_server> kvs_ptr(new key_value_server());
 	system->push_obj(kvs_ptr);

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



	
	tick_timer_manager::get_singleton().delay(boost::bind(&system_interface::exit, system_interface::get_singleton_ptr()), boost::chrono::seconds(30));
	if(system->init())
	{
		system->run();
		system->shutdown();
	}
// 	system->push<timer_manager, false, true, false>();
// 	system->push<plugins_manager, true, true, true>();
// 	system->push<CEGUIManager, true, true, true>();
// 	system->push<InputUpdateInOgre, true, false, true>();
// 	system->push<PluginsManager, true, false, true>();
// 	system->push<ComponentUpdate, false, true, false>();
	return 0;
}
