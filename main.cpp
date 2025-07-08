#include "myMath/func/Easing.h"
#include <Novice.h>
#include <cstdint>
#include <imgui.h>
const int kAfterImageNum = 700;

// 2次元のベクトル
typedef struct Vector2 {
	float x;
	float y;
} Vector2;

// ゲームオブジェクト
typedef struct GameObject {
	Vector2 pos; // 位置
	float size;  // 半径
} GameObject;

// 線形補間に必要なもの
typedef struct LerpData {
	GameObject gameObject; // ゲームオブジェクト
	Vector2 beginPos;      // 開始位置
	Vector2 endPos;        // 終了位置
} LerpData;

const char kWindowTitle[] = "02_02_演出王:イージング";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 750, 550);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// イージングのラベル
	const char* easingLabel[static_cast<int32_t>(kEasingTypeCount)] = {
		"Ease In", "Ease Out", "Ease InOut", "Ease InBack", "Ease OutBack" };
	// イージングのラベルのダブルポインタ
	char** pp_easingLabel = const_cast<char**>(easingLabel);

	// 線形補間する数
	LerpData lerpData[static_cast<int32_t>(kEasingTypeCount)] = {};

	// 基準
	Vector2 origin = { 100.0f, 200.0f };

	// イージングの種類ごとの位置を初期化
	for (int32_t i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
		lerpData[i].gameObject.size = 64.0f; // サイズ設定
		lerpData[i].gameObject.pos = {
			origin.x,
			origin.y + (lerpData[i].gameObject.size * static_cast<float>(i)) }; // 現在地を設定
		lerpData[i].beginPos = lerpData[i].gameObject.pos;                     // 初期位置を設定
		lerpData[i].endPos = {
			origin.x + 500.0f,
			origin.y + (lerpData[i].gameObject.size * static_cast<float>(i)) }; // 最終位置を設定
	}

	// インバックだけは開始位置をずらす
	lerpData[static_cast<int32_t>(kInBack)].beginPos.x = 100.0f;

	// イージングのインスタンスを生成
	Easing* easing = new Easing();

	// 動いているかどうか
	bool isMove = false;
	// イージングが開始したかどうか
	bool isEasingStart = false;

	// 最終フレーム
	float endFrame = 60.0f;

#pragma region 残像
	float afterImageTimer = 0;

	Vector2 afterImage[static_cast<int32_t>(kEasingTypeCount)][kAfterImageNum] = { 0 };
	bool isEasingAfter[kAfterImageNum] = { false };
#pragma endregion
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

		// スペース押すと動き出す
		if (isEasingStart) {
			// スペース押されていないなら
			if (!isMove) {
				// スペース押された
				isMove = true;
				// フレームの初期化
				easing->SetFrame(0.0f);
				// エンドフレームのセット
				easing->SetEndFrame(endFrame);
				for (int i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
					// 赤いボックスの初期化
					lerpData[i].gameObject.pos = lerpData[i].beginPos;
					// 残像を初期化
					for (int j = 0; j < kAfterImageNum; j++) {
						// 残像の生存フラグを初期化
						isEasingAfter[j] = false;
						// 残像の位置を初期化
						afterImage[i][j] = lerpData[i].beginPos;
					}
				}
				// イージングの開始フラグを折る
				isEasingStart = false;
			}
		}

		// フレームの動き
		if (isMove) {
			easing->Update();
		}

		// もしフレームが1になっていれば
		if (easing->GetFrame() >= easing->GetEndFrame()) {
			isMove = false;
		}

		// 残像同士の距離感
		if (afterImageTimer > 0) {
			if (isMove) {
				afterImageTimer--;
			}
		} else {
			afterImageTimer = 1;
		}

		// 残像の位置
		if (afterImageTimer <= 0 && isMove) {
			for (int i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
				for (int j = 0; j < kAfterImageNum; j++) {
					// もし残像が生成されていなければ
					if (!isEasingAfter[j]) {
						// 残像を生成
						isEasingAfter[j] = true;
						// 残像の位置を更新
						afterImage[i][j] = lerpData[i].gameObject.pos;
						break;
					}
				}
			}
		}

		// イージングの動き
		for (int32_t i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
			lerpData[i].gameObject.pos.x =
				lerpData[i].beginPos.x + (lerpData[i].endPos.x - lerpData[i].beginPos.x) *
				easing->GetEasing(static_cast<EasingType>(i));
		}

		ImGui::Begin("Debug Easing");
		ImGui::Text("Frame:%.1f", easing->GetFrame());
		ImGui::DragFloat("EndFrame", &endFrame, 0.1f, 0.0f, 180.0f, "%.1f");
		if (ImGui::Button("Easing On")) {
			if (!isMove) {
				isEasingStart ^= true;
			}
		}
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 残像
		for (int32_t i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
			for (int32_t j = 0; j < kAfterImageNum; j++) {
				if (isEasingAfter[j]) {
					Novice::DrawBox(
						static_cast<int32_t>(afterImage[i][j].x),
						static_cast<int32_t>(afterImage[i][j].y),
						static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f),
						static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f), 0.0f,
						0xC4C4C455,
						kFillModeSolid);
				}
			}
		}

		// 赤いボックスの最終地点
		for (int32_t i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
			Novice::DrawBox(
				static_cast<int32_t>(lerpData[i].endPos.x),
				static_cast<int32_t>(lerpData[i].endPos.y),
				static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f),
				static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f), 0.0f, 0xFF000055,
				kFillModeSolid);
		}

		// 赤いボックスの描画
		for (int32_t i = 0; i < static_cast<int32_t>(kEasingTypeCount); i++) {
			Novice::ScreenPrintf(
				static_cast<int32_t>(lerpData[i].beginPos.x),
				static_cast<int32_t>(
					lerpData[i].gameObject.pos.y - (lerpData[i].gameObject.size / 4.0f)),
				"%s", *(pp_easingLabel + i));
			Novice::DrawBox(
				static_cast<int32_t>(lerpData[i].gameObject.pos.x),
				static_cast<int32_t>(lerpData[i].gameObject.pos.y),
				static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f),
				static_cast<int32_t>(lerpData[i].gameObject.size / 2.0f), 0.0f, RED,
				kFillModeSolid);
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

	delete easing;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}