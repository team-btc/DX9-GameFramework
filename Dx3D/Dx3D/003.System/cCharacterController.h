#pragma once

class cCharacterController
{
private:
    Matrix4         m_matWorld;
    Vector3         m_vPosition;
    Vector3         m_vDest;
    Vector3         m_vDir;
    float           m_fMoveSpeed;
    vector<Vector3> m_vecPath;

    bool            m_isDebugMode;

    SYNTHESIZE(bool, m_isMovable, Move);

public:
    cCharacterController();
    ~cCharacterController();

    void Update();
    void Render();

    Vector3* GetPosition() { return &m_vPosition; }
    Vector3* GetDirection() { return &m_vDir; }
    Matrix4* GetWorldTM() { return &m_matWorld; }
    Vector3* GetDestPosition();
    bool IsStop();
};

