#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>
const char kWindowTitle[] = "イイヅカ_ソラ";

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;

	//加法
	Vector2 operator+(const Vector2& v) {
		return { x + v.x, y + v.y };
	}

	//減法
	Vector2 operator-(const Vector2& v) {
		return { x - v.x, y - v.y };
	}

	//除法(float)
	Vector2 operator/(float n) {
		return { x / n, y / n };
	}
}Vector2;

//オブジェクトの大きさ
typedef struct Size {
	float width;// 幅
	float height;// 高さ
}Size;

//オブジェクトの頂点インデックス
enum VertexIndex :int32_t {
	kLeft,
	kTop,
	kRight,
	kVertxCount// 頂点の数
};

//回転するオブジェクト
typedef struct RotateObject {
	Vector2 center; // 回転の中心点
	Size size; // オブジェクトの大きさ
	float theta; // 回転角度（ラジアン）
	uint32_t textureHandle; // テクスチャのハンドル
	Vector2 rotateVertices[static_cast<int32_t>(kVertxCount)]; // 回転の頂点座標（左上、右上、左下、右下）
	Vector2 vertices[static_cast<int32_t>(kVertxCount)]; // 頂点座標（左上、右上、左下、右下）
}RotateObject;

//弧度法へ変換
float ConversionRadian(float degree) {
	// ラジアンを度に変換
	return degree * static_cast<float>(M_PI) / 180.0f;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//回転するかのフラグ
	bool isRotate = false;

	//回転オブジェクトの初期化
	RotateObject rotateObject = {};
	rotateObject.center = { 400.0f,400.0f };// 中心点
	rotateObject.size = { 100.0f,100.0f };// オブジェクトの大きさ
	rotateObject.theta = ConversionRadian(0.0f);// 回転角度（ラジアン）
	rotateObject.textureHandle = Novice::LoadTexture("white1x1.png");// テクスチャの読み込み

	//初期の頂点座標の初期化
	Vector2 kVertices[static_cast<int32_t>(kVertxCount)] = {
		{-rotateObject.size.width / 2.0f,rotateObject.size.height / 2.0f},
		{0.0f,-rotateObject.size.height / 2.0f},
		{rotateObject.size.width / 2.0f,rotateObject.size.height / 2.0f},
	};

	//三角形の重心
	Vector2 centroid = {
		(kVertices[static_cast<int>(kLeft)] + kVertices[static_cast<int>(kTop)] + kVertices[static_cast<int>(kRight)]) / 3.0f
	};

	//重心を中心にするための頂点座標の初期化
	Vector2 localVertices[static_cast<int32_t>(kVertxCount)] = {
		{kVertices[static_cast<int>(kLeft)] - centroid},
		{kVertices[static_cast<int>(kTop)] - centroid},
		{kVertices[static_cast<int>(kRight)] - centroid},
	};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//回転させるかの切り替えスイッチ
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			isRotate ^= true;
		}

		//θを動かす
		if (isRotate) {
			rotateObject.theta += 1.0f / 120.0f * float(M_PI);
		}

		//回転の処理
		for (int32_t i = 0; i < static_cast<int32_t>(kVertxCount); i++) {
			//回転
			rotateObject.rotateVertices[i].x = localVertices[i].x * cosf(rotateObject.theta) - localVertices[i].y * sinf(rotateObject.theta);
			rotateObject.rotateVertices[i].y = localVertices[i].y * cosf(rotateObject.theta) + localVertices[i].x * sinf(rotateObject.theta);

			//回転の中心を動かす
			rotateObject.vertices[i].x = rotateObject.rotateVertices[i].x + rotateObject.center.x;
			rotateObject.vertices[i].y = rotateObject.rotateVertices[i].y + rotateObject.center.y;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		/// 

		//回転しているか
		Novice::ScreenPrintf(0, 0, "isRotate:%s", isRotate ? "true" : "false");

		//白い三角形
		Novice::DrawTriangle(
			static_cast<int>(rotateObject.vertices[kLeft].x), static_cast<int>(rotateObject.vertices[kLeft].y),
			static_cast<int>(rotateObject.vertices[kTop].x), static_cast<int>(rotateObject.vertices[kTop].y),
			static_cast<int>(rotateObject.vertices[kRight].x), static_cast<int>(rotateObject.vertices[kRight].y),
			WHITE, kFillModeSolid
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}