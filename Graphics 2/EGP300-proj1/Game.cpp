#include "Game.h"



Game::Game(void)
{
}

Game::~Game(void)
{
	endGame();
}

void Game::ResetCamera(void)
{
	mpCamera->hardReset( width, height );
}

void Game::hookResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	mpDebug->screenHeight = height;
	mpDebug->screenWidth = width;

	glViewport(0,0, width, height);

	mpCamera->setProjectionMatrix( (float)width, (float)height );
}

void Game::hookMouseMoved(int x, int y)
{
	if (mouseFree)
		return;

	if (x == width / 2 && y == height / 2)
		return;

	float dX = (width / 2.0f) - x;
	float dY = (height / 2.0f) - y;

	dX = -glm::radians(dX) * m_CameraLookSpeed;
	dY = -glm::radians(dY) * m_CameraLookSpeed;

	mpCamera->setPitch(mpCamera->getPitch() - dY);
	mpCamera->setYaw(mpCamera->getYaw() - dX);

	glutWarpPointer( width / 2, height / 2);
}

void Game::hookMousePassive(int x, int y)
{
	if (mouseFree)
		return;

	if (x == width / 2 && y == height / 2)
		return;

	glutWarpPointer( width / 2, height / 2);
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mat4x4 mView = mpCamera->getViewMatrix();
	mat4x4 proj = mpCamera->getProjectionMatrix();
	mat4x4 viewProj = proj * mView;

	mpResourceManager->drawAllObjects(mView, proj, viewProj, shaderManager, mpShaderManager);

	//glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	mpDebug->drawMessages();
}

void Game::update()
{
	mpDebug->update();
	double deltaTime = mpDebug->getDeltaTime();

		if (Paused)
			PausedUpdate(deltaTime);
		else
			UnpausedUpdate(deltaTime);

		FixedUpdate(deltaTime);
	
}

void Game::PausedUpdate(double deltaTime)
{
	mpDebug->addTextToScreen("PAUSED", vec2(width / 2.0, height - 40));
}

void Game::UnpausedUpdate(double deltaTime)
{
	mpParticleForceManager->updateForces(deltaTime);
	mpResourceManager->updateObjects(mpCamera->getPos(), (float)deltaTime);	
	mpContactHandler->update(deltaTime);
	mpShaderManager->update((float)deltaTime);

	WorldUpdate(deltaTime);
}

void Game::FixedUpdate(double deltaTime)
{
	mpCamera->update();

	if (mpDebug->getDebugMode())
	{
		vec3 temp = mpCamera->getPos();
		mpDebug->addTextToScreen("Cam Pos:", vec2(10, height - 70), false, 0.0f);
		mpDebug->addTextToScreen("X: " + std::to_string(temp.x), vec2(10, height - 90), false, 0.0f);
		mpDebug->addTextToScreen("Y: " + std::to_string(temp.y), vec2(10, height - 110), false, 0.0f);
		mpDebug->addTextToScreen("Z: " + std::to_string(temp.z), vec2(10, height - 130), false, 0.0f);
		
		WorldDebug();
		
	}
	
	WorldFixedUpdate(deltaTime);
}

void Game::WorldDebug()
{
	if (help)
	{
		mpDebug->addTextToScreen("WSAD+EQ: Movement", vec2(10, height * .5f + 200));
		mpDebug->addTextToScreen("ESC: Quit", vec2(10, height * .5f + 180));
		mpDebug->addTextToScreen("r: Reset World / R: Reset Camera ", vec2(10, height * .5f + 160));
		mpDebug->addTextToScreen("F: Switch Movement Mode (Smooth vs Rigid)", vec2(10, height * .5f + 140));
		mpDebug->addTextToScreen("P: Toggle Pause", vec2(10, height * .5f + 120));
		mpDebug->addTextToScreen("L: Release / Lock Mouse", vec2(10, height * .5f + 100));
		mpDebug->addTextToScreen("F1: Toggle Debug Mode", vec2(10, height * .5f + 80));
		mpDebug->addTextToScreen("F2: Toggle Wireframe Mode", vec2(10, height * .5f + 60));
		mpDebug->addTextToScreen("F3: Capture Screenshot", vec2(10, height * .5f + 40));
		mpDebug->addTextToScreen("F11: Toggle Fullscreen", vec2(10, height * .5f + 20));
		mpDebug->addTextToScreen("H: Toggle Help Text", vec2(10, height * .5f));
	}
}

