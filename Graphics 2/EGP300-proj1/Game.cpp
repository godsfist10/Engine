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
		mpDebug->addTextToScreen("R: Reset ", vec2(10, height * .5f + 160));
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
	mpDebug = new Debug();
	mpResourceManager = new ResourceManager();
	mpTerrainManager = new TerrainManager();
	mpShaderManager = new Shader_Manager();
	mpParticleForceManager = new ParticleForceRegistry();
	mpContactHandler = new ContactHandler();

	mpDebug->init(mpResourceManager, "Assets/TextMaterials/whiteTextMaterial.mtl");
	
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


#pragma region standardPrefabSetup


		//mpResourceManager->LoadFile("Assets/StandardObjects/Cube/cube.obj", "cubePrefab");
	    //cube = mpResourceManager->addNewPhysicsObject("cube", mpResourceManager->getObject("cubePrefab")->getModelMap());
		//mpResourceManager->LoadFile("Assets/StandardObjects/Sphere/Sphere.obj");
		//mpResourceManager->LoadFile("Assets/StandardObjects/Torus/Torus.obj");

		//Object* cube = mpResourceManager->addNewObject("cube", mpResourceManager->getObject("CubeTest")->getModelMap());
		//cube->setIsPrefab(true);
		//Object* sphere = mpResourceManager->addNewObject("sphere", mpResourceManager->getObject("Assets/Sphere/Sphere")->getModelMap());
		//Object* torus = mpResourceManager->addNewObject("torus", mpResourceManager->getObject("Assets/Torus/Torus")->getModelMap());

