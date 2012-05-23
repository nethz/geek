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
/// @file       component_wrapper.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/05
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_COMPONENT_WRAPPER_HPP
#define GEEK_TOOLKITS_COMPONENT_WRAPPER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/component/config.hpp>
#include <geek/toolkits/component/component.hpp>
#include <geek/toolkits/component/component_factories.hpp>


class component_wrapper
{
public:
	inline component_wrapper(char const* name) 
	{
		using namespace geek;
		std::cout<<"create component_wrapper;"<<std::endl;
		component_ = component_factories::get_instance().create(name);
	}
	inline ~component_wrapper(void)  
	{
		std::cout<<"destory component_wrapper;"<<std::endl;
	}
	inline geek::component * get_component(void)
	{
		return component_.get();
	}

private:
	geek::component_ptr component_;
};

#endif // GEEK_TOOLKITS_COMPONENT_WRAPPER_HPP



