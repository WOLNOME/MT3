#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <algorithm>
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2A_02_ウシオユウキ_MT3_2-4_確認課題";

//定数
const int  kWindowWidth = 1280;
const int kWindowHeight = 720;

//構造体
//球
struct Sphere {
	Vector3 center;
	float radius;
};

//直線
struct Line {
	Vector3 origin;//始点
	Vector3 diff;//終点への差分ベクトル
};
//半直線
struct Ray {
	Vector3 origin;//始点
	Vector3 diff;//終点への差分ベクトル
};
//線分
struct Segment {
	Vector3 origin;//始点
	Vector3 diff;//終点への差分ベクトル
};
//平面
struct Plane {
	Vector3 normal;//法線
	float distance;//距離
};
//三角形
struct Triangle {
	Vector3 vertices[3];
};
//AABB
struct AABB {
	Vector3 min;
	Vector3 max;
};
//OBB
struct OBB {
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};



//関数

//ベクトルの加法
Vector3 Add(const Vector3& v1, const Vector3& v2);
//ベクトルの減法
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
//ベクトルの積
Vector3 Multiply(const float& s, const Vector3& v);
//ベクトルの内積
float Dot(const Vector3& v1, const Vector3& v2);


//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
//行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//単位行列
Matrix4x4 MakeIdentity4x4();

//数値表示
const int kColumnWidth = 60;
const int kRowHeight = 20;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
//座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
//y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
//z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

//3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//レンダリングパイプライン
// 余接
float Cot(float rad);
//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//ビューポート変換行列
Matrix4x4 makeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

//グリッド描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

//球描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
//最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
//ベクトルの長さ
float Length(const Vector3& v);
//球同士の当たり判定
bool isCollision(const Sphere& s1, const Sphere& s2);
//ベクトルの正規化
Vector3 Normalize(const Vector3& v);
//平面の法線から矩形を構成する4頂点をもとめる関数
Vector3 Perpendicular(const Vector3& vector);
//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//平面と球の当たり判定
bool isCollision(const Plane& plane, const Sphere& sphere);
//直線と平面の当たり判定
bool isCollision(const Line& segment, const Plane& plane);
//半直線と平面の当たり判定
bool isCollision(const Ray& segment, const Plane& plane);
//線分と平面の当たり判定
bool isCollision(const Segment& segment, const Plane& plane);
//線分の描画
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//線分と三角形の当たり判定
bool isCollision(const Segment& segment, const Triangle& triangle);
//三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//AABB同士の当たり判定
bool isCollision(const AABB& a, const AABB& b);
//AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
//AABBと球の中心の最近接点
Vector3 ClosestPoint(const AABB& aabb, const Sphere& sphere);
//AABBと球の当たり判定
bool isCollision(const AABB& aabb, const Sphere& sphere);
//平面と直線の衝突点
Vector3 CollisionPoint(const Line& l, const Plane& p);
//平面と直線の衝突点
Vector3 CollisionPoint(const Ray& r, const Plane& p);
//平面と線分の衝突点
Vector3 CollisionPoint(const Segment& s, const Plane& p);
//AABBと直線の当たり判定
bool isCollision(const AABB& aabb, const Line& line);
//AABBと半直線の当たり判定
bool isCollision(const AABB& aabb, const Ray& ray);
//AABBと線分の当たり判定
bool isCollision(const AABB& aabb, const Segment& segment);
//OBBと球の衝突判定
bool isCollision(OBB obb, const Vector3& rotate, const Sphere& sphere);
//OBBの描画
void DrawOBB(OBB obb, const Vector3& rotate, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//初期化
	Vector3 rotate{ 0.0f,0.0f,0.0f };
	//obb
	OBB obb{
		.center{-1.0f,0.0f,0.0f},
		.orientations = {{1.0f,0.0f,0.0f},
						 {0.0f,1.0f,0.0f},
						 {0.0f,0.0f,1.0f}},
		.size{0.5f,0.5f,0.5f}
	};
	uint32_t color1 = 0xffffffff;
	//球
	Sphere sphere{
		.center{0.0f,0.0f,0.0f},
		.radius{0.5f}
	};
	uint32_t color2 = 0xffffffff;

	//カメラの座標と角度
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		///
		/// ↓更新処理ここから
		///

		//カメラの移動

		//当たり判定処理
		if (isCollision(obb, rotate, sphere)) {
			color1 = 0xff0000ff;
		}
		else {
			color1 = 0xffffffff;
		}

		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = makeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//OBBの描画
		DrawOBB(obb, rotate, worldViewProjectionMatrix, viewPortMatrix, color1);
		//球の描画
		DrawSphere(sphere, worldViewProjectionMatrix, viewPortMatrix, color2);

		//グリッド
		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);


		//imgui
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat("rotateX", &rotate.x, 0.01f);
		ImGui::DragFloat("rotateY", &rotate.y, 0.01f);
		ImGui::DragFloat("rotateZ", &rotate.z, 0.01f);
		ImGui::DragFloat3("obb.orientations[0]", &obb.orientations[0].x, 0.01f);
		ImGui::DragFloat3("obb.orientations[1]", &obb.orientations[1].x, 0.01f);
		ImGui::DragFloat3("obb.orientations[2]", &obb.orientations[2].x, 0.01f);
		ImGui::DragFloat3("obb.size", &obb.size.x, 0.01f);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 c;
	c = {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
	return c;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 c;
	c = {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	};
	return c;
}

