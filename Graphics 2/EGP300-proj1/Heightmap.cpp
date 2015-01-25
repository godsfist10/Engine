#include "Heightmap.h"


Heightmap::Heightmap(void)
{
}

Heightmap::Heightmap(ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& mapHeight, const int& triDensity, const string& objectName)
{
	loadHeightMap(resourceManager, heightMapFilename, textureFilename, mapWidthX, mapWidthY, mapHeight, triDensity);
	resourceManager->addObjectToMap(objectName, this);
	mHeightMapFileName = heightMapFilename;
	mTextureFilename = textureFilename;
}

Heightmap::Heightmap(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& triDensity, const string& objectName)
{
	loadHeightlessMap(resourceManager, textureFilename, mapWidthX, mapWidthY, triDensity);
	resourceManager->addObjectToMap(objectName, this);
	mTextureFilename = textureFilename;
}

Heightmap::Heightmap(const Heightmap& rhs) : Object(rhs)
{
	mOffset = rhs.mOffset;
	mHeightMapFileName = rhs.mHeightMapFileName;
	mTextureFilename = rhs.mTextureFilename;
	terrainSize = rhs.terrainSize;
}

Heightmap::Heightmap(ResourceManager* resourceManager, const string& objectName, Heightmap* heightMap)
{
	string heightMapFileName = Arc_Dirname(heightMap->getHeightMapFileName());
	mOffset = heightMap->getOffset();
	addModel(heightMapFileName, resourceManager->getModel(Arc_Dirname(heightMap->getHeightMapFileName())));
	resourceManager->addObjectToMap(objectName, this);
}

Heightmap::~Heightmap(void)
{
	cleanup();
}

void Heightmap::loadHeightMap(ResourceManager* resourceManager, const string& heightMapFilename, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& mapHeight, const int& triDensity)
{
	cleanup();

	terrainSize = (float)mapWidthX;

	SDL_Surface* pSurface = IMG_Load(heightMapFilename.c_str());

	if( !pSurface)
	{
		cout << "Heightmap image failed to load\n";
		return;
	}
	if(pSurface->w < triDensity)
	{
		cout << "Heightmap image width is smaller than requested density\n";
	}
	if(pSurface->h < triDensity)
	{
		cout << "Heightmap image height is smaller than requested density\n";
	}

	int widthX  = glm::clamp(pSurface->w, 0, triDensity);
	int widthY = glm::clamp(pSurface->h, 0, triDensity);

	GLfloat colStep = (widthX < pSurface->w ? ((GLfloat)pSurface->w / (GLfloat)triDensity) : 1.0f);
	GLfloat rowStep = (widthY < pSurface->h ? ((GLfloat)pSurface->h / (GLfloat)triDensity) : 1.0f);

	unsigned int mapVerts = widthX * widthY;

	ArrayList<vec3> rawVerts;
	rawVerts.resize(mapVerts);

	ArrayList<vec2> rawTexCoords;
	rawTexCoords.resize(mapVerts);

	GLfloat stepX = mapWidthX / (float)widthX;
	GLfloat stepY = mapWidthY / (float)widthY;

	mOffset.x = stepX;
	mOffset.y = stepY;

	m_Heights.clear();
	m_Heights.resize(triDensity);
	
	float mapHeightHalf = mapHeight * .5f;
	int index = 0;

	for (int row = 0; row < widthY; ++row)
	{
		m_Heights.add(ArrayList<float>());
		m_Heights[row].resize(triDensity);

		for (int col = 0; col < widthX; ++col)
		{
			int realRow = (int)((GLfloat)row * rowStep);
			int realCol = (int)((GLfloat)col * colStep);

			Uint8* pPixel = (Uint8*) pSurface->pixels + (realRow * pSurface->pitch) + (realCol * pSurface->format->BytesPerPixel);

			float value = ((GLfloat)pPixel[0] / 255.0f);
			float heightVal = glm::lerp(-mapHeightHalf, mapHeightHalf, value);

			m_Heights[row][col] = heightVal;

			rawVerts[index].x = (col * stepX);
			rawVerts[index].y = heightVal;
			rawVerts[index].z = (row * stepY);

			rawTexCoords[index].x = ((GLfloat)col / (GLfloat)widthX);
			rawTexCoords[index].y = ((GLfloat)row / (GLfloat)widthY);

			if( col == widthX - 1) //this will stitch the col side
			{
				int startOfCol = index - col;

				float averagedVal = (rawVerts[startOfCol].y + rawVerts[index].y) * .5f;
				rawVerts[index].y = averagedVal;
				rawVerts[startOfCol].y = averagedVal;
			}

			if( row == widthY - 1)
			{
				int otherSideIndex = col;

				float averagedVal = (rawVerts[otherSideIndex].y + rawVerts[index].y) * .5f;
				rawVerts[index].y = averagedVal;
				rawVerts[otherSideIndex].y = averagedVal;
			}

			index++;
		}
	}

	int numVerts = (widthX * widthY) + ((widthX - 1) * (widthY - 2));

	ArrayList<vec3> verts;
	verts.resize(numVerts);

	ArrayList<vec2> texCoords;
	texCoords.resize(numVerts);

	index = 0;
	for (int row = 0; row < widthY - 1; ++row)
	{
		if ((row & 1) == 0)
		{
			for (int col = 0; col < widthX; ++col)
			{
				int rawInd = col + (row * widthX);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;

				rawInd = col + ((row + 1) * widthX);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;
			}
		}
		else
		{
			for (int col = widthX - 1; col > 0; --col)
			{
				int rawInd = col + ((row + 1) * widthX);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;

				rawInd = col - 1 + (row * widthX);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;
			}
		}
	}

	if ((widthY & 1) && widthY > 2)
	{
		int rawInd = (widthY - 1) * widthX;
		verts[index]     = rawVerts[rawInd];
		texCoords[index] = rawTexCoords[rawInd];
		index++;
	}

	SDL_FreeSurface(pSurface);

	GLBatch* heightMapBatch = new GLBatch();
	heightMapBatch->Begin(GL_TRIANGLE_STRIP, numVerts, 1);

	if(!verts.isEmpty())
		heightMapBatch->CopyVertexData3f((GLfloat*)&verts[0]);
	if(!texCoords.isEmpty())
		heightMapBatch->CopyTexCoordData2f((GLfloat*)&texCoords[0], 0);

	heightMapBatch->End();

	Model* tempModel = resourceManager->addNewModel(Arc_Dirname(heightMapFilename));
	tempModel->addBatch(heightMapBatch);

	Material* tempMat = resourceManager->addNewMaterial(Arc_Dirname(heightMapFilename));
	tempMat->setDiffuseMap(resourceManager->LoadTexture(textureFilename, GL_LINEAR, GL_LINEAR, GL_REPEAT));
	tempModel->setMaterial(tempMat);

	addModel(Arc_Dirname(heightMapFilename), tempModel);

}

