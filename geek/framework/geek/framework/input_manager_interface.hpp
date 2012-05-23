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
/// @file       input_manager_interface.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_FRAMEWORK_INPUT_MANAGER_INTERFACE_HPP
#define GEEK_FRAMEWORK_INPUT_MANAGER_INTERFACE_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/framework/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/prerequisites.hpp>
#include <boost/signals2/signal.hpp>
namespace geek{


	enum key_code
	{
		KC_UNASSIGNED  = 0x00,
		KC_ESCAPE      = 0x01,
		KC_1           = 0x02,
		KC_2           = 0x03,
		KC_3           = 0x04,
		KC_4           = 0x05,
		KC_5           = 0x06,
		KC_6           = 0x07,
		KC_7           = 0x08,
		KC_8           = 0x09,
		KC_9           = 0x0A,
		KC_0           = 0x0B,
		KC_MINUS       = 0x0C,    // - on main keyboard
		KC_EQUALS      = 0x0D,
		KC_BACK        = 0x0E,    // backspace
		KC_TAB         = 0x0F,
		KC_Q           = 0x10,
		KC_W           = 0x11,
		KC_E           = 0x12,
		KC_R           = 0x13,
		KC_T           = 0x14,
		KC_Y           = 0x15,
		KC_U           = 0x16,
		KC_I           = 0x17,
		KC_O           = 0x18,
		KC_P           = 0x19,
		KC_LBRACKET    = 0x1A,
		KC_RBRACKET    = 0x1B,
		KC_RETURN      = 0x1C,    // Enter on main keyboard
		KC_LCONTROL    = 0x1D,
		KC_A           = 0x1E,
		KC_S           = 0x1F,
		KC_D           = 0x20,
		KC_F           = 0x21,
		KC_G           = 0x22,
		KC_H           = 0x23,
		KC_J           = 0x24,
		KC_K           = 0x25,
		KC_L           = 0x26,
		KC_SEMICOLON   = 0x27,
		KC_APOSTROPHE  = 0x28,
		KC_GRAVE       = 0x29,    // accent
		KC_LSHIFT      = 0x2A,
		KC_BACKSLASH   = 0x2B,
		KC_Z           = 0x2C,
		KC_X           = 0x2D,
		KC_C           = 0x2E,
		KC_V           = 0x2F,
		KC_B           = 0x30,
		KC_N           = 0x31,
		KC_M           = 0x32,
		KC_COMMA       = 0x33,
		KC_PERIOD      = 0x34,    // . on main keyboard
		KC_SLASH       = 0x35,    // / on main keyboard
		KC_RSHIFT      = 0x36,
		KC_MULTIPLY    = 0x37,    // * on numeric keypad
		KC_LMENU       = 0x38,    // left Alt
		KC_SPACE       = 0x39,
		KC_CAPITAL     = 0x3A,
		KC_F1          = 0x3B,
		KC_F2          = 0x3C,
		KC_F3          = 0x3D,
		KC_F4          = 0x3E,
		KC_F5          = 0x3F,
		KC_F6          = 0x40,
		KC_F7          = 0x41,
		KC_F8          = 0x42,
		KC_F9          = 0x43,
		KC_F10         = 0x44,
		KC_NUMLOCK     = 0x45,
		KC_SCROLL      = 0x46,    // Scroll Lock
		KC_NUMPAD7     = 0x47,
		KC_NUMPAD8     = 0x48,
		KC_NUMPAD9     = 0x49,
		KC_SUBTRACT    = 0x4A,    // - on numeric keypad
		KC_NUMPAD4     = 0x4B,
		KC_NUMPAD5     = 0x4C,
		KC_NUMPAD6     = 0x4D,
		KC_ADD         = 0x4E,    // + on numeric keypad
		KC_NUMPAD1     = 0x4F,
		KC_NUMPAD2     = 0x50,
		KC_NUMPAD3     = 0x51,
		KC_NUMPAD0     = 0x52,
		KC_DECIMAL     = 0x53,    // . on numeric keypad
		KC_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
		KC_F11         = 0x57,
		KC_F12         = 0x58,
		KC_F13         = 0x64,    //                     (NEC PC98)
		KC_F14         = 0x65,    //                     (NEC PC98)
		KC_F15         = 0x66,    //                     (NEC PC98)
		KC_KANA        = 0x70,    // (Japanese keyboard)
		KC_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
		KC_CONVERT     = 0x79,    // (Japanese keyboard)
		KC_NOCONVERT   = 0x7B,    // (Japanese keyboard)
		KC_YEN         = 0x7D,    // (Japanese keyboard)
		KC_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
		KC_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
		KC_PREVTRACK   = 0x90,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
		KC_AT          = 0x91,    //                     (NEC PC98)
		KC_COLON       = 0x92,    //                     (NEC PC98)
		KC_UNDERLINE   = 0x93,    //                     (NEC PC98)
		KC_KANJI       = 0x94,    // (Japanese keyboard)
		KC_STOP        = 0x95,    //                     (NEC PC98)
		KC_AX          = 0x96,    //                     (Japan AX)
		KC_UNLABELED   = 0x97,    //                        (J3100)
		KC_NEXTTRACK   = 0x99,    // Next Track
		KC_NUMPADENTER = 0x9C,    // Enter on numeric keypad
		KC_RCONTROL    = 0x9D,
		KC_MUTE        = 0xA0,    // Mute
		KC_CALCULATOR  = 0xA1,    // Calculator
		KC_PLAYPAUSE   = 0xA2,    // Play / Pause
		KC_MEDIASTOP   = 0xA4,    // Media Stop
		KC_VOLUMEDOWN  = 0xAE,    // Volume -
		KC_VOLUMEUP    = 0xB0,    // Volume +
		KC_WEBHOME     = 0xB2,    // Web home
		KC_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
		KC_DIVIDE      = 0xB5,    // / on numeric keypad
		KC_SYSRQ       = 0xB7,
		KC_RMENU       = 0xB8,    // right Alt
		KC_PAUSE       = 0xC5,    // Pause
		KC_HOME        = 0xC7,    // Home on arrow keypad
		KC_UP          = 0xC8,    // UpArrow on arrow keypad
		KC_PGUP        = 0xC9,    // PgUp on arrow keypad
		KC_LEFT        = 0xCB,    // LeftArrow on arrow keypad
		KC_RIGHT       = 0xCD,    // RightArrow on arrow keypad
		KC_END         = 0xCF,    // End on arrow keypad
		KC_DOWN        = 0xD0,    // DownArrow on arrow keypad
		KC_PGDOWN      = 0xD1,    // PgDn on arrow keypad
		KC_INSERT      = 0xD2,    // Insert on arrow keypad
		KC_DELETE      = 0xD3,    // Delete on arrow keypad
		KC_LWIN        = 0xDB,    // Left Windows key
		KC_RWIN        = 0xDC,    // Right Windows key
		KC_APPS        = 0xDD,    // AppMenu key
		KC_POWER       = 0xDE,    // System Power
		KC_SLEEP       = 0xDF,    // System Sleep
		KC_WAKE        = 0xE3,    // System Wake
		KC_WEBSEARCH   = 0xE5,    // Web Search
		KC_WEBFAVORITES= 0xE6,    // Web Favorites
		KC_WEBREFRESH  = 0xE7,    // Web Refresh
		KC_WEBSTOP     = 0xE8,    // Web Stop
		KC_WEBFORWARD  = 0xE9,    // Web Forward
		KC_WEBBACK     = 0xEA,    // Web Back
		KC_MYCOMPUTER  = 0xEB,    // My Computer
		KC_MAIL        = 0xEC,    // Mail
		KC_MEDIASELECT = 0xED     // Media Select
	};


