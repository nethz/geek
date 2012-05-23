/*----------------------------------------------------------------------------
LUA SCRIPT LIB.C
Small library of Lua glue routines to support script management.

contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/
#include "stable_headers.hpp"
#include <geek/view_lua/config.hpp>
#include <geek/view_lua/lua_script.hpp>
#include  <geek/view_lua/lua_manager.hpp>

#include    <windows.h>     // just for Beep()
#include    <stdio.h>
#include    <string.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include    "lua_script_lib.hpp"

using namespace geek;

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Public Prototypes                             */    
/*--------------------------------------------------------------------------*/

static lua_script*  get_script_object(lua_State* l);
static int          lua_beep        (lua_State* l);
static int          lua_wait_frame   (lua_State* l);
static int          lua_wait_time    (lua_State* l);
static int          lua_wait_sec     (lua_State* l);


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/

// table of function names and entry points that will be registered with Lua
static const luaL_reg script_lib[] =
{
	{"beep",         lua_beep},
	{"wait_frame",    lua_wait_frame},
	{"wait_time",     lua_wait_time},
	{"wait_sec",      lua_wait_sec},
	{NULL, NULL}
};


/*----------------------------------------------------------------------------
LUA OPEN SCRIPT LIB
Registers all the basic script support functions with Lua.
*/

void lua_open_script_lib(lua_State* l)
{
	// register our library of routines under the table name Script
	luaL_openlib(l, "script", script_lib, 0);
}


/*----------------------------------------------------------------------------
LUA BEEP
Expose the windows Beep() function.  Grab frequency and duration from
the stack.
*/

static int lua_beep(lua_State* l)
{
	Beep((int)luaL_checknumber(l, 1), (int)luaL_checknumber(l, 2));
	return(0);
}


/*----------------------------------------------------------------------------
LUA WAIT FRAME
Causes a script to wait for the specified number of frames.
*/

static int lua_wait_frame(lua_State* l)
{
	lua_script*  s;

	s = get_script_object(l);

	s->wait_frame_ = (int)luaL_checknumber(l, 1);
	s->state_     = LSS_WAITFRAME;

	return(lua_yield(l, 1));
}


/*----------------------------------------------------------------------------
LUA WAIT TIME
Causes a script to wait until the given timestamp.
*/

static int lua_wait_time(lua_State* l)
{
	lua_script*  s;

	s = get_script_object(l);

	s->wait_timestamp_ = (float)luaL_checknumber(l, 1);
	s->state_         = LSS_WAITTIME;

	return(lua_yield(l, 1));
}


/*----------------------------------------------------------------------------
LUA WAIT SEC
Causes a script to wait for the given number of seconds.
*/

static int lua_wait_sec(lua_State* l)
{
	lua_script*  s;

	s = get_script_object(l);

	s->wait_timestamp_ = s->time_ + (float)luaL_checknumber(l, 1);
	s->state_         = LSS_WAITTIME;

	return(lua_yield(l, 1));
}


/*----------------------------------------------------------------------------
GET SCRIPT OBJECT
Retrieves the address of the C++ object managing the given lua_State.
*/

static lua_script* get_script_object(lua_State* l)
{
	lua_pushlightuserdata(l, l);
	lua_gettable(l, LUA_GLOBALSINDEX );
	return((lua_script*)lua_touserdata(l, -1));
}
