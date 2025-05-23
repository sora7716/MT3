#include "AABB.h"
using namespace std;

//初期化
void AABB::Initialize(Camera* camera, const AABBData&& aabb) {
	camera_ = camera;
	aabbData_ = aabb;
}

//更新
void AABB::Update() {
	//AABBのデータから面を求める
	localVertecies_[0].leftTop = { aabbData_.min.x,aabbData_.max.y,aabbData_.min.z };
	localVertecies_[0].rightTop = { aabbData_.max.x,aabbData_.max.y,aabbData_.min.z };
	localVertecies_[0].leftBottom = { aabbData_.min.x,aabbData_.min.y,aabbData_.min.z };
	localVertecies_[0].rightBottom = { aabbData_.max.x,aabbData_.min.y,aabbData_.min.z };

	localVertecies_[1].leftTop = { aabbData_.min.x,aabbData_.max.y,aabbData_.max.z };
	localVertecies_[1].rightTop = { aabbData_.max.x,aabbData_.max.y,aabbData_.max.z };
	localVertecies_[1].leftBottom = { aabbData_.min.x,aabbData_.min.y,aabbData_.max.z };
	localVertecies_[1].rightBottom = { aabbData_.max.x,aabbData_.min.y,aabbData_.max.z };

	//スクリーン座標に変換
	for (int i = 0; i < kAABB2DNum; i++) {
		screenVertecies_[i].leftTop = Rendering::Transform(Rendering::Transform(localVertecies_[i].leftTop, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		screenVertecies_[i].rightTop = Rendering::Transform(Rendering::Transform(localVertecies_[i].rightTop, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		screenVertecies_[i].leftBottom = Rendering::Transform(Rendering::Transform(localVertecies_[i].leftBottom, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		screenVertecies_[i].rightBottom = Rendering::Transform(Rendering::Transform(localVertecies_[i].rightBottom, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
	}

}

//衝突したときの判定
void AABB::OnCollision() {
	if (aabbData_.isHit) {
		SetColor(RED);
	} else {
		SetColor(WHITE);
	}
}

//デバックテキスト
void AABB::DebugText(const char* label) {
#ifdef _DEBUG
	string minMoji = string(label) + ".min";
	ImGui::SliderFloat3(minMoji.c_str(), &aabbData_.min.x, 0.0f, 3.0f);

	string maxMoji = string(label) + ".max";
	ImGui::SliderFloat3(maxMoji.c_str(), &aabbData_.max.x, 0.0f, 3.0f);
#endif // _DEBUG
}


//描画
void AABB::Draw()const {
	//正面と背面の生成
	for (int i = 0; i < kAABB2DNum; i++) {
		Novice::DrawLine((int)screenVertecies_[i].leftTop.x, (int)screenVertecies_[i].leftTop.y, (int)screenVertecies_[i].rightTop.x, (int)screenVertecies_[i].rightTop.y, aabbData_.color);
		Novice::DrawLine((int)screenVertecies_[i].rightTop.x, (int)screenVertecies_[i].rightTop.y, (int)screenVertecies_[i].rightBottom.x, (int)screenVertecies_[i].rightBottom.y, aabbData_.color);
		Novice::DrawLine((int)screenVertecies_[i].rightBottom.x, (int)screenVertecies_[i].rightBottom.y, (int)screenVertecies_[i].leftBottom.x, (int)screenVertecies_[i].leftBottom.y, aabbData_.color);
		Novice::DrawLine((int)screenVertecies_[i].leftBottom.x, (int)screenVertecies_[i].leftBottom.y, (int)screenVertecies_[i].leftTop.x, (int)screenVertecies_[i].leftTop.y, aabbData_.color);
	}
	//側面の生成
	Novice::DrawLine((int)screenVertecies_[0].leftTop.x, (int)screenVertecies_[0].leftTop.y, (int)screenVertecies_[1].leftTop.x, (int)screenVertecies_[1].leftTop.y, aabbData_.color);
	Novice::DrawLine((int)screenVertecies_[0].rightTop.x, (int)screenVertecies_[0].rightTop.y, (int)screenVertecies_[1].rightTop.x, (int)screenVertecies_[1].rightTop.y, aabbData_.color);
	Novice::DrawLine((int)screenVertecies_[0].leftBottom.x, (int)screenVertecies_[0].leftBottom.y, (int)screenVertecies_[1].leftBottom.x, (int)screenVertecies_[1].leftBottom.y, aabbData_.color);
	Novice::DrawLine((int)screenVertecies_[0].rightBottom.x, (int)screenVertecies_[0].rightBottom.y, (int)screenVertecies_[1].rightBottom.x, (int)screenVertecies_[1].rightBottom.y, aabbData_.color);
}

// 衝突したかどうかのセッター
void AABB::SetIsHit(bool isHit) {
	aabbData_.isHit = isHit;
}

//色のセッター
void AABB::SetColor(uint32_t color) {
	aabbData_.color = color;
}

//ローカル座標のセッター
void AABB::SetLocalVertex2D(RectVertex local) {
	localVertecies_[0] = local;
	localVertecies_[1] = local;
}

//aabbデータのセッター
void AABB::SetAABB(const AABBData& aabbData) {
	aabbData_ = aabbData;
}

//AABBのデータのゲッター
AABBData AABB::GetAABBMaterial() const {
	return aabbData_;
}

