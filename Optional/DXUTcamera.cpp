//--------------------------------------------------------------------------------------
// File: DXUTcamera.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=320437
//--------------------------------------------------------------------------------------
#include "DXUTcamera.h"
#include <cmath>
#include <algorithm>

#define MOUSE_LEFT_BUTTON 0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON 0x04

//======================================================================================
// CDXUTBaseCamera
//======================================================================================

//--------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------
CDXUTBaseCamera::CDXUTBaseCamera() noexcept
    : m_bIsKeyDown{false, false, false, false, false, false, false},
      m_vKeyboardDirection(0, 0, 0),
      m_ptLastMousePosition{0, 0},
      m_nCurrentButtonMask(0),
      m_vMouseDelta(0, 0),
      m_vEyePt(0, 0, 0),
      m_vEyeDir(0, 0, 1),
      m_vUpDir(0, 1, 0),
      m_fRotationScaler(DirectX::XM_PI),
      m_fMoveScaler(5.0F),
      m_bInvertPitch(false),
      m_bEnableUpDownMovement(true)
{
}

//--------------------------------------------------------------------------------------
// Call this from your message proc so this class can handle window messages
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::HandleKeyDownMessage(D3DUtil_CameraKeys mappedKey)
{
    if (mappedKey > CAM_UNKNOWN && mappedKey < CAM_MAX_KEYS)
    {
        m_bIsKeyDown[mappedKey] = true;
    }

    // Get keyboard/mouse/gamepad input
    UpdateKeyboardDirection();
}

void CDXUTBaseCamera::HandleKeyUpMessage(D3DUtil_CameraKeys mappedKey)
{
    // Map this key to a D3DUtil_CameraKeys enum and update the
    // state of m_bIsKeyDown[] by removing the KEY_IS_DOWN_MASK mask.
    if (mappedKey > CAM_UNKNOWN && mappedKey < CAM_MAX_KEYS)
    {
        m_bIsKeyDown[mappedKey] = false;
    }

    // Get keyboard/mouse/gamepad input
    UpdateKeyboardDirection();
}

void CDXUTBaseCamera::HandleMouseMoveMessage(float normalizedX, float normalizedY, bool leftButton, bool middleButton, bool rightButton)
{
    if (leftButton)
    {
        m_nCurrentButtonMask |= MOUSE_LEFT_BUTTON;
    }
    else
    {
        m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
    }

    if (middleButton)
    {

        m_nCurrentButtonMask |= MOUSE_MIDDLE_BUTTON;
    }
    else
    {
        m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
    }

    if (rightButton)
    {
        m_nCurrentButtonMask |= MOUSE_RIGHT_BUTTON;
    }
    else
    {
        m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
    }

    // Get current position of mouse
    DirectX::XMFLOAT2 ptCurMousePos = {normalizedX, normalizedY};

    // Get mouse input
    UpdateMouseDelta(ptCurMousePos);
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::UpdateKeyboardDirection()
{
    if (GetKeyBoardInput())
    {
        int keyboard_forward_backward_direction = 0;
        if (m_bIsKeyDown[CAM_MOVE_FORWARD])
        {
            ++keyboard_forward_backward_direction;
        }

        if (m_bIsKeyDown[CAM_MOVE_BACKWARD])
        {
            --keyboard_forward_backward_direction;
        }

        int keyboard_up_down_direction = 0;
        if (m_bEnableUpDownMovement)
        {
            if (m_bIsKeyDown[CAM_MOVE_UP])
            {
                ++keyboard_up_down_direction;
            }

            if (m_bIsKeyDown[CAM_MOVE_DOWN])
            {
                --keyboard_up_down_direction;
            }
        }

        int keyboard_right_left_movement = 0;
        if (m_bIsKeyDown[CAM_STRAFE_RIGHT])
        {
            ++keyboard_right_left_movement;
        }

        if (m_bIsKeyDown[CAM_STRAFE_LEFT])
        {
            --keyboard_right_left_movement;
        }

        m_vKeyboardDirection = DirectX::XMFLOAT3(static_cast<float>(keyboard_right_left_movement), static_cast<float>(keyboard_up_down_direction), static_cast<float>(keyboard_forward_backward_direction));

        if ((((0 != keyboard_right_left_movement) ? 1 : 0) + ((0 != keyboard_up_down_direction) ? 1 : 0) + ((0 != keyboard_forward_backward_direction) ? 1 : 0)) >= 2)
        {
            DirectX::XMStoreFloat3(&m_vKeyboardDirection, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_vKeyboardDirection)));
        }
    }
    else
    {
        m_vKeyboardDirection = DirectX::XMFLOAT3(0, 0, 0);
    }
}

