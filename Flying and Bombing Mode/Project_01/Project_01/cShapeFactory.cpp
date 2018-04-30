#include "cShapeFactory.h"

#include "cSphere.h"
#include "cPlane.h"
#include "cATATSphere.h"

#include "cGameObject.h"
#include "Physics.h"
#include "cMesh.h"

#include <vector>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

extern std::vector< cGameObject* >  g_vecGameObjects;
extern std::vector< cMesh > g_vecMesh;

const float OFFSET = 0.00001f;

//Abstract factory
iShape* cShapeFactory::CreateShape(std::string shapeType, cGameObject* newObj)
{
	iShape* newShape = NULL;
	if (shapeType == "PLANE")
	{
		newShape = new cPlane;
		AssembleShape(newShape, shapeType, newObj);
	}
	if (shapeType == "SPHERE")
	{
		newShape = new cSphere;
		AssembleShape(newShape, shapeType, newObj);
	}
	if (newShape != NULL)
		vec_pShapes.push_back(newShape);
	return newShape;
}

iShape* cShapeFactory::CreateShape(std::string shapeType, cGameObject* newObj, cGameObject* myATAT)
{
	iShape* newShape = NULL;
	if (shapeType == "ATATSPHERE")
	{
		newShape = new cATATSphere;
		AssembleShape(newShape, shapeType, newObj, myATAT);
	}
	if (newShape != NULL)
		vec_pShapes.push_back(newShape);
	return newShape;
}


//Builder pattern
void cShapeFactory::AssembleShape(iShape* newShape, std::string objType, cGameObject* newObj)
{
	if (objType == "PLANE")
	{
		cPlane* pPlane = (cPlane*)newShape;
		pPlane->pObject = newObj;
	}
	else if (objType == "SPHERE")
	{
		cSphere* pSphere = (cSphere*)newShape;
		pSphere->pObject = newObj;
		pSphere->radius = newObj->scale;
		pSphere->startPos = newObj->position;
	}
}

void cShapeFactory::AssembleShape(iShape* newShape, std::string objType, cGameObject* newObj, cGameObject* myATAT)
{
	if (objType == "ATATSPHERE")
	{
		cATATSphere* pATATSphere = (cATATSphere*)newShape;
		pATATSphere->pObject = newObj;
		pATATSphere->radius = newObj->scale;
		pATATSphere->myATAT = myATAT;
		pATATSphere->startPos = newObj->position;
	}
}

//Mediator that oversees collision, and calls for physics updates.
void cShapeFactory::physicsStep(double deltaTime)
{
	for (unsigned int i = 0; i < vec_pShapes.size(); i++)
	{
		iShape* pCurShape = this->vec_pShapes[i];
		pCurShape->physicsStep(deltaTime);

		int shapeType = pCurShape->shapeType();

		if (shapeType == 1) // This shape is a plane
		{
			// We don't do much with the plane...
		}

		else if (shapeType == 2) // This shape is a sphere
		{
			cSphere* pCurSphere = (cSphere*)pCurShape;
			for (int indexEO = 0; indexEO != vec_pShapes.size(); indexEO++)
			{
				// Don't test for myself
				if (i == indexEO)
					continue;	// It's me!! 

				iShape* pOtherShape = this->vec_pShapes[indexEO];
				// Is Another object
				int otherShapeType = pOtherShape->shapeType();

				if (otherShapeType == 1) // Check for Sphere-Plane collision
				{
					if (pCurSphere->hasExploded)
						continue;
					cPlane* pOtherPlane = (cPlane*)pOtherShape;
					cMesh thisMesh;
					for (unsigned int i = 0; i < ::g_vecMesh.size(); i++)
					{
						if (pOtherPlane->pObject->meshName == ::g_vecMesh[i].name)
						{
							thisMesh = ::g_vecMesh[i];
							break;
						}
					}
					for (int indexT = 0; indexT < thisMesh.numberOfTriangles; indexT++)
					{
						int ptA = thisMesh.pTriangles[indexT].vertex_ID_0;
						int ptB = thisMesh.pTriangles[indexT].vertex_ID_1;
						int ptC = thisMesh.pTriangles[indexT].vertex_ID_2;

						glm::vec3 vertexA = glm::vec3(thisMesh.pVertices[ptA].x, thisMesh.pVertices[ptA].y, thisMesh.pVertices[ptA].z);
						glm::vec3 vertexB = glm::vec3(thisMesh.pVertices[ptB].x, thisMesh.pVertices[ptB].y, thisMesh.pVertices[ptB].z);
						glm::vec3 vertexC = glm::vec3(thisMesh.pVertices[ptC].x, thisMesh.pVertices[ptC].y, thisMesh.pVertices[ptC].z);
						glm::vec3 spherePos = glm::vec3(pCurSphere->getPosition().x, pCurSphere->getPosition().y, pCurSphere->getPosition().z);

						glm::vec3 closestPoint = ClosestPtPointTriangle(spherePos, vertexA, vertexB, vertexC);

						float distance = glm::distance(pCurSphere->getPosition(), closestPoint);

						if (distance - OFFSET <= pCurSphere->radius)
						{	//THIS IS A COLLISION
							pCurSphere->hasExploded = true;
							break;
						}
					}
				}

				else if (otherShapeType == 2) // Check for Sphere-Sphere collision
				{
					//cSphere* pOtherSphere = (cSphere*)pOtherShape;
					//if (PenetrationTestSphereSphere(pCurSphere, pOtherSphere))
					//{
					//	float m1, m2, x1, x2;
					//	glm::vec3 v1, v2, v1x, v2x, v1y, v2y, x(pCurSphere->getPosition() - pOtherSphere->getPosition());

					//	x = glm::normalize(x);
					//	v1 = pCurSphere->getVelocity();
					//	x1 = glm::dot(x, v1);
					//	v1x = x * x1;
					//	v1y = v1 - v1x;
					//	m1 = 1.0f; //Mass of the sphere. Set mass of all spheres to 1, just to simplify...

					//	x = -x;

					//	v2 = pOtherSphere->getVelocity();
					//	x2 = glm::dot(x, v2);
					//	v2x = x * x2;
					//	v2y = v2 - v2x;
					//	m2 = 1.0f;

					//	pCurSphere->setVelocity(glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x*(2 * m2) / (m1 + m2) + v1y));
					//	pOtherSphere->setVelocity(glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x*(m2 - m1) / (m1 + m2) + v2y));
					//	pCurSphere->issueAlert(i, indexEO, otherShapeType);
					//}
				}

				else if (otherShapeType == 3)
				{
					cSphere* pOtherSphere = (cSphere*)pOtherShape;
					cATATSphere* pATATSphere = (cATATSphere*)pOtherShape;
					if (PenetrationTestSphereSphere(pCurSphere, pOtherSphere))
					{
						pATATSphere->ATATHit();
					}
				}
			}
		}
	}
}