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
/// @file       ogre_graphics_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_OGRE_OGRE_GRAPHICS_MANAGER_HPP
#define GEEK_VIEW_OGRE_OGRE_GRAPHICS_MANAGER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/view_ogre/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/framework/window_interface.hpp>
namespace Ogre
{
	class Camera;
	class SceneManager;
	class RenderWindow;
	class Camera;
	class Viewport;
	class LogManager;
	class Root;
}
namespace geek{
// 
//	class ogre_rendering_queued_listener;
	//class ogre_frame_listener;
	class camera_man;
	class GEEK_VIEW_OGRE_DECL ogre_graphics_manager : public singleton<ogre_graphics_manager>//, public window_listener
	{
	private:
		class render_system_info
		{
			public:
				std::string render_system_name;
				std::string win_name;
				unsigned int width;
				unsigned int height;
				bool full_screen;
		};
	public:
		ogre_graphics_manager(void);
		~ogre_graphics_manager(void);
		Ogre::Camera* get_camera(void);
		Ogre::SceneManager* get_scene_manager(void);
		Ogre::RenderWindow* get_render_window(void);
		void update(duration_type const& interval);
		bool init(void);
		void shutdown(void);
		void no_output(bool no_output = true);
		void silent(bool silent);
		//void debug_camera(bool debug = true);
		void set_weak_window_ptr(weak_window_ptr weak_win);
// 		void add_rendering_queued_listener(ogre_rendering_queued_listener* listener);
// 		void remove_rndering_queued_listener(ogre_rendering_queued_listener* listener);

		void set_plugins_path(std::string const& plugins_path);
		void set_scene_manager_name(std::string const& scene_manager_name);
		void set_render_system(std::string const& render_system_name, std::string const& win_name, unsigned int width, unsigned int height, bool fullScreen);
	private:


		void on_size_change(size_t width, size_t height);
		void on_closed(void){exit_ = true;}
		ogre_window_ptr create_render_window_in(void);
		ogre_window_ptr create_render_window_out(weak_window_ptr win);
		void setup_resources(void);
		void clear_resources(void);

		void create_scene_manager(void);
		void destroy_scene_manager(void);

		void create_camera(void);
		void destroy_camera(void);

		void create_viewports(void);
		void destroy_viewports(void);

		void load_resources(void);



		void create_simple_scene(void);
		void destroy_simple_scene(void);


		bool _config(void);
		bool _parame(void);
		boost::scoped_ptr<Ogre::Root> root_;
		ogre_window_ptr window_;

		Ogre::SceneManager* scene_manager_;
		Ogre::Camera* camera_;

		Ogre::Viewport* viewport_;
	private:
		boost::scoped_ptr<Ogre::LogManager> log_mgr_;
		bool init_;
		bool exit_;
		bool no_output_;
		bool silent_;
		weak_window_ptr weak_win_;
	private:
		std::string plugins_path_;
		std::string scene_manager_name_;
		boost::scoped_ptr<render_system_info> render_system_info_; 
		//boost::scoped_ptr<camera_man> camera_man_; 

	};

// 
// 	SYSTEMUNIT_BEGIN(ogre_graphics_manager) 
// 	SYSTEMUNIT_INIT 
// 	SYSTEMUNIT_UPDATE 
// 	SYSTEMUNIT_SHUTDOWN 
// 	SYSTEMUNIT_END(ogre_graphics_manager) 

}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, ogre_graphics_manager) 

#endif // GEEK_VIEW_OGRE_OGRE_GRAPHICS_MANAGER_HPP