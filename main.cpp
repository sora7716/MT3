#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
const char kWindowTitle[] = "GC1D 01 イイヅカ　ソラ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//テクスチャ
	int textureHandle = Novice::LoadTexture("white1x1.png");

	//中心点
	float centerPosX = 400;
	float centerPosY = 400;
	float width = 200;
	float height = 100;

	//初期地点
	const float kLeftTopX = -width / 2.0f;
	const float kLeftTopY = -height / 2.0f;

	const float kLeftTopX2 = -width / 2.0f;
	const float kLeftTopY2 = height / 2.0f;

	const float kRightTopX = width / 2.0f;
	const float kRightTopY = -height / 2.0f;

	const float kRightTopX2 = width / 2.0f;
	const float kRightTopY2 = height / 2.0f;

	//回転
	float theta = float(M_PI);
	float leftTopRotateX = 0.0f;
	float leftTopRotateY = 0.0f;

	float leftTopRotateX2 = 0.0f;
	float leftTopRotateY2 = 0.0f;

	float rightTopRotateX = 0.0f;
	float rightTopRotateY = 0.0f;

	float rightTopRotateX2 = 0.0f;
	float rightTopRotateY2 = 0.0f;

	//元の位置に戻す
	float leftPosX = centerPosX;
	float leftPosY = centerPosY;

	float leftPosX2 = centerPosX;
	float leftPosY2 = centerPosY + height;

	float rightPosX = centerPosX + width;
	float rightPosY = centerPosY;

	float rightPosX2 = centerPosX + width;
	float rightPosY2 = centerPosY + height;

	//開店するかのフラグ
	bool isRotate = false;

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

		//回転させるかの切り替えスイッチ
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			isRotate ^= true;
		}

		//Θを動かす
		if (isRotate) {
			theta += 1.0 / 15.0 * float(M_PI);
		}

		//加法定理して回転させる
		leftTopRotateX = kLeftTopX * cosf(theta) - kLeftTopY * sinf(theta);
		leftTopRotateY = kLeftTopY * cosf(theta) + kLeftTopX * sinf(theta);

		leftTopRotateX2 = kLeftTopX2 * cosf(theta) - kLeftTopY2 * sinf(theta);
		leftTopRotateY2 = kLeftTopY2 * cosf(theta) + kLeftTopX2 * sinf(theta);

		rightTopRotateX = kRightTopX * cosf(theta) - kRightTopY * sinf(theta);
		rightTopRotateY = kRightTopY * cosf(theta) + kRightTopX * sinf(theta);

		rightTopRotateX2 = kRightTopX2 * cosf(theta) - kRightTopY2 * sinf(theta);
		rightTopRotateY2 = kRightTopY2 * cosf(theta) + kRightTopX2 * sinf(theta);

		//回転の中心を動かす
		leftPosX = leftTopRotateX + centerPosX;
		leftPosY = leftTopRotateY + centerPosY;

		leftPosX2 = leftTopRotateX2 + centerPosX;
		leftPosY2 = leftTopRotateY2 + centerPosY;

		rightPosX = rightTopRotateX + centerPosX;
		rightPosY = rightTopRotateY + centerPosY;

		rightPosX2 = rightTopRotateX2 + centerPosX;
		rightPosY2 = rightTopRotateY2 + centerPosY;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		/// 
		
		//回転しているか
		Novice::ScreenPrintf(0, 0, "isRotate:%s", isRotate ? "true" : "false");
		
		//白いボックス
		Novice::DrawQuad(
			static_cast<int>(leftPosX), static_cast<int>(leftPosY),
			static_cast<int>(rightPosX), static_cast<int>(rightPosY),
			static_cast<int>(leftPosX2), static_cast<int>(leftPosY2),
			static_cast<int>(rightPosX2), static_cast<int>(rightPosY2),
			0, 0, 1, 1, textureHandle, WHITE
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