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
    CAM_STRAFE_LEFT = 0,
    CAM_STRAFE_RIGHT,
    CAM_MOVE_FORWARD,
    CAM_MOVE_BACKWARD,
    CAM_MOVE_UP,
    CAM_MOVE_DOWN,
    CAM_RESET,
    CAM_CONTROLDOWN,
    CAM_MAX_KEYS,
    CAM_UNKNOWN = 0xFF
};

#define KEY_IS_DOWN_MASK 0x01

#define MOUSE_LEFT_BUTTON 0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON 0x04

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

    // Functions to change camera matrices
    void Reset();
    void SetViewParams(_In_ DirectX::CXMVECTOR vEyePt, _In_ DirectX::CXMVECTOR vLookatPt, _In_ DirectX::CXMVECTOR vUp);
    void SetProjParams(_In_ float fFOV, _In_ float fAspect, _In_ float fNearPlane, _In_ float fFarPlane);

    // Functions to change behavior
    inline void SetRightHanded(_In_ bool bRightHanded) { m_bRightHanded = bRightHanded; }
    inline void SetInvertPitch(_In_ bool bInvertPitch) { m_bInvertPitch = bInvertPitch; }
    inline void SetDrag(_In_ bool bMovementDrag, _In_ float fTotalDragTimeToZero = 0.25f)
    {
        m_bMovementDrag = bMovementDrag;
        m_fTotalDragTimeToZero = fTotalDragTimeToZero;
    }
    inline void SetEnableYAxisMovement(_In_ bool bEnableYAxisMovement) { m_bEnableYAxisMovement = bEnableYAxisMovement; }
    inline void SetEnablePositionMovement(_In_ bool bEnablePositionMovement) { m_bEnablePositionMovement = bEnablePositionMovement; }
    inline void SetClipToBoundary(_In_ bool bClipToBoundary, _In_opt_ DirectX::XMFLOAT3 *pvMinBoundary, _In_opt_ DirectX::XMFLOAT3 *pvMaxBoundary)
    {
        m_bClipToBoundary = bClipToBoundary;
        if (pvMinBoundary)
            m_vMinBoundary = *pvMinBoundary;
        if (pvMaxBoundary)
            m_vMaxBoundary = *pvMaxBoundary;
    }
    inline void SetScalers(_In_ float fRotationScaler = 1.0f, _In_ float fMoveScaler = 5.0f)
    {
        m_fRotationScaler = fRotationScaler;
        m_fMoveScaler = fMoveScaler;
    }
    inline void SetNumberOfFramesToSmoothMouseData(_In_ int nFrames)
    {
        if (nFrames > 0)
            m_fFramesToSmoothMouseData = (float)nFrames;
    }
    inline void SetResetCursorAfterMove(_In_ bool bResetCursorAfterMove) { m_bResetCursorAfterMove = bResetCursorAfterMove; }

    // Functions to get state
    inline DirectX::XMMATRIX GetViewMatrix() const { return DirectX::XMLoadFloat4x4(&m_mView); }
    inline DirectX::XMMATRIX GetProjMatrix() const { return DirectX::XMLoadFloat4x4(&m_mProj); }
    inline DirectX::XMVECTOR GetEyePt() const { return DirectX::XMLoadFloat3(&m_vEye); }
    inline DirectX::XMVECTOR GetLookAtPt() const { return DirectX::XMLoadFloat3(&m_vLookAt); }
    inline DirectX::XMVECTOR GetUp() const { return DirectX::XMLoadFloat3(&m_vUp); }
    inline float GetFOV() const { return m_fFOV; }
    inline float GetNearClip() const { return m_fNearPlane; }
    inline float GetFarClip() const { return m_fFarPlane; }

    inline bool IsBeingDragged() const { return (m_bMouseLButtonDown || m_bMouseMButtonDown || m_bMouseRButtonDown); }
    inline bool IsMouseLButtonDown() const { return m_bMouseLButtonDown; }
    inline bool IsMouseMButtonDown() const { return m_bMouseMButtonDown; }
    inline bool sMouseRButtonDown() const { return m_bMouseRButtonDown; }

