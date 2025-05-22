#include "gameObject/Camera.h"
#include "gameObject/Grid.h"
#include "gameObject/Sphere.h"
#include "gameObject/Plane.h"

const char kWindowTitle[] = "イイヅカ_ソラ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = new Camera();
	camera->Initialize(1280, 720);
	
	Grid* grid = new Grid();
	grid->Initialize(camera);

	Sphere* sphere = new Sphere();
	SphereData sphreData;
	sphreData.center = { 0.0f,0.0f,0.0f };
	sphreData.radius = 0.5f;
	sphreData.color = WHITE;
	sphere->Initialize(camera, std::move(sphreData));

	Plane* plane = new Plane();
	PlaneData planeData;
	planeData.normal = {0.0f,0.0f,0.0f};
	planeData.distance = {};
	planeData.color = WHITE;
	plane->Initialize(camera, std::move(planeData));
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
		
		if (keys[DIK_W]) {
			sphreData.isHit = true;
		} else {
			sphreData.isHit = false;
		}
		camera->Update(keys, preKeys);
		grid->Update();
		sphere->Update();
		plane->Update();
		sphere->OnCollision();

		camera->DebugText();
		sphere->DebugText();
		plane->DebugText();
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		grid->Draw();
		sphere->Draw();
		plane->Draw();
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
