/*!
 * @file   balGfxPerspectiveCamera.h
 * @brief  透視投影カメラ
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// math
#include <math/balMath.h>

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

class PerspectiveCamera
{
public:
    /*!
     * コンストラクター
     */
    PerspectiveCamera();

    /*!
     * デストラクター
     */
    virtual ~PerspectiveCamera();

    /*!
     * 画面のアスペクト比を設定します
     * @param[in] aspect 横幅/高さ
     */
    void setAspect(float aspect);

    /*!
     * クリッピング領域を設定します
     * @param[in] z_near 最低距離
     * @param[in] z_far  最高距離
     */
    void setNearFar(float z_near, float z_far);

    /*!
     * Y方向の画角を設定します
     * @param[in] fovy Y方向の画角
     */
    void setFovy(const Radian& fovy);

    /*!
     * 視点、注視点を指定します
     * @param[in] eye_position カメラの位置
     * @param[in] look_at      カメラの注視点
     * @param[in] up           カメラの上方向ベクトル
     */
    void setLookAt(const MathVector3& eye_position, const MathVector3& look_at, const MathVector3& up = MathVector3(0.f, 1.f, 0.f));

    /*!
     * プロジェクション行列を直接設定します
     * @param[in] matrix プロジェクション行列
     */
    void setProjectionMatrix(const MathMatrix44& matrix);

    /*!
     * ビュー行列を直接設定します
     * @param[in] matrix ビュー行列
     */
    void setViewMatrix(const MathMatrix44& matrix);

public:
    /*!
     * プロジェクション行列を取得します
     */
    const MathMatrix44& getProjectionMatrix() const { return mProjectionMatrix; }

    /*!
     * ビュー行列を取得します
     */
    const MathMatrix44& getViewMatrix() const { return mViewMatrix; }

    /*!
     * プロジェクション・ビュー行列を取得します
     */
    const MathMatrix44& getViewProjectionMatrix() const { return mViewProjectionMatrix; }

public:
    /*!
     * 行列を更新します
     */
    void updateMatrix();

private:
    enum class EFlag
    {
        InvalidProjectionMatrix,
        InvalidViewMatrix,
        InvalidViewProjectionMatrix
    };

private:
    std::bitset<8> mFlags;

    float        mAspect;
    float        mNear;
    float        mFar;
    Radian       mFovy;
    MathVector3  mEyePosition;
    MathVector3  mLookAt;
    MathVector3  mUp;

    MathMatrix44 mProjectionMatrix;
    MathMatrix44 mViewMatrix;
    MathMatrix44 mViewProjectionMatrix;
};

}
