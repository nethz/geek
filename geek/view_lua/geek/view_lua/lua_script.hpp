
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
/// @file       lua_script.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/06
///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------
LUA SCRIPT
Class to manage a single Lua thread/_state/script.

contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef GEEK_VIEW_LUA_LUA_SCRIPT_HPP
#define GEEK_VIEW_LUA_LUA_SCRIPT_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/view_lua/config.hpp>
#include <geek/toolkits/auto_list.hpp>
/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

typedef enum lua_script_state_tag
{
	LSS_WAITFRAME,
	LSS_WAITTIME,
	LSS_RUNNING,
	LSS_NOTLOADED,
	LSS_DONE
} LUASCRIPTSTATE;


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

typedef struct lua_State lua_State;
namespace geek
{
class lua_manager;
class lua_script;
typedef boost::shared_ptr<lua_script> lua_script_ptr;
class GEEK_VIEW_LUA_DECL lua_script:public geek::auto_list<lua_script>
{
public:                         // ---------------------------------------

	lua_manager * manager_;            // lua_manager object that created this script
	LUASCRIPTSTATE  state_;              // are we waiting, not loaded yet, etc.
	float wait_timestamp_;      // _time to wake up
	int wait_frame_;          // number of frames to wait
	float time_;               // current _time

	lua_script(lua_manager* mgr);
	~lua_script(void);
	bool update(float elapsed_sec);
	void  run_file(std::string const& file_nme);
	int run_string(std::string const& command_string);
	void call_fun (std::string const& fn_name, int i_param);
	void abort_wait(void);

private:                        // ---------------------------------------

	lua_State* thread_state_;
	std::string last_error_string_;//[256];
	void resume_script(float param);
	void format_error(void);
	void output_error(std::string const& str_type);

};

}
/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif // GEEK_VIEW_LUA_LUA_SCRIPT_HPP

