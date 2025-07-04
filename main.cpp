#include <Novice.h>
#include <cmath>
#include <numbers>

const char kWindowTitle[] = "GC1D 01 イイヅカ　ソラ";

//ベクトルの長さ
const float kVectorMax = 2.0f;//最大値
const float kVectorMin = 0.5f;//最低値

//2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
	Vector2 operator+(const Vector2& v) {
		Vector2 result{
			x + v.x,
			y + v.y,
		};
		return result;
	}

	Vector2 operator*(float num) {
		Vector2 result{
			x * num,
			y * num,
		};
		return result;
	}

	Vector2& operator/=(float num) {
		this->x /= num;
		this->y /= num;
		return *this;
	}
}Vector2;

//float*Vector2
Vector2 operator*(float num, const Vector2& v) {
	Vector2 result{
		v.x * num,
		v.y * num,
	};
	return result;
}

enum TriangleVertex {
	kLeft,
	kTop,
	kRight,
	kTriangleVertexCount,
};

//回転するオブジェクト
typedef struct RotateObject {
	Vector2 center; // 中心座標
	Vector2 rotateObjectCenter; // 回転オブジェクトの中心
	Vector2 rotateVector; //回転ベクトル
	Vector2 localPosition[static_cast<int>(kTriangleVertexCount)]; // 画面上の位置
	Vector2 screenPosition[static_cast<int>(kTriangleVertexCount)]; // 画面上の位置
}RotateObject;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//回転
	float theta = 0.0f;
	//円周率
	float pi = std::numbers::pi_v<float>;

	RotateObject rotateObject = {};
	rotateObject.center = { 640.0f, 360.0f }; // 中心座標
	rotateObject.localPosition[static_cast<int>(kLeft)] = { -100.0f, 0.0f }; // 左頂点のローカル座標
	rotateObject.localPosition[static_cast<int>(kTop)] = { 0.0f, -100.0f }; // 上頂点のローカル座標
	rotateObject.localPosition[static_cast<int>(kRight)] = { 100.0f, 0.0f }; // 右頂点のローカル座標
	rotateObject.rotateVector = { 200.0f, 200.0f }; // 回転ベクトル（上方向）
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


		//回転
		theta -= 1.0f / 120.0f * pi;
		rotateObject.rotateObjectCenter = {};
		for (int i = 0; i < 3; i++) {
			rotateObject.rotateObjectCenter.x += rotateObject.localPosition[i].x;
			rotateObject.rotateObjectCenter.y += rotateObject.localPosition[i].y;
		}
		rotateObject.rotateObjectCenter /= 3.0f;

		rotateObject.rotateObjectCenter.x -= rotateObject.center.x;
		rotateObject.rotateObjectCenter.y -= rotateObject.center.y;

		for (int i = 0; i < 3; i++) {
			rotateObject.screenPosition[i].x = rotateObject.rotateObjectCenter.x * std::cos(theta) - rotateObject.rotateObjectCenter.y * std::sin(theta);
			rotateObject.screenPosition[i].y = rotateObject.rotateObjectCenter.y * std::cos(theta) + rotateObject.rotateObjectCenter.x * std::sin(theta);

			// 回転後の座標に重心と画面中心を足す
			rotateObject.screenPosition[i] = {
				rotated.x + rotateObject.rotateObjectCenter.x + rotateObject.center.x,
				rotated.y + rotateObject.rotateObjectCenter.y + rotateObject.center.y
			};
		}
		rotateObject.screenPosition[static_cast<int>(kLeft)].x += -100.0f;
		rotateObject.screenPosition[static_cast<int>(kTop)].y += -100.0f;
		rotateObject.screenPosition[static_cast<int>(kRight)].x += 100.0f;

		Novice::ScreenPrintf(0, 0, "theta:%f", theta);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kLeft)].x),
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kLeft)].y),
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kTop)].x),
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kTop)].y),
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kRight)].x),
			static_cast<int>(rotateObject.screenPosition[static_cast<int>(kRight)].y),
			WHITE, kFillModeSolid
		);

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