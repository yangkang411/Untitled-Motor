#include "GUI/GUI_Manager.h"

#include "MotorCasaPaco.h"
#include <iostream>

#include <CEGUI/Event.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OgreRenderWindow.h>
#include "checkML.h"

#include <functional>

GUI_Manager* GUI_Manager::instance = 0;

GUI_Manager::GUI_Manager(Ogre::RenderWindow* window)
{
	CEGUI::OgreRenderer& aux = CEGUI::OgreRenderer::bootstrapSystem(*MotorCasaPaco::getInstance()->getOgreWin());
	renderer = &aux;

	setupResources();

	createRoot();

	setupDefaultResources();
	//Callbacks?
}

GUI_Manager::~GUI_Manager()
{
	renderer->destroySystem();
	layouts.clear(); //Igual no es esto, pero algo hay que hacer con el vector(?)
	delete root;
}

GUI_Manager* GUI_Manager::getInstance()
{
	if (instance == 0 || instance == nullptr)
	{
		return nullptr;
	}

	return instance;
}

void GUI_Manager::createRoot()
{
	CEGUI::WindowManager& a = CEGUI::WindowManager::getSingleton();
	winManager = &a;

	root = new GUI_Element(a.createWindow("DefaultWindow", "root"));
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root->getWindowElement());
}

void GUI_Manager::loadLayout(std::string filename, bool visible)
{
	try
	{
		CEGUI::Window* layout = winManager->getSingleton().loadLayoutFromFile(filename); //Habria que a�adir el archivo del que proceden como opcion, en caso de usar mas de uno?
		root->addChild(layout);
		layouts.push_back(layout);

		if (!visible)
			layout->setVisible(false);
	}
	catch (std::exception e)
	{
		//Javi, las excepciones
	}
}

void GUI_Manager::loadScheme(std::string filename)
{
	try
	{
		CEGUI::SchemeManager::getSingleton().createFromFile(filename);
	}
	catch (std::exception e)
	{
		//Javi, las excepciones
	}
}

CEGUI::System& GUI_Manager::getSystem()
{
	CEGUI::System& aa = CEGUI::System::getSingleton();

	return aa;
}

void GUI_Manager::update(float deltaTime)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(deltaTime);
	CEGUI::System::getSingleton().renderAllGUIContexts();

	//Mas cosas a a�adir
}

bool GUI_Manager::test(const CEGUI::EventArgs& e) {
	std::cout << "joder";
	return true;
}

void GUI_Manager::updateScreenSize(int width, int height)
{
	CEGUI::System::getSingleton().getRenderer()->setDisplaySize(CEGUI::Size<float>(width, height));
}

void GUI_Manager::setEvents(CEGUI::PushButton* button, std::function<bool(const CEGUI::EventArgs&)> function)
{
	button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(function));
}

void GUI_Manager::loadFont(std::string name)
{
	CEGUI::FontManager::getSingleton().createFromFile(name);
}

void GUI_Manager::setupDefaultResources()
{
	loadScheme("GWEN.scheme");
	loadFont("shump-big.font");
	loadFont("shump-normal.font");
	loadFont("shump-little.font");

	GUI_Manager::getInstance()->setDefaultFont(MotorCasaPaco::getInstance()->getScreenWidth());
}

void GUI_Manager::setDefaultFont(int value)
{
	if (value >= 1280)
		CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("shump-big");
	else if (value > 800)
		CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("shump-normal");
	else
		CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("shump-little");
}

void GUI_Manager::addChild(int type, std::string name)
{
	switch (type)
	{
	case 0: //Layout
	{
	}
	break;
	case 1:
	{
		/*CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(winManager->getSingleton().createWindow("TaharezLook/Button"));
		testButton->setText("Hello World!");
		testButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0.5), CEGUI::UDim(0.5, 0.5)));
		testButton->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0.15), CEGUI::UDim(0.15, 0.15)));
		testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI_Manager::test, instance));
		root->addChild(testButton);*/
	}
	break;
	default:
	{
		//	loadLayout(name);
	}
	break;
	}
}

CEGUI::PushButton* GUI_Manager::getPushButton(std::string name)
{
	CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(root->getWindowElement()->getChild(name));
	return button;
}

CEGUI::Window* GUI_Manager::getStaticText(std::string name)
{
	return root->getWindowElement()->getChild(name);
}

void GUI_Manager::changeText(CEGUI::Window* window, std::string text)
{
	window->setText(text);
}

void GUI_Manager::appendText(CEGUI::Window* window, std::string text, bool space)
{
	if (space)
	{
		std::string aux;
		aux = window->getText().c_str();
		aux = aux + " " + text;
		window->setText(aux);
	}
	else
	{
		std::string aux;
		aux = window->getText().c_str();
		aux = aux + text;
		window->setText(aux);
	}
}

CEGUI::Window* GUI_Manager::getWindow(std::string name)
{
	CEGUI::Window* window = static_cast<CEGUI::Window*>(root->getWindowElement()->getChild(name));
	return window;
}

void GUI_Manager::setMouseCursor(const std::string& imageFile)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(imageFile);
}

void GUI_Manager::showMouseCursor()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void GUI_Manager::hideMouseCursor()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

GUI_Element* GUI_Manager::getRoot()
{
	return root;
}

CEGUI::WindowManager* GUI_Manager::getWinManager()
{
	return winManager;
}

void GUI_Manager::injectPosition(int x, int y)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
}

void GUI_Manager::setLayoutVisible(int layout, bool visible)
{
	layouts[layout]->setVisible(visible);
}

void GUI_Manager::changeImage(std::string image, std::string newImage) 	// "Layout/Nombre", "ImageSet/Nombre
{
	CEGUI::Window* thumbnail = static_cast<CEGUI::Window*>(root->getWindowElement()->getChild(image));
	thumbnail->setProperty("Image", newImage);
}

void GUI_Manager::changeAlpha(std::string  image, float alpha)
{
	CEGUI::Window* thumbnail = static_cast<CEGUI::Window*>(root->getWindowElement()->getChild(image));
	thumbnail->setAlpha(alpha);
}

float GUI_Manager::getAlpha(std::string image)
{
	CEGUI::Window* thumbnail = static_cast<CEGUI::Window*>(root->getWindowElement()->getChild(image));
	return thumbnail->getAlpha();
}

std::vector<CEGUI::Window*> GUI_Manager::getLayouts()
{
	return layouts;
}

bool GUI_Manager::setupInstance(Ogre::RenderWindow* window)
{
	if (instance == 0)
	{
		instance = new GUI_Manager(window);
		return true;
	}

	return false;
}

void GUI_Manager::setupResources()
{
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}

void GUI_Manager::clean()
{
	delete instance;
}

void GUI_Manager::clear()
{
	//getRoot()->getWindowElement()->destroy();
	delete root;
	layouts.clear(); //Igual no es esto, pero algo hay que hacer con el vector(?)

	createRoot();
}