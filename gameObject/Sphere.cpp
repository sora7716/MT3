#include "Sphere.h"
#include "gameObject/Camera.h"
//#include "gameObject/plane/Plane.h"
#include "gameObject/Grid.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <numbers>
#include <string>
using namespace std;

//初期化
void Sphere::Initialize(Camera* camera, const SphereData&& sphereData) {
	//球の素材を記録
	sphereData_ = sphereData;
	//カメラを記録
	camera_ = camera;
	//ワールドトランスフォームの初期化
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };
	transform_.scale = { sphereData_.radius,sphereData_.radius,sphereData_.radius };
	transform_.translate = sphereData_.center;
	//カラーの初期化
	defaultColor = sphereData_.color;
}

//更新処理
void Sphere::Update() {
	//中身を削除
	coordA_.screen.clear();
	coordB_.screen.clear();
	coordC_.screen.clear();

	//球の計算
	const uint32_t kSubdivision = 10;//分割数
	const float kLatEvery = pi_f / float(kSubdivision);//経度分割1つ分の角度(θd)
	const float kLonEvery = 2.0f * pi_f / float(kSubdivision);//緯度分割1つ分の角度(φd)

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float lat = -pi_f / 2.0f + kLatEvery * latIndex;//θ
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//φ
			//ローカル座標
			coordA_.local = {
				 cos(lat) * cos(lon),
				 sin(lat),
				 cos(lat) * sin(lon)
			};

			coordB_.local = {
				 cos(lat + kLatEvery) * cos(lon),
				 sin(lat + kLatEvery),
				 cos(lat + kLatEvery) * sin(lon)
			};

			coordC_.local = {
				cos(lat + kLatEvery) * cos(lon + kLonEvery),
				sin(lat + kLatEvery),
				cos(lat + kLatEvery) * sin(lon + kLonEvery)
			};
			//スクリーン座標を求める
			coordA_.screen.push_back(BaseGameObject::ScreenTransform(camera_, coordA_.local));
			coordB_.screen.push_back(BaseGameObject::ScreenTransform(camera_, coordB_.local));
			coordC_.screen.push_back(BaseGameObject::ScreenTransform(camera_, coordC_.local));
		}
	}
}

//デバックテキスト
void Sphere::DebugText(const char* name) {
#ifdef _DEBUG
	string radiusLabel = string(name) + ".scale";
	ImGui::DragFloat3(radiusLabel.c_str(), &transform_.scale.x, 0.01f);

	string rotateLabel = string(name) + ".rotate";
	ImGui::DragFloat3(rotateLabel.c_str(), &transform_.rotate.x, 0.1f);

	string centerLabel = string(name) + ".center";
	ImGui::DragFloat3(centerLabel.c_str(), &transform_.translate.x, 0.01f);
	/*ImGui::DragFloat3("ce", &lissajousCenter_.x, 0.1f);*/
#endif // _DEBUG
}

//描画
void Sphere::Draw() {
	for (int i = 0;i < coordA_.screen.size();i++) {
		//縦の線の描画
		Novice::DrawLine(
			(int)coordA_.screen[i].x, (int)coordA_.screen[i].y,
			(int)coordB_.screen[i].x, (int)coordB_.screen[i].y,
			sphereData_.color
		);
		//横の線の描画
		Novice::DrawLine(
			(int)coordB_.screen[i].x, (int)coordB_.screen[i].y,
			(int)coordC_.screen[i].x, (int)coordC_.screen[i].y,
			sphereData_.color
		);
	}
}

//当たった時の判定
void Sphere::OnCollision() {
	//判定を見る
	if (sphereData_.isHit) {
		SetColor(RED);
	} else {
		SetColor(defaultColor);
	}
}

//カラーのセッター
void Sphere::SetColor(uint32_t color) {
	sphereData_.color = color;
}

//スフィアの素材のゲッター
const SphereData& Sphere::GetSphereMaterial()const {
	return sphereData_;
}

//平面のセッター
void Sphere::SetPlane(Plane* plane) {
	plane_ = plane;
}

//カメラのゲッター
void Sphere::SetCamera(Camera* camera) {
	camera_ = camera;
}

//球の素材のセッター
void Sphere::SetSphere(const SphereData& sphereData) {
	sphereData_ = sphereData;
}

//当たり判定のセッター
void Sphere::SetIsHit(bool isHit) {
	sphereData_.isHit = isHit;
}

