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

#include "script_component.hpp"
#include "script_interface.hpp"
#include <Ogre/Ogre.h>
#include <geek/view_ogre/ogre_graphics_manager.hpp>
using namespace geek;


script_component::script_component(void):script_interface_(new script_interface())
{
	script_interface_->hello_world = boost::bind(&script_component::script_hellow_world, this);
	query_.add(script_interface_.get());
	
}
script_component::~script_component(void)
{
	GEEK_LOG<<"script_component::~script_component(void)"<<GEEK_WARNING;
}


void script_component::script_hellow_world(void) const
{
	static int i = 0;
	i++;
	switch(i%3)
	{
	case 0:
		ogre_graphics_manager::get_singleton().get_camera()->getViewport()->setBackgroundColour(Ogre::ColourValue::Red);
		break;
	case 1:
		ogre_graphics_manager::get_singleton().get_camera()->getViewport()->setBackgroundColour(Ogre::ColourValue::Blue);
		break;
	case 2:
		ogre_graphics_manager::get_singleton().get_camera()->getViewport()->setBackgroundColour(Ogre::ColourValue::White);
		break;
	}
	GEEK_LOG<<"hellow world!!!"<<GEEK_NOTIFICATION;
	
}

