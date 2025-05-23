#pragma once
#include <cstdint>
#include "myMath/Vector3.h"

//平面に使う構造体
typedef struct PlaneData {
	Vector3 normal;//法線
	float distance;//向き
	uint32_t color;//色
}PlaneData;

//スフィアに使う構造体
typedef struct SphereData {
	Vector3 center;//中心
	float radius;//半径
	uint32_t color = WHITE;//色
	bool isHit = false;//衝突したか
}SphereData;

//直線に使う構造体
typedef struct StraightLine {
	Vector3 origin;//始点
	Vector3 diff;//終点への差分ベクトル
}StraightLine;

//半直線に使う構造体
typedef struct Ray {
	Vector3 origin;//始点
	Vector3 diff; //終点への差分ベクトル
}Ray;

//線分に使う構造体
typedef struct Segment {
	Vector3 origin;//始点
	Vector3 diff; //終点への差分ベクトル
}Segment;

//AABBに使う構造体
typedef struct AABBData {
	Vector3 min;//最小点
	Vector3 max;//最大点
	uint32_t color = WHITE;//色
	bool isHit = false;//衝突したか
}AABBData;

//矩形の頂点
typedef struct RectVertex {
	Vector3 leftTop;
	Vector3 rightTop;
	Vector3 leftBottom;
	Vector3 rightBottom;
}RectVertex;


//OBBのデータ
typedef struct OBBData {
	Vector3 center;//中心 translation
	Vector3 orientations[3] = {
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
	};//座標軸。正規化・直行必須 rotation
	Vector3 size = { 1.0f,1.0f,1.0f };//座標軸方向の長さの半分。中心から面までの距離 scale
	Vector3 rotate = {};//回転
	uint32_t color = WHITE;//色
}ObbData;