protected:
    CDXUTBaseCamera() noexcept;

    inline bool IsKeyDown(_In_ uint8_t key) const { return ((key & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK); }

    DirectX::XMVECTOR XM_CALLCONV ConstrainToBoundary(_In_ DirectX::FXMVECTOR v)
    {
        DirectX::XMVECTOR vMin = DirectX::XMLoadFloat3(&m_vMinBoundary);
        DirectX::XMVECTOR vMax = DirectX::XMLoadFloat3(&m_vMaxBoundary);

        // Constrain vector to a bounding box
        return DirectX::XMVectorClamp(v, vMin, vMax);
    }

    virtual bool GetKeyBoardInput() const = 0;
    void UpdateKeyboardDirection();
    virtual bool GetMouseInput() const = 0;
    void UpdateMouseDelta(DirectX::XMFLOAT2 const &ptCurMousePos);
    void UpdateVelocity(_In_ float fElapsedTime);

    DirectX::XMFLOAT4X4 m_mView; // View matrix
    DirectX::XMFLOAT4X4 m_mProj; // Projection matrix

    int m_cKeysDown;                         // Number of camera keys that are down.
    uint8_t m_aKeys[CAM_MAX_KEYS];           // State of input - KEY_IS_DOWN_MASK
    DirectX::XMFLOAT3 m_vKeyboardDirection;  // Direction vector of keyboard input
    DirectX::XMFLOAT2 m_ptLastMousePosition; // Last absolute position of mouse cursor
    int m_nCurrentButtonMask;                // mask of which buttons are down
    DirectX::XMFLOAT2 m_vMouseDelta;         // Mouse relative delta smoothed over a few frames
    float m_fFramesToSmoothMouseData;        // Number of frames to smooth mouse data over
    DirectX::XMFLOAT3 m_vDefaultEye;         // Default camera eye position
    DirectX::XMFLOAT3 m_vDefaultLookAt;      // Default LookAt position
    DirectX::XMFLOAT3 m_vDefaultUp;          // Default up direction
    DirectX::XMFLOAT3 m_vEye;                // Camera eye position
    DirectX::XMFLOAT3 m_vLookAt;             // LookAt position
    DirectX::XMFLOAT3 m_vUp;                 // Up direction
    float m_fCameraYawAngle;                 // Yaw angle of camera
    float m_fCameraPitchAngle;               // Pitch angle of camera

    DirectX::XMFLOAT3 m_vVelocity;     // Velocity of camera
    DirectX::XMFLOAT3 m_vVelocityDrag; // Velocity drag force
    float m_fDragTimer;                // Countdown timer to apply drag
    float m_fTotalDragTimeToZero;      // Time it takes for velocity to go from full to 0
    DirectX::XMFLOAT2 m_vRotVelocity;  // Velocity of camera

    float m_fFOV;       // Field of view
    float m_fAspect;    // Aspect ratio
    float m_fNearPlane; // Near plane
    float m_fFarPlane;  // Far plane

    float m_fRotationScaler; // Scaler for rotation
    float m_fMoveScaler;     // Scaler for movement

    bool m_bMouseLButtonDown;       // True if left button is down
    bool m_bMouseMButtonDown;       // True if middle button is down
    bool m_bMouseRButtonDown;       // True if right button is down
    bool m_bMovementDrag;           // If true, then camera movement will slow to a stop otherwise movement is instant
    bool m_bRightHanded;            // Right-Handed
    bool m_bInvertPitch;            // Invert the pitch axis
    bool m_bEnablePositionMovement; // If true, then the user can translate the camera/model
    bool m_bEnableYAxisMovement;    // If true, then camera can move in the y-axis
    bool m_bClipToBoundary;         // If true, then the camera will be clipped to the boundary
    bool m_bResetCursorAfterMove;   // If true, the class will reset the cursor position so that the cursor always has space to move

    DirectX::XMFLOAT3 m_vMinBoundary; // Min point in clip boundary
    DirectX::XMFLOAT3 m_vMaxBoundary; // Max point in clip boundary
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
    void FrameMove(_In_ float fElapsedTime);

    // Functions to change behavior
    void SetRotateButtons(_In_ bool bLeft, _In_ bool bMiddle, _In_ bool bRight, _In_ bool bRotateWithoutButtonDown = false);

    // Functions to get state
    inline DirectX::XMMATRIX GetWorldMatrix() const { return DirectX::XMLoadFloat4x4(&m_mCameraWorld); }

    inline DirectX::XMVECTOR GetWorldRight() const { return DirectX::XMVector3TransformNormal(DirectX::g_XMIdentityR0, DirectX::XMLoadFloat4x4(&m_mCameraWorld)); }
    inline DirectX::XMVECTOR GetWorldUp() const { return DirectX::XMVector3TransformNormal(DirectX::g_XMIdentityR1, DirectX::XMLoadFloat4x4(&m_mCameraWorld)); }
    inline DirectX::XMVECTOR GetWorldAhead() const { return DirectX::XMVector3TransformNormal(m_bRightHanded ? DirectX::g_XMNegIdentityR2 : DirectX::g_XMIdentityR2, DirectX::XMLoadFloat4x4(&m_mCameraWorld)); }
    inline DirectX::XMVECTOR GetEyePt() const { return DirectX::XMVector3TransformCoord(DirectX::g_XMZero, DirectX::XMLoadFloat4x4(&m_mCameraWorld)); }

protected:
    bool GetKeyBoardInput() const override;
    bool GetMouseInput() const override;

    DirectX::XMFLOAT4X4 m_mCameraWorld; // World matrix of the camera (inverse of the view matrix)

    int m_nActiveButtonMask; // Mask to determine which button to enable for rotation
    bool m_bRotateWithoutButtonDown;
};

#endif
