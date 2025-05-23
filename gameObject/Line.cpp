#include "Line.h"
#include "gameObject/Camera.h"
#include <algorithm>
#include <string>
using namespace std;

//コンストラクタ
Line::Line() {
	t_ = {};
	project_ = {};
	closestPoint_ = {};
	point_ = {};
	segment_ = {};
	camera_ = nullptr;
	color_ = 0u;
	isHit_ = false;
}

//デストラクタ
Line::~Line() {

}

//初期化
void Line::Initialize(Camera* camera, const Segment&& segment) {
	camera_ = camera;
	point_ = {};
	segment_ = segment;
	color_ = WHITE;
	isHit_ = false;
}

//線分の更新
void Line::UpdateSegment() {
	Project(point_ - segment_.origin, segment_.diff);
	ClosestPoint();
}

/// <summary>
/// デバックテキスト
/// </summary>
void Line::DebugText(const char* name) {
#ifdef _DEBUG
	string originLabel = string(name) + ".origin";
	ImGui::DragFloat3(originLabel.c_str(), &segment_.origin.x, 0.1f);
	string diffLabel = string(name) + ".diff";
	ImGui::DragFloat3(diffLabel.c_str(), &segment_.diff.x, 0.1f);
	//ImGui::DragFloat3("bezierPints[0]", &bezierPoints_[0].x, 0.1f);
	//ImGui::DragFloat3("bezierPints[1]", &bezierPoints_[1].x, 0.1f);
	//ImGui::DragFloat3("bezierPints[2]", &bezierPoints_[2].x, 0.1f);
	//ImGui::DragFloat3("point", &point_.x,0.1f);
	//ImGui::InputFloat3("Project", &project_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	//ImGui::InputFloat3("cp", &closestPoint_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	//ImGui::DragFloat3("catmullRomPoint[0]", &catmullRomPoints_[0].x, 0.1f);
	//ImGui::DragFloat3("catmullRomPoint[1]", &catmullRomPoints_[1].x, 0.1f);
	//ImGui::DragFloat3("catmullRomPoint[2]", &catmullRomPoints_[2].x, 0.1f);
	//ImGui::DragFloat3("catmullRomPoint[3]", &catmullRomPoints_[3].x, 0.1f);
#endif // _DEBUG
}

