/*!
 * @file   balController.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/module/cntl/balController.h>

namespace bal::mod::cntl {

// ----------------------------------------------------------------------------

Controller::Controller() {}

// ----------------------------------------------------------------------------

Controller::~Controller() {}

// ----------------------------------------------------------------------------

void Controller::initialize(const InitializeArg& arg)
{
    // 値保持
    mIndex = arg.mIndex;
}

// ----------------------------------------------------------------------------

void Controller::update()
{
    // 状態取得
    XINPUT_STATE state;
    DWORD result = XInputGetState(mIndex, &state);
    if (result == ERROR_SUCCESS)
    {
        mPrevStatus = mStatus;
        mStatus     = Status::Connected;

        // 過去の状態をコピー
        uint32_t prev_status = mPressMask;

        // 現在の状態をチェック
        mPressMask  = 0;
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A)              != 0) { mPressMask |= ButtonA; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B)              != 0) { mPressMask |= ButtonB; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X)              != 0) { mPressMask |= ButtonX; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)              != 0) { mPressMask |= ButtonY; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)        != 0) { mPressMask |= ButtonUp; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      != 0) { mPressMask |= ButtonDown; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      != 0) { mPressMask |= ButtonLeft; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     != 0) { mPressMask |= ButtonRight; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START)          != 0) { mPressMask |= ButtonStart; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)           != 0) { mPressMask |= ButtonBack; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0) { mPressMask |= ButtonL1; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) { mPressMask |= ButtonR1; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     != 0) { mPressMask |= ButtonL3; }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    != 0) { mPressMask |= ButtonR3; }

        // トリガーの状態
        mLeftTrigger  = static_cast<float>(state.Gamepad.bLeftTrigger)  / 255.f;
        mRightTrigger = static_cast<float>(state.Gamepad.bRightTrigger) / 255.f;
        if (state.Gamepad.bLeftTrigger  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) { mPressMask |= ButtonL2; }
        if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) { mPressMask |= ButtonR2; }

        // スティックの状態
        if (state.Gamepad.sThumbLX >= 0)
        {
            int16_t value = Math::Max(state.Gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
            int16_t range = std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            mLeftStick.setX(static_cast<float>(value) / static_cast<float>(range));
        }
        else if (state.Gamepad.sThumbLX < 0)
        {
            int16_t value = Math::Min(state.Gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
            int16_t range = -(std::numeric_limits<int16_t>::min() + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            mLeftStick.setX(static_cast<float>(value) / static_cast<float>(range));
        }
        if (state.Gamepad.sThumbLY >= 0)
        {
            int16_t value = Math::Max(state.Gamepad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
            int16_t range = std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            mLeftStick.setY(static_cast<float>(value) / static_cast<float>(range));
        }
        else if (state.Gamepad.sThumbLY < 0)
        {
            int16_t value = Math::Min(state.Gamepad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
            int16_t range = -(std::numeric_limits<int16_t>::min() + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            mLeftStick.setY(static_cast<float>(value) / static_cast<float>(range));
        }

        if (state.Gamepad.sThumbRX >= 0)
        {
            int16_t value = Math::Max(state.Gamepad.sThumbRX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
            int16_t range = std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            mRightStick.setX(static_cast<float>(value) / static_cast<float>(range));
        }
        else if (state.Gamepad.sThumbRX < 0)
        {
            int16_t value = Math::Min(state.Gamepad.sThumbRX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
            int16_t range = -(std::numeric_limits<int16_t>::min() + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            mRightStick.setX(static_cast<float>(value) / static_cast<float>(range));
        }
        if (state.Gamepad.sThumbRY >= 0)
        {
            int16_t value = Math::Max(state.Gamepad.sThumbRY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
            int16_t range = std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
            mRightStick.setY(static_cast<float>(value) / static_cast<float>(range));
        }
        else if (state.Gamepad.sThumbRY < 0)
        {
            int16_t value = Math::Min(state.Gamepad.sThumbRY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
            int16_t range = -(std::numeric_limits<int16_t>::min() + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            mRightStick.setY(static_cast<float>(value) / static_cast<float>(range));
        }

        // 過去の状態を照らし合わせて on 系をチェック
        mOnPressMask   = (prev_status ^ mPressMask) & mPressMask;
        mOnReleaseMask = (prev_status ^ mPressMask) & prev_status;
    }
    //else if (result == ERROR_DEVICE_NOT_CONNECTED)
    else
    {
        // 非接続
        mPrevStatus = mStatus;
        mStatus     = Status::Disconnected;
    }
}

// ----------------------------------------------------------------------------

}
