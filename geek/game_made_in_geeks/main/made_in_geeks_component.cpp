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

#include "made_in_geeks_component.hpp"
using namespace geek;

made_in_geeks_component::made_in_geeks_component(void):game_interface_(new game_interface())
{
	
	game_interface_->init = boost::bind(&made_in_geeks_component::game_init, this);
	game_interface_->update = boost::bind(&made_in_geeks_component::game_update, this, _1);
	game_interface_->shutdown = boost::bind(&made_in_geeks_component::game_shutdown, this);
	query_.add(game_interface_.get());
}


bool made_in_geeks_component::game_init(void)
{
	return true;
}
void made_in_geeks_component::game_update(int step)
{
// 	static int i = 0;
// 	i+= step;
// 	std::cout<<step<<":"<<i<<std::endl;
// 	while(i>60)
// 	{
// 		i -= 60;
// 
// 		std::cout<<":========================================================="<<std::endl;
// 	}
}
void made_in_geeks_component::game_shutdown(void)
{

}


made_in_geeks_component::~made_in_geeks_component(void)
{

}