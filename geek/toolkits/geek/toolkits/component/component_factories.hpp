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
/// @file       component_factories.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_COMPONENT_FACTORIES_HPP
#define GEEK_TOOLKITS_COMPONENT_FACTORIES_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/component/config.hpp>
#include <boost/signals2/signal.hpp>


namespace geek{
///////////////////////////////////////////////////////////////////////
/// @brief      component factories ; 组件工厂
///
/// @details    Factory pattern ; 工厂模式
/// @date       2011/09/27
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_COMPONENT_DECL component_factories
{
public:
	typedef boost::function<component* (void)> factory_type;
	~component_factories(void);

	/////////////////////////////////////////////////
	/// @brief     get instance ref ;得到实例引用
	/// @details   Singleton Pattern ; 单件模式
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	static component_factories & get_instance(void);

	/////////////////////////////////////////////////
	/// @brief     get instance ref ;得到实例指针
	/// @details   Singleton Pattern ; 单件模式
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	static component_factories * get_instance_ptr(void);


	/////////////////////////////////////////////////
	/// @brief     add factory ; 添加工厂
	/// @details   
	/// @param type_name component name ; 组件类型名称 
	/// @param factory factory ; 工厂
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	component_factories & add_factory(std::string const& type_name, factory_type const& factory);

	/////////////////////////////////////////////////
	/// @brief     remove factory ; 移除工厂
	/// @details   
	/// @param type_name component name ; 组件类型名称 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	component_factories & remove_factory(std::string const& type_name);

	/////////////////////////////////////////////////
	/// @brief     create component ; 创建组件
	/// @details   
	/// @param type_name component name ; 组件类型名称 
	/// @return  component smart ptr ; 组件 智能指针
	/// @note      
	/////////////////////////////////////////////////
	component_ptr create(std::string const& type_name);

	boost::signals2::signal<void (component* object)> on_component_create;
	boost::signals2::signal<void (component* object)> on_component_destroy;
private:
	void destroy(component* object);
	component_factories(void);
	typedef std::map<std::string, factory_type> factory_map;
	factory_map map_;
	
};


}
#endif // GEEK_TOOLKITS_COMPONENT_FACTORIES_HPP



