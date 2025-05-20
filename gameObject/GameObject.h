#pragma once
#include <Novice.h>
#include <cmath>
#include <array>
#define _USE_MATH_DEFINES
#ifdef _DEBUG
#include <ImGui.h>
#endif // _DEBUG
#include "myMath/Vector3.h"
#include "myMath/Matrix4x4.h"
#include "myMath/func/Math.h"
#include "myMath/func/Rendering.h"
//#include "scene/func/ScreenPrintf.h"
//#include "scene/func/Aithmetic.h"

class GameObject {

public://構造体など

	//ベクトル2の整数型
	typedef struct Vector2Int {
		int x;
		int y;
	}Vector2Int;

	//AABBに使う素材
	typedef struct AABBMaterial {
		Vector3 min;//最小点
		Vector3 max;//最大点
		uint32_t color = WHITE;//色
		bool isHit = false;//衝突したか
	}AABBMaterial;

	//AABBを2Dで作るときに使う
	typedef struct Vertex2D {
		Vector3 leftTop;
		Vector3 rightTop;
		Vector3 leftBottom;
		Vector3 rightBottom;
	}Vertex2D;

	//スフィアに使う構造体
	typedef struct SphereMaterial {
		Vector3 center;
		float radius;
		uint32_t color = WHITE;//色
		bool isHit = false;//衝突したか
	}SphereMaterial;

	//平面の構造体
	typedef struct PlaneMaterial {
		Vector3 normal;
		float distance;
		uint32_t color;
	}PlaneMaterial;

	//直線
	typedef struct StraightLine {
		Vector3 origin;//始点
		Vector3 diff;//終点への差分ベクトル
	}StraightLine;

	//半直線
	typedef struct Ray {
		Vector3 origin;//始点
		Vector3 diff; //終点への差分ベクトル
	}Ray;

	//線分
	typedef struct Segment {
		Vector3 origin;//始点
		Vector3 diff; //終点への差分ベクトル
	}Segment;


	//頂点
	enum class Vertex {
		kTop,
		kRight,
		kLeft,
		kVertexNum,
	};

	//三角形の素材
	typedef struct TriangleMaterial {
		Vector3 kLocalVertices_[static_cast<int>(Vertex::kVertexNum)];
		Vector3 center;
		float radian;
		Vector3 size;
		Vector3 normal[4];
		uint32_t color = WHITE;//色
		bool isHit = false;//衝突したか
	}TriangleMaterial;

	//OBBの素材
	typedef struct ObbMaterial {
		Vector3 center;//中心 translation
		Vector3 orientations[3] = {
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
		};//座標軸。正規化・直行必須 rotation
		Vector3 size = { 1.0f,1.0f,1.0f };//座標軸方向の長さの半分。中心から面までの距離 scale
		Vector3 rotation = {};//回転
		uint32_t color = WHITE;//色
	}OBBMaterial;

	//カプセル作るときの素材
	typedef struct CapsuleMaterial {
		Segment segment;//線分
		float radius;//半径
		uint32_t color = WHITE;//色
		bool isHit = false;//衝突したかの判定
	}CapsuleMaterial;

	//六角形を作るときに使う素材
	typedef struct HexagonMaterial {
		Vector3 center;//中心
		Vector3 size = { 1.0f,1.0f,1.0f };//座標軸方向の長さの半分。中心から面までの距離 scale
		Vector3 normal[4];//法線ベクトル
		Vector3 rotation = {};//回転
		uint32_t color = WHITE;//色
		bool isHit = false;//衝突したか
	}HexagonMaterial;

public://静的メンバ変数
	static inline const int kAABB2DNum = 2;//2次元で見たAABBの数
protected://メンバ関数

	GameObject() = default;
	~GameObject() = default;

	/// <summary>
	/// 倍率
	/// </summary>
	/// <param name="front">手前に来るキーを押してか?</param>
	/// <param name="behind">後ろに行くキーを押したか?</param>
	void Scale(bool front, bool behind);

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// X座標の移動
	/// </summary>
	/// <param name="left">左に行くキーを押したか?</param>
	/// <param name="right">右に行くキーを押したか?</param>
	/// <param name="type">カメラだったら-1,それ以外は1を入れる</param>
	void TranslateX(bool left, bool right, float type);

	/// <summary>
	/// Y座標の移動
	/// </summary>
	/// <param name="up">上に行くボタン</param>
	/// <param name="down">下に行くボタン</param>
	/// <param name="type">カメラだったら-1,それ以外は1を入れる</param>
	void TranslateY(bool up, bool down, float type);

	/// <summary>
	/// Z座標の移動
	/// </summary>
	/// <param name="front">前に行く</param>
	/// <param name="back">後ろに行く</param>
	/// <param name="type">カメラだったら-1,それ以外は1を入れる</param>
	void TranslateZ(bool front, bool back, float type);

	/// <summary>
	/// X座標の回転
	/// </summary>
	/// <param name="bottom1"></param>
	/// <param name="bottom2"></param>
	void RotateX(bool bottom1, bool bottom2);

	/// <summary>
	/// Y座標の回転
	/// </summary>
	/// <param name="bottom1"></param>
	/// <param name="bottom2"></param>
	void RotateY(bool bottom1, bool bottom2);

	/// <summary>
	/// Z座標の回転
	/// </summary>
	/// <param name="bottom1"></param>
	/// <param name="bottom2"></param>
	void RotateZ(bool bottom1, bool bottom2);

	/// <summary>
	/// 移動をまとめた
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="right">右</param>
	/// <param name="up">上</param>
	/// <param name="down">下</param>
	/// <param name="front">前</param>
	/// <param name="behind">後ろ</param>
	/// <param name="type"></param>
	void Movement(bool left, bool right, bool up, bool down, bool front, bool behind, float type = 1.0f);

	/// <summary>
	/// 回転をまとめた
	/// <param name="left">左</param>
	/// <param name="right">右</param>
	/// <param name="up">上</param>
	/// <param name="down">下</param>
	/// <param name="front">前</param>
	/// <param name="behind">後ろ</param>
	void Rotation(bool left, bool right, bool up, bool down, bool front, bool behind);

	/// <summary>
	/// 拡大縮小
	/// </summary>
	void Scaling();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

protected:
	int windowWidth_ = 0;//横幅
	int windowHeight_ = 0;//縦幅

	Vector3 scale_ = {};//倍率
	Vector3 rotate_ = {};//角度
	Vector3 translate_ = {};//ポジション

	Vector3   scaling_ = {};//拡縮
	Vector3   radian_ = {};//回転
	Vector3   velocity_ = {};//移動速度

	Vector2Int mousePos_ = {};//マウスのポジション
	Vector2Int preMousePos_ = {};//前のマウスのポジション
};
