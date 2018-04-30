#ifndef _cSphere_HG_
#define _cSphere_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

#include "iShape.h"
#include "cCollisionAlert.h"

class cGameObject;

class cSphere : public iShape
{
public:
	cSphere();		// constructor
	~cSphere();		// destructor
						//Orientation of model in space
	float radius;
	glm::vec3 startPos;

	cCollisionAlert* Alerter;

	cGameObject* pObject;

	virtual void physicsStep(double deltaTime);

	void setVelocity(glm::vec3 newVelocity);
	void setPosition(glm::vec3 newPos);

	glm::vec3 getVelocity();
	glm::vec3 getPosition();

	bool hasExploded;

	void issueAlert(int pos1, int pos2, int type2);

	virtual int shapeType();
};


#endif