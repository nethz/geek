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
/// @file       sound_manager_interface.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_FRAMEWORK_SOUND_MANAGER_INTERFACE_HPP
#define GEEK_FRAMEWORK_SOUND_MANAGER_INTERFACE_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/framework/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/prerequisites.hpp>
#include <boost/signals2/signal.hpp>
namespace geek{


class GEEK_FRAMEWORK_DECL sound_player
{

public:
	sound_player(boost::shared_ptr<void> obj);
	virtual ~sound_player(void);

	/**������Ƶ����
	@param times ���Ŵ�����Ĭ��ֵΪһ�Σ��������Ϊ0���߸���������ѭ�����š�
	@returns �Ƿ񲥷ųɹ�
	*/
	boost::function<bool (int times)> play;

	///����Ƿ񲥷�
	boost::function<bool (void)> is_playing;


	///������Ƶ���ݵ��ڴ���
	boost::function<bool (void)> load;


	///���ڴ���ж����Ƶ����
	boost::function<void (void)> unload;

	///ֹͣ����
	boost::function<void (void)> stop;

	///������Ƶ�Ƿ���ͣ
	boost::function<void (bool)> set_paused;

	///������Ƶ������С
	boost::function<void (float volum)> set_volume;

	
protected:
	boost::shared_ptr<void> obj_;
};

typedef boost::shared_ptr<sound_player> sound_player_ptr;

class GEEK_FRAMEWORK_DECL sound_manager_interface : public singleton<sound_manager_interface>
{
public:
	sound_manager_interface(void);
	virtual ~sound_manager_interface(void);
	///������Դ����Ŀ¼
	virtual void set_resources_root(std::string const& path) = 0;
	///����һ����Ƶ
	virtual sound_player_ptr create_player(std::string const& file_name, bool stream = false) = 0;

	///�򵥲��Žӿڣ�ֻ����һ�顣
	virtual void quick_play(std::string const& file_name, bool stream = false) = 0;
};




}
#endif // GEEK_FRAMEWORK_INPUT_MANAGER_INTERFACE_HPP


