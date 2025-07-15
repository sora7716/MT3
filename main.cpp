#include <Novice.h>
#include <imgui.h>
const char kWindowTitle[] = "AL1_スクロール";
//画面サイズ
const float kWindowWidth = 1280.0f;//幅
const float kWindowHeight = 720.0f;//高さ

//2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;

	//加算
	Vector2 operator+(Vector2& v) {
		Vector2 result = {};
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	//減算
	Vector2 operator-(Vector2& v) {
		Vector2 result = {};
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	/// <summary>
	/// クロス積
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>クロス積</returns>
	float Cross(const Vector2& v) {
		return x * v.y - y * v.x;
	}
}Vector2;

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;
	Vector2 velocity;
	unsigned int color;
	bool isAlive;
}GameObject;

//背景データ
typedef struct BgData {
	GameObject gameObject;
	int textureHandle;
}BgData;

//プレイヤーデータ
typedef struct PlayerData {
	GameObject gameObject;
	float size;
}PlayerData;

//線分
typedef struct Segment {
	Vector2 origin;
	Vector2 diff;
	Vector2 velocity;
}Segment;

//背景のテクスチャの数
const int kBgImageNum = 4;

//スピード
const float kSpeed = 5.0f;

//背景の余裕
const float bgBuffer = 10;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//背景のオブジェクト
	BgData bgObjects[kBgImageNum] = {};
	bgObjects[0].textureHandle = Novice::LoadTexture("./images/bg1.png");
	bgObjects[1].textureHandle = Novice::LoadTexture("./images/bg2.png");
	bgObjects[2].textureHandle = Novice::LoadTexture("./images/bg3.png");
	bgObjects[3].textureHandle = Novice::LoadTexture("./images/bg4.png");
	//初期化
	for (int i = 0; i < kBgImageNum; i++) {
		bgObjects[i].gameObject.position.x = kWindowWidth * i;
		bgObjects[i].gameObject.color = WHITE;
		bgObjects[i].gameObject.isAlive = true;
	}
	//背景の最後の位置
	int bgEndFrame = kBgImageNum - 1;
	//スクロール開始フラグ
	bool isScrolling = false;

	//プレイヤー
	PlayerData player = {};
	player.gameObject.position = { 0.0f,563.0f };
	player.gameObject.color = RED;
	player.gameObject.isAlive = true;
	player.size = 30.0f;

	//線
	Segment segment = {};
	segment.origin.x = kWindowWidth / 2.0f;
	segment.diff = { 0.0f,kWindowHeight };
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

		//背景
		for (int i = 0; i < kBgImageNum; i++) {
			//移動
			bgObjects[i].gameObject.position.x += bgObjects[i].gameObject.velocity.x;
			bgObjects[i].gameObject.position.y += bgObjects[i].gameObject.velocity.y;

			//背景の操作
			if (isScrolling) {
				if (keys[DIK_D]) {
					bgObjects[i].gameObject.velocity.x = -kSpeed;
				} else if (keys[DIK_A]) {
					bgObjects[i].gameObject.velocity.x = kSpeed;
				} else {
					bgObjects[i].gameObject.velocity.x = 0.0f;
				}
			} else {
				bgObjects[i].gameObject.velocity.x = 0.0f;
			}
		}

		//背景の最後の位置が左端に来たら
		if (bgObjects[bgEndFrame].gameObject.position.x <= 0.0f + bgBuffer) {
			isScrolling = false;
		}

		//プレイヤー
		//移動
		player.gameObject.position.x += player.gameObject.velocity.x;
		player.gameObject.position.y += player.gameObject.velocity.y;
		//スクロールがfalseだった場合
		if (!isScrolling) {
			if (keys[DIK_D]) {
				player.gameObject.velocity.x = kSpeed;
			} else if (keys[DIK_A]) {
				player.gameObject.velocity.x = -kSpeed;
			} else {
				player.gameObject.velocity.x = 0.0f;
			}
		} else {
			player.gameObject.velocity.x = 0.0f;
		}

		//境界線の動き
		//移動
		segment.origin.x += segment.velocity.x;
		segment.origin.y += segment.velocity.y;
		//境界線の操作
		if (keys[DIK_RIGHT]) {
			segment.velocity.x = kSpeed;
		} else if (keys[DIK_LEFT]) {
			segment.velocity.x = -kSpeed;
		} else {
			segment.velocity.x = 0.0f;
		}

		ImGui::Begin("bg");
		ImGui::Checkbox("isScrolling", &isScrolling);
		ImGui::End();

		ImGui::Begin("player");
		ImGui::DragFloat2("position", &player.gameObject.position.x, 0.1f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//背景の描画
		for (const auto& bgObject : bgObjects) {
			if (bgObject.gameObject.isAlive) {
				Novice::DrawSprite(
					static_cast<int>(bgObject.gameObject.position.x),
					static_cast<int>(bgObject.gameObject.position.y),
					bgObject.textureHandle,
					1.0f, 1.0f, 0.0f, bgObject.gameObject.color
				);
			}
		}

		//プレイヤー
		if (player.gameObject.isAlive) {
			Novice::DrawBox(
				static_cast<int>(player.gameObject.position.x),
				static_cast<int>(player.gameObject.position.y),
				static_cast<int>(player.size),
				static_cast<int>(player.size),
				0.0f, player.gameObject.color, kFillModeSolid
			);
		}

		//スクロールする境界線
		Novice::DrawLine(
			static_cast<int>(segment.origin.x),
			static_cast<int>(segment.origin.y),
			static_cast<int>(segment.origin.x + segment.diff.x),
			static_cast<int>(segment.origin.y + segment.diff.y),
			RED
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
