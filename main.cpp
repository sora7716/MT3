#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 度数法を弧度法に直す
/// </summary>
/// <param name="degree">角度(度数)</param>
/// <returns>角度(弧度)</returns>
float MakeRadian(float degree) {
	return degree * (static_cast<float>(M_PI) / 180.0f);
}
const char kWindowTitle[] = "イイヅカ_ソラ";

//スピードの定数
const float kSpeed = 5.0f;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//位置
	float posX = 0.0f;
	float posY = 0.0f;

	//半径
	float radius = 30.0f;

	//スピード
	float speedX = 0.0f;
	float speedY = 0.0f;

	//角度
	float theta = 0.0f;

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

		//移動(角度で向きをきめる)
		posX += speedX * cosf(theta);
		posY -= speedY * sinf(theta);

		//キー入力
		//縦移動
		if (keys[DIK_W]) {
			//角度を打ち込む
			theta = MakeRadian(90.0f);
			speedY = kSpeed;
		} else if (keys[DIK_S]) {
			//角度を打ち込む
			theta = MakeRadian(270.0f);
			speedY = kSpeed;
		} else {
			speedY = 0.0f;
		}

		//横移動
		if (keys[DIK_D]) {
			//角度を打ち込む
			theta = MakeRadian(0.0f);
			speedX = kSpeed;
		} else if (keys[DIK_A]) {
			//角度を打ち込む
			theta = MakeRadian(180.0f);
			speedX = kSpeed;
		} else {
			speedX = 0.0f;
		}

		//斜め移動
		if (keys[DIK_W] && keys[DIK_D]) {
			//角度を打ち込む
			theta = MakeRadian(45.0f);
			speedX = kSpeed;
			speedY = kSpeed;
		} else if (keys[DIK_W] && keys[DIK_A]) {
			//角度を打ち込む
			theta = MakeRadian(135.0f);
			speedX = kSpeed;
			speedY = kSpeed;
		} else if (keys[DIK_S] && keys[DIK_D]) {
			//角度を打ち込む
			theta = MakeRadian(-45.0f);
			speedX = kSpeed;
			speedY = kSpeed;

		} else if (keys[DIK_S] && keys[DIK_A]) {
			//角度を打ち込む
			theta = MakeRadian(-135.0f);
			speedX = kSpeed;
			speedY = kSpeed;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawBox(static_cast<int>(posX - radius), static_cast<int>(posY + radius), static_cast<int>(radius * 2.0f), static_cast<int>(radius * 2.0f), 0.0f, WHITE, kFillModeSolid);
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
