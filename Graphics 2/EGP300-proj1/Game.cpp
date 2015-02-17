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

	if (m_waterworld)
	{
		mpResourceManager->drawObject(mView, proj, viewProj, ShaderManager, "water", "waterShader");
		mpResourceManager->drawObject(mView, proj, viewProj, ShaderManager, "fishy", "colorBombShader");
	}
	

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
	mpResourceManager->updateObjects(mpCamera->getPos(), (float)mpDebug->getDeltaTime());	

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
	
	if (help)
	{
		mpDebug->addTextToScreen("WSAD+EQ: Movement", vec2(10, height * .5f + 200));
		mpDebug->addTextToScreen("ESC: Quit", vec2(10, height * .5f + 180));
		mpDebug->addTextToScreen("R: Reset ", vec2(10, height * .5f + 160));
		mpDebug->addTextToScreen("F: Switch Movement Mode (Smooth vs Rigid)", vec2(10, height * .5f + 140));
		mpDebug->addTextToScreen("P: Toggle Pause", vec2(10, height * .5f + 120));
		mpDebug->addTextToScreen("L: Release / Lock Mouse", vec2(10, height * .5f + 100));
		mpDebug->addTextToScreen("F1: Toggle Debug Mode", vec2(10, height * .5f + 80));
		mpDebug->addTextToScreen("F2: Toggle Wireframe Mode", vec2(10, height * .5f + 60));
		mpDebug->addTextToScreen("F3: Capture Screenshot", vec2(10, height * .5f + 40));
		mpDebug->addTextToScreen("F11: Toggle Fullscreen", vec2(10, height * .5f + 20));
		mpDebug->addTextToScreen("H: Toggle Help Text", vec2(10, height * .5f));

		if (m_space)
		{
			mpDebug->addTextToScreen("1-9: Set Cam XY pos to Coordinated Planet And Show Related Data", vec2(10, height * .5f -20));
			mpDebug->addTextToScreen("0: Follow No Planet", vec2(10, height * .5f - 40));
			mpDebug->addTextToScreen("T/Y: Increase/Decrease Time Passed Per Frame", vec2(10, height * .5f - 60));
		}
	}

	if (m_waterworld)
		waterWorldFixedUpdate();
	if (m_space)
		spaceWorldFixedUpdate();
}

void Game::spaceWorldDebug()
{
	if (PlanetToFollow != nullptr)
	{
		float xPos = (float)(width - 200);
		float yStartPos = (float)(height - 30);
		float yAdding = 20.0f;

		mpDebug->addTextToScreen("Following: " + mpResourceManager->getObjectKey(PlanetToFollow), vec2(xPos, yStartPos));
		mpDebug->addTextToScreen("Pos: ", vec2(xPos, yStartPos - yAdding));
		mpDebug->addTextToScreen("  X: " + glm::to_string(PlanetToFollow->getPos().x), vec2(xPos, yStartPos - yAdding * 2));
		mpDebug->addTextToScreen("  Y: " + glm::to_string(PlanetToFollow->getPos().y), vec2(xPos, yStartPos - yAdding * 3));
		mpDebug->addTextToScreen("  Z: " + glm::to_string(PlanetToFollow->getPos().z), vec2(xPos, yStartPos - yAdding * 4));
		mpDebug->addTextToScreen("Vel: " + mpResourceManager->getObjectKey(PlanetToFollow), vec2(xPos, yStartPos - yAdding * 5));
		mpDebug->addTextToScreen("  X: " + glm::to_string(PlanetToFollow->getVelocity().x), vec2(xPos, yStartPos - yAdding * 6));
		mpDebug->addTextToScreen("  Y: " + glm::to_string(PlanetToFollow->getVelocity().y), vec2(xPos, yStartPos - yAdding * 7));
		mpDebug->addTextToScreen("  Z: " + glm::to_string(PlanetToFollow->getVelocity().z), vec2(xPos, yStartPos - yAdding * 8));
		mpDebug->addTextToScreen("Acc: " + mpResourceManager->getObjectKey(PlanetToFollow), vec2(xPos, yStartPos - yAdding * 9));
		mpDebug->addTextToScreen("  X: " + glm::to_string(PlanetToFollow->getAcceleration().x), vec2(xPos, yStartPos - yAdding * 10));
		mpDebug->addTextToScreen("  Y: " + glm::to_string(PlanetToFollow->getAcceleration().y), vec2(xPos, yStartPos - yAdding * 11));
		mpDebug->addTextToScreen("  Z: " + glm::to_string(PlanetToFollow->getAcceleration().z), vec2(xPos, yStartPos - yAdding * 12));
	}
	if (systemTimeAdjuster == 0)
	{
		mpDebug->addTextToScreen("Timescale: 1 frame = 1 day" , vec2(300, height - 30));
	}
	else if (systemTimeAdjuster == 1)
	{
		mpDebug->addTextToScreen("Timescale: 1 frame = 1 week", vec2(300, height - 30));
	}
	else if (systemTimeAdjuster == 2)
	{
		mpDebug->addTextToScreen("Timescale: 1 frame = 1 month", vec2(300, height - 30));
	}
	else if (systemTimeAdjuster == 3)
	{
		mpDebug->addTextToScreen("Timescale: 1 frame = 1 year", vec2(300, height - 30));
	}
}