#pragma endregion standardPrefabSetup

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

			Object* fishy = mpResourceManager->addNewObject("fishy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			fishy->Translate(50, 5, 50);
			//mpResourceManager->applyShaderToObject("fishy", "diffuse");
			//fishy->setIsPrefab(true);

			physhy = mpResourceManager->addNewPhysicsObject("physhy", mpResourceManager->getObject("Assets/Fish")->getModelMap());
			physhy->init(Vector3D(50, 5, 40), Vector3D(0, 0, 3));

			mpResourceManager->LoadFile("Assets/StandardObjects/Cube/cube.obj", "CubePlease");
			Ground = mpResourceManager->addNewPhysicsObject("ground", mpResourceManager->getObject("CubePlease")->getModelMap());
			Ground->setPos(100, 0, 300);
			Ground->setMass(MAXFLOAT);
			Ground->setScale(50, .05f, 600);

			mpResourceManager->LoadFile("Assets/Planets/EarthPretty.obj");
			Earth = mpResourceManager->addNewPhysicsObject("Earth", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth->init(Vector3D(100.0f, 50.0f, 0));
			Earth->setScale(1);
			Earth->setMass(1.0f);

			Earth2 = mpResourceManager->addNewPhysicsObject("Earth2", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth2->init(Vector3D(100.0f, 50.0f, 50));
			Earth2->setScale(1);
			Earth2->setMass(1.0f);

			Earth3 = mpResourceManager->addNewPhysicsObject("Earth3", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth3->init(Vector3D(100.0f, 50.0f, 100));
			Earth3->setScale(1);
			Earth3->setMass(1.0f);

			Earth4 = mpResourceManager->addNewPhysicsObject("Earth4", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth4->init(Vector3D(100.0f, 50.0f, 150));
			Earth4->setScale(1);
			Earth4->setMass(1.0f);

			Earth5 = mpResourceManager->addNewPhysicsObject("Earth5", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth5->init(Vector3D(100.0f, 30.0f, 200));
			Earth5->setScale(1);
			Earth5->setMass(1.0f);

			Earth6 = mpResourceManager->addNewPhysicsObject("Earth6", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			Earth6->init(Vector3D(100.0f, 50.0f, 250));
			Earth6->setScale(1);
			Earth6->setMass(1.0f);

			PhysicsObject* Cube1 = mpResourceManager->addNewPhysicsObject("Cube1", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube2 = mpResourceManager->addNewPhysicsObject("Cube2", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube3 = mpResourceManager->addNewPhysicsObject("Cube3", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube4 = mpResourceManager->addNewPhysicsObject("Cube4", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube5 = mpResourceManager->addNewPhysicsObject("Cube5", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube6 = mpResourceManager->addNewPhysicsObject("Cube6", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube7 = mpResourceManager->addNewPhysicsObject("Cube7", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Cube8 = mpResourceManager->addNewPhysicsObject("Cube8", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			
			Cube1->setScale(.2f);
			Cube2->setScale(.2f);
			Cube3->setScale(.2f);
			Cube4->setScale(.2f);
			Cube5->setScale(.2f);
			Cube6->setScale(.2f);
			Cube7->setScale(.2f);
			Cube8->setScale(.2f);

			Cube1->init(Vector3D(100.0f, 50.0f, 300));
			Cube2->init(Vector3D(105.0f, 50.0f, 300));
			Cube3->init(Vector3D(105.0f, 50.0f, 305));
			Cube4->init(Vector3D(100.0f, 50.0f, 305));
			Cube5->init(Vector3D(100.0f, 55.0f, 300));
			Cube6->init(Vector3D(105.0f, 55.0f, 300));
			Cube7->init(Vector3D(105.0f, 55.0f, 305));
			Cube8->init(Vector3D(100.0f, 55.0f, 305));

			Cube1->setMass(1.0f);
			Cube2->setMass(1.0f);
			Cube3->setMass(1.0f);
			Cube4->setMass(1.0f);
			Cube5->setMass(1.0f);
			Cube6->setMass(1.0f);
			Cube7->setMass(1.0f);
			Cube8->setMass(1.0f);
			
			PhysicsObject* Tet1 = mpResourceManager->addNewPhysicsObject("Tet1", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Tet2 = mpResourceManager->addNewPhysicsObject("Tet2", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Tet3 = mpResourceManager->addNewPhysicsObject("Tet3", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			PhysicsObject* Tet4 = mpResourceManager->addNewPhysicsObject("Tet4", mpResourceManager->getObject("Assets/Planets")->getModelMap());

			Tet1->setScale(.2f);
			Tet2->setScale(.2f);
			Tet3->setScale(.2f);
			Tet4->setScale(.2f);

			Tet1->setMass(1.0f);
			Tet2->setMass(1.0f);
			Tet3->setMass(1.0f);
			Tet4->setMass(1.0f);

			Tet1->init(Vector3D(95.0f, 45.0f, 355));
			Tet2->init(Vector3D(95.0f, 55.0f, 345));
			Tet3->init(Vector3D(105.0f, 45.0f, 345));
			Tet4->init(Vector3D(105.0f, 55.0f, 355));

			//Add Gravity and ground contact/forces
			GravityForceGenerator* gravGen = new GravityForceGenerator(Vector3D(0, -9.8f, 0));
			mpContactHandler->AddRunTimeContactGenerator(new GroundContactGen(Ground->getPos().y));

			mpParticleForceManager->add(Earth, gravGen);
			mpParticleForceManager->add(Earth2, gravGen);
			mpParticleForceManager->add(Earth3, gravGen);
			mpParticleForceManager->add(Earth4, gravGen);
			mpParticleForceManager->add(Earth5, gravGen);
			mpParticleForceManager->add(Earth6, gravGen);
			mpParticleForceManager->add(Cube1, gravGen);
			mpParticleForceManager->add(Cube2, gravGen);
			mpParticleForceManager->add(Cube3, gravGen);
			mpParticleForceManager->add(Cube4, gravGen);
			mpParticleForceManager->add(Cube5, gravGen);
			mpParticleForceManager->add(Cube6, gravGen);
			mpParticleForceManager->add(Cube7, gravGen);
			mpParticleForceManager->add(Cube8, gravGen);
			mpParticleForceManager->add(Tet1, gravGen);
			mpParticleForceManager->add(Tet2, gravGen);
			mpParticleForceManager->add(Tet3, gravGen);
			mpParticleForceManager->add(Tet4, gravGen);

			mpParticleForceManager->addForceGeneratorToList(gravGen);

			DragForceGenerator* waterDragGen = new DragForceGenerator(.1f);
			mpParticleForceManager->add(Earth, waterDragGen);
			mpParticleForceManager->add(Earth2, waterDragGen);
			mpParticleForceManager->addForceGeneratorToList(waterDragGen);
			
			BuoyancyForceGen* bouyGen = new BuoyancyForceGen(5.0f, .012f, 150.0f);
			mpParticleForceManager->add(Earth, bouyGen);
			mpParticleForceManager->addForceGeneratorToList(bouyGen);

			SpringForceGen* springGen = new SpringForceGen(Earth4, .3f, 30.0);
			mpParticleForceManager->add(Earth3, springGen);

			SpringForceGen* springGen2 = new SpringForceGen(Earth3, .3f, 30.0);
			mpParticleForceManager->add(Earth4, springGen2);

			
			mpParticleForceManager->add(Cube1, new SpringForceGen(Tet2, .3f, 30.0));
			mpParticleForceManager->add(Tet2, new SpringForceGen(Cube1, .3f, 30.0));
		
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Earth5, Earth6, 50));

			//Cube rods
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube2, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube3, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube4, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube1, 5));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube5, Cube6, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube6, Cube7, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube7, Cube8, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube8, Cube5, 5));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube5, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube6, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube7, 5));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube8, 5));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube6, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube8, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube7, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube5, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube8, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube6, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube5, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube7, (float)std::sqrt(50)));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube3, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube4, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube5, Cube7, (float)std::sqrt(50)));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube6, Cube8, (float)std::sqrt(50)));

			//Tet rods
			float length = (2.0f * (float)std::sqrt(2.0))*5.0f;
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet2, length));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet3, length));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet4, length));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet2, Tet3, length));
			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet2, Tet4, length));

			mpContactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet3, Tet4, length));
			

			mpContactHandler->AddCollisionObject(Earth2);
			mpContactHandler->AddCollisionObject(Earth3);
			mpContactHandler->AddCollisionObject(Earth4);
			mpContactHandler->AddCollisionObject(Earth5);
			mpContactHandler->AddCollisionObject(Earth6);
			mpContactHandler->AddCollisionObject(Cube1);
			mpContactHandler->AddCollisionObject(Cube2);
			mpContactHandler->AddCollisionObject(Cube3);
			mpContactHandler->AddCollisionObject(Cube4);
			mpContactHandler->AddCollisionObject(Cube5);
			mpContactHandler->AddCollisionObject(Cube6);
			mpContactHandler->AddCollisionObject(Cube7);
			mpContactHandler->AddCollisionObject(Cube8);
			mpContactHandler->AddCollisionObject(Tet1);
			mpContactHandler->AddCollisionObject(Tet2);
			mpContactHandler->AddCollisionObject(Tet3);
			mpContactHandler->AddCollisionObject(Tet4);
		
