#pragma once
#include "ResourceManager.h"
using namespace glm;

class Heightmap : public Object
{
public:
	Heightmap(void);
	Heightmap( ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& mapHeight, const int& triDensity, const string& objectName);
	Heightmap(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& triDensity, const string& objectName);
	Heightmap(ResourceManager* resourceManager, const string& objectName, Heightmap* heightMap);

	inline Heightmap( const Heightmap& rhs );

	virtual ~Heightmap(void);
	void loadHeightMap(ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& mapHeight, const int& triDensity);
	void loadHeightlessMap(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& triDensity);
	
	glm::vec2 getOffset() const { return mOffset; }
	float getTerrainSize() const { return terrainSize; }
	string getHeightMapFileName() const { return mHeightMapFileName; }
	string getTextureFilename() const { return mTextureFilename; }

private:

	ArrayList<ArrayList<float>> m_Heights;
	vec2 mOffset;
	string mHeightMapFileName, mTextureFilename;
	float terrainSize;
	
	

};