Vector3 Multiply(const float& s, const Vector3& v)
{
	Vector3 c;
	c.x = s * v.x;
	c.y = s * v.y;
	c.z = s * v.z;
	return c;
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	float c;
	c = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return c;
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 c;
	c.m[0][0] = m1.m[0][0] + m2.m[0][0];
	c.m[0][1] = m1.m[0][1] + m2.m[0][1];
	c.m[0][2] = m1.m[0][2] + m2.m[0][2];
	c.m[0][3] = m1.m[0][3] + m2.m[0][3];
	c.m[1][0] = m1.m[1][0] + m2.m[1][0];
	c.m[1][1] = m1.m[1][1] + m2.m[1][1];
	c.m[1][2] = m1.m[1][2] + m2.m[1][2];
	c.m[1][3] = m1.m[1][3] + m2.m[1][3];
	c.m[2][0] = m1.m[2][0] + m2.m[2][0];
	c.m[2][1] = m1.m[2][1] + m2.m[2][1];
	c.m[2][2] = m1.m[2][2] + m2.m[2][2];
	c.m[2][3] = m1.m[2][3] + m2.m[2][3];
	c.m[3][0] = m1.m[3][0] + m2.m[3][0];
	c.m[3][1] = m1.m[3][1] + m2.m[3][1];
	c.m[3][2] = m1.m[3][2] + m2.m[3][2];
	c.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return c;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 c;
	c.m[0][0] = m1.m[0][0] - m2.m[0][0];
	c.m[0][1] = m1.m[0][1] - m2.m[0][1];
	c.m[0][2] = m1.m[0][2] - m2.m[0][2];
	c.m[0][3] = m1.m[0][3] - m2.m[0][3];
	c.m[1][0] = m1.m[1][0] - m2.m[1][0];
	c.m[1][1] = m1.m[1][1] - m2.m[1][1];
	c.m[1][2] = m1.m[1][2] - m2.m[1][2];
	c.m[1][3] = m1.m[1][3] - m2.m[1][3];
	c.m[2][0] = m1.m[2][0] - m2.m[2][0];
	c.m[2][1] = m1.m[2][1] - m2.m[2][1];
	c.m[2][2] = m1.m[2][2] - m2.m[2][2];
	c.m[2][3] = m1.m[2][3] - m2.m[2][3];
	c.m[3][0] = m1.m[3][0] - m2.m[3][0];
	c.m[3][1] = m1.m[3][1] - m2.m[3][1];
	c.m[3][2] = m1.m[3][2] - m2.m[3][2];
	c.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return c;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 c;
	c.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	c.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	c.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	c.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	c.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	c.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	c.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	c.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	c.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	c.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	c.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	c.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	c.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	c.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	c.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	c.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return c;
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 c;
	float A;
	A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	c.m[0][0] = (1.0f / A) * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	c.m[0][1] = (1.0f / A) * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	c.m[0][2] = (1.0f / A) * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	c.m[0][3] = (1.0f / A) * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);
	c.m[1][0] = (1.0f / A) * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	c.m[1][1] = (1.0f / A) * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	c.m[1][2] = (1.0f / A) * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	c.m[1][3] = (1.0f / A) * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);
	c.m[2][0] = (1.0f / A) * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	c.m[2][1] = (1.0f / A) * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	c.m[2][2] = (1.0f / A) * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	c.m[2][3] = (1.0f / A) * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);
	c.m[3][0] = (1.0f / A) * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	c.m[3][1] = (1.0f / A) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	c.m[3][2] = (1.0f / A) * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	c.m[3][3] = (1.0f / A) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return c;
}

Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 c;
	c.m[0][0] = m.m[0][0];
	c.m[0][1] = m.m[1][0];
	c.m[0][2] = m.m[2][0];
	c.m[0][3] = m.m[3][0];
	c.m[1][0] = m.m[0][1];
	c.m[1][1] = m.m[1][1];
	c.m[1][2] = m.m[2][1];
	c.m[1][3] = m.m[3][1];
	c.m[2][0] = m.m[0][2];
	c.m[2][1] = m.m[1][2];
	c.m[2][2] = m.m[2][2];
	c.m[2][3] = m.m[3][2];
	c.m[3][0] = m.m[0][3];
	c.m[3][1] = m.m[1][3];
	c.m[3][2] = m.m[2][3];
	c.m[3][3] = m.m[3][3];
	return c;
}

