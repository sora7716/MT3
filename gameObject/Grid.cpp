#include "Grid.h"
#include "gameObject/Camera.h"
#include "myMath/func/Aithmetic.h"

//初期化
void Grid::Initialize(Camera* camera) {
	camera_ = camera;//カメラのコピー
	//座標入れる箱のサイズを初期化
	xLine_.resize(kSubdivision + 1);
	zLine_.resize(kSubdivision + 1);
	//初期値を設定
	for (int i = 0;i < kSubdivision + 1;i++) {
		gridTransforms_.push_back({ { 1.0f,1.0f,1.0f }, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} });
		transform_ = gridTransforms_[i];
	}
}

//更新
void Grid::Update() {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const float kGridEvery = (kGridHalfWidth * 2.0f) / (float)kSubdivision;//1つ分の長さ

	for (uint32_t i = 0; i <= kSubdivision; i++) {
		//ローカルの開始位置
		xLine_[i].localStart.x = (-(float)kSubdivision / 2.0f + (float)i) * kGridEvery;
		xLine_[i].localStart.z = -kGridHalfWidth;

		//ローカルの終了位置
		xLine_[i].localEnd.x = (-(float)kSubdivision / 2.0f + (float)i) * kGridEvery;
		xLine_[i].localEnd.z = kGridHalfWidth;

		//スクリーン座標に変換
		xLine_[i].screenStart = BaseGameObject::ScreenTransform(camera_, xLine_[i].localStart);
		xLine_[i].screenEnd = BaseGameObject::ScreenTransform(camera_, xLine_[i].localEnd);

		//中心だけ黒
		xLine_[i].color = (i == kSubdivision / 2) ? BLUE : 0xFFFFFF88;
	}

	for (uint32_t i = 0; i <= kSubdivision; i++) {
		//ローカルの開始位置
		zLine_[i].localStart.z = (-(float)kSubdivision / 2.0f + (float)i) * kGridEvery;
		zLine_[i].localStart.x = -kGridHalfWidth;

		//ローカルの終了位置
		zLine_[i].localEnd.z = (-(float)kSubdivision / 2.0f + (float)i) * kGridEvery;
		zLine_[i].localEnd.x = kGridHalfWidth;

		//スクリーン座標に変換
		zLine_[i].screenStart = BaseGameObject::ScreenTransform(camera_, zLine_[i].localStart);
		zLine_[i].screenEnd = BaseGameObject::ScreenTransform(camera_, zLine_[i].localEnd);

		//中心だけ黒
		zLine_[i].color = (i == kSubdivision / 2) ? GREEN : 0xFFFFFF88;
	}
}

//描画
void Grid::Draw() {
	for (int i = 0;i <= kSubdivision;i++) {
		Novice::DrawLine(
			(int)xLine_[i].screenStart.x, (int)xLine_[i].screenStart.y,
			(int)xLine_[i].screenEnd.x, (int)xLine_[i].screenEnd.y,
			xLine_[i].color
		);
		Novice::DrawLine(
			(int)zLine_[i].screenStart.x, (int)zLine_[i].screenStart.y,
			(int)zLine_[i].screenEnd.x, (int)zLine_[i].screenEnd.y,
			zLine_[i].color
		);
	}
}
