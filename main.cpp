#include <Novice.h>
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "AL2_1-4";
//画面のサイズ
const float kWindowWidth = 1280.0f;//ウィンドウの幅
const float kWindowHeight = 720.0f;//ウィンドウの高さ
//スプライトのサイズ
const float kSpriteSize = 600.0f;//スプライトの幅

//2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//2次元のベクトルInt
typedef struct Vector2Int {
	int x;
	int y;
}Vector2Int;

//4次元のベクトル
typedef struct Vector4 {
	float x;
	float y;
	float z;
	float w;
}Vector4;

/// <summary>
/// 正規化された値をunsigned intに変換
/// </summary>
/// <param name="normal">正規化された値</param>
/// <returns></returns>
unsigned int NormalizeColorByte(float normal) {
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
unsigned int ColorCodeFromVector4(const Vector4& color) {
	int r = NormalizeColorByte(color.x);
	int g = NormalizeColorByte(color.y);
	int b = NormalizeColorByte(color.z);
	int a = NormalizeColorByte(color.w);
	return (r << 24) | (g << 16) | (b << 8) | a;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//スプライトのロード
	int particle = Novice::LoadTexture("./Resources/particle.png");

	//マウスの座標
	Vector2Int mousePos = { 0, 0 };

	//真ん中のスプライトの座標
	Vector2 particlePos[3];
	for (int32_t i = 0; i < 3; i++) {
		particlePos[i] = {kWindowWidth / 2.0f - (100 * static_cast<float>(i * 2 + 1)),kWindowHeight / 2.0f - 300 };
	}

	//ブレンドモード
	BlendMode blend = BlendMode::kBlendModeNone;

	//カラー(Vector4)
	Vector4 color4 = { 1.0f, 1.0f, 1.0f, 1.0f };
	//カラー
	unsigned int color = WHITE;
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
		
		ImGui::Begin("Debug Blend");
		const char* blendModes[] = { "None", "Normal", "Add", "Subtract" };
		int blendIndex = static_cast<int>(blend);
		if (ImGui::Combo("BlendMode", &blendIndex, blendModes, IM_ARRAYSIZE(blendModes))) {
			// blendIndexが変更されたらBlendModeに変換して反映
			blend = static_cast<BlendMode>(blendIndex);
		}
		ImGui::ColorEdit4("color", &color4.x);
		color = ColorCodeFromVector4(color4);
		ImGui::End();
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//ブレンドしたくない↓
		Novice::SetBlendMode(BlendMode::kBlendModeNone);
		//背景
		Novice::DrawBox(0, 0, static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight), 0.0f, BLACK, kFillModeSolid);
		//縦のライン
		Novice::DrawLine(static_cast<int32_t>(kWindowWidth/2.0f), 0, static_cast<int32_t>(kWindowWidth / 2.0f), static_cast<int32_t>(kWindowHeight), WHITE);
		//横のライン
		Novice::DrawLine(0, static_cast<int32_t>(kWindowHeight/2.0f), static_cast<int32_t>(kWindowWidth), static_cast<int32_t>(kWindowHeight/2.0f), WHITE);
		//ブレンドしたくない↑

		//ブレンドしたい↓
		Novice::SetBlendMode(blend);
		//マウス
		Novice::DrawSprite(mousePos.x - static_cast<int32_t>(kSpriteSize/2.0f), mousePos.y - static_cast<int32_t>(kSpriteSize / 2.0f), particle, 1, 1, 0.0f, color);
		//真ん中のスプライト
		for (int i = 0; i < 3; i++){
			Novice::DrawSprite((int)particlePos[i].x, (int)particlePos[i].y, particle, 1, 1, 0.0f, color);
		}
		//ブレンドしたい↑

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