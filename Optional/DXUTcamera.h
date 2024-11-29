//--------------------------------------------------------------------------------------
// File: Camera.h
//
// Helper functions for Direct3D programming.
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=320437
//--------------------------------------------------------------------------------------
#ifndef _DXUT_CAMERA_H_
#define _DXUT_CAMERA_H_

#include <stddef.h>
#include <stdint.h>
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#include <DirectXMath.h>
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

//--------------------------------------------------------------------------------------
// used by CCamera to map WM_KEYDOWN keys
//--------------------------------------------------------------------------------------
enum D3DUtil_CameraKeys
{
    CAM_UNKNOWN = 0,
    CAM_STRAFE_LEFT = 1,
    CAM_STRAFE_RIGHT = 2,
    CAM_MOVE_FORWARD = 3,
    CAM_MOVE_BACKWARD = 4,
    CAM_MOVE_UP = 5,
    CAM_MOVE_DOWN = 6,
    CAM_MAX_KEYS = 7
};

//--------------------------------------------------------------------------------------
// Simple base camera class that moves and rotates.  The base class
//       records mouse and keyboard input for use by a derived class, and
//       keeps common state.
//--------------------------------------------------------------------------------------
class CDXUTBaseCamera
{
public:
    // Call these from client and use Get*Matrix() to read new matrices
    void HandleKeyDownMessage(D3DUtil_CameraKeys mappedKey);
    void HandleKeyUpMessage(D3DUtil_CameraKeys mappedKey);
    void HandleMouseMoveMessage(float normalizedX, float normalizedY, bool leftButton, bool middleButton, bool rightButton);

    // Functions to change camera state
    inline void SetEyePt(DirectX::XMFLOAT3 const &vEyePt) { m_vEyePt = vEyePt; }
    inline void SetEyeDir(DirectX::XMFLOAT3 const &vEyeDir) { m_vEyeDir = vEyeDir; }
    inline void SetUpDir(DirectX::XMFLOAT3 const &vUpDir) { m_vUpDir = vUpDir; }

    // Functions to change behavior
    inline void SetInvertPitch(bool bInvertPitch) { m_bInvertPitch = bInvertPitch; }
    inline void SetEnableUpDownMovement(bool bEnableUpDownMovement) { m_bEnableUpDownMovement = bEnableUpDownMovement; }

    inline void SetScalers(float fRotationScaler = DirectX::XM_PI, float fMoveScaler = 5.0F)
    {
        m_fRotationScaler = fRotationScaler;
        m_fMoveScaler = fMoveScaler;
    }

    // Functions to get state
    inline DirectX::XMVECTOR GetEyePt() const { return DirectX::XMLoadFloat3(&m_vEyePt); }
    inline DirectX::XMVECTOR GetEyeDir() const { return DirectX::XMLoadFloat3(&m_vEyeDir); }
    inline DirectX::XMVECTOR GetUpDir() const { return DirectX::XMLoadFloat3(&m_vUpDir); }

protected:
    CDXUTBaseCamera() noexcept;

    virtual bool GetKeyBoardInput() const = 0;
    void UpdateKeyboardDirection();
    virtual bool GetMouseInput() const = 0;
    void UpdateMouseDelta(DirectX::XMFLOAT2 const &ptCurMousePosition);
    void UpdateVelocity(DirectX::XMVECTOR &out_vRotationVelocity, DirectX::XMVECTOR &out_vMoveVelocity);

    bool m_bIsKeyDown[CAM_MAX_KEYS];         // State of input - KEY_IS_DOWN_MASK
    DirectX::XMFLOAT3 m_vKeyboardDirection;  // Direction vector of keyboard input
    DirectX::XMFLOAT2 m_ptLastMousePosition; // Last absolute position of mouse cursor
    int m_nCurrentButtonMask;                // mask of which buttons are down
    DirectX::XMFLOAT2 m_vMouseDelta;         // Mouse relative delta smoothed over a few frames
    DirectX::XMFLOAT3 m_vEyePt;              // Camera eye position
    DirectX::XMFLOAT3 m_vEyeDir;             // Camera eye direction
    DirectX::XMFLOAT3 m_vUpDir;              // Camera up direction

    float m_fRotationScaler; // Scaler for rotation
    float m_fMoveScaler;     // Scaler for movement

    bool m_bInvertPitch;          // Invert the pitch axis
    bool m_bEnableUpDownMovement; // If true, then camera can move in the up-axis
};

//--------------------------------------------------------------------------------------
// Simple first person camera class that moves and rotates.
//       It allows yaw and pitch but not roll.  It uses WM_KEYDOWN and
//       GetCursorPos() to respond to keyboard and mouse input and updates the
//       view matrix based on input.
//--------------------------------------------------------------------------------------
class CDXUTFirstPersonCamera : public CDXUTBaseCamera
{
public:
    CDXUTFirstPersonCamera() noexcept;

    // Call these from client and use Get*Matrix() to read new matrices
    void FrameMove(float fElapsedTime);

    // Functions to change behavior
    void SetRotateButtons(bool bLeft, bool bMiddle, bool bRight, bool bRotateWithoutButtonDown = false);
    inline void SetEnablePositionMovement(bool bEnablePositionMovement) { m_bEnablePositionMovement = bEnablePositionMovement; }

protected:
    bool GetKeyBoardInput() const override;
    bool GetMouseInput() const override;

    int m_nActiveButtonMask; // Mask to determine which button to enable for rotation
    bool m_bRotateWithoutButtonDown;
    bool m_bEnablePositionMovement; // If true, then the user can translate the camera/model
};

#endif
