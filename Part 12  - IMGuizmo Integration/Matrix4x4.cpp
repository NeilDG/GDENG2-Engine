#include "Matrix4x4.h"
#include "ImGuizmo.h"


Matrix4x4::Matrix4x4()
{
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::setIdentity()
{
	//set array as zeros
	this->matrixInitialize();
	this->matrix[0][0] = 1;
	this->matrix[1][1] = 1;
	this->matrix[2][2] = 1;
	this->matrix[3][3] = 1;
}

void Matrix4x4::setMatrix(const Matrix4x4 matrix)
{
	::memcpy(this->matrix, matrix.matrix, sizeof(float) * 16);
}

void Matrix4x4::setMatrix(float matrix[4][4])
{
	::memcpy(this->matrix, matrix, sizeof(float) * 16);
}

void Matrix4x4::setMatrix(float matrix[16])
{
	::memcpy(this->matrix, matrix, sizeof(float) * 16);
}

void Matrix4x4::setTranslation(const Vector3D translation)
{
	this->setIdentity();
	this->matrix[3][0] = translation.getX();
	this->matrix[3][1] = translation.getY();
	this->matrix[3][2] = translation.getZ();
}

void Matrix4x4::setScale(const Vector3D scale)
{
	this->setIdentity();
	this->matrix[0][0] = scale.getX();
	this->matrix[1][1] = scale.getY();
	this->matrix[2][2] = scale.getZ();
}

void Matrix4x4::setRotationX(float x)
{
	this->setIdentity();
	this->matrix[1][1] = cos(x);
	this->matrix[1][2] = sin(x);
	this->matrix[2][1] = -sin(x);
	this->matrix[2][2] = cos(x);
}

void Matrix4x4::setRotationY(float y)
{
	this->setIdentity();
	this->matrix[0][0] = cos(y);
	this->matrix[0][2] = -sin(y);
	this->matrix[2][0] = sin(y);
	this->matrix[2][2] = cos(y);
}


void Matrix4x4::setRotationZ(float z)
{
	this->setIdentity();
	this->matrix[0][0] = cos(z);
	this->matrix[0][1] = sin(z);
	this->matrix[1][0] = -sin(z);
	this->matrix[1][1] = cos(z);
}

Vector3D Matrix4x4::getRotation()
{
	float roll = atan2f(this->matrix[1][2], this->matrix[2][2]);
	float c2 = sqrtf(this->matrix[0][0] * this->matrix[0][0] + this->matrix[0][1] * this->matrix[0][1]);
	float s1 = sinf(roll);
	float c1 = cosf(roll);
	float pitch = atan2f(-this->matrix[0][2], c2);
	float yaw = atan2f(s1 * this->matrix[2][0] - c1 * this->matrix[1][0],
		c1 * this->matrix[1][1] - s1 * this->matrix[2][1]);
	return Vector3D(roll, yaw, pitch);
}

void Matrix4x4::setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
{
	this->setIdentity();
	float yscale = 1.0f / tan(fov / 2.0f);
	float xscale = yscale / aspect;
	this->matrix[0][0] = xscale;
	this->matrix[1][1] = yscale;
	this->matrix[2][2] = zfar / (zfar - znear);
	this->matrix[2][3] = 1.0f;
	this->matrix[3][2] = (-znear * zfar) / (zfar - znear);
}

void Matrix4x4::setOrthoLH(float width, float height, float near_plane, float far_plane)
{
	this->setIdentity();
	this->matrix[0][0] = 2.0f / width;
	this->matrix[1][1] = 2.0f / height;
	this->matrix[2][2] = 1.0f / (far_plane - near_plane);
	this->matrix[3][2] = -(near_plane / (far_plane - near_plane));
}

void Matrix4x4::setInverse()
{
	int a, i, j;
	Matrix4x4 out;
	Vector4D v, vec[3];
	float det = 0.0f;

	det = this->determinant();
	if (!det) return;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (j != i)
			{
				a = j;
				if (j > i) a = a - 1;
				vec[a].x = (this->matrix[j][0]);
				vec[a].y = (this->matrix[j][1]);
				vec[a].z = (this->matrix[j][2]);
				vec[a].w = (this->matrix[j][3]);
			}
		}
		v.cross(vec[0], vec[1], vec[2]);

		out.matrix[0][i] = pow(-1.0f, i) * v.x / det;
		out.matrix[1][i] = pow(-1.0f, i) * v.y / det;
		out.matrix[2][i] = pow(-1.0f, i) * v.z / det;
		out.matrix[3][i] = pow(-1.0f, i) * v.w / det;
	}

	this->setMatrix(out);
}

