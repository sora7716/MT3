#pragma once
#include "gameObject/Camera.h"
#include "myMath/ShapeData.h"
#include "myMath/RenderingData.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG
#include <string>
#include <vector>
//座標
typedef struct Coord {
	Vector3 local;
	std::vector<Vector3> screen;
}Coord;

/// <summary>
/// ゲームオブジェクトの基盤
/// </summary>
class BaseGameObject{
public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseGameObject() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BaseGameObject() = default;

protected://メンバ関数
	/// <summary>
	/// world*viewProjectionを求める
	/// </summary>
	/// <param name="camera">カメラ</param>
	void MakeWvpMatrix(Camera* camera);

	/// <summary>
	/// スクリーン座標へ変換
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="local">ローカル座標</param>
	/// <returns>スクリーン座標</returns>
	Vector3 ScreenTransform(Camera*camera,const Vector3& local);

public://メンバ関数
	/// <summary>
	/// ワールドビュープロジェクションのゲッター
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetWorldViewProjection();

	/// <summary>
	/// ワールドマトリックスのゲッター
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetWorldMatrix();

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3 GetWorldPosition();
protected://メンバ変数
	//ワールドマトリックス
	Matrix4x4 worldMatrix_ = {};
	//wvpマトリックス
	Matrix4x4 worldViewProjectionMatrix_ = {};
	//正規化デバイス座標系
	Vector3   ndcVertex_ = {};
	//トランスフォーム
	Transform transform_ = {};
};