Matrix4x4 MakeIdentity4x4()
{
	Matrix4x4 c;
	c.m[0][0] = 1.0f;
	c.m[0][1] = 0.0f;
	c.m[0][2] = 0.0f;
	c.m[0][3] = 0.0f;
	c.m[1][0] = 0.0f;
	c.m[1][1] = 1.0f;
	c.m[1][2] = 0.0f;
	c.m[1][3] = 0.0f;
	c.m[2][0] = 0.0f;
	c.m[2][1] = 0.0f;
	c.m[2][2] = 1.0f;
	c.m[2][3] = 0.0f;
	c.m[3][0] = 0.0f;
	c.m[3][1] = 0.0f;
	c.m[3][2] = 0.0f;
	c.m[3][3] = 1.0f;
	return c;
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name)
{
	Novice::ScreenPrintf(x, y, "%s", name);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 c;
	c.m[0][0] = 1;
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = 1;
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = 1;
	c.m[2][3] = 0;
	c.m[3][0] = translate.x;
	c.m[3][1] = translate.y;
	c.m[3][2] = translate.z;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 c;
	c.m[0][0] = scale.x;
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = scale.y;
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = scale.z;
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 c;
	c.m[0][0] = 1;
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = std::cos(radian);
	c.m[1][2] = std::sin(radian);
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = -std::sin(radian);
	c.m[2][2] = std::cos(radian);
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 c;
	c.m[0][0] = std::cos(radian);
	c.m[0][1] = 0;
	c.m[0][2] = -std::sin(radian);
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = 1;
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = std::sin(radian);
	c.m[2][1] = 0;
	c.m[2][2] = std::cos(radian);
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 c;
	c.m[0][0] = std::cos(radian);
	c.m[0][1] = std::sin(radian);
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = -std::sin(radian);
	c.m[1][1] = std::cos(radian);
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = 1;
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 c;
	//回転行列統合
	Matrix4x4 rx = MakeRotateXMatrix(rotate.x);
	Matrix4x4 ry = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rz = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rxyz = Multiply(rx, Multiply(ry, rz));

	c.m[0][0] = scale.x * rxyz.m[0][0];
	c.m[0][1] = scale.x * rxyz.m[0][1];
	c.m[0][2] = scale.x * rxyz.m[0][2];
	c.m[0][3] = 0;
	c.m[1][0] = scale.y * rxyz.m[1][0];
	c.m[1][1] = scale.y * rxyz.m[1][1];
	c.m[1][2] = scale.y * rxyz.m[1][2];
	c.m[1][3] = 0;
	c.m[2][0] = scale.z * rxyz.m[2][0];
	c.m[2][1] = scale.z * rxyz.m[2][1];
	c.m[2][2] = scale.z * rxyz.m[2][2];
	c.m[2][3] = 0;
	c.m[3][0] = translate.x;
	c.m[3][1] = translate.y;
	c.m[3][2] = translate.z;
	c.m[3][3] = 1;

	return c;
}

