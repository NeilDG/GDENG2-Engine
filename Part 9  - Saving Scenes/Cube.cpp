#include "Cube.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "SceneCameraHandler.h"
#include "ShaderLibrary.h"


Cube::Cube(String name, bool skipInit):AGameObject(name, AGameObject::PrimitiveType::CUBE)
{
	if (skipInit) {
		return;
	}

	ShaderNames shaderNames;
	void* shaderByteCode = NULL;
	size_t sizeShader = 0;
	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.BASE_VERTEX_SHADER_NAME, &shaderByteCode, &sizeShader);

	//create buffers for drawing. vertex data that needs to be drawn are temporarily placed here.
	Vertex quadList[] = {
		//X, Y, Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,1,1),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,1), Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,0.5f,-0.5f),   Vector3D(0,0,0),  Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(0,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{Vector3D(0.5f,-0.5f,0.5f),    Vector3D(1,0,0), Vector3D(0,0.2f,0) },
		{Vector3D(0.5f,0.5f,0.5f),    Vector3D(1,1,0), Vector3D(0,0.2f,0.2f) },
		{Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,0,0),  Vector3D(0,0.2f,0.2f) },
		{Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,0,0), Vector3D(0,0.2f,0) },
	};

	this->vertexBuffer = GraphicsEngine::getInstance()->createVertexBuffer();
	this->vertexBuffer->load(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);

	unsigned int indexList[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};
	this->indexBuffer = GraphicsEngine::getInstance()->createIndexBuffer();
	this->indexBuffer->load(indexList, ARRAYSIZE(indexList));

	//create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::getInstance()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));
}

Cube::~Cube()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	/*float inc = this->orientation.y;
	inc += deltaTime * 0.5f;
	this->orientation.y = inc;*/
}

void Cube::draw(int width, int height)
{
	//set vertex shader and pixel shader for the object
	ShaderNames shaderNames;
	DeviceContext* deviceContext = GraphicsEngine::getInstance()->getImmediateContext();
	deviceContext->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME), ShaderLibrary::getInstance()->getPixelShader(shaderNames.BASE_PIXEL_SHADER_NAME));

	CBData cbData = {};

	if (this->overrideMatrix) {
		cbData.worldMatrix = this->localMatrix;
	}
	else {
		/*Matrix4x4 allMatrix; allMatrix.setIdentity();
		Matrix4x4 translationMatrix; translationMatrix.setIdentity();  translationMatrix.setTranslation(this->getLocalPosition());
		Matrix4x4 scaleMatrix; scaleMatrix.setScale(this->getLocalScale());
		Vector3D rotation = this->getLocalRotation();
		Matrix4x4 xMatrix; xMatrix.setRotationX(rotation.getX());
		Matrix4x4 yMatrix; yMatrix.setRotationY(rotation.getY());
		Matrix4x4 zMatrix; zMatrix.setRotationZ(rotation.getZ());

		//Scale --> Rotate --> Transform as recommended order.
		Matrix4x4 rotMatrix; rotMatrix.setIdentity();
		rotMatrix = rotMatrix.multiplyTo(xMatrix.multiplyTo(yMatrix.multiplyTo(zMatrix)));

		allMatrix = allMatrix.multiplyTo(scaleMatrix.multiplyTo(rotMatrix));
		allMatrix = allMatrix.multiplyTo(translationMatrix);
		cbData.worldMatrix = allMatrix;*/
		this->updateLocalMatrix();
		cbData.worldMatrix = this->localMatrix;
	}
	
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	float aspectRatio = (float)width / (float)height;
	//cbData.projMatrix.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);
	cbData.projMatrix.setPerspectiveFovLH(2, aspectRatio, 0.1f, 1000.0f);

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(this->constantBuffer);
	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->drawTriangle(this->indexBuffer->getIndexSize(), 0, 0);

	//this->localMatrix = cbData.projMatrix;
}
