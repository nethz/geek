/*----------------------------------------------------------------------------
LUA SCRIPT.CPP
Object to manage a single Lua thread/coroutine script.

contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include "stable_headers.hpp"
#include <geek/view_lua/config.hpp>
#include <geek/view_lua/lua_script.hpp>
#include <geek/view_lua/lua_manager.hpp>

#include    <stdio.h>
#include    <string.h>
#include    <assert.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace geek;

/*----------------------------------------------------------------------------
CONSTRUCTOR
*/


geek::auto_list<lua_script>::list_type geek::auto_list<lua_script>::list_;
lua_script::lua_script(lua_manager* mgr)
{
	manager_             = mgr;
	state_               = LSS_NOTLOADED;
	time_             = 0;
	last_error_string_ =  "No error.\n";

	// create a thread/_state for this object
	thread_state_ = lua_newthread(manager_->master_state_);

	// save a pointer to the thread _manager object in the global table
	// using the new thread's vm pointer as a key
	//lua_pushthread(thread_state_);	
	lua_pushlightuserdata(manager_->master_state_, thread_state_);
	lua_pushlightuserdata(manager_->master_state_, this );
	lua_settable(manager_->master_state_, LUA_GLOBALSINDEX );
}


/*----------------------------------------------------------------------------
DESTRUCTOR
*/

lua_script::~lua_script()
{
	// Unhook from the script _manager
	//_manager->UnlinkScript(this);
	//lua_pop(_threadState, 3);
	unlink();
	//lua_close(_threadState);
}


/*----------------------------------------------------------------------------
RUN FILE
*/

void lua_script::run_file(std::string const& file_name)
{
	int         status;

	assert(manager_->master_state_);
	assert(thread_state_);

	status = luaL_loadfile(thread_state_, file_name.c_str());
	if (status == 0)
	{
		resume_script(0.0f);
	}
	else 
	{
		format_error();
		output_error("Syntax Error");
	}
}


/*----------------------------------------------------------------------------
RUN STRING
*/

int lua_script::run_string(std::string const& command_string)
{
	int         status =0;

	assert(manager_->master_state_);
	assert(thread_state_);

	status = luaL_loadbuffer(thread_state_, command_string.c_str(), command_string.size(), "Console");
	if (status == 0)
	{
		status = lua_pcall(thread_state_, lua_gettop(thread_state_)-2, 0, 0);
	}

	if (status)
	{
		format_error();
		output_error("Runtime Error:");
		return(-1);
	}

	return(0);
}


/*----------------------------------------------------------------------------
UPDATE
The purpose of update is basically to check if any suspended scripts need
to be turned back on again!

Returns the _next script in the list.  This assists in linked list traversal
where this script may delete it's _next sibling.

*/

bool lua_script::update(float elapsed_sec)
{
	time_ += elapsed_sec;
	switch(state_)
	{
	case LSS_WAITTIME:
		if (time_ >= wait_timestamp_)
			resume_script(0.0f);
		break;
	case LSS_WAITFRAME:
		wait_frame_--;
		if (wait_frame_ <= 0)
			resume_script(0.0f);
		break;
	case LSS_NOTLOADED:
		break;
		//        case LSS_DONE:
	default :
		// if we aren't waiting and we get here, the script must be done
		//            if (autoDelete)
		//                {
		//                lua_script*  sNext;
		//
		//                sNext = _next;
		//                delete(this);
		//                return(sNext);
		//                }
		break;
	}
	return true;
	//return(_next);
}


/*----------------------------------------------------------------------------
ABORT WAIT
Resumes esecution of the script, pushing 1 as a return value from the
call that suspended the script.
*/

void lua_script::abort_wait()
{
	resume_script(1.0f);
}


/*----------------------------------------------------------------------------
RESUME SCRIPT
Runs or resumes a LUA script.  Executes until a yeild is encountered or
the script terminates.

The parameter to this function is pushed on the stack as the return value
of the previous yield.
*/

void lua_script::resume_script(float param)
{
	int         status;

	// we're about to run/resume the thread, so set the global
	state_  = LSS_RUNNING;

	// param is treated as a return value from the function that yielded
	lua_pushnumber(thread_state_, param);

	status = lua_resume(thread_state_, 1);

	if(status == LUA_YIELD)
	{
		 //int r = luaL_checknumber( thread_state_, -1 );
	}else if (status != 0)
	{
		format_error();
		output_error("Runtime Error:");
	}
}


/*----------------------------------------------------------------------------
CALL FN
Calls a Lua function.  Shows how to pass a single parameter, but not how
to retrieve return values.
*/

void lua_script::call_fun( const std::string & fn_name, int i_param)
{
	int         status;

	// find the lua function and push it on the stack
	lua_getglobal (thread_state_, fn_name.c_str());

	// push our single argument
	lua_pushnumber(thread_state_, i_param);

	// now, call into Lua
	status = lua_pcall(thread_state_, 1, 0, 0);

	if (status)
	{
		format_error();
		output_error("Runtime Error:");
	}
}


/*----------------------------------------------------------------------------
FORMAT ERROR
Grabs the last error message and stuffs it in lastErrorString.
*/

void lua_script::format_error()
{
	char const* msg;

	msg = lua_tostring(thread_state_, -1);
	if (msg == NULL)
		msg = "(error with no message)";
	lua_pop(thread_state_, 1);
	last_error_string_ = msg;
}


/*----------------------------------------------------------------------------
OUTPUT ERROR
*/

void lua_script::output_error(std::string const& str_type)
{
	std::string err_msg = (boost::format("Lua Manager : %s %s ")%str_type%last_error_string_).str();
	GEEK_LOG_ERROR_MESSAGE(err_msg);
	GEEK_THROW_EXCEPTION(err_internal_error, err_msg);
}