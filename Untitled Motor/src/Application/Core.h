#pragma once

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreFileSystemLayer.h>
#include <SDL.h>
#include <OgreFrameListener.h>

#include "TestEntity.h"
#include "AudioManager.h"

#include "Scene.h"

class Core : public Ogre::FrameListener
{

public:
	~Core();

	static Core* getInstance();
	static bool setupInstance(const Ogre::String& appName);
	static void clean();

	void init();

	void changeScene(Ogre::String name);

	void initPhysicsTestScene();

	void initLoadingTestScene();
	void testMessageSystem();

	void start();

	void pollEvents();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	//virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }
	virtual void windowMoved(Ogre::RenderWindow* rw) {}
	virtual void windowResized(Ogre::RenderWindow* rw) {}
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	virtual void windowClosed(Ogre::RenderWindow* rw) {}
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {}

	Ogre::Root* getRoot();
	Ogre::SceneManager* getSM();
	Ogre::RenderWindow* getOgreWin();
	SDL_Window* getSDLWin();

private:

	Core(const Ogre::String& appName);

	void sceneCleanup();

	void setupWindow(Ogre::String windowName);
	void setupRoot();
	void setup();
	void shutdown();
	void updateRender(); //actualiza el render de los objetos con rigidbody
	bool checkConfig();

	Ogre::Root* root;
	Ogre::SceneManager* sm;

	SDL_Window* sdlWindow;
	Ogre::RenderWindow* window;
	Ogre::String appName;
	
	std::vector<TestEntity> testEntities;
	AudioManager* audioManager;

	Scene* currentScene;

	static Core* instance;
};
