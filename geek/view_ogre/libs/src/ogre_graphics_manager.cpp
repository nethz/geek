
#include "stable_headers.hpp"
#include "ogre_window.hpp"
#include <geek/view_ogre/ogre_graphics_manager.hpp>
#include <geek/toolkits/key_value_server/key_value_server.hpp>
//namespace geek
// {
// class ogre_frame_listener: public  Ogre::FrameListener
// {
// public:
// 	void addRenderingQueuedListener(ogre_rendering_queued_listener * listener);
// 	void removeRenderingQueuedListener(ogre_rendering_queued_listener * listener);
// 	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
// private:
// 	typedef std::vector<ogre_rendering_queued_listener *>  listener_list;
// 	listener_list listeners_;
// };
// }
// 
// using namespace geek;
// void ogre_frame_listener::addRenderingQueuedListener(ogre_rendering_queued_listener * listener)
// {
// 	 listeners_.push_back(listener);
// }
// void ogre_frame_listener::removeRenderingQueuedListener(ogre_rendering_queued_listener * listener)
// {
// 	 listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
// }
// 
// bool ogre_frame_listener::frameRenderingQueued(const Ogre::FrameEvent& evt)
// {
// 	listener_list::iterator it;
// 	bool ret =true;
// 	for(it = listeners_.begin(); it!= listeners_.end(); ++it)
// 	{
// 		(*it)->update(boost::chrono::milliseconds(evt.timeSinceLastFrame * 1000));
// 	}			   
// 
// 	return true;
// }
// 
// void ogre_graphics_manager::addRenderingQueuedListener(OgreRenderingQueuedListener * listener)
// {
// 	 frame_listener_->addRenderingQueuedListener( listener);
// }
// void ogre_graphics_manager::removeRenderingQueuedListener(OgreRenderingQueuedListener * listener)
// {
// 	
// 	 frame_listener_->removeRenderingQueuedListener( listener);
// }
using namespace geek;

template<> ogre_graphics_manager* singleton<ogre_graphics_manager>::singleton_ = NULL;


ogre_graphics_manager::ogre_graphics_manager(void):
root_(),
window_(),
scene_manager_(NULL),
camera_(NULL),
viewport_(NULL),
init_(false),
exit_(false),
/*frame_listener_(new ogre_frame_listener()),*/
plugins_path_("plugins.cfg"),
no_output_(false),
silent_(false)
{


	//mFSLayer = OGRE_NEW_T(FileSystemLayerImpl, Ogre::MEMCATEGORY_GENERAL)(OGRE_VERSION_NAME);
}
ogre_graphics_manager::~ogre_graphics_manager(void)
{
	init_ = false;

	window_.reset();
	root_.reset();
	log_mgr_.reset();
	//OGRE_DELETE_T(mFSLayer, FileSystemLayer, Ogre::MEMCATEGORY_GENERAL);
}
Ogre::Camera* ogre_graphics_manager::get_camera(void)
{

	return camera_;
}
Ogre::SceneManager* ogre_graphics_manager::get_scene_manager(void)
{
	return scene_manager_;
}
Ogre::RenderWindow* ogre_graphics_manager::get_render_window(void)
{
	return window_->get_render_window();
}
void ogre_graphics_manager::update(duration_type const& interval)
{
	if(init_)
		window_->render();
// 	if(camera_man_)
// 		camera_man_->update(interval);
}


// void ogre_graphics_manager::debug_camera(bool debug)
// {
// 	if(!geek::input_manager_interface::get_singleton_ptr())
// 	{
// 		GEEK_THROW_EXCEPTION(err_rt_assertion_failed, "can't find input_manager_interface.");
// 	}
// 	if(debug)
// 	{
// 
// 		camera_man_.reset(new camera_man(camera_));
// 		camera_man_->link();
// 		
// 
// 	}
// 	else
// 	{
// 		
// 		camera_man_->unlink();
// 		camera_man_.reset();
// 	}
// 
// }
void  ogre_graphics_manager::no_output(bool no_output)
{
	no_output_ = no_output;
}

