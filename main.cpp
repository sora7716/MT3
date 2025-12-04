#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#include <imgui.h>
#include <cstdint>
const char kWindowTitle[] = "AL2_1-5";

//2次元のベクトル
struct Vector2 {
	float x;
	float y;

	//加算(+=)
	const Vector2& operator+=(const Vector2& v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	}
};

//サイズ
struct Size {
	float width;
	float height;
};

//2次元の整数ベクトル
struct Vector2Int {
	int32_t x;
	int32_t y;
};

//パーティクルのデータ
struct ParticleData {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	int32_t radius;
	Vector2Int random;
	uint32_t color;
	bool isAlive;
};

//四次元のベクトル
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

/// <summary>
/// 正規化された値をunsigned intに変換
/// </summary>
/// <param name="normal">正規化された値</param>
/// <returns></returns>
uint32_t NormalizeColorByte(float normal) {
	int result = 0;
	if (normal < 0.0f) {
		result = 0;
	} else if (normal >= 1.0f) {
		result = 255;
	}
	result = static_cast<int>(normal * 255.0f);
	return result;
}

/// <summary>
/// Vector4をカラーコードに変換
/// </summary>
/// <param name="color">カラー</param>
/// <returns>カラーコード</returns>
uint32_t ColorCodeFromVector4(const Vector4& color) {
	int r = NormalizeColorByte(color.x);
	int g = NormalizeColorByte(color.y);
	int b = NormalizeColorByte(color.z);
	int a = NormalizeColorByte(color.w);
	return (r << 24) | (g << 16) | (b << 8) | a;
}

//パーティクルの数
const int32_t kParticleNum = 300;
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
	Vector2 particleAcceleration = { 0.0f, 0.3f };
	Vector4 particleColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float particleRadius = 5.0f;
	int32_t randomRadiusRange = 8;//ランダムな半径にする場合の範囲
	bool isRandomRadius = false;//ランダムな半径にするかどうか
	bool isRandomColor = false;//ランダムカラーにするかどうか

	//パーティクルの変数↓
	ParticleData particle[kParticleNum] = { 0 };
	for (int32_t i = 0; i < kParticleNum; i++) {
		particle[i].position = { 0,0 };
		particle[i].velocity = { 0,0 };
		particle[i].acceleration = particleAcceleration;
		particle[i].radius = 0;
		particle[i].random = { 0,0 };
		particle[i].color = WHITE;
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
		if (!isRandomRadius) {
			ImGui::DragFloat("ParticleSize", &particleRadius, 0.1f,0.0f, 100.0f, "%.1f");
		} else {
			ImGui::DragInt("ParticleRadiusRange", &randomRadiusRange, 1.0f, 1, 100, "%d");
		}
		if (!isRandomColor) {
			ImGui::ColorEdit4("Color", &particleColor.x);
		}
		ImGui::DragFloat2("ParticleAcceleration", &particleAcceleration.x, 0.1f, -10.0f, 10.0f, "%.1f");
		ImGui::Checkbox("IsRandomColor", &isRandomColor);
		ImGui::Checkbox("IsRandomSize", &isRandomRadius);
		if (ImGui::Button("Reset")) {
			for (int32_t i = 0; i < kParticleNum; i++) {
				particle[i].isAlive = false;
				particle[i].position = { 0,0 };
				particle[i].velocity = { 0,0 };
			}
		}
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
				if (isRandomRadius) {
					particle[i].radius = rand() % randomRadiusRange + 1;
				}
				//位置を設定
				particle[i].position = {
					static_cast<float>(particle[i].random.x),
					static_cast<float>(particle[i].random.y)
				};
				//ランダムカラーを設定
				if (isRandomColor) {
					Vector4 randomColor = {
						static_cast<float>(rand() % 256) / 255.0f,
						static_cast<float>(rand() % 256) / 255.0f,
						static_cast<float>(rand() % 256) / 255.0f,
						1.0f
					};
					//色を変更
					particle[i].color = ColorCodeFromVector4(randomColor);
				}
				break;
			}
		}

		//パーティクルの動き
		for (int32_t i = 0; i < kParticleNum; i++) {
			//色を変更(ランダムカラーが設定されてなければ)
			if (!isRandomColor) {
				particle[i].color = ColorCodeFromVector4(particleColor);
			}
			//大きさを変更(ランダムサイズが設定されてなければ)
			if (!isRandomRadius) {
				particle[i].radius = static_cast<int32_t>(particleRadius);
			}
			if (particle[i].isAlive) {
				particle[i].velocity += particle[i].acceleration;
				particle[i].position += particle[i].velocity;
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
					particle[i].radius, particle[i].radius, 0.0f, particle[i].color, kFillModeSolid);
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