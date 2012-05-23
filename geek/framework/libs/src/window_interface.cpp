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
#include <boost/foreach.hpp>
#include <geek/framework/window_interface.hpp>
//#include "System/WindowInterfaceImpl.h"

using namespace geek;

// void window_listener::on_size_change(size_t width, size_t height){}
// void window_listener::on_closed(void){}
window_interface::window_interface(void)//:_impl(new WindowInterfaceImpl())
{

}
window_interface::~window_interface(void)
{

}

// void window_interface::add_listener(window_listener * listener)
// {
// 	listeners_.push_back(listener);
// }
// void window_interface::remove_listener(window_listener * listener)
// {
// 	listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
// }


//void visitor(WindowListener::VisitorFun fun);


//boost::scoped_ptr<WindowInterfaceImpl> _impl;
/*
void window_interface::_size_change(size_t width, size_t height)
{

		
	BOOST_FOREACH(window_listener * listener, listeners_)
	{
		listener->on_size_change(width, height);
	}
 }

void window_interface::_closed(void)
{
	BOOST_FOREACH(window_listener * listener, listeners_)
	{
		listener->on_closed();
	}
}
*/