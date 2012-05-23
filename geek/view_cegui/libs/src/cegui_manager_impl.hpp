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
/// @file       cegui_manager_impl.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/02
///////////////////////////////////////////////////////////////////////
#ifndef GEEK_VIEW_CEGUI_CEGUI_MANAGER_IMPL_HPP
#define GEEK_VIEW_CEGUI_CEGUI_MANAGER_IMPL_HPP



// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/key_value_server.hpp>
#include <geek/view_cegui/config.hpp>
#include <geek/view_ogre/ogre_graphics_manager.hpp>
#include <geek/framework/system_interface.hpp>
#include <geek/prerequisites.hpp>
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4251)
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIPropertyHelper.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/CEGUI.h>
#pragma warning(pop)

#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#	pragma comment(lib,"imm32.lib")
#endif

//局部钩子函数 截取WM_CHAR消息实现中文输入
LRESULT CALLBACK chinese_char_hook_proc(int code, WPARAM w_param, LPARAM l_param);

namespace geek {


	class cegui_manager_impl//: public Orz::KeyListener,public Orz::MouseListener, public WindowListener
	{
	public:
		inline cegui_manager_impl(void):logo_geometry_(NULL), enable_mouse_event_(false), enable_key_event_(false)
		{

		}
		~cegui_manager_impl(void)
		{
			
			//GEEK_LOG_TRACE_MESSAGE("cegui_manager_impl");
		}

		bool init(void)
		{

#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
			g_hHook_ = SetWindowsHookEx(WH_GETMESSAGE, chinese_char_hook_proc, NULL, GetCurrentThreadId());
#endif

			if(!key_value_server::get_singleton_ptr() || !key_value_server::get_singleton().has("window"))
				return false;

			try
			{
				win_ = key_value_server::get_singleton().get_value<weak_window_ptr>("window");//win = get_any_ptr<WeakWindowPtr>(SystemInterface::getSingleton().getParame("WINDOW"));
			}
			catch(...)
			{
				GEEK_LOG_ERROR_MESSAGE("bool init(void) error");
			}
			window_ptr wp;
			if(wp = win_.lock())
			{

				size_change_connection_ = wp->on_size_change.connect(boost::bind(&cegui_manager_impl::on_size_change, this, _1, _2));
				//wp->addListener(this);
				// use *r
			}
			else
			{
				return false;
			}

		

			renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*geek::ogre_graphics_manager::get_singleton().get_render_window());
			CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

			if( key_value_server::get_singleton_ptr() && 
				key_value_server::get_singleton().has("cegui_logo") && 
				key_value_server::get_singleton().get_value<bool>("cegui_logo"))
			{

				// setup required to do direct rendering of FPS value
				const CEGUI::Rect scrn(CEGUI::Vector2(0, 0),
					renderer_->getDisplaySize());
				// setup for logo
				CEGUI::ImagesetManager::getSingleton().
					createFromImageFile("cegui_logo", "logo.png", "imagesets");
				logo_geometry_ = &renderer_->createGeometryBuffer();
				logo_geometry_->setClippingRegion(scrn);
				logo_geometry_->setPivot(CEGUI::Vector3(50, 34.75f, 0));
				logo_geometry_->setTranslation(CEGUI::Vector3(10, 520, 0));
				CEGUI::ImagesetManager::getSingleton().get("cegui_logo").
					getImage("full_image").draw(*logo_geometry_,
					CEGUI::Rect(0, 0, 100, 69.5f), 0);

				// clearing this queue actually makes sure it's created(!)
				renderer_->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

				renderer_->getDefaultRenderingRoot().
					subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
					CEGUI::Event::Subscriber(
					&cegui_manager_impl::overlay_handler, this));
			}

