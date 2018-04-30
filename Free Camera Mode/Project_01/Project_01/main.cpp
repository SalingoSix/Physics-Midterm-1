#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h> // atan2

#include "SceneHandler.h"
#include "cShaderManager.h"
#include "cMesh.h"
#include "cVAOMeshManager.h"
#include "cGameObject.h"
#include "cLightManager.h"
#include "cShapeFactory.h"
#include "Physics.h"
#include "cSphere.h"
#include "cATATSphere.h"

//Camera variables
float angle = 0.0f;
float lookx = 0.0f, looky = 3.0f, lookz = -1.0f;
float camPosx = 0.0f, camPosy = 3.0f, camPosz = 30.0f;
glm::vec3 g_cameraXYZ = glm::vec3(0.0f, 3.0f, 30.0f);
glm::vec3 g_cameraTarget_XYZ = glm::vec3(0.0f, 3.0f, 25.0f);

//Global handlers for shader, VAOs, game objects and lights
cShaderManager* g_pShaderManager = new cShaderManager();
cVAOMeshManager* g_pVAOManager = new cVAOMeshManager();
std::vector <cMesh> g_vecMesh;
std::vector <cGameObject*> g_vecGameObject;
std::vector <glm::vec3> g_vecObjStart;
cLightManager* g_pLightManager = new cLightManager();

cShapeFactory* g_pShapeFactory = new cShapeFactory;
std::vector< iShape* > g_vecShapes;

//QUESTION 3: Making Speeders move
int speederValue = 7;
bool speederDown, speederUp, speederLeft, speederRight = false;
//QUESTION 4: Tossin' bombs
bool tossaBomb = false;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	const float CAMERASPEED = 0.03f;
	const float CAMERAMOVE = 0.3f;
	//const float CAMERAMOVE = 30.0f;
	switch (key)
	{
		//MOVEMENT USING TANK CONTROLS WASD
	case GLFW_KEY_A:		// Look Left
		angle -= CAMERASPEED * 3;
		lookx = sin(angle);
		lookz = -cos(angle);
		break;
	case GLFW_KEY_D:		// Look Right
		angle += CAMERASPEED * 3;
		lookx = sin(angle);
		lookz = -cos(angle);
		break;
	case GLFW_KEY_W:		// Move Forward (relative to which way you're facing)
		camPosx += lookx * CAMERAMOVE;
		camPosz += lookz * CAMERAMOVE;
		break;
	case GLFW_KEY_S:		// Move Backward
		camPosx -= lookx * CAMERAMOVE;
		camPosz -= lookz * CAMERAMOVE;
		break;
	case GLFW_KEY_Q:		// Look Down (along y axis)
		camPosy -= CAMERAMOVE;
		looky -= CAMERAMOVE;
		break;
	case GLFW_KEY_E:		// Look Up (along y axis)
		camPosy += CAMERAMOVE;
		looky += CAMERAMOVE;
		break;
	case GLFW_KEY_DOWN:		// Look Down (along y axis)
		looky -= CAMERASPEED;
		break;
	case GLFW_KEY_UP:		// Look Up (along y axis)
		looky += CAMERASPEED;
		break;
	}
	
	////QUESTION 3: Making Speeder move
	//if (key == GLFW_KEY_P && action == GLFW_PRESS)
	//{
	//	speederValue++;
	//	if (speederValue == 10)
	//		speederValue = 7;
	//}

	//if (key == GLFW_KEY_W && action == GLFW_PRESS)
	//{
	//	speederUp = true;
	//}
	//if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	//{
	//	speederUp = false;
	//}

	//if (key == GLFW_KEY_D && action == GLFW_PRESS)
	//{
	//	speederRight = true;
	//}
	//if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	//{
	//	speederRight = false;
	//}

	//if (key == GLFW_KEY_S && action == GLFW_PRESS)
	//{
	//	if (!speederUp)
	//		speederDown = true;
	//}
	//if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	//{
	//	speederDown = false;
	//}

	//if (key == GLFW_KEY_A && action == GLFW_PRESS)
	//{
	//	if (!speederRight)
	//		speederLeft = true;
	//}
	//if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	//{
	//	speederLeft = false;
	//}

	////QUESTION 4: Tossing a bomb
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	//{
	//	tossaBomb = true;
	//}
	
	//Keys 1 through 8 will turn on and off the 8 point lights on the scene
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[0].lightIsOn = !(::g_pLightManager->vecLights[0].lightIsOn);
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[1].lightIsOn = !(::g_pLightManager->vecLights[1].lightIsOn);
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[2].lightIsOn = !(::g_pLightManager->vecLights[2].lightIsOn);
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[3].lightIsOn = !(::g_pLightManager->vecLights[3].lightIsOn);
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[4].lightIsOn = !(::g_pLightManager->vecLights[4].lightIsOn);
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[5].lightIsOn = !(::g_pLightManager->vecLights[5].lightIsOn);
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[6].lightIsOn = !(::g_pLightManager->vecLights[6].lightIsOn);
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		::g_pLightManager->vecLights[7].lightIsOn = !(::g_pLightManager->vecLights[7].lightIsOn);


}

