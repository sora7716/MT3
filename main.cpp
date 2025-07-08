#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#include <imgui.h>
#include <cstdint>
const char kWindowTitle[] = "Particle";

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
	int32_t x;
	int32_t y;
}Vector2Int;

//パーティクルのデータ
typedef struct ParticleData {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	int32_t radius;
	Vector2Int random;
	bool isAlive;
}ParticleData;

//パーティクルの数
const int kParticleNum = 300;
//ウィンドウのサイズ
const float kWindowWidth = 600.0f;
const float kWindowHeight = 400.0f;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//パーティクル全体にかかる加速度
	Vector2 particleAcceleration = { -0.3f, 0.3f };

	//パーティクルの変数↓
	ParticleData particle[kParticleNum] = { 0 };
	for (int32_t i = 0; i < kParticleNum; i++) {
		particle[i].position = { 0,0 };
		particle[i].velocity = { 0,0 };
		particle[i].acceleration = particleAcceleration;
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
	srand((uint32_t)time(nullptr));

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

		ImGui::Begin("Debug Particle");
		ImGui::DragFloat2("EmitterSize", &emitSize.width, 0.1f, 1.0f, 1000.0f, "%.1f");
		ImGui::DragFloat2("ParticleAcceleration", &particleAcceleration.x, 0.1f, -10.0f, 10.0f, "%.1f");
		ImGui::End();

		//パーティクルの召喚
		for (int32_t i = 0; i < kParticleNum; i++) {
			//パーティクルが生きていないならば
			if (!particle[i].isAlive) {
				//パーティクルを召喚する
				particle[i].isAlive = true;
				//加速度を設定
				particle[i].acceleration = particleAcceleration;
				//ランダムで位置を決める
				particle[i].random.x = rand() % static_cast<int32_t>(emitSize.width) + mousePos.x - static_cast<int32_t>(emitSize.width / 2.0f);
				particle[i].random.y = rand() % static_cast<int32_t>(emitSize.height) + mousePos.y - static_cast<int32_t>(emitSize.height / 2.0f);
				//大きさもランダムで決める
				particle[i].radius = rand() % 8 + 3;
				//位置を設定
				particle[i].position = {
					static_cast<float>(particle[i].random.x),
					static_cast<float>(particle[i].random.y)
				};
				break;
			}
		}

		//パーティクルの動き
		for (int32_t i = 0; i < kParticleNum; i++) {
			if (particle[i].isAlive) {
				particle[i].velocity.y += particle[i].acceleration.y;
				particle[i].position.y += particle[i].velocity.y;
			}
			if (particle[i].position.y >= 720 || particle[i].position.y < 0 || 
				particle[i].position.x >= 1280 || particle[i].position.x < 0) {
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
		Novice::DrawBox(0, 0, static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight), 0.0f, BLACK, kFillModeSolid);

		//横のライン
		Novice::DrawLine(0, static_cast<int32_t>(kWindowHeight / 2.0f), static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight / 2.0f), WHITE);

		//縦のライン
		Novice::DrawLine(static_cast<int32_t>(kWindowWidth / 2.0f), 0, static_cast<int32_t>(kWindowWidth / 2.0f), static_cast<int32_t>(kWindowHeight), WHITE);

		//パーティクル
		for (int32_t i = 0; i < kParticleNum; i++) {
			if (particle[i].isAlive) {
				Novice::DrawEllipse(
					static_cast<int32_t>(particle[i].position.x),
					static_cast<int32_t>(particle[i].position.y),
					particle[i].radius, particle[i].radius, 0.0f, WHITE, kFillModeSolid);
			}
		}

		//ボックス
		Novice::DrawBox(
			mousePos.x - static_cast<int32_t>(emitSize.width / 2.0f),
			mousePos.y - static_cast<int32_t>(emitSize.height / 2.0f),
			static_cast<int32_t>(emitSize.width),
			static_cast<int32_t>(emitSize.height),
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