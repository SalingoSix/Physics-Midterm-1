#ifndef _iShape_HG_
#define _iShape_HG_

class iShape
{
public:
	virtual ~iShape() {};

	virtual void physicsStep(double deltaTime) = 0;

	virtual int shapeType() = 0;
};

#endif
