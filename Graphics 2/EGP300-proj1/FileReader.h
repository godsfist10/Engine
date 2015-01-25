#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#undef main

#include <string>
#include <sstream>
#include "Model.h"
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

class FileReader
{
public:
	FileReader(void);
	~FileReader(void);
	
	map<string, Model*> readFile(string filename);
	GLuint LoadTexture(string fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);


};

