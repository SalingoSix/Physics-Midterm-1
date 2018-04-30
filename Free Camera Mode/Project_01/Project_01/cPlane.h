#ifndef _cPlane_HG_
#define _cPlane_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

#include "iShape.h"

class cGameObject;

class cPlane : public iShape
{
public:
	cPlane();		// constructor
	~cPlane();		// destructor
					//Orientation of model in space
	cGameObject* pObject;

	virtual void physicsStep(double deltaTime);

	virtual int shapeType();
};


#endif