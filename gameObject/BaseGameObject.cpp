#include "BaseGameObject.h"
#include "myMath/func/Rendering.h"

//ワールドビュープロジェクション
void BaseGameObject::MakeWvpMatrix(Camera* camera) {
	//ワールド座標系
	worldMatrix_ = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//wvpマトリックス
	worldViewProjectionMatrix_ = worldMatrix_ * camera->GetViewProjectionMatrix();
}

//スクリーン座標へ変換
Vector3 BaseGameObject::ScreenTransform(Camera* camera, const Vector3& local) {
	//wvpマトリックスを作成
	MakeWvpMatrix(camera);
	//正規化デバイス座標系に変換
	ndcVertex_ = Rendering::Transform(local, worldViewProjectionMatrix_);
	return Rendering::Transform(ndcVertex_, camera->GetViewportMatrix());
}

// ワールドビュープロジェクションのゲッター
const Matrix4x4& BaseGameObject::GetWorldViewProjection() {
	// TODO: return ステートメントをここに挿入します
	return worldViewProjectionMatrix_;
}

const Matrix4x4& BaseGameObject::GetWorldMatrix() {
	// TODO: return ステートメントをここに挿入します
	return worldMatrix_;
}

//ワールド座標のゲッター
const Vector3 BaseGameObject::GetWorldPosition() {
	return { worldMatrix_.m[3][0],worldMatrix_.m[3][1],worldMatrix_.m[3][2] };
}
