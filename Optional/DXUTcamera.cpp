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
#include "../thirdparty/Reversed-Z/reversed_z.h"

//======================================================================================
// CDXUTBaseCamera
//======================================================================================

//--------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------
CDXUTBaseCamera::CDXUTBaseCamera() noexcept : m_mView{},
                                              m_mProj{},
                                              m_cKeysDown(0),
                                              m_aKeys{},
                                              m_vKeyboardDirection(0, 0, 0),
                                              m_ptLastMousePosition{0, 0},
                                              m_nCurrentButtonMask(0),
                                              m_vMouseDelta(0, 0),
                                              m_fFramesToSmoothMouseData(2.0f),
                                              m_vDefaultEye(0, 0, 0),
                                              m_vDefaultLookAt(0, 0, 0),
                                              m_vDefaultUp(0, 1, 0),
                                              m_vEye(0, 0, 0),
                                              m_vLookAt(0, 0, 0),
                                              m_vUp(0, 1, 0),
                                              m_fCameraYawAngle(0.0f),
                                              m_fCameraPitchAngle(0.0f),
                                              m_vVelocity(0, 0, 0),
                                              m_vVelocityDrag(0, 0, 0),
                                              m_fDragTimer(0.0f),
                                              m_fTotalDragTimeToZero(0.25),
                                              m_vRotVelocity(0, 0),
                                              m_fFOV(0),
                                              m_fAspect(0),
                                              m_fNearPlane(0),
                                              m_fFarPlane(1),
                                              m_fRotationScaler(1.0f),
                                              m_fMoveScaler(5.0f),
                                              m_bMouseLButtonDown(false),
                                              m_bMouseMButtonDown(false),
                                              m_bMouseRButtonDown(false),
                                              m_bMovementDrag(false),
                                              m_bRightHanded(false),
                                              m_bInvertPitch(false),
                                              m_bEnablePositionMovement(true),
                                              m_bEnableYAxisMovement(true),
                                              m_bClipToBoundary(false),
                                              m_bResetCursorAfterMove(false),
                                              m_vMinBoundary(-1, -1, -1),
                                              m_vMaxBoundary(1, 1, 1)
{
    // Setup the view matrix
    SetViewParams(DirectX::g_XMZero, DirectX::g_XMIdentityR2, DirectX::g_XMIdentityR1);

    // Setup the projection matrix
    SetProjParams(DirectX::XM_PI / 4, 1.0f, 1.0f, 1000.0f);
}

void CDXUTBaseCamera::SetRightHanded(bool bRightHanded)
{
    if (m_bRightHanded != bRightHanded)
    {
        m_bRightHanded = bRightHanded;

        SetViewParams(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat3(&m_vLookAt), DirectX::XMLoadFloat3(&m_vUp));

        SetProjParams(m_fFOV, m_fAspect, m_fNearPlane, m_fFarPlane);
    }
}

