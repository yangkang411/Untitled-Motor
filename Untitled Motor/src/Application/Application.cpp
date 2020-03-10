// Application.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Entity.h"


#include <OgreException.h>
#include <iostream>
#include <stdexcept>

//#include <btBulletDynamicsCommon.h>
#define NOMINMAX

#include "windows.h"
#include <fmod.hpp>

#include "Core.h"

using namespace FMOD;

Core* core;

#ifdef  _DEBUG
    int main(int argc, char* argv[])
#else
    #include <Windows.h>
    int WINAPI
    WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	core = new Core("Motor Casa Paco");

	SDL_Init(SDL_INIT_EVERYTHING);

	try
	{
		core->init();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		delete core;
		SDL_Quit();

		return 0;
	}

	core->initPhysicsTestScene();

	core->start();

	delete core;
	SDL_Quit();

    return 0;
}