float Cot(float rad)
{
	float c;

	c = 1 / std::tan(rad);

	return c;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 c;
	c.m[0][0] = (1 / aspectRatio) * Cot(fovY / 2);
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = Cot(fovY / 2);
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = farClip / (farClip - nearClip);
	c.m[2][3] = 1;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	c.m[3][3] = 0;
	return c;

}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 c;
	c.m[0][0] = 2 / (right - left);
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = 2 / (top - bottom);
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = 1 / (farClip - nearClip);
	c.m[2][3] = 0;
	c.m[3][0] = (left + right) / (left - right);
	c.m[3][1] = (top + bottom) / (bottom - top);
	c.m[3][2] = nearClip / (nearClip - farClip);
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 makeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 c;
	c.m[0][0] = width / 2;
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = -height / 2;
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = maxDepth - minDepth;
	c.m[2][3] = 0;
	c.m[3][0] = left + (width / 2);
	c.m[3][1] = top + (height / 2);
	c.m[3][2] = minDepth;
	c.m[3][3] = 1;
	return c;
}

Vector3 Cross(const Vector3& a, const Vector3& b)
{
	Vector3 c;

	c.x = (a.y * b.z) - (a.z * b.y);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);

	return c;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const Vector3 kOrigin = { 0.0f,0.0f,0.0f };//原点
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//1つ分の長さ
	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//ローカル座標を求める
		Vector3 kLocalVertices[2] = {
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f}
		};
		kLocalVertices[0].x = kOrigin.x - kGridHalfWidth + (xIndex * kGridEvery);
		kLocalVertices[0].z = kOrigin.z - kGridHalfWidth;
		kLocalVertices[1].x = kOrigin.x - kGridHalfWidth + (xIndex * kGridEvery);
		kLocalVertices[1].z = kOrigin.z + kGridHalfWidth;


		Vector3 screenVertices[2];
		for (uint32_t i = 0; i < 2; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		//描画
		Novice::DrawLine(
			(int)screenVertices[0].x, (int)screenVertices[0].y,
			(int)screenVertices[1].x, (int)screenVertices[1].y,
			0xaaaaaaff
		);
	}
	//左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//ローカル座標を求める
		Vector3 kLocalVertices[2] = {
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f}
		};
		kLocalVertices[0].x = kOrigin.x - kGridHalfWidth;
		kLocalVertices[0].z = kOrigin.z - kGridHalfWidth + (zIndex * kGridEvery);
		kLocalVertices[1].x = kOrigin.x + kGridHalfWidth;
		kLocalVertices[1].z = kOrigin.z - kGridHalfWidth + (zIndex * kGridEvery);


		Vector3 screenVertices[2];
		for (uint32_t i = 0; i < 2; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		//描画
		Novice::DrawLine(
			(int)screenVertices[0].x, (int)screenVertices[0].y,
			(int)screenVertices[1].x, (int)screenVertices[1].y,
			0xaaaaaaff
		);
	}

}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	float pi = (float)M_PI;
	const uint32_t kSubdivision = 20;//分割数
	const float kLonEvery = 2.0f * pi / kSubdivision;//経度分割1つ分の角度
	const float kLatEvery = pi / kSubdivision;//緯度分割1つ分の角度
	//緯度の方向に分割 -π/2~π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;//現在の経度
		//経度の方向に分割 -π/2~π/2
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の緯度
			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = {
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon)
			};
			b = {
				sphere.center.x + sphere.radius * cosf(kLatEvery + lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(kLatEvery + lat),
				sphere.center.z + sphere.radius * cosf(kLatEvery + lat) * sinf(lon)
			};
			c = {
				sphere.center.x + sphere.radius * cosf(lat) * cosf(kLonEvery + lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(kLonEvery + lon)
			};

			Vector3 screenVerticesA, screenVerticesB, screenVerticesC;
			Vector3 ndcVertexA = Transform(a, viewProjectionMatrix);
			Vector3 ndcVertexB = Transform(b, viewProjectionMatrix);
			Vector3 ndcVertexC = Transform(c, viewProjectionMatrix);
			screenVerticesA = Transform(ndcVertexA, viewportMatrix);
			screenVerticesB = Transform(ndcVertexB, viewportMatrix);
			screenVerticesC = Transform(ndcVertexC, viewportMatrix);

			//描画
			Novice::DrawLine(
				(int)screenVerticesA.x, (int)screenVerticesA.y,
				(int)screenVerticesB.x, (int)screenVerticesB.y,
				color
			);
			Novice::DrawLine(
				(int)screenVerticesA.x, (int)screenVerticesA.y,
				(int)screenVerticesC.x, (int)screenVerticesC.y,
				color
			);
		}
	}
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	Vector3 c;
	float n = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	float l = powf(sqrtf(powf(v2.x, 2) + powf(v2.y, 2) + powf(v2.z, 2)), 2);
	c.x = n / l * v2.x;
	c.y = n / l * v2.y;
	c.z = n / l * v2.z;
	return c;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 cp;
	Vector3 a;
	Vector3 proj;
	a = Subtract(point, segment.origin);
	proj = Project(a, segment.diff);
	cp = {
		segment.origin.x + proj.x,
		segment.origin.y + proj.y,
		segment.origin.z + proj.z
	};
	return cp;
}

