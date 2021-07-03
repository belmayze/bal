/*!
 * @file   balMathCommonMatrix.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// c++
#include <cmath>
// bal
#include <math/balMath.h>
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

//! 列優先 4x4 行列
//
// x.x | y.x | z.x | t.x
// x.y | y.y | z.y | t.y
// x.z | y.z | z.z | t.z
//   0 |   0 |   0 |   0
//
// 行優先の方が、内積計算時にメモリー配列がきれいなので高速化できる
class alignas(16) MathCommonMatrix44
{
public:
    //! コンストラクター
    MathCommonMatrix44()
    {
        m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
        m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f; m[1][3] = 0.f;
        m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f; m[2][3] = 0.f;
        m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
    }
    //! コンストラクター
    MathCommonMatrix44(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    )
    {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }

public:
    /*!
     * 単位行列に戻します
     */
    inline void setIdentity();

    /*!
     * スケールをセットします
     * @param[in] scale スケール値
     */
    inline void setScale(const MathVector3& scale);

    /*!
     * X軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateX(const Radian& radian);

    /*!
     * Y軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateY(const Radian& radian);

    /*!
     * Z軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateZ(const Radian& radian);

    /*!
     * 回転をセットします
     * @note Z * Y * X を計算します
     * @param[in] x X軸角度
     * @param[in] y Y軸角度
     * @param[in] z Z軸角度
     */
    inline void setRotate(const Radian& x, const Radian& y, const Radian& z);

    /*!
     * ヨー・ピッチ・ロールをセットします
     * @note Z * X * Y を計算します
     */
    inline void setYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll);

    /*!
     * 平行移動をセットします
     * @param[in] translate 平行移動量
     */
    inline void setTranslate(const MathVector3& translate);

    /*!
     * 行列を倒置にします
     */
    inline void setTranspose();

    /*!
     * 右手座標系のカメラ行列を作成します
     * @note カメラの正面が -Z 方向となります
     * @param[in] eye_pt  カメラの位置
     * @param[in] look_at 注視点
     * @param[in] up      上方向ベクトル
     */
    inline void setLookAtRH(const MathVector3& eye_pt, const MathVector3& look_at, const MathVector3& up);

    /*! 
     * 右手座標系の平行投影行列を作成します
     * @param[in] width  画面の横幅
     * @param[in] height 画面の高さ
     * @param[in] z_near Z方向の近距離限界
     * @param[in] z_far  Z方向の遠距離限界
     */
    inline void setPerspectiveOrthoRH(float width, float height, float z_near, float z_far);

    /*!
     * 右手座標系のプロジェクション行列を作成します
     * @note 出力される Z は、0.0～1.0 の範囲です
     * @param[in] fovy Y方向画角
     * @param[in] aspect 画面のアスペクト比
     * @param[in] z_near Z方向の近距離限界
     * @param[in] z_far  Z方向の遠距離限界
     */
    inline void setPerspectiveProjectionRH(const Radian& fovy, float aspect, float z_near, float z_far);

public:
    /*!
     * 行にベクトルをセットします
     * @param[in] index インデックス
     * @param[in] v     ベクトル
     */
    inline void setCol(int index, const MathVector4& v);

    /*!
     * 列にベクトルをセットします
     * @param[in] index インデックス
     * @param[in] v     ベクトル
     */
    inline void setRow(int index, const MathVector4& v);

public:
    /*!
     * 行ベクトルを取得します
     * @param[in] index インデックス
     */
    inline MathVector4 getCol(int index) const;

    /*!
     * 列ベクトルを取得します
     * @param[in] index インデックス
     */
    inline MathVector4 getRow(int index) const;

public:
    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*! 行列同士の乗算 */
    inline MathCommonMatrix44 operator*(const MathCommonMatrix44& rhs) const;
    /*! ベクトルとの乗算 */
    inline MathCommonVector4 operator*(const MathCommonVector4& rhs) const;

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    const float* operator[](int row) const { return m[row]; }
          float* operator[](int row)       { return m[row]; }

protected:
    float m[4][4];
};

//! 列優先 3x4 行列
//
// x.x | y.x | z.x | t.x
// x.y | y.y | z.y | t.y
// x.z | y.z | z.z | t.z
class MathCommonMatrix34
{
public:
    //! コンストラクター
    MathCommonMatrix34()
    {
        m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
        m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f; m[1][3] = 0.f;
        m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f; m[2][3] = 0.f;
    }
    //! コンストラクター
    MathCommonMatrix34(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23
    )
    {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
    }

public:
    /*!
     * 単位行列に戻します
     */
    inline void setIdentity();

    /*!
     * スケールをセットします
     * @param[in] scale スケール値
     */
    inline void setScale(const MathVector3& scale);

    /*!
     * X軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateX(const Radian& radian);

    /*!
     * Y軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateY(const Radian& radian);

    /*!
     * Z軸回転をセットします
     * @param[in] radian 角度
     */
    inline void setRotateZ(const Radian& radian);

    /*!
     * 回転をセットします
     * @note Z * Y * X を計算します
     * @param[in] x X軸角度
     * @param[in] y Y軸角度
     * @param[in] z Z軸角度
     */
    inline void setRotate(const Radian& x, const Radian& y, const Radian& z);

    /*!
     * ヨー・ピッチ・ロールをセットします
     * @note Z * X * Y を計算します
     */
    inline void setYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll);

    /*!
     * 平行移動をセットします
     * @param[in] translate 平行移動量
     */
    inline void setTranslate(const MathVector3& translate);

    /*!
     * 右手座標系のカメラ行列を作成します
     * @note カメラの正面が -Z 方向となります
     * @param[in] eye_pt  カメラの位置
     * @param[in] look_at 注視点
     * @param[in] up      上方向ベクトル
     */
    inline void setLookAtRH(const MathVector3& eye_pt, const MathVector3& look_at, const MathVector3& up);

public:
    /*!
     * 行にベクトルをセットします
     * @param[in] index インデックス
     * @param[in] v     ベクトル
     */
    inline void setCol(int index, const MathVector4& v);

    /*!
     * 列にベクトルをセットします
     * @param[in] index インデックス
     * @param[in] v     ベクトル
     */
    inline void setRow(int index, const MathVector3& v);

public:
    /*!
     * 行ベクトルを取得します
     * @param[in] index インデックス
     */
    inline MathVector4 getCol(int index) const;

    /*!
     * 列ベクトルを取得します
     * @param[in] index インデックス
     */
    inline MathVector3 getRow(int index) const;

public:
    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*! 行列同士の乗算 */
    inline MathCommonMatrix34 operator*(const MathCommonMatrix34& rhs) const;
    /*! ベクトルとの乗算 */
    inline MathCommonVector4 operator*(const MathCommonVector4& rhs) const;

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    const float* operator[](int row) const { return m[row]; }
          float* operator[](int row)       { return m[row]; }

protected:
    float m[3][4];
};

}

#include <math/balMathCommonMatrix_inl.h>
