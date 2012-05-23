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

////////////////////////////////////////////////////////////////////////
///  
/// @file       stable_headers.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_SCRIPT_EXECUTABLE_STABLE_HEADERS_HPP
#define GEEK_SCRIPT_EXECUTABLE_STABLE_HEADERS_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/config.hpp>
#include <geek/toolkits/singleton.hpp>


#include <geek/framework/system.hpp>
#include <geek/toolkits/timer_manager/tick_timer_manager.hpp>
#include <geek/toolkits/dyn_lib_manager.hpp>
#include <geek/toolkits/concurrent/concurrent.hpp>
#include <geek/toolkits/key_value_server.hpp>
#include <geek/view_ogre/ogre_graphics_manager.hpp>
#include <geek/view_ois/ois_input_manager.hpp>
#include <geek/view_cegui/cegui_manager.hpp>
#ifdef _ENABLE_SCRIPT
#	include <geek/view_cegui/cegui_lua_module.hpp>
#endif//_ENABLE_SCRIPT
#include <geek/toolkits/dyn_lib_manager.hpp>
#include <geek/view_lua/lua_manager.hpp>



#endif //GEEK_SCRIPT_EXECUTABLE_STABLE_HEADERS_HPP
