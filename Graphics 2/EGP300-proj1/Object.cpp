#include "Object.h"


Object::Object(void)
	:mScale(1.0f,1.0f,1.0f)
	,mPos(0.0f,0.0f,0.0f)
	,mRotation(0.0f,0.0f,0.0f)
	,mIsPrefab(false)
	,startPos(0, 0, 0)
{
}

Object::Object(const Map<string, Model*> &modelsMap)
	:mScale(1.0f,1.0f,1.0f)
	,mPos(0.0f,0.0f,0.0f)
	,mRotation(0.0f,0.0f,0.0f)
	,mIsPrefab(false)
	,startPos(0,0,0)
	,mModelsMap(modelsMap)
{
}

void Object::resetObject()
{
	mPos = startPos;
}

void Object::init(glm::vec3 pos)
{
	mPos = pos;
	startPos = pos;
}

void Object::update(double deltaTime)
{

}


Object::~Object(void)
{
	cleanup();
}

void Object::draw(const mat4x4& view, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager)
{
	if( mIsPrefab )
		return;

	mat4x4 mModel = mat4(1.0f);
	mat4x4 mViewProjection, mMVP;

	mModel = translate(mModel, vec3(mPos.x, mPos.y, mPos.z));
	mModel = scale(mModel, vec3(mScale.x, mScale.y, mScale.z));

	mModel = rotate(mModel, mRotation.x, vec3(1.0f, 0.0f, 0.0f));
	mModel = rotate(mModel, mRotation.y, vec3(0.0f, 1.0f, 0.0f));
	mModel = rotate(mModel, mRotation.z, vec3(0.0f, 0.0f, 1.0f));

	mMVP = ProjectionViewPrecalced * mModel;

	for (auto it = mModelsMap.itBegin(); it != mModelsMap.itEnd(); ++it)
	{
		Model* pModelToDraw = it->second;
		pModelToDraw->drawModel(mMVP, shaderManager);
	}
}

void Object::draw(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, Shader_Manager* shaderManager, const string& shaderName)
{
	mat4x4 mModel = mat4(1.0f);
	mat4x4 mViewProjection, mMVP;

	mModel = translate(mModel, vec3(mPos.x, mPos.y, mPos.z));
	mModel = scale(mModel, vec3(mScale.x, mScale.y, mScale.z));

	mModel = rotate(mModel, mRotation.x, vec3(1.0f, 0.0f, 0.0f));
	mModel = rotate(mModel, mRotation.y, vec3(0.0f, 1.0f, 0.0f));
	mModel = rotate(mModel, mRotation.z, vec3(0.0f, 0.0f, 1.0f));

	mMVP = ProjectionViewPrecalced * mModel;

	for (auto it = mModelsMap.itBegin(); it != mModelsMap.itEnd(); ++it)
	{
		Model* pModelToDraw = it->second;
		pModelToDraw->drawModel(mMVP, shaderManager, shaderName);
	}
}

void Object::cleanup()
{
	mModelsMap.clear();
}