float Length(const Vector3& v)
{
	float c;
	c = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
	return c;
}

bool isCollision(const Sphere& s1, const Sphere& s2)
{
	//2つの球の中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}
	else {
		return false;
	}
}

Vector3 Normalize(const Vector3& v)
{
	Vector3 c;
	//長さを求める
	float length = Length(v);
	//length=0で無ければ正規化
	if (length != 0) {
		c.x = v.x / length;
		c.y = v.y / length;
		c.z = v.z / length;
	}
	else {
		assert("正規化できません");
	}
	return c;
}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	//法線がz成分のみなら
	return{ 0.0f,-vector.z,vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//平面の中心を求める
	Vector3 center = Multiply(plane.distance, plane.normal);
	//平面の頂点を求める
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));//法線と垂直なベクトル
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };//0の逆ベクトル
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);//0と法線のクロス積
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };//2の逆ベクトル
	//以上のベクトルを中心点に定数倍して足すと4頂点が完成
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[1].x, (int)points[1].y, color);
	Novice::DrawLine((int)points[3].x, (int)points[3].y, (int)points[0].x, (int)points[0].y, color);
}

bool isCollision(const Plane& plane, const Sphere& sphere)
{
	//球の中心と平面との距離を計算
	float distance;
	float k;
	k = sqrtf(powf(Dot(plane.normal, sphere.center) - plane.distance, 2));
	Vector3 q;//球の中心から平面に垂直に線を下したときに交わる点
	q = Subtract(sphere.center, Multiply(k, plane.normal));
	distance = Length(Subtract(sphere.center, q));
	//衝突判定
	if (distance <= sphere.radius) {
		return true;
	}
	else {
		return false;
	}
}

bool isCollision(const Line& line, const Plane& plane)
{
	//垂直判定を求める
	float dot = Dot(plane.normal, line.diff);
	//平行の場合衝突していない。
	if (dot == 0.0f) {
		return false;
	}
	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	//直線の当たり判定
	if (t >= 0 || t < 0) {
		return true;
	}
	else {
		return false;
	}
}

