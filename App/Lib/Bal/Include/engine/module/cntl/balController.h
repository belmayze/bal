/*!
 * @file   balController.h
 * @brief  コントローラー
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// math
#include <math/balMath.h>

// ----------------------------------------------------------------------------
namespace bal::mod::cntl {

class Controller
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t mIndex = 0;
    };

    //! 状態
    enum class Status
    {
        Connected,   //!< 接続
        Disconnected //!< 非接続
    };
    //! ボタンのマスク
    enum ButtonMask
    {
        ButtonA     = 1 << 0,
        ButtonB     = 1 << 1,
        ButtonX     = 1 << 2,
        ButtonY     = 1 << 3,
        ButtonUp    = 1 << 4,
        ButtonDown  = 1 << 5,
        ButtonLeft  = 1 << 6,
        ButtonRight = 1 << 7,
        ButtonStart = 1 << 8,
        ButtonBack  = 1 << 9,
        ButtonL1    = 1 << 10,
        ButtonR1    = 1 << 11,
        ButtonL2    = 1 << 12,
        ButtonR2    = 1 << 13,
        ButtonL3    = 1 << 14,
        ButtonR3    = 1 << 15,
    };

public:
    /*!
     * コンストラクター
     */
    Controller();

    /*!
     * デストラクター
     */
    virtual ~Controller();

    /*!
     * 初期化
     * @param[in] arg 初期化引数
     */
    void initialize(const InitializeArg& arg);

    /*!
     * 更新処理
     */
    void update();

public:
    //! 接続された瞬間
    bool isOnConnected() const { return (mPrevStatus != mStatus && mStatus == Status::Connected); }

    //! 接続されているか取得
    bool isConnected() const { return mStatus == Status::Connected; }

    //! 接続が解除された瞬間
    bool isOnDisconnected() const { return (mPrevStatus != mStatus && mStatus == Status::Disconnected); }

    //! ボタンが押されたか取得
    bool isOnPress(uint32_t mask) const { return isConnected() ? (mOnPressMask & mask) == mask : false; }

    //! ボタンが押されているか取得
    bool isPressed(uint32_t mask) const { return isConnected() ? (mPressMask & mask) == mask : false; }

    //! ボタンが離されたか取得
    bool isOnRelease(uint32_t mask) const { return isConnected() ? (mOnReleaseMask & mask) == mask : false; }

    //! 左スティック状態
    const MathVector2& getLeftStick() const { return mLeftStick; }

    //! 右スティック状態
    const MathVector2& getRightStick() const { return mRightStick; }
    
    //! トリガー状態
    float getLeftTrigger() const { return mLeftTrigger; }
    float getRightTrigger() const { return mRightTrigger; }

private:
    uint32_t    mIndex         = 0;
    Status      mPrevStatus    = Status::Disconnected;
    Status      mStatus        = Status::Disconnected;
    uint32_t    mOnPressMask   = 0;
    uint32_t    mPressMask     = 0;
    uint32_t    mOnReleaseMask = 0;
    float       mLeftTrigger   = 0.f;
    float       mRightTrigger  = 0.f;
    MathVector2 mLeftStick;
    MathVector2 mRightStick;
};

}
