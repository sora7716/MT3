#include <Novice.h>
#include <math.h>

const char kWindowTitle[] = "イイヅカ_ソラ";

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
	Vector2 operator-(Vector2 v) {
		Vector2 result = {};
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}
	Vector2 operator+(Vector2 v) {
		Vector2 result = {};
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}
}Vector2;

//線分
typedef struct Segment {
	Vector2 origin;
	Vector2 diff;

}Segment;

bool IsCollision(Vector2 pointPos, Segment segment) {
	Vector2 beginToEndForLine = (segment.origin + segment.diff) - segment.origin;
	Vector2 lineBeginToPoint = segment.origin - pointPos;

	float len = sqrtf(beginToEndForLine.x * beginToEndForLine.x + beginToEndForLine.y * beginToEndForLine.y) * sqrtf(lineBeginToPoint.x * lineBeginToPoint.x + lineBeginToPoint.y * lineBeginToPoint.y);

	float dot = beginToEndForLine.x * lineBeginToPoint.x + beginToEndForLine.y * lineBeginToPoint.y;

	if (dot != len) {
		return false;
	}
	return sqrtf(beginToEndForLine.x * beginToEndForLine.x + beginToEndForLine.y * beginToEndForLine.y) > sqrtf(lineBeginToPoint.x * lineBeginToPoint.x + lineBeginToPoint.y * lineBeginToPoint.y);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector2 pointPos = {};
	Vector2 pointVelocity = {};
	unsigned int pointColor = WHITE;

	Segment segment = {};
	segment.origin = { 100.0f,100.0f };
	segment.diff = { 1000.0f,100.0f };

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

		pointPos.x += pointVelocity.x;
		pointPos.y += pointVelocity.y;

		if (IsCollision(pointPos, segment)) {
			pointColor = RED;
		} else {
			pointColor = WHITE;
		}

		if (keys[DIK_W]) {
			pointVelocity.y = -5.0f;
		} else if (keys[DIK_S]) {
			pointVelocity.y = 5.0f;
		} else {
			pointVelocity.y = 0.0f;
		}

		if (keys[DIK_A]) {
			pointVelocity.x = -5.0f;
		} else if (keys[DIK_D]) {
			pointVelocity.x = 5.0f;
		} else {
			pointVelocity.x = 0.0f;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawLine(
			static_cast<int>(segment.origin.x),
			static_cast<int>(segment.origin.y),
			static_cast<int>(segment.origin.x + segment.diff.x),
			static_cast<int>(segment.origin.y + segment.diff.y),
			WHITE
		);

		Novice::DrawEllipse(
			static_cast<int>(pointPos.x),
			static_cast<int>(pointPos.y),
			10,
			10,
			0.0f, pointColor,
			kFillModeSolid
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
