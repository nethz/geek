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

#include <geek/view_fmod/fmod_sound_manager.hpp>
#include "fmod_sound_manager_impl.hpp"

using namespace geek;
fmod_sound_manager::fmod_sound_manager(void):impl_(new fmod_sound_manager_impl())
{

}
fmod_sound_manager::~fmod_sound_manager(void)
{

}

bool fmod_sound_manager::init(void)
{
	return impl_->init();
}
void fmod_sound_manager::shutdown(void)
{
	impl_->shutdown();
}
void fmod_sound_manager::update(duration_type const& interval)
{
	impl_->update(interval);
}
sound_player_ptr fmod_sound_manager::create_player(std::string const& file_name, bool stream)
{
	return impl_->create_player(file_name, stream);
}
void fmod_sound_manager::quick_play(std::string const& file_name, bool stream)
{

}
void fmod_sound_manager::set_resources_root(std::string const& path)
{

}
