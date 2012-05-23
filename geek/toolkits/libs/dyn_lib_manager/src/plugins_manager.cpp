
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
#include "stable_headers.hpp"
#include <geek/toolkits/dyn_lib_manager/dyn_lib.hpp>
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/exception.hpp>
#include <geek/toolkits/dyn_lib_manager/plugins_manager.hpp>

#include "dyn_lib_impl.hpp"

#include <boost/foreach.hpp>

using namespace geek;

//------------------------------------------------------------------------------------------------- // 这个宽总共100列的注释是为了利于review代码，区分函数
typedef void (*dll_start_plugin)(void);
typedef void (*dll_init_plugin)(void);
typedef void (*dll_stop_plugin)(void);
typedef void (*dll_shutdown_plugin)(void);
//-------------------------------------------------------------------------------------------------
template<> plugins_manager* geek::singleton<plugins_manager>::singleton_ = NULL;
//-------------------------------------------------------------------------------------------------
plugins_manager::plugins_manager(void):is_initialised_(false)
{
}
//-------------------------------------------------------------------------------------------------
plugins_manager::~plugins_manager(void)
{
}
//-------------------------------------------------------------------------------------------------
bool plugins_manager::init(void)
{
	BOOST_FOREACH(dyn_lib_ptr& plugin, plugin_lib_list_)
	{
		try // we insure one plugin error no effect others
		{
			// Call plugin initialise (optional)
			dll_init_plugin func = (dll_init_plugin)plugin->get_symbol("dll_initialise_plugin");
			if (func)
			{
				func();
			}
		}
		catch(std::exception& ex)
		{
			GEEK_LOG_ERROR_MESSAGE(ex.what());
		}
		catch(...)
		{
			
			GEEK_LOG_ERROR_MESSAGE("can't init plugin: "+ plugin->get_name());
		}
	}

	is_initialised_ = true;
	return true;
}
//-------------------------------------------------------------------------------------------------
void plugins_manager::shutdown(void)
{
	// NB Shutdown plugins in reverse order to enforce dependencies
	BOOST_REVERSE_FOREACH(dyn_lib_ptr& plugin, plugin_lib_list_)
	{
		// Call plugin shutdown (optional)
		dll_shutdown_plugin func = (dll_shutdown_plugin)plugin->get_symbol("dll_shutdown_plugin");
		try // we insure one plugin error no effect others
		{
			if (func)
			{
				func();
			}
		}
		catch(std::exception& ex)
		{
			GEEK_LOG_ERROR_MESSAGE(ex.what());
		}
		catch(...)
		{
			GEEK_LOG_ERROR_MESSAGE("can't shutdown plugin: "+ plugin->get_name());
		}
	}

	// need clear() member function ?
	BOOST_REVERSE_FOREACH(dyn_lib_ptr& plugin, plugin_lib_list_)
	{
		// Call plugin shutdown
		dll_stop_plugin func = (dll_stop_plugin)plugin->get_symbol("dll_stop_plugin");
		try // we insure one plugin error no effect others
		{
			if(func)
			{
				func();
			}
		}
		catch(std::exception& ex)
		{
			GEEK_LOG_ERROR_MESSAGE(ex.what());
		}
		catch(...)
		{
			GEEK_LOG_ERROR_MESSAGE("can't stop plugin: "+ plugin->get_name());
		}
		// Unload library & destroy
		//!!(*i)->unload();
		plugin.reset();
		//DynLibManager::getSingleton().unload(*i);
	}

	plugin_lib_list_.clear();
	is_initialised_ = false;
}
//-------------------------------------------------------------------------------------------------
void plugins_manager::load_plugin_from_file(std::string const& file_name)
{
	std::ifstream file;
	file.open(file_name.c_str());
	if(!file.is_open())
	{
		GEEK_THROW_EXCEPTION(err_file_not_found, "can not open "+ file_name);
	}
	std::string plugin_name;
	while(!file.eof())
	{
		file >> plugin_name;
		try
		{
			load_plugin(plugin_name);
		}
		catch(std::exception& ex)
		{
			GEEK_LOG_ERROR_MESSAGE(ex.what());
		}
		catch(...)
		{
			GEEK_LOG_ERROR_MESSAGE("can't load plugin: "+ plugin_name);
		}
	}
	file.close();
}
//-------------------------------------------------------------------------------------------------
void plugins_manager::load_plugin(std::string const& plugin_name)
{
	GEEK_LOG_NORMAL_MESSAGE("Load plugin: " + plugin_name);

	// Load plugin library
	dyn_lib_ptr lib(new dyn_lib(plugin_name));
	// lib->load();
	// Store for later unload
	plugin_lib_list_.push_back(lib);

	// Call startup function
	dll_start_plugin start_func = (dll_start_plugin)lib->get_symbol("dll_start_plugin");

	if (!start_func)
	{
		GEEK_THROW_EXCEPTION(err_item_not_found, "can not found symbol: dll_start_plugin");
	}
	start_func();

	if (is_initialised_)
	{
		// initialise too
		dll_init_plugin init_func = (dll_init_plugin)lib->get_symbol("dll_initialise_plugin");
		if (init_func)
		{
			init_func(); //GEEK_THROW_EXCEPTION(err_item_not_found, "can not found symbol: dll_initialise_plugin");
		}
		
	}
}
//-------------------------------------------------------------------------------------------------
bool plugins_manager::find_plugin_by_name(dyn_lib_ptr plugin, std::string const* plugin_name)
{
	BOOST_ASSERT(plugin != NULL);
	BOOST_ASSERT(plugin_name != NULL);
	
	if (plugin->get_name() == *plugin_name)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
void plugins_manager::unload_plugin(std::string const& plugin_name)
{
	dyn_lib_list_t::iterator end_itr(plugin_lib_list_.end());
	dyn_lib_list_t::iterator itr(
		std::find_if(
			plugin_lib_list_.begin(),
			end_itr,
			boost::bind(&plugins_manager::find_plugin_by_name, this, _1, &plugin_name)
			)
		);

	if (itr != end_itr)
	{
		// Call plugin shutdown
		dyn_lib_ptr& plugin = *itr;
		dll_stop_plugin func = (dll_stop_plugin)plugin->get_symbol("dll_stop_plugin");

		if (func)
		{
			try // insure unload_plugin not throw, like delete
			{
				func();
			}
			catch(std::exception& ex)
			{
				GEEK_LOG_ERROR_MESSAGE(ex.what());
			}
			catch(...)
			{
				GEEK_LOG_ERROR_MESSAGE("Ignored exception, unload plugin: "+ plugin_name);
			}
		}

		plugin_lib_list_.erase(std::remove(plugin_lib_list_.begin(), plugin_lib_list_.end(), plugin), plugin_lib_list_.end());
	}
}
//------------------------------------------------------------------------------------------------- // 最后一定空一行，即便有了这个注释
