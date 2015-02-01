#include "ResourceManager.h"


ResourceManager::ResourceManager(void)
{
	
}

ResourceManager::~ResourceManager(void)
{
	cleanup();
}

Texture* ResourceManager::LoadTexture(string fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	SDL_Surface *surface = IMG_Load(fileName.c_str());

	if ( ! surface)
	{
		cout << "Texture: " + fileName + " does not exist and did not load";
		return 0;
	}

	GLuint texID = 0;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	GLenum mode;

	if(fileName.substr(fileName.find_last_of(".") + 1) == "tga") 
	{
		mode = GL_BGR;

		if (surface->format->BytesPerPixel == 4)
			mode = GL_BGRA;
	}
	else
	{
		mode = GL_RGB;

		if (surface->format->BytesPerPixel == 4)
			mode = GL_RGBA;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	Texture* newTexture = new Texture(texID);
	stringstream temp;
	temp << m_TexturesMap.getSize();
	m_TexturesMap.add(temp.str(), newTexture);

	SDL_FreeSurface(surface);

	return newTexture;
}

void ResourceManager::loadMTLFile(const string& filename)
{
	ifstream file(filename);

	if ( ! file)
		return; // Shit broke

	string line;
	Material* currentMaterial = nullptr;

	while ( ! file.eof())
	{
		getline(file, line);

		if (line.length() == 0 || line[0] == '#')
			continue;

		// BUILD materials MAP HERE
		size_t spacePos = line.find(' ');
		if (spacePos == string::npos)
			continue;

		string command;
		command.assign(line, 0, spacePos);

		string data;
		data.assign(line, spacePos + 1, line.size() - spacePos);

		stringstream sstream(data);

		if (command == "newmtl")
		{
			currentMaterial = addNewMaterial(data);
		}
		else
		{
			if( currentMaterial == nullptr)
			{
				currentMaterial = addNewMaterial("DefaultMaterial");
			}

			if(command == "Kd")
			{
				float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

				stringstream ssPiece(data);

				ssPiece >> color[0];
				ssPiece >> color[1];
				ssPiece >> color[2];
				ssPiece >> color[3];

				currentMaterial->setDiffuseColor(vec4(color[0], color[1], color[2], color[3]));
			}
			else if (command == "map_Kd")
			{
				currentMaterial->setDiffuseMap(ResourceManager::LoadTexture(Arc_Dirname(filename) + "/" + data, GL_LINEAR, GL_LINEAR, GL_REPEAT));
			}
		}
	}

	if(!currentMaterial->hasDiffuseMap())
	{
		cout << "MTL file:" + filename + " does not include a reference to a diffuse map\n";
	}

}

void ResourceManager::cleanup()
{
	for (auto it = m_ModelsMap.itBegin(); it != m_ModelsMap.itEnd(); ++it)
	{
		Model* pModel = it->second;
		delete pModel;
	}
	m_ModelsMap.clear();

	for (auto it = m_MaterialsMap.itBegin(); it != m_MaterialsMap.itEnd(); ++it)
	{
		Material* pMat = it->second;
		delete pMat;
	}
	m_MaterialsMap.clear();

	for (auto it = m_TexturesMap.itBegin(); it != m_TexturesMap.itEnd(); ++it)
	{
		Texture* pTex = it->second;
		GLuint texID = pTex->getTextureID();
		glDeleteTextures(1, &texID);
		delete pTex;
	}
	m_TexturesMap.clear();

	for (auto it = m_ObjectsMap.itBegin(); it != m_ObjectsMap.itEnd(); ++it)
	{
		Object* pObject = it->second;
		delete pObject;
	}
	m_ObjectsMap.clear();

	BillBoardDrawOrder.clear();
	for (auto it = m_BillboardsMap.itBegin(); it != m_BillboardsMap.itEnd(); ++it)
	{
		BillboardedTexture* pBillboard = it->second;
		delete pBillboard;

	}
	m_BillboardsMap.clear();

}

void ResourceManager::LoadFile(const string& filename)
{
	// Massive help from Stephen Lane-Walsh

	string FileDirectory = Arc_Dirname(filename);

	Map<string, Model*> returnMap;
	
	ifstream file(filename);

	if ( ! file)
	{
		cout << "File: " + filename + " not loaded because it does not exist";
		return; // Shit broke

	}
	Model* currentModel = nullptr;

	// Stores all of them
	vector<vec3> verts;
	vector<vec3> norms;
	vector<vec2> texCoords;

	// Stores the copies for each of the groups (tris, quads)
	vector<vec3> triVerts;
	vector<vec3> quadVerts;

	vector<vec3> triNorms;
	vector<vec3> quadNorms;

	vector<vec4> triColors;
	vector<vec4> quadColors;

	vector<vec2> triTexCoords;
	vector<vec2> quadTexCoords;

	int triVertTotal = 0;
	int quadVertTotal = 0;

	bool oneMoreTime = true;

	string line;
	while ( ! file.eof() || oneMoreTime)
	{
		getline(file, line);

		if (file.eof() && oneMoreTime)
		{
			oneMoreTime = false;
			line = "o ";
		}

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

		if (cmd == "v")
		{
			static vec3 vert;

			ss >> vert.x >> vert.y >> vert.z;

			verts.push_back(vert);
		}
		else if (cmd == "vt")
		{
			static vec2 coord;

			ss >> coord.x >> coord.y;

			texCoords.push_back(coord);
		}
		else if (cmd == "vn")
		{
			static vec3 norm;

			ss >> norm.x >> norm.y >> norm.z;

			norms.push_back(norm);
		}
		else if (cmd == "f")
		{
			int vertInds[4] = { -1, -1, -1, -1 };
			int normInds[4] = { -1, -1, -1, -1 };
			int texInds[4]  = { -1, -1, -1, -1 };

			string piece;
			int count = 0;
			for (int i = 0; getline(ss, piece, ' '); ++i)
			{
				if (piece.size() == 0)
				{
					--i;
					continue;
				}

				count++;

				bool vertAndNorm = (piece.find("//") != string::npos);

				for (unsigned int j = 0; j < piece.size(); ++j)
				{
					if (piece[j] == '/')
						piece[j] = ' ';
				}

				stringstream ssPiece(piece);
				if (vertAndNorm)
				{
					ssPiece >> vertInds[i];
					ssPiece >> normInds[i];
				}
				else
				{
					ssPiece >> vertInds[i];
					ssPiece >> texInds[i];
					ssPiece >> normInds[i];
				}
			}

			if (count == 3) // Tri
			{
				triVertTotal += 3;

				if (vertInds[0] != -1 && vertInds[1] != -1 && vertInds[2] != -1)
				{
					triVerts.push_back(verts[vertInds[0] - 1]);
					triVerts.push_back(verts[vertInds[1] - 1]);
					triVerts.push_back(verts[vertInds[2] - 1]);
				}

				if (normInds[0] != -1 && normInds[1] != -1 && normInds[2] != -1)
				{
					triNorms.push_back(norms[normInds[0] - 1]);
					triNorms.push_back(norms[normInds[1] - 1]);
					triNorms.push_back(norms[normInds[2] - 1]);
				}

				if (texInds[0] != -1 && texInds[1] != -1 && texInds[2] != -1)
				{
					triTexCoords.push_back(vec2(texCoords[texInds[0] - 1].x, 1.0f - texCoords[texInds[0] - 1].y));
					triTexCoords.push_back(vec2(texCoords[texInds[1] - 1].x, 1.0f - texCoords[texInds[1] - 1].y));
					triTexCoords.push_back(vec2(texCoords[texInds[2] - 1].x, 1.0f - texCoords[texInds[2] - 1].y));
				}
			}
			else if (count == 4) // Quad
			{
				quadVertTotal += 4;

				if (vertInds[0] != -1 && vertInds[1] != -1 && vertInds[2] != -1 && vertInds[3] != -1)
				{
					quadVerts.push_back(verts[vertInds[0] - 1]);
					quadVerts.push_back(verts[vertInds[1] - 1]);
					quadVerts.push_back(verts[vertInds[2] - 1]);
					quadVerts.push_back(verts[vertInds[3] - 1]);
				}

				if (normInds[0] != -1 && normInds[1] != -1 && normInds[2] != -1 && normInds[3] != -1)
				{
					quadNorms.push_back(norms[normInds[0] - 1]);
					quadNorms.push_back(norms[normInds[1] - 1]);
					quadNorms.push_back(norms[normInds[2] - 1]);
					quadNorms.push_back(norms[normInds[3] - 1]);
				}

				if (texInds[0] != -1 && texInds[1] != -1 && texInds[2] != -1 && texInds[3] != -1)
				{
					quadTexCoords.push_back(vec2(texCoords[texInds[0] - 1].x,    1.0f - texCoords[texInds[0] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[1] - 1].x,    1.0f - texCoords[texInds[1] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[2] - 1].x,    1.0f - texCoords[texInds[2] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[3] - 1].x,    1.0f - texCoords[texInds[3] - 1].y));
				}
			}
		}
		else if (cmd == "o" || cmd == "g")
		{
			// We're already working on a model
			if ( currentModel != nullptr)
			{
				GLBatch* tris  = new GLBatch();
				GLBatch* quads = new GLBatch();

				triColors.resize(triVertTotal, vec4(1.0f, 1.0f, 1.0f, 1.0f));
				quadColors.resize(quadVertTotal, vec4(1.0f, 1.0f, 1.0f, 1.0f));

				tris->Begin(GL_TRIANGLES, triVertTotal, 1);

				if ( triVerts.size() > 0)
					tris->CopyVertexData3f((GLfloat*)&triVerts[0]);

				if ( triNorms.size() > 0)
					tris->CopyNormalDataf((GLfloat*)&triNorms[0]);

				if ( triColors.size() > 0)
					tris->CopyColorData4f((GLfloat*)&triColors[0]);

				
				if ( triTexCoords.size() > 0)
					tris->CopyTexCoordData2f((GLfloat*)&triTexCoords[0], 0);

				tris->End();


				quads->Begin(GL_TRIANGLES, quadVertTotal, 1);

				if ( quadVerts.size() > 0)
					quads->CopyVertexData3f((GLfloat*)&quadVerts[0]);

				if ( quadNorms.size() > 0)
					quads->CopyNormalDataf((GLfloat*)&quadNorms[0]);

				if ( quadColors.size() > 0)
					quads->CopyColorData4f((GLfloat*)&quadColors[0]);

				if ( quadTexCoords.size() > 0)
					quads->CopyTexCoordData2f((GLfloat*)&quadTexCoords[0], 0);

				quads->End();

				currentModel->addBatch(tris);
				currentModel->addBatch(quads);

				triVertTotal = 0;
				quadVertTotal = 0;

				triVerts.clear();
				triNorms.clear();
				triColors.clear();
				triTexCoords.clear();

				quadVerts.clear();
				quadNorms.clear();
				quadColors.clear();
				quadTexCoords.clear();

			}

			if (data.size() != 0)
			{
				currentModel = new Model();
				returnMap.add(data, currentModel);
			}

		}
		else if (cmd == "mtllib")
		{
			loadMTLFile(FileDirectory + "/" + data);
		}
		else if (cmd == "usemtl")
		{
			currentModel->setMaterial(getMaterial(data));
		}
	}

	file.close();
	addModelsToMap(returnMap);
	Object* newObject = new Object(returnMap);
	newObject->setIsPrefab(true);
	m_ObjectsMap.add(FileDirectory, newObject);

}

void ResourceManager::LoadFile(const string& filename, const string& key)
{
	if (this->hasObject(key))
	{
		cout << "Key: " + key + "already in use in object map\n";
		return;
	}

	string FileDirectory = Arc_Dirname(filename);

	Map<string, Model*> returnMap;

	ifstream file(filename);

	if (!file)
	{
		cout << "File: " + filename + " not loaded because it does not exist\n";
		return; // Shit broke

	}
	Model* currentModel = nullptr;

	// Stores all of them
	vector<vec3> verts;
	vector<vec3> norms;
	vector<vec2> texCoords;

	// Stores the copies for each of the groups (tris, quads)
	vector<vec3> triVerts;
	vector<vec3> quadVerts;

	vector<vec3> triNorms;
	vector<vec3> quadNorms;

	vector<vec4> triColors;
	vector<vec4> quadColors;

	vector<vec2> triTexCoords;
	vector<vec2> quadTexCoords;

	int triVertTotal = 0;
	int quadVertTotal = 0;

	bool oneMoreTime = true;

	string line;
	while (!file.eof() || oneMoreTime)
	{
		getline(file, line);

		if (file.eof() && oneMoreTime)
		{
			oneMoreTime = false;
			line = "o ";
		}

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

		if (cmd == "v")
		{
			static vec3 vert;

			ss >> vert.x >> vert.y >> vert.z;

			verts.push_back(vert);
		}
		else if (cmd == "vt")
		{
			static vec2 coord;

			ss >> coord.x >> coord.y;

			texCoords.push_back(coord);
		}
		else if (cmd == "vn")
		{
			static vec3 norm;

			ss >> norm.x >> norm.y >> norm.z;

			norms.push_back(norm);
		}
		else if (cmd == "f")
		{
			int vertInds[4] = { -1, -1, -1, -1 };
			int normInds[4] = { -1, -1, -1, -1 };
			int texInds[4] = { -1, -1, -1, -1 };

			string piece;
			int count = 0;
			for (int i = 0; getline(ss, piece, ' '); ++i)
			{
				if (piece.size() == 0)
				{
					--i;
					continue;
				}

				count++;

				bool vertAndNorm = (piece.find("//") != string::npos);

				for (unsigned int j = 0; j < piece.size(); ++j)
				{
					if (piece[j] == '/')
						piece[j] = ' ';
				}

				stringstream ssPiece(piece);
				if (vertAndNorm)
				{
					ssPiece >> vertInds[i];
					ssPiece >> normInds[i];
				}
				else
				{
					ssPiece >> vertInds[i];
					ssPiece >> texInds[i];
					ssPiece >> normInds[i];
				}
			}

			if (count == 3) // Tri
			{
				triVertTotal += 3;

				if (vertInds[0] != -1 && vertInds[1] != -1 && vertInds[2] != -1)
				{
					triVerts.push_back(verts[vertInds[0] - 1]);
					triVerts.push_back(verts[vertInds[1] - 1]);
					triVerts.push_back(verts[vertInds[2] - 1]);
				}

				if (normInds[0] != -1 && normInds[1] != -1 && normInds[2] != -1)
				{
					triNorms.push_back(norms[normInds[0] - 1]);
					triNorms.push_back(norms[normInds[1] - 1]);
					triNorms.push_back(norms[normInds[2] - 1]);
				}

				if (texInds[0] != -1 && texInds[1] != -1 && texInds[2] != -1)
				{
					triTexCoords.push_back(vec2(texCoords[texInds[0] - 1].x, 1.0f - texCoords[texInds[0] - 1].y));
					triTexCoords.push_back(vec2(texCoords[texInds[1] - 1].x, 1.0f - texCoords[texInds[1] - 1].y));
					triTexCoords.push_back(vec2(texCoords[texInds[2] - 1].x, 1.0f - texCoords[texInds[2] - 1].y));
				}
			}
			else if (count == 4) // Quad
			{
				quadVertTotal += 4;

				if (vertInds[0] != -1 && vertInds[1] != -1 && vertInds[2] != -1 && vertInds[3] != -1)
				{
					quadVerts.push_back(verts[vertInds[0] - 1]);
					quadVerts.push_back(verts[vertInds[1] - 1]);
					quadVerts.push_back(verts[vertInds[2] - 1]);
					quadVerts.push_back(verts[vertInds[3] - 1]);
				}

				if (normInds[0] != -1 && normInds[1] != -1 && normInds[2] != -1 && normInds[3] != -1)
				{
					quadNorms.push_back(norms[normInds[0] - 1]);
					quadNorms.push_back(norms[normInds[1] - 1]);
					quadNorms.push_back(norms[normInds[2] - 1]);
					quadNorms.push_back(norms[normInds[3] - 1]);
				}

				if (texInds[0] != -1 && texInds[1] != -1 && texInds[2] != -1 && texInds[3] != -1)
				{
					quadTexCoords.push_back(vec2(texCoords[texInds[0] - 1].x, 1.0f - texCoords[texInds[0] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[1] - 1].x, 1.0f - texCoords[texInds[1] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[2] - 1].x, 1.0f - texCoords[texInds[2] - 1].y));
					quadTexCoords.push_back(vec2(texCoords[texInds[3] - 1].x, 1.0f - texCoords[texInds[3] - 1].y));
				}
			}
		}
		else if (cmd == "o" || cmd == "g")
		{
			// We're already working on a model
			if (currentModel != nullptr)
			{
				GLBatch* tris = new GLBatch();
				GLBatch* quads = new GLBatch();

				triColors.resize(triVertTotal, vec4(1.0f, 1.0f, 1.0f, 1.0f));
				quadColors.resize(quadVertTotal, vec4(1.0f, 1.0f, 1.0f, 1.0f));

				tris->Begin(GL_TRIANGLES, triVertTotal, 1);

				if (triVerts.size() > 0)
					tris->CopyVertexData3f((GLfloat*)&triVerts[0]);

				if (triNorms.size() > 0)
					tris->CopyNormalDataf((GLfloat*)&triNorms[0]);

				if (triColors.size() > 0)
					tris->CopyColorData4f((GLfloat*)&triColors[0]);


				if (triTexCoords.size() > 0)
					tris->CopyTexCoordData2f((GLfloat*)&triTexCoords[0], 0);

				tris->End();


				quads->Begin(GL_TRIANGLES, quadVertTotal, 1);

				if (quadVerts.size() > 0)
					quads->CopyVertexData3f((GLfloat*)&quadVerts[0]);

				if (quadNorms.size() > 0)
					quads->CopyNormalDataf((GLfloat*)&quadNorms[0]);

				if (quadColors.size() > 0)
					quads->CopyColorData4f((GLfloat*)&quadColors[0]);

				if (quadTexCoords.size() > 0)
					quads->CopyTexCoordData2f((GLfloat*)&quadTexCoords[0], 0);

				quads->End();

				currentModel->addBatch(tris);
				currentModel->addBatch(quads);

				triVertTotal = 0;
				quadVertTotal = 0;

				triVerts.clear();
				triNorms.clear();
				triColors.clear();
				triTexCoords.clear();

				quadVerts.clear();
				quadNorms.clear();
				quadColors.clear();
				quadTexCoords.clear();

			}

			if (data.size() != 0)
			{
				currentModel = new Model();
				returnMap.add(data, currentModel);
			}

		}
		else if (cmd == "mtllib")
		{
			loadMTLFile(FileDirectory + "/" + data);
		}
		else if (cmd == "usemtl")
		{
			currentModel->setMaterial(getMaterial(data));
		}
	}

	file.close();
	addModelsToMap(returnMap);
	Object* newObject = new Object(returnMap);
	newObject->setIsPrefab(true);
	m_ObjectsMap.add(key, newObject);

}

Material* ResourceManager::addNewMaterial(const string& key)
{
	Material* tempMat = new Material();
	m_MaterialsMap.add(key, tempMat);
	return tempMat;
}

Model* ResourceManager::addNewModel(const string& key)
{
	Model* tempModel = new Model();
	m_ModelsMap.add(key, tempModel);
	return tempModel;
}

Object* ResourceManager::addNewObject(const string& key)
{
	Object* tempObject = new Object();
	m_ObjectsMap.add(key, tempObject);
	return tempObject;
}

Object* ResourceManager::addNewObject(const string& ObjectName, const Map<string, Model*> &modelsMap)
{
	Object* tempObject = new Object(modelsMap);
	m_ObjectsMap.add(ObjectName, tempObject);
	return tempObject;
}

PhysicsObject* ResourceManager::addNewPhysicsObject(const string& ObjectName, const Map<string, Model*> &modelsMap)
{
	PhysicsObject* tempObject = new PhysicsObject(modelsMap);
	m_ObjectsMap.add(ObjectName, tempObject);
	return tempObject;
}

Particle* ResourceManager::addNewParticle(const string& particleName, const Map<string, Model*> &modelsMap, const float& lifespan)
{
	Particle* tempObject = new Particle(modelsMap, lifespan);
	m_ObjectsMap.add(particleName, tempObject);
	return tempObject;
}

void ResourceManager::deleteObject(const string& key)
{
	if (hasObject(key))
	{
		delete m_ObjectsMap[key];
		m_ObjectsMap.removeKey(key);
	}

}
void ResourceManager::deleteBillboard(const string& key)
{
	if (hasBillboard(key))
	{
		delete m_BillboardsMap[key];
		m_BillboardsMap.removeKey(key);
	}
}

void ResourceManager::updateObjects(vec3 cameraPos)
{
	for( auto it = m_ObjectsMap.itBegin(); it != m_ObjectsMap.itEnd(); ++it)
	{
		Object* pObject = it->second;
		pObject->update();
	}

	for (auto it = m_BillboardsMap.itBegin(); it != m_BillboardsMap.itEnd(); ++it)
	{
		BillboardedTexture* pBillboard = it->second;
		pBillboard->billBoardUpdate(cameraPos);
	}

	if (m_BillboardsMap.getSize() != BillBoardDrawOrder.getSize())
	{
		cout << "Resource Manager:  billboard draw order vector not the same size as billboard map\n";
	}

	//furthest first
	if (BillBoardDrawOrder.getSize() > 0)
	{
		float dist;
		for (unsigned int i = 1; i < BillBoardDrawOrder.getSize(); ++i)
		{
			dist = getXZDistance(cameraPos, BillBoardDrawOrder[i - 1]->getPos());
			if (getXZDistance(cameraPos, BillBoardDrawOrder[i]->getPos()) > dist)
			{
				BillboardedTexture* temp = BillBoardDrawOrder[i];

				BillBoardDrawOrder.removeAt(i);
				BillBoardDrawOrder.insertAt(temp, i - 1);

				i -= 1;
			}
		}
	}
}

void ResourceManager::drawAllObjects(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager)
{
	for( auto it = m_ObjectsMap.itBegin(); it != m_ObjectsMap.itEnd(); ++it)
	{
		Object* pObject = it->second;
		pObject->draw(viewPoint, ProjectionMatrix, ProjectionViewPrecalced, shaderManager);
	}

	for (unsigned int i = 0; i < BillBoardDrawOrder.getSize(); ++i)
	{
		BillBoardDrawOrder[i]->draw(viewPoint, ProjectionMatrix, ProjectionViewPrecalced, shaderManager);
	}
	
}

float ResourceManager::getXZDistance(vec3 camPos, vec3 pos)
{
	return( pow(camPos.x - pos.x, 2.0f) + pow( camPos.z - pos.z, 2.0f));
}

void ResourceManager::drawObject(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, Shader_Manager* shaderManager, const string& objectName)
{
	if( !hasObject(objectName))
	{
		cout << "Cannot draw: " + objectName;
		return;
	}

	getObject(objectName)->draw(viewPoint, ProjectionMatrix, ProjectionViewPrecalced, shaderManager);
}