void ogre_graphics_manager::set_weak_window_ptr(weak_window_ptr weak_win)
{
	weak_win_ = weak_win;
}
void ogre_graphics_manager::silent(bool silent)
{
	silent_ = silent;
}
//void ogre_graphics_manager::output(bool enable)
//{
//	_output = enable;
//}


bool ogre_graphics_manager::_config(void)
{
	using namespace Ogre;
	bool configDialog = true;
	if(render_system_info_)
	{
		const RenderSystemList & rList = root_->getAvailableRenderers();
		RenderSystemList::const_iterator it = rList.begin();
		RenderSystem *rSys = 0;
		while(it != rList.end()){

			rSys = * (it++);
			if(rSys->getName().find(render_system_info_->render_system_name) !=  std::string::npos){    
				//设置渲染器，并结束循环
				root_->setRenderSystem(rSys);
				
				break;
			}

		}


	}
	if(root_->getRenderSystem() == NULL)
	{
		
		if(silent_)
		{
			
			if(root_->restoreConfig())
			{
				configDialog = false; 
			}
		}

		
		if(configDialog && !root_->showConfigDialog())
		{
			return false;
		}
	}
	

	return true;
}
bool ogre_graphics_manager::_parame(void)
{
	if(!key_value_server::get_singleton_ptr())
	{
		return false;
	}
	if(key_value_server::get_singleton().has("ogre_no_output"))
	{
		no_output(key_value_server::get_singleton().get_value<bool>("ogre_no_output"));
	}

	if(key_value_server::get_singleton().has("ogre_silent"))
	{
		silent(key_value_server::get_singleton().get_value<bool>("ogre_silent"));
	}

	if(key_value_server::get_singleton().has("window"))
	{
		set_weak_window_ptr(key_value_server::get_singleton().get_value<weak_window_ptr>("window"));
	}

	

		
		//!!_window->addListener(this);
	
	return true;
}

bool ogre_graphics_manager::init(void)
{
	_parame();

	log_mgr_.reset(new Ogre::LogManager());

	Ogre::Log * log = log_mgr_->createLog("ogre.log", true, !no_output_, false);
	root_.reset(new Ogre::Root(plugins_path_, "ogre.cfg"));

	if(!_config())
	{
		return false;
	}

	bool config_dialog = true;


	if(weak_win_.lock())
	{
		window_ = create_render_window_out(weak_win_);
		//!!_window->addListener(this);
	}
	else
 	{
		if(render_system_info_)
		{
			Ogre::Root::getSingleton().initialise(false, "geek window");
		}
		else
		{
			Ogre::Root::getSingleton().initialise(true, "geek window");
		}
		window_ = create_render_window_in();
		//!!_window->addListener(this);
	}




	setup_resources();
	load_resources();
	create_scene_manager();
	create_camera();
	create_viewports();

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//
//	create_scene_manager();

	

	init_ = true;

	get_scene_manager()->setSkyBox(true, "Examples/CloudyNoonSkyBox"); 
	//createRenderWindow();
	//_root->addFrameListener(frame_listener_.get());
	return true;
}
void ogre_graphics_manager::shutdown(void)
{
	//camera_man_.reset();
	//_root->removeFrameListener(frame_listener_.get());
	init_ = false;
	destroy_simple_scene();
	clear_resources();
	destroy_viewports();
	destroy_camera();
	destroy_scene_manager();
	window_.reset();
	root_.reset();
	log_mgr_.reset();
}

ogre_window_ptr ogre_graphics_manager::create_render_window_in(void)
{
	
	window_interface::size_change_callback size_change = boost::bind(&ogre_graphics_manager::on_size_change, this, _1, _2);
	window_interface::closed_callback closed = boost::bind(&ogre_graphics_manager::on_closed, this);
 	if(render_system_info_)
 		return  ogre_window_ptr(new ogre_window_in(render_system_info_->win_name, render_system_info_->width, render_system_info_->height, render_system_info_->full_screen,NULL,
 		size_change,  closed));
	return ogre_window_ptr(new ogre_window_in(size_change,  closed));
}

