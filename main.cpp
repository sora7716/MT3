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

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	unsigned int color;
	bool isAlive;
}GameObject;

bool IsCollision(GameObject obj1, GameObject obj2) {
	//X座標の当たってない判定
	if (obj1.position.x < obj2.position.x + obj2.size.x && obj1.position.x + obj1.size.x < obj2.position.x) {
		return false;
	} else if (obj2.position.x + obj2.size.x < obj1.position.x && obj2.position.x < obj1.position.x + obj1.size.x) {
		return false;
	}

	// Y座標の当たってない判定
	else if (obj1.position.y + obj1.size.y > obj2.position.y && obj1.position.y > obj2.size.y && obj1.position.y) {
		return false;
	} else if (obj2.position.y + obj2.size.y > obj1.position.y && obj2.position.y > obj1.position.y + obj1.size.y) {
		return false;
	}

	return true;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	GameObject box[2] = {};

	box[0].position = { 100.0f,100.0f };
	box[1].position = { 200.0f,200.0f };
	box[0].size = { 100.0f,100.0f };
	box[1].size = { 50.0f,50.0f };
	box[0].color = WHITE;
	box[1].color = WHITE;
	box[0].isAlive = true;
	box[1].isAlive = true;


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

		box[0].position.x += box[0].velocity.x;
		box[0].position.y += box[0].velocity.y;

		if (keys[DIK_W]) {
			box[0].velocity.y = -5.0f;
		} else if (keys[DIK_S]) {
			box[0].velocity.y = 5.0f;
		} else {
			box[0].velocity.y = 0.0f;
		}

		if (keys[DIK_A]) {
			box[0].velocity.x = -5.0f;
		} else if (keys[DIK_D]) {
			box[0].velocity.x = 5.0f;
		} else {
			box[0].velocity.x = 0.0f;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		for (int i = 0; i < 2; i++) {
			Novice::DrawBox(
				static_cast<int>(box[i].position.x),
				static_cast<int>(box[i].position.y),
				static_cast<int>(box[i].size.x),
				static_cast<int>(box[i].size.y),
				0.0f, box[i].color, kFillModeSolid
			);
		}

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
