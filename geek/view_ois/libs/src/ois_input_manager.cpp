/*
-----------------------------------------------------------------------------
This source file is a part of geek
(Game Engine Extensible Kit)
For the latest info, see http://gdgeek.com/

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
#include <geek/view_ois/ois_input_manager.hpp>

using namespace geek;
ois_input_manager::ois_input_manager(void):w32_mouse_(false),esc_(false)
{

}

bool ois_input_manager::on_init(void)
{	
	GEEK_LOG_NOTIFICATION_MESSAGE("call ois_input_manager::init(void);");

	if(!read_parame())
		return false;
	window_ptr wp;
	if(wp = window_.lock())
	{
		GEEK_LOG_NOTIFICATION_MESSAGE("The window handle is "+boost::lexical_cast<std::string>(wp->get_handle()));
	}
	else
	{
		return false;
	}


	OIS::ParamList param_list;    
	connection_list_.push_back(wp->on_size_change.connect(boost::bind(&ois_input_manager::on_size_change, this, _1, _2)));
	connection_list_.push_back(on_key_pressed().connect(boost::bind(&ois_input_manager::key_pressed_callback, this, _1)));
	//!!wp->addListener(this);
	param_list.insert( std::make_pair( std::string( "WINDOW" ), boost::lexical_cast<std::string>(wp->get_handle()) ) );


	if(w32_mouse_)
	{
		param_list.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		param_list.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	}
	ois_manager_ = OIS::InputManager::createInputSystem(param_list);

	keyboard_ = static_cast<OIS::Keyboard*>(ois_manager_->createInputObject( OIS::OISKeyboard, true ));
	mouse_ = static_cast<OIS::Mouse*>(ois_manager_->createInputObject( OIS::OISMouse, true ));

	const OIS::MouseState& ms = mouse_->getMouseState();
	ms.width = static_cast<int>(wp->get_width());
	ms.height = static_cast<int>(wp->get_height());

	keyboard_->setEventCallback(this);
	mouse_->setEventCallback(this);



	if(esc_)
		enble_esc();
	return true;
}


void ois_input_manager::set_w32mouse(bool w32mouse)
{
	w32_mouse_ = w32mouse;
}
void ois_input_manager::no_esc(void)
{
	enble_esc(false);
}
void ois_input_manager::enble_esc(bool on)
{
	// 			if(on)
	// 				_inputManager->addKeyListener(this);
	// 			else 
	// 				_inputManager->removeKeyListener(this);
}


void ois_input_manager::set_window(weak_window_ptr window)
{
	window_ = window;
}

void ois_input_manager::on_shutdown(void)
{	
	//_inputManager = NULL;
	if(ois_manager_)
	{

		ois_manager_->destroyInputObject( mouse_ );
		ois_manager_->destroyInputObject( keyboard_ );
		OIS::InputManager::destroyInputSystem(ois_manager_);
		ois_manager_ = NULL;//y::TDiJoyConnecter& Orz::Joy::TDiJoyConnecter::getSingleton(): Assertion `_singleton' failed.

		mouse_ = NULL;
		keyboard_ = NULL;
	}

	BOOST_FOREACH(boost::signals2::connection conn, connection_list_)
	{
		conn.disconnect();
	}
	connection_list_.clear();
	
	GEEK_LOG<<"on_mouse_pressed:"<<on_mouse_pressed().num_slots()<<GEEK_NOTIFICATION;
	GEEK_LOG<<"on_mouse_released:"<<on_mouse_released().num_slots()<<GEEK_NOTIFICATION;
	GEEK_LOG<<"on_mouse_moved:"<<on_mouse_moved().num_slots()<<GEEK_NOTIFICATION;
	GEEK_LOG<<"on_key_released:"<<on_key_released().num_slots()<<GEEK_NOTIFICATION;
	GEEK_LOG<<"on_key_pressed:"<<on_key_pressed().num_slots()<<GEEK_NOTIFICATION;
	//input_manager_interface::shutdown();


}
void ois_input_manager::update(duration_type const& interval)
{
	keyboard_->capture();
	mouse_->capture();
}

ois_input_manager::~ois_input_manager(void)
{

}


bool ois_input_manager::read_parame(void)
{
	if(!key_value_server::get_singleton_ptr())
		return false;
	std::string window("window");
	if(key_value_server::get_singleton().has("ois_window"))
	{
		window = "ois_window";
	}else if(key_value_server::get_singleton().has("window"))
	{
		window = "window";
	}else
	{

		GEEK_LOG_ERROR_MESSAGE("have no window handle.");
		return false;
	}

	weak_window_ptr win;
	try
	{
		set_window(key_value_server::get_singleton().get_value<weak_window_ptr>(window));
	}
	catch(...)
	{
		GEEK_LOG_ERROR_MESSAGE("bool init(void) error");
	}

	if(key_value_server::get_singleton().has("w32_mouse"))
	{
		if(key_value_server::get_singleton().get_value<bool>("w32_mouse"))
		{
			set_w32mouse(true);
		}
	}



	if(
		(!key_value_server::get_singleton().has("no_esc"))
		||(!key_value_server::get_singleton().get_value<bool>("no_esc"))
		)
	{
		enble_esc(true);
	}else
	{
		enble_esc(false);
	}
	return true;
}
void ois_input_manager::on_size_change(size_t width, size_t height)
{
	if(mouse_)
	{
		const OIS::MouseState& ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;
	}
}
bool ois_input_manager::key_pressed_callback(key_event const& evt)
{
	if(evt.get_key() == geek::KC_ESCAPE)
	{
		system_interface::get_singleton().exit();
		return true;
	}
	return false;
}


bool ois_input_manager::keyPressed(OIS::KeyEvent const& arg)
{
	on_key_pressed()(key_event(static_cast<geek::key_code>(arg.key) ,arg.text));
	// 			if(input_manager_)
	// 				input_manager_->_keyPressed();
	return true;
}

bool ois_input_manager::keyReleased(OIS::KeyEvent const& arg )
{
	on_key_released()(key_event(static_cast<geek::key_code>(arg.key) ,arg.text));
	// 			if(_inputManager)
	// 				_inputManager->_keyReleased(KeyEvent(static_cast<Orz::KeyCode>(arg.key) ,arg.text));
	return true;
}

bool ois_input_manager::mouseMoved(OIS::MouseEvent const& arg )
{
	on_mouse_moved()(mouse_event(arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, arg.state.Z.rel));
	return true;
}



bool ois_input_manager::mousePressed(OIS::MouseEvent const& arg, OIS::MouseButtonID id )
{
	on_mouse_pressed()(mouse_event( arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, static_cast<mouse_button_id>(id)));
	return true;
}


bool ois_input_manager::mouseReleased(OIS::MouseEvent const& arg, OIS::MouseButtonID id )
{
	on_mouse_released()(mouse_event( arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, static_cast<mouse_button_id>(id)));
	return true;
}