			return true;
		}

		void update(duration_type const& interval)
		{
			static float rot = 0.0f;
			if(logo_geometry_)
			{
				logo_geometry_->setRotation(CEGUI::Vector3(rot, 0, 0));
				rot += 0.18f * boost::chrono::nanoseconds(interval).count();
				if (rot > 360.0f)
					rot -= 360.0f;
			}
		}

		void shutdown(void)
		{
			size_change_connection_.disconnect();
			enable_mouse(false);
			enable_key(false);

#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
			UnhookWindowsHookEx(g_hHook_);
#endif
			CEGUI::OgreRenderer::destroySystem();
		}

		void on_size_change(size_t width, size_t height)
		{
			CEGUI::System::getSingleton().notifyDisplaySizeChanged(
				CEGUI::Size(static_cast<float>(width),
				static_cast<float>(height)));


		}


		void enable_mouse(bool enable)
		{
			if(enable_mouse_event_ == enable)
				return;
			enable_mouse_event_ = enable;
			if(enable_mouse_event_)
			{
				mouse_moved_connection_ = geek::input_manager_interface::get_singleton().on_mouse_moved().connect(boost::bind(&cegui_manager_impl::mouse_moved, this, _1));
				mouse_pressed_connection_ = geek::input_manager_interface::get_singleton().on_mouse_pressed().connect(boost::bind(&cegui_manager_impl::mouse_pressed, this, _1));
				mouse_released_connection_ = geek::input_manager_interface::get_singleton().on_mouse_released().connect(boost::bind(&cegui_manager_impl::mouse_released, this, _1));

			}
			else
			{
				mouse_moved_connection_.disconnect();
				mouse_pressed_connection_.disconnect();
				mouse_released_connection_.disconnect();
			}

		}
		void enable_key(bool enable)
		{
			if(enable_key_event_ == enable)
				return;
			enable_key_event_ = enable;

			if(enable_key_event_)
			{
				key_pressed_connection_ =  geek::input_manager_interface::get_singleton().on_key_pressed().connect(boost::bind(&cegui_manager_impl::key_pressed, this, _1));
				key_released_connection_ =  geek::input_manager_interface::get_singleton().on_key_pressed().connect(boost::bind(&cegui_manager_impl::key_released, this, _1));

			}else
			{

				key_pressed_connection_.disconnect();
				key_released_connection_.disconnect();
			}

		}


		//! handler for rendering the CEGUI fps & logo geometry
		bool overlay_handler(const CEGUI::EventArgs& args)
		{ 
			using namespace CEGUI;

			if (static_cast<const RenderQueueEventArgs&>(args).queueID != RQ_OVERLAY)
				return false;


			logo_geometry_->draw();
			return true;
		}
		inline CEGUI::OgreRenderer * get_renderer(void)
		{
			return renderer_;
		}

#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
		//***********************cegui中文输入***begin*********************************************
		static HHOOK   g_hHook_ ;

		static	bool chn_inject_char(CEGUI::utf32 code_point)
		{
#ifndef UNICODE
			static char s_tempChar[3] = "";
			static wchar_t s_tempWchar[2] = L"";
			static bool s_flag = false;
			unsigned char uch = (unsigned char)code_point;
			if( uch >= 0x80 )
			{
				if( !s_flag )
				{
					s_tempChar[1] = (char)uch; //第二个字节
					s_flag = true;
					return true;
				}
				else if( uch >= 0x80 )
				{
					s_tempChar[0] = (char)uch; //第一个字节
					s_flag = false;
					MultiByteToWideChar( 0, 0, s_tempChar, 2, s_tempWchar, 1); //转成宽字节
					s_tempWchar[1] = L'\0';
					CEGUI::utf32 code = (CEGUI::utf32)s_tempWchar[0];
					return CEGUI::System::getSingleton().injectChar( code );
				}
				else
				{
					return CEGUI::System::getSingleton().injectChar(code_point);
				}
			}
			else
			{
				s_flag = false;
				return CEGUI::System::getSingleton().injectChar(code_point);
			}
#else //UNICODE
			return CEGUI::System::getSingleton().injectChar(code_point );
#endif //UNICODE
		}
