#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "AL2_1-6";

//ベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//int型のベクトル
typedef struct Vector2Int {
	int32_t x;
	int32_t y;
} Vector2Int;

const int32_t kParticleNum = 3000; //パーティクルの数
const int32_t kWindowWidth = 1280;//スクリーンの横幅
const int32_t kWidowHeight = 720; //スクリーンの縦幅
const Vector2Int kMap = { 40,23 };//背景にボックスを置く数
const int32_t kBoxSize = 32;//背景の白ボックスのサイズ
const int32_t kParticleSize = 80; //パーティクルのサイズ

//パーティクルに使うやつ
typedef struct ParticleProp {
	Vector2 position; //パーティクルの位置
	Vector2 beginPosition; //パーティクルの初期位置
	float speed; //パーティクルの速度
	float angle; //パーティクルの角度
	uint32_t color; //パーティクルの色
	bool isAlive; //パーティクルが生きているかどうか
}ParticleProp;

/// <summary>
/// 度数法を弧度法に直す
/// </summary>
/// <param name="angle">度数法</param>
/// <returns>弧度法</returns>
float ConversionToRadian(float angle) {
	return angle * (static_cast<float>(M_PI) / 180.0f); //角度をラジアンに変換
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWidowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//テクスチャ
	int32_t particleImageHandle = Novice::LoadTexture("./Resources/xparticle.jpg");

	//ランド関数に使う
	srand((unsigned int)time(nullptr));

	//マウス用の変数
	Vector2Int mousePos = {};

	//パーティクルの速度の基準値
	float particleBaseSpeed = 1.0f;
	//パーティクル
	ParticleProp particle[kParticleNum] = {};

	//マウスをクリックしどうか
	bool isPressClick = false;

	//ブレンドモード
	BlendMode blendMode = BlendMode::kBlendModeNone; //ブレンドモードの初期値

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

		//クリックした瞬間だけ
		if (Novice::IsTriggerMouse(0)) {
			isPressClick = true; //パーティクルが生きている状態にする
			for (int32_t i = 0; i < kParticleNum; i++) {
				//マウスのクリックした位置に配置
				particle[i].position.x = static_cast<float>(mousePos.x - (kParticleSize / 2));
				particle[i].position.y = static_cast<float>(mousePos.y - (kParticleSize / 2));
				particle[i].beginPosition = particle[i].position; //パーティクルの初期位置を設定
			}
		}

		//パーティクルの生成
		if (isPressClick) {
			for (int32_t i = 0; i < kParticleNum; i++) {
				//パーティクルが生きていないなら
				if (!particle[i].isAlive) {
					particle[i].isAlive = true;//パーティクルが生きている状態にする
					particle[i].speed = particleBaseSpeed; //パーティクルの速度
					particle[i].color = WHITE; //パーティクルの色
					particle[i].angle = static_cast<float>(rand() % 360);//度数法でランダムな角度をもとめる
					particle[i].angle = ConversionToRadian(particle[i].angle);//角度をラジアンに変換
					break;
				}
			}
		}

		//パーティクルの移動
		for (int32_t i = 0; i < kParticleNum; i++) {
			if (particle[i].isAlive) {
				//極座標で動かす
				particle[i].position.x += particle[i].speed * cosf(particle[i].angle);
				particle[i].position.y += particle[i].speed * sinf(particle[i].angle);
				//パーティクルのアルファ値を変化させていく
				if (particle[i].color > 0xFFFFFF00) {
					particle[i].color -= 0x00000001;
				} else {
					particle[i].isAlive = false;//パーティクルが死んだ状態にする
					particle[i].position = particle[i].beginPosition;//パーティクルの位置を初期位置に戻す
					particle[i].color = WHITE; //パーティクルの色を元に戻す
				}
			}
		}

		ImGui::Begin("Debug Particle");
		const char* blendModes[] = { "None", "Normal", "Add", "Subtract" ,"Multiply","Screen"};
		int blendIndex = static_cast<int>(blendMode);
		if (ImGui::Combo("BlendMode", &blendIndex, blendModes, IM_ARRAYSIZE(blendModes))) {
			// blendIndexが変更されたらBlendModeに変換して反映
			blendMode = static_cast<BlendMode>(blendIndex);
		}
		ImGui::DragFloat("Particle Speed", &particleBaseSpeed, 0.01f, 0.1f, 10.0f,"%.1f");
		ImGui::Text("Click : MoveEmitter");
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//ブレンドしたくない
		Novice::SetBlendMode(BlendMode::kBlendModeNone);
		//背景
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK, kFillModeSolid);
		//白い線
		for (int32_t x = 0; x < kMap.x; x++) {
			for (int32_t y = 0; y < kMap.y; y++) {
				Novice::DrawBox(x * kBoxSize, y * kBoxSize, kBoxSize, kBoxSize, 0.0f, 0xB3B3B355, kFillModeWireFrame);
			}
		}

		//ブレンドしたい
		Novice::SetBlendMode(blendMode);
		//パーティクル
		for (int32_t i = 0; i < kParticleNum; i++) {
			if (particle[i].isAlive) {
				Novice::DrawSprite(
					static_cast<int32_t>(particle[i].position.x),
					static_cast<int32_t>(particle[i].position.y),
					particleImageHandle,
					1.0f, 1.0f, 0.0f, particle[i].color
				);
			}
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