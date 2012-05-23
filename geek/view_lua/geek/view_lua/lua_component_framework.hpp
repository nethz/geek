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
/// @file       lua_component_framework.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/06
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_LUA_LUA_COMPONENT_FRAMEWORK_HPP
#define GEEK_VIEW_LUA_LUA_COMPONENT_FRAMEWORK_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/view_lua/config.hpp>
#include <geek/view_lua/lua_script.hpp>
#include <geek/toolkits/component.hpp>
#include <geek/framework/game_interface.hpp>

namespace geek
{

	class GEEK_VIEW_LUA_DECL lua_component_framework
	{

	public: 
		lua_component_framework(std::string const& lua_game = "game.lua", duration_type step_time = boost::chrono::milliseconds(step_milliseconds));
		~lua_component_framework(void);
                     
			
		bool init(void);
		void update(duration_type const& interval);
		void shutdown(void);
	private:
		void on_destroy(component* object);
		void on_create(component* object);
		std::string const lua_game_; 
		lua_script_ptr lua_script_;


		boost::signals2::connection on_destroy_connection_;
		boost::signals2::connection on_create_connection_;
		typedef geek::vector<game_interface *>::type interface_list;
		interface_list interface_list_;
		duration_type accumulator_;
		duration_type step_time_;

	};



}
#endif // GEEK_VIEW_LUA_LUA_COMPONENT_FRAMEWORK_HPP