//--------------------------------------------------------------------------------------
// Client can call this to change the position and direction of camera
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::SetViewParams(DirectX::CXMVECTOR vEyePt, DirectX::CXMVECTOR vLookatPt, DirectX::CXMVECTOR vUp)
{
    DirectX::XMStoreFloat3(&m_vEye, vEyePt);
    DirectX::XMStoreFloat3(&m_vDefaultEye, vEyePt);

    DirectX::XMStoreFloat3(&m_vLookAt, vLookatPt);
    DirectX::XMStoreFloat3(&m_vDefaultLookAt, vLookatPt);

    DirectX::XMStoreFloat3(&m_vUp, vUp);
    DirectX::XMStoreFloat3(&m_vDefaultUp, vUp);

    // Calc the view matrix
    DirectX::XMMATRIX mView = m_bRightHanded ? DirectX::XMMatrixLookAtRH(vEyePt, vLookatPt, vUp) : DirectX::XMMatrixLookAtLH(vEyePt, vLookatPt, vUp);
    DirectX::XMStoreFloat4x4(&m_mView, mView);

    DirectX::XMMATRIX mInvView = DirectX::XMMatrixInverse(nullptr, mView);

    // The axis basis vectors and camera position are stored inside the
    // position matrix in the 4 rows of the camera's world matrix.
    // To figure out the yaw/pitch of the camera, we just need the Z basis vector
    DirectX::XMFLOAT3 zBasisRotation;
    DirectX::XMStoreFloat3(&zBasisRotation, DirectX::XMVector3TransformNormal(DirectX::g_XMIdentityR2, mInvView));
    // RightHanded still the same
    // DirectX::XMVECTOR zBasis = DirectX::XMVector3TransformNormal(DirectX::g_XMNegIdentityR2, mInvView);
    // calculate zBasisRotation from zBasis and DirectX::g_XMNegIdentityR2

    m_fCameraYawAngle = std::copysign(1.0F, zBasisRotation.x) * DirectX::XMScalarACos(zBasisRotation.z / std::sqrt(zBasisRotation.x * zBasisRotation.x + zBasisRotation.z * zBasisRotation.z));
    m_fCameraPitchAngle = -DirectX::XMScalarASin(zBasisRotation.y);
}

//--------------------------------------------------------------------------------------
// Calculates the projection matrix based on input params
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane)
{
    // Set attributes for the projection matrix
    m_fFOV = fFOV;
    m_fAspect = fAspect;
    m_fNearPlane = fNearPlane;
    m_fFarPlane = fFarPlane;

    DirectX::XMMATRIX mProj = m_bRightHanded ? DirectX_Math_Matrix_PerspectiveFovRH_ReversedZ(fFOV, fAspect, fNearPlane, fFarPlane) : DirectX_Math_Matrix_XMMatrixPerspectiveFovLH_ReversedZ(fFOV, fAspect, fNearPlane, fFarPlane);
    DirectX::XMStoreFloat4x4(&m_mProj, mProj);
}

//--------------------------------------------------------------------------------------
// Call this from your message proc so this class can handle window messages
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::HandleKeyDownMessage(D3DUtil_CameraKeys mappedKey)
{
    // Map this key to a D3DUtil_CameraKeys enum and update the
    // state of m_aKeys[] by adding the KEY_IS_DOWN_MASK mask
    // only if the key is not down
    if (mappedKey != CAM_UNKNOWN)
    {
        _Analysis_assume_(mappedKey >= 0 && mappedKey < CAM_MAX_KEYS);
        if (!IsKeyDown(m_aKeys[mappedKey]))
        {
            m_aKeys[mappedKey] = KEY_IS_DOWN_MASK;
            ++m_cKeysDown;
        }
    }

    // Get keyboard/mouse/gamepad input
    UpdateKeyboardDirection();
}

void CDXUTBaseCamera::HandleKeyUpMessage(D3DUtil_CameraKeys mappedKey)
{
    // Map this key to a D3DUtil_CameraKeys enum and update the
    // state of m_aKeys[] by removing the KEY_IS_DOWN_MASK mask.
    if (mappedKey != CAM_UNKNOWN && mappedKey < CAM_MAX_KEYS)
    {
        m_aKeys[mappedKey] &= ~KEY_IS_DOWN_MASK;
        --m_cKeysDown;
    }

    // Get keyboard/mouse/gamepad input
    UpdateKeyboardDirection();
}

