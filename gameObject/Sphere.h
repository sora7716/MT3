#pragma once
#include "gameObject/BaseGameObject.h"

//前方宣言
class Grid;
class Plane;

//スフィアに使う構造体
typedef struct SphereData {
	Vector3 center;
	float radius;
	uint32_t color = WHITE;//色
	bool isHit = false;//衝突したか
}SphereMaterial;

/// <summary>
/// スフィア
/// </summary>
class Sphere :public BaseGameObject {
private://構造体
	//座標
	typedef struct Coord {
		Vector3 local;
		std::vector<Vector3> screen;
	}Coord;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sphere() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sphere() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="sphere">スフィアの素材{positon,radius,color}</param>
	void Initialize(Camera* camera, SphereData* sphereMaterial);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// デバックテキスト
	/// </summary>
	/// <param name="label">名前</param>
	void DebugText(const char* name = "sphere");

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たった時の判定
	/// </summary>
	/// <param name="isHit">衝突判定</param>
	void OnCollision();

	/// <summary>
	/// カラーのセッター
	/// </summary>
	/// <param name="color"></param>
	void SetColor(uint32_t color);

	/// <summary>
	/// スフィアの素材のゲッター
	/// </summary>
	/// <returns></returns>
	const SphereData& GetSphereMaterial()const;

	/// <summary>
	/// 平面のセッター
	/// </summary>
	/// <param name="plaen">平面</param>
	void SetPlane(Plane* plane);

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera);

	/// <summary>
	/// 球の素材のセッター
	/// </summary>
	/// <param name="material">球の素材</param>
	void SetSphere(SphereData* sphereData);

	/// <summary>
	/// 当たり判定のセッター
	/// </summary>
	/// <param name="isHit">hitフラグ</param>
	void SetIsHit(bool isHit);

private://メンバ変数
	//カメラ
	Camera* camera_ = nullptr;
	//スフィアのデータ
	SphereData* sphereData_ = {};
	//平面
	Plane* plane_ = nullptr;
	//スクリーン座標
	Coord coordA_ = {};
	Coord coordB_ = {};
	Coord coordC_ = {};
	//デフォルトのカラー
	unsigned int defaultColor = 0u;
};

