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
/// @file       lua_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/05
///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------
LUA MANAGER.H
Manages the single, master Lua _state.

contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

// signal header has been included


#ifndef GEEK_VIEW_LUA_SYSTEM_INTERFACE_HPP
#define GEEK_VIEW_LUA_SYSTEM_INTERFACE_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/view_lua/config.hpp>

#include <geek/view_lua/lua_script.hpp>
#include <geek/framework/system_interface.hpp>
/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/



typedef struct lua_State lua_State;
namespace geek
{
	
	class lua_script;
	typedef boost::shared_ptr<lua_script> lua_script_ptr;
	class GEEK_VIEW_LUA_DECL lua_manager: public geek::singleton<lua_manager>
	{

	public: //orz interface
		bool init(void);
		void shutdown(void);
		void update(duration_type const& interval);
		lua_State* get_lua_state(void);
	public:                         // ---------------------------------------
		lua_State*  master_state_;

		lua_manager(void);
		~lua_manager(void);

		lua_script_ptr  create_script(void);
		int num_scripts(void);

	private:                        // ---------------------------------------
		// lua_script *  _head;


	};



}
/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif // GEEK_VIEW_LUA_SYSTEM_INTERFACE_HPP
