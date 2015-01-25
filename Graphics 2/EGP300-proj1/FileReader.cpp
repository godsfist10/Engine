#include "FileReader.h"


FileReader::FileReader(void)
{
}

FileReader::~FileReader(void)
{
}

map<string, Model*> FileReader::readFile(string filename)
{
	// Massive help from Stephen Lane-Walsh

	map<string, Model*> returnMap;

	ifstream file(filename);

	if ( ! file)
		return returnMap; // Shit broke

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
					triTexCoords.push_back(texCoords[texInds[0] - 1]);
					triTexCoords.push_back(texCoords[texInds[1] - 1]);
					triTexCoords.push_back(texCoords[texInds[2] - 1]);
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
					quadTexCoords.push_back(texCoords[texInds[0] - 1]);
					quadTexCoords.push_back(texCoords[texInds[1] - 1]);
					quadTexCoords.push_back(texCoords[texInds[2] - 1]);
					quadTexCoords.push_back(texCoords[texInds[3] - 1]);
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

				tris->Begin(GL_TRIANGLES, triVertTotal);

				if ( triVerts.size() > 0)
					tris->CopyVertexData3f((GLfloat*)&triVerts[0]);

				if ( triNorms.size() > 0)
					tris->CopyNormalDataf((GLfloat*)&triNorms[0]);

				if ( triColors.size() > 0)
					tris->CopyColorData4f((GLfloat*)&triColors[0]);

				//enable texture coords once I have materials
				//if ( triTexCoords.size() > 0)
				//	tris->CopyTexCoordData2f((GLfloat*)&triTexCoords[0]);

				tris->End();


				quads->Begin(GL_TRIANGLES, quadVertTotal);

				if ( quadVerts.size() > 0)
					quads->CopyVertexData3f((GLfloat*)&quadVerts[0]);

				if ( quadNorms.size() > 0)
					quads->CopyNormalDataf((GLfloat*)&quadNorms[0]);

				if ( quadColors.size() > 0)
					quads->CopyColorData4f((GLfloat*)&quadColors[0]);

				//if ( quadTexCoords.size() > 0)
				//	quads->CopyTexCoordData2f((GLfloat*)&quadTexCoords[0]);

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
				returnMap.insert(pair<string, Model*>(data, currentModel));
			}

		}
		else if (cmd == "mtllib")
		{
			//loadMTLFile(data);

		}
		else if (cmd == "usemtl")
		{

		}
	}

	file.close();
	return returnMap;
}

GLuint FileReader::LoadTexture(string fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	SDL_Surface *surface = IMG_Load(fileName.c_str());

	if ( ! surface)
	{
		// Error
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

	

	SDL_FreeSurface(surface);

	return texID;
}