#pragma endregion WaterWorldSetup

#pragma region SpaceWorldSetup 
		/* //keeping this for reference if I need it
			//Model scale:  1 : 12,740,000
		
			float earthVelocity = 29.8f;
			float planetSizeScaleDiv = 10.0f;
			// radius, mass, pos, vel
			mpResourceManager->LoadFile("Assets/Planets/EarthPretty.obj");
			Sun = mpResourceManager->addNewPhysicsObject("Sun", mpResourceManager->getObject("Assets/Planets")->getModelMap());
			mpResourceManager->applyMaterialToObject("Sun", "SunModel", "Planet", "SunMat", "Assets/Planets/Sun.mtl");
			Sun->Translate(0, 0, 0);
			Sun->setScale(.25f / planetSizeScaleDiv);
			Sun->setMass(1.0f);
			
			//Skybox* spacebox = new Skybox("Assets/Skybox/milkywayAttempt.jpg", mpResourceManager, 5000, "nebulaBox");
		
		*/

#pragma endregion SpaceWorldSetup	
	


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
	if (key == 'w' || key == 'W')
	{
		mpCamera->command(vec3(1, 0, 0));
	}
	if (key == 's' || key == 'S')
	{
		mpCamera->command(vec3(-1, 0, 0));
	}
	if (key == 'e' || key == 'E')
	{
		mpCamera->command(vec3(0, 1, 0));
	}
	if (key == 'q' || key == 'Q')
	{
		mpCamera->command(vec3(0, -1, 0));
	}
	if (key == 'a' || key == 'A')
	{
		mpCamera->command(vec3(0, 0, -1));
	}
	if (key == 'd' || key == 'D')
	{
		mpCamera->command(vec3(0, 0, 1));
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
	if (key == 'r' || key == 'R')
	{
		//mpCamera->hardReset(width, height);
		mpDebug->addMessageToScreen("Reset World");
		ResetWorld();
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

	exit(0);
}


void Game::ResetWorld()
{
	// radius, mass, pos, vel
	Paused = true;

	//mpResourceManager->resetAllObjects();

	Earth->setPos(vec3(100.0f, 50.0f, 0));
	Earth->setVelocity(vec3(0, 0, 0));
	Earth->setAcceleration(vec3(0, 0, 0));
	Earth2->resetObject();
	Earth3->resetObject();
	Earth4->resetObject();
	Earth5->resetObject();
	Earth6->resetObject();

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

	mpResourceManager->getObject("fishy")->setRotation(glm::vec3(0, 0, 0));
}




