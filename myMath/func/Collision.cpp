#define NOMINMAX
#include "Collision.h"
//#include "gameObject/OBB/OBB.h"
//#include "gameObject/hexagon/Hexagon.h"
//#include "gameObject/capsule/Capsule.h"
//#include "gameObject/triangle/Triangle.h"
#include <algorithm> 
using namespace std;

//インスタンスのゲッター
Collision* Collision::GetInstance() {
	//関数内にstatic変数として宣言する
	static Collision instance;

	return &instance;
}

//球同士
bool Collision::IsCollision(const GameObject::SphereMaterial& sphere1, const GameObject::SphereMaterial& sphere2) {
	float distance = Math::Length(sphere1.center - sphere2.center);
	return distance <= sphere1.radius + sphere2.radius;
}

////球と平面
//bool Collision::IsCollision(const GameObject::SphereMaterial& sphere, const GameObject::PlaneMaterial& plane) {
//	Vector3 normal = plane.normal;
//	float distance = plane.distance;
//	float k = fabs(Math::Dot(normal, sphere.center) - distance);
//
//	return k <= sphere.radius;
//}
//
////線と平面
//bool Collision::IsCollision(const GameObject::Segment& segment, const GameObject::PlaneMaterial& plane) {
//	//垂直判定を行うための、法線と線の内積を求める
//	float dot = Math::Dot(plane.normal, segment.diff);
//	if (std::abs(dot) < 1e-6f) {
//		return false;
//	}
//
//	float t = (plane.distance - Math::Dot(segment.origin, plane.normal)) / dot;
//	if (t >= 0.0f && t <= 1.0f) {
//		return true;
//	}
//	return false;
//}
//
//// 線と三角形
//bool Collision::IsCollision(const GameObject::Segment& segment, const GameObject::TriangleMaterial& triangle) {
//
//	Vector3 v01 = triangle.kLocalVertices_[1] - triangle.kLocalVertices_[0];
//	Vector3 v12 = triangle.kLocalVertices_[2] - triangle.kLocalVertices_[1];
//
//	Vector3 normal = Math::Normalize(Math::Cross(v01, v12));
//	float dot = Math::Dot(normal, segment.diff);
//	float distance = Math::Dot(triangle.kLocalVertices_[0], normal);
//	float t = (distance - Math::Dot(segment.origin, normal)) / dot;
//
//	Vector3 intersect = segment.origin + t * segment.diff;
//	Vector3 v1p = intersect - triangle.kLocalVertices_[1];
//	Vector3 v2p = intersect - triangle.kLocalVertices_[2];
//	Vector3 v0p = intersect - triangle.kLocalVertices_[0];
//	Vector3 v20 = triangle.kLocalVertices_[0] - triangle.kLocalVertices_[2];
//
//	//衝突したかどうかのフラグ
//	bool isHit = false;
//
//	if (std::abs(dot) < 1e-6f) {
//		isHit = false;
//	}
//	if (t >= kTMin && t <= kTMax) {
//		isHit = true;
//	}
//
//	if (Math::Dot(Math::Cross(v01, v1p), normal) < 0.0f) {
//		isHit = false;
//	}
//	else if (Math::Dot(Math::Cross(v12, v2p), normal) < 0.0f) {
//		isHit = false;
//	}
//	else if (Math::Dot(Math::Cross(v20, v0p), normal) < 0.0f) {
//		isHit = false;
//	}
//	return isHit;
//}
//
//// AABB同士
//bool Collision::IsCollision(GameObject::AABBMaterial aabb1, GameObject::AABBMaterial aabb2) {
//	//X座標の当たってない判定
//	if (aabb1.min.x < aabb2.max.x && aabb1.max.x < aabb2.min.x) {
//		return false;
//	}
//	else if (aabb2.max.x < aabb1.min.x && aabb2.min.x < aabb1.max.x) {
//		return false;
//	}
//
//	// Y座標の当たってない判定
//	else if (aabb1.max.y > aabb2.min.y && aabb1.min.y > aabb2.max.y) {
//		return false;
//	}
//	else if (aabb2.max.y > aabb1.min.y && aabb2.min.y > aabb1.max.y) {
//		return false;
//	}
//
//	// Z座標の当たってない判定
//	else if (aabb1.min.z < aabb2.max.z && aabb1.max.z < aabb2.min.z) {
//		return false;
//	}
//	else if (aabb2.max.z < aabb1.min.z && aabb2.min.z < aabb1.max.z) {
//		return false;
//	}
//
//	return true;
//}
//
//// AABBと球
//bool Collision::IsCollision(GameObject::AABBMaterial& aabb, const GameObject::SphereMaterial& sphere) {
//	Vector3 closestPoint = {
//		clamp(sphere.center.x,aabb.min.x,aabb.max.x),
//		clamp(sphere.center.y,aabb.min.y,aabb.max.y),
//		clamp(sphere.center.z,aabb.min.z,aabb.max.z),
//	};
//	//最近接点と球の中心との距離を求める
//	float distance = Math::Length(closestPoint - sphere.center);
//	//球の半径より上記の距離が小さかったら衝突
//	return distance <= sphere.radius;
//}
//
//// AABBと線分
//bool Collision::IsCollision(const GameObject::AABBMaterial& aabb, const GameObject::Segment& segment) {
//	GameObject::AABBMaterial t;
//	Vector3 tNear;
//	Vector3 tFar;
//
//	t.min.x = (aabb.min.x - segment.origin.x) / segment.diff.x;
//	t.max.x = (aabb.max.x - segment.origin.x) / segment.diff.x;
//
//	t.min.y = (aabb.min.y - segment.origin.y) / segment.diff.y;
//	t.max.y = (aabb.max.y - segment.origin.y) / segment.diff.y;
//
//	t.min.z = (aabb.min.z - segment.origin.z) / segment.diff.z;
//	t.max.z = (aabb.max.z - segment.origin.z) / segment.diff.z;
//
//	tNear.x = min(t.min.x, t.max.x);
//	tFar.x = max(t.min.x, t.max.x);
//
//	tNear.y = min(t.min.y, t.max.y);
//	tFar.y = max(t.min.y, t.max.y);
//
//	tNear.z = min(t.min.z, t.max.z);
//	tFar.z = max(t.min.z, t.max.z);
//
//	float tMin = max(tNear.x, max(tNear.z, tNear.y));
//	float tMax = min(tFar.x, min(tFar.z, tFar.y));
//	bool isHit = false;
//	if (tMin <= tMax) {
//		if (tMin * tMax < 0.0f) {
//			isHit = true;
//		}
//		if (kTMin <= tMin && tMin <= kTMax || kTMin <= tMax && tMax <= kTMax) {
//			isHit = true;
//		}
//	}
//	else {
//		isHit = false;
//	}
//	return isHit;
//}
//
//// 衝突判定(AABBと平面)
//bool Collision::IsCollision(const GameObject::AABBMaterial& aabb, const GameObject::PlaneMaterial& plane) {
//	Vector3 normal = plane.normal;
//	float distance = plane.distance;
//
//	// AABBの8つの頂点を計算
//	Vector3 vertices[8] = {
//		aabb.min,
//		Vector3(aabb.min.x, aabb.min.y, aabb.max.z),
//		Vector3(aabb.min.x, aabb.max.y, aabb.min.z),
//		Vector3(aabb.min.x, aabb.max.y, aabb.max.z),
//		Vector3(aabb.max.x, aabb.min.y, aabb.min.z),
//		Vector3(aabb.max.x, aabb.min.y, aabb.max.z),
//		Vector3(aabb.max.x, aabb.max.y, aabb.min.z),
//		aabb.max
//	};
//
//	// 平面の方程式を使って、各頂点が平面のどちら側にあるかを判定
//	bool allPositive = true;
//	bool allNegative = true;
//
//	for (int i = 0; i < 8; i++) {
//		float dotProduct = Math::Dot(normal, vertices[i]) - distance;
//
//		if (dotProduct > 0) {
//			allNegative = false;
//		}
//		if (dotProduct < 0) {
//			allPositive = false;
//		}
//	}
//
//	// 全ての頂点が平面の同じ側にあれば衝突していないと判断
//	if (allPositive || allNegative) {
//		return false;
//	}
//
//	return true;
//
//}
//
//// OBBと球
//bool Collision::IsCollision(OBB* obb, const GameObject::SphereMaterial& sphere) {
//	Vector3 centerInOBBLocalSpace = Math::Transform(sphere.center, obb->GetOBBWorldMatrixInvers());
//	GameObject::AABBMaterial aabbOBBLocal = { .min = -obb->GetSize(),.max = obb->GetSize() };
//	GameObject::SphereMaterial sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };
//	//ローカル空間で衝突判定
//	return IsCollision(aabbOBBLocal, sphereOBBLocal);
//}
//
//// OBBと線分の当たり判定
//bool Collision::IsCollision(OBB* obb, const GameObject::Segment segment) {
//	Vector3 localSegmentOrigin = Math::Transform(segment.origin, obb->GetOBBWorldMatrixInvers());
//	Vector3 localSegmentEnd = Math::Transform(segment.origin + segment.diff, obb->GetOBBWorldMatrixInvers());
//	GameObject::AABBMaterial localAABB{
//		.min = -obb->GetSize(),
//			.max = obb->GetSize(),
//	};
//	GameObject::Segment localSegment;
//	localSegment.origin = localSegmentOrigin;
//	localSegment.diff = localSegmentEnd - localSegmentOrigin;
//	return IsCollision(localAABB, localSegment);
//}
//
//// OBBとAABB/OBB
//bool Collision::IsCollision(OBB* obb1, OBB* obb2) {
//	//分割軸の数
//	Vector3 separateAxes[15];
//	//面の法線
//	/*ターゲット*/
//	separateAxes[0] = obb1->GetOBBMaterial().orientations[0];
//	separateAxes[1] = obb1->GetOBBMaterial().orientations[1];
//	separateAxes[2] = obb1->GetOBBMaterial().orientations[2];
//	/*ターゲットではないほう*/
//	separateAxes[3] = obb2->GetOBBMaterial().orientations[0];
//	separateAxes[4] = obb2->GetOBBMaterial().orientations[1];
//	separateAxes[5] = obb2->GetOBBMaterial().orientations[2];
//	//9つの辺のクロス積
//	separateAxes[6] = Math::Cross(obb1->GetOBBMaterial().orientations[0], obb2->GetOBBMaterial().orientations[0]);
//	separateAxes[7] = Math::Cross(obb1->GetOBBMaterial().orientations[0], obb2->GetOBBMaterial().orientations[1]);
//	separateAxes[8] = Math::Cross(obb1->GetOBBMaterial().orientations[0], obb2->GetOBBMaterial().orientations[2]);
//	separateAxes[9] = Math::Cross(obb1->GetOBBMaterial().orientations[1], obb2->GetOBBMaterial().orientations[0]);
//	separateAxes[10] = Math::Cross(obb1->GetOBBMaterial().orientations[1], obb2->GetOBBMaterial().orientations[1]);
//	separateAxes[11] = Math::Cross(obb1->GetOBBMaterial().orientations[1], obb2->GetOBBMaterial().orientations[2]);
//	separateAxes[12] = Math::Cross(obb1->GetOBBMaterial().orientations[2], obb2->GetOBBMaterial().orientations[0]);
//	separateAxes[13] = Math::Cross(obb1->GetOBBMaterial().orientations[2], obb2->GetOBBMaterial().orientations[1]);
//	separateAxes[14] = Math::Cross(obb1->GetOBBMaterial().orientations[2], obb2->GetOBBMaterial().orientations[2]);
//
//	//半分のベクトル
//	/*ターゲット*/
//	Vector3 targetDirection[3]{
//		obb1->GetOBBMaterial().orientations[0] * obb1->GetOBBMaterial().size.x,
//		obb1->GetOBBMaterial().orientations[1] * obb1->GetOBBMaterial().size.y,
//		obb1->GetOBBMaterial().orientations[2] * obb1->GetOBBMaterial().size.z,
//	};
//	/*OBB*/
//	Vector3 obbDirection[3]{
//		obb2->GetOBBMaterial().orientations[0] * obb2->GetOBBMaterial().size.x,
//		obb2->GetOBBMaterial().orientations[1] * obb2->GetOBBMaterial().size.y,
//		obb2->GetOBBMaterial().orientations[2] * obb2->GetOBBMaterial().size.z,
//	};
//
//	//頂点の数
//	const int kCornerNum = 8;
//
//	// 点(頂点)
//	Vector3 targetCorners[kCornerNum] = {
//	  obb1->GetOBBMaterial().center + targetDirection[0] + targetDirection[1] + targetDirection[2],//背面の右上
//	  obb1->GetOBBMaterial().center + targetDirection[0] + targetDirection[1] - targetDirection[2],//正面の右上
//	  obb1->GetOBBMaterial().center + targetDirection[0] - targetDirection[1] + targetDirection[2],//背面の右下
//	  obb1->GetOBBMaterial().center + targetDirection[0] - targetDirection[1] - targetDirection[2],//正面の右下
//	  obb1->GetOBBMaterial().center - targetDirection[0] + targetDirection[1] + targetDirection[2],//背面の左上
//	  obb1->GetOBBMaterial().center - targetDirection[0] + targetDirection[1] - targetDirection[2],//正面の左上
//	  obb1->GetOBBMaterial().center - targetDirection[0] - targetDirection[1] + targetDirection[2],//背面の左下
//	  obb1->GetOBBMaterial().center - targetDirection[0] - targetDirection[1] - targetDirection[2],//正面の左下
//	};
//	Vector3 obbCorners[kCornerNum] = {
//	 obb2->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の右上
//	 obb2->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の右上
//	 obb2->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の右下
//	 obb2->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の右下
//	 obb2->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の左上
//	 obb2->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の左上
//	 obb2->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の左下
//	 obb2->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の左下
//	};
//	// 中心点間のベクトル
//	Vector3 centerToCenter = obb1->GetOBBMaterial().center - obb2->GetOBBMaterial().center;
//
//	//当たったかの判定
//	bool isHit = true;
//
//	//当たり判定の計算
//	for (const auto& separateAxis : separateAxes) {
//		float minTarget = (numeric_limits<float>::max)();
//		float maxTarget = (numeric_limits<float>::lowest)();
//		float minOBB = minTarget;
//		float maxOBB = maxTarget;
//		for (auto cornerIndex = 0; cornerIndex < kCornerNum; cornerIndex++) {
//			float targetDistance = Math::Dot(targetCorners[cornerIndex], separateAxis);
//			minTarget = (min)(targetDistance, minTarget);
//			maxTarget = (max)(targetDistance, maxTarget);
//			float obbDistance = Math::Dot(obbCorners[cornerIndex], separateAxis);
//			minOBB = (min)(obbDistance, minOBB);
//			maxOBB = (max)(obbDistance, maxOBB);
//		}
//		//それぞれを射影した範囲長の合計を求める
//		float sumSpan = maxTarget - minTarget + maxOBB - minOBB;
//		//最大範囲を求める
//		float longSpan = (max)(maxTarget, maxOBB) - (min)(minTarget, minOBB);
//		//分離軸が見つかる判定
//		if (sumSpan < longSpan) {
//			isHit = false;
//		}
//	}
//	//当たり判定を返す
//	return isHit;
//}
////衝突判定(OBBと平面)
//bool Collision::IsCollision(OBB* obb, const GameObject::PlaneMaterial& plane) {
//	Vector3 centerInOBBLocalSpace = Math::Transform(plane.normal * plane.distance, obb->GetOBBWorldMatrixInvers());
//	GameObject::AABBMaterial aabbOBBLocal = { .min = -obb->GetSize(),.max = obb->GetSize() };
//	GameObject::PlaneMaterial planeOBBLocal{ centerInOBBLocalSpace, plane.distance };
//	//ローカル空間で衝突判定
//	return IsCollision(aabbOBBLocal, planeOBBLocal);
//}
//
////六角形と線分
//bool Collision::IsCollision(Hexagon* hexagon, Line* line, int surface) {
//	// 六角形の各辺
//	Vector3 edge[6]{
//		hexagon->GetLocalVertex(surface)[1] - hexagon->GetLocalVertex(surface)[0],
//		hexagon->GetLocalVertex(surface)[2] - hexagon->GetLocalVertex(surface)[1],
//		hexagon->GetLocalVertex(surface)[3] - hexagon->GetLocalVertex(surface)[2],
//		hexagon->GetLocalVertex(surface)[4] - hexagon->GetLocalVertex(surface)[3],
//		hexagon->GetLocalVertex(surface)[5] - hexagon->GetLocalVertex(surface)[4],
//		hexagon->GetLocalVertex(surface)[0] - hexagon->GetLocalVertex(surface)[5],
//	};
//
//	//衝突したかどうか
//	bool isHit = false;
//
//	//六角形の面の法線
//	Vector3 normal = Math::Normalize(Math::Cross(edge[0], edge[1]));
//
//
//	//疑似的に平面を作成
//	GameObject::PlaneMaterial plane = { .normal = normal,.distance = Math::Dot(hexagon->GetLocalVertex(surface)[0],normal) };
//
//	//平面の法線とラインの差分が向き合っているかどうか
//	float dot = Math::Dot(plane.normal, line->GetSegment().diff);
//
//	//平行だったらtrueを返す
//	if (std::abs(dot) < 1e-6f) {
//		isHit = false;
//	}
//
//	//平面を求める
//	float t = (plane.distance - Math::Dot(line->GetSegment().origin, plane.normal)) / dot;
//
//	//六角形の各辺に重なっているかの計算
//	Vector3 intersect = line->GetSegment().origin + t * line->GetSegment().diff;
//	Vector3 v0p = intersect - hexagon->GetLocalVertex(0)[0];
//	Vector3 v1p = intersect - hexagon->GetLocalVertex(0)[1];
//	Vector3 v2p = intersect - hexagon->GetLocalVertex(0)[2];
//	Vector3 v3p = intersect - hexagon->GetLocalVertex(0)[3];
//	Vector3 v4p = intersect - hexagon->GetLocalVertex(0)[4];
//	Vector3 v5p = intersect - hexagon->GetLocalVertex(0)[5];
//
//
//	//平面の中にある六角形に線が当たっているかの判定
//	if (t >= kTMin && t <= kTMax) {
//
//		if (Math::Dot(Math::Cross(edge[0], v0p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else if (Math::Dot(Math::Cross(edge[1], v1p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else if (Math::Dot(Math::Cross(edge[2], v2p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else if (Math::Dot(Math::Cross(edge[3], v3p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else if (Math::Dot(Math::Cross(edge[4], v4p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else if (Math::Dot(Math::Cross(edge[5], v5p), normal) < 0.0f) {
//			isHit = false;
//		}
//		else {
//			isHit = true;
//		}
//	}
//
//	return isHit;
//}
//
//// カプセルと平面
//bool Collision::IsCollision(const GameObject::CapsuleMaterial& capsule, const GameObject::PlaneMaterial& plane) {
//	// カプセルの始点と終点
//	Vector3 capsuleStart = capsule.segment.origin;
//	Vector3 capsuleEnd = capsuleStart + capsule.segment.diff;
//
//	//平面上の任意の点(法線ベクトル*平面のオフセット距離)
//	Vector3 planeCenter = plane.normal * plane.distance;
//
//	//平面上の法線
//	Vector3 normal = plane.normal;
//
//	//平面の中心座標とカプセルの始点との距離
//	Vector3 d = planeCenter - capsuleStart;
//
//	//カプセルの始点から終点の差分ベクトル
//	Vector3 ba = capsuleEnd - capsuleStart;
//
//	//長さを求める
//	float len = Math::Length(ba);
//
//	//正規化する
//	Vector3 e = Math::Normalize(ba);
//
//	//内積を求める
//	float dot = Math::Dot(d, e);
//	float t = dot / len;
//
//	//クランプ
//	t = clamp(t, 0.0f, 1.0f);
//
//	//線形補完する
//	Vector3 f = Math::Lerp(capsuleStart, capsuleEnd, t);
//
//	//カプセルの半径と距離を比較
//	float distance = abs(Math::Dot(normal, f) - plane.distance);
//
//	//距離がカプセルの半径以下なら衝突
//	return distance <= capsule.radius;
//}
//
////衝突判定(六角形とOBB)
//bool Collision::IsCollision(Hexagon* hexagon, OBB* obb) {
//	//分割軸の数
//	std::vector<Vector3> separateAxes;
//	separateAxes.resize(19);
//
//	//面の法線
//	Vector3 normal[4];
//	normal[0] = hexagon->GetHexagonMaterial().normal[0];
//	normal[1] = hexagon->GetHexagonMaterial().normal[1];
//	normal[2] = hexagon->GetHexagonMaterial().normal[2];
//	normal[3] = hexagon->GetHexagonMaterial().normal[3];
//
//	//六角柱の面の法線
//	separateAxes[0] = normal[0];
//	separateAxes[1] = normal[1];
//	separateAxes[2] = normal[2];
//	separateAxes[3] = normal[3];
//	//OBBの面の法線
//	separateAxes[4] = obb->GetOBBMaterial().orientations[0];
//	separateAxes[5] = obb->GetOBBMaterial().orientations[1];
//	separateAxes[6] = obb->GetOBBMaterial().orientations[2];
//	//クロス積で求める
//	separateAxes[7] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[0]);
//	separateAxes[8] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[1]);
//	separateAxes[9] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[2]);
//	separateAxes[10] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[3]);
//	separateAxes[11] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[0]);
//	separateAxes[12] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[1]);
//	separateAxes[13] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[2]);
//	separateAxes[14] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[3]);
//	separateAxes[15] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[0]);
//	separateAxes[16] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[1]);
//	separateAxes[17] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[2]);
//	separateAxes[18] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[3]);
//
//	//半分のベクトル
//	/*obb*/
//	Vector3 obbDirection[3]{
//		obb->GetOBBMaterial().orientations[0] * obb->GetOBBMaterial().size.x,
//		obb->GetOBBMaterial().orientations[1] * obb->GetOBBMaterial().size.y,
//		obb->GetOBBMaterial().orientations[2] * obb->GetOBBMaterial().size.z,
//	};
//
//	//頂点の数
//	const int kOBBCornerNum = 8;
//	const int kHexagonCornerNum = 12;
//
//	// 点(頂点)
//	Vector3 obbCorners[kOBBCornerNum] = {
//	  obb->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の右上
//	  obb->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の右上
//	  obb->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の右下
//	  obb->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の右下
//	  obb->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の左上
//	  obb->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の左上
//	  obb->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の左下
//	  obb->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の左下
//	};
//	Vector3 hexagonConers[kHexagonCornerNum];
//
//	// 半径と高さ
//	Vector3 size = hexagon->GetHexagonMaterial().size;
//
//	// 六角形の頂点計算
//	for (int i = 0; i < 6; i++) {
//		float theta = 60.0f * static_cast<float>(i);
//		float angle = theta * rad;
//		float x = size.x * cos(angle);
//		float z = size.z * sin(angle);
//
//		// 上面の頂点 (z = +height)
//		hexagonConers[i] = Math::Transform(Vector3(x, -size.y, z), Math::MakeRotateXYZMatrix(hexagon->GetRotate()) * Math::MakeTranslateMatrix(hexagon->GetHexagonMaterial().center));
//
//		// 上面の頂点 (z = +height)
//		hexagonConers[i + 6] = Math::Transform(Vector3(x, size.y, z), Math::MakeRotateXYZMatrix(hexagon->GetRotate()) * Math::MakeTranslateMatrix(hexagon->GetHexagonMaterial().center));
//	}
//
//	//分離軸を増やす(六角形を回転させると高さが合わなくなるから)
//	for (int i = 0; i < 6; i++) {
//		int next = (i + 1) % 6;
//		// 六角柱の辺
//		Vector3 edgeTop = hexagonConers[next] - hexagonConers[i];
//		Vector3 edgeBottom = hexagonConers[next + 6] - hexagonConers[i + 6];
//		// OBB の辺
//		Vector3 obbEdges[3] = {
//			obbDirection[0], obbDirection[1], obbDirection[2]
//		};
//
//		// 辺同士のクロス積を分離軸に追加
//		for (int j = 0; j < 3; j++) {
//			Vector3 crossAxisTop = Math::Cross(edgeTop, obbEdges[j]);
//			separateAxes.push_back(Math::Normalize(crossAxisTop));
//			Vector3 crossAxisBottom = Math::Cross(edgeBottom, obbEdges[j]);
//			separateAxes.push_back(Math::Normalize(crossAxisBottom));
//		}
//	}
//
//	// 中心点間のベクトル
//	Vector3 centerToCenter = hexagon->GetHexagonMaterial().center - obb->GetOBBMaterial().center;
//
//	//当たったかの判定
//	bool isHit = true;
//
//	//当たり判定の計算
//	for (const auto& separateAxis : separateAxes) {
//		float minOBB = (numeric_limits<float>::max)();
//		float maxOBB = (numeric_limits<float>::lowest)();
//		float minHexagon = minOBB;
//		float maxHexagon = maxOBB;
//		for (auto obbCornerIndex = 0; obbCornerIndex < kOBBCornerNum; obbCornerIndex++) {
//			float obbDistance = Math::Dot(obbCorners[obbCornerIndex], separateAxis);
//			minOBB = (min)(obbDistance, minOBB);
//			maxOBB = (max)(obbDistance, maxOBB);
//		}
//		for (auto hexagonCornerIndex = 0; hexagonCornerIndex < kHexagonCornerNum; hexagonCornerIndex++) {
//			float hexagonDistance = Math::Dot(hexagonConers[hexagonCornerIndex], separateAxis);
//			minHexagon = (min)(hexagonDistance, minHexagon);
//			maxHexagon = (max)(hexagonDistance, maxHexagon);
//		}
//
//		//それぞれを射影した範囲長の合計を求める
//		float sumSpan = maxOBB - minOBB + maxHexagon - minHexagon;
//		//最大範囲を求める
//		float longSpan = (max)(maxOBB, maxHexagon) - (min)(minOBB, minHexagon);
//		if (sumSpan < longSpan) {
//			isHit = false;
//		}
//	}
//
//	return isHit;
//
//}
//
//bool Collision::IsCollision(Triangle* triangle, OBB* obb) {
//	//分割軸の数
//	Vector3 separateAxes[19];
//
//	//面の法線
//	Vector3 normal[4];
//	normal[0] = triangle->GetTriangleMaterial().normal[0];
//	normal[1] = triangle->GetTriangleMaterial().normal[1];
//	normal[2] = triangle->GetTriangleMaterial().normal[2];
//	normal[3] = triangle->GetTriangleMaterial().normal[3];
//
//	//三角柱の面の法線
//	separateAxes[0] = normal[0];
//	separateAxes[1] = normal[1];
//	separateAxes[2] = normal[2];
//	separateAxes[3] = normal[3];
//	//OBBの面の法線
//	separateAxes[4] = obb->GetOBBMaterial().orientations[0];
//	separateAxes[5] = obb->GetOBBMaterial().orientations[1];
//	separateAxes[6] = obb->GetOBBMaterial().orientations[2];
//	//クロス積で求める
//	separateAxes[7] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[0]);
//	separateAxes[8] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[1]);
//	separateAxes[9] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[2]);
//	separateAxes[10] = Math::Cross(obb->GetOBBMaterial().orientations[0], normal[3]);
//	separateAxes[11] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[0]);
//	separateAxes[12] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[1]);
//	separateAxes[13] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[2]);
//	separateAxes[14] = Math::Cross(obb->GetOBBMaterial().orientations[1], normal[3]);
//	separateAxes[15] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[0]);
//	separateAxes[16] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[1]);
//	separateAxes[17] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[2]);
//	separateAxes[18] = Math::Cross(obb->GetOBBMaterial().orientations[2], normal[3]);
//
//	//半分のベクトル
//	/*obb*/
//	Vector3 obbDirection[3]{
//		obb->GetOBBMaterial().orientations[0] * obb->GetOBBMaterial().size.x,
//		obb->GetOBBMaterial().orientations[1] * obb->GetOBBMaterial().size.y,
//		obb->GetOBBMaterial().orientations[2] * obb->GetOBBMaterial().size.z,
//	};
//
//	//頂点の数
//	const int kOBBCornerNum = 8;
//	const int kHexagonCornerNum = 12;
//
//	// 点(頂点)
//	Vector3 obbCorners[kOBBCornerNum] = {
//	  obb->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の右上
//	  obb->GetOBBMaterial().center + obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の右上
//	  obb->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の右下
//	  obb->GetOBBMaterial().center + obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の右下
//	  obb->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] + obbDirection[2],//背面の左上
//	  obb->GetOBBMaterial().center - obbDirection[0] + obbDirection[1] - obbDirection[2],//正面の左上
//	  obb->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] + obbDirection[2],//背面の左下
//	  obb->GetOBBMaterial().center - obbDirection[0] - obbDirection[1] - obbDirection[2],//正面の左下
//	};
//	Vector3 hexagonConers[kHexagonCornerNum];
//
//	// 半径と高さ
//	Vector3 size = triangle->GetTriangleMaterial().size;
//	// 六角形の頂点計算
//	for (int i = 0; i < 6; i++) {
//		float angle = i * (2 * pi_f / 3.0f);  // 120度ごとに頂点がある
//		float x = size.x * cos(angle);
//		float z = size.z * sin(angle);
//
//		// 底面の頂点 (z = -height)
//		hexagonConers[i] = Vector3(x, -size.y, z) + triangle->GetTriangleMaterial().center;
//
//		// 上面の頂点 (z = +height)
//		hexagonConers[i + 6] = Vector3(x, size.y, z) + triangle->GetTriangleMaterial().center;
//	}
//
//	// 中心点間のベクトル
//	Vector3 centerToCenter = triangle->GetTriangleMaterial().center - obb->GetOBBMaterial().center;
//
//	//当たったかの判定
//	bool isHit = true;
//
//	//当たり判定の計算
//	for (const auto& separateAxis : separateAxes) {
//		float minOBB = (numeric_limits<float>::max)();
//		float maxOBB = (numeric_limits<float>::lowest)();
//		float minHexagon = minOBB;
//		float maxHexagon = maxOBB;
//		for (auto obbCornerIndex = 0; obbCornerIndex < kOBBCornerNum; obbCornerIndex++) {
//			float obbDistance = Math::Dot(obbCorners[obbCornerIndex], separateAxis);
//			minOBB = (min)(obbDistance, minOBB);
//			maxOBB = (max)(obbDistance, maxOBB);
//		}
//		for (auto triangleCornerIndex = 0; triangleCornerIndex < kHexagonCornerNum; triangleCornerIndex++) {
//			float triangleDistance = Math::Dot(hexagonConers[triangleCornerIndex], separateAxis);
//			minHexagon = (min)(triangleDistance, minHexagon);
//			maxHexagon = (max)(triangleDistance, maxHexagon);
//		}
//		//それぞれを射影した範囲長の合計を求める
//		float sumSpan = maxOBB - minOBB + maxHexagon - minHexagon;
//		//最大範囲を求める
//		float longSpan = (max)(maxOBB, maxHexagon) - (min)(minOBB, minHexagon);
//		if (sumSpan < longSpan) {
//			isHit = false;
//		}
//	}
//
//	return isHit;
//}
//
////六角柱とOBBの当たり判定
//bool operator==(Hexagon& hexagon, OBB& obb) {
//	return Collision::GetInstance()->IsCollision(&hexagon, &obb);
//}