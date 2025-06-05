#pragma once
#include "myMath/Vector3.h"
#include "myMath/ShapeData.h"

//前方宣言
class Sphere;
class Plane;
class Line;
class Triangle;
class AABB;
class OBB;
class Hexagon;
class Capsule;

/// <summary>
/// 衝突判定達
/// </summary>
class Collision final {

public://メンバ関数

	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static Collision* GetInstance();

	/// <summary>
	///  衝突判定(球同士)
	/// </summary>
	/// <param name="sphere1">球1</param>
	/// <param name="sphere2">球2</param>
	bool IsCollision(const SphereData& sphere1, const SphereData& sphere2);

	/// <summary>
	/// 衝突判定(球と平面)
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <returns>当たったかどうか</returns>
	bool IsCollision(const SphereData& sphere, const PlaneData& plane);

	/*/// <summary>
	/// 衝突判定(線と平面)
	/// </summary>
	/// <param name="segment">線分</param>
	/// <param name="plane">平面</param>
	bool IsCollision(const GameObject::Segment& segment, const GameObject::PlaneMaterial& plane);

	/// <summary>
	/// 衝突判定(線と三角形)
	/// </summary>
	/// <param name="segment">線分</param>
	/// <param name="triangle">三角形</param>
	bool IsCollision(const GameObject::Segment& segment, const GameObject::TriangleMaterial& triangle);

	/// <summary>
	/// 衝突判定(AABB同士)
	/// </summary>
	/// <param name="aabb1">AABB1</param>
	/// <param name="aabb2">AABB2</param>
	bool IsCollision(GameObject::AABBMaterial aabb1, GameObject::AABBMaterial aabb2);

	/// <summary>
	/// 衝突判定(AABBと球)
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="sphere">球</param>
	/// <returns>衝突判定</returns>
	bool IsCollision(GameObject::AABBMaterial& aabb, const GameObject::SphereMaterial& sphere);

	/// <summary>
	/// 衝突判定(AABBと線分:返り値あり)
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="segment">線分</param>
	/// <returns>衝突判定</returns>
	bool IsCollision(const GameObject::AABBMaterial& aabb, const GameObject::Segment& segment);

	/// <summary>
	/// 衝突判定(AABBと平面)
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	bool IsCollision(const GameObject::AABBMaterial& aabb, const GameObject::PlaneMaterial& plane);

	/// <summary>
	/// 衝突判定(OBBと球)
	/// </summary>
	/// <param name="target">ターゲット</param>
	/// <param name="sphare">球</param>
	bool IsCollision(OBB* obb, const GameObject::SphereMaterial& sphere);

	/// <summary>
	/// 衝突判定(OBBと線分)
	/// </summary>
	/// <param name="target">ターゲット</param>
	/// <param name="segment">線分</param>
	bool IsCollision(OBB* obb, const GameObject::Segment segment);

	/// <summary>
	/// 衝突判定(OBB同士:AABB/OBB)
	/// </summary>
	/// <param name="target">ターゲット</param>
	/// <param name="obb">OBB</param>
	bool IsCollision(OBB* obb1, OBB* obb2);

	/// <summary>
	/// 衝突判定(OBBと平面)
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="plane">平面</param>
	/// <returns>衝突したか</returns>
	bool IsCollision(OBB* obb, const GameObject::PlaneMaterial& plane);

	/// <summary>
	/// 衝突判定(六角形と線)
	/// </summary>
	/// <param name="hexagon">六角形</param>
	/// <param name="line">線分</param>
	/// <param name="surface">面</param>
	/// <returns>衝突したか</returns>
	bool IsCollision(Hexagon* hexagon, Line* line, int surface);

	/// <summary>
	/// 衝突判定(カプセルと平面)
	/// </summary>
	/// <param name="capsule">カプセル</param>
	/// <param name="plane">平面</param>
	/// <returns>衝突したか</returns>
	bool IsCollision(const GameObject::CapsuleMaterial& capsule, const GameObject::PlaneMaterial& plane);

	/// <summary>
	/// 衝突判定(六角形と四角)
	/// </summary>
	/// <param name="hexagon">六角形</param>
	/// <param name="obb">OBB</param>
	/// <returns>衝突したか</returns>
	bool IsCollision(Hexagon* hexagon, OBB* obb);

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="triangle">三角柱</param>
	/// <param name="obb">OBB</param>
	/// <returns>衝突したか</returns>
	bool IsCollision(Triangle* triangle, OBB* obb);*/

	//コピーコンストラクタを禁止する
	Collision(const Collision& collision) = delete;

	//代入演算子を禁止する
	Collision& operator=(const Collision& collision) = delete;
	
private://静的メンバ変数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Collision() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Collision() = default;

	static inline const float kTMin = 0.0f;
	static inline const float kTMax = 1.0f;
};

//六角柱とOBBの当たり判定
//bool operator==(Hexagon&hexagon,OBB&obb);
