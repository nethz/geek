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
/// @file       made_in_geeks_component.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/03
///////////////////////////////////////////////////////////////////////

#ifndef GAME_MADE_IN_GEEKS_MADE_IN_GEEKS_COMPONENT_HPP
#define GAME_MADE_IN_GEEKS_MADE_IN_GEEKS_COMPONENT_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/framework/game_interface.hpp>

#include <geek/toolkits/component.hpp>

namespace geek
{
	class made_in_geeks_component: public component
	{
	public:
		made_in_geeks_component(void);
		virtual ~made_in_geeks_component(void);

	private:
		bool game_init(void);
		void game_update(int step);
		void game_shutdown(void);
		boost::scoped_ptr<game_interface> game_interface_;
	};
}
#endif //GAME_MADE_IN_GEEKS_MADE_IN_GEEKS_COMPONENT_HPP
