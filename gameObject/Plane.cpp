#include "Plane.h"
#include "gameObject/Camera.h"
#include "gameObject/Grid.h"
#include "myMath/func/Rendering.h"
#include <string>
using namespace std;
//初期化
void Plane::Initialize(Camera* camera, PlaneData* planeData) {
	camera_ = camera;
	planeData_ = planeData;
}

//更新処理
void Plane::Update() {
	Vector3 center = planeData_.distance * planeData_.normal;
	Vector3 perpendiculars[4]{};

	perpendiculars[0] = Math::Normalize(Perpendicular(planeData_.normal));//1
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };//2
	perpendiculars[2] = Math::Cross(planeData_.normal, perpendiculars[0]);//3
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };//4

	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = 2.0f * perpendiculars[index];
		Vector3 point = center + extend;
		points_[index] = Rendering::Transform(Rendering::Transform(point, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
	}
}

#ifdef _DEBUG
//デバックテキスト
void Plane::DebugText(const char* name) {
	string nomalLabel = (string)name + ".normal";
	ImGui::DragFloat3(nomalLabel.c_str(), &planeData_.normal.x, 0.01f);
	planeData_.normal = Math::Normalize(planeData_.normal);
	string distanceLabel = (string)name + ".distance";
	ImGui::DragFloat(distanceLabel.c_str(), &planeData_.distance, 0.01f);
}
#endif // _DEBUG

//描画処理
void Plane::Draw() {
	Novice::DrawLine(
		(int)points_[0].x, (int)points_[0].y,
		(int)points_[2].x, (int)points_[2].y,
		planeData_.color
	);
	Novice::DrawLine(
		(int)points_[1].x, (int)points_[1].y,
		(int)points_[2].x, (int)points_[2].y,
		planeData_.color
	);
	Novice::DrawLine(
		(int)points_[0].x, (int)points_[0].y,
		(int)points_[3].x, (int)points_[3].y,
		planeData_.color
	);
	Novice::DrawLine(
		(int)points_[1].x, (int)points_[1].y,
		(int)points_[3].x, (int)points_[3].y,
		planeData_.color
	);

}

//平面のデータのゲッター
const PlaneData& Plane::GetPlaneMaterial() {
	// TODO: return ステートメントをここに挿入します
	return planeData_;
}

//平面のデータのセッター
void Plane::SetPlane(const PlaneData& material) {
	planeData_ = material;
}

//垂直
Vector3 Plane::Perpendicular(const Vector3& v) {
	if (v.x != 0.0f || v.y != 0.0f) {
		return { -v.y,v.x,0.0f };
	} else {
		return { 0.0f,-v.z,v.y };
	}
}
