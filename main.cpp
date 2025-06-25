#include <Novice.h>
#include <cmath>
#include <numbers>

const char kWindowTitle[] = "イイヅカ_ソラ";
const float kDeltaTime = 1.0f / 60.0f;//デルタタイム

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//円
typedef struct Circle {
	Vector2 center;
	float radius;
	unsigned int color;
}Circle;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//円
	Circle circle[2] = {
		{{300.0f,300.0f},50.0f,WHITE},
		{{800.0f,500.0f},50.0f,RED}
	};
	//スタートポジション
	Vector2 startPos = circle[0].center;
	//エンドポジション
	Vector2 endPos = circle[1].center;
	//フレーム
	float frame = 0.0f;
	//パラメーター
	float param = 0.0f;
	//移動フラグ
	bool isMoving = false;
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

		//スペースを押すとフラグを切り替える
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			isMoving ^= true;
		}

		//動かす
		if (isMoving) {
			frame += kDeltaTime;
		}

		//サイン波を使用して行ったり来たりさせる
		param = std::sin(std::numbers::pi_v<float>*2.0f * (frame / 4.0f) - (std::numbers::pi_v<float> / 2.0f));

		//線形補間
		circle[0].center.x = startPos.x + ((param + 1.0f) / 2.0f) * (endPos.x - startPos.x);
		circle[0].center.y = startPos.y + ((param + 1.0f) / 2.0f) * (endPos.y - startPos.y);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//円の描画
		for (int i = 1; i >= 0; i--) {
			Novice::DrawEllipse(
				static_cast<int>(circle[i].center.x),
				static_cast<int>(circle[i].center.y),
				static_cast<int>(circle[i].radius),
				static_cast<int>(circle[i].radius),
				0.0f, circle[i].color, kFillModeSolid
			);
		}

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
