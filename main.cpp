#include <Novice.h>
#include <imgui.h>
const char kWindowTitle[] = "AL1_スクロール";
//画面サイズ
const float kWindowWidth = 1280.0f;//幅
const float kWindowHeight = 720.0f;//高さ
//背景のテクスチャの数
const int kBgImageNum = 4;
//スピード
const float kSpeed = 5.0f;
//背景の余裕
const float bgBuffer = 10;
//マップ
const float kMapMinPos = 0.0f;//最小値
const float kMapMaxPos = kWindowWidth * static_cast<float>(kBgImageNum);//最大値

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
typedef struct BgListData {
	GameObject gameObject[kBgImageNum];
	int textureHandle[kBgImageNum];
	int begin;
	int end;
}BgData;

//プレイヤーデータ
typedef struct PlayerData {
	GameObject gameObject;
	float size;
	Vector2 worldPos;
	Vector2 worldVelocity;
}PlayerData;

//線分
typedef struct Segment {
	Vector2 origin;
	Vector2 diff;
	Vector2 velocity;
}Segment;

//スクロールデータ
typedef struct ScrollData {
	Vector2 startPos;
	Vector2 endPos;
	Vector2 position;
	Vector2 velocity;
	bool isScroll;
}ScrollData;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//背景のオブジェクト
	BgListData bgObjectList = {};
	bgObjectList.textureHandle[0] = Novice::LoadTexture("./images/bg1.png");
	bgObjectList.textureHandle[1] = Novice::LoadTexture("./images/bg2.png");
	bgObjectList.textureHandle[2] = Novice::LoadTexture("./images/bg3.png");
	bgObjectList.textureHandle[3] = Novice::LoadTexture("./images/bg4.png");
	//初期化
	for (int i = 0; i < kBgImageNum; i++) {
		bgObjectList.gameObject[i].position.x = kWindowWidth * i;
		bgObjectList.gameObject[i].color = WHITE;
		bgObjectList.gameObject[i].isAlive = true;
	}
	//背景の最後の位置
	bgObjectList.begin = 0;
	bgObjectList.end = kBgImageNum - 1;

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

	//スクロール
	ScrollData scroll = {};
	scroll.isScroll = false;

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
		//ワールド座標
		//移動
		player.worldPos.x += player.worldVelocity.x;
		player.worldPos.y -= player.worldVelocity.y;

		//操作
		if (keys[DIK_D]) {
			player.worldVelocity.x = kSpeed;
		} else if (keys[DIK_A]) {
			player.worldVelocity.x = -kSpeed;
		} else {
			player.worldVelocity.x = 0.0f;
		}

		//スクロールの動き
		//移動
		scroll.position.x += scroll.velocity.x;
		scroll.position.y -= scroll.velocity.y;

		//操作
		if (scroll.isScroll) {
			if (keys[DIK_D]) {
				scroll.velocity.x = kSpeed;
			} else if (keys[DIK_A]) {
				scroll.velocity.x = -kSpeed;
			} else {
				scroll.velocity.x = 0.0f;
			}
		}

		//スクロールの制限
		scroll.startPos.x = kMapMinPos + segment.origin.x;
		scroll.endPos.x = kMapMaxPos - segment.origin.x;

		if(player.gameObject.position.x)
		if (player.worldPos.x > scroll.startPos.x) {
			scroll.isScroll = true;
		} else {
			scroll.isScroll = false;
		}

		if (player.worldPos.x >= scroll.endPos.x) {
			scroll.isScroll = false;
		}

		//画面の制限
		/*if (player.worldPos.x <= 0.0f) {
			bgObjectList.gameObject[bgBeginFrame].position.x = 0.0f;
			scroll.isScroll = false;
		} else if (player.worldPos.x > kWindowWidth * static_cast<float>(kBgImageNum - 1)) {
			bgObjectList.gameObject[bgEndFrame].position.x = 0.0f;
			scroll.isScroll = false;
		}*/

		//背景
		for (int i = 0; i < kBgImageNum; i++) {
			//移動
			bgObjectList.gameObject[i].position.x += bgObjectList.gameObject[i].velocity.x;
			bgObjectList.gameObject[i].position.y -= bgObjectList.gameObject[i].velocity.y;

			//操作
			if (scroll.isScroll) {
				if (keys[DIK_D]) {
					bgObjectList.gameObject[i].velocity.x = -kSpeed;
				} else if (keys[DIK_A]) {
					bgObjectList.gameObject[i].velocity.x = kSpeed;
				} else {
					bgObjectList.gameObject[i].velocity.x = 0.0f;
				}
			} else {
				bgObjectList.gameObject[i].velocity.x = 0.0f;
			}
		}

		//プレイヤーの動き
		//移動
		player.gameObject.position.x += player.gameObject.velocity.x;
		player.gameObject.position.y -= player.gameObject.velocity.y;
		//操作
		if (!scroll.isScroll) {
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
		segment.origin.y -= segment.velocity.y;

		//操作
		if (keys[DIK_RIGHT]) {
			segment.velocity.x = kSpeed;
		} else if (keys[DIK_LEFT]) {
			segment.velocity.x = -kSpeed;
		} else {
			segment.velocity.x = 0.0f;
		}



		ImGui::Begin("bg");
		ImGui::Checkbox("isScroll", &scroll.isScroll);
		ImGui::DragFloat2("segmentOrigin", &segment.origin.x, 0.1f);
		ImGui::Text("mapMin%f", kMapMinPos);
		ImGui::Text("scrollStart%f", scroll.startPos.x);
		ImGui::Text("mapMax%f", kMapMaxPos);
		ImGui::Text("scrollEnd%f", scroll.endPos.x);
		ImGui::DragFloat2("bg[0].pos", &bgObjectList.gameObject[0].position.x, 0.1f);
		ImGui::DragFloat2("bg[1].pos", &bgObjectList.gameObject[1].position.x, 0.1f);
		ImGui::DragFloat2("bg[2].pos", &bgObjectList.gameObject[2].position.x, 0.1f);
		ImGui::DragFloat2("bg[3].pos", &bgObjectList.gameObject[3].position.x, 0.1f);
		ImGui::End();

		ImGui::Begin("player");
		ImGui::DragFloat2("position", &player.gameObject.position.x, 0.1f);
		ImGui::Text("worldPos:%f", player.worldPos.x);
		ImGui::Text("seigen:%f", kWindowWidth * static_cast<float>(kBgImageNum - 1));
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//背景の描画
		for (int i = 0; i < kBgImageNum; i++) {
			if (bgObjectList.gameObject[i].isAlive) {
				Novice::DrawSprite(
					static_cast<int>(bgObjectList.gameObject[i].position.x),
					static_cast<int>(bgObjectList.gameObject[i].position.y),
					bgObjectList.textureHandle[i],
					1.0f, 1.0f, 0.0f, bgObjectList.gameObject[i].color
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