void CDXUTBaseCamera::HandleMouseMoveMessage(float normalizedX, float normalizedY, bool leftButton, bool middleButton, bool rightButton)
{
    if (leftButton)
    {
        m_bMouseLButtonDown = true;
        m_nCurrentButtonMask |= MOUSE_LEFT_BUTTON;
    }
    else
    {
        m_bMouseLButtonDown = false;
        m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
    }

    if (middleButton)
    {

        m_bMouseMButtonDown = true;
        m_nCurrentButtonMask |= MOUSE_MIDDLE_BUTTON;
    }
    else
    {
        m_bMouseMButtonDown = false;
        m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
    }

    if (rightButton)
    {
        m_bMouseRButtonDown = true;
        m_nCurrentButtonMask |= MOUSE_RIGHT_BUTTON;
    }
    else
    {
        m_bMouseRButtonDown = false;
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
        m_vKeyboardDirection = DirectX::XMFLOAT3(0, 0, 0);

        // Update acceleration vector based on keyboard state
        if (IsKeyDown(m_aKeys[CAM_MOVE_FORWARD]))
        {
            m_vKeyboardDirection.z += m_bRightHanded ? -1 : 1;
        }

        if (IsKeyDown(m_aKeys[CAM_MOVE_BACKWARD]))
        {
            m_vKeyboardDirection.z += m_bRightHanded ? 1 : -1;
        }

        if (m_bEnableYAxisMovement)
        {
            if (IsKeyDown(m_aKeys[CAM_MOVE_UP]))
            {
                m_vKeyboardDirection.y += 1;
            }

            if (IsKeyDown(m_aKeys[CAM_MOVE_DOWN]))
            {
                m_vKeyboardDirection.y += -1;
            }
        }

        if (IsKeyDown(m_aKeys[CAM_STRAFE_RIGHT]))
        {
            m_vKeyboardDirection.x += 1;
        }

        if (IsKeyDown(m_aKeys[CAM_STRAFE_LEFT]))
        {
            m_vKeyboardDirection.x += -1;
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
void CDXUTBaseCamera::UpdateMouseDelta(DirectX::XMFLOAT2 const &ptCurMousePos)
{
    // Calc how far it's moved since last frame
    if (GetMouseInput())
    {
        DirectX::XMFLOAT2 ptCurMouseDelta;
        ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
        ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

        // Cardinal Rotation
        // Right
        if ((ptCurMouseDelta.x >= ptCurMouseDelta.y) && (ptCurMouseDelta.x >= -ptCurMouseDelta.y))
        {
            ptCurMouseDelta.x = 1.0F;
            ptCurMouseDelta.y = 0.0F;
        }
        // Left
        else if ((ptCurMouseDelta.x <= ptCurMouseDelta.y) && (ptCurMouseDelta.x <= -ptCurMouseDelta.y))
        {
            ptCurMouseDelta.x = -1.0F;
            ptCurMouseDelta.y = 0.0F;
        }
        // Up
        else if ((ptCurMouseDelta.y < ptCurMouseDelta.x) && (ptCurMouseDelta.y < -ptCurMouseDelta.x))
        {
            ptCurMouseDelta.x = 0.0F;
            ptCurMouseDelta.y = -1.0F;
        }
        // Down
        else
        {
            assert((ptCurMouseDelta.y > ptCurMouseDelta.x) && (ptCurMouseDelta.y > -ptCurMouseDelta.x));

            ptCurMouseDelta.x = 0.0F;
            ptCurMouseDelta.y = 1.0F;
        }

        DirectX::XMVECTOR vCurMouseDelta = DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&ptCurMouseDelta));

        // Smooth the relative mouse data over a few frames so it isn't
        // jerky when moving slowly at low frame rates.
        float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
        DirectX::XMVECTOR vMouseDelta = DirectX::XMVector2Normalize(DirectX::XMVectorLerp(DirectX::XMLoadFloat2(&m_vMouseDelta), vCurMouseDelta, fPercentOfNew));

        DirectX::XMStoreFloat2(&m_vMouseDelta, vMouseDelta);
    }

    // Record current position for next time
    m_ptLastMousePosition = ptCurMousePos;
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::UpdateVelocity(float fElapsedTime)
{
    DirectX::XMVECTOR vMouseDelta = DirectX::XMLoadFloat2(&m_vMouseDelta);
    DirectX::XMVECTOR vRotVelocity = DirectX::XMVectorScale(vMouseDelta, m_fRotationScaler);

    DirectX::XMStoreFloat2(&m_vRotVelocity, vRotVelocity);

    DirectX::XMVECTOR vKeyboardDirection = DirectX::XMLoadFloat3(&m_vKeyboardDirection);
    DirectX::XMVECTOR vAccel = vKeyboardDirection;

    // Normalize vector so if moving 2 dirs (left & forward),
    // the camera doesn't move faster than if moving in 1 dir
    vAccel = DirectX::XMVector3Normalize(vAccel);

    // Scale the acceleration vector
    vAccel = DirectX::XMVectorScale(vAccel, m_fMoveScaler);

    if (m_bMovementDrag)
    {
        // Is there any acceleration this frame?
        if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vAccel)) > 0)
        {
            // If so, then this means the user has pressed a movement key
            // so change the velocity immediately to acceleration
            // upon keyboard input.  This isn't normal physics
            // but it will give a quick response to keyboard input
            DirectX::XMStoreFloat3(&m_vVelocity, vAccel);

            m_fDragTimer = m_fTotalDragTimeToZero;

            DirectX::XMStoreFloat3(&m_vVelocityDrag, DirectX::XMVectorScale(vAccel, (1.0F / m_fDragTimer)));
        }
        else
        {
            // If no key being pressed, then slowly decrease velocity to 0
            if (m_fDragTimer > 0)
            {
                // Drag until timer is <= 0
                DirectX::XMVECTOR vVelocity = DirectX::XMLoadFloat3(&m_vVelocity);
                DirectX::XMVECTOR vVelocityDrag = DirectX::XMLoadFloat3(&m_vVelocityDrag);

                vVelocity = DirectX::XMVectorSubtract(vVelocity, DirectX::XMVectorScale(vVelocityDrag, fElapsedTime));

                DirectX::XMStoreFloat3(&m_vVelocity, vVelocity);

                m_fDragTimer -= fElapsedTime;
            }
            else
            {
                // Zero velocity
                m_vVelocity = DirectX::XMFLOAT3(0, 0, 0);
            }
        }
    }
    else
    {
        // No drag, so immediately change the velocity
        DirectX::XMStoreFloat3(&m_vVelocity, vAccel);
    }
}

