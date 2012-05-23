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
/// @file       key_value_server.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_KEY_VALUE_SERVER_HPP
#define GEEK_TOOLKITS_KEY_VALUE_SERVER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <boost/algorithm/string.hpp>
#include <geek/toolkits/key_value_server/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/toolkits/exception.hpp>


namespace geek{
class GEEK_TOOLKITS_KEY_VALUE_SERVER_DECL key_value_server: public singleton<key_value_server>
{
private:
	
	typedef std::map<std::string, boost::any> key_value_map;
public:
	key_value_server(bool ignore_case = false):ignore_case_(ignore_case)
	{
		
	}
	
	template<class T>
	void set_value(std::string const& key, T const& t)
	{
		key_value_.insert(std::make_pair(ignore_case(key),  boost::any(t)));
		
	}
		
	template<class T>
	T get_value(std::string const& key)
	{
		key_value_map::iterator it = key_value_.find(ignore_case(key));
		if(it == key_value_.end())
			GEEK_THROW_EXCEPTION(err_item_not_found, "can't find :"+ key);

		return boost::any_cast<T>(it->second);
	}

	bool has(std::string const& key)
	{
		key_value_map::iterator it = key_value_.find(ignore_case(key));
		if(it == key_value_.end())
			return false;
		return true;
	}
private:
	
	std::string ignore_case(std::string const& key)
	{
		if(ignore_case_)
		{
			return boost::algorithm::to_lower_copy(key);
		}
		

		return key;
		
	}
	key_value_map key_value_;
	bool ignore_case_;
};
}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP



