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
/// @file       camera_man.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_OGRE_CAMERA_MAN_HPP
#define GEEK_VIEW_OGRE_CAMERA_MAN_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/view_ogre/config.hpp>
#include <geek/framework/input_manager_interface.hpp>
#include <ogre/Ogre.h>
namespace Ogre
{
	class Camera;
	class SceneManager;
	class RenderWindow;
	class Camera;
	class Viewport;
	class LogManager;
	class Root;
	class SceneNode;
}
namespace geek{




	enum camera_style   // enumerator values for different styles of camera movement
	{
		CS_FREELOOK,
		CS_ORBIT,
		CS_MANUAL
	};

	/*=============================================================================
	| Utility class for controlling the camera in samples.
	=============================================================================*/
	class GEEK_VIEW_OGRE_DECL camera_man
    {
    public:
		camera_man(void);

		virtual ~camera_man() ;

		/*-----------------------------------------------------------------------------
		| Swaps the camera on our camera man for another camera.
		-----------------------------------------------------------------------------*/
		virtual void set_camera(Ogre::Camera* cam);
		virtual Ogre::Camera* get_camera() const;

		/*-----------------------------------------------------------------------------
		| Sets the target we will revolve around. Only applies for orbit style.
		-----------------------------------------------------------------------------*/
		virtual void set_target(Ogre::SceneNode* target);

		virtual Ogre::SceneNode* get_target(void) const;

		/*-----------------------------------------------------------------------------
		| Sets the spatial offset from the target. Only applies for orbit style.
		-----------------------------------------------------------------------------*/
		virtual void set_yaw_pitch_dist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);

		/*-----------------------------------------------------------------------------
		| Sets the camera's top speed. Only applies for free-look style.
		-----------------------------------------------------------------------------*/
		virtual void set_top_speed(Ogre::Real top_speed);

		virtual float get_top_speed();

		/*-----------------------------------------------------------------------------
		| Sets the movement style of our camera man.
		-----------------------------------------------------------------------------*/
		virtual void set_style(camera_style style);

		virtual camera_style get_style();

		/*-----------------------------------------------------------------------------
		| Manually stops the camera when in free-look mode.
		-----------------------------------------------------------------------------*/
		virtual void manual_stop();


		/*-----------------------------------------------------------------------------
		| Processes key presses for free-look style movement.
		-----------------------------------------------------------------------------*/
		bool inject_key_down(geek::key_event const& evt);
		/*-----------------------------------------------------------------------------
		| Processes key releases for free-look style movement.
		-----------------------------------------------------------------------------*/
		bool inject_key_up(geek::key_event const& evt);

		/*-----------------------------------------------------------------------------
		| Processes mouse movement differently for each style.
		-----------------------------------------------------------------------------*/
// #if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
// 		virtual void injectMouseMove(const OIS::MultiTouchEvent& evt)
// #else
		bool inject_mouse_move(geek::mouse_event const& evt);

		/*-----------------------------------------------------------------------------
		| Processes mouse presses. Only applies for orbit style.
		| Left button is for orbiting, and right button is for zooming.
		-----------------------------------------------------------------------------*/
// #if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
// 		virtual void injectMouseDown(const OIS::MultiTouchEvent& evt)
// 		{
// 			if (mStyle == CS_ORBIT)
// 			{
//                 mOrbiting = true;
// 			}
// 		}
// #else
		bool inject_mouse_down(geek::mouse_event const& evt);
//#endif

		/*-----------------------------------------------------------------------------
		| Processes mouse releases. Only applies for orbit style.
		| Left button is for orbiting, and right button is for zooming.
// 		-----------------------------------------------------------------------------*/
// #if GEEK_PLATFORM == GEEK_PLATFORM_IPHONE
// 		virtual void inject_mouseUp(const OIS::MultiTouchEvent& evt)
// 		{
// 			if (mStyle == CS_ORBIT)
// 			{
//                 mOrbiting = false;
// 			}
// 		}
// #else


		void inject_mouse_up(geek::mouse_event const& evt);

		void update(duration_type const& interval);
		bool init(void);
		void shutdown(void);
//#endif

    protected:

		Ogre::Camera* camera_;
		camera_style style_;
		Ogre::SceneNode* target_;
		bool orbiting_;
		bool zooming_;
		Ogre::Real top_speed_;
		Ogre::Vector3 velocity_;
		bool going_forward_;
		bool going_back_;
		bool going_left_;
		bool going_right_;
		bool going_up_;
		bool going_down_;
		bool fast_move_;
		typedef geek::vector<boost::signals2::connection>::type connection_list;
		connection_list connection_list_;
    };
}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, ogre_graphics_manager) 

#endif // GEEK_VIEW_OGRE_CAMERA_MAN_HPP