ogre_window_ptr ogre_graphics_manager::create_render_window_out(weak_window_ptr win)
{
	
	window_interface::size_change_callback size_change = boost::bind(&ogre_graphics_manager::on_size_change, this, _1, _2);
	window_interface::closed_callback closed = boost::bind(&ogre_graphics_manager::on_closed, this);
	return boost::shared_ptr<ogre_window_out>(new ogre_window_out(win, size_change, closed));
}

void ogre_graphics_manager::on_size_change(size_t width, size_t height)
{
	if(height <= 0)
		return;
	assert(camera_);
	get_render_window()->resize(width, height);
	get_render_window()->windowMovedOrResized();

	for(int ct = 0; ct < get_render_window()->getNumViewports(); ++ct)
	{
		Ogre::Viewport* viewport = get_render_window()->getViewport(ct);
		Ogre::Camera* camera = viewport->getCamera();
		camera->setAspectRatio(static_cast<Ogre::Real>(viewport->getActualWidth()) / static_cast<Ogre::Real>(viewport->getActualHeight()));
		//pViewport->update();
	}
	//_camera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
}

void ogre_graphics_manager::setup_resources(void)
{
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String sec, type, arch;

	// go through all specified resource groups
	while (seci.hasMoreElements())
	{
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		// go through all resource paths
		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;


			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
			//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("D:/Test", "FileSystem", "General");
		}
	}
}


void ogre_graphics_manager::clear_resources(void)
{

	using namespace Ogre;


	Ogre::ResourceGroupManager::ResourceManagerIterator resMgrs =
		Ogre::ResourceGroupManager::getSingleton().getResourceManagerIterator();

	while (resMgrs.hasMoreElements())
	{
		resMgrs.getNext()->unloadUnreferencedResources();
	}
}

void ogre_graphics_manager::create_scene_manager(void)
{
	if(scene_manager_name_.empty())
	{
		scene_manager_ = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

	}else
	{
		scene_manager_ = Ogre::Root::getSingleton().createSceneManager(scene_manager_name_);
	}
}
void ogre_graphics_manager::destroy_scene_manager(void)
{
	Ogre::Root::getSingleton().destroySceneManager(scene_manager_);
	scene_manager_ = 0;
}


void ogre_graphics_manager::create_camera(void)
{
	camera_ = scene_manager_->createCamera("PlayerCam");
	camera_->setNearClipDistance(5);
// 	if(camera_man_ && camera_man_->get_camera() != camera_)
// 	{
// 		camera_man_->set_camera(camera_);
// 	}
}
void ogre_graphics_manager::destroy_camera(void)
{

}

void ogre_graphics_manager::create_viewports(void)
{
	viewport_ = window_->get_render_window()->addViewport(camera_);
	camera_->setAspectRatio(
		Ogre::Real(viewport_->getActualWidth()) / Ogre::Real(viewport_->getActualHeight()));
	viewport_->setBackgroundColour(Ogre::ColourValue::Green);

}
void ogre_graphics_manager::destroy_viewports(void)
{

}


void ogre_graphics_manager::load_resources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}



void ogre_graphics_manager::create_simple_scene(void)
{


}
void ogre_graphics_manager::destroy_simple_scene(void)
{

}


void ogre_graphics_manager::set_plugins_path(std::string const& plugins_path)
{
	plugins_path_ = plugins_path;
}
void ogre_graphics_manager::set_scene_manager_name(std::string const& scene_manager_name)
{
	scene_manager_name_ = scene_manager_name;
}

void ogre_graphics_manager::set_render_system(std::string const& render_system_name, std::string const& win_name, unsigned int width, unsigned int height, bool full_screen)
{
	render_system_info_.reset(new render_system_info());
	render_system_info_->render_system_name = render_system_name;
	render_system_info_->win_name = win_name;
	render_system_info_->width = width;
	render_system_info_->height = height;
	render_system_info_->full_screen = full_screen;
}