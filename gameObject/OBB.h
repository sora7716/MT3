#pragma once
#include "gameObject/BaseGameObject.h"
#include "AABB.h"

/// <summary>
/// OBB
/// </summary>
class OBB :public BaseGameObject {
public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	OBB() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OBB() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="obbData">obbのデータ</param>
	void Initialize(Camera* camera, const OBBData&& obbData);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// デバックテキスト
	/// </summary>
	/// <param name="label">名前</param>
	void DebagText(const char* label = "obb");

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドマトリックス逆行列のゲッター
	/// </summary>
	/// <returns>ワールドマトリックス逆行列</returns>
	Matrix4x4 GetOBBWorldMatrixInvers()const;

	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetSize()const;

	/// <summary>
	/// OBBのデータのゲッター
	/// </summary>
	/// <returns>OBBのデータ</returns>
	OBBData GetOBBMaterial()const;

	/// <summary>
	/// ローカルの頂点
	/// </summary>
	/// <returns></returns>
	RectVertex* GetLocalVertex();

	/// <summary>
	/// 衝突したら
	/// </summary>
	void OnCollision(bool isHit);

	/// <summary>
	/// ポジションのセッター
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos);

private://メンバ関数

	/// <summary>
	/// 頂点を作成
	/// </summary>
	void MakeVertecies();

	/// <summary>
	/// 操作
	/// </summary>
	/// <param name="keys">現在のキー</param>
	/// <param name="preKeys">過去のキー</param>
	void Control(const char* keys, const char preKeys);

private://メンバ変数
	OBBData obbData_ = {};//obbデータ
	Matrix4x4 invers_ = {};//obbの逆行列
	Camera* camera_ = nullptr;//カメラ
	AABBData aabb_ = {};//AABBのマテリアル
	RectVertex localVertecies_[AABB::kAABB2DNum] = {};//ローカルの頂点
	RectVertex screenVertecies_[AABB::kAABB2DNum] = {};//スクリーンの頂点
};

