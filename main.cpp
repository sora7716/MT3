#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "イイヅカ_ソラ";

//定数
const int kWindowWidth = 1280; //ウィンドウの幅
const int kWindowHeight = 720; //ウィンドウの高さ

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;//位置
	Vector2 speed;//速度
	float radius;//半径
	unsigned int color;//色
	int isAlive; //生存フラグ
}GameObject;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//プレイヤー
	GameObject player{};
	player.position = {
		static_cast<float>(kWindowWidth / 2),
		static_cast<float>(kWindowHeight - 100.0f)
	};//位置
	player.speed = { 5.0f,5.0f };//速度
	player.radius = 50.0f;//サイズ
	player.color = WHITE;//色
	player.isAlive = true;//生存フラグ
	//移動方向
	Vector2 playerDirection = {};

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

		//上下
		if (keys[DIK_W]) {
			playerDirection.y = -1.0f;
		} else if (keys[DIK_S]) {
			playerDirection.y = 1.0f;
		} else {
			playerDirection.y = 0.0f;
		}

		//左右
		if (keys[DIK_A]) {
			playerDirection.x = -1.0f;
		} else if (keys[DIK_D]) {
			playerDirection.x = 1.0f;
		} else {
			playerDirection.x = 0.0f;
		}

		//移動ベクトルの長さを計算
		float len = sqrtf(powf(playerDirection.x, 2.0f) + powf(playerDirection.y, 2.0f));
		if (len != 0.0f) {
			//正規化
			playerDirection.x /= len;
			playerDirection.y /= len;
		}

		//プレイヤーの移動
		player.position.x += playerDirection.x * player.speed.x;
		player.position.y += playerDirection.y * player.speed.y;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		if (player.isAlive) {
			Novice::DrawBox(
				static_cast<int>(player.position.x - player.radius),
				static_cast<int>(player.position.y - player.radius),
				static_cast<int>(player.radius * 2.0f),
				static_cast<int>(player.radius * 2.0f),
				0.0f,
				player.color,
				kFillModeSolid
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
