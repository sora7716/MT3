#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
const char kWindowTitle[] = "GC1D 01 イイヅカ　ソラ";

typedef struct Vector2 {
	float x;
	float y;
}Vector2;

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

	const float kLeftBottomX = -width / 2.0f;
	const float kLeftBottomY = height / 2.0f;

	const float kRightTopX = width / 2.0f;
	const float kRightTopY = -height / 2.0f;

	const float kRightBottomX = width / 2.0f;
	const float kRightBottomY = height / 2.0f;

	//回転
	float theta = float(M_PI);
	float leftTopRotateX = 0.0f;
	float leftTopRotateY = 0.0f;

	float leftBottomRotateX = 0.0f;
	float leftBottomRotateY = 0.0f;

	float rightTopRotateX = 0.0f;
	float rightTopRotateY = 0.0f;

	float rightBottomRotateX = 0.0f;
	float rightBottomRotateY = 0.0f;

	//元の位置に戻す
	float leftTopPosX = 0.0f;
	float leftTopPosY = 0.0f;

	float leftBottomPosX = 0.0f;
	float leftBottomPosY =0.0f;

	float rightTopPosX = 0.0f;
	float rightTopPosY = 0.0f;

	float rightBottomPosX = 0.0f;
	float rightBottomPosY = 0.0f;

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

		//θを動かす
		if (isRotate) {
			theta += 1.0f / 15.0f * float(M_PI);
		}

		//加法定理して回転させる
		leftTopRotateX = kLeftTopX * cosf(theta) - kLeftTopY * sinf(theta);
		leftTopRotateY = kLeftTopY * cosf(theta) + kLeftTopX * sinf(theta);

		leftBottomRotateX = kLeftBottomX * cosf(theta) - kLeftBottomY * sinf(theta);
		leftBottomRotateY = kLeftBottomY * cosf(theta) + kLeftBottomX * sinf(theta);

		rightTopRotateX = kRightTopX * cosf(theta) - kRightTopY * sinf(theta);
		rightTopRotateY = kRightTopY * cosf(theta) + kRightTopX * sinf(theta);

		rightBottomRotateX = kRightBottomX * cosf(theta) - kRightBottomY * sinf(theta);
		rightBottomRotateY = kRightBottomY * cosf(theta) + kRightBottomX * sinf(theta);

		//回転の中心を動かす
		leftTopPosX = leftTopRotateX + centerPosX;
		leftTopPosY = leftTopRotateY + centerPosY;

		leftBottomPosX = leftBottomRotateX + centerPosX;
		leftBottomPosY = leftBottomRotateY + centerPosY;

		rightTopPosX = rightTopRotateX + centerPosX;
		rightTopPosY = rightTopRotateY + centerPosY;

		rightBottomPosX = rightBottomRotateX + centerPosX;
		rightBottomPosY = rightBottomRotateY + centerPosY;

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
			static_cast<int>(leftTopPosX), static_cast<int>(leftTopPosY),
			static_cast<int>(rightTopPosX), static_cast<int>(rightTopPosY),
			static_cast<int>(leftBottomPosX), static_cast<int>(leftBottomPosY),
			static_cast<int>(rightBottomPosX), static_cast<int>(rightBottomPosY),
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