void Game::WorldUpdate(double deltaTime)
{
	mpResourceManager->getObject("fishy")->modifyRotation(.1f, .1f, .1f);
	//PhyshyFriends->update(mpDebug->getDeltaTime());

}
void Game::WorldFixedUpdate(double deltaTime)
{

	if (m_cloudSkybox->getIsPrefab() && mpCamera->getPos().y >= mpResourceManager->getObject("water")->getPos().y)
	{
		//cout << "switch to cloud box\n";
		m_cloudSkybox->setIsPrefab(false);
		m_underWaterSkybox->setIsPrefab(true);
	}
	else if (m_underWaterSkybox->getIsPrefab() && mpCamera->getPos().y < mpResourceManager->getObject("water")->getPos().y)
	{
		//cout << "switch to water box\n";
		m_cloudSkybox->setIsPrefab(true);
		m_underWaterSkybox->setIsPrefab(false);
	}

	m_underWaterSkybox->setPos(mpCamera->getPos());
	m_cloudSkybox->setPos(mpCamera->getPos());


	mpTerrainManager->update(mpCamera);
}

void Game::start(int argNum, char* args[])
{
	
	m_CameraMoveSpeed = .5f;
	m_CameraLookSpeed = .2f;
	wireframe = false;
	fog = false;
	mouseFree = false;
	Paused = true;
	fullscreen = false;
	help = false;
	
	mpDebug = new Debug();
	mpCamera = new Camera();
	mpResourceManager = new ResourceManager();
	mpTerrainManager = new TerrainManager();
	mpShaderManager = new Shader_Manager();
	mpParticleForceManager = new ParticleForceRegistry();
	mpContactHandler = new ContactHandler();
	mpWorldHandler = new WorldHandler();

	mpDebug->init(mpResourceManager, "Assets/TextMaterials/whiteTextMaterial.mtl");
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);   
	shaderManager.InitializeStockShaders();
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	  
	width = 800;
	height = 600;

	mpDebug->screenHeight = height;
	mpDebug->screenWidth = width;

	if( argNum > 1)
	{
		for (int i = 0; i < argNum; i++)
		{
			cout << args[i];
		}
	}

	setUpWorld(argNum, args);
}

