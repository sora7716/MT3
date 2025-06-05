#include "gameObject/Camera.h"
#include "gameObject/Grid.h"
#include "gameObject/Sphere.h"
#include "gameObject/Plane.h"
#include "gameObject/Line.h"
#include "gameObject/AABB.h"
#include "gameObject/OBB.h"
#include "myMath/func/Collision.h"

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
	planeData.normal = {0.0f,1.0f,0.0f};
	planeData.distance = 1.0f;
	planeData.color = WHITE;
	plane->Initialize(camera, std::move(planeData));

	Sphere* point[2];
	point[0] = new Sphere();
	point[1] = new Sphere();

	Line* line[3];
	Segment segment[3];
	for (int i = 0;i < 3;i++) {
		segment[i].origin = {};
		segment[i].diff = { 1.0f,1.0f,1.0f };
		line[i] = new Line();
		line[i]->Initialize(camera, std::move(segment[i]));
	}
	Vector3 bezierPoint[3] = {
		{1.0f,0.0f,1.0f},
		{1.0f,1.0f,2.5f},
		{1.0f,1.0f,1.0f},
	};

	line[0]->SetSegment({ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} });
	line[0]->SetPoint({ -1.5f,0.6f,0.6f });

	std::vector<Vector3>catmullRomPoint;
	catmullRomPoint.push_back({ 1.0f,1.0f,1.0f });
	catmullRomPoint.push_back({ 1.0f,2.0f,1.0f });
	catmullRomPoint.push_back({ 1.0f,1.0f,2.0f });
	catmullRomPoint.push_back({ 2.0f,1.0f,1.0f });

	AABB* aabb = new AABB();
	AABBData aabbData = {
		.min = {},
		.max = {1.0f,1.0f,1.0f}
	};
	aabb->Initialize(camera, std::move(aabbData));

	OBB* obb = new OBB();
	OBBData obbData;
	obbData.center = {};
	obbData.color = WHITE;
	obb->Initialize(camera, std::move(obbData));
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
		
		camera->Update(keys, preKeys);
		grid->Update();
		sphere->Update();
		sphere->OnCollision();
		plane->Update();

		point[0]->Initialize(camera, { line[0]->GetPoint(), 0.01f });
		point[1]->Initialize(camera, { line[0]->GetClosestPoint(), 0.01f });
		point[0]->Update();
		point[1]->Update();
		line[1]->SetBezierPoints(bezierPoint);
		line[2]->SetCatmullRomPoints(catmullRomPoint);

		camera->DebugText();
		sphere->DebugText();
		plane->DebugText();

		aabb->Update();
		//aabb->DebugText();

		obb->Update();

		
		sphere->SetIsHit(Collision::GetInstance()->IsCollision(sphere->GetSphereMaterial(), plane->GetPlaneMaterial()));
	    //obb->DebagText();

		/*ImGui::Begin("bezier");
		ImGui::DragFloat3("0", &bezierPoint[0].x, 0.1f);
		ImGui::DragFloat3("1", &bezierPoint[1].x, 0.1f);
		ImGui::DragFloat3("2", &bezierPoint[2].x, 0.1f);
		ImGui::End();

		ImGui::Begin("catmullRom");
		ImGui::DragFloat3("0", &catmullRomPoint[0].x, 0.1f);
		ImGui::DragFloat3("1", &catmullRomPoint[1].x, 0.1f);
		ImGui::DragFloat3("2", &catmullRomPoint[2].x, 0.1f);
		ImGui::DragFloat3("3", &catmullRomPoint[3].x, 0.1f);
		ImGui::End();*/
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		grid->Draw();
		sphere->Draw();
		plane->Draw();
		//line[0]->DrawSegment();
		//line[1]->DrawBezier();
		//line[2]->DrawCatmullRom();
		//aabb->Draw();
		//obb->Draw();
		/*point[0]->Draw();
		point[1]->Draw();*/
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
