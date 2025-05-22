#pragma once
#include <cmath>
#define _USE_MATH_DEFINES
#include "gameObject/BaseGameObject.h"

//カメラの前方宣言
class Camera;
class Grid;

/// <summary>
/// 平面
/// </summary>
class Plane :public BaseGameObject {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Plane() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Plane() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="planeData">平面の素材{normal,distance,color}</param>
	void Initialize(Camera* camera, const PlaneData&& planeData);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// デバックテキスト
	/// </summary>
	void DebugText(const char* name = "plane");

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 平面のデータのゲッター
	/// </summary>
	/// <returns></returns>
	const PlaneData& GetPlaneMaterial();

	/// <summary>
	/// 平面のデータのセッター
	/// </summary>
	/// <param name="planeData_">平面のデータ</param>
	void SetPlane(const PlaneData& planeData);

private://メンバ関数
	/// <summary>
	/// 垂直
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 Perpendicular(const Vector3& v);

private://メンバ変数
	//カメラ
	Camera* camera_ = nullptr;
	//平面を描画するときの4頂点
	Vector3 points_[4]{};
	//平面のデータ
	PlaneData planeData_ = {};
};

