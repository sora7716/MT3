#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <imgui.h>

const char kWindowTitle[] = "クラス_番号_シ_メイ_AL2_02_03_イージング";


struct Vector2
{
	float x;
	float y;
};

struct Ease {
	// 現在位置
	Vector2 nowPos{ 0,0 };
	// 開始位置・終了位置
	Vector2 startPos{ 40,40 };
	Vector2 endPos{ 600,60 };
	// frame.x : 今のフレーム数（経過時間）
	// frame.y : 終了フレーム（アニメーションが終わるフレーム）
	Vector2 frameCount{ 0,60 };
	// 四角形のサイズ
	Vector2 size{ 20,20 };
	// 0.0 ~ 1.0 の間で進行度を表す（t）
	float t = 0.0f;
	// イージング後のt
	float easedT = 0.0f;
};

float EaseInFunc(float t) {
	return t * t * t;
}

float EaseOutFunc(float t) {
	return 1.0f - powf(1.0f - t, 3.0f);
}

float EaseInOutFunc(float t) {
	return -(cosf(float(M_PI) * t) - 1.0f) / 2.0f;
}

float EaseInBackFunc(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float EaseOutBackFunc(float t) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);
}

void EaseTrail(Ease& ease, float (*easingFunc)(float));

void DrawEaseTrail(Ease& ease, float (*easingFunc)(float));

const int WIN_WIDTH = 700;
const int WIN_HEIGHT = 500;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	int isMoving = false;
	float easeFrame = 60.0f;

	Ease easeIn{};
	easeIn.nowPos = { 40,150 };
	easeIn.startPos = { easeIn.nowPos.x,easeIn.nowPos.y };
	easeIn.endPos = { 600,easeIn.nowPos.y };

	Ease easeOut{};
	easeOut.nowPos = { 40,200 };
	easeOut.startPos = { easeOut.nowPos.x,easeOut.nowPos.y };
	easeOut.endPos = { 600,easeOut.nowPos.y };

	Ease easeInOut{};
	easeInOut.nowPos = { 40,300 };
	easeInOut.startPos = { easeInOut.nowPos.x,easeInOut.nowPos.y };
	easeInOut.endPos = { 600,easeInOut.nowPos.y };

	Ease easeInBack{};
	easeInBack.nowPos = { 40,350 };
	easeInBack.startPos = { easeInBack.nowPos.x,easeInBack.nowPos.y };
	easeInBack.endPos = { 600,easeInBack.nowPos.y };

	Ease easeOutBack{};
	easeOutBack.nowPos = { 40,400 };
	easeOutBack.startPos = { easeOutBack.nowPos.x,easeOutBack.nowPos.y };
	easeOutBack.endPos = { 600,easeOutBack.nowPos.y };


	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		ImGui::Begin("Debug Easing");
		ImGui::Text("Frame:%.1f", easeIn.frameCount.x);
		ImGui::SliderFloat("EndFrame", &easeFrame, 0, 150);

		// スペースでアニメーション開始
		if (ImGui::Button("Easing On")) {

			easeIn.frameCount = { 0,easeFrame };
			easeOut.frameCount = { 0,easeFrame };
			easeInOut.frameCount = { 0,easeFrame };
			easeInBack.frameCount = { 0,easeFrame };
			easeOutBack.frameCount = { 0,easeFrame };

			isMoving = true;
		}

		if (isMoving) {
			EaseTrail(easeIn, EaseInFunc);
			EaseTrail(easeOut, EaseOutFunc);
			EaseTrail(easeInOut, EaseInOutFunc);
			EaseTrail(easeInBack, EaseInBackFunc);
			EaseTrail(easeOutBack, EaseOutBackFunc);
		}


		ImGui::End();
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		// ---------- 背景 ---------- //
		Novice::DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, 0.0f, 0xB4B4B4FF, kFillModeSolid);


		// ---------- 残像 ---------- //
		DrawEaseTrail(easeIn, EaseInFunc);
		DrawEaseTrail(easeOut, EaseOutFunc);
		DrawEaseTrail(easeInOut, EaseInOutFunc);
		DrawEaseTrail(easeInBack, EaseInBackFunc);
		DrawEaseTrail(easeOutBack, EaseOutBackFunc);


		// ---------- 本体 ---------- //
		Novice::DrawBox(static_cast<int>(easeIn.nowPos.x), static_cast<int>(easeIn.nowPos.y), static_cast<int>(easeIn.size.x), static_cast<int>(easeIn.size.y), 0.0f, 0xFF9696FF, kFillModeSolid);
		Novice::DrawBox(static_cast<int>(easeOut.nowPos.x), static_cast<int>(easeOut.nowPos.y), static_cast<int>(easeOut.size.x), static_cast<int>(easeOut.size.y), 0.0f, 0xFF9696FF, kFillModeSolid);
		Novice::DrawBox(static_cast<int>(easeInOut.nowPos.x), static_cast<int>(easeInOut.nowPos.y), static_cast<int>(easeInOut.size.x), static_cast<int>(easeInOut.size.y), 0.0f, 0xFF9696FF, kFillModeSolid);
		Novice::DrawBox(static_cast<int>(easeInBack.nowPos.x), static_cast<int>(easeInBack.nowPos.y), static_cast<int>(easeInBack.size.x), static_cast<int>(easeInBack.size.y), 0.0f, 0xFF9696FF, kFillModeSolid);
		Novice::DrawBox(static_cast<int>(easeOutBack.nowPos.x), static_cast<int>(easeOutBack.nowPos.y), static_cast<int>(easeOutBack.size.x), static_cast<int>(easeOutBack.size.y), 0.0f, 0xFF9696FF, kFillModeSolid);


		// ---------- デバッグ用 ---------- //
		Novice::ScreenPrintf(int(easeIn.startPos.x), int(easeIn.startPos.y) - 20, "EaseIn");
		Novice::ScreenPrintf(int(easeOut.startPos.x), int(easeOut.startPos.y) - 20, "EaseOut");
		Novice::ScreenPrintf(int(easeOut.startPos.x), int(easeOut.startPos.y) + 50, "=== exVer ===");
		Novice::ScreenPrintf(int(easeInOut.startPos.x), int(easeInOut.startPos.y) - 20, "EaseInOut");
		Novice::ScreenPrintf(int(easeInBack.startPos.x), int(easeInBack.startPos.y) - 20, "EaseInBack");
		Novice::ScreenPrintf(int(easeOutBack.startPos.x), int(easeOutBack.startPos.y) - 20, "EaseOutBack");


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

