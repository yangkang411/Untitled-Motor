#pragma once
#include "GUI/GUI_Element.h"

namespace CEGUI {
	class String;
	class System;
	class EventArgs;
	class OgreRenderer;
	class WindowManager;
	class PushButton;
	class EventArgs;
}

namespace Ogre {
	class RenderWindow;
}

class GUI_Manager
{
public:
	~GUI_Manager();

	static GUI_Manager* getInstance();
	static bool setupInstance(Ogre::RenderWindow* window);
	void setupResources();
	static void clean();
	void createRoot();
	void loadLayout(std::string filename);
	void loadScheme(std::string filename);
	CEGUI::System& getSystem();
	void update(float deltaTime);
	void addChild(int type, std::string name);
	CEGUI::PushButton* getPushButton(std::string name);
	CEGUI::Window* getWindow(std::string name);
	void setMouseCursor(const std::string& imageFile);
	void showMouseCursor();
	void hideMouseCursor();
	bool test(const CEGUI::EventArgs& e);

	template<typename T>
	void setEvents(CEGUI::PushButton* button, bool (T::* function)(const CEGUI::EventArgs&), T* obj);

	GUI_Element* getRoot();
	CEGUI::WindowManager* getWinManager();

private:
	GUI_Manager(Ogre::RenderWindow* window);

	static GUI_Manager* instance;
	CEGUI::OgreRenderer* renderer;
	GUI_Element* root;
	CEGUI::WindowManager* winManager;
};

