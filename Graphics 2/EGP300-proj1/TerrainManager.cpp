#include "TerrainManager.h"


TerrainManager::TerrainManager(void)
{
}


TerrainManager::~TerrainManager(void)
{
}

void TerrainManager::cleanup()
{
	m_ManagedTerrain.clear();
}


void TerrainManager::createTerrain(ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidth, const int& mapHeight, const int& triDensity, const string& objectName, bool repeating)
{
	if( !repeating)
	{
		Heightmap* tempHeightmap = new Heightmap(resourceManager, heightMapFilename, textureFilename, mapWidth, mapWidth, mapHeight, triDensity, objectName);
		addTerrainToMap(objectName, tempHeightmap);
	}
	else
	{
		//todo
		Heightmap* tempHeightmap = new Heightmap(resourceManager, heightMapFilename, textureFilename, mapWidth, mapWidth, mapHeight, triDensity, objectName);
		addTerrainToMap(objectName, tempHeightmap);
		makeRepeatable(resourceManager, tempHeightmap, objectName);
	}
}

void TerrainManager::createHeightlessTerrain(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidth, const int& triDensity, const string& objectName, bool repeating)
{
	if( !repeating)
	{
		Heightmap* tempHeightmap = new Heightmap(resourceManager, textureFilename, mapWidth, mapWidth, triDensity, objectName);
		tempHeightmap->setIsPrefab(true);
		addTerrainToMap(objectName, tempHeightmap);
	}
	else
	{
		Heightmap* tempHeightmap = new Heightmap(resourceManager, textureFilename, mapWidth, mapWidth, triDensity, objectName);
		addTerrainToMap(objectName, tempHeightmap);
		makeRepeatable(resourceManager, tempHeightmap, objectName);
	}
}

void TerrainManager::makeRepeatable(ResourceManager* resourceManager, Heightmap* heightmap, const string& objName)
{
	for(int i = 0; i < 8; i++)
	{
		stringstream ss;
		ss << i;
		string tempObjName = objName + ss.str();
		Heightmap* tempHeightmap = new Heightmap(resourceManager, tempObjName, heightmap);
		addTerrainToMap(tempObjName, tempHeightmap);
	}

	float terrainSize = heightmap->getTerrainSize();
	float terrainOffsets = terrainSize * .5f;
	float actualOffsets = heightmap->getOffset().x;

	jumpDistance = (terrainSize - actualOffsets);
	xBorders = vec2( - terrainOffsets, terrainOffsets );
	zBorders = vec2( - terrainOffsets, terrainOffsets );

	heightmap->setPos(vec3(-terrainSize - terrainOffsets + actualOffsets,0, -terrainSize - terrainOffsets + actualOffsets));

	Heightmap* tempH = getHeightmap(objName + "0");
	tempH->setPos(vec3( -terrainOffsets ,0 , -terrainSize - terrainOffsets + actualOffsets));

	tempH = getHeightmap(objName + "1");
	tempH->setPos(vec3(terrainSize - terrainOffsets - actualOffsets, 0 , -terrainSize - terrainOffsets + actualOffsets));

	tempH = getHeightmap(objName + "2");
	tempH->setPos(vec3(-terrainSize - terrainOffsets + actualOffsets , 0 , -terrainOffsets));

	tempH = getHeightmap(objName + "3");
	tempH->setPos(vec3(-terrainOffsets , 0 , -terrainOffsets));

	tempH = getHeightmap(objName + "4");
	tempH->setPos(vec3(terrainSize - terrainOffsets - actualOffsets , 0 , -terrainOffsets));

	tempH = getHeightmap(objName + "5");
	tempH->setPos(vec3(-terrainSize - terrainOffsets + actualOffsets, 0 , terrainSize - terrainOffsets - actualOffsets));

	tempH = getHeightmap(objName + "6");
	tempH->setPos(vec3(-terrainOffsets , 0 , terrainSize - terrainOffsets - actualOffsets));

	tempH = getHeightmap(objName + "7");
	tempH->setPos(vec3(terrainSize - terrainOffsets - actualOffsets , 0 , terrainSize - terrainOffsets - actualOffsets));

	//cout << "nailed it";
}



void TerrainManager::update(Camera* camera)
{
	bool checkem = false;
	vec3 cameraPos = camera->getPos();

	if( cameraPos.x < xBorders.x)
	{
		checkem = true;
		xBorders.x -= jumpDistance;
		xBorders.y -= jumpDistance;
	}
	if( cameraPos.x > xBorders.y )
	{
		checkem = true;
		xBorders.x += jumpDistance;
		xBorders.y += jumpDistance;
	}

	if( cameraPos.z < zBorders.x)
	{
		checkem = true;
		zBorders.x -= jumpDistance;
		zBorders.y -= jumpDistance;
	}
	if( cameraPos.z > zBorders.y )
	{
		checkem = true;
		zBorders.x += jumpDistance;
		zBorders.y += jumpDistance;
	}


	if( checkem)
	{
		for( auto it = m_ManagedTerrain.itBegin(); it != m_ManagedTerrain.itEnd(); ++it)
		{
			Heightmap* pMap = it->second;
			checkMap(cameraPos, pMap);
		}
	}
		
}

void TerrainManager::checkMap(vec3 cameraPos, Heightmap* heightmap)
{
	vec3 pos = heightmap->getPos();
	vec3 checkpos = pos + jumpDistance *.5f;
	if( abs(checkpos.x - xBorders.x) > jumpDistance )
	{
		if( abs(checkpos.x - xBorders.y) > jumpDistance)
		{
			//this terrain is to far x wise
			if( checkpos.x < xBorders.x)
			{
				//terrain needs ++x
				heightmap->setPos(vec3(pos.x + (jumpDistance * 3.0f), pos.y, pos.z));
			}
			else
			{
				//terrain needs --x
				heightmap->setPos(vec3(pos.x - (jumpDistance * 3.0f), pos.y, pos.z));
			}

		}
	}

	if( abs(checkpos.z - zBorders.x) > jumpDistance )
	{
		if( abs(checkpos.z - zBorders.y) > jumpDistance )
		{
			//this terrain to far z wise
			if( checkpos.z < zBorders.x)
			{
				//terrain needs ++ z
				heightmap->setPos(vec3(pos.x , pos.y, pos.z + (jumpDistance * 3.0f)));
			}
			else
			{
				//terrain needs --z
				heightmap->setPos(vec3(pos.x, pos.y, pos.z - (jumpDistance * 3.0f)));
			}


		}
	}

}


void TerrainManager::setHeightOfRepeatingTerrain(const string& objectName, float height)
{
	if( hasHeightmap(objectName))
	{
		Heightmap* origMap = getHeightmap(objectName);
		vec3 pos1 = origMap->getPos();
		pos1.y = height;
		origMap->setPos(pos1);

		for(int i = 0; i < 8; i++ )
		{
			std::stringstream ss;
			ss << i;
			string mapName = objectName + ss.str();
			Heightmap* tempMap = getHeightmap(mapName);
			vec3 pos = tempMap->getPos();
			pos.y = height;
			tempMap->setPos(pos);
		}
	}
	else
	{
		cout << "Error setting height of repeatable heightmap " + objectName;
	}
}



