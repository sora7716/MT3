#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

//イージングの種類
enum EasingType {
	kIn,
	kOut,
	kInOut,
	kInBack,
	kOutBack,
	kEasingTypeCount,
};

class Easing {
public://メンバ変数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Easing() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Easing() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// easingのゲッター
	/// </summary>
	/// <param name="easingType">easingタイプ</param>
	/// <returns>easing</returns>
	float GetEasing(EasingType easingType);

	/// <summary>
	/// フレームのセッター
	/// </summary>
	/// <param name="frame">フレーム</param>
	void SetFrame(float frame);

	/// <summary>
	/// エンドフレームのセッター
	/// </summary>
	/// <param name="endFrame">エンドフレーム</param>
	void SetEndFrame(float endFrame);

	/// <summary>
	/// フレームのゲッター
	/// </summary>
	/// <returns>フレーム</returns>
	const float GetFrame()const;

	/// <summary>
	/// エンドフレームのゲッター
	/// </summary>
	/// <returns>エンドフレーム</returns>
	const float GetEndFrame()const;
private://メンバ関数
	/// <summary>
	/// EaseIn
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>EaseIn</returns>
	float In(float frame);

	/// <summary>
	/// EaseOut
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>EaseOut</returns>
	float Out(float frame);

	/// <summary>
	/// インアウト
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>EaseInOut</returns>
	float InOut(float frame);

	/// <summary>
	/// EaseInBack
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>EaseInBack</returns>
	float InBack(float frame);

	/// <summary>
	/// EaseOutBack
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>EaseOutBack</returns>
	float OutBack(float frame);
private://メンバ関数ポインタのテーブル
	//イージング関数のテーブル
	static float (Easing::* UpdateEasingTable[])(float frame);
private://メンバ変数
	//フレーム
	float frame_ = 0.0f;
	//エンドフレーム
	float endFrame_ = 60.0f;
};