//--------------------------------------------------------------------------------------
// Figure out the mouse delta based on mouse movement
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::UpdateMouseDelta(DirectX::XMFLOAT2 const &ptCurMousePosition)
{
    // Calc how far it's moved since last frame
    if (GetMouseInput())
    {
        DirectX::XMFLOAT2 vNewMouseDelta;
        vNewMouseDelta.x = ptCurMousePosition.x - m_ptLastMousePosition.x;
        vNewMouseDelta.y = ptCurMousePosition.y - m_ptLastMousePosition.y;

        // Cardinal Rotation
        // Right
        if ((vNewMouseDelta.x > vNewMouseDelta.y) && (vNewMouseDelta.x > -vNewMouseDelta.y))
        {
            vNewMouseDelta.x = 1.0F;
            vNewMouseDelta.y = 0.0F;
        }
        // Left
        else if ((vNewMouseDelta.x < vNewMouseDelta.y) && (vNewMouseDelta.x < -vNewMouseDelta.y))
        {
            vNewMouseDelta.x = -1.0F;
            vNewMouseDelta.y = 0.0F;
        }
        // Up
        else if ((vNewMouseDelta.y < vNewMouseDelta.x) && (vNewMouseDelta.y < -vNewMouseDelta.x))
        {
            vNewMouseDelta.x = 0.0F;
            vNewMouseDelta.y = -1.0F;
        }
        // Down
        else if ((vNewMouseDelta.y > vNewMouseDelta.x) && (vNewMouseDelta.y > -vNewMouseDelta.x))
        {

            vNewMouseDelta.x = 0.0F;
            vNewMouseDelta.y = 1.0F;
        }
        else
        {
            vNewMouseDelta.x = 0.0F;
            vNewMouseDelta.y = 0.0F;
        }

        m_vMouseDelta = vNewMouseDelta;
    }
    else
    {
        DirectX::XMStoreFloat2(&m_vMouseDelta, DirectX::XMVectorZero());
    }

    // Record current position for next time
    m_ptLastMousePosition = ptCurMousePosition;
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input if any
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::UpdateVelocity(DirectX::XMVECTOR &out_vRotationVelocity, DirectX::XMVECTOR &out_vMoveVelocity)
{
    DirectX::XMVECTOR vMouseDelta = DirectX::XMLoadFloat2(&m_vMouseDelta);
    out_vRotationVelocity = DirectX::XMVectorScale(vMouseDelta, m_fRotationScaler);

    DirectX::XMVECTOR vKeyboardDirection = DirectX::XMLoadFloat3(&m_vKeyboardDirection);
    out_vMoveVelocity = DirectX::XMVectorScale(vKeyboardDirection, m_fMoveScaler);
}

//======================================================================================
// CDXUTFirstPersonCamera
//======================================================================================

CDXUTFirstPersonCamera::CDXUTFirstPersonCamera() noexcept
    : m_nActiveButtonMask(MOUSE_RIGHT_BUTTON),
      m_bRotateWithoutButtonDown(false),
      m_bEnablePositionMovement(true)
{
}

//--------------------------------------------------------------------------------------
// Update the view matrix based on user input & elapsed time
//--------------------------------------------------------------------------------------
void CDXUTFirstPersonCamera::FrameMove(float fElapsedTime)
{
    // Get amount of velocity based on the keyboard input (if any)
    DirectX::XMVECTOR vRotationVelocity;
    DirectX::XMVECTOR vMoveVelocity;
    UpdateVelocity(vRotationVelocity, vMoveVelocity);

    float fMoveForwardDelta = 0.0F;
    float fMoveUpDelta = 0.0F;
    float fMovePitchDelta = 0.0F;
    {
        DirectX::XMVECTOR vMoveDelta = DirectX::XMVectorScale(vMoveVelocity, fElapsedTime);

        fMoveForwardDelta = DirectX::XMVectorGetZ(vMoveDelta);
        fMoveUpDelta = DirectX::XMVectorGetY(vMoveDelta);
        fMovePitchDelta = DirectX::XMVectorGetX(vMoveDelta);
    }

    // Nose: moves from side to side
    float fRotYawDelta = 0.0F;
    // Pitch: nose up or tail up
    float fRotPitchDelta = 0.0F;
    {
        DirectX::XMVECTOR vRotYawPitchDelta = DirectX::XMVectorScale(vRotationVelocity, fElapsedTime);

        fRotYawDelta = -DirectX::XMVectorGetX(vRotYawPitchDelta);
        fRotPitchDelta = -DirectX::XMVectorGetY(vRotYawPitchDelta);
        // Invert pitch if requested
        if (m_bInvertPitch)
        {
            fRotPitchDelta = -fRotPitchDelta;
        }
    }

    DirectX::XMVECTOR const vEyePt = DirectX::XMLoadFloat3(&m_vEyePt);
    DirectX::XMVECTOR const vEyeDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_vEyeDir));
    DirectX::XMVECTOR const vUpDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_vUpDir));
    DirectX::XMVECTOR const vPitchAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vEyeDir, vUpDir));

    // Update the eye position
    {
        DirectX::XMVECTOR const vNewEyePt = DirectX::XMVectorAdd(vEyePt, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vEyeDir, fMoveForwardDelta), DirectX::XMVectorScale(vUpDir, fMoveUpDelta)), DirectX::XMVectorScale(vPitchAxis, fMovePitchDelta)));

        DirectX::XMStoreFloat3(&m_vEyePt, vNewEyePt);
    }

    // Update the eye direction
    {
        if (m_bEnableUpDownMovement)
        {
            float const fCameraPitchLimit = DirectX::XMScalarACos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vEyeDir, vUpDir)));
            fRotPitchDelta = std::max(std::min(fCameraPitchLimit, fRotPitchDelta), fCameraPitchLimit - DirectX::XM_PI);
        }
        else
        {
            fRotPitchDelta = 0.0F;
        }

        DirectX::XMVECTOR const vNewEyeDir = DirectX::XMVector3TransformNormal(DirectX::XMVector3TransformNormal(vEyeDir, DirectX::XMMatrixRotationAxis(vUpDir, fRotYawDelta)), DirectX::XMMatrixRotationAxis(vPitchAxis, fRotPitchDelta));

        DirectX::XMStoreFloat3(&m_vEyeDir, vNewEyeDir);
    }
}

//--------------------------------------------------------------------------------------
// Enable or disable each of the mouse buttons for rotation drag.
//--------------------------------------------------------------------------------------
void CDXUTFirstPersonCamera::SetRotateButtons(bool bLeft, bool bMiddle, bool bRight, bool bRotateWithoutButtonDown)
{
    m_nActiveButtonMask = (bLeft ? MOUSE_LEFT_BUTTON : 0) |
                          (bMiddle ? MOUSE_MIDDLE_BUTTON : 0) |
                          (bRight ? MOUSE_RIGHT_BUTTON : 0);
    m_bRotateWithoutButtonDown = bRotateWithoutButtonDown;
}

bool CDXUTFirstPersonCamera::GetKeyBoardInput() const
{
    return (m_bEnablePositionMovement);
}

bool CDXUTFirstPersonCamera::GetMouseInput() const
{
    return ((m_nActiveButtonMask & m_nCurrentButtonMask) || m_bRotateWithoutButtonDown);
}
