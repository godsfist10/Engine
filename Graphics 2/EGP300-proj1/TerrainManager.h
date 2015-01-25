#pragma once

#include <Arc/ArcCore.h>

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <string>
#include <sstream>

#include "Camera.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "Heightmap.h"

#include <Arc/Map.h>

using namespace Arc;
using namespace std;

class TerrainManager
{
public:
	TerrainManager(void);
	~TerrainManager(void);
	void cleanup();

	void createTerrain(ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidth, const int& mapHeight, const int& triDensity, const string& objectName, bool repeating);
	void createHeightlessTerrain(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidth, const int& triDensity, const string& objectName, bool repeating);

	void update(Camera* camera);
	void setHeightOfRepeatingTerrain(const string& objectName, float height);


	inline bool hasHeightmap( const string& key ) { return m_ManagedTerrain.containsKey(key); }
	inline Heightmap* getHeightmap( const string& key ) { return (hasHeightmap(key) ? m_ManagedTerrain[key] : nullptr); }


private:

	void makeRepeatable(ResourceManager* resourceManager, Heightmap* heightmap, const string& objName);
	void checkMap(vec3 cameraPos, Heightmap* heightmap);
    inline void addTerrainToMap(const string& key, Heightmap* hmap) { m_ManagedTerrain.add(key,hmap);}

	Map<string, Heightmap*> m_ManagedTerrain;
	
	vec2 xBorders, zBorders;
	float jumpDistance;

};

