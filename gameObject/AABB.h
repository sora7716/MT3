#pragma once
#include "gameObject/BaseGameObject.h"

class Camera;
//AABBクラス
class AABB :public BaseGameObject {
public://メンバ関数
	/// <summary>
	/// コンストラクター
	/// </summary>
	AABB() = default;

	/// <summary>
	/// デストラクター
	/// </summary>
	~AABB() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="aabb">aabb</param>
	void Initialize(Camera* camera, const AABBData&& aabb);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突したときの判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// デバックテキスト
	/// </summary>
	/// <param name="label">名前</param>
	void DebugText(const char* label = "aabb");

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()const;

	/// <summary>
	/// 衝突したかどうかのセッター
	/// </summary>
	/// <param name="isHit">セットしたいフラグ</param>
	void SetIsHit(bool isHit);

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">セットしたいカラー</param>
	void SetColor(uint32_t color);

	/// <summary>
	/// ローカル座標のセッター
	/// </summary>
	/// <param name="local">ローカル座標</param>
	void SetLocalVertex2D(RectVertex local);

	/// <summary>
	/// aabbデータのセッター
	/// </summary>
	/// <param name="aabbData">aabbData</param>
	void SetAABB(const AABBData& aabbData);

	/// <summary>
	/// AABBのデータのゲッター
	/// </summary>
	/// <returns>AABBデータ</returns>
	AABBData GetAABBMaterial()const;
private://静的メンバ変数
	static inline const int kAABB2DNum = 2;//2次元で見たAABBの数
private://メンバ変数
	AABBData aabbData_ = {};
	Camera* camera_ = nullptr;
	RectVertex localVertecies_[kAABB2DNum] = {};
	RectVertex screenVertecies_[kAABB2DNum] = {};

};

