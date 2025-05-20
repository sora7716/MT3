#pragma once
#include "gameObject/BaseGameObject.h"

//前方宣言
class Camera;
class Grid :public BaseGameObject {
private://構造体
	//グリッドの座標
	typedef struct Coord {
		Vector3 localStart;
		Vector3 localEnd;
		Vector3 screenStart;
		Vector3 screenEnd;
		unsigned int color;
	}Coord;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Grid() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Grid() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private://静的メンバ変数
	//分割数
	static inline const uint32_t kSubdivision = 10;
private://メンバ変数
	//カメラ
	Camera* camera_ = nullptr;
	std::vector<Transform> gridTransforms_ = {};
	//x軸の線
	std::vector<Coord>xLine_ = {};
	//z軸の線
	std::vector<Coord>zLine_ = {};
};

