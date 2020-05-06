#include "Scene/SceneManager.h"
#include "Resources/ResourceManager.h"
#include "checkML.h"
#include "MotorCasaPaco.h"
#include <OgreRoot.h>
#include "GUI/GUI_Manager.h"

SceneManager* SceneManager::instance = 0;

SceneManager::~SceneManager()
{
	delete currentScene_;
	currentScene_ = nullptr;
}

SceneManager::SceneManager()
{
}

SceneManager* SceneManager::getInstance()
{
	if (instance == 0 || instance == nullptr)
	{
		return nullptr;
	}

	return instance;
}

bool SceneManager::setupInstance()
{
	if (instance == 0)
	{
		instance = new SceneManager();
		return true;
	}

	return false;
}

void SceneManager::clean()
{
	delete instance;
}

Scene* SceneManager::loadScene(const std::string& scene)
{
	json j = ResourceManager::getInstance()->loadSceneFile(scene);

	currentScene_ = new Scene();
	currentScene_->setupScene(j);

	return currentScene_;
}

Scene* SceneManager::getCurrentScene()
{
	return currentScene_;
}

void SceneManager::sceneCleanup()
{
	GUI_Manager::getInstance()->clear();
	delete currentScene_;
	currentScene_ = nullptr;
}

void SceneManager::changeScene(const std::string& name)
{
	if (currentScene_ != nullptr && name == currentScene_->getName()) {
#ifdef _DEBUG
		printf("WARNING: Scene %s is currently active, ignoring 'changeScene()'...", name);
#endif
		return;
	}

	if (changeSceneRequested_ && name == nextScene_) {
#ifdef _DEBUG
		printf("WARNING: Scene change to %s already requested, ignoring 'changeScene()'...", name);
#endif
		return;
	}

	nextScene_ = name;
	changeSceneRequested_ = true;
}

void SceneManager::start(std::string initialScene){
	changeScene(initialScene);
	processChangeSceneRequest();
}

void SceneManager::preUpdate()
{
	currentScene_->preupdate();
}

void SceneManager::physicsUpdate()
{
	currentScene_->physicsUpdate();
}

void SceneManager::update()
{
	currentScene_->update();
}

void SceneManager::lateUpdate()
{
	currentScene_->lateUpdate();
}

void SceneManager::processChangeSceneRequest()
{
	if (currentScene_ != nullptr)
	{
		sceneCleanup();
		MotorCasaPaco::getInstance()->getSM()->clearScene();
	}

	currentScene_ = loadScene(nextScene_);
	currentScene_->start();

	nextScene_ = "";
	changeSceneRequested_ = false;
}

void SceneManager::endFrame()
{
	if (!changeSceneRequested_)
		return;

	processChangeSceneRequest();
}