//線分の描画
void Line::DrawSegment() {
	Vector3 start{};
	Vector3 end{};
	start = Rendering::Transform(Rendering::Transform(segment_.origin, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
	end = Rendering::Transform(Rendering::Transform((segment_.origin + segment_.diff), camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
	Novice::DrawLine(
		(int)start.x, (int)start.y,
		(int)end.x, (int)end.y,
		color_
	);
}

//ベジエ曲線の描画
void Line::DrawBezier() {
	for (int i = 0; i < kDivision; i++) {
		//フレームを分割
		float frame[2] = { static_cast<float>(i) / kDivision,static_cast<float>(i + 1) / kDivision };
		//ベジエ曲線
		Vector3 bezier[2];
		for (int j = 0; j < 2; j++) {
			bezier[j] = Math::Bezier(bezierPoints_, frame[j]);
		}

		//スクリーン座標に変換
		for (int k = 0;k < 2;k++) {
			bezier[k] = Rendering::Transform(Rendering::Transform(bezier[k], camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		}

		//描画
		Novice::DrawLine(
			(int)bezier[0].x, (int)bezier[0].y,
			(int)bezier[1].x, (int)bezier[1].y,
			color_
		);
	}
}

// スプライン曲線
void Line::DrawCatmullRom() {
	for (int i = 0; i < kDivision; i++) {
		//フレームを分割
		float frame[2] = { static_cast<float>(i) / kDivision,static_cast<float>(i + 1) / kDivision };
		//スプライト曲線曲線
		Vector3 catmullRom[2];
		for (int j = 0; j < 2; j++) {
			catmullRom[j] = Math::CatmullRomPosition(catmullRomPoints_, frame[j]);
		}

		//スクリーン座標に変換
		for (int k = 0;k < 2;k++) {
			catmullRom[k] = Rendering::Transform(Rendering::Transform(catmullRom[k], camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		}

		//描画
		Novice::DrawLine(
			(int)catmullRom[0].x, (int)catmullRom[0].y,
			(int)catmullRom[1].x, (int)catmullRom[1].y,
			color_
		);
	}
}

// オブジェクトの通った道を表示する
void Line::DrawObjectRoad(const Vector3& pos) {
	// 軌跡を保存するための可変長配列
	static std::vector<Vector3> path;
	path.push_back(pos); // sphereの現在位置を保存
	// 保存した軌跡を線でつなげる
	for (size_t i = 1; i < path.size(); i++) {
		Vector3 start = path[i - 1];
		Vector3 end = path[i];

		// スクリーン座標に変換
		start = Rendering::Transform(Rendering::Transform(start, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());
		end = Rendering::Transform(Rendering::Transform(end, camera_->GetViewProjectionMatrix()), camera_->GetViewportMatrix());

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(start.x), static_cast<int>(start.y),
			static_cast<int>(end.x), static_cast<int>(end.y),
			WHITE
		);
	}

	// 軌跡のサイズを制限（必要であれば）
	const size_t maxPathSize = 1000; // 軌跡の最大保存数(分割数)
	if (path.size() > maxPathSize) {
		path.erase(path.begin()); // 最も古い位置を削除
	}
}

//衝突した場合の判定
void Line::OnCollision(bool isHit) {
	isHit_ = isHit;
	if (isHit_) {
		SetColor(RED);
	} else {
		SetColor(WHITE);
	}
}

//最近接点のゲッター
Vector3 Line::GetClosestPoint()const {
	return closestPoint_;
}

//原点のセッター
Vector3 Line::GetPoint()const {
	return point_;
}

//線分のゲッター
const Segment& Line::GetSegment() const {
	// TODO: return ステートメントをここに挿入します
	return segment_;
}

//正射影ベクトルのゲッター
Vector3 Line::GetProject() const {
	return project_;
}

//媒介変数のゲッター
float Line::GetT() const {
	return t_;
}

//原点のセッター
void Line::SetPoint(const Vector3& point) {
	point_ = point;
}

//線分のセッター
void Line::SetSegment(const Segment& segment) {
	segment_ = segment;
}

//色のセッター
void Line::SetColor(uint32_t color) {
	color_ = color;
}

//衝突判定のセッター
void Line::SetIsHit(bool isHit) {
	isHit_ = isHit;
}

// ベジエ曲線の制御点のセッター
void Line::SetBezierPoints(Vector3* controlPoints) {
	for (int i = 0; i < 3; i++) {
		bezierPoints_[i] = controlPoints[i];
	}
}

// ベジエ曲線の制御点のゲッター
Vector3* Line::GetBezierPoints() {
	return bezierPoints_;
}

// スプライン曲線の制御点のセッター
void Line::SetCatmullRomPoints(std::vector<Vector3> controlPoints) {
	catmullRomPoints_ = controlPoints;
}

// スプライン曲線の制御点のゲッター
std::vector<Vector3> Line::GetCatmullRomPoints() {
	return catmullRomPoints_;
}

//正射影ベクトル
void Line::Project(const Vector3& v1, const Vector3& v2) {
	float dot1 = Math::Dot(v1, v2);
	float length = Math::Dot(v2, v2);
	project_ = dot1 / length * v2;
	t_ = dot1 / length;
}

// 最近接点
void Line::ClosestPoint() {
	t_ = clamp(t_, 0.0f, 1.0f);
	closestPoint_ = segment_.origin + segment_.diff * t_;
	//closestPoint_ = segment_.origin + project_;
}

