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
using namespace geek;
template<> input_manager_interface* geek::singleton<input_manager_interface>::singleton_ = NULL;

input_manager_interface::input_manager_interface(void)
{


}
input_manager_interface::~input_manager_interface(void)
{

}

bool input_manager_interface::init(void)
{

	on_mouse_pressed_.reset(new mouse_signal());
	on_mouse_released_.reset(new mouse_signal());
	on_mouse_moved_.reset(new mouse_signal());

	on_key_released_.reset(new key_signal());
	on_key_pressed_.reset(new key_signal());
	return on_init();

}


void input_manager_interface::shutdown(void)
{
	on_shutdown();
	on_mouse_pressed_.reset();
	on_mouse_released_.reset();
	on_mouse_moved_.reset();

	on_key_released_.reset();
	on_key_pressed_.reset();
}

input_manager_interface::mouse_signal & input_manager_interface::on_mouse_pressed(void)
{
	assert(on_mouse_pressed_.get());
	return *on_mouse_pressed_;
}
input_manager_interface::mouse_signal & input_manager_interface::on_mouse_released(void)
{
	assert(on_mouse_released_.get());
	return *on_mouse_released_;
}
input_manager_interface::mouse_signal & input_manager_interface::on_mouse_moved(void)
{
	assert(on_mouse_moved_.get());
	return *on_mouse_moved_;
}


input_manager_interface::key_signal & input_manager_interface::on_key_released(void)
{
	assert(on_key_released_.get());
	return *on_key_released_;
}

input_manager_interface::key_signal & input_manager_interface::on_key_pressed(void)
{
	assert(on_key_pressed_.get());
	return *on_key_pressed_;
}
// 