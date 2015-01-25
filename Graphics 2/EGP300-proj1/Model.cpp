#include "Model.h"


Model::Model(void)
	:mpMaterial(nullptr)
{
}


Model::~Model(void)
{
	cleanup();
}


void Model::drawModel(const mat4x4& mvp, GLShaderManager& shaderManager)
{
	mpMaterial->bindTexture();

	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, &mvp, &mpMaterial->getDiffuseColor(), 0);
	for (auto it = batches.begin(); it != batches.end(); ++it)
	{
		GLBatch* pBatch = (*it);
		pBatch->Draw();
	}
}

void Model::drawModel(const mat4x4& mvp, Shader_Manager* shaderManager)
{
	mpMaterial->bindTexture();

	shaderManager->setupForDraw("f", mvp);

	for (auto it = batches.begin(); it != batches.end(); ++it)
	{
		GLBatch* pBatch = (*it);
		pBatch->Draw();
	}

}

void Model::addBatch(GLBatch* batch)
{
	batches.push_back(batch);
}

void Model::cleanup()
{
	batches.clear();
}
