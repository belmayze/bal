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