void Heightmap::loadHeightlessMap(ResourceManager* resourceManager, const string& textureFilename, const int& mapWidthX, const int& mapWidthY, const int& triDensity)
{
	cleanup();

	unsigned int mapVerts = triDensity * triDensity;

	ArrayList<vec3> rawVerts;
	rawVerts.resize(mapVerts);

	ArrayList<vec2> rawTexCoords;
	rawTexCoords.resize(mapVerts);

	GLfloat stepX = mapWidthX / (float)triDensity;
	GLfloat stepY = mapWidthY / (float)triDensity;

	mOffset.x = stepX;
	mOffset.y = stepY;

	m_Heights.clear();
	m_Heights.resize(triDensity);
	
	int index = 0;

	for (int row = 0; row < triDensity; ++row)
	{
		m_Heights.add(ArrayList<float>());
		m_Heights[row].resize(triDensity);

		for (int col = 0; col < triDensity; ++col)
		{
			m_Heights[row][col] = 0.0f;

			rawVerts[index].x = (col * stepX);
			rawVerts[index].y = 0.0f;
			rawVerts[index].z = (row * stepY);

			rawTexCoords[index].x = ((GLfloat)col / (GLfloat)triDensity);
			rawTexCoords[index].y = ((GLfloat)row / (GLfloat)triDensity);

			index++;
		}
	}

	int numVerts = (triDensity * triDensity) + ((triDensity - 1) * (triDensity - 2));

	ArrayList<vec3> verts;
	verts.resize(numVerts);

	ArrayList<vec2> texCoords;
	texCoords.resize(numVerts);

	index = 0;
	for (int row = 0; row < triDensity - 1; ++row)
	{
		if ((row & 1) == 0) //bitwise and
		{
			for (int col = 0; col < triDensity; ++col)
			{
				int rawInd = col + (row * triDensity);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;

				rawInd = col + ((row + 1) * triDensity);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;
			}
		}
		else
		{
			for (int col = triDensity - 1; col > 0; --col)
			{
				int rawInd = col + ((row + 1) * triDensity);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;

				rawInd = col - 1 + (row * triDensity);
				verts[index]     = rawVerts[rawInd];
				texCoords[index] = rawTexCoords[rawInd];
				++index;
			}
		}
	}

	if ((triDensity & 1) && triDensity > 2)
	{
		int rawInd = (triDensity - 1) * triDensity;
		verts[index]     = rawVerts[rawInd];
		texCoords[index] = rawTexCoords[rawInd];
		index++;
	}

	GLBatch* heightMapBatch = new GLBatch();
	heightMapBatch->Begin(GL_TRIANGLE_STRIP, numVerts, 1);

	if(!verts.isEmpty())
		heightMapBatch->CopyVertexData3f((GLfloat*)&verts[0]);
	if(!texCoords.isEmpty())
		heightMapBatch->CopyTexCoordData2f((GLfloat*)&texCoords[0], 0);

	heightMapBatch->End();

	Model* tempModel = resourceManager->addNewModel(Arc_Dirname(textureFilename));
	tempModel->addBatch(heightMapBatch);

	Material* tempMat = resourceManager->addNewMaterial(Arc_Dirname(textureFilename));
	tempMat->setDiffuseMap(resourceManager->LoadTexture(textureFilename, GL_LINEAR, GL_LINEAR, GL_REPEAT));
	tempModel->setMaterial(tempMat);

	addModel(Arc_Dirname(textureFilename), tempModel);
}