void Game::setUpWorld(int argNum, char* args[])
{

#pragma region WaterWorldSetup
	
			int waterTerrainSize = 8000;
			int terrainSize = 3000;
			int terrainTriDensity = 100;

			mpResourceManager->LoadFile("Assets/Fish/FISHY.obj");

			m_cloudSkybox = new Skybox("Assets/Skybox/cloudbox2.jpg", mpResourceManager, 3000, "cloudSkybox");
			m_cloudSkybox->setIsPrefab(true);
			m_underWaterSkybox = new Skybox("Assets/WaterBox/waterSkybox.png", mpResourceManager, 3000, "underwaterSkybox");

			mpTerrainManager->createTerrain(mpResourceManager, "Assets/Heightmap/heightmap_water.jpg", "Assets/Heightmap/heightmap_waterTexture.jpg", terrainSize, 200, terrainTriDensity, "repeatHM", true);
			mpTerrainManager->setHeightOfRepeatingTerrain("repeatHM", -200.0f);

			m_waterMap = new Heightmap(mpResourceManager, "Assets/Water/waterTexture.jpg", waterTerrainSize, waterTerrainSize, 200, "water");
			m_waterMap->setPos(vec3(-waterTerrainSize * .5f, 150, -waterTerrainSize * .5f));
			mpResourceManager->applyShaderToObject(m_waterMap, "waveShader");
			
			Object* fishy = mpResourceManager->addNewObject("fishy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			fishy->Translate(50, 5, 50);
			//mpResourceManager->applyShaderToObject("fishy", "diffuse");
			//fishy->setIsPrefab(true);

			physhy = mpResourceManager->addNewPhysicsObject("physhy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			physhy->init(Vector3D(50, 5, 40), Vector3D(0, 0, 3));

			/*
			BillboardedTexture* billboard1 = new BillboardedTexture(mpResourceManager, "Assets/Cloud/cloud.png", true, "cloud2");
			billboard1->setPos(vec3(-400, 400, -800));
			billboard1->setScale(vec3(.6f, .6f, .6f));

			BillboardedTexture* billboard2 = new BillboardedTexture(mpResourceManager, "Assets/Cloud/cloud.png", true, "cloud3");
			billboard2->setPos(vec3(700, 400, 700));
			billboard2->setScale(vec3(.6f, .6f, .6f));

			BillboardedTexture* billboard = new BillboardedTexture(mpResourceManager, "Assets/Cloud/cloud.png", true, "cloud1");
			billboard->setPos(vec3(-900, 400, 300));
			billboard->setScale(vec3(.6f, .6f, .6f));


			BillboardedTexture* grass = new BillboardedTexture(mpResourceManager, "Assets/Grass/grass.png", false, "grass");
			grass->setPos(vec3(270, -100, 270));
			grass->setScale(vec3(.25f, .25f, .25f));

			BillboardedTexture* grass2 = new BillboardedTexture(mpResourceManager, "Assets/Grass/grass.png", false, "grass2");
			grass2->setPos(vec3(270, -100, 270));
			grass2->setScale(vec3(.25f, .25f, .25f));

			grass2->setRotation(vec3(0, PI / 2.3f, 0));
			*/

			//PhyshyFriends = new ParticleEffect(mpResourceManager, "FriendsSpawn", vec3(5, 0, 0), 100, 10, vec3(1, 0, 0));
			//PhyshyFriends->startEffect("Assets/Fish");

#pragma endregion WaterWorldSetup

	//LOAD EVERYTHING EVER
	mpWorldHandler->loadTextFile("Assets/WorldFiles/PhysicsDemo.txt", mpResourceManager, mpContactHandler, mpParticleForceManager);

	ResetCamera();
}

/*Controls:

Arrow keys: alter cameras pitch and yaw
F1: Debug Text
F2: Wireframe Toggle
F3: Screen Shot
F11: FullScreen mode

*/

void Game::hookSpecialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		mpCamera->setYaw(mpCamera->getYaw() + 0.1f);
	}

	if (key == GLUT_KEY_RIGHT)
	{
		mpCamera->setYaw(mpCamera->getYaw() - 0.1f);
	}

	if( key == GLUT_KEY_UP)
	{
		mpCamera->setPitch(mpCamera->getPitch() + 0.05f);
	}
	if( key == GLUT_KEY_DOWN)
	{
		mpCamera->setPitch(mpCamera->getPitch() - 0.05f);
	}
	if( key == GLUT_KEY_F3)
	{
		mpCamera->screenShot(width, height);
		mpDebug->addMessageToScreen("Screenshot captured");
	}
	if( key == GLUT_KEY_F1)
	{
		mpDebug->toggleDebugMode();
		if (mpDebug->getDebugMode())
			mpDebug->addMessageToScreen("Debug Mode: ON");
		else
			mpDebug->addMessageToScreen("Debug Mode: OFF");
	}
	if( key == GLUT_KEY_F2)
	{
		if(!wireframe)
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			wireframe = true;
			mpDebug->addMessageToScreen("Drawing Mode: Wireframe");
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			wireframe = false;
			mpDebug->addMessageToScreen("Drawing Mode: Normal");
		}
	}
	if (key == GLUT_KEY_F11)
	{
		if (!fullscreen)
		{
			fullScreen_Saves.x = (float)width;
			fullScreen_Saves.y = (float)height;
			fullScreen_Saves.z = (float)glutGet(GLUT_WINDOW_X);
			fullScreen_Saves.w = (float)glutGet(GLUT_WINDOW_Y);
			glutFullScreen();
			fullscreen = true;
			mpDebug->addMessageToScreen("FullScreen: true");
		}
		else
		{
			glutPositionWindow((int)fullScreen_Saves.z, (int)fullScreen_Saves.w);
			glutReshapeWindow((int)fullScreen_Saves.x, (int)fullScreen_Saves.y);
			
			fullscreen = false;
			mpDebug->addMessageToScreen("FullScreen: false");

		}
		
	}

}

