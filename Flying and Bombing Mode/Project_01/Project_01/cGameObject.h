#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

class cGameObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor
	//Orientation of model in space
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 orientation2;		// HACK (will elimiate this with)
	float scale;
	bool wireFrame;

	//Values pertaining to physics
	glm::vec3 vel;			// Velocity
	glm::vec3 accel;		// Acceleration
	bool bIsUpdatedInPhysics;

	glm::vec4 diffuseColour;

	std::string meshName;
};

#endif