	enum mouse_button_id
	{
		mb_left = 0, mb_right, mb_middle,
		mb_button3, mb_button4,	mb_button5, mb_button6,	mb_button7
	};


	class key_event
	{
	public:
		inline key_event(key_code key, unsigned int text):key_(key),text_(text){}


		inline key_event(key_event const& evt)
		{
			key_ = evt.key_;
			text_ = evt.text_;
		}
		inline key_event& operator = (key_event const& evt)
		{
			key_ = evt.key_;
			text_ = evt.text_;
			return *this;
		}

		inline void swap(key_event& evt) throw()
		{
			using std::swap;
			swap(key_, evt.key_);
			swap(text_, evt.text_);
		}
		key_code get_key(void) const
		{
			return key_;
		}

		unsigned int get_text(void) const
		{
			return text_;
		}

	private:
		key_code key_;
		unsigned int text_;
	};
	class mouse_event
	{
	public:

		inline mouse_event(int x, int y, int abs_x, int abs_y_, int z):x_(x),y_(y),abs_x_(abs_x),abs_y_(abs_y_),z_(z){}
		inline mouse_event(int x, int y, int abs_x, int abs_y_, mouse_button_id mb):x_(x),y_(y),abs_x_(abs_x),abs_y_(abs_y_),button_(mb){}



