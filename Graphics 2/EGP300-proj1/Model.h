#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <vector>

#include "ShaderManager.h"

#include "Material.h"


using namespace std;
using namespace glm;

							
class Model
{

public:
	Model();
	~Model();

	void drawModel(const mat4x4& mvp, GLShaderManager& shaderManager);
	void drawModel(const mat4x4& mvp, Shader_Manager* shaderManager, const string& shaderName );
	void addBatch(GLBatch* tris);
	void cleanup();

	vector<GLBatch*> getBatches() const { return batches; }
	inline void setBatches(vector<GLBatch*> newBatches) { batches = newBatches; }
	inline void setMaterial(Material* material) {mpMaterial = material;}

private:

	vector<GLBatch*> batches;
	Material* mpMaterial;
	
};

