#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include <imgui.h>
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2A_02_ウシオユウキ_MT3_1-1_確認課題";

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

//関数

//ベクトルの加法
Vector3 Add(const Vector3& v1, const Vector3& v2);
//ベクトルの減法
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
//ベクトルの積
float Multiply(const Vector3& v1, const Vector3& v2);

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
Matrix4x4 MakePerspectiveForMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//初期化
	Segment segment = {
		{-2.0f,-1.0f,0.0f},
		{3.0f,2.0f,2.0f}
	};
	Vector3 point = { -1.5f,0.6f,0.6f };

	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);



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



		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveForMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = makeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//点
		Sphere pointSphere{ point,0.01f };
		Sphere closestPointSphere{ closestPoint,0.01f };
		DrawSphere(pointSphere, worldViewProjectionMatrix, viewPortMatrix, RED);
		DrawSphere(closestPointSphere, worldViewProjectionMatrix, viewPortMatrix, BLACK);

		//線分
		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewPortMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		//グリッド
		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);


		//imgui
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::InputFloat3("Point", &point.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
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

float Multiply(const Vector3& v1, const Vector3& v2)
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

Matrix4x4 MakePerspectiveForMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
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
