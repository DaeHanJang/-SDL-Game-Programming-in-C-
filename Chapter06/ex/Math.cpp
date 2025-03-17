#include "Math.h"

const Vector2 Vector2::Zero(0.f, 0.f);
const Vector2 Vector2::UnitX(1.f, 0.f);
const Vector2 Vector2::UnitY(0.f, 1.f);
const Vector2 Vector2::NegUnitX(-1.f, 0.f);
const Vector2 Vector2::NegUnitY(0.f, -1.f);

const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
const Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
const Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
const Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);
const Vector3 Vector3::NegUnitX(-1.f, 0.f, 0.f);
const Vector3 Vector3::NegUnitY(0.f, -1.f, 0.f);
const Vector3 Vector3::NegUnitZ(0.f, 0.f, -1.f);
const Vector3 Vector3::Infinity(Math::Infinity, Math::Infinity, Math::Infinity);
const Vector3 Vector3::NegInfinity(Math::NegInfinity, Math::NegInfinity, Math::NegInfinity);

static float m3Ident[3][3] = {
	{ 1.f, 0.f, 0.f }, 
	{ 0.f, 1.f, 0.f }, 
	{ 0.f, 0.f, 1.f } 
};
const Matrix3 Matrix3::Identity(m3Ident);

static float m4Ident[4][4] = {
	{ 1.f, 0.f, 0.f, 0.f }, 
	{ 0.f, 1.f, 0.f, 0.f }, 
	{ 0.f, 0.f, 1.f, 0.f }, 
	{ 0.f, 0.f, 0.f, 1.f } 
};
const Matrix4 Matrix4::Identity(m4Ident);

const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);

Vector2 Vector2::Transform(const Vector2& vec, const Matrix3& mat, float w) {
	Vector2 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] + w * mat.mat[2][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] + w * mat.mat[2][1];
	//w을 이용해 새로운 값을 반환하지 않기 때문에 무시
	return retVal;
}

Vector3 Vector3::Transform(const Vector3& vec, const Matrix4& mat, float w) {
	Vector3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	//w을 이용해 새로운 값을 반환하지 않기 때문에 무시
	return retVal;
}
//벡터를 행렬로 변환하고 w 성분으로 정규화
Vector3 Vector3::TransformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w) {
	Vector3 retVal;
	retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
		vec.z * mat.mat[2][0] + w * mat.mat[3][0];
	retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
		vec.z * mat.mat[2][1] + w * mat.mat[3][1];
	retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
		vec.z * mat.mat[2][2] + w * mat.mat[3][2];
	float transformedW = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
		vec.z * mat.mat[2][3] + w * mat.mat[3][3];
	if (!Math::NearZero(Math::Abs(transformedW))) {
		transformedW = 1.f / transformedW;
		retVal *= transformedW;
	}
	return retVal;
}
//3D 벡터를 쿼터니언으로 변환
Vector3 Vector3::Transform(const Vector3& v, const Quaternion& q) {
	// v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3 qv(q.x, q.y, q.z);
	Vector3 retVal = v;
	retVal += 2.f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w * v);
	return retVal;
}

void Matrix4::Invert() {
	float tmp[12];
	float src[16];
	float dst[16];
	float det;

	//전치 행렬
	//1행을 1열로
	src[0] = mat[0][0];
	src[4] = mat[0][1];
	src[8] = mat[0][2];
	src[12] = mat[0][3];

	//2행을 2열로
	src[1] = mat[1][0];
	src[5] = mat[1][1];
	src[9] = mat[1][2];
	src[13] = mat[1][3];

	//3행을 3열로
	src[2] = mat[2][0];
	src[6] = mat[2][1];
	src[10] = mat[2][2];
	src[14] = mat[2][3];

	//4행을 4열로
	src[3] = mat[3][0];
	src[7] = mat[3][1];
	src[11] = mat[3][2];
	src[15] = mat[3][3];

	//여인수 계산
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	//행렬식 계산
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	//역행렬은 행렬식으로 나눔
	det = 1 / det;
	for (int j = 0; j < 16; j++) dst[j] *= det;

	//되돌림
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) mat[i][j] = dst[i * 4 + j];
	}
}

Matrix4 Matrix4::CreateFromQuaternion(const class Quaternion& q) {
	float mat[4][4];

	mat[0][0] = 1.f - 2.f * q.y * q.y - 2.f * q.z * q.z;
	mat[0][1] = 2.f * q.x * q.y + 2.f * q.w * q.z;
	mat[0][2] = 2.f * q.x * q.z - 2.f * q.w * q.y;
	mat[0][3] = 0.f;

	mat[1][0] = 2.f * q.x * q.y - 2.f * q.w * q.z;
	mat[1][1] = 1.f - 2.f * q.x * q.x - 2.f * q.z * q.z;
	mat[1][2] = 2.f * q.y * q.z + 2.f * q.w * q.x;
	mat[1][3] = 0.f;

	mat[2][0] = 2.f * q.x * q.z + 2.f * q.w * q.y;
	mat[2][1] = 2.f * q.y * q.z - 2.f * q.w * q.x;
	mat[2][2] = 1.f - 2.f * q.x * q.x - 2.f * q.y * q.y;
	mat[2][3] = 0.f;

	mat[3][0] = 0.f;
	mat[3][1] = 0.f;
	mat[3][2] = 0.f;
	mat[3][3] = 1.f;

	return Matrix4(mat);
}
