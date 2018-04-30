#ifndef _cATATphere_HG_
#define _cATATSphere_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

#include "iShape.h"
#include "cCollisionAlert.h"

class cGameObject;

//QUESTION 5: Blowing up ATATs
class cATATSphere : public iShape
{
public:
	cATATSphere();		// constructor
	~cATATSphere();		// destructor
					//Orientation of model in space
	float radius;
	glm::vec3 startPos;

	cCollisionAlert* Alerter;

	cGameObject* pObject;
	cGameObject* myATAT;

	virtual void physicsStep(double deltaTime);

	//void setVelocity(glm::vec3 newVelocity);
	void setPosition(glm::vec3 newPos);

	//glm::vec3 getVelocity();
	glm::vec3 getPosition();

	void issueAlert(int pos1, int pos2, int type2);

	void ATATHit();

	virtual int shapeType();
};


#endif