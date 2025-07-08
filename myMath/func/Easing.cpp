#include "Easing.h"
#include <cstdint>

//イージングテーブルの初期化
float (Easing::* Easing::UpdateEasingTable[])(float frame) = {
	&Easing::In,
	&Easing::Out,
	&Easing::InOut,
	&Easing::InBack,
	&Easing::OutBack
};

//更新
void Easing::Update() {
	if (frame_ < endFrame_) {
		frame_++;
	}
}

//easing関数
float Easing::GetEasing(EasingType easingType) {
	return (this->*UpdateEasingTable[static_cast<int32_t>(easingType)])(frame_ / endFrame_);
}

//フレームのセッター
void Easing::SetFrame(float frame) {
	frame_ = frame;
}

//エンドフレームのセッター
void Easing::SetEndFrame(float endFrame) {
	endFrame_ = endFrame;
}

//フレームのゲッター
const float Easing::GetFrame() const {
	return frame_;
}

//エンドフレームのゲッター
const float Easing::GetEndFrame() const {
	return endFrame_;
}

//EaseIn
float Easing::In(float frame) {

	return 1 - cosf((frame * float(M_PI) / 2));
}

//EaseOut
float Easing::Out(float frame) {
	return sinf((frame * float(M_PI)) / 2);
}

//EaseInOut
float Easing::InOut(float frame) {
	return -(cosf(float(M_PI) * frame) - 1) / 2;
}

//EaseInBack
float Easing::InBack(float frame) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * frame * frame * frame - c1 * frame * frame;
}

//EaseOutBack
float Easing::OutBack(float frame) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return float(1 + c3 * pow(frame - 1, 3) + c1 * pow(frame - 1, 2));
}