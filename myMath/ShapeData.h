#pragma once
#include <cstdint>
#include "myMath/Vector3.h"

//平面の構造体
typedef struct PlaneData {
	Vector3 normal;
	float distance;
	uint32_t color;
}PlaneData;

//スフィアに使う構造体
typedef struct SphereData {
	Vector3 center;
	float radius;
	uint32_t color = WHITE;//色
	bool isHit = false;//衝突したか
}SphereData;