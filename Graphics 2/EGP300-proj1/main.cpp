

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#undef main

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Game.h"
#include "Debug.h"
#include "Camera.h"
#include "Model.h"
#include <vector>
#include <map>

#include <Arc/ArcCore.h>
#include <Arc/MemoryTracker.h>

#include <Arc/Map.h>
#include <Arc/ArrayList.h>

using namespace std;
using namespace Arc;

#pragma region new and happy variables

Game* game;

#pragma endregion

void ChangeSize(int w, int h)
{
	game->hookResize( w , h );
}

void myInit(int argNum, char* args[])
{
	game = new Game();
	game->start(argNum, args);
}

void RenderScene(void)
{
	game->render();
	glutSwapBuffers();
}

void Keys(unsigned char key, int x, int y)
{
	game->hookKey(key, x, y);
}

void MouseMove(int x, int y)
{ 
	game->hookMouseMoved(x, y);
}

void MouseMovePassive( int x, int y)
{
	game->hookMousePassive(x,y);
}

void SpecialKeys(int key, int x, int y)
{
	game->hookSpecialKey(key, x, y);
}

void myUpdate()
{
	game->update();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA| GLUT_DEPTH | GLUT_STENCIL);

	Arc_InitCore();

	cout << "Running Arc version " << Arc_GetVersionString() << endl;

	Arc_InitMemoryTracker();

	glutInitWindowSize(1280, 800);

	int sdlFlags = SDL_INIT_TIMER | SDL_INIT_AUDIO;
	if (SDL_Init(sdlFlags) != 0)
	{
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	int sdlImageFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if ((IMG_Init(sdlImageFlags) & sdlImageFlags) != sdlImageFlags)
	{
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glutCreateWindow("Game Phyics");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMovePassive);
	glutKeyboardFunc(Keys);
	glutIdleFunc(myUpdate);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	glutSetCursor(GLUT_CURSOR_NONE);

	myInit(argc, argv);

	glutMainLoop();
	return 0;
}