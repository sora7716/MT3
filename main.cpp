#include <Novice.h>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>
#include <imgui.h>

const char kWindowTitle[] = "イイヅカ_ソラ";

//画面の大きさ
const float kWindowWidth = 1280.0f;//横幅
const float kWindowHeight = 720.0f;//縦幅
//三角形の頂点数
const int kTriangleVertexNum = 3;

//頂点の数
enum VertexPosition {
	kLeft,
	kTop,
	kRight
};

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;

	//減算
	Vector2& operator-=(const Vector2 v) {
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	//加算
	Vector2 operator+=(const Vector2& v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	//加算
	Vector2 operator+(const Vector2& v) {
		Vector2 result = {};
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	//減算
	Vector2 operator-(const Vector2& v) {
		Vector2 result = {};
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	//除算
	Vector2 operator/(float n) {
		Vector2 result = {};
		result.x = x / n;
		result.y = y / n;
		return result;
	}

	//除算
	Vector2& operator/=(float n) {
		this->x /= n;
		this->y /= n;
		return *this;
	}

	//乗算
	Vector2 operator*(const Vector2& v) {
		Vector2 result = {};
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	/// <summary>
	/// ノルムのゲッター
	/// </summary>
	/// <returns>ノルム</returns>
	float GetNorm() {
		return sqrtf(powf(x, 2.0f) + powf(y, 2.0f));
	}

	/// <summary>
	/// 正規化ベクトルのゲッター
	/// </summary>
	/// <returns>正規化ベクトル</returns>
	Vector2 GetNormal() {
		Vector2 result = {};
		float len = this->GetNorm();
		if (len != 0.0f) {
			result = *this / len;
		}
		return result;
	}

	/// <summary>
	/// クロス積のゲッター
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>クロス積</returns>
	float GetCross(const Vector2& v) {
		return x * v.y - y * v.x;
	}

}Vector2;

//回転のデータ
typedef struct RotateData {
	float theta;//θ
	Vector2 centroid;//重心
	Vector2 localVertices[kTriangleVertexNum];//ローカル座標
	Vector2 screenVertices[kTriangleVertexNum];//スクリーン座標
}RotateData;

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	unsigned int color;
	bool isAlive;
}GameObject;

//三角形のデータ
typedef struct TriangleData {
	GameObject gameObject;
	RotateData rotateData;
}TriangleData;

//プレイヤーのデータ
typedef struct PlayerData {
	GameObject gameObject;
	Vector2 direction;
	float vectorLen;
}PlayerData;

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="screenVertices">三角形の各頂点</param>
/// <param name="target">ターゲット</param>
/// <returns>衝突しているかの判定</returns>
bool IsCollision(Vector2* triangleVertices, Vector2 target) {

	//三角形の各辺の差分ベクトル
	Vector2 topToLeft = triangleVertices[static_cast<int>(kLeft)] - triangleVertices[static_cast<int>(kTop)];
	Vector2 leftToRight = triangleVertices[static_cast<int>(kRight)] - triangleVertices[static_cast<int>(kLeft)];
	Vector2 rightToTop = triangleVertices[static_cast<int>(kTop)] - triangleVertices[static_cast<int>(kRight)];

	//三角形の各頂点とターゲットの差分ベクトル
	Vector2 topToTarget = target - triangleVertices[static_cast<int>(kTop)];
	Vector2 leftToTarget = target - triangleVertices[static_cast<int>(kLeft)];
	Vector2 rightToTarget = target - triangleVertices[static_cast<int>(kRight)];

	//外積
	float cross[3] = {
		topToLeft.GetCross(topToTarget),
		leftToRight.GetCross(leftToTarget),
		rightToTop.GetCross(rightToTarget)
	};

	return cross[0] < 0.0f && cross[1] < 0.0f && cross[2] < 0.0f;
}

/// <summary>
/// 弧度法に変換
/// </summary>
/// <param name="degree">度数法</param>
/// <returns>弧度法</returns>
float ConversionForRadian(float degree) {
	return degree * (static_cast<float>(M_PI) / 180.0f);
}

/// <summary>
/// クランプ
/// </summary>
/// <param name="num">対象の値</param>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <returns>クランプした値</returns>
float Clamp(float num, float min, float max) {
	if (num < min) {
		return min;
	} else if (num > max) {
		return max;
	}
	return num;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//三角形
	TriangleData triangleData = {};
	triangleData.gameObject.position = { kWindowWidth / 2.0f,kWindowHeight / 2.0f };//位置
	triangleData.gameObject.size = { 200.0f,200.0f };//大きさ
	triangleData.gameObject.isAlive = true;//生存フラグ
	triangleData.gameObject.color = WHITE;//色
	//ローカル座標
	triangleData.rotateData.localVertices[static_cast<int>(kLeft)] = {
		-triangleData.gameObject.size.x / 2.0f,triangleData.gameObject.size.y / 2.0f
	};
	triangleData.rotateData.localVertices[static_cast<int>(kTop)] = {
		0.0f,-triangleData.gameObject.size.y / 2.0f
	};
	triangleData.rotateData.localVertices[static_cast<int>(kRight)] = {
		triangleData.gameObject.size.x / 2.0f,triangleData.gameObject.size.y / 2.0f
	};
	//重心を求める
	triangleData.rotateData.centroid = (triangleData.rotateData.localVertices[static_cast<int>(kLeft)] + triangleData.rotateData.localVertices[static_cast<int>(kTop)] + triangleData.rotateData.localVertices[static_cast<int>(kRight)]) / 3.0f;
	//重心を回転中心にする
	for (int i = 0; i < kTriangleVertexNum; i++) {
		triangleData.rotateData.localVertices[i] -= triangleData.rotateData.centroid;
	}

	//円
	PlayerData circle = {};
	circle.gameObject.position = { 500.0f,500.0f };
	circle.gameObject.size = { 50.0f,50.0f };//大きさ
	circle.gameObject.color = GREEN;//色
	circle.gameObject.velocity = { 5.0f,5.0f };//速度
	circle.gameObject.isAlive = true;//生存フラグ
	Vector2 circleToTriangle = {};
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

		//triangleData.rotateData.theta += 0.1f;

		//三角形の処理
		if (triangleData.gameObject.isAlive) {
			for (int i = 0; i < kTriangleVertexNum; i++) {
				//加法定理で現在の角度を算出
				triangleData.rotateData.screenVertices[i].x = triangleData.rotateData.localVertices[i].x * cosf(triangleData.rotateData.theta) - triangleData.rotateData.localVertices[i].y * sinf(triangleData.rotateData.theta);

				triangleData.rotateData.screenVertices[i].y = triangleData.rotateData.localVertices[i].y * cosf(triangleData.rotateData.theta) + triangleData.rotateData.localVertices[i].x * sinf(triangleData.rotateData.theta);

				//描画したいところに移動
				triangleData.rotateData.screenVertices[i] += triangleData.gameObject.position;
			}
		}

		//円の処理
		if (circle.gameObject.isAlive) {
			//移動の処理
			//縦
			if (keys[DIK_W]) {
				circle.direction.y = -1.0f;
			} else if (keys[DIK_S]) {
				circle.direction.y = 1.0f;
			} else {
				circle.direction.y = 0.0f;
			}

			//横
			if (keys[DIK_A]) {
				circle.direction.x = -1.0f;
			} else if (keys[DIK_D]) {
				circle.direction.x = 1.0f;
			} else {
				circle.direction.x = 0.0f;
			}

			//正規化ベクトルを取得する
			circle.direction = circle.direction.GetNormal();

			//移動
			circle.gameObject.position += circle.gameObject.velocity * circle.direction;
		}

		//衝突判定のターゲットポイント
		Vector2 targetPoint = {
			Clamp(circle.gameObject.position.x,-circle.gameObject.size.x,circle.gameObject.size.x),
			Clamp(circle.gameObject.position.y,-circle.gameObject.size.y,circle.gameObject.size.y)
		};

		//衝突判定
		if (IsCollision(triangleData.rotateData.screenVertices, targetPoint)) {
			circle.gameObject.color = BLUE - 0x00000055;
		} else {
			circle.gameObject.color = GREEN - 0x00000055;
		}

		///
		/// ↑更新処理ここまで
		///

		//三角形が生存していたら
		if (triangleData.gameObject.isAlive) {
			Novice::DrawTriangle(
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kLeft)].x),
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kLeft)].y),
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kTop)].x),
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kTop)].y),
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kRight)].x),
				static_cast<int>(triangleData.rotateData.screenVertices[static_cast<int>(kRight)].y),
				triangleData.gameObject.color, kFillModeSolid
			);
		}

		//円が生存していたら
		if (circle.gameObject.isAlive) {
			Novice::DrawEllipse(
				static_cast<int>(circle.gameObject.position.x),
				static_cast<int>(circle.gameObject.position.y),
				static_cast<int>(circle.gameObject.size.x),
				static_cast<int>(circle.gameObject.size.y),
				0.0f, circle.gameObject.color, kFillModeSolid
			);
		}

		///
		/// ↓描画処理ここから
		///

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
