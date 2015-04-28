#include "WorldHandler.h"


WorldHandler::WorldHandler()
{
}


WorldHandler::~WorldHandler()
{
}



void WorldHandler::loadTextFile(string filename, ResourceManager* resourceManager, ContactHandler* contactHandler, ParticleForceRegistry* particleForceManager)
{
	ifstream file(filename);

	if (!file)
	{
		Debug::log("File: " + filename + " not loaded because it does not exist\n");
		return; // Shit broke

	}

	string line;
	while (!file.eof())
	{
		getline(file, line);

		if (line.length() == 0 || line[0] == '#')
			continue;

		size_t spacePos = line.find(' ');
		if (spacePos == string::npos)
			continue;

		string cmd;
		cmd.assign(line, 0, spacePos);

		string data;
		data.assign(line, spacePos + 1, line.size() - spacePos);

		stringstream ss(data);


		if (cmd == "PhysicsObject")
		{
			string objName;
			string modelMap;
			Vector3D pos = Vector3D::ZERO;
			Vector3D scale = Vector3D::ZERO;
			float mass;
			int collisionObject;

			ss >> objName >> modelMap;
			ss >> pos.X >> pos.Y >> pos.Z;
			ss >> scale.X >> scale.Y >> scale.Z;
			ss >> mass;
			ss >> collisionObject;

			/*cout << "Pos: " << pos.X << " " << pos.Y << "  " << pos.Z << endl;
			cout << "Scale: " << scale.X << " " << scale.Y << "  " << scale.Z << endl;
			cout << "Mass:  " << mass << endl;
			cout << "collisionObject:  " << collisionObject << endl;*/

			PhysicsObject* TempObj = resourceManager->addNewPhysicsObject(objName, resourceManager->getObject(modelMap)->getModelMap());
			TempObj->init(pos);
			TempObj->setScale(scale.toVec3());
			if (mass != -1)
				TempObj->setMass(mass);
			else
				TempObj->setMass(MAX_FLOAT);
			particleForceManager->add(TempObj, mGravGen);

			if (collisionObject == 1)
				contactHandler->AddCollisionObject(TempObj);

		}
		if (cmd == "LoadFile")
		{
			string fileName;
			string modelName;

			ss >> fileName >> modelName;

			resourceManager->LoadFile(fileName, modelName);

		}
		if (cmd == "WaterDragGen")
		{
			string toApplyTo;
			float dragAmount;
			
			ss >> toApplyTo;
			ss >> dragAmount;
			
			if (!resourceManager->hasPhysicsObject(toApplyTo))
			{
				Debug::log(toApplyTo + ": invalid object name");
				break;
			}

			DragForceGenerator* waterDragGen = new DragForceGenerator(dragAmount);
			particleForceManager->add(resourceManager->getPhysicsObject(toApplyTo), waterDragGen);
			particleForceManager->addForceGeneratorToList(waterDragGen);

		}
		if (cmd == "BouyGen")
		{
			string toApplyTo;
			float maxDepth;
			float volume;
			float waterHeight;
			
			ss >> toApplyTo;
			ss >> maxDepth >> volume >> waterHeight;
			
			if (!resourceManager->hasPhysicsObject(toApplyTo))
			{
				Debug::log(toApplyTo + ": invalid object name");
				break;
			}

			BuoyancyForceGen* bouyGen = new BuoyancyForceGen(maxDepth, volume, waterHeight);
			particleForceManager->add(resourceManager->getPhysicsObject(toApplyTo), bouyGen);
			particleForceManager->addForceGeneratorToList(bouyGen);

		}
		if (cmd == "SpringGen")
		{
			
			string anchor;
			string toApplyTo;
			float springConst;
			float restLength;
			
			ss >> anchor >> toApplyTo;
			ss >> springConst >> restLength;

			if (!resourceManager->hasPhysicsObject(toApplyTo))
			{
				Debug::log(toApplyTo + ": invalid object name");
				break;
			}

			SpringForceGen* springGen = new SpringForceGen(resourceManager->getPhysicsObject(anchor), springConst, restLength);
			particleForceManager->add(resourceManager->getPhysicsObject(toApplyTo), springGen);

		}
		if (cmd == "SpringConnector")
		{
			string toApplyTo1;
			string toApplyTo2;
			float springConst;
			float restLength;

			ss >> toApplyTo1 >> toApplyTo2;
			ss >> springConst >> restLength;

			if (!resourceManager->hasPhysicsObject(toApplyTo1))
			{
				Debug::log(toApplyTo1 + ": invalid object name");
				break;
			}
			if (!resourceManager->hasPhysicsObject(toApplyTo2))
			{
				Debug::log(toApplyTo2 + ": invalid object name");
				break;
			}

			SpringForceGen* springGen = new SpringForceGen(resourceManager->getPhysicsObject(toApplyTo1), springConst, restLength);
			particleForceManager->add(resourceManager->getPhysicsObject(toApplyTo2), springGen);

			SpringForceGen* springGenReverse = new SpringForceGen(resourceManager->getPhysicsObject(toApplyTo2), springConst, restLength);
			particleForceManager->add(resourceManager->getPhysicsObject(toApplyTo1), springGenReverse);

		}
		if (cmd == "RodConnector")
		{
			string applyTo1;
			string applyTo2;
			float restingDist;

			ss >> applyTo1 >> applyTo2;
			ss >> restingDist;

			if (!resourceManager->hasPhysicsObject(applyTo1))
			{
				Debug::log(applyTo1 + ": invalid object name");
				break;
			}
			if (!resourceManager->hasPhysicsObject(applyTo2))
			{
				Debug::log(applyTo2 + ": invalid object name");
				break;
			}

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(resourceManager->getPhysicsObject(applyTo1), resourceManager->getPhysicsObject(applyTo2), restingDist));

		}
		if (cmd == "Ground")
		{
			string objName;
			string modelMap;
			Vector3D pos = Vector3D::ZERO;
			Vector3D scale = Vector3D::ZERO;


			ss >> objName >> modelMap;
			ss >> pos.X >> pos.Y >> pos.Z;
			ss >> scale.X >> scale.Y >> scale.Z;

			PhysicsObject* TempObj = resourceManager->addNewPhysicsObject(objName, resourceManager->getObject(modelMap)->getModelMap());
			TempObj->init(pos);
			TempObj->setScale(scale);
			TempObj->setMass(MAX_FLOAT);

			contactHandler->AddCollisionObject(TempObj);

			contactHandler->AddRunTimeContactGenerator(new GroundContactGen(TempObj->getPos().y));
		}
		if (cmd == "Gravity")
		{
			Vector3D grav = Vector3D::ZERO;
			ss >> grav.X >> grav.Y >> grav.Z;
			GravityForceGenerator* gravGen = new GravityForceGenerator(Vector3D(0, -9.8f, 0));
			particleForceManager->addForceGeneratorToList(gravGen);
			mGravGen = gravGen;
		}
		if (cmd == "ParticleTet")
		{

			string modelMap;
			string baseName;
			float scale;
			float mass;
			Vector3D pos;
			float distBetweenCorners;

			ss >> baseName >> modelMap;
			ss >> pos.X >> pos.Y >> pos.Z >> scale >> mass >> distBetweenCorners;


			PhysicsObject* Tet1 = resourceManager->addNewPhysicsObject(baseName + "1", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Tet2 = resourceManager->addNewPhysicsObject(baseName + "2", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Tet3 = resourceManager->addNewPhysicsObject(baseName + "3", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Tet4 = resourceManager->addNewPhysicsObject(baseName + "4", resourceManager->getObject(modelMap)->getModelMap());

			Tet1->setScale(scale);
			Tet2->setScale(scale);
			Tet3->setScale(scale);
			Tet4->setScale(scale);

			Tet1->setMass(mass);
			Tet2->setMass(mass);
			Tet3->setMass(mass);
			Tet4->setMass(mass);

			Tet1->init(Vector3D(pos.X - distBetweenCorners, pos.Y - distBetweenCorners, pos.Z + distBetweenCorners));
			Tet2->init(Vector3D(pos.X - distBetweenCorners, pos.Y + distBetweenCorners, pos.Z - distBetweenCorners));
			Tet3->init(Vector3D(pos.X + distBetweenCorners, pos.Y - distBetweenCorners, pos.Z - distBetweenCorners));
			Tet4->init(Vector3D(pos.X + distBetweenCorners, pos.Y + distBetweenCorners, pos.Z + distBetweenCorners));
			
			float length = (2.0f * (float)std::sqrt(2.0))*distBetweenCorners;

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet2, length));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet3, length));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet1, Tet4, length));

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet2, Tet3, length));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet2, Tet4, length));

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Tet3, Tet4, length));

			contactHandler->AddCollisionObject(Tet1);
			contactHandler->AddCollisionObject(Tet2);
			contactHandler->AddCollisionObject(Tet3);
			contactHandler->AddCollisionObject(Tet4);

			particleForceManager->add(Tet1, mGravGen);
			particleForceManager->add(Tet2, mGravGen);
			particleForceManager->add(Tet3, mGravGen);
			particleForceManager->add(Tet4, mGravGen);

		}
		if (cmd == "ParticleCube")
		{
			string modelMap;
			string baseName;
			float scale;
			float mass;
			Vector3D pos;
			float distBetweenCorners;

			ss >> baseName >> modelMap;
			ss >> pos.X >> pos.Y >> pos.Z >> scale >> mass >> distBetweenCorners;

			PhysicsObject* Cube1 = resourceManager->addNewPhysicsObject(baseName + "1", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube2 = resourceManager->addNewPhysicsObject(baseName + "2", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube3 = resourceManager->addNewPhysicsObject(baseName + "3", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube4 = resourceManager->addNewPhysicsObject(baseName + "4", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube5 = resourceManager->addNewPhysicsObject(baseName + "5", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube6 = resourceManager->addNewPhysicsObject(baseName + "6", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube7 = resourceManager->addNewPhysicsObject(baseName + "7", resourceManager->getObject(modelMap)->getModelMap());
			PhysicsObject* Cube8 = resourceManager->addNewPhysicsObject(baseName + "8", resourceManager->getObject(modelMap)->getModelMap());

			Cube1->setScale(scale);
			Cube2->setScale(scale);
			Cube3->setScale(scale);
			Cube4->setScale(scale);
			Cube5->setScale(scale);
			Cube6->setScale(scale);
			Cube7->setScale(scale);
			Cube8->setScale(scale);

			Cube1->init(Vector3D(pos.X, pos.Y, pos.Z));
			Cube2->init(Vector3D(pos.X + distBetweenCorners, pos.Y, pos.Z));
			Cube3->init(Vector3D(pos.X + distBetweenCorners, pos.Y, pos.Z + distBetweenCorners));
			Cube4->init(Vector3D(pos.X, pos.Y, pos.Z + distBetweenCorners));
			Cube5->init(Vector3D(pos.X, pos.Y + distBetweenCorners, pos.Z));
			Cube6->init(Vector3D(pos.X + distBetweenCorners, pos.Y + distBetweenCorners, pos.Z));
			Cube7->init(Vector3D(pos.X + distBetweenCorners, pos.Y + distBetweenCorners, pos.Z + distBetweenCorners));
			Cube8->init(Vector3D(pos.X, pos.Y + distBetweenCorners, pos.Z + distBetweenCorners));

			Cube1->setMass(mass);
			Cube2->setMass(mass);
			Cube3->setMass(mass);
			Cube4->setMass(mass);
			Cube5->setMass(mass);
			Cube6->setMass(mass);
			Cube7->setMass(mass);
			Cube8->setMass(mass);


			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube2, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube4, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube3, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube1, distBetweenCorners));

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube5, Cube6, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube6, Cube7, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube7, Cube8, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube8, Cube5, distBetweenCorners));

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube5, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube6, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube7, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube8, distBetweenCorners));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube6, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube8, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube7, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube5, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube8, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube3, Cube6, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube5, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube4, Cube7, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));

			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube1, Cube3, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube2, Cube4, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube5, Cube7, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			contactHandler->AddRunTimeContactGenerator(new RodContactGen(Cube6, Cube8, (float)std::sqrt(distBetweenCorners*distBetweenCorners + distBetweenCorners*distBetweenCorners)));
			
			contactHandler->AddCollisionObject(Cube1);
			contactHandler->AddCollisionObject(Cube2);
			contactHandler->AddCollisionObject(Cube3);
			contactHandler->AddCollisionObject(Cube4);
			contactHandler->AddCollisionObject(Cube5);
			contactHandler->AddCollisionObject(Cube6);
			contactHandler->AddCollisionObject(Cube7);
			contactHandler->AddCollisionObject(Cube8);

			particleForceManager->add(Cube1, mGravGen);
			particleForceManager->add(Cube2, mGravGen);
			particleForceManager->add(Cube3, mGravGen);
			particleForceManager->add(Cube4, mGravGen);
			particleForceManager->add(Cube5, mGravGen);
			particleForceManager->add(Cube6, mGravGen);
			particleForceManager->add(Cube7, mGravGen);
			particleForceManager->add(Cube8, mGravGen);
		}

	}
}