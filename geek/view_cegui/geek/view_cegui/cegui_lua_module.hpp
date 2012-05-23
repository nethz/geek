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
/// @file       cegui_lua_module.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/02
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_CEGUI_CEGUI_LUA_MODULE_HPP
#define GEEK_VIEW_CEGUI_CEGUI_LUA_MODULE_HPP


// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#ifdef _ENABLE_SCRIPT

#include <geek/view_cegui/config.hpp>
#include <geek/toolkits/singleton.hpp>

#include <geek/framework/system_interface.hpp>

namespace CEGUI
{
class LuaScriptModule;
}
typedef struct lua_State lua_State;
namespace geek
{



class GEEK_VIEW_CEGUI_DECL cegui_lua_module 
{

public:
	cegui_lua_module(lua_State* state = NULL);
	~cegui_lua_module(void);
	bool init(void);
	void shutdown(void);
private:
	lua_State * state_;
	boost::shared_ptr<CEGUI::LuaScriptModule> script_module_;
};



}
#endif //_ENABLE_SCRIPT
#endif // GEEK_VIEW_CEGUI_CEGUI_LUA_MODULE_HPP