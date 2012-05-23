/*
-----------------------------------------------------------------------------
This source file is a part of geek
(Game Engine Extensible Kit)
For the latest info, see http://gdgeek.com/

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

#ifdef _ENABLE_SCRIPT
#include <geek/view_cegui/cegui_lua_module.hpp>
#include <CEGUI/ScriptingModules/LuaScriptModule/CEGUILua.h>
#include <CEGUI/cegui.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void destroy_lua_module(CEGUI::LuaScriptModule * module)                 //¹Ø±Õsocket  
{  
	CEGUI::LuaScriptModule::destroy(*module);
} 
using namespace geek;

cegui_lua_module::cegui_lua_module(lua_State* state):state_(state)
{

}
cegui_lua_module::~cegui_lua_module(void)
{


}
bool cegui_lua_module::init(void)
{
	using namespace CEGUI;
	LuaScriptModule * module = &LuaScriptModule::create(state_);
	script_module_.reset(module, destroy_lua_module);
	CEGUI::System::getSingleton().setScriptingModule(module);
	return true;
}
void cegui_lua_module::shutdown(void)
{

	CEGUI::System::getSingleton().setScriptingModule(NULL);
	script_module_.reset();
}
#endif