#endif

		//***************************end********************************************

	private: 



		bool mouse_pressed(mouse_event const& evt)
		{
			return CEGUI::System::getSingleton().injectMouseButtonDown(convert_geek_mouse_button_to_cegui(evt.get_button()));
		}
		bool mouse_released(mouse_event const& evt)
		{
			return CEGUI::System::getSingleton().injectMouseButtonUp(convert_geek_mouse_button_to_cegui(evt.get_button()));
		}
		bool mouse_moved(mouse_event const& evt)
		{
			if(key_value_server::get_singleton().has("w32_mouse") && key_value_server::get_singleton().get_value<bool>("w32_mouse"))
			{
				CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(evt.get_abs_x()), static_cast<float>(evt.get_abs_y()));//injectMouseMove( static_cast<float>(evt.getX()), static_cast<float>(evt.getY()) );
			}
			else
				CEGUI::System::getSingleton().injectMouseMove(static_cast<float>(evt.get_x()), static_cast<float>(evt.get_y()));//injectMouseMove( static_cast<float>(evt.getX()), static_cast<float>(evt.getY()) );

			int z = evt.get_z();

			if(z != 0)
			{
				CEGUI::System::getSingleton().injectMouseWheelChange(static_cast<float>(evt.get_z()));
			}
			return false;
		}

		bool key_pressed(key_event const& evt)
		{					
			//bin.wang 2011.1.7 最初代码
			//return CEGUI::System::getSingleton().injectChar( evt.getText() )|| ret;	
			if (evt.get_key()!= geek::KC_BACK)//为啥？
			{//处理特殊事件
				CEGUI::System::getSingleton().injectKeyDown( evt.get_key() ) ;
			}
			//bin.wang 2011.1.11 改动代码
			return false;
		}
		bool key_released(key_event const& evt)
		{
			return CEGUI::System::getSingleton().injectKeyUp( evt.get_key() );		
		}
		static CEGUI::MouseButton convert_geek_mouse_button_to_cegui(mouse_button_id id)
		{
			switch (id)
			{
				case mb_left: return CEGUI::LeftButton;
				case mb_right: return CEGUI::RightButton;
				case mb_middle:	return CEGUI::MiddleButton;
				case mb_button3: return CEGUI::X1Button;
			default: return CEGUI::LeftButton;
			}
		}
		bool enable_mouse_event_;
		bool enable_key_event_;


		CEGUI::OgreRenderer* renderer_;
		CEGUI::GeometryBuffer* logo_geometry_;
		weak_window_ptr win_;

		boost::signals2::connection size_change_connection_;
		boost::signals2::connection mouse_moved_connection_;
		boost::signals2::connection mouse_pressed_connection_;
		boost::signals2::connection mouse_released_connection_;
		boost::signals2::connection key_pressed_connection_;
		boost::signals2::connection key_released_connection_;


	};
}

#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
//***********************cegui中文输入************************************************
HHOOK geek::cegui_manager_impl::g_hHook_ = NULL;


//局部钩子函数 截取WM_CHAR消息实现中文输入
LRESULT CALLBACK chinese_char_hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
	
	MSG* msg = (MSG*)lParam;
	switch(msg->message)
	{
	case WM_CHAR:
		{
			switch(msg->wParam)
			{
				//输入状态下的特殊功能键位处理
			case VK_RETURN:
				{
					break;
				}

			case VK_TAB:
				{
					break;
				}

			case VK_BACK:
				{
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Backspace);
					break;
				}

			case VK_SPACE:
				{
					CEGUI::System::getSingleton().injectChar((CEGUI::utf32)msg->wParam);
					break;
				}
			default:
				{
					geek::cegui_manager_impl::chn_inject_char((CEGUI::utf32)msg->wParam);
					return true;
				}
				break;
			}
		}
	}
	return CallNextHookEx(geek::cegui_manager_impl::g_hHook_, code, wParam, lParam);
}
#endif //GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#endif // GEEK_VIEW_CEGUI_CEGUI_MANAGER_IMPL_HPP

