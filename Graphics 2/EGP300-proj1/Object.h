#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GL/glut.h>
#include <GLFrustum.h>
#include <vector>
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Model.h"

#include <Arc/Map.h>
#include "ShaderManager.h"

using namespace std;
using namespace glm;
using namespace Arc;

class Shader_Manager;

class Object
{
public:
	Object(void);
	Object(const Map<string, Model*> &modelsMap);

	inline Object(const Object& rhs)
		: mModelsMap(rhs.mModelsMap),
		mPos(rhs.mPos),
		mRotation(rhs.mRotation),
		mScale(rhs.mScale),
		mIsPrefab(rhs.mIsPrefab)
	{ }

	virtual ~Object(void);

	virtual void update(double deltaTime);

	virtual void draw(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, Shader_Manager* shaderManager, const string& shaderName);
	virtual void draw(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager);
	virtual void cleanup();

	void init(glm::vec3 pos);

	inline bool getIsPrefab() const { return mIsPrefab; }
	inline void setIsPrefab(bool val) { mIsPrefab = val; }

	inline void setPos(vec3 pos) { mPos = pos; }
	inline void setPos(float x, float y, float z) { setPos(glm::vec3(x, y, z)); }
	inline void setPos(Vector3D pos) { setPos(glm::vec3(pos.X, pos.Y, pos.Z)); }
	inline vec3 getPos() {return mPos;}
	inline void Translate(float x, float y, float z){mPos.x += x; mPos.y += y; mPos.z += z;}
	inline void Translate(vec3 val) { mPos += val; }
	inline void modifyPos(vec3 val) { mPos += val; }

	inline void setScale(vec3 scale) {mScale = scale;}
	inline void setScale(float scale) { mScale.x = scale; mScale.y = scale; mScale.z = scale; }
	inline void setScale(float x, float y, float z) { mScale.x = x; mScale.y = y; mScale.z = z; }
	inline void setScale(Vector3D scale) { mScale.x = scale.X, mScale.y = scale.Y, mScale.z = scale.Z; }
	inline vec3 getScale() {return mScale;}
	inline void modifyScale(float x, float y, float z){mScale.x += x; mScale.y += y; mScale.z += z;}

	inline void setRotation(vec3 rotation) {mRotation = rotation;}
	inline vec3 getRotation() {return mRotation;}
	inline void modifyRotation(float x, float y, float z){mRotation.x += x; mRotation.y += y; mRotation.z += z;}

	inline bool hasModel( const string& key ) { return mModelsMap.containsKey(key); }
	inline void addModel(string modelName, Model* modelToAdd){mModelsMap.add(modelName, modelToAdd);}
	inline void addModelMap(Map<string, Model*> &modelMapToAdd){ mModelsMap.insert(modelMapToAdd.itBegin(), modelMapToAdd.itEnd());}
	inline void removeModelFromMap(const string& key) { mModelsMap.removeKey(key); }

	inline Model* getModel(string modelName){return (hasModel(modelName) ? mModelsMap[modelName] : nullptr);}
	inline Map<string, Model*> getModelMap() { return mModelsMap; }

	virtual void resetObject();

protected:

	Map<string, Model*> mModelsMap;
	
	vec3 mPos;
	vec3 mScale;
	vec3 mRotation;
	
	bool mIsPrefab;
	
	vec3 startPos;
};

