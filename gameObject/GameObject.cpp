#include"GameObject.h"

//倍率
void GameObject::Scale(bool front,bool behind){
	scale_.z += velocity_.z;
	//キー入力
	if (behind) {
		velocity_.z = 5.0f / 60.0f;
	}
	else if (front && scale_.x > 0.5f) {
		velocity_.z = -5.0f / 60.0f;
	}
	else { velocity_.z = 0.0f; }

}

//回転
void GameObject::Rotate(){
	rotate_ += radian_;//回転
	//Novice::ScreenPrintf(0, 20, "%f", rotate_.GetVector().y);
}

//X座標の移動
void GameObject::TranslateX(bool left,bool right,float type){
	//キー入力
	if (left) {
		velocity_.x = -5.0f / 60.0f * type;
	}
	else if (right) {
		velocity_.x = 5.0f / 60.0f * type;
	}
	else { velocity_.x = 0.0f; }
}

//Y座標の移動
void GameObject::TranslateY(bool up, bool down, float type){
	if (up) {
		velocity_.y = 5.0f / 60.0f * type;
	}
	else if (down) {
		velocity_.y = -5.0f / 60.0f * type;
	}
	else { velocity_.y = 0.0f; }
}

//Z座標の移動
void GameObject::TranslateZ(bool front, bool back, float type){
	if (front) {
		velocity_.z = 5.0f / 60.0f * type;
	}
	else if (back) {
		velocity_.z = -5.0f / 60.0f * type;
	}
	else { velocity_.z = 0.0f; }
}

//X座標の回転
void GameObject::RotateX(bool bottom1, bool bottom2){
	if (bottom1) {
		radian_.x = 1.0f / 60.0f;
	}
	else if (bottom2) {
		radian_.x = -1.0f / 60.0f;
	}
	else {
		radian_.x = 0.0f;
	}
}

//Y座標の回転
void GameObject::RotateY(bool bottom1, bool bottom2){
	if (bottom1) {
		radian_.y= 1.0f / 60.0f;
	}
	else if (bottom2) {
		radian_.y = -1.0f / 60.0f;
	}
	else {
		radian_.y = 0.0f;
	}
}

//Z座標の回転
void GameObject::RotateZ(bool bottom1, bool bottom2){
	if (bottom1) {
		radian_.z = 1.0f / 60.0f;
	}
	else if (bottom2) {
		radian_.z = -1.0f / 60.0f;
	}
	else {
		radian_.z = 0.0f;
	}
}

//移動をまとめた
void GameObject::Movement(bool left, bool right, bool up, bool down, bool front, bool behind, float type){
	translate_ += velocity_;//移動
	TranslateX(left, right, type);
	TranslateY(up, down, type);
	TranslateZ(front, behind, type);
}

//回転をまとめた
void GameObject::Rotation(bool left, bool right, bool up, bool down, bool front, bool behind){
	rotate_ += radian_;
	RotateX(up, down);
	RotateY(front, behind);
	RotateZ(left, right);
}

//拡大縮小
void GameObject::Scaling(){
	scale_.z += scaling_.z;
	if (Novice::GetWheel() < -1) {
		if (scale_.z > 0.4f) {
			scaling_.z = 1.0f;
		}
		else {
			scaling_.z = 0.0f;
		}
	}
	else if (Novice::GetWheel() >= 1) {
		scaling_.z = 1.0f;
	}
	scaling_.z *= (float)Novice::GetWheel() / 1000.0f;
}

//回転
void GameObject::Rotation(){
	rotate_ += radian_;
	Novice::GetMousePosition(&mousePos_.x, &mousePos_.y);
	if (Novice::IsPressMouse(1)) {
		if (mousePos_.x != preMousePos_.x) {
			radian_.y = 0.1f;
			float direction = 0;
			if (mousePos_.x - preMousePos_.x > 2) {
				direction = 1;
			}
			else if (mousePos_.x - preMousePos_.x <= -2) {
				direction = -1;
			}
			radian_.y *= direction;
			preMousePos_.x = mousePos_.x;
		}
		else {
			radian_.y = 0;
		}
		if (mousePos_.y != preMousePos_.y) {
			radian_.x = 0.1f;
			float direction = 0;
			if (mousePos_.y - preMousePos_.y > 3) {
				direction = 1;
			}
			else if (mousePos_.y - preMousePos_.y <= -3) {
				direction = -1;
			}
			radian_.x *= direction;
			preMousePos_.y = mousePos_.y;
		}
		else {
			radian_.x = 0;
		}
	}
	else {
		radian_ = {};
	}
}




