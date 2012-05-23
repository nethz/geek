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
/// @file       ois_input_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_OIS_OIS_INPUT_MANAGER_HPP
#define GEEK_VIEW_OIS_OIS_INPUT_MANAGER_HPP

#include <geek/view_ois/config.hpp>
#include <geek/framework/input_manager_interface.hpp>
#include <geek/framework/window_interface.hpp>
#include <geek/framework/system_interface.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>
#include <OIS/OIS.h>
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/key_value_server.hpp>
namespace geek{

	class GEEK_VIEW_OIS_DECL ois_input_manager: public input_manager_interface, public OIS::KeyListener,public OIS::MouseListener
	{
	public:
		ois_input_manager(void);



		inline void set_w32mouse(bool w32mouse = true);
		inline void no_esc(void);
		inline void enble_esc(bool on = true);


		inline void set_window(weak_window_ptr window);

		void update(duration_type const& interval);

		~ois_input_manager(void);


	private:


		bool on_init(void);
		void on_shutdown(void);


		bool read_parame(void);
		void on_size_change(size_t width, size_t height);
		bool key_pressed_callback(key_event const& evt);


		virtual bool keyPressed(OIS::KeyEvent const& arg);
		virtual bool keyReleased(OIS::KeyEvent const& arg );

		virtual bool mouseMoved(OIS::MouseEvent const& arg );



		virtual bool mousePressed(OIS::MouseEvent const& arg, OIS::MouseButtonID id );


		virtual bool mouseReleased(OIS::MouseEvent const& arg, OIS::MouseButtonID id );

	private:

		OIS::InputManager* ois_manager_;
		OIS::Mouse* mouse_;
		OIS::Keyboard* keyboard_;
		bool w32_mouse_;
		weak_window_ptr window_;

		bool esc_;

		typedef geek::vector<boost::signals2::connection>::type connection_list;
		connection_list connection_list_;
	};

}

#endif // GEEK_VIEW_OIS_OIS_INPUT_MANAGER_HPP