bool isCollision(const Ray& ray, const Plane& plane)
{
	//垂直判定を求める
	float dot = Dot(plane.normal, ray.diff);
	//平行の場合衝突していない。
	if (dot == 0.0f) {
		return false;
	}
	//tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
	//半直線の当たり判定
	if (t >= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool isCollision(const Segment& segment, const Plane& plane)
{
	//平行判定
	float dot = Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	//媒介変数
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	//当たり判定結果
	if (t >= 0 && t <= 1) {
		return true;
	}
	else {
		return false;
	}
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

bool isCollision(const Segment& segment, const Triangle& triangle)
{
	//三角形のある面を作る
	Plane plane;
	//三角形の座標から、法線nを求める
	Vector3 vv1, vv2, n;
	vv1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	vv2 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	n = Normalize(Cross(vv1, vv2));
	//ディスタンスを求める
	float d = Dot(triangle.vertices[0], n);
	//面に変換
	plane.normal = n;
	plane.distance = d;

	//面と線の当たり判定
	//接触点Pを求める
	float dot = Dot(segment.diff, plane.normal);
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
	//当たり判定
	if (isCollision(segment, plane)) {
		//必要なベクトル
		Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
		Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
		Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
		Vector3 v0p = Subtract(p, triangle.vertices[0]);
		Vector3 v1p = Subtract(p, triangle.vertices[1]);
		Vector3 v2p = Subtract(p, triangle.vertices[2]);
		//各辺を結んだベクトルと、頂点の衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);
		//全ての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			//衝突
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//三角形の各頂点
	Vector3 triangleVertex[3];
	triangleVertex[0] = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	triangleVertex[1] = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	triangleVertex[2] = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);
	//描画
	Novice::DrawTriangle((int)triangleVertex[0].x, (int)triangleVertex[0].y, (int)triangleVertex[1].x, (int)triangleVertex[1].y, (int)triangleVertex[2].x, (int)triangleVertex[2].y, color, kFillModeWireFrame);


}

bool isCollision(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		//衝突
		return true;
	}
	else {
		return false;
	}
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//立方体の各頂点
	Vector3 CubeVertex[8];
	CubeVertex[0] = Transform(Transform({ aabb.min.x,aabb.min.y,aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[1] = Transform(Transform({ aabb.min.x,aabb.max.y,aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[2] = Transform(Transform({ aabb.min.x,aabb.min.y,aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[3] = Transform(Transform({ aabb.min.x,aabb.max.y,aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[4] = Transform(Transform({ aabb.max.x,aabb.min.y,aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[5] = Transform(Transform({ aabb.max.x,aabb.max.y,aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[6] = Transform(Transform({ aabb.max.x,aabb.min.y,aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	CubeVertex[7] = Transform(Transform({ aabb.max.x,aabb.max.y,aabb.max.z }, viewProjectionMatrix), viewportMatrix);

	//描画
	//x軸方向
	for (int i = 0; i <= 3; i++) {
		Novice::DrawLine((int)CubeVertex[i].x, (int)CubeVertex[i].y, (int)CubeVertex[i + 4].x, (int)CubeVertex[i + 4].y, color);
	}

	//y軸方向
	for (int i = 0; i <= 6; i += 2) {
		Novice::DrawLine((int)CubeVertex[i].x, (int)CubeVertex[i].y, (int)CubeVertex[i + 1].x, (int)CubeVertex[i + 1].y, color);
	}

	//z軸方向
	for (int i = 0; i <= 1; i++) {
		Novice::DrawLine((int)CubeVertex[i].x, (int)CubeVertex[i].y, (int)CubeVertex[i + 2].x, (int)CubeVertex[i + 2].y, color);
	}
	for (int i = 4; i <= 5; i++) {
		Novice::DrawLine((int)CubeVertex[i].x, (int)CubeVertex[i].y, (int)CubeVertex[i + 2].x, (int)CubeVertex[i + 2].y, color);
	}

}

Vector3 ClosestPoint(const AABB& aabb, const Sphere& sphere)
{
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};
	return closestPoint;
}

bool isCollision(const AABB& aabb, const Sphere& sphere)
{
	//球の中心とAABBとの最近接点を求める
	Vector3 closestPoint = ClosestPoint(aabb, sphere);
	//最近接点と球の中心の距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	//距離が平均よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	else {
		return false;
	}
}

Vector3 CollisionPoint(const Line& l, const Plane& p)
{
	///衝突している前提の関数
	Vector3 result;
	float dot = Dot(l.diff, p.normal);
	//媒介変数
	float t = (p.distance - Dot(l.origin, p.normal)) / dot;
	result = Add(l.origin, Multiply(t, l.diff));
	return result;
}

Vector3 CollisionPoint(const Ray& r, const Plane& p)
{
	///衝突している前提の関数
	Vector3 result;
	float dot = Dot(r.diff, p.normal);
	//媒介変数
	float t = (p.distance - Dot(r.origin, p.normal)) / dot;
	result = Add(r.origin, Multiply(t, r.diff));
	return result;
}

Vector3 CollisionPoint(const Segment& s, const Plane& p)
{
	///衝突している前提の関数
	Vector3 result;
	float dot = Dot(s.diff, p.normal);
	//媒介変数
	float t = (p.distance - Dot(s.origin, p.normal)) / dot;
	result = Add(s.origin, Multiply(t, s.diff));
	return result;
}

bool isCollision(const AABB& aabb, const Line& line)
{
	//segmentの成分が全て0(点)の場合エラー
	if (line.diff.x == 0 && line.diff.y == 0 && line.diff.z == 0) {
		assert("線の成分が全て0");
	}

	//6つの平面を構造体に入れる
	Plane pxmin;
	Plane pxmax;
	Plane pymin;
	Plane pymax;
	Plane pzmin;
	Plane pzmax;
	//法線の値を入力
	pxmin.normal = { 1,0,0 };
	pxmax.normal = { 1,0,0 };
	pymin.normal = { 0,1,0 };
	pymax.normal = { 0,1,0 };
	pzmin.normal = { 0,0,1 };
	pzmax.normal = { 0,0,1 };
	//距離の値を入力
	pxmin.distance = Dot(aabb.min, pxmin.normal);
	pymin.distance = Dot(aabb.min, pymin.normal);
	pzmin.distance = Dot(aabb.min, pzmin.normal);
	pxmax.distance = Dot(aabb.max, pxmax.normal);
	pymax.distance = Dot(aabb.max, pymax.normal);
	pzmax.distance = Dot(aabb.max, pzmax.normal);
	//それぞれの平面と線分の衝突点を求める
	if (isCollision(line, pxmin)) {
		Vector3 cp = CollisionPoint(line, pxmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(line, pxmax)) {
		Vector3 cp = CollisionPoint(line, pxmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(line, pymin)) {
		Vector3 cp = CollisionPoint(line, pymin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(line, pymax)) {
		Vector3 cp = CollisionPoint(line, pymax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(line, pzmin)) {
		Vector3 cp = CollisionPoint(line, pzmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(line, pzmax)) {
		Vector3 cp = CollisionPoint(line, pzmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	//これ以上衝突条件は無い
	return false;

}

bool isCollision(const AABB& aabb, const Ray& ray)
{
	//segmentの成分が全て0(点)の場合エラー
	if (ray.diff.x == 0 && ray.diff.y == 0 && ray.diff.z == 0) {
		assert("線の成分が全て0");
	}

	//6つの平面を構造体に入れる
	Plane pxmin;
	Plane pxmax;
	Plane pymin;
	Plane pymax;
	Plane pzmin;
	Plane pzmax;
	//法線の値を入力
	pxmin.normal = { 1,0,0 };
	pxmax.normal = { 1,0,0 };
	pymin.normal = { 0,1,0 };
	pymax.normal = { 0,1,0 };
	pzmin.normal = { 0,0,1 };
	pzmax.normal = { 0,0,1 };
	//距離の値を入力
	pxmin.distance = Dot(aabb.min, pxmin.normal);
	pymin.distance = Dot(aabb.min, pymin.normal);
	pzmin.distance = Dot(aabb.min, pzmin.normal);
	pxmax.distance = Dot(aabb.max, pxmax.normal);
	pymax.distance = Dot(aabb.max, pymax.normal);
	pzmax.distance = Dot(aabb.max, pzmax.normal);
	//それぞれの平面と線分の衝突点を求める
	if (isCollision(ray, pxmin)) {
		Vector3 cp = CollisionPoint(ray, pxmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(ray, pxmax)) {
		Vector3 cp = CollisionPoint(ray, pxmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(ray, pymin)) {
		Vector3 cp = CollisionPoint(ray, pymin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(ray, pymax)) {
		Vector3 cp = CollisionPoint(ray, pymax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(ray, pzmin)) {
		Vector3 cp = CollisionPoint(ray, pzmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(ray, pzmax)) {
		Vector3 cp = CollisionPoint(ray, pzmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	//これ以上衝突条件は無い
	return false;
}

bool isCollision(const AABB& aabb, const Segment& segment)
{
	//segmentの成分が全て0(点)の場合エラー
	if (segment.diff.x == 0 && segment.diff.y == 0 && segment.diff.z == 0) {
		assert("線の成分が全て0");
	}

	//6つの平面を構造体に入れる
	Plane pxmin;
	Plane pxmax;
	Plane pymin;
	Plane pymax;
	Plane pzmin;
	Plane pzmax;
	//法線の値を入力
	pxmin.normal = { 1,0,0 };
	pxmax.normal = { 1,0,0 };
	pymin.normal = { 0,1,0 };
	pymax.normal = { 0,1,0 };
	pzmin.normal = { 0,0,1 };
	pzmax.normal = { 0,0,1 };
	//距離の値を入力
	pxmin.distance = Dot(aabb.min, pxmin.normal);
	pymin.distance = Dot(aabb.min, pymin.normal);
	pzmin.distance = Dot(aabb.min, pzmin.normal);
	pxmax.distance = Dot(aabb.max, pxmax.normal);
	pymax.distance = Dot(aabb.max, pymax.normal);
	pzmax.distance = Dot(aabb.max, pzmax.normal);
	//それぞれの平面と線分の衝突点を求める
	if (isCollision(segment, pxmin)) {
		Vector3 cp = CollisionPoint(segment, pxmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(segment, pxmax)) {
		Vector3 cp = CollisionPoint(segment, pxmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(segment, pymin)) {
		Vector3 cp = CollisionPoint(segment, pymin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(segment, pymax)) {
		Vector3 cp = CollisionPoint(segment, pymax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(segment, pzmin)) {
		Vector3 cp = CollisionPoint(segment, pzmin);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	if (isCollision(segment, pzmax)) {
		Vector3 cp = CollisionPoint(segment, pzmax);
		if (
			cp.x >= aabb.min.x && cp.x <= aabb.max.x &&
			cp.y >= aabb.min.y && cp.y <= aabb.max.y &&
			cp.z >= aabb.min.z && cp.z <= aabb.max.z
			) {
			return true;
		}
	}
	//線分がaabb内にある場合→始点と終点がそれぞれmin max内
	Vector3 ep = Add(segment.origin, segment.diff);
	if (
		segment.origin.x >= aabb.min.x && segment.origin.x <= aabb.max.x &&
		segment.origin.y >= aabb.min.y && segment.origin.y <= aabb.max.y &&
		segment.origin.z >= aabb.min.z && segment.origin.z <= aabb.max.z &&
		ep.x >= aabb.min.x && ep.x <= aabb.max.x &&
		ep.y >= aabb.min.y && ep.y <= aabb.max.y &&
		ep.z >= aabb.min.z && ep.z <= aabb.max.z
		) {
		return true;
	}

	//これ以上衝突条件は無い
	return false;


}

bool isCollision(OBB obb, const Vector3& rotate, const Sphere& sphere)
{
	//obbのワールドマトリックスを作る
	Matrix4x4 obbWorldMatrix = MakeIdentity4x4();

	//回転行列を生成
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	//回転行列から軸を抽出
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];
	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];
	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	//回転成分を与える
	obbWorldMatrix.m[0][0] = obb.orientations[0].x;
	obbWorldMatrix.m[0][1] = obb.orientations[1].x;
	obbWorldMatrix.m[0][2] = obb.orientations[2].x;
	obbWorldMatrix.m[1][0] = obb.orientations[0].y;
	obbWorldMatrix.m[1][1] = obb.orientations[1].y;
	obbWorldMatrix.m[1][2] = obb.orientations[2].y;
	obbWorldMatrix.m[2][0] = obb.orientations[0].z;
	obbWorldMatrix.m[2][1] = obb.orientations[1].z;
	obbWorldMatrix.m[2][2] = obb.orientations[2].z;
	//平行移動成分を与える
	obbWorldMatrix.m[3][0] = obb.center.x;
	obbWorldMatrix.m[3][1] = obb.center.y;
	obbWorldMatrix.m[3][2] = obb.center.z;
	//OBBのローカル空間座標を取得
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, Inverse(obbWorldMatrix));
	//OBBを基にAABBを作成
	AABB aabbOBBLocal{ .min = Multiply(-1.0f,obb.size),.max = obb.size };
	//球体もOBBのローカル空間で作る
	Sphere sphereOBBLocal{ centerInOBBLocalSpace,sphere.radius };
	//ローカル空間で当たり判定
	if (isCollision(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}
	else {
		return false;
	}

}

void DrawOBB(OBB obb, const Vector3& rotate, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//obbのワールドマトリックスを作る
	Matrix4x4 obbWorldMatrix = MakeIdentity4x4();

	//回転行列を生成
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	//回転行列から軸を抽出
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];
	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];
	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	//回転成分を与える
	obbWorldMatrix.m[0][0] = obb.orientations[0].x;
	obbWorldMatrix.m[0][1] = obb.orientations[1].x;
	obbWorldMatrix.m[0][2] = obb.orientations[2].x;
	obbWorldMatrix.m[1][0] = obb.orientations[0].y;
	obbWorldMatrix.m[1][1] = obb.orientations[1].y;
	obbWorldMatrix.m[1][2] = obb.orientations[2].y;
	obbWorldMatrix.m[2][0] = obb.orientations[0].z;
	obbWorldMatrix.m[2][1] = obb.orientations[1].z;
	obbWorldMatrix.m[2][2] = obb.orientations[2].z;
	//平行移動成分を与える
	obbWorldMatrix.m[3][0] = obb.center.x;
	obbWorldMatrix.m[3][1] = obb.center.y;
	obbWorldMatrix.m[3][2] = obb.center.z;
	//OBBを基にAABBを作成
	AABB aabbOBBLocal{ .min = Multiply(-1.0f,obb.size),.max = obb.size };
	//AABBをワールド座標に変換する
	AABB aabbOBBworld{ .min = Transform(aabbOBBLocal.min,obbWorldMatrix),.max = Transform(aabbOBBLocal.min,obbWorldMatrix) };
	//AABBの描画
	DrawAABB(aabbOBBworld, viewProjectionMatrix, viewportMatrix, color);
}