int main()
{
	srand(time(NULL));
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1080, 720,
		"Snowspeeder Funsies",
		NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";

	::g_pShaderManager->setBasePath("assets//shaders//");

	if (!::g_pShaderManager->createProgramFromFile(
		"simpleShader", vertShader, fragShader))
	{
		std::cout << "Failed to create shader program. Shutting down." << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		return -1;
	}
	std::cout << "The shaders comipled and linked OK" << std::endl;
	GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("simpleShader");

	std::string* plyDirects;
	std::string* plyNames;
	int numPlys;

	if (!readPlysToLoad(&plyDirects, &plyNames, &numPlys))
	{
		std::cout << "Couldn't find files to read. Giving up hard.";
		return -1;
	}

	for (int i = 0; i < numPlys; i++)
	{	//Load each file into a VAO object
		cMesh newMesh;
		newMesh.name = plyNames[i];
		if (!LoadPlyFileIntoMesh(plyDirects[i], newMesh))
		{
			std::cout << "Didn't load model" << std::endl;
		}
		g_vecMesh.push_back(newMesh);
		if (!::g_pVAOManager->loadMeshIntoVAO(newMesh, shaderID))
		{
			std::cout << "Could not load mesh into VAO" << std::endl;
		}
	}

	int numEntities;
	if (!readEntityDetails(&g_vecGameObject, &numEntities, &g_vecObjStart))
	{
		std::cout << "Unable to find game objects for placement." << std::endl;
	}

	//QUESTION 1: Make the Walkers
	for (int index = 0; index < 3; index++)
	{
		cGameObject* newObj = new cGameObject();
		newObj->meshName = "ATAT";
		float randX = rand() % 101;
		randX = (randX / 100) - 5 + index;
		float positionX = randX;
		float randZ = rand() % 381;
		randZ = (randZ / 100) + 3.80;
		float positionZ = randZ;
		newObj->position.x = positionX;
		newObj->position.y = 1.0f;
		newObj->position.z = positionZ;

		cMesh thisMesh;
		for (unsigned int i = 0; i < ::g_vecMesh.size(); i++)
		{
			if (::g_vecGameObject[0]->meshName == ::g_vecMesh[i].name)
			{
				thisMesh = ::g_vecMesh[i];
				break;
			}
		}
		float closestTriDist = 99999;
		float closestTriYValue = 0;
		for (int indexT = 0; indexT < thisMesh.numberOfTriangles; indexT++)
		{
			int ptA = thisMesh.pTriangles[indexT].vertex_ID_0;
			int ptB = thisMesh.pTriangles[indexT].vertex_ID_1;
			int ptC = thisMesh.pTriangles[indexT].vertex_ID_2;

			glm::vec3 vertexA = glm::vec3(thisMesh.pVertices[ptA].x, 0.0f, thisMesh.pVertices[ptA].z);
			glm::vec3 vertexB = glm::vec3(thisMesh.pVertices[ptB].x, 0.0f, thisMesh.pVertices[ptB].z);
			glm::vec3 vertexC = glm::vec3(thisMesh.pVertices[ptC].x, 0.0f, thisMesh.pVertices[ptC].z);
			glm::vec3 spherePos = glm::vec3(newObj->position.x, 1.0f, newObj->position.z);

			glm::vec3 closestPoint = ClosestPtPointTriangle(spherePos, vertexA, vertexB, vertexC);

			float distance = glm::distance(newObj->position, closestPoint);
			if (distance < closestTriDist)
			{
				closestTriDist = distance;
				closestTriYValue = (thisMesh.pVertices[ptA].y + thisMesh.pVertices[ptB].y + thisMesh.pVertices[ptC].y) / 3;
			}
		}

		newObj->position.y = closestTriYValue;

		newObj->orientation2.y = -1.0f;
		newObj->diffuseColour.x = 0.5f;
		newObj->diffuseColour.y = 0.5f;
		newObj->diffuseColour.z = 0.5f;
		newObj->bIsUpdatedInPhysics = false;
		newObj->scale = 0.075f;
		::g_vecGameObject.push_back(newObj);

		cGameObject* ATATSphere = new cGameObject();

		ATATSphere->meshName = "Sphere";

		ATATSphere->position.x = newObj->position.x;
		ATATSphere->position.y = newObj->position.y + 0.25f;
		ATATSphere->position.z = newObj->position.z;

		ATATSphere->scale = 0.25f;

		ATATSphere->diffuseColour.x = 0.0f;
		ATATSphere->diffuseColour.y = 0.0f;
		ATATSphere->diffuseColour.z = 0.0f;

		ATATSphere->wireFrame = true;

		ATATSphere->bIsUpdatedInPhysics = false;

		iShape* myNewShape = g_pShapeFactory->CreateShape("ATATSPHERE", ATATSphere, newObj);
		cATATSphere* myNewSphere = (cATATSphere*)myNewShape;
		g_vecShapes.push_back(myNewSphere);
		g_vecGameObject.push_back(ATATSphere);
	}

	//QUESTION 2: Make the Speeders
	for (int index = 0; index < 3; index++)
	{
		cGameObject* newObj = new cGameObject();
		newObj->meshName = "Speeder";
		float randX = rand() % 121;
		randX = (randX / 100) - 10.9 + index*2;
		float positionX = randX;
		float randZ = rand() % 125;
		randZ = (randZ / 100) -12;
		float positionZ = randZ;
		newObj->position.x = positionX;
		newObj->position.y = 1.0f;
		newObj->position.z = positionZ;

		cMesh thisMesh;
		for (unsigned int i = 0; i < ::g_vecMesh.size(); i++)
		{
			if (::g_vecGameObject[0]->meshName == ::g_vecMesh[i].name)
			{
				thisMesh = ::g_vecMesh[i];
				break;
			}
		}
		float closestTriDist = 99999;
		float closestTriYValue = 0;
		for (int indexT = 0; indexT < thisMesh.numberOfTriangles; indexT++)
		{
			int ptA = thisMesh.pTriangles[indexT].vertex_ID_0;
			int ptB = thisMesh.pTriangles[indexT].vertex_ID_1;
			int ptC = thisMesh.pTriangles[indexT].vertex_ID_2;

			glm::vec3 vertexA = glm::vec3(thisMesh.pVertices[ptA].x, 0.0f, thisMesh.pVertices[ptA].z);
			glm::vec3 vertexB = glm::vec3(thisMesh.pVertices[ptB].x, 0.0f, thisMesh.pVertices[ptB].z);
			glm::vec3 vertexC = glm::vec3(thisMesh.pVertices[ptC].x, 0.0f, thisMesh.pVertices[ptC].z);
			glm::vec3 spherePos = glm::vec3(newObj->position.x, 1.0f, newObj->position.z);

			glm::vec3 closestPoint = ClosestPtPointTriangle(spherePos, vertexA, vertexB, vertexC);

			float distance = glm::distance(newObj->position, closestPoint);
			if (distance < closestTriDist)
			{
				closestTriDist = distance;
				closestTriYValue = (thisMesh.pVertices[ptA].y + thisMesh.pVertices[ptB].y + thisMesh.pVertices[ptC].y) / 3;
			}
		}

		newObj->position.y = closestTriYValue + 0.5f;

		newObj->orientation2.y = 0.7f;
		newObj->diffuseColour.x = 1.0f;
		newObj->diffuseColour.y = 0.0f;
		newObj->diffuseColour.z = 0.1f;
		newObj->bIsUpdatedInPhysics = false;
		newObj->scale = 0.001f;
		::g_vecGameObject.push_back(newObj);
	}
	


	std::cout << glGetString(GL_VENDOR) << " "
		<< glGetString(GL_RENDERER) << ", "
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("simpleShader");

	GLint uniLoc_mModel = glGetUniformLocation(currentProgID, "mModel");
	GLint uniLoc_mView = glGetUniformLocation(currentProgID, "mView");
	GLint uniLoc_mProjection = glGetUniformLocation(currentProgID, "mProjection");

	GLint uniLoc_materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");

	::g_pLightManager->createLights();	// There are 10 lights in the shader
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);

	glEnable(GL_DEPTH);

	double lastTimeStep = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4x4 m, p;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		::g_pLightManager->CopyInfoToShader();

		//QUESTION 3: Making Speeder move
		if (speederValue > 3)
		{
			if (speederUp)
			{
				g_vecGameObject[speederValue]->position.z -= 0.03;
			}
			else if (speederDown)
			{
				g_vecGameObject[speederValue]->position.z += 0.03;
			}

			if (speederRight)
			{
				g_vecGameObject[speederValue]->position.x += 0.03;
			}
			else if (speederLeft)
			{
				g_vecGameObject[speederValue]->position.x -= 0.03;
			}

			cMesh thisMesh;
			for (unsigned int i = 0; i < ::g_vecMesh.size(); i++)
			{
				if (::g_vecGameObject[0]->meshName == ::g_vecMesh[i].name)
				{
					thisMesh = ::g_vecMesh[i];
					break;
				}
			}
			float closestTriDist = 99999;
			float closestTriYValue = 0;
			for (int indexT = 0; indexT < thisMesh.numberOfTriangles; indexT++)
			{
				int ptA = thisMesh.pTriangles[indexT].vertex_ID_0;
				int ptB = thisMesh.pTriangles[indexT].vertex_ID_1;
				int ptC = thisMesh.pTriangles[indexT].vertex_ID_2;

				glm::vec3 vertexA = glm::vec3(thisMesh.pVertices[ptA].x, 0.0f, thisMesh.pVertices[ptA].z);
				glm::vec3 vertexB = glm::vec3(thisMesh.pVertices[ptB].x, 0.0f, thisMesh.pVertices[ptB].z);
				glm::vec3 vertexC = glm::vec3(thisMesh.pVertices[ptC].x, 0.0f, thisMesh.pVertices[ptC].z);
				glm::vec3 spherePos = glm::vec3(::g_vecGameObject[speederValue]->position.x, 1.0f, ::g_vecGameObject[speederValue]->position.z);

				glm::vec3 closestPoint = ClosestPtPointTriangle(spherePos, vertexA, vertexB, vertexC);

				float distance = glm::distance(::g_vecGameObject[speederValue]->position, closestPoint);
				if (distance < closestTriDist)
				{
					closestTriDist = distance;
					closestTriYValue = (thisMesh.pVertices[ptA].y + thisMesh.pVertices[ptB].y + thisMesh.pVertices[ptC].y) / 3;
				}
			}

			::g_vecGameObject[speederValue]->position.y = closestTriYValue + 0.5f;
		}

		//QUESTION 4: Tossing bombs
		if (tossaBomb)
		{
			cGameObject* newObj = new cGameObject();

			newObj->meshName = "Sphere";

			newObj->position.x = ::g_vecGameObject[speederValue]->position.x;
			newObj->position.y = ::g_vecGameObject[speederValue]->position.y + 0.2f;
			newObj->position.z = ::g_vecGameObject[speederValue]->position.z;

			newObj->scale = 0.05f;

			newObj->diffuseColour.x = 1.0f;
			newObj->diffuseColour.y = 0.5f;
			newObj->diffuseColour.z = 0.5f;

			newObj->bIsUpdatedInPhysics = true;

			float bombSpeed = 1.5f;
			float xVel = 0.0f, zVel = 0.0f;
			float yVel = bombSpeed;

			if (speederUp)
				zVel = -bombSpeed;
			else if (speederDown)
				zVel = bombSpeed;

			if (speederRight)
				xVel = bombSpeed;
			else if (speederLeft)
				xVel = -bombSpeed;

			iShape* myNewShape = g_pShapeFactory->CreateShape("SPHERE", newObj);
			cSphere* myNewSphere = (cSphere*)myNewShape;
			myNewSphere->setVelocity(glm::vec3(xVel, yVel, zVel));
			g_vecShapes.push_back(myNewSphere);
			g_vecGameObject.push_back(newObj);
			tossaBomb = false;
		}

		unsigned int sizeOfVector = ::g_vecGameObject.size();	//*****//
		for (int index = 0; index != sizeOfVector; index++)
		{
			// Is there a game object? 
			if (::g_vecGameObject[index] == 0 || ::g_vecGameObject[index]->wireFrame == true)	//if ( ::g_GameObjects[index] == 0 )
			{	// Nothing to draw
				continue;		// Skip all for loop code and go to next
			}

			// Was near the draw call, but we need the mesh name
			std::string meshToDraw = ::g_vecGameObject[index]->meshName;		//::g_GameObjects[index]->meshName;

			sVAOInfo VAODrawInfo;
			if (::g_pVAOManager->lookupVAOFromName(meshToDraw, VAODrawInfo) == false)
			{	// Didn't find mesh
				continue;
			}

			m = glm::mat4x4(1.0f);	

			glm::mat4 matPreRotX = glm::mat4x4(1.0f);
			matPreRotX = glm::rotate(matPreRotX, ::g_vecGameObject[index]->orientation.x,
				glm::vec3(0.0f, 0.0f, 1.0f));
			m = m * matPreRotX;
			
			glm::mat4 matPreRotY = glm::mat4x4(1.0f);
			matPreRotY = glm::rotate(matPreRotY, ::g_vecGameObject[index]->orientation.y,
				glm::vec3(0.0f, 0.0f, 1.0f));
			m = m * matPreRotY;
			
			glm::mat4 matPreRotZ = glm::mat4x4(1.0f);
			matPreRotZ = glm::rotate(matPreRotZ, ::g_vecGameObject[index]->orientation.z,
				glm::vec3(0.0f, 0.0f, 1.0f));
			m = m * matPreRotZ;

			glm::mat4 trans = glm::mat4x4(1.0f);
			trans = glm::translate(trans,
				::g_vecGameObject[index]->position);
			m = m * trans;

			glm::mat4 matPostRotZ = glm::mat4x4(1.0f);
			matPostRotZ = glm::rotate(matPostRotZ, ::g_vecGameObject[index]->orientation2.z,
				glm::vec3(0.0f, 0.0f, 1.0f));
			m = m * matPostRotZ;

			glm::mat4 matPostRotY = glm::mat4x4(1.0f);
			matPostRotY = glm::rotate(matPostRotY, ::g_vecGameObject[index]->orientation2.y,
				glm::vec3(0.0f, 1.0f, 0.0f));
			m = m * matPostRotY;


			glm::mat4 matPostRotX = glm::mat4x4(1.0f);
			matPostRotX = glm::rotate(matPostRotX, ::g_vecGameObject[index]->orientation2.x,
				glm::vec3(1.0f, 0.0f, 0.0f));
			m = m * matPostRotX;

			float finalScale = ::g_vecGameObject[index]->scale;
			glm::mat4 matScale = glm::mat4x4(1.0f);
			matScale = glm::scale(matScale,
				glm::vec3(finalScale,
						finalScale,
						finalScale));
			m = m * matScale;

			p = glm::perspective(0.6f,			// FOV
				ratio,		// Aspect ratio
				0.1f,			// Near (as big as possible)
				1000.0f);	// Far (as small as possible)

							// View or "camera" matrix
			glm::mat4 v = glm::mat4(1.0f);	// identity

			g_cameraXYZ.x = camPosx;
			g_cameraXYZ.y = camPosy;
			g_cameraXYZ.z = camPosz;

			g_cameraTarget_XYZ.x = camPosx + lookx;
			g_cameraTarget_XYZ.y = looky;
			g_cameraTarget_XYZ.z = camPosz + lookz;

			v = glm::lookAt(g_cameraXYZ,						// "eye" or "camera" position
				g_cameraTarget_XYZ,		// "At" or "target" 
				glm::vec3(0.0f, 1.0f, 0.0f));	// "up" vector


			glUniform4f(uniLoc_materialDiffuse,
				::g_vecGameObject[index]->diffuseColour.r,
				::g_vecGameObject[index]->diffuseColour.g,
				::g_vecGameObject[index]->diffuseColour.b,
				::g_vecGameObject[index]->diffuseColour.a);


			//        glUseProgram(program);
			::g_pShaderManager->useShaderProgram("simpleShader");

			glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(m));

			glUniformMatrix4fv(uniLoc_mView, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(v));

			glUniformMatrix4fv(uniLoc_mProjection, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(p));

			if(::g_vecGameObject[index]->wireFrame)
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBindVertexArray(VAODrawInfo.VAO_ID);

			glDrawElements(GL_TRIANGLES,
				VAODrawInfo.numberOfIndices,
				GL_UNSIGNED_INT,
				0);

			glBindVertexArray(0);

		}//for ( int index = 0...

		double curTime = glfwGetTime();
		double deltaTime = curTime - lastTimeStep;

		g_pShapeFactory->physicsStep(deltaTime);

		lastTimeStep = curTime;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}