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
#include <geek/framework/input_manager_interface.hpp>
#include <geek/view_ogre/camera_man.hpp>
#include <geek/view_ogre/ogre_graphics_manager.hpp>


#include <ogre/Ogre.h>

using namespace geek;




/*=============================================================================
| Utility class for controlling the camera in samples.
=============================================================================*/
camera_man::camera_man(void)
: camera_(0)
, target_(0)
, orbiting_(false)
, zooming_(false)
, top_speed_(150)
, velocity_(Ogre::Vector3::ZERO)
, going_forward_(false)
, going_back_(false)
, going_left_(false)
, going_right_(false)
, going_up_(false)
, going_down_(false)
, fast_move_(false)
{

	//set_camera(cam);
}

camera_man::~camera_man() {}

/*-----------------------------------------------------------------------------
| Swaps the camera on our camera man for another camera.
-----------------------------------------------------------------------------*/
void camera_man::set_camera(Ogre::Camera* cam)
{
	camera_ = cam;
}

Ogre::Camera* camera_man::get_camera() const
{
	return camera_;
}

/*-----------------------------------------------------------------------------
| Sets the target we will revolve around. Only applies for orbit style.
-----------------------------------------------------------------------------*/
void camera_man::set_target(Ogre::SceneNode* target)
{
	if (target_ != target)
	{
		target_ = target;
		if(target)
		{
			set_yaw_pitch_dist(Ogre::Degree(0), Ogre::Degree(15), 150);
			camera_->setAutoTracking(true, target_);
		}
		else
		{
			camera_->setAutoTracking(false);
		}

	}


}

Ogre::SceneNode* camera_man::get_target(void) const
{
	return target_;
}

/*-----------------------------------------------------------------------------
| Sets the spatial offset from the target. Only applies for orbit style.
-----------------------------------------------------------------------------*/
void camera_man::set_yaw_pitch_dist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
{
	camera_->setPosition(target_->_getDerivedPosition());
	camera_->setOrientation(target_->_getDerivedOrientation());
	camera_->yaw(yaw);
	camera_->pitch(-pitch);
	camera_->moveRelative(Ogre::Vector3(0, 0, dist));
}

/*-----------------------------------------------------------------------------
| Sets the camera's top speed. Only applies for free-look style.
-----------------------------------------------------------------------------*/
void camera_man::set_top_speed(Ogre::Real top_speed)
{
	top_speed_ = top_speed;
}

float camera_man::get_top_speed()
{
	return top_speed_;
}

/*-----------------------------------------------------------------------------
| Sets the movement style of our camera man.
-----------------------------------------------------------------------------*/
void camera_man::set_style(camera_style style)
{
	if (style_ != CS_ORBIT && style == CS_ORBIT)
	{
		set_target(target_ ? target_ : camera_->getSceneManager()->getRootSceneNode());
		camera_->setFixedYawAxis(true);
		manual_stop();
		set_yaw_pitch_dist(Ogre::Degree(0), Ogre::Degree(15), 150);

	}
	else if (style_ != CS_FREELOOK && style == CS_FREELOOK)
	{
		camera_->setAutoTracking(false);
		camera_->setFixedYawAxis(true);
	}
	else if (style_ != CS_MANUAL && style == CS_MANUAL)
	{
		camera_->setAutoTracking(false);
		manual_stop();
	}
	style_ = style;

}

camera_style camera_man::get_style()
{
	return style_;
}

/*-----------------------------------------------------------------------------
| Manually stops the camera when in free-look mode.
-----------------------------------------------------------------------------*/
void camera_man::manual_stop()
{
	if (style_ == CS_FREELOOK)
	{
		going_forward_ = false;
		going_back_ = false;
		going_left_ = false;
		going_right_ = false;
		going_up_ = false;
		going_down_ = false;
		velocity_ = Ogre::Vector3::ZERO;
	}
}

void camera_man::update(duration_type const& interval)
{
	float time = duration_to_float_seconds(interval);

	if (style_ == CS_FREELOOK)
	{
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (going_forward_) accel += camera_->getDirection();
		if (going_back_) accel -= camera_->getDirection();
		if (going_right_) accel += camera_->getRight();
		if (going_left_) accel -= camera_->getRight();
		if (going_up_) accel += camera_->getUp();
		if (going_down_) accel -= camera_->getUp();

		// if accelerating, try to reach top speed in a certain time
		Ogre::Real top_speed = fast_move_ ? top_speed_ * 20 :top_speed_;
		if (accel.squaredLength() != 0)
		{
			accel.normalise();
			velocity_ += accel * top_speed * time * 10;
		}
		// if not accelerating, try to stop in a certain time
		else velocity_ -= velocity_ * time * 10;

		Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

		// keep camera velocity below top speed and above epsilon
		if (velocity_.squaredLength() > top_speed * top_speed)
		{
			velocity_.normalise();
			velocity_ *= top_speed;
		}
		else if (velocity_.squaredLength() < tooSmall * tooSmall)
			velocity_ = Ogre::Vector3::ZERO;

		if (velocity_ != Ogre::Vector3::ZERO) camera_->move(velocity_ * time);
	}

}

