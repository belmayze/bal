/*!
 * @file   balGfxPerspectiveCamera.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/module/gfx/balGfxPerspectiveCamera.h>

namespace bal::mod::gfx {

// ----------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera() {}

// ----------------------------------------------------------------------------

PerspectiveCamera::~PerspectiveCamera() {}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setAspect(float aspect)
{
    mAspect = aspect;
    mFlags.set(static_cast<size_t>(EFlag::InvalidProjectionMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setNearFar(float z_near, float z_far)
{
    mNear = z_near;
    mFar  = z_far;
    mFlags.set(static_cast<size_t>(EFlag::InvalidProjectionMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setFovy(const Radian& fovy)
{
    mFovy = fovy;
    mFlags.set(static_cast<size_t>(EFlag::InvalidProjectionMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setLookAt(const MathVector3& eye_position, const MathVector3& look_at, const MathVector3& up)
{
    mEyePosition = eye_position;
    mLookAt      = look_at;
    mUp          = up;
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setProjectionMatrix(const MathMatrix44& matrix)
{
    mProjectionMatrix = matrix;
    mFlags.reset(static_cast<size_t>(EFlag::InvalidProjectionMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::setViewMatrix(const MathMatrix44& matrix)
{
    mViewMatrix = matrix;
    mFlags.reset(static_cast<size_t>(EFlag::InvalidViewMatrix));
    mFlags.set(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
}

// ----------------------------------------------------------------------------

void PerspectiveCamera::updateMatrix()
{
    if (mFlags.test(static_cast<size_t>(EFlag::InvalidProjectionMatrix)))
    {
        mFlags.reset(static_cast<size_t>(EFlag::InvalidProjectionMatrix));
        mProjectionMatrix.setPerspectiveProjectionRH(mFovy, mAspect, mNear, mFar);
    }

    if (mFlags.test(static_cast<size_t>(EFlag::InvalidViewMatrix)))
    {
        mFlags.reset(static_cast<size_t>(EFlag::InvalidViewMatrix));
        mViewMatrix.setLookAtRH(mEyePosition, mLookAt, mUp);
    }

    if (mFlags.test(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix)))
    {
        mFlags.reset(static_cast<size_t>(EFlag::InvalidViewProjectionMatrix));
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
}

// ----------------------------------------------------------------------------

}
