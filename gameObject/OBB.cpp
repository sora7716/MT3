#include "OBB.h"
using namespace std;

//初期化
void OBB::Initialize(Camera* camera, const OBBData&& obbData) {
	camera_ = camera;//カメラを受け取る
	//OBBの値を設定
	obbData_ = obbData;
	//色を決める
	aabb_.color = obbData_.color;
}

//更新
void OBB::Update() {
	//角度
	//obbData_.rotate += 1.0f;

	//サイズを設定
	aabb_.min = -obbData_.size;
	aabb_.max = obbData_.size;

	//頂点を作成
	MakeVertecies();

	//回転行列から抽出
	Rendering::MakeOBBRotateMatrix(obbData_.orientations, obbData_.rotate);
	//抽出した値からワールドマトリックスを求める
	worldMatrix_ = Rendering::MakeOBBWorldMatrix(obbData_.orientations, obbData_.center);
	worldViewProjectionMatrix_ = worldMatrix_ * camera_->GetViewProjectionMatrix();

	//スクリーン座標に変換
	for (int i = 0; i < AABB::kAABB2DNum; i++) {
		screenVertecies_[i].leftTop = Rendering::Transform(Rendering::Transform(localVertecies_[i].leftTop, worldViewProjectionMatrix_), camera_->GetViewportMatrix());
		screenVertecies_[i].rightTop = Rendering::Transform(Rendering::Transform(localVertecies_[i].rightTop, worldViewProjectionMatrix_), camera_->GetViewportMatrix());
		screenVertecies_[i].leftBottom = Rendering::Transform(Rendering::Transform(localVertecies_[i].leftBottom, worldViewProjectionMatrix_), camera_->GetViewportMatrix());
		screenVertecies_[i].rightBottom = Rendering::Transform(Rendering::Transform(localVertecies_[i].rightBottom, worldViewProjectionMatrix_), camera_->GetViewportMatrix());
	}
}

//デバックテキスト
void OBB::DebagText(const char* label) {
	string sizeMoji = string(label) + "size";
	ImGui::SliderFloat3(sizeMoji.c_str(), &obbData_.size.x, 0.0f, 3.0f);
	string rotateMoji = string(label) + ".rotation";
	ImGui::DragFloat3(rotateMoji.c_str(), &obbData_.rotate.x, 0.01f);
	string translationMoji = string(label) + ".translation";
	ImGui::DragFloat3(translationMoji.c_str(), &obbData_.center.x, 0.01f);
}

//描画
void OBB::Draw() {
	//正面と背面の生成
	for (int i = 0; i < AABB::kAABB2DNum; i++) {
		Novice::DrawLine((int)screenVertecies_[i].leftTop.x, (int)screenVertecies_[i].leftTop.y, (int)screenVertecies_[i].rightTop.x, (int)screenVertecies_[i].rightTop.y, aabb_.color);
		Novice::DrawLine((int)screenVertecies_[i].rightTop.x, (int)screenVertecies_[i].rightTop.y, (int)screenVertecies_[i].rightBottom.x, (int)screenVertecies_[i].rightBottom.y, aabb_.color);
		Novice::DrawLine((int)screenVertecies_[i].rightBottom.x, (int)screenVertecies_[i].rightBottom.y, (int)screenVertecies_[i].leftBottom.x, (int)screenVertecies_[i].leftBottom.y, aabb_.color);
		Novice::DrawLine((int)screenVertecies_[i].leftBottom.x, (int)screenVertecies_[i].leftBottom.y, (int)screenVertecies_[i].leftTop.x, (int)screenVertecies_[i].leftTop.y, aabb_.color);
	}
	//側面の生成
	Novice::DrawLine((int)screenVertecies_[0].leftTop.x, (int)screenVertecies_[0].leftTop.y, (int)screenVertecies_[1].leftTop.x, (int)screenVertecies_[1].leftTop.y, aabb_.color);
	Novice::DrawLine((int)screenVertecies_[0].rightTop.x, (int)screenVertecies_[0].rightTop.y, (int)screenVertecies_[1].rightTop.x, (int)screenVertecies_[1].rightTop.y, aabb_.color);
	Novice::DrawLine((int)screenVertecies_[0].leftBottom.x, (int)screenVertecies_[0].leftBottom.y, (int)screenVertecies_[1].leftBottom.x, (int)screenVertecies_[1].leftBottom.y, aabb_.color);
	Novice::DrawLine((int)screenVertecies_[0].rightBottom.x, (int)screenVertecies_[0].rightBottom.y, (int)screenVertecies_[1].rightBottom.x, (int)screenVertecies_[1].rightBottom.y, aabb_.color);
}

// ワールドマトリックス逆行列のゲッター
Matrix4x4 OBB::GetOBBWorldMatrixInvers()const {
	Matrix4x4 obbWorldMatrixInvers = ~worldMatrix_;
	return obbWorldMatrixInvers;
}

//サイズのゲッター
Vector3 OBB::GetSize()const {
	return obbData_.size;
}

//OBBのデータのゲッター
OBBData OBB::GetOBBMaterial() const {
	return obbData_;
}

// ローカルの頂点
RectVertex* OBB::GetLocalVertex() {
	return localVertecies_;
}

void OBB::OnCollision(bool isHit) {
	if (isHit) {
		aabb_.color = RED;
	} else {
		aabb_.color = obbData_.color;
	}
}

// ポジションのセッター
void OBB::SetPosition(const Vector3& pos) {
	obbData_.center = pos;
}

//頂点を作成
void OBB::MakeVertecies() {
	localVertecies_[0].leftTop = { aabb_.min.x,aabb_.max.y,aabb_.min.z };
	localVertecies_[0].rightTop = { aabb_.max.x,aabb_.max.y,aabb_.min.z };
	localVertecies_[0].leftBottom = { aabb_.min.x,aabb_.min.y,aabb_.min.z };
	localVertecies_[0].rightBottom = { aabb_.max.x,aabb_.min.y,aabb_.min.z };

	localVertecies_[1].leftTop = { aabb_.min.x,aabb_.max.y,aabb_.max.z };
	localVertecies_[1].rightTop = { aabb_.max.x,aabb_.max.y,aabb_.max.z };
	localVertecies_[1].leftBottom = { aabb_.min.x,aabb_.min.y,aabb_.max.z };
	localVertecies_[1].rightBottom = { aabb_.max.x,aabb_.min.y,aabb_.max.z };
}

void OBB::Control(const char* keys, const char preKeys) {
	(void)keys, preKeys;
}
