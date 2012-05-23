/*----------------------------------------------------------------------------
lua_manager.H
Lua Script Manager.
contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/
#include "stable_headers.hpp"
#include <geek/view_lua/config.hpp>
#include <geek/view_lua/lua_script.hpp>
#include  <geek/view_lua/lua_manager.hpp>

#include    <stdio.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lua_script_lib.hpp"

using namespace geek;

template<> lua_manager* singleton<lua_manager>::singleton_ = NULL;

lua_State* lua_manager::get_lua_state(void)
{
	if(master_state_ == NULL)
	{
		master_state_ = lua_open();
	}
	return master_state_;
}
bool lua_manager::init(void)
{

	
	if (get_lua_state())
	{
		luaL_openlibs(master_state_);
		// open any auxiliary libaraies we may need
		//luaopen_base (_masterState); 
		//luaopen_io   (_masterState); 
		//luaopen_string  (_masterState); 
		//luaopen_math (_masterState); 
		//// including our own!
		lua_open_script_lib(master_state_);
	}


	return true;
}
void lua_manager::shutdown(void)
{

	lua_close(master_state_);
	lua_script::unlink_all();
	
}
/*----------------------------------------------------------------------------
CONSTRUCTOR
*/

lua_manager::lua_manager(void):master_state_(NULL)
{

	
}


/*----------------------------------------------------------------------------
DESTRUCTOR
Destroy all children scripts we are managing, and then close the Lua _state.
*/

lua_manager::~lua_manager()
{
}


/*----------------------------------------------------------------------------
CREATE SCRIPT
Creates a script object.
*/

lua_script_ptr lua_manager::create_script()  
{
	lua_script_ptr script(new lua_script(this));
	// create a script object
	script->link();

	return script;
}



/*----------------------------------------------------------------------------
UPDATE
Call every script's update routine.
For convenience, scripts return their child when Update is called.  This
helps when a script destroys itself during Update.
*/

void lua_manager::update(duration_type const& interval)
{
	for(lua_script::iterator it = lua_script::begin();
		it != lua_script::end();
		++it)
	{
		(*it)->update(duration_to_float_seconds(interval));//!!
	}
}