void Game::waterWorldUpdate()
{
	mpResourceManager->getObject("fishy")->modifyRotation(.1f, .1f, .1f);
	PhyshyFriends->update(mpDebug->getDeltaTime());
	ShaderManager->update(mpDebug->getDeltaTime());

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
	help = false;
	systemTimeAdjuster = 0; //starts at day per frame

	mpDebug = new Debug();
	mpCamera = new Camera();
	mpDebug = new Debug();
	mpResourceManager = new ResourceManager();
	mpTerrainManager = new TerrainManager();
	ShaderManager = new Shader_Manager();
	
	
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

			ShaderManager->init("WaterVertShader.vp", "WaterFragShader.fp");
			ShaderManager->init("ColorVertShader.vp", "ColorFragShader");

			mpResourceManager->LoadFile("Assets/Fish/FISHY.obj");

			m_cloudSkybox = new Skybox("Assets/Skybox/cloudbox2.jpg", mpResourceManager, 3000, "cloudSkybox");
			m_cloudSkybox->setIsPrefab(true);
			m_underWaterSkybox = new Skybox("Assets/WaterBox/waterSkybox.png", mpResourceManager, 3000, "underwaterSkybox");

			mpTerrainManager->createTerrain(mpResourceManager, "Assets/Heightmap/heightmap_water.jpg", "Assets/Heightmap/heightmap_waterTexture.jpg", terrainSize, 200, terrainTriDensity, "repeatHM", true);
			mpTerrainManager->setHeightOfRepeatingTerrain("repeatHM", -200.0f);


			m_waterMap = new Heightmap(mpResourceManager, "Assets/Water/waterTexture.jpg", waterTerrainSize, waterTerrainSize, 200, "water");
			m_waterMap->setPos(vec3(-waterTerrainSize * .5f, 150, -waterTerrainSize * .5f));
			m_waterMap->setIsPrefab(true);

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
			fishy->setIsPrefab(true);

			physhy = mpResourceManager->addNewPhysicsObject("physhy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			physhy->modifyVelocity(vec3(0, 0, 1));

			PhyshyFriends = new ParticleEffect(mpResourceManager, "FriendsSpawn", vec3(5, 0, 0), 100, 300, vec3(1, 0, 0));
			PhyshyFriends->startEffect("Assets/Fish");

		}
#pragma endregion WaterWorldSetup

#pragma region SpaceWorldSetup

		//Model scale:  1 : 12,740,000
		if (m_space)
		{
			float earthVelocity = 29.8f;
			float planetSizeScaleDiv = 10.0f;
			// radius, mass, pos, vel
			mpResourceManager->LoadFile("Assets/Planets/EarthPretty.obj");
			Sun = mpResourceManager->addNewPhysicsObject("Sun", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Sun", "SunModel", "Planet", "SunMat", "Assets/Planets/Sun.mtl");
			Sun->Translate(0, 0, 0);
			Sun->setScale(.25f / planetSizeScaleDiv);
			Sun->setMass(1.0f);
			
			Mercury = mpResourceManager->addNewPhysicsObject("Mercury", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Mercury", "MercuryModel", "Planet", "MercuryMat", "Assets/Planets/Mercury.mtl");
			Mercury->Translate(0.31f, 0, 0);
			Mercury->setScale(0.05f / planetSizeScaleDiv);
			Mercury->setMass(0.0000001666f);
			Mercury->setVelocity(vec3(0.0f, 0.0f, 0.0000003925392803303289f));

			Venus = mpResourceManager->addNewPhysicsObject("Venus", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Venus", "VenusModel", "Planet", "VenusMat", "Assets/Planets/Venus.mtl");
			Venus->Translate(0.718f, 0, 0);
			Venus->setScale(.1f / planetSizeScaleDiv);
			Venus->setMass(0.000002447f);
			Venus->setVelocity(vec3(0, 0, 0.0000002377854700252f));

			Earth = mpResourceManager->addNewPhysicsObject("Earth", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth->Translate(0.98f, 0, 0);
			Earth->setScale(.025f / planetSizeScaleDiv);
			Earth->setMass(0.000003003f);
			Earth->setVelocity(vec3(0, 0, 0.000000202542989816901f));

			Moon = mpResourceManager->addNewPhysicsObject("Moon", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Moon", "MoonModel", "Planet", "MoonMat", "Assets/Planets/Moon.mtl");
			Moon->Translate(0.979f, 0, 0);
			Moon->setScale(0.005f / planetSizeScaleDiv);
			Moon->setMass(0.0000000000000123f);
			Moon->setVelocity(vec3(0, 0, (float)(0.0000002059429908 + 0.000000007197722768554639)));

			Mars = mpResourceManager->addNewPhysicsObject("Mars", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Mars", "MarsModel", "Planet", "MarsMat", "Assets/Planets/Mars.mtl");
			Mars->Translate(1.38f, 0, 0);
			Mars->setScale(0.08f / planetSizeScaleDiv);
			Mars->setMass(.0000003232f);
			Mars->setVelocity(vec3(0, 0, .0000001624394778196f));

			Jupiter = mpResourceManager->addNewPhysicsObject("Jupiter", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Jupiter", "JupiterModel", "Planet", "JupiterMat", "Assets/Planets/Jupiter.mtl");
			Jupiter->Translate(4.95f, 0, 0);
			Jupiter->setScale(1.1f / planetSizeScaleDiv);
			Jupiter->setMass(.0009547919f);
			Jupiter->setVelocity(vec3(0, 0, .0000000879036575732f));

			Saturn = mpResourceManager->addNewPhysicsObject("Saturn", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Saturn", "SaturnModel", "Planet", "SaturnMat", "Assets/Planets/Saturn.mtl");
			Saturn->Translate(9.02f, 0, 0);
			Saturn->setScale(0.941f / planetSizeScaleDiv);
			Saturn->setMass(.000285885f);
			Saturn->setVelocity(vec3(0, 0, .0000000654213857054f));

			Uranus = mpResourceManager->addNewPhysicsObject("Uranus", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Uranus", "UranusModel", "Planet", "UranusMat", "Assets/Planets/Uranus.mtl");
			Uranus->Translate(18.3f, 0, 0);
			Uranus->setScale(0.4f / planetSizeScaleDiv);
			Uranus->setMass(.000043662f);
			Uranus->setVelocity(vec3(0, 0, .0000000461798016744f));

			Neptune = mpResourceManager->addNewPhysicsObject("Neptune", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Neptune", "NeptuneModel", "Planet", "NeptuneMat", "Assets/Planets/Neptune.mtl");
			Neptune->Translate(30.0f, 0, 0);
			Neptune->setScale(0.388f / planetSizeScaleDiv);
			Neptune->setMass(.000051513f);
			Neptune->setVelocity(vec3(0, 0, .0000000368628241436f));

			mpResourceManager->giveAllPhysicsObjectsGravity();
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
			mpDebug->addMessageToScreen("Camera Movement: Smooth/Fly");
		else
			mpDebug->addMessageToScreen("Camera Movement: Rigid");
	}
	if (key == 'r' || key == 'R')
	{
		mpCamera->hardReset(width, height);
		mpDebug->addMessageToScreen("Reset");
		if (m_space)
			resetSpaceWorld();
	}
	if (key == 'p' || key == 'P')
	{
		Paused = !Paused;
	}
	if (key == 'l' || key == 'L')
	{
		mouseFree = !mouseFree;
		if ( mouseFree)
			mpDebug->addMessageToScreen("Mouse: Free");
		else
			mpDebug->addMessageToScreen("Mouse: Locked");
	}
	if (key == 'h' || key == 'H')
	{
		help = !help;
	}
	if (m_space)
	{
	
		if (key == '1')
		{
			if (PlanetToFollow != Sun)
				PlanetToFollow = Sun;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '2')
		{
			if (PlanetToFollow != Mercury)
				PlanetToFollow = Mercury;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '3')
		{
			if (PlanetToFollow != Venus)
				PlanetToFollow = Venus;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '4')
		{
			if (PlanetToFollow != Earth)
				PlanetToFollow = Earth;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '5')
		{
			if (PlanetToFollow != Mars)
				PlanetToFollow = Mars;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '6')
		{
			if (PlanetToFollow != Jupiter)
				PlanetToFollow = Jupiter;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '7')
		{
			if (PlanetToFollow != Saturn)
				PlanetToFollow = Saturn;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '8')
		{
			if (PlanetToFollow != Uranus)
				PlanetToFollow = Uranus;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '9')
		{
			if (PlanetToFollow != Neptune)
				PlanetToFollow = Neptune;
			else
				PlanetToFollow = nullptr;
		}
		if (key == '0')
		{
			PlanetToFollow = nullptr;
			mpCamera->setPos(vec3(-5, 11, 15));
			
			mpCamera->setYaw(1.1f);
			mpCamera->setPitch(-.55f);
		}

		if (key == 't' || key == 'T')
		{
			// day per frame
			//systemTime *= 86400.0; 
			// week per frame
			//systemTime *= 604800.0;
			// month per frame
			//systemTime *= 2628000.0;
			// year per frame
			//systemTime *= 31536000.0;
			if (systemTimeAdjuster == 1)
			{
				systemTimeAdjuster = 0;
				mpResourceManager->modifyPhysicsSystemTime(86400.0);
			}
			else if (systemTimeAdjuster == 2)
			{
				systemTimeAdjuster = 1;
				mpResourceManager->modifyPhysicsSystemTime(604800.0);
			}
			else if (systemTimeAdjuster == 3)
			{
				systemTimeAdjuster = 2;
				mpResourceManager->modifyPhysicsSystemTime(2628000.0);
			}
		}
		if (key == 'y' || key == 'Y')
		{
			if (systemTimeAdjuster == 0)
			{
				systemTimeAdjuster = 1;
				mpResourceManager->modifyPhysicsSystemTime(604800.0);
			}
			else if (systemTimeAdjuster == 1)
			{
				systemTimeAdjuster = 2;
				mpResourceManager->modifyPhysicsSystemTime(2628000.0);
			}
			else if (systemTimeAdjuster == 2)
			{
				systemTimeAdjuster = 3;
				mpResourceManager->modifyPhysicsSystemTime(31536000.0);
			}

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
	delete ShaderManager;
	delete mpDebug;

	exit(0);
}


void Game::resetSpaceWorld()
{
	// radius, mass, pos, vel
	Paused = true;

	Sun->setPos(vec3(0, 0, 0));
	Sun->setVelocity(vec3(0, 0, 0));
	Sun->setAcceleration(vec3(0, 0, 0));

	Mercury->setPos(vec3(0.31f, 0, 0));
	Mercury->setVelocity(vec3(0.0f, 0.0f, 0.0000003925392803303289f));
	Mercury->setAcceleration(vec3(0, 0, 0));

	Venus->setPos(vec3(0.718f, 0, 0));
	Venus->setVelocity(vec3(0, 0, 0.0000002377854700252f));
	Venus->setAcceleration(vec3(0, 0, 0));

	Earth->setPos(vec3(0.98f, 0, 0));
	Earth->setVelocity(vec3(0, 0, 0.000000202542989816901f));
	Earth->setAcceleration(vec3(0, 0, 0));

	Moon->setPos(vec3(0.979f, 0, 0));
	Moon->setVelocity(vec3(0, 0, (float)(0.0000002059429908 + 0.000000007197722768554639)));
	Moon->setAcceleration(vec3(0, 0, 0));

	Mars->setPos(vec3(1.38f, 0, 0));
	Mars->setVelocity(vec3(0, 0, .0000001624394778196f));
	Mars->setAcceleration(vec3(0, 0, 0));

	Jupiter->setPos(vec3(4.95f, 0, 0));
	Jupiter->setVelocity(vec3(0, 0, .0000000879036575732f));
	Jupiter->setAcceleration(vec3(0, 0, 0));

	Saturn->setPos(vec3(9.02f, 0, 0));
	Saturn->setVelocity(vec3(0, 0, .0000000654213857054f));
	Sun->setAcceleration(vec3(0, 0, 0));

	Uranus->setPos(vec3(18.3f, 0, 0));
	Uranus->setVelocity(vec3(0, 0, .0000000461798016744f));
	Uranus->setAcceleration(vec3(0, 0, 0));

	Neptune->setPos(vec3(30.0f, 0, 0));
	Neptune->setVelocity(vec3(0, 0, .0000000368628241436f));
	Neptune->setAcceleration(vec3(0, 0, 0));

}




