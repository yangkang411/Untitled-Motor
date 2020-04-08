#pragma once
#include "Entity/Component.h"

#include <Ogre.h>


class Skybox :
	public Component
{
public:
	enum SkyType {BOX, DOME, PLANE};
	Skybox(json& args);
	~Skybox() override;
	void init(json& args) override;
	void redefine(json& args) override;

	void setEnabled(bool b);

private:
	
	void makeSky();

	SkyType skyType;
	std::string material;
	Ogre::Real distance;
	bool drawFirst;


	Ogre::Real curvature;
	Ogre::Real tiling;

	Ogre::Plane plane;
	Ogre::Vector3 normal;
	Ogre::Real scale;
	Ogre::Real bow;
	int xSegments;
	int ySegments;
};

