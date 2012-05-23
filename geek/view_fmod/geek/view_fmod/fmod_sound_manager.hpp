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
/// @file       fmod_sound_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_HPP
#define GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_HPP


// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/view_fmod/config.hpp>

#include <geek/framework/sound_manager_interface.hpp>




namespace geek{
	class fmod_sound_manager_impl;
	template class GEEK_VIEW_FMOD_DECL boost::scoped_ptr<fmod_sound_manager_impl>;
	class GEEK_VIEW_FMOD_DECL fmod_sound_manager : public sound_manager_interface
	{

	public:
		fmod_sound_manager(void);
		virtual ~fmod_sound_manager(void);

		bool init(void);
		void shutdown(void);
		void update(duration_type const& interval);
		virtual sound_player_ptr create_player(std::string const& file_name, bool stream = false) = 0;
		virtual void quick_play(std::string const& file_name, bool stream = false) = 0;
		virtual void set_resources_root(std::string const& path) = 0;
	private:
		boost::scoped_ptr<fmod_sound_manager_impl> impl_;
	};



}

#endif // GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_HPP
