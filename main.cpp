#include <Novice.h>
#include <imgui.h>
const char kWindowTitle[] = "AL1_スクロール";
//画面サイズ
const float kWindowWidth = 1280.0f;//幅
const float kWindowHeight = 720.0f;//高さ

//背景の画像数
const int kBgTexNum = 4;
//スピードの定数
const float kSpeed = 10.0f;

//二次元のベクトル
typedef struct Vector2 {
	float x;
	float y;

	//加算
	Vector2& operator+=(Vector2 v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	}
}Vector2;

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	unsigned int color;
}GameObject;

//背景リストのデータ
typedef struct BgListData {
	GameObject gameObject[kBgTexNum];
	int textureHandle[kBgTexNum];
}BgListData;

//スクロール
typedef struct ScrollData {
	Vector2 position;
	Vector2 velocity;
	Vector2 startPos;
	Vector2 endPos;
	bool isStart;
}ScrollData;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//背景
	BgListData bgList = {};
	//画像の読み込み
	bgList.textureHandle[0] = Novice::LoadTexture("./images/bg1.png");
	bgList.textureHandle[1] = Novice::LoadTexture("./images/bg2.png");
	bgList.textureHandle[2] = Novice::LoadTexture("./images/bg3.png");
	bgList.textureHandle[3] = Novice::LoadTexture("./images/bg4.png");

	//ゲームオブジェクトの初期化
	for (int i = 0; i < kBgTexNum; i++) {
		bgList.gameObject[i].position.x = kWindowWidth * i;
		bgList.gameObject[i].color = WHITE;
	}

	//スクロール
	ScrollData scroll = {};
	scroll.startPos.x = kWindowWidth / 2.0f;
	scroll.endPos.x = kWindowWidth * static_cast<float>(kBgTexNum - 1);
	scroll.isStart = false;

	//プレイヤー
	GameObject player = {};
	player.position.y = -63.0f;
	player.size = { 30.0f,30.0f };
	player.color = BLUE;

	//境界線の速度
	Vector2 lineVelocity = {};

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

		//スクロール
		if (scroll.isStart) {
			//キー入力で操作
			if (keys[DIK_A]) {
				scroll.velocity.x = kSpeed;
			} else if (keys[DIK_D]) {
				scroll.velocity.x = -kSpeed;
			} else {
				scroll.velocity.x = 0.0f;
			}

			//スクロールの移動
			scroll.position += scroll.velocity;

			for (int i = 0; i < kBgTexNum; i++) {
				//背景の移動
				bgList.gameObject[i].position += scroll.velocity;
			}

			//スクロールの制限
			if (scroll.position.x < -kWindowWidth * static_cast<float>(kBgTexNum - 1)) {
				scroll.position.x = -kWindowWidth * static_cast<float>(kBgTexNum - 1);
				scroll.isStart = false;
				for (int i = 0; i < kBgTexNum; i++) {
					bgList.gameObject[(kBgTexNum - 1) - i].position.x = -kWindowWidth * static_cast<float>(i);
				}
			} else if (scroll.position.x > 0.0f) {
				scroll.position.x = 0.0f;
				scroll.isStart = false;
				for (int i = 0; i < kBgTexNum; i++) {
					bgList.gameObject[i].position.x = kWindowWidth * static_cast<float>(i);
				}
			}

			//線から外れたら
			if (player.position.x + player.size.x < scroll.startPos.x) {
				scroll.isStart = false;
			}
		}

		//プレイヤー
		if (!scroll.isStart) {
			//キー入力で操作
			if (keys[DIK_A]) {
				player.velocity.x = -kSpeed;
			} else if (keys[DIK_D]) {
				player.velocity.x = kSpeed;
			} else {
				player.velocity.x = 0.0f;
			}

			//プレイヤーの移動
			player.position += player.velocity;

			//プレイヤーの制限
			if (player.position.x < 0.0f) {
				player.position.x = 0.0f;
			} else if (player.position.x > kWindowWidth - player.size.x) {
				player.position.x = kWindowWidth - player.size.x;
			}

			//スクロールを開始するか
			if (bgList.gameObject[0].position.x >= 0.0f) {
				if (player.position.x + player.size.x > scroll.startPos.x) {
					scroll.isStart = true;
					player.position.x = scroll.startPos.x - player.size.x;
				}
			} else if (bgList.gameObject[kBgTexNum - 1].position.x >= 0.0f) {
				if (player.position.x + player.size.x < scroll.startPos.x) {
					scroll.isStart = true;
					player.position.x = scroll.startPos.x - player.size.x;
				}
			}
		}

		//プレイヤーの座標を変換
		float playerWorldPosY = (player.position.y - 500.0f) * -1.0f;

		//境界線
		//移動
		scroll.startPos += lineVelocity;

		//キー入力で操作
		if (keys[DIK_RIGHT]) {
			lineVelocity.x = kSpeed;
		} else if (keys[DIK_LEFT]) {
			lineVelocity.x = -kSpeed;
		} else {
			lineVelocity.x = 0.0f;
		}

		ImGui::Begin("scroll");
		ImGui::DragFloat2("position", &scroll.position.x, 0.1f);
		ImGui::DragFloat2("velocity", &scroll.velocity.x, 0.1f);
		ImGui::DragFloat2("bg[0].position", &bgList.gameObject[0].position.x, 0.1f);
		ImGui::DragFloat2("bg[1].position", &bgList.gameObject[1].position.x, 0.1f);
		ImGui::DragFloat2("bg[2].position", &bgList.gameObject[2].position.x, 0.1f);
		ImGui::DragFloat2("bg[3].position", &bgList.gameObject[3].position.x, 0.1f);
		ImGui::End();

		ImGui::Begin("player");
		ImGui::DragFloat2("position", &player.position.x, 0.1f);
		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//背景
		for (int i = 0; i < kBgTexNum; i++) {
			Novice::DrawSprite(
				static_cast<int>(bgList.gameObject[i].position.x),
				static_cast<int>(bgList.gameObject[i].position.y),
				bgList.textureHandle[i],
				1.0f, 1.0f, 0.0f, bgList.gameObject[i].color
			);
		}

		//プレイヤー
		Novice::DrawBox(
			static_cast<int>(player.position.x),
			static_cast<int>(playerWorldPosY),
			static_cast<int>(player.size.x),
			static_cast<int>(player.size.y),
			0.0f, player.color, kFillModeSolid
		);

		Novice::DrawLine(
			static_cast<int>(scroll.startPos.x),
			0,
			static_cast<int>(scroll.startPos.x),
			static_cast<int>(kWindowHeight),
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