//--------------------------------------------------------------------------------------
// Reset the camera's position back to the default
//--------------------------------------------------------------------------------------
void CDXUTBaseCamera::Reset()
{
    DirectX::XMVECTOR vDefaultEye = DirectX::XMLoadFloat3(&m_vDefaultEye);
    DirectX::XMVECTOR vDefaultLookAt = DirectX::XMLoadFloat3(&m_vDefaultLookAt);
    DirectX::XMVECTOR vDefaultUp = DirectX::XMLoadFloat3(&m_vDefaultUp);

    SetViewParams(vDefaultEye, vDefaultLookAt, vDefaultUp);
}

//======================================================================================
// CDXUTFirstPersonCamera
//======================================================================================

CDXUTFirstPersonCamera::CDXUTFirstPersonCamera() noexcept : m_mCameraWorld{},
                                                            m_nActiveButtonMask(0x07),
                                                            m_bRotateWithoutButtonDown(false)
{
}

//--------------------------------------------------------------------------------------
// Update the view matrix based on user input & elapsed time
//--------------------------------------------------------------------------------------
void CDXUTFirstPersonCamera::FrameMove(float fElapsedTime)
{
    if (IsKeyDown(m_aKeys[CAM_RESET]))
    {
        Reset();
    }

    //// Get the mouse movement (if any) if the mouse button are down
    // if( (m_nActiveButtonMask & m_nCurrentButtonMask) || m_bRotateWithoutButtonDown )
    //     UpdateMouseDelta( fElapsedTime );

    // Get amount of velocity based on the keyboard input and drag (if any)
    UpdateVelocity(fElapsedTime);

    // Simple euler method to calculate position delta
    DirectX::XMVECTOR vVelocity = DirectX::XMLoadFloat3(&m_vVelocity);
    DirectX::XMVECTOR vPosDelta = DirectX::XMVectorScale(vVelocity, fElapsedTime);

    // If rotating the camera
    if ((m_nActiveButtonMask & m_nCurrentButtonMask) || m_bRotateWithoutButtonDown)
    {
        // Update the pitch & yaw angle based on mouse movement
        DirectX::XMVECTOR vRotVelocity = DirectX::XMLoadFloat2(&m_vRotVelocity);
        DirectX::XMVECTOR vYawPitchDelta = DirectX::XMVectorScale(vRotVelocity, fElapsedTime);
        float fYawDelta = DirectX::XMVectorGetX(vYawPitchDelta);
        float fPitchDelta = DirectX::XMVectorGetY(vYawPitchDelta);

        // Invert pitch if requested
        if (m_bInvertPitch)
            fPitchDelta = -fPitchDelta;

        // Invert yaw & pitch if right-handed
        if (m_bRightHanded)
        {
            fYawDelta = -fYawDelta;
            fPitchDelta = -fPitchDelta;
        }

        m_fCameraPitchAngle += fPitchDelta;
        m_fCameraYawAngle += fYawDelta;

        // Limit pitch to straight up or straight down
        m_fCameraPitchAngle = std::max(-DirectX::XM_PI / 2.0f, m_fCameraPitchAngle);
        m_fCameraPitchAngle = std::min(+DirectX::XM_PI / 2.0f, m_fCameraPitchAngle);
    }

    // Make a rotation matrix based on the camera's yaw & pitch
    DirectX::XMMATRIX mCameraRot = DirectX::XMMatrixRotationRollPitchYaw(m_fCameraPitchAngle, m_fCameraYawAngle, 0);

    // Transform vectors based on camera's rotation matrix
    DirectX::XMVECTOR vWorldUp = DirectX::XMVector3TransformCoord(DirectX::g_XMIdentityR1, mCameraRot);
    DirectX::XMVECTOR vWorldAhead = DirectX::XMVector3TransformCoord(m_bRightHanded ? DirectX::g_XMNegIdentityR2 : DirectX::g_XMIdentityR2, mCameraRot);

    // Transform the position delta by the camera's rotation
    if (!m_bEnableYAxisMovement)
    {
        // If restricting Y movement, do not include pitch
        // when transforming position delta vector.
        mCameraRot = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_fCameraYawAngle, 0.0f);
    }
    DirectX::XMVECTOR vPosDeltaWorld = DirectX::XMVector3TransformCoord(vPosDelta, mCameraRot);

    // Move the eye position
    DirectX::XMVECTOR vEye = DirectX::XMLoadFloat3(&m_vEye);
    vEye = DirectX::XMVectorAdd(vEye, vPosDeltaWorld);
    if (m_bClipToBoundary)
    {
        vEye = ConstrainToBoundary(vEye);
    }
    DirectX::XMStoreFloat3(&m_vEye, vEye);

    // Update the lookAt position based on the eye position
    DirectX::XMVECTOR vLookAt = DirectX::XMVectorAdd(vEye, vWorldAhead);
    DirectX::XMStoreFloat3(&m_vLookAt, vLookAt);

    // Update the view matrix
    DirectX::XMMATRIX mView = m_bRightHanded ? DirectX::XMMatrixLookAtRH(vEye, vLookAt, vWorldUp) : DirectX::XMMatrixLookAtLH(vEye, vLookAt, vWorldUp);
    DirectX::XMStoreFloat4x4(&m_mView, mView);

    DirectX::XMMATRIX mCameraWorld = DirectX::XMMatrixInverse(nullptr, mView);
    DirectX::XMStoreFloat4x4(&m_mCameraWorld, mCameraWorld);
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