/*----------------------------------------------------------------------
 この処理では、スタート地点からゴール地点まで物体を動かすために、
 “今はどこにいるのか” を毎フレーム計算しています。

 流れとしては…

 ① 動きがどれだけ進んだか（何フレーム進んだか）を数える
 ② 動き全体の中で「今は何割の位置まで来たか」を計算する（0.0～1.0）
 ③ その割合をイージングに通して、動き方（ゆっくり・加速）を調整する
 ④ 調整された割合に応じて「今の位置」を計算する

 つまり、この関数の目的は
 「現在位置を求めること」だけです。
 ----------------------------------------------------------------------*/
void EaseTrail(Ease& ease, float (*easingFunc)(float)) {
	// ① 動きがまだ終わっていないなら、1フレームぶん時間を進める
	if (ease.frameCount.x < ease.frameCount.y)
	{
		ease.frameCount.x++;
	}

	// ② 全体の中で、今どれだけ進んだかを 0.0～1.0 の数字にする
	//    例：全60フレーム中30フレ進んでいたら → 30/60 = 0.5（50%）
	ease.t = ease.frameCount.x / ease.frameCount.y;

	// ③ ただの割合（t）だと直線的な動きになるので、
	//    イージング関数を使って「動き方のカーブ」をつける
	ease.easedT = easingFunc(ease.t);

	// ④ easedT の値（0.0～1.0）をもとにして、
	//    スタート位置とゴール位置の間の「今の位置」を計算する
	ease.nowPos.x = (1.0f - ease.easedT) * ease.startPos.x + ease.easedT * ease.endPos.x;
}


/*----------------------------------------------------------------------
 この処理では、「いまの位置に来るまでに通ってきた場所」を
 もう一度フレームごとに計算し直して、その場所に薄い四角を描いています。

 流れとしては…

 ① 0フレーム目～今のフレームの1つ手前までを順番に見る
 ② そのフレームのとき、全体の何割進んでいたか（0.0～1.0）を計算する
 ③ その割合をイージングに通して、その時点の位置を求める
 ④ 求めた位置に、半透明の四角を描く（＝残像として並べる）

 これを毎フレーム繰り返すことで、「通ってきた軌跡」が線のように見える。
 ----------------------------------------------------------------------*/
void DrawEaseTrail(Ease& ease, float (*easingFunc)(float)) {
	// ① 0フレーム目～今のフレームの1つ手前までを順番に処理する
	for (int i = 0; i < ease.frameCount.x; i++)
	{
		// 描く回数が多すぎるとベタ塗りになるので、2フレームに1回だけ描くように間引く
		if (i % 2 == 0)
		{
			// ② 「このフレーム(i)のとき、全体の何割進んでいたか」を 0.0～1.0 の値で出す
			//    例: 全60フレーム中 i=30 なら → 30/60 = 0.5（半分まで進んでいた）
			ease.t = i / static_cast<float>(ease.frameCount.y);

			// ③ 上で出した「何割進んでいたか」をイージングに通して、
			//    そのフレームにおける“動き方（ゆっくり・加速など）”を反映した値に変える
			ease.easedT = easingFunc(ease.t);

			// ④ イージング後の値を使って、
			//    「このフレームのときは、スタートとゴールの間のどの位置にいたか」を計算し、
			//    その場所に薄い四角（残像用の四角）を描く
			Novice::DrawBox
			(
				static_cast<int>((1.0f - ease.easedT) * ease.startPos.x + ease.easedT * ease.endPos.x),
				static_cast<int>(ease.nowPos.y),
				static_cast<int>(ease.size.x), static_cast<int>(ease.size.y),
				0.0f, 0xFFFFFF55, kFillModeSolid
			);
		}
	}
}