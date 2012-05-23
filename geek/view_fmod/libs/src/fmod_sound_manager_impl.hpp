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
/// @file       stable_headers.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/02
///////////////////////////////////////////////////////////////////////


#ifndef GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_IMPL_HPP
#define GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_IMPL_HPP


// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/format.hpp>
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/exception.hpp>
#include <geek/view_fmod/config.hpp>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

namespace geek
{
	void err_check(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::string fmod_error = (boost::format("{FMOD error! (%d) %s}")%result%FMOD_ErrorString(result)).str();
			GEEK_LOG_ERROR_MESSAGE(fmod_error);
			GEEK_THROW_EXCEPTION(err_internal_error, fmod_error);
		}
	}


	class fmod_sound_player//: public sound_player
	{

	public:
		fmod_sound_player(FMOD::System* system, std::string const& file_name, bool stream = false):
			sound_(NULL),
			channel_(NULL),
			system_(system),
			stream_(stream),
			file_name_(file_name)
		{

		}
		virtual ~fmod_sound_player(void)
		{
			if(sound_)
				unload();
		}
	public:



		bool load()
		{
			if(stream_)
				system_->createSound(file_name_.c_str(),  FMOD_SOFTWARE|FMOD_CREATESTREAM|FMOD_2D, 0, &sound_);
			else
				system_->createSound(file_name_.c_str(),  FMOD_SOFTWARE|FMOD_2D, 0, &sound_);
			sound_->setMode(FMOD_LOOP_OFF);
			return true;
		}
		bool play(int times)
		{
			if(!sound_)
				return false;
			system_->playSound(FMOD_CHANNEL_FREE, sound_, false, &channel_);

			if(times <= 0)
				channel_->setMode(FMOD_LOOP_NORMAL);
			else
				channel_->setLoopCount(times-1);


			return true;
		}


		void set_volume(float volume)
		{

			if(!channel_)
				return;
			channel_->setVolume(volume);
			return;
		}


// 		virtual bool setPosition(float posi)
// 		{
// 			if(!channel_)
// 				return false;
// 			channel_->setPosition(static_cast<int>(posi * 1000.0f), FMOD_TIMEUNIT_MS);
// 			return true;
// 
// 		}
// 		virtual float getPosition(void)
// 		{
// 			if(!channel_)
// 				return false;
// 			unsigned int posi;
// 			channel_->getPosition(&posi, FMOD_TIMEUNIT_MS);
// 			return static_cast<float>(posi)/1000.0f;
// 		}
// 		virtual float getLength(void)
// 		{
// 			if(!sound_)
// 				return false;
// 			unsigned int length;
// 			sound_->getLength(&length,FMOD_TIMEUNIT_MS );
// 			return static_cast<float>(length)/1000.0f;
// 		}


		void stop(void)
		{
			if(!channel_)
				return;
			channel_->stop();
			return;
		}
		void set_paused(bool paused)
		{
			if(!channel_)
				return;
			channel_->setPaused(paused);
			return;
		}

		bool is_playing(void)
		{
			if(!channel_)
				return false;
			bool playing;
			channel_->isPlaying(&playing);
			return playing;
		}
		void unload(void)
		{
			if(!sound_)
			{
				channel_ = NULL;
				return;
			}	
			sound_->release();
			sound_ = NULL;
			channel_ = NULL;
			return;
		}

	private:
		FMOD::Sound* sound_;
		FMOD::Channel* channel_;
		FMOD::System* system_;
		bool loaded_;
		bool stream_;
		const std::string file_name_;
	};



	class fmod_sound_manager_impl
	{

	public:
		fmod_sound_manager_impl(void):system_(NULL),init_(false)
		{

		}
		~fmod_sound_manager_impl(void)
		{
			if(init_)
				shutdown();
		}
		bool init(void)
		{


			FMOD_RESULT   result;
			result = FMOD::System_Create(&system_);
			err_check(result);
#if GEEK_PLATFORM == GEEK_PLATFORM_LINUX
			result = system_->setOutput(FMOD_OUTPUTTYPE_ALSA);
			err_check(result);
#endif
			result = system_->init(32, FMOD_INIT_NORMAL, 0);
			err_check(result);
			init_ = true;
			return true;
		}
		void shutdown(void)
		{
			system_->close();
			system_->release(); 
			init_ = false;
		}
		void update(geek::duration_type const& interval)
		{
			if(init_)
				system_->update();
		}
		sound_player_ptr create_player(std::string const& file_name, bool stream)
		{

			boost::shared_ptr<fmod_sound_player> obj(new fmod_sound_player(system_, file_name, stream));
			sound_player_ptr player(new sound_player(obj));
			player->is_playing = (boost::bind(&fmod_sound_player::is_playing, obj.get()));
			player->load = (boost::bind(&fmod_sound_player::load, obj.get()));
			player->play = (boost::bind(&fmod_sound_player::play, obj.get(), _1));
			player->set_paused = (boost::bind(&fmod_sound_player::set_paused, obj.get(), _1));
			player->set_volume = (boost::bind(&fmod_sound_player::set_volume, obj.get(), _1));
			player->stop = (boost::bind(&fmod_sound_player::stop, obj.get()));
			player->unload = (boost::bind(&fmod_sound_player::unload, obj.get()));

			return player;
		}


	private:
		FMOD::System * system_;
		bool init_;

	};



}

#endif // GEEK_VIEW_FMOD_FMOD_SOUND_MANAGER_IMPL_HPP
