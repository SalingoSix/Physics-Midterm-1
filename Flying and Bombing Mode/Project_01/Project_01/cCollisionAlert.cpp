#include "cCollisionAlert.h"

#include <iostream>
#include <string>

cCollisionAlert* cCollisionAlert::getInstance()
{
	if (cCollisionAlert::m_pTheOnlyAlerter == 0)
		cCollisionAlert::m_pTheOnlyAlerter = new cCollisionAlert;
	return cCollisionAlert::m_pTheOnlyAlerter;
}

cCollisionAlert* cCollisionAlert::m_pTheOnlyAlerter = 0;

cCollisionAlert::cCollisionAlert()
{
	return;
}

cCollisionAlert::~cCollisionAlert()
{
	return;
}

void cCollisionAlert::makeAlert(int obj1, int obj2, int type1, int type2)
{
	std::string object1 = "Unknown Object";
	std::string object2 = object1;
	if (type1 == 1)
		object1 = "Plane";
	else if (type1 == 2)
		object1 = "Ball";
	else if (type1 == 3)
		object1 = "CRAZY BALL";

	if (type2 == 1)
		object2 = "Plane";
	else if (type2 == 2)
		object2 = "Ball";
	else if (type2 == 3)
		object2 = "CRAZY BALL";

	std::cout << "Object " << obj1 << " (" << object1 << ") has collided with object " << obj2 << " (" << object2 << ").\n";
	return;
}