Matrix4x4 Matrix4x4::multiplyTo(Matrix4x4 matrix)
{
	Matrix4x4 out;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out.matrix[i][j] =
				this->matrix[i][0] * matrix.matrix[0][j] + this->matrix[i][1] * matrix.matrix[1][j] +
				this->matrix[i][2] * matrix.matrix[2][j] + this->matrix[i][3] * matrix.matrix[3][j];
		}
	}

	return out;
}

Vector4D Matrix4x4::multiplyTo(Vector4D in)
{
	Vector4D out;
	out.x = in.getX() * (this->matrix[0][0] + this->matrix[0][1] + this->matrix[0][2] + this->matrix[0][3]);
	out.y = in.getY() * (this->matrix[1][0] + this->matrix[1][1] + this->matrix[1][2] + this->matrix[1][3]);
	out.z = in.getZ() * (this->matrix[2][0] + this->matrix[2][1] + this->matrix[2][2] + this->matrix[2][3]);
	out.w = in.getW() * (this->matrix[3][0] + this->matrix[3][1] + this->matrix[3][2] + this->matrix[3][3]);

	/*out.x = in.getX() * (this->matrix[0][0] + this->matrix[1][0] + this->matrix[2][0] + this->matrix[3][0]);
	out.y = in.getY() * (this->matrix[0][1] + this->matrix[1][1] + this->matrix[2][1] + this->matrix[3][1]);
	out.z = in.getZ() * (this->matrix[0][2] + this->matrix[1][2] + this->matrix[2][2] + this->matrix[3][2]);
	out.w = in.getW() * (this->matrix[0][3] + this->matrix[1][3] + this->matrix[2][3] + this->matrix[3][3]);*/

	return out;
}

Vector3D Matrix4x4::getZDirection()
{
	return Vector3D(this->matrix[2][0], this->matrix[2][1], this->matrix[2][2]);
}

Vector3D Matrix4x4::getXDirection()
{
	return Vector3D(this->matrix[0][0], this->matrix[0][1], this->matrix[0][2]);
}

Vector3D Matrix4x4::getTranslation()
{
	return Vector3D(this->matrix[3][0], this->matrix[3][1], this->matrix[3][2]);
}

Vector3D Matrix4x4::getScale()
{
	return Vector3D(this->matrix[0][0], this->matrix[1][1], this->matrix[2][2]);
}

void Matrix4x4::debugPrint()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << this->matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

float* Matrix4x4::getMatrix()
{
	//re-arrange to be compatible with react physics
	/*float matrix4x4[16];
	matrix4x4[0] = matrix[0][0]; matrix4x4[1] = matrix[1][0];
	matrix4x4[2] = matrix[2][0]; matrix4x4[3] = 0.0;
	matrix4x4[4] = matrix[0][1]; matrix4x4[5] = matrix[1][1];
	matrix4x4[6] = matrix[2][1]; matrix4x4[7] = 0.0;
	matrix4x4[8] = matrix[0][2]; matrix4x4[9] = matrix[1][2];
	matrix4x4[10] = matrix[2][2]; matrix4x4[11] = 0.0;
	matrix4x4[12] = matrix[3][0]; matrix4x4[13] = matrix[3][1];
	matrix4x4[14] = matrix[3][2]; matrix4x4[15] = 1.0;

	return matrix4x4;*/

	return *this->matrix;
}

float* Matrix4x4::getMatrix16()
{
	float matrix[16] =
	{ this->matrix[0][0], this->matrix[0][1], this->matrix[0][2], this->matrix[0][3],
	 this->matrix[1][0], this->matrix[1][1], this->matrix[1][2], this->matrix[1][3],
	 this->matrix[2][0], this->matrix[2][1], this->matrix[2][2], this->matrix[2][3],
	 this->matrix[3][0], this->matrix[3][1], this->matrix[3][2], this->matrix[3][3] };
	return matrix;
}

void Matrix4x4::matrixInitialize()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = 0.0f;
		}
	}
}

float Matrix4x4::determinant()
{
	Vector4D minor, v1, v2, v3;
	float det;

	v1 = Vector4D(this->matrix[0][0], this->matrix[1][0], this->matrix[2][0], this->matrix[3][0]);
	v2 = Vector4D(this->matrix[0][1], this->matrix[1][1], this->matrix[2][1], this->matrix[3][1]);
	v3 = Vector4D(this->matrix[0][2], this->matrix[1][2], this->matrix[2][2], this->matrix[3][2]);


	minor.cross(v1, v2, v3);
	det = -(this->matrix[0][3] * minor.getX() + this->matrix[1][3] * minor.getY() + this->matrix[2][3] * minor.getZ() +
		this->matrix[3][3] * minor.getW());
	return det;
}
