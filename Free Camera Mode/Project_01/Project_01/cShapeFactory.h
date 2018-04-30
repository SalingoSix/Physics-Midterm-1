#ifndef _cShapeFactory_HG_
#define _cShapeFactory_HG_

#include "iShape.h"

#include <string>
#include <vector>

class cGameObject;

class cShapeFactory
{
public:
	iShape* CreateShape(std::string shapeType, cGameObject* newObj);
	iShape* CreateShape(std::string shapeType, cGameObject* newObj, cGameObject* myATAT);


	void AssembleShape(iShape* newShape, std::string objType, cGameObject* newObj);
	void AssembleShape(iShape* newShape, std::string objType, cGameObject* newObj, cGameObject* myATAT);

	void physicsStep(double deltaTime);

private:
	std::vector <iShape*> vec_pShapes;
};


#endif