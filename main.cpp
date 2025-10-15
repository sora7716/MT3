#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "イイヅカ_ソラ";

struct Vector2 {
	float x;
	float y;
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

struct GameObject {
	Vector2 position;
	Vector2 velocity;
	float radius;
	Vector4 color;
};

/// <summary>
/// 色変えようの関数
/// </summary>
/// <param name="color">色</param>
/// <returns>カラーコードを返す</returns>
unsigned int ChangeColor(Vector4 color) {
	int r = static_cast<int>(color.x * 255.0f) << (8 * 3);
	int g = static_cast<int>(color.y * 255.0f) << (8 * 2);
	int b = static_cast<int>(color.z * 255.0f) << (8 * 1);
	int a = static_cast<int>(color.w * 255.0f);
	return r + g + b + a;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	GameObject circle = {
		.position{640.0f,360.0f},
		.velocity{},
		.radius = 50.0f,
		.color = {1.0f,1.0f,1.0f,1.0f}
	};

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

		ImGui::Begin("circle");
		ImGui::ColorEdit4("color", &circle.color.x);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawEllipse(
			static_cast<int>(circle.position.x),
			static_cast<int>(circle.position.y),
			static_cast<int>(circle.radius),
			static_cast<int>(circle.radius),
			0.0f, ChangeColor(circle.color), kFillModeSolid
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
