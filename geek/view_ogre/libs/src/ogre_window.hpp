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
/// @file       ogre_window.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_OGRE_WINDOW_HPP
#define GEEK_OGRE_WINDOW_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/toolkits/log_system.hpp>
#include <geek/view_ogre/config.hpp>
#include <geek/framework/window_interface.hpp>
#include <Ogre/OgreCommon.h>
#include <geek/toolkits/key_value_server/key_value_server.hpp>
namespace geek
{

	class ogre_window_interface : public window_interface , public Ogre::WindowEventListener
	{
	public:
		ogre_window_interface(Ogre::RenderWindow* render_win):render_win_(render_win), window_destroyed_(false)
		{

			Ogre::WindowEventUtilities::addWindowEventListener(render_win, this);
		}
		virtual size_t get_handle()
		{

			size_t window_hnd = 0;
			render_win_->getCustomAttribute("WINDOW", &window_hnd);
			return window_hnd;
		}

		virtual size_t get_width()
		{
			return render_win_->getWidth();
		}

		virtual size_t get_height()
		{
			return render_win_->getHeight();
		}


		virtual void windowResized(Ogre::RenderWindow* rw)
		{
			on_size_change(get_width(), get_height());
			//visitor(boost::bind(&OgreWindowInterface::onSizeChange, this, _1));
		}

		virtual void windowClosed(Ogre::RenderWindow* rw)
		{
			on_closed();
			window_destroyed_ = true;
		}
		bool get_window_destroyed(void) const
		{
			return window_destroyed_;
		}

		virtual ~ogre_window_interface(void)
		{
			if(!window_destroyed_)
				Ogre::WindowEventUtilities::removeWindowEventListener(render_win_, this);
		}

	private:
		bool window_destroyed_;
		Ogre::RenderWindow* render_win_;
	};

	class ogre_window//: public Ogre::WindowEventListener, , public WindowListener
	{

	public:

		virtual void pre_render(void){}
		virtual void post_render(void){}
		void render(void)
		{
			pre_render();
			try
			{
				if(render_win_->isActive())
					Ogre::Root::getSingleton().renderOneFrame();
			}
			catch(std::exception& e)
			{
				GEEK_LOG_ERROR_MESSAGE(e.what());
			}
			post_render();
		}
		ogre_window(void):render_win_(NULL)
		{
		}
		virtual ~ogre_window(void){}

		Ogre::RenderWindow * get_render_window(void) const
		{
			return render_win_;
		}

		void set_render_window(Ogre::RenderWindow* render_win)
		{

			render_win_ = render_win;
		}
	private:
		Ogre::RenderWindow* render_win_;
	protected:

		void set_callback(window_interface* window, window_interface::size_change_callback size_change, window_interface::closed_callback closed)
		{
			assert(window != NULL);
			
			if(size_change)
			{
				size_change_connection_ = window->on_size_change.connect(size_change);
			}

			if(closed)
			{
				closed_connection_ = window->on_closed.connect(closed);
			}
		}
		void remove_callback(void)
		{
			size_change_connection_.disconnect();
			closed_connection_.disconnect();
		}
		boost::signals2::connection size_change_connection_;
		boost::signals2::connection closed_connection_;
	};

	class ogre_window_in: public ogre_window
	{
	public:




		virtual void pre_render(void)
		{
			Ogre::WindowEventUtilities::messagePump();
		}

		virtual ~ogre_window_in(void)
		{

			remove_callback();

			//	Ogre::WindowEventUtilities::removeWindowEventListener(getRenderWindow(), _winInterface.get());
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderWindow(get_render_window()->getName());


		}



		ogre_window_in(
			window_interface::size_change_callback size_change = window_interface::size_change_callback(), 
			window_interface::closed_callback closed = window_interface::closed_callback()
			):win_interface_()
		{
			try
			{
				set_render_window(Ogre::Root::getSingleton().getAutoCreatedWindow());

			}
			catch(std::exception& e)
			{
				GEEK_LOG_ERROR_MESSAGE(e.what());
			}

			win_interface_.reset(new ogre_window_interface(get_render_window()));
			set_callback(win_interface_.get(), size_change, closed);
	

			weak_window_ptr win = window_ptr(win_interface_);
			if(key_value_server::get_singleton_ptr() && (!key_value_server::get_singleton().has("window")))
 				key_value_server::get_singleton().set_value<weak_window_ptr>("window", win);

		}
		

		
		ogre_window_in(
			std::string const& name,
			unsigned int width,
			unsigned int height,
			bool fullScreen, 
			const Ogre::NameValuePairList* misc_params = 0, 
			window_interface::size_change_callback size_change = window_interface::size_change_callback(), 
			window_interface::closed_callback closed = window_interface::closed_callback()
			):win_interface_()
		{
			try
			{
				set_render_window(Ogre::Root::getSingleton().createRenderWindow(name, width, height, fullScreen, misc_params));//createRenderWindow("OgreWindowIn", 800, 600, false, 0);//->initialise(true, SystemInterface::getSingleton().getInfo().getAppName());
			}
			catch(std::exception& e)
			{
				GEEK_LOG_ERROR_MESSAGE(e.what());
			}

			win_interface_.reset(new ogre_window_interface(get_render_window()));
			set_callback(win_interface_.get(), size_change, closed);
	
		}
	private:
		boost::shared_ptr<ogre_window_interface> win_interface_;

	};

	class ogre_window_out: public ogre_window
	{

	public:


		ogre_window_out(weak_window_ptr win, 
			window_interface::size_change_callback size_change = window_interface::size_change_callback(), 
			window_interface::closed_callback closed = window_interface::closed_callback()
			
):ogre_window(), win_(win)
		{



			window_ptr wp = win_.lock();
			set_callback(wp.get(), size_change, closed);
			Ogre::NameValuePairList params;
			params["externalWindowHandle"] =  boost::lexical_cast<std::string>(static_cast<unsigned int>(wp->get_handle()));//handle;


			set_render_window( Ogre::Root::getSingleton().createRenderWindow("geek Out",//SystemInterface::getSingleton().getInfo().getAppName(), 
				static_cast<unsigned int>(wp->get_width()),
				static_cast<unsigned int>(wp->get_height()),//SystemInterface::getSingleton().getInfo().renderHeight, 
				false, 
				&params)
				);

		}
		virtual ~ogre_window_out(void)
		{
			if(window_ptr wp = win_.lock())
			{
				remove_callback();
			}
		}

	protected:
		weak_window_ptr win_;
		//window_listener* listener_;
	};
}
#endif // GEEK_OGRE_WINDOW_HPP