		inline mouse_event(mouse_event const& evt)
		{
			x_ = evt.x_;
			y_ = evt.y_;
			z_ = evt.z_;

			abs_x_ = evt.abs_x_;
			abs_y_ = evt.abs_y_;
		}
		inline mouse_event& operator=(mouse_event const& evt)
		{

			x_ = evt.x_;
			y_ = evt.y_;
			z_ = evt.z_;
			abs_x_ = evt.abs_x_;
			abs_y_ = evt.abs_y_;

			return *this;
		}

		inline void swap(mouse_event & evt) throw()
		{
			using std::swap;
			swap(x_, evt.x_);
			swap(y_, evt.y_);
			swap(z_, evt.z_);
			swap(abs_x_, evt.abs_x_);
			swap(abs_y_, evt.abs_y_);
		}

		int get_x(void) const
		{
			return x_;
		}




		int get_y(void) const
		{
			return y_;
		}


		int get_abs_x(void) const
		{
			return abs_x_;
		}


		int get_abs_y(void) const
		{
			return abs_y_;
		}

		int get_z(void) const
		{
			return z_;
		}



		mouse_button_id get_button(void) const
		{
			return static_cast<mouse_button_id>(button_);
		}
	private:
		int x_;
		int y_;
		int abs_x_;
		int abs_y_;
		union
		{
			int z_;
			int button_;
		};
	};
	typedef std::pair<size_t, size_t> mouse_state;



	//class IInputManagerImpl;
	//template class _OrzFrameworkBaseExport boost::scoped_ptr<IInputManagerImpl>;


	class GEEK_FRAMEWORK_DECL input_manager_interface : public singleton<input_manager_interface>
	{
	public:

		typedef boost::function<bool (mouse_event const& evt)> mouse_callback;
		typedef boost::function<bool (key_event const& evt)> key_callback;
// 		typedef std::vector<KeyListener *> KeyListenerList;
	
	public:
		typedef boost::signals2::signal<bool (mouse_event const& evt)> mouse_signal;
		typedef boost::signals2::signal<bool (key_event const& evt)> key_signal;

		input_manager_interface(void);
		virtual ~input_manager_interface(void);

		bool init(void);
		void shutdown(void);

		mouse_signal & on_mouse_pressed(void);
		mouse_signal & on_mouse_released(void);
		mouse_signal & on_mouse_moved(void);


		key_signal & on_key_released(void);
		key_signal & on_key_pressed(void);
	protected:

		virtual bool on_init(void) = 0;
		virtual void on_shutdown(void) = 0;

		boost::scoped_ptr<mouse_signal> on_mouse_pressed_;
		boost::scoped_ptr<mouse_signal> on_mouse_released_;
		boost::scoped_ptr<mouse_signal> on_mouse_moved_;

		boost::scoped_ptr<key_signal> on_key_released_;
		boost::scoped_ptr<key_signal> on_key_pressed_;


	};




}
#endif // GEEK_FRAMEWORK_INPUT_MANAGER_INTERFACE_HPP


