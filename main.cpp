#include "gameObject/Camera.h"
#include "gameObject/Grid.h"
#include "gameObject/Sphere.h"
#include "gameObject/Plane.h"
#include "myMath/func/Collision.h"
#include <memory>

const char kWindowTitle[] = "イイヅカ_ソラ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//カメラ
	std::unique_ptr<Camera> camera = std::make_unique<Camera>();
	camera->Initialize(1280, 720);

	//グリッド
	Grid* grid = new Grid();
	grid->Initialize(camera.get());

	//球
	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	SphereData sphreData;
	sphreData.center = { 0.0f,0.0f,0.0f };
	sphreData.radius = 0.5f;
	sphreData.color = WHITE;
	sphere->Initialize(camera.get(), std::move(sphreData));

	//平面
	std::unique_ptr<Plane> plane = std::make_unique<Plane>();
	PlaneData planeData;
	planeData.normal = { 0.0f,1.0f,0.0f };
	planeData.distance = 1.0f;
	planeData.color = WHITE;
	plane->Initialize(camera.get(), std::move(planeData));

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

		//カメラ
		camera->Update(keys, preKeys);
		//グリッド
		grid->Update();
		//球
		sphere->Update();
		//球の当たった時の処理
		sphere->OnCollision();
		//平面
		plane->Update();

		//衝突判定
		//球と平面
		sphere->SetIsHit(Collision::GetInstance()->IsCollision(sphere->GetTransform(), plane->GetPlaneData()));

		//デバックテキスト
		camera->DebugText();
		sphere->DebugText();
		plane->DebugText();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//グリッド
		grid->Draw();
		//球
		sphere->Draw();
		//平面
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