/*Controls:

WSAD: strafing movement
Q / E: Vertical movement
Esc: Quit
R: Reset Camera Pos
F: Activate acceleration and velocity of camera
P: Pause game
L: free mouse
H: help

*/
void Game::hookKey(unsigned char key, int x, int y)
{
	int shiftFactor = 4;
	if (key == 'w')
	{
		mpCamera->command(vec3(1, 0, 0));
	}
	if (key == 'W')
	{
		mpCamera->command(vec3(1 * shiftFactor, 0, 0));
	}
	if (key == 's')
	{
		mpCamera->command(vec3(-1, 0, 0));
	}
	if (key == 'S')
	{
		mpCamera->command(vec3(-1 * shiftFactor, 0, 0));
	}
	if (key == 'e')
	{
		mpCamera->command(vec3(0, 1, 0));
	}
	if (key == 'E')
	{
		mpCamera->command(vec3(0, 1 * shiftFactor, 0));
	}
	if (key == 'q')
	{
		mpCamera->command(vec3(0, -1, 0));
	}
	if (key == 'Q')
	{
		mpCamera->command(vec3(0, -1 * shiftFactor, 0));
	}
	if (key == 'a')
	{
		mpCamera->command(vec3(0, 0, -1));
	}
	if (key == 'A')
	{
		mpCamera->command(vec3(0, 0, -1 * shiftFactor));
	}
	if (key == 'd')
	{
		mpCamera->command(vec3(0, 0, 1));
	}
	if (key == 'D')
	{
		mpCamera->command(vec3(0, 0, 1 * shiftFactor));
	}
	if (key == 27) //escape
	{
		endGame();
	}
	if (key == 'f' || key == 'F')
	{
		mpCamera->FlyMode(!mpCamera->FlyMode());
		if (mpCamera->FlyMode())
			mpDebug->addMessageToScreen("Camera Movement: Smooth/Fly");
		else
			mpDebug->addMessageToScreen("Camera Movement: Rigid");
	}
	if (key == 'r')
	{
		//mpCamera->hardReset(width, height);
		mpDebug->addMessageToScreen("Reset World");
		ResetWorld();
	}
	if (key == 'R')
	{
		mpDebug->addMessageToScreen("Reset Camera");
		mpCamera->hardReset(getScreenWidth(), getScreenHeight());
	}
	if (key == 'p' || key == 'P')
	{
		Paused = !Paused;
	}
	if (key == 'l' || key == 'L')
	{
		mouseFree = !mouseFree;
		if (mouseFree)
			mpDebug->addMessageToScreen("Mouse: Free");
		else
			mpDebug->addMessageToScreen("Mouse: Locked");
	}
	if (key == 'h' || key == 'H')
	{
		help = !help;
	}

	/*if(key == 'g' || key == 'G')
	{
	if( fog)
	{
	fog =  false;
	glDisable(GL_FOG);
	cout << "no fog\n";
	}
	else
	{
	fog = true;
	glEnable(GL_FOG);
	cout << "yes fog\n";
	}
	}*/

}
	

void Game::endGame()
{
	IMG_Quit();

	delete mpParticleForceManager;
	delete mpResourceManager;
	delete mpCamera;
	delete mpTerrainManager;
	delete mpShaderManager;
	delete mpDebug;
	delete mpContactHandler;
	delete mpWorldHandler;

	exit(0);
}


void Game::ResetWorld()
{
	// radius, mass, pos, vel
	Paused = true;

	mpResourceManager->resetAllPhysicsObjects();
	/*
	mpResourceManager->getObject("Cube1")->resetObject();
	mpResourceManager->getObject("Cube2")->resetObject();
	mpResourceManager->getObject("Cube3")->resetObject();
	mpResourceManager->getObject("Cube4")->resetObject();
	mpResourceManager->getObject("Cube5")->resetObject();
	mpResourceManager->getObject("Cube6")->resetObject();
	mpResourceManager->getObject("Cube7")->resetObject();
	mpResourceManager->getObject("Cube8")->resetObject();

	mpResourceManager->getObject("Tet1")->resetObject();
	mpResourceManager->getObject("Tet2")->resetObject();
	mpResourceManager->getObject("Tet3")->resetObject();
	mpResourceManager->getObject("Tet4")->resetObject();

	physhy->setPos(50, 5, 40);
	*/
	mpResourceManager->getObject("fishy")->setRotation(glm::vec3(0, 0, 0));
}




