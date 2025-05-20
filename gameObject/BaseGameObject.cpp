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

//OBB空間で表示
void BaseGameObject::OBBWvpMatrix(const Camera* camera) {
	Vector3 orientation[3]{
	1,0,0,
	0,1,0,
	0,0,1, };
	Rendering::MakeOBBRotateMatrix(orientation, transform_.rotate);//回転行列から抽出
	worldMatrix_ = Rendering::MakeOBBWorldMatrix(orientation, transform_.translate);//抽出した値からワールドマトリックスを求める
	//wvpマトリックス
	worldViewProjectionMatrix_ = worldMatrix_ * camera->GetViewProjectionMatrix();
}

// OBB空間に変換
void BaseGameObject::OBBTransform(const Camera* camera, const Vector3& local, Vector3& screen) {
	//OBB空間に変換
	OBBWvpMatrix(camera);
	//正規化デバイス座標系
	ndcVertex_ = Rendering::Transform(local, worldViewProjectionMatrix_);
	//スクリーン座標
	screen = Rendering::Transform(ndcVertex_, camera->GetViewportMatrix());
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
