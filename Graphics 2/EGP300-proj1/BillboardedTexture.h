#pragma once

#include "Object.h"
 #include "ResourceManager.h"

using namespace glm;

class ResourceManager;

class BillboardedTexture : public Object
{
public:
	BillboardedTexture(void);
	BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename);
	BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, bool axisAligning);
	BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, const string& objectName);
	BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, bool axisAligning, const string& objectName);
	~BillboardedTexture(void);

	void loadBillboard(ResourceManager* resourceManager, const string& textureFilename);
	virtual void update(vec3 cameraPos);

	bool AxisAligned() const { return axisAligned; }
	void AxisAligned(bool val) { axisAligned = val; }

protected:

	bool axisAligned;

};
