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

	//mpResourceManager->drawObject(mView, proj, viewProj, waterShaderManager, "water");
	mpResourceManager->drawAllObjects(mView, proj, viewProj, shaderManager);

	glClear(GL_DEPTH_BUFFER_BIT);
	mpDebug->drawMessages();
}

void Game::update()
{
	mpDebug->update();
	
		if (Paused)
			PausedUpdate();
		else
			UnpausedUpdate();

		FixedUpdate();
	
}

void Game::PausedUpdate()
{
	mpDebug->addTextToScreen("PAUSED", vec2(width / 2.0, height - 40));
}

void Game::UnpausedUpdate()
{
	mpResourceManager->updateObjects(mpCamera->getPos());	

	if (m_waterworld)
		waterWorldUpdate();
	if (m_space)
		spaceWorldUpdate();
}

void Game::FixedUpdate()
{
	mpCamera->update();

	if (mpDebug->getDebugMode())
	{
		vec3 temp = mpCamera->getPos();
		mpDebug->addTextToScreen("Cam Pos:", vec2(10, height - 70), false, 0.0f);
		mpDebug->addTextToScreen("X: " + std::to_string(temp.x), vec2(10, height - 90), false, 0.0f);
		mpDebug->addTextToScreen("Y: " + std::to_string(temp.y), vec2(10, height - 110), false, 0.0f);
		mpDebug->addTextToScreen("Z: " + std::to_string(temp.z), vec2(10, height - 130), false, 0.0f);
		
		if (m_space)
			spaceWorldDebug();
	}
	
	if (m_waterworld)
		waterWorldFixedUpdate();
	if (m_space)
		spaceWorldFixedUpdate();
}

void Game::spaceWorldDebug()
{
	if (PlanetToFollow != nullptr)
		mpDebug->addTextToScreen("Following: " + mpResourceManager->getObjectKey(PlanetToFollow) , vec2(width - 200, height - 40), false, 0.0f);
}

void Game::waterWorldUpdate()
{
	mpResourceManager->getObject("fishy")->modifyRotation(.1f, .1f, .1f);
	PhyshyFriends->update();
	waterShaderManager->update();
}
void Game::waterWorldFixedUpdate()
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

void Game::spaceWorldUpdate()
{
	if (PlanetToFollow != nullptr)
	{
		mpCamera->setPos(PlanetToFollow->getPos().x, mpCamera->getPos().y, PlanetToFollow->getPos().z);
	}
}
void Game::spaceWorldFixedUpdate()
{

}


