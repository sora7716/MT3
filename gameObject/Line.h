#pragma once
#include "gameObject/BaseGameObject.h"

//前方宣言
class Camera;

class Line :public BaseGameObject {
public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Line();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Line();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, const Segment&& segment);

	/// <summary>
	/// 線分の更新
	/// </summary>
	void UpdateSegment();

	/// <summary>
	/// デバックテキスト
	/// </summary>
	/// <param name="name">名前</param>
	void DebugText(const char* name = "segment");

	/// <summary>
	/// 線分の描画
	/// </summary>
	void DrawSegment();

	/// <summary>
	/// ベジエ曲線の描画
	/// </summary>
	void DrawBezier();

	/// <summary>
	/// スプライン曲線
	/// </summary>
	void DrawCatmullRom();

	/// <summary>
	/// オブジェクトの通った道を表示する
	/// </summary>
	/// <param name="pos">ポジション</param>
	void DrawObjectRoad(const Vector3& pos);

	/// <summary>
	/// 衝突した場合の判定
	/// </summary>
	/// <param name="isHit">衝突したかどうか</param>
	void OnCollision(bool isHit);

	/// <summary>
	/// 最近接点のゲッター
	/// </summary>
	/// <returns>最近接点</returns>
	Vector3 GetClosestPoint()const;

	/// <summary>
	/// 原点のセッター
	/// </summary>
	/// <returns>原点</returns>
	Vector3 GetPoint()const;

	/// <summary>
	/// 線分のゲッター
	/// </summary>
	/// <returns>線分</returns>
	const Segment& GetSegment()const;

	/// <summary>
	/// 正射影ベクトルのゲッター
	/// </summary>
	/// <returns>正射影ベクトル</returns>
	Vector3 GetProject()const;

	/// <summary>
	/// 媒介変数のゲッター
	/// </summary>
	/// <returns></returns>
	float GetT()const;

	/// <summary>
	/// 原点のセッター
	/// </summary>
	/// <param name="point">セットしたい値</param>
	void SetPoint(const Vector3& point);

	/// <summary>
	/// 線分のセッター
	/// </summary>
	/// <param name="segment">セットしたい値</param>
	void SetSegment(const Segment& segment);

	/// <summary>
	/// 色のセッター
	/// </summary>
	void SetColor(uint32_t color);

	/// <summary>
	/// 衝突判定のセッター
	/// </summary>
	/// <param name="isHit"></param>
	void SetIsHit(bool isHit);

	/// <summary>
	/// ベジエ曲線の制御点のセッター
	/// </summary>
	/// <param name="controlPoints">制御点</param>
	void SetBezierPoints(Vector3* controlPoints);

	/// <summary>
	/// ベジエ曲線の制御点のゲッター
	/// </summary>
	/// <returns>制御点の座標</returns>
	Vector3* GetBezierPoints();

	/// <summary>
	/// スプライン曲線の制御点のセッター
	/// </summary>
	/// <param name="controlPoints">制御点</param>
	void SetCatmullRomPoints(std::vector<Vector3>controlPoints);

	/// <summary>
	/// スプライン曲線の制御点のゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<Vector3>GetCatmullRomPoints();

private://メンバ関数
	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	void Project(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 最近接点
	/// </summary>
	/// <param name="point">原点</param>
	/// <param name="segment">線分</param>
	void ClosestPoint();

public:
	static inline const int kDivision = 100;
private://メンバ変数
	float t_;//媒介変数表示
	Vector3 project_;//正射影ベクトル
	Vector3 closestPoint_;//最近接点
	Vector3 point_;//原点
	Segment segment_;//線分
	Camera* camera_;//カメラ
	uint32_t color_;//色
	bool isHit_;//当たったかどうか
	Vector3 bezierPoints_[3] = {};//ベジエ曲線用の制御点
	std::vector<Vector3>catmullRomPoints_{};//スプライン曲線用の制御点
};

