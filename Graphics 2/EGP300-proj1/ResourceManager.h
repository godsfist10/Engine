#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#undef main

#include <string>
#include <sstream>

#include "Material.h"
#include "Model.h"
#include "Object.h"
#include "BillboardedTexture.h"
#include "PhysicsObject.h"
#include "Particle.h"
#include "Debug.h"

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <Arc/Map.h>
#include <Arc/StringFunctions.h>

class BillboardedTexture;
class PhysicsObject;

using namespace std;
using namespace Arc;

class ResourceManager
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	void updateObjects(vec3 cameraPos);
	void drawAllObjects( const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, GLShaderManager& shaderManager);
	void drawObject(const mat4x4& viewPoint, const mat4x4& ProjectionMatrix, const mat4x4& ProjectionViewPrecalced, Shader_Manager* shaderManager, const string& objectName);
	void cleanup();
	void LoadFile(const string& filename);
	void LoadFile(const string& filename, const string& key);
	Texture* LoadTexture(string fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void loadMTLFile(const string& filename);

	inline bool hasModel( const string& key ) { return m_ModelsMap.containsKey(key); }
	inline bool hasMaterial( const string& key ) { return m_MaterialsMap.containsKey(key); }
	inline bool hasObject( const string& key ) { return m_ObjectsMap.containsKey(key); }
	inline bool hasTexture( const string& key) { return m_TexturesMap.containsKey(key); }
	inline bool hasBillboard(const string& key) { return m_BillboardsMap.containsKey(key); }
	
	inline Map<string, Model*> getModelsMap() const { return m_ModelsMap; }
	inline Map<string, Material*> getMaterialsMap() const { return m_MaterialsMap; }
	inline Map<string , Object*> getObjectsMap() const { return m_ObjectsMap; }
	inline Map<string, Texture*> getTexturesMap() const { return m_TexturesMap; }
	inline Map<string, BillboardedTexture*> getBillboardsMap() const { return m_BillboardsMap; }

	inline void addModelsToMap( Map<string, Model*>& modelsToAdd) { m_ModelsMap.insert(modelsToAdd.itBegin(), modelsToAdd.itEnd());}
	inline void addMaterialsToMap( Map<string, Material*>& materialsToAdd)  { m_MaterialsMap.insert(materialsToAdd.itBegin(), materialsToAdd.itEnd()); }
	inline void addTexturesToMap( Map<string, Texture*>& texturesToAdd) { m_TexturesMap.insert(texturesToAdd.itBegin(), texturesToAdd.itEnd()); }
	inline void addObjectsToMap( Map<string, Object*>&objects) { m_ObjectsMap.insert(objects.itBegin(), objects.itEnd()); }
	inline void addBillboardsToMap(Map<string, BillboardedTexture*>&billboards) { m_BillboardsMap.insert(billboards.itBegin(), billboards.itEnd()); } //if this is used I need to add them to draw order
	
	inline void addModelToMap( const string& key, Model* model) { m_ModelsMap.add(key, model); }
	inline void addMaterialToMap( const string& key, Material* material) { m_MaterialsMap.add(key, material); }
	inline void addObjectToMap( const string& key, Object* object) { m_ObjectsMap.add(key, object); }
	inline void addTextureToMap( const string& key, Texture* texture) { m_TexturesMap.add(key, texture); }
	inline void addBillboardToMap(const string& key, BillboardedTexture* billboard) { m_BillboardsMap.add(key, billboard); addBillboardToDrawOrder(billboard); }
	inline void addBillboardToDrawOrder(BillboardedTexture* billboard) { BillBoardDrawOrder.add(billboard); }
	
	inline void removeObjectFromMap( const string& key) { hasObject(key) ? m_ObjectsMap.removeKey(key) : nullptr;}
	inline void removeBillboardFromMap( const string& key) { hasBillboard(key) ? m_BillboardsMap.removeKey(key) : nullptr;}
	void deleteObject(const string& key);
	void deleteBillboard(const string& key);


	inline Model* getModel(const string& key ) { return (hasModel(key) ? m_ModelsMap[key] : nullptr);}
	inline Material* getMaterial( const string& key ) { return (hasMaterial(key) ? m_MaterialsMap[key] : nullptr); }
	inline Texture* getTexture( const string& key ) { return (hasTexture(key) ? m_TexturesMap[key] : nullptr); }
	inline Object* getObject( const string& key ) { return (hasObject(key) ? m_ObjectsMap[key] : nullptr); }
	inline BillboardedTexture* getBillboard(const string& key) { return (hasBillboard(key) ? m_BillboardsMap[key] : nullptr); }
	
	Model* addNewModel(const string& key );
	Object* addNewObject( const string& key );
	Material* addNewMaterial(const string& key );
	Object* addNewObject( const string& ObjectName, const Map<string, Model*> &modelsMap );
	PhysicsObject* addNewPhysicsObject(const string& ObjectName, const Map<string, Model*> &modelsMap);
	Particle* addNewParticle(const string& particleName, const Map<string, Model*> &modelsMap,  const float& lifespan);


private:

	float getXZDistance(vec3 camPos, vec3 pos);

	ArrayList<BillboardedTexture*> BillBoardDrawOrder;
	Map<string, Object*> m_ObjectsMap;
	Map<string, Texture*> m_TexturesMap;
	Map<string, Material*> m_MaterialsMap;
	Map<string, Model*> m_ModelsMap;
	Map<string, BillboardedTexture*> m_BillboardsMap;

};

