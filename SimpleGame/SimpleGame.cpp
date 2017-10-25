/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Object.h"
#include "sCeneMgr.h"
Renderer *g_Renderer = NULL;
//Object *p_object = new Object();
sCeneMgr* scene = NULL;


void BuildObject(void)
{
	scene->buildObjects();
	//Object *p_object = new Object();
	//p_object->Setposition(100, 100, 10);
	//p_object->Setsize(20);

}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	//DWORD currTime = timeGetTime();
	//DWORD elapsedTime = currTime - g_prevTime;
	//g_prevTime = currTime;
	scene->update();
	scene->Draw();
	//scene->
	//for (int i = 0; i < 50; i++)
	//{
	//	g_Renderer->DrawSolidRect(scene.Getobject(i)->GetpositionX(), scene.Getobject(i)->GetpositionY(),0, 20, 1, 0, 1, 1);
	//
	//}
	//scene.update();
	
	

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
	//if (button == GLUT_LEFT_BUTTON &&state == GLUT_DOWN)
	//{
	//	g_LButtonDown = true;
	//}
	//if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	//{
	//	if (g_LButtonDown)
	//	{
	//
	//	}
	//}
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	//BuildObject();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	//g_Renderer = new Renderer(500, 500);
	//if (!g_Renderer->IsInitialized())
	//{
	//	std::cout << "Renderer could not be initialized.. \n";
	//}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	scene = new sCeneMgr(500,500);
	BuildObject();
	glutMainLoop();

	delete g_Renderer;
	delete scene;

    return 0;
}

