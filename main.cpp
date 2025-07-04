#include <Novice.h>
#include <math.h>
const char kWindowTitle[] = "GC1D 01 イイヅカ　ソラ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float posY[3];
	typedef struct Vector2 {
		float x;
		float y;
	}Ventor2;

	typedef struct Ball {
		Vector2 position;
		float radius;
		unsigned int  color;
	}Ball;
	Vector2 vectorA{ 100,100 };
	Vector2 vectorA2{ 1000,400 };
	Ball posB{ {500,300},30,WHITE };

	float moveX;
	float moveY;
	float newX;
	float newY;
	float len[2];
	float speed = 10;

	float external;


	float distanceX[2] = { 0 };
	float distanceY[2] = { 0 };


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
		moveX = 0;
		moveY = 0;
		newX = 0;
		newY = 0;

		if (keys[DIK_RIGHT] || keys[DIK_D]) {
			moveX = 1;

		}
		if (keys[DIK_LEFT] || keys[DIK_A]) {
			moveX = -1;

		}
		if (keys[DIK_UP] || keys[DIK_W]) {
			moveY = 1;

		}
		if (keys[DIK_DOWN] || keys[DIK_S]) {
			moveY = -1;

		}
		len[0] = sqrtf(moveX * moveX + moveY * moveY);
		if (len[0] != 0.0f) {
			newX = moveX / len[0];
			newY = moveY / len[0];
		}


		posB.position.x += newX * speed;
		posB.position.y += newY * speed;

		//a(線の始点と終点)
		distanceX[0] = vectorA2.x - vectorA.x;
		distanceY[0] = vectorA2.y - vectorA.y;

		//b(線の始点とプレイヤーとの距離)
		distanceX[1] = posB.position.x - vectorA.x;
		distanceY[1] = posB.position.y - vectorA.y;

		//外積
		external = distanceX[0] * distanceY[1] - distanceY[0] * distanceX[1];

		//右側に来たら青それ以外は白
		if (external <= 0.0f) {
			posB.color = BLUE;
		} else {
			posB.color = WHITE;
		}
		Novice::ScreenPrintf(0, 0, "%f", external);

		posY[0] = (vectorA.y - 500) * -1;
		posY[1] = (vectorA2.y - 500) * -1;
		posY[2] = (posB.position.y - 500) * -1;


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawLine(int(vectorA.x), int(posY[0]), int(vectorA2.x), int(posY[1]), WHITE);
		Novice::DrawEllipse(int(posB.position.x), int(posY[2]), int(posB.radius), int(posB.radius), 0.0f, posB.color, kFillModeSolid);
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