void Game::start(int argNum, char* args[])
{
	m_space = true;
	m_waterworld = false;

	PlanetToFollow = nullptr;
	m_CameraMoveSpeed = .5f;
	m_CameraLookSpeed = .2f;
	wireframe = false;
	fog = false;
	mouseFree = false;
	Paused = true;
	fullscreen = false;

	mpDebug = new Debug();
	mpCamera = new Camera();
	mpDebug = new Debug();
	mpResourceManager = new ResourceManager();
	mpTerrainManager = new TerrainManager();
	waterShaderManager = new Shader_Manager();
	
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);   
	shaderManager.InitializeStockShaders();
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	/*GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	glFogi (GL_FOG_MODE, GL_LINEAR );
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, .02f);
	glHint (GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
	glFogf(GL_FOG_END, 100.0f);
	*/

	glEnable(GL_BLEND);
	  
	width = 800;
	height = 600;

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
	
	if(argNum > 1)
	{
		for( int i = 1; i < argNum; i++)
		{
			mpResourceManager->LoadFile(args[i]);
		}
	}
	else
	{

#pragma region standardPrefabSetup


		//mpResourceManager->LoadFile("Assets/StandardObjects/Cube/Cube.obj", "CubeTest");
		//mpResourceManager->LoadFile("Assets/StandardObjects/Sphere/Sphere.obj");
		//mpResourceManager->LoadFile("Assets/StandardObjects/Torus/Torus.obj");

		//Object* cube = mpResourceManager->addNewObject("cube", mpResourceManager->getObject("CubeTest")->getModelMap());
		//cube->setIsPrefab(true);
		//Object* sphere = mpResourceManager->addNewObject("sphere", mpResourceManager->getObject("Assets/Sphere/Sphere")->getModelMap());
		//Object* torus = mpResourceManager->addNewObject("torus", mpResourceManager->getObject("Assets/Torus/Torus")->getModelMap());

#pragma endregion standardPrefabSetup

#pragma region WaterWorldSetup
		
		if (m_waterworld)
		{

			int waterTerrainSize = 8000;
			int terrainSize = 3000;
			int terrainTriDensity = 100;

			waterShaderManager->init("WaterVertShader.vp", "WaterFragShader.fp");

			mpResourceManager->LoadFile("Assets/Fish/FISHY.obj");

			m_cloudSkybox = new Skybox("Assets/Skybox/cloudbox2.jpg", mpResourceManager, 3000, "cloudSkybox");
			m_cloudSkybox->setIsPrefab(true);
			m_underWaterSkybox = new Skybox("Assets/WaterBox/waterSkybox.png", mpResourceManager, 3000, "underwaterSkybox");

			mpTerrainManager->createTerrain(mpResourceManager, "Assets/Heightmap/heightmap_water.jpg", "Assets/Heightmap/heightmap_waterTexture.jpg", terrainSize, 200, terrainTriDensity, "repeatHM", true);
			mpTerrainManager->setHeightOfRepeatingTerrain("repeatHM", -200.0f);


			m_waterMap = new Heightmap(mpResourceManager, "Assets/Water/waterTexture.jpg", waterTerrainSize, waterTerrainSize, 200, "water");
			m_waterMap->setPos(vec3(-waterTerrainSize * .5f, 150, -waterTerrainSize * .5f));
			//m_waterMap->setIsPrefab(true);

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

			Object* fishy = mpResourceManager->addNewObject("fishy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			fishy->Translate(5, 5, 5);

			physhy = mpResourceManager->addNewPhysicsObject("physhy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			physhy->modifyVelocity(vec3(0, 0, 0));
			physhy->modifyAcceleration(vec3(0, 0, .005));

			PhyshyFriends = new ParticleEffect(mpResourceManager, "FriendsSpawn", vec3(5, 0, 0), 100, 300, vec3(1, 0, 0));
			PhyshyFriends->startEffect("Assets/Fish");

		}
#pragma endregion WaterWorldSetup

#pragma region SpaceWorldSetup

		//Model scale:  1 : 12,740,000
		if (m_space)
		{
			float earthVelocity = 30.3f;
			float planetSizeScaleDiv = 10.0f;

			mpResourceManager->LoadFile("Assets/Planets/EarthPretty.obj");

			Sun = mpResourceManager->addNewPhysicsObject("Sun", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Sun", "SunModel", "Planet", "SunMat", "Assets/Planets/Sun.mtl");
			Sun->Translate(0, 0, 0);
			Sun->setScale(139.2f / 10.0f);
			Sun->setMass(0.0553);

			Mercury = mpResourceManager->addNewPhysicsObject("Mercury", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Mercury", "MercuryModel", "Planet", "MercuryMat", "Assets/Planets/Mercury.mtl");
			Mercury->Translate(387.0f, 0, 0);
			Mercury->setScale(.4879f / planetSizeScaleDiv);
			Mercury->setMass(0.0553);
			Mercury->setVelocity(vec3(0, 0, earthVelocity * 1.607f));

			Venus = mpResourceManager->addNewPhysicsObject("Venus", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Venus", "VenusModel", "Planet", "VenusMat", "Assets/Planets/Venus.mtl");
			Venus->Translate(723.0f, 0, 0);
			Venus->setScale(1.2104f / planetSizeScaleDiv);
			Venus->setMass(0.815);
			Venus->setVelocity(vec3(0, 0, earthVelocity * 1.174f));

			Earth = mpResourceManager->addNewPhysicsObject("Earth", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth->Translate(1000.0f, 0, 0);
			Earth->setMass(5.9726);
			Earth->setScale(1.2742f / planetSizeScaleDiv);
			Earth->setMass(1);
			Earth->setVelocity(vec3(0, 0, earthVelocity));

			Moon = mpResourceManager->addNewPhysicsObject("Moon", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Moon", "MoonModel", "Planet", "MoonMat", "Assets/Planets/Moon.mtl");
			Moon->Translate(1000.257f, 0, 0);
			Moon->setScale(.3474f / planetSizeScaleDiv);
			Moon->setMass(0.012306f);
			Moon->setVelocity(vec3(0, 0, 1.022f));

			Mars = mpResourceManager->addNewPhysicsObject("Mars", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Mars", "MarsModel", "Planet", "MarsMat", "Assets/Planets/Mars.mtl");
			Mars->Translate(1524.0f, 0, 0);
			Mars->setScale(.6779f / planetSizeScaleDiv);
			Mars->setMass(0.107f);
			Mars->setVelocity(vec3(0, 0, earthVelocity * 0.802f));

			Jupiter = mpResourceManager->addNewPhysicsObject("Jupiter", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Jupiter", "JupiterModel", "Planet", "JupiterMat", "Assets/Planets/Jupiter.mtl");
			Jupiter->Translate(5203.0f, 0, 0);
			Jupiter->setScale(13.9822f / planetSizeScaleDiv);
			Jupiter->setMass(317.83f);
			Jupiter->setVelocity(vec3(0, 0, earthVelocity * 0.434f));

			Saturn = mpResourceManager->addNewPhysicsObject("Saturn", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Saturn", "SaturnModel", "Planet", "SaturnMat", "Assets/Planets/Saturn.mtl");
			Saturn->Translate(9537.0f, 0, 0);
			Saturn->setScale(11.6464f / planetSizeScaleDiv);
			Saturn->setMass(317.83f);
			Saturn->setVelocity(vec3(0, 0, earthVelocity * 0.323f));

			Uranus = mpResourceManager->addNewPhysicsObject("Uranus", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Uranus", "UranusModel", "Planet", "UranusMat", "Assets/Planets/Uranus.mtl");
			Uranus->Translate(191910.0f, 0, 0);
			Uranus->setScale(5.0724f / planetSizeScaleDiv);
			Uranus->setMass(317.83f);
			Uranus->setVelocity(vec3(0, 0, earthVelocity * 0.228f));

			Neptune = mpResourceManager->addNewPhysicsObject("Neptune", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Neptune", "NeptuneModel", "Planet", "NeptuneMat", "Assets/Planets/Neptune.mtl");
			Neptune->Translate(30069.0f, 0, 0);
			Neptune->setScale(4.9244f / planetSizeScaleDiv);
			Neptune->setMass(317.83f);
			Neptune->setVelocity(vec3(0, 0, earthVelocity * 0.182f));

			GravityGenerator* generator = new GravityGenerator();
			generator->setSourceObject(Sun);
			generator->addToRegistry(Mercury);
			generator->addToRegistry(Venus);
			generator->addToRegistry(Earth);
			generator->addToRegistry(Moon);
			generator->addToRegistry(Mars);
			generator->addToRegistry(Jupiter);
			generator->addToRegistry(Saturn);
			generator->addToRegistry(Uranus);
			generator->addToRegistry(Neptune);
		
			mpResourceManager->addForceGeneratorToMap("SunGravity", generator);

			GravityGenerator* MoonGenerator = new GravityGenerator();
			MoonGenerator->setSourceObject(Earth);

			mpResourceManager->addForceGeneratorToMap("EarthGravity", MoonGenerator);
			generator->addToRegistry(Moon);
			//Skybox* spacebox = new Skybox("Assets/Skybox/milkywayAttempt.jpg", mpResourceManager, 5000, "nebulaBox");
		}

#pragma endregion SpaceWorldSetup

		
	}

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
		mpDebug->addTextToScreen("Screenshot captured", vec2(width / 2.0f, 10), false, 1.0f);
	}
	if( key == GLUT_KEY_F1)
	{
		mpDebug->toggleDebugMode();
		if (mpDebug->getDebugMode())
			mpDebug->addTextToScreen("Debug Mode: ON", vec2(width / 2.0f, 10), false, .6f);
		else
			mpDebug->addTextToScreen("Debug Mode: OFF", vec2(width / 2.0f, 10), false, .6f);
	}
	if( key == GLUT_KEY_F2)
	{
		if(!wireframe)
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			wireframe = true;
			mpDebug->addTextToScreen("Drawing Mode: Wireframe", vec2(width / 2.0f, 10), false, .6f);
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			wireframe = false;
			mpDebug->addTextToScreen("Drawing Mode: Normal", vec2(width / 2.0f, 10), false, .6f);
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
		}
		else
		{
			glutPositionWindow((int)fullScreen_Saves.z, (int)fullScreen_Saves.w);
			glutReshapeWindow((int)fullScreen_Saves.x, (int)fullScreen_Saves.y);
			
			fullscreen = false;

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
O: output mouse pos

*/
void Game::hookKey(unsigned char key, int x, int y)
{
	if(key == 'w' || key == 'W')
	{
		mpCamera->command(vec3(1,0,0));
	}
	if(key == 's' || key == 'S')
	{
		mpCamera->command(vec3(-1,0,0));
	}
	if(key == 'e' || key == 'E')
	{
		mpCamera->command(vec3(0,1,0));
	}
	if(key == 'q' || key == 'Q')
	{
		mpCamera->command(vec3(0,-1,0));
	}
	if( key == 'a' || key == 'A')
	{
		mpCamera->command(vec3(0,0,-1));
	}
	if( key == 'd' || key == 'D')
	{
		mpCamera->command(vec3(0,0,1));
	}
	if(key == 27) //escape
	{
		endGame();
	}
	if(key == 'f' || key == 'F')
	{
		mpCamera->FlyMode(!mpCamera->FlyMode());
		if (mpCamera->FlyMode())
			mpDebug->addTextToScreen("Camera Movement: Smooth/Fly", vec2(width / 2.0f, 10), false, .6f);
		else
			mpDebug->addTextToScreen("Camera Movement: Rigid", vec2(width / 2.0f, 10), false, .6f);
	}
	if (key == 'r' || key == 'R')
	{
		mpCamera->hardReset(width, height);
	}
	if (key == 'p' || key == 'P')
	{
		Paused = !Paused;
	}
	if (key == 'l' || key == 'L')
	{
		mouseFree = !mouseFree;
		if ( mouseFree)
			mpDebug->addTextToScreen("Mouse: Free", vec2(10, height - 40), false, 0.6f);
		else
			mpDebug->addTextToScreen("Mouse: Locked", vec2(10, height - 40), false, 0.6f);
	}
	if (key == 'o' || key == 'O')
	{
		if (!mpDebug->getDebugMode())
		{
			vec3 temp = mpCamera->getPos();
			mpDebug->addTextToScreen("Cam Pos:", vec2(10, height - 70), false, 1.0f);
			mpDebug->addTextToScreen("X: " + std::to_string(temp.x), vec2(10, height - 90), false, 1.0f);
			mpDebug->addTextToScreen("Y: " + std::to_string(temp.y), vec2(10, height - 110), false, 1.0f);
			mpDebug->addTextToScreen("Z: " + std::to_string(temp.z), vec2(10, height - 130), false, 1.0f);
		}
	}
	if (m_space)
	{
	
		if (key == '1')
		{
			PlanetToFollow = Sun;
		}
		if (key == '2')
		{
			PlanetToFollow = Mercury;
		}
		if (key == '3')
		{
			PlanetToFollow = Venus;
		}
		if (key == '4')
		{
			PlanetToFollow = Earth;
		}
		if (key == '5')
		{
			PlanetToFollow = Mars;
		}
		if (key == '6')
		{
			PlanetToFollow = Jupiter;
		}
		if (key == '7')
		{
			PlanetToFollow = Saturn;
		}
		if (key == '8')
		{
			PlanetToFollow = Uranus;
		}
		if (key == '9')
		{
			PlanetToFollow = Neptune;
		}
		if (key == '0')
		{
			PlanetToFollow = nullptr;
		}
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

	delete mpResourceManager;
	delete mpCamera;
	delete mpTerrainManager;
	delete waterShaderManager;
	delete mpDebug;

	exit(0);
}







