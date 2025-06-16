#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "GC1D 01 イイヅカ　ソラ";

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
	Vector2& operator+=(const Vector2& v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	Vector2 operator*(float num) {
		Vector2 result{
			x * num,
			y * num,
		};
		return result;
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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//ベクトル
	Vector2 vector = { 300,200 };

	//回転
	float theta = 0.0f;
	Vector2 rotation = {};

	//ポジションA
	Vector2 centerPos = { 600.0f,400.0f };

	//ポジションB
	Vector2 topPos = {};
	float scaleSpeed = 0.01f;
	float scale = 1.0f;

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

		//スケール加算
		scale += scaleSpeed;

		//0.5～2.0の範囲に収める
		if (scale <= 0.5f || scale > 2.0f) {
			scaleSpeed *= -1;
		}

		//回転
		theta -= 1.0f / 120.0f * float(M_PI);
		rotation.x = vector.x * cosf(theta) - vector.y * sinf(theta);
		rotation.y = vector.y * cosf(theta) + vector.x * sinf(theta);

		//元の位置に戻す
		topPos = centerPos + scale * rotation;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawLine((int)centerPos.x, (int)centerPos.y, (int)topPos.x, (int)topPos.y, WHITE);
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