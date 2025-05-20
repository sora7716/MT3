#pragma once
#include "gameObject/GameObject.h"
class Camera :public GameObject {

public://メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="kWindowWidth"></param>
	/// <param name="kWindowHeight"></param>
	void Initialize(int kWindowWidth, int kWindowHeight);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="keys"></param>
	/// <param name="preKeys"></param>
	void Update(char* keys, char* preKeys);

	/// <summary>
	/// デバックテキスト
	/// </summary>
	/// <param name="name">名前</param>
	void DebugText(const char* name="camera");

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ビュープロジェクションマトリックスのゲッター
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewProjectionMatrix()const;

	/// <summary>
	/// ビューポートマトリックスのゲッター
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewportMatrix()const;

	/// <summary>
	/// scale_のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale();

	/// <summary>
	/// rotate_のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetRotate();

	/// <summary>
	/// translate_のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetTranslate();

	/// <summary>
	/// 同次クリップ空間のゲッター
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetProjctionMatrix()const;

	/// <summary>
	/// ビューマトリックスのゲッター
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewMatrix()const;

private://メンバ変数

	Matrix4x4 cameraMatrix_ = {};//カメラマトリックス
	Matrix4x4 viewMatrix_ = {};//ビューマトリックス
	Matrix4x4 projectionMatrix_ = {};//同次クリップ空間
	Matrix4x4 viewProjectionMatrix_ = {};//wvpマトリックス
	Matrix4x4 viewportMatrix_ = {};//ビューポートマトリックス

private://メンバ変数
	/// <summary>
	/// レンダリングパイプライン
	/// </summary>
	void RenderingPipeline();
};

