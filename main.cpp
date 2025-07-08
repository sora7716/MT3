#include <Novice.h>
#include <stdlib.h>
#include <time.h>
const char kWindowTitle[] = "GC1A_01_イイヅカ_ソラ_タイトル";

//2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//サイズ
typedef struct Size {
	float width;
	float height;
}Size;

//2次元の整数ベクトル
typedef struct Vector2Int {
	int x;
	int y;
}Vector2Int;

//パーティクルのデータ
typedef struct ParticleData {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	int radius;
	Vector2Int random;
	bool isAlive;
}ParticleData;

const int PARTICLE_NUM = 300;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 600, 400);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//パーティクルの変数↓
	ParticleData particle[PARTICLE_NUM] = { 0 };
	for (int i = 0; i < PARTICLE_NUM; i++) {
		particle[i].position = { 0,0 };
		particle[i].velocity = { 0,0 };
		particle[i].acceleration = { -0.3f,0.3f };
		particle[i].radius = 0;
		particle[i].random = { 0,0 };
		particle[i].isAlive = false;
	}
	//パーティクルの変数↑

	//マウスの座標
	Vector2Int mousePos = {};
	//ボックスのサイズ
	Size emitSize{
		50,50
	};
	//ランド関数に使う
	srand((unsigned int)time(nullptr));

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		Novice::GetMousePosition(&mousePos.x, &mousePos.y);

		///
		/// ↓更新処理ここから
		///
		//ボックスの横幅変更
		if (keys[DIK_A] && emitSize.width > 10) {
			emitSize.width--;
		} else if (keys[DIK_D]) {
			emitSize.width++;
		}

		//ボックスの縦幅変更
		if (keys[DIK_S] && emitSize.height > 10) {
			emitSize.height--;
		} else if (keys[DIK_W]) {
			emitSize.height++;
		}

		//パーティクルの召喚
		for (int i = 0; i < PARTICLE_NUM; i++) {
			if (!particle[i].isAlive) {
				particle[i].isAlive = true;
				particle[i].random.x = rand() % (int)emitSize.width + mouseX - (int)emitSize.width / 2;
				particle[i].random.y = rand() % (int)emitSize.height + mouseY - (int)emitSize.height / 2;
				particle[i].radius = rand() % 8 + 3;
				particle[i].position = { (float)particle[i].random.x,(float)particle[i].random.y };
				break;
			}
		}

		//パーティクルの動き
		for (int i = 0; i < PARTICLE_NUM; i++) {
			if (particle[i].isAlive) {
				particle[i].velocity.y += particle[i].acceleration.y;
				particle[i].position.y += particle[i].velocity.y;
			}
			if (particle[i].position.y >= 720) {
				particle[i].isAlive = false;
				particle[i].position = { 0,0 };
				particle[i].velocity = { 0,0 };
			}
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//背景
		Novice::DrawBox(0, 0, 600, 400, 0.0f, BLACK, kFillModeSolid);

		//横のライン
		Novice::DrawLine(0, 200, 600, 200, WHITE);

		//縦のライン
		Novice::DrawLine(300, 0, 300, 400, WHITE);

		//パーティクル
		for (int i = 0; i < PARTICLE_NUM; i++) {
			if (particle[i].isAlive) {
				Novice::DrawEllipse((int)particle[i].position.x, (int)particle[i].position.y, (int)particle[i].radius, particle[i].radius, 0.0f, WHITE, kFillModeSolid);
			}
		}

		//ボックス
		Novice::DrawBox(
			mouseX - (int)emitSize.width / 2,
			mouseY - (int)emitSize.height / 2,
			(int)emitSize.width,
			(int)emitSize.height,
			0.0f, WHITE, kFillModeWireFrame);

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