#include <Novice.h>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

//タイトル
const char kWindowTitle[] = "イイヅカ_ソラ";

//画面
const float kWindowWidth = 1280.0f;//横幅
const float kWindowHeight = 720.0f;//縦幅

//敵の数
const int kEnemyCount = 3;

//スコア
const int kAddScore = 100;
const int kSubScore = -50;

//頂点の位置
enum VertexPosition {
	kLeft,//左
	kTop,//上
	kRight,//右
	kVertexCount//頂点数
};

//2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
}Vector2;

//ゲームオブジェクト
typedef struct GameObject {
	Vector2 position;//現在の位置
	Vector2 velocity;//速度
	float radius;//半径
	unsigned int color;//色
	int isAlive;//生存フラグ
}GameObject;

//回転するデータ
typedef struct RotateData {
	float theta;//θ
	Vector2 centroid;//重心
	Vector2 localVertices[static_cast<int>(kVertexCount)];//ローカル座標
	Vector2 screenVertices[static_cast<int>(kVertexCount)];//スクリーン座標
}RotateData;

//プレイヤー
typedef struct PlayerData {
	GameObject gameObject;//ゲームオブジェクト
	RotateData rotateData;//回転のデータ
	Vector2 direction;//向いている方向
}PlayerData;

//敵
typedef struct EnemyData {
	GameObject gameObject;//ゲームオブジェクト
	Vector2 direction;//向いている方向
}EnemyData;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//プレイヤー
	PlayerData player = {};
	player.gameObject.position = { kWindowWidth / 2.0f,kWindowHeight / 2.0f };//初期位置
	player.gameObject.radius = 50.0f;//サイズ
	player.gameObject.color = WHITE;//色
	player.gameObject.isAlive = true;//生存している

	//ローカル座標を設定
	player.rotateData.localVertices[static_cast<int>(kLeft)] = { -player.gameObject.radius,player.gameObject.radius };
	player.rotateData.localVertices[static_cast<int>(kTop)] = { 0.0f,-player.gameObject.radius };
	player.rotateData.localVertices[static_cast<int>(kRight)] = { player.gameObject.radius,player.gameObject.radius };

	//重心を求める
	player.rotateData.centroid.x = (player.rotateData.localVertices[static_cast<int>(kLeft)].x + player.rotateData.localVertices[static_cast<int>(kTop)].x + player.rotateData.localVertices[static_cast<int>(kRight)].x) / 3.0f;

	player.rotateData.centroid.y = (player.rotateData.localVertices[static_cast<int>(kLeft)].y + player.rotateData.localVertices[static_cast<int>(kTop)].y + player.rotateData.localVertices[static_cast<int>(kRight)].y) / 3.0f;

	//重心をローカルの中心に合わせる
	for (int i = 0; i < static_cast<int>(kVertexCount); i++) {
		player.rotateData.localVertices[i].x -= player.rotateData.centroid.x;
		player.rotateData.localVertices[i].y -= player.rotateData.centroid.y;
	}

	//弾
	GameObject bullet = {};
	bullet.velocity = { 10.0f,10.0f };//速度
	bullet.radius = 10.0f;//サイズ
	bullet.color = RED;//色
	bullet.isAlive = false;//生存していない

	//敵
	EnemyData enemies[kEnemyCount] = {};
	//敵の初期化
	for (int i = 0; i < kEnemyCount; i++) {
		enemies[i].gameObject.position = { 100.0f * static_cast<float>(i + 1),100.0f * static_cast<float>(i + 1) };//初期位置
		enemies[i].gameObject.color = WHITE;//色
		enemies[i].gameObject.radius = 30.0f;//サイズ
		enemies[i].gameObject.isAlive = true;//生存している
	}

	//向き合っているかを判定
	Vector2 playerCenterToPlayerTop = {};//プレイヤーの中心からプレイヤーの天辺
	Vector2 enemyToPlayer = {};//敵からプレイヤー
	Vector2 directionNormal[2] = {};//向いている方向の正規化ベクトル

	//スコア
	int score = 0;

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

		//回転角を初期化
		player.rotateData.theta += 0.01f;

		//スペースを押したら
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			//弾が生存していないなら
			if (!bullet.isAlive) {
				//球を打つフラグをtrueにする
				bullet.isAlive = true;
				//プレイヤーの向いている方向を求める
				player.direction.x = player.rotateData.screenVertices[kTop].x - player.gameObject.position.x;
				player.direction.y = player.rotateData.screenVertices[kTop].y - player.gameObject.position.y;
				//長さを求める
				float length = sqrtf(powf(player.direction.x, 2.0f) + powf(player.direction.y, 2.0f));
				//正規化をする
				if (length != 0.0f) {
					player.direction.x /= length;
					player.direction.y /= length;
				}
				//球の位置をプレイヤーの位置にする
				bullet.position = player.rotateData.screenVertices[kTop];
			}
		}

		//プレイヤーが生存しているなら
		if (player.gameObject.isAlive) {
			for (int i = 0; i < static_cast<int>(kVertexCount); i++) {
				//加法定理を用いて回転
				player.rotateData.screenVertices[i].x = player.rotateData.localVertices[i].x * cosf(player.rotateData.theta) - player.rotateData.localVertices[i].y * sinf(player.rotateData.theta);
				player.rotateData.screenVertices[i].y = player.rotateData.localVertices[i].y * cosf(player.rotateData.theta) + player.rotateData.localVertices[i].x * sinf(player.rotateData.theta);

				//回転位置を動かす
				player.rotateData.screenVertices[i].x += player.gameObject.position.x;
				player.rotateData.screenVertices[i].y += player.gameObject.position.y;
			}
		}

		//弾の挙動
		if (bullet.isAlive) {
			bullet.position.x += player.direction.x * bullet.velocity.x;//弾の位置を更新
			bullet.position.y += player.direction.y * bullet.velocity.y;//弾の位置を更新

			//画面外に出たら
			if (bullet.position.x > kWindowWidth || bullet.position.x<0.0f ||
				bullet.position.y>kWindowHeight || bullet.position.y < 0.0f) {
				bullet.isAlive = false;//弾を消す
				if (score > 0) {
					score += kSubScore;//スコアの減産
				}
			}
		}

		//衝突判定
		for (int i = 0; i < kEnemyCount; i++) {
			//敵と弾が生存しているなら
			if (enemies[i].gameObject.isAlive && bullet.isAlive) {
				//敵から弾の差分ベクトル
				Vector2 enemyToBullet = { bullet.position.x - enemies[i].gameObject.position.x, bullet.position.y - enemies[i].gameObject.position.y };
				//敵と球の距離を求める
				float distance = sqrtf(powf(enemyToBullet.x, 2.0f) + powf(enemyToBullet.y, 2.0f));
				//敵と弾の距離と敵と弾の半径の和の比較
				if (distance < bullet.radius + enemies[i].gameObject.radius) {
					enemies[i].gameObject.isAlive = false;//敵を消す
					bullet.isAlive = false;//弾を消す
					score += kAddScore;//スコアを加算
				}
			}
		}

		//プレイヤーの向いている方向に敵がいたら敵の色を変更する
		//プレイヤーの中心からプレイヤーのてっぺんを見てる
		//敵の数分回す
		for (int i = 0; i < kEnemyCount; i++) {
			if (enemies[i].gameObject.isAlive) {
				playerCenterToPlayerTop = {
					player.rotateData.screenVertices[static_cast<int>(kTop)].x - player.gameObject.position.x,
					player.rotateData.screenVertices[static_cast<int>(kTop)].y - player.gameObject.position.y
				};
				//長さを求める
				float len = sqrtf(powf(playerCenterToPlayerTop.x, 2.0f) + powf(playerCenterToPlayerTop.y, 2.0f));
				//正規化する
				if (len != 0.0f) {
					directionNormal[0].x = playerCenterToPlayerTop.x / len;
					directionNormal[0].y = playerCenterToPlayerTop.y / len;
				}
				//敵からプレイヤーを見る
				enemyToPlayer = {
					player.gameObject.position.x - enemies[i].gameObject.position.x,
					player.gameObject.position.y - enemies[i].gameObject.position.y
				};

				//長さを求める
				len = sqrtf(powf(enemyToPlayer.x, 2.0f) + powf(enemyToPlayer.y, 2.0f));

				//正規化をする
				if (len != 0.0f) {
					directionNormal[1].x = enemyToPlayer.x / len;
					directionNormal[1].y = enemyToPlayer.y / len;
				}

				//内積をとる
				float dot = directionNormal[0].x * directionNormal[1].x + directionNormal[0].y * directionNormal[1].y;

				//向き合っていたら
				if (dot < 0.0f) {
					enemies[i].gameObject.color = BLUE;
				} else {
					enemies[i].gameObject.color = WHITE;
				}
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//プレイヤーが生存しているなら
		if (player.gameObject.isAlive) {
			Novice::DrawTriangle(
				static_cast<int>(player.rotateData.screenVertices[kLeft].x),
				static_cast<int>(player.rotateData.screenVertices[kLeft].y),
				static_cast<int>(player.rotateData.screenVertices[kTop].x),
				static_cast<int>(player.rotateData.screenVertices[kTop].y),
				static_cast<int>(player.rotateData.screenVertices[kRight].x),
				static_cast<int>(player.rotateData.screenVertices[kRight].y),
				player.gameObject.color, kFillModeSolid
			);
		}

		//弾が生存しているなら
		if (bullet.isAlive) {
			Novice::DrawEllipse(
				static_cast<int>(bullet.position.x),
				static_cast<int>(bullet.position.y),
				static_cast<int>(bullet.radius),
				static_cast<int>(bullet.radius),
				0.0f, bullet.color, kFillModeSolid
			);
		}

		//敵の描画
		for (int i = 0; i < kEnemyCount; i++) {
			//敵が生存しているなら
			if (enemies[i].gameObject.isAlive) {
				Novice::DrawEllipse(
					static_cast<int>(enemies[i].gameObject.position.x),
					static_cast<int>(enemies[i].gameObject.position.y),
					static_cast<int>(enemies[i].gameObject.radius),
					static_cast<int>(enemies[i].gameObject.radius),
					0.0f, enemies[i].gameObject.color, kFillModeSolid
				);
			}
		}

		//スコアの表示
		Novice::ScreenPrintf(0, 0, "score:%d", score);

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