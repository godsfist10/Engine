#include "BillboardedTexture.h"


BillboardedTexture::BillboardedTexture(void)
 {
	axisAligned = false;
	}

BillboardedTexture::BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename)
{
	loadBillboard(resourceManager, textureFilename);
	axisAligned = false;
}

BillboardedTexture::BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, bool axisAligning)
{
	loadBillboard(resourceManager, textureFilename);
	axisAligned = axisAligning;
}

BillboardedTexture::BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, bool axisAligning, const string& objectName)
{
	loadBillboard(resourceManager, textureFilename);
	axisAligned = axisAligning;
	resourceManager-> addBillboardToMap(objectName, this);
}

BillboardedTexture::BillboardedTexture(ResourceManager* resourceManager, const string& textureFilename, const string& objectName)
{
	loadBillboard(resourceManager, textureFilename);
	axisAligned = false;
	resourceManager-> addBillboardToMap(objectName, this);
}


BillboardedTexture::~BillboardedTexture(void)
{
}

void BillboardedTexture::loadBillboard(ResourceManager* resourceManager, const string& textureFilename)
{
	cleanup();

	if (resourceManager-> hasModel(Arc_Dirname(textureFilename)))
	{
		addModel(Arc_Dirname(textureFilename), resourceManager-> getModel(Arc_Dirname(textureFilename)));
		return;
	}

	SDL_Surface* pSurface = IMG_Load(textureFilename.c_str());

	if (!pSurface)
	{
		cout << "texture: " + textureFilename + " failed to load\n";
		return;
	}

	float halfTextureWidth = pSurface-> w * .5f;
	float halfTextureHeight = pSurface-> h * .5f;

	vec3 textureBoxVerts[4] = {
		vec3(-1.0f * halfTextureWidth, -1.0f * halfTextureHeight, 0.0f), 
		vec3(-1.0f * halfTextureWidth, 1.0f * halfTextureHeight, 0.0f),
		vec3(1.0f * halfTextureWidth, -1.0f * halfTextureHeight, 0.0f),
		vec3(1.0f * halfTextureWidth, 1.0f * halfTextureHeight, 0.0f)
	};

	vec2 texCoords[4] = {
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f)
	};

	SDL_FreeSurface(pSurface);

	GLBatch* billboardBatch = new GLBatch();
	billboardBatch-> Begin(GL_TRIANGLE_STRIP, 4, 1);

	billboardBatch-> CopyVertexData3f((GLfloat*)&textureBoxVerts[0]);
	billboardBatch-> CopyTexCoordData2f((GLfloat*)&texCoords[0], 0);
	billboardBatch-> End();

	Model* tempModel = resourceManager-> addNewModel(Arc_Dirname(textureFilename));
	tempModel-> addBatch(billboardBatch);

	Material* tempMat = resourceManager-> addNewMaterial(Arc_Dirname(textureFilename));
	tempMat-> setDiffuseMap(resourceManager-> LoadTexture(textureFilename, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE));
	tempModel-> setMaterial(tempMat);

	addModel(Arc_Dirname(textureFilename), tempModel);

}

void BillboardedTexture::billBoardUpdate(vec3 cameraPos)
{
	if (axisAligned)
	{
		vec3 diff = cameraPos - mPos;
		mRotation.y = atan2((diff.x), (diff.z));
	}

}