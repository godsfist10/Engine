#include "Skybox.h"


Skybox::Skybox(void)
	:Object()
{
}

Skybox::Skybox(const string& textureFilename, ResourceManager* resourceManager, const GLfloat& scale)
{
	loadSkybox(textureFilename, resourceManager, scale);
}

Skybox::Skybox(const string& textureFilename, ResourceManager* resourceManager, const GLfloat& scale, const string& objectName)
{
	loadSkybox(textureFilename, resourceManager, scale);
	resourceManager->addObjectToMap(objectName, this);

}

Skybox::~Skybox()
{
	cleanup();
}


void Skybox::draw(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager)
{
	Object::draw(viewPoint, ProjectionMatrix, ProjectionViewPrecalced, shaderManager);
	//clearDepthBuffer();
}

void Skybox::loadSkybox(const string& texFilename, ResourceManager* resourceManager, const GLfloat& scale)
{
	cleanup();

	Texture* pTexture = resourceManager->LoadTexture(texFilename, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

	if( !pTexture)
		return;


	vec3 skyboxVerts[24] = {
		vec3(-1.0f,  1.0f, -1.0f), vec3( 1.0f,  1.0f, -1.0f), vec3( 1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), // Front
		vec3(-1.0f,  1.0f,  1.0f), vec3(-1.0f,  1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f,  1.0f), // Left
		vec3( 1.0f,  1.0f,  1.0f), vec3(-1.0f,  1.0f,  1.0f), vec3(-1.0f, -1.0f,  1.0f), vec3( 1.0f, -1.0f,  1.0f), // Back
		vec3( 1.0f,  1.0f, -1.0f), vec3( 1.0f,  1.0f,  1.0f), vec3( 1.0f, -1.0f,  1.0f), vec3( 1.0f, -1.0f, -1.0f), // Right
		vec3(-1.0f,  1.0f,  1.0f), vec3( 1.0f,  1.0f,  1.0f), vec3( 1.0f,  1.0f, -1.0f), vec3(-1.0f,  1.0f, -1.0f), // Top
		vec3(-1.0f, -1.0f,  1.0f), vec3( 1.0f, -1.0f,  1.0f), vec3( 1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f)  // Bottom
	};

	const GLfloat ONE_FOURTH   = 0.25f;
	const GLfloat TWO_FOURTH   = 0.5f;
	const GLfloat THREE_FOURTH = 0.75f;
	const GLfloat ONE_THIRD    = 1.0f/3.0f;
	const GLfloat TWO_THIRD    = 2.0f/3.0f;


	vec2 skyboxTxcrds[24] = {
		vec2( ONE_FOURTH,   1.0f - TWO_THIRD ), vec2( TWO_FOURTH,   1.0f - TWO_THIRD ), vec2( TWO_FOURTH,   1.0f - ONE_THIRD ), vec2( ONE_FOURTH,   1.0f - ONE_THIRD ), // Front
		vec2( 0.0f,         1.0f - TWO_THIRD ), vec2( ONE_FOURTH,   1.0f - TWO_THIRD ), vec2( ONE_FOURTH,   1.0f - ONE_THIRD ), vec2( 0.0f,         1.0f - ONE_THIRD ), // Left
		vec2( THREE_FOURTH, 1.0f - TWO_THIRD ), vec2( 1.0f,         1.0f - TWO_THIRD ), vec2( 1.0f,         1.0f - ONE_THIRD ), vec2( THREE_FOURTH, 1.0f - ONE_THIRD ), // Back
		vec2( TWO_FOURTH,   1.0f - TWO_THIRD ), vec2( THREE_FOURTH, 1.0f - TWO_THIRD ), vec2( THREE_FOURTH, 1.0f - ONE_THIRD ), vec2( TWO_FOURTH,   1.0f - ONE_THIRD ), // Right
		vec2( ONE_FOURTH,   1.0f - 1.0f      ), vec2( TWO_FOURTH,   1.0f - 1.0f      ), vec2( TWO_FOURTH,   1.0f - TWO_THIRD ), vec2( ONE_FOURTH,   1.0f - TWO_THIRD ), // Up
		vec2( ONE_FOURTH,   1.0f - 0.0f      ), vec2( TWO_FOURTH,   1.0f - 0.0f      ), vec2( TWO_FOURTH,   1.0f - ONE_THIRD ), vec2( ONE_FOURTH,   1.0f - ONE_THIRD )  // Down
	};

	for (int i = 0; i < 24; ++i)
		skyboxVerts[i] *= scale;

	Material* pMaterial =  resourceManager->addNewMaterial(Arc_Dirname(texFilename));
	pMaterial->setDiffuseMap(pTexture);

	GLBatch* batch = new GLBatch();
	batch->Begin(GL_QUADS, 24, 1);
	batch->CopyVertexData3f((GLfloat*)&skyboxVerts[0]);
	batch->CopyTexCoordData2f((GLfloat*)&(skyboxTxcrds[0]), 0);
	batch->End();

	Model* pModel = resourceManager->addNewModel(Arc_Dirname(texFilename));
	pModel->addBatch(batch);
	pModel->setMaterial(pMaterial);

	addModel(Arc_Dirname(texFilename), pModel);

}