/*-----------------------------------------------------------------------------
| Processes key presses for free-look style movement.
-----------------------------------------------------------------------------*/
bool camera_man::inject_key_down(geek::key_event const& evt)
{
	if (style_ == CS_FREELOOK)
	{
		if (evt.get_key() == geek::KC_W || evt.get_key() == geek::KC_UP) going_forward_ = true;
		else if (evt.get_key() == geek::KC_S || evt.get_key()== geek::KC_DOWN) going_back_ = true;
		else if (evt.get_key() == geek::KC_A || evt.get_key() == geek::KC_LEFT) going_left_ = true;
		else if (evt.get_key() == geek::KC_D || evt.get_key() == geek::KC_RIGHT) going_right_ = true;
		else if (evt.get_key() == geek::KC_PGUP) going_up_ = true;
		else if (evt.get_key() == geek::KC_PGDOWN) going_down_ = true;
		else if (evt.get_key() == geek::KC_LSHIFT) fast_move_ = true;
	}

	return true;
}

/*-----------------------------------------------------------------------------
| Processes key releases for free-look style movement.
-----------------------------------------------------------------------------*/
bool camera_man::inject_key_up(geek::key_event const& evt)
{
	if (style_ == CS_FREELOOK)
	{
		if (evt.get_key() == geek::KC_W || evt.get_key() == geek::KC_UP) going_forward_ = false;
		else if (evt.get_key() == geek::KC_S || evt.get_key() == geek::KC_DOWN) going_back_ = false;
		else if (evt.get_key() == geek::KC_A || evt.get_key() == geek::KC_LEFT) going_left_ = false;
		else if (evt.get_key() == geek::KC_D || evt.get_key() == geek::KC_RIGHT) going_right_ = false;
		else if (evt.get_key() == geek::KC_PGUP) going_up_ = false;
		else if (evt.get_key() == geek::KC_PGDOWN) going_down_ = false;
		else if (evt.get_key() == geek::KC_LSHIFT) fast_move_ = false;
	}
	return true;
}

/*-----------------------------------------------------------------------------
| Processes mouse movement differently for each style.
-----------------------------------------------------------------------------*/
// #if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
// 		virtual void injectMouseMove(const OIS::MultiTouchEvent& evt)
// #else
bool camera_man::inject_mouse_move(geek::mouse_event const& evt)
//#endif
{
	if (style_ == CS_ORBIT)
	{
		Ogre::Real dist = (camera_->getPosition() - target_->_getDerivedPosition()).length();

		if (orbiting_)   // yaw around the target, and pitch locally
		{
			camera_->setPosition(target_->_getDerivedPosition());

			camera_->yaw(Ogre::Degree(- evt.get_x() * 0.25f));
			camera_->pitch(Ogre::Degree(- evt.get_y() * 0.25f));

			camera_->moveRelative(Ogre::Vector3(0, 0, dist));

			// don't let the camera go over the top or around the bottom of the target
		}
		else if (zooming_)  // move the camera toward or away from the target
		{
			// the further the camera is, the faster it moves
			camera_->moveRelative(Ogre::Vector3(0, 0, evt.get_y() * 0.004f * dist));
		}
		else if (evt.get_z() != 0)  // move the camera toward or away from the target
		{
			// the further the camera is, the faster it moves
			camera_->moveRelative(Ogre::Vector3(0, 0, -evt.get_z() * 0.0008f * dist));
		}
	}
	else if (style_ == CS_FREELOOK)
	{
		camera_->yaw(Ogre::Degree(-evt.get_x() * 0.15f));
		camera_->pitch(Ogre::Degree(-evt.get_y() * 0.15f));
	}

	return true;
}

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
bool camera_man::inject_mouse_down(geek::mouse_event const& evt)
{
	if (style_ == CS_ORBIT)
	{
		if (evt.get_button() == geek::mb_left) orbiting_ = true;
		else if (evt.get_button()  == geek::mb_right) zooming_ = true;
	}

	return true;
}
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

bool camera_man::init(void)
{
	assert(input_manager_interface::get_singleton_ptr());
	assert(ogre_graphics_manager::get_singleton_ptr());

	set_camera(ogre_graphics_manager::get_singleton().get_camera());
	
	set_style(CS_FREELOOK);
	connection_list_.push_back
		(input_manager_interface::get_singleton().on_mouse_pressed().connect(boost::bind(&camera_man::inject_mouse_down, this, _1)));
	connection_list_.push_back
		(input_manager_interface::get_singleton().on_mouse_moved().connect(boost::bind(&camera_man::inject_mouse_move, this, _1)));
	connection_list_.push_back
		(input_manager_interface::get_singleton().on_key_pressed().connect(boost::bind(&camera_man::inject_key_down, this, _1)));
	connection_list_.push_back
		(input_manager_interface::get_singleton().on_key_released().connect(boost::bind(&camera_man::inject_key_up, this, _1)));

	return true;
}

void camera_man::shutdown(void)
{
	assert(input_manager_interface::get_singleton_ptr());
	BOOST_FOREACH(boost::signals2::connection conn, connection_list_)
	{
		conn.disconnect();
	}
	connection_list_.clear();

}
void camera_man::inject_mouse_up(geek::mouse_event const& evt)
{
	if (style_ == CS_ORBIT)
	{
		if (evt.get_button() == geek::mb_left) orbiting_ = false;
		else if (evt.get_button() == geek::mb_right) zooming_ = false;
	}
}