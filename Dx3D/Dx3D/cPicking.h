#pragma once
#include "cObject.h"


class cPicking : public cObject
{
private:
    LPMESH              m_pSphere;          // �浹 ������ �׸� ��
    Vector3             m_vPickedPos;       // �浹 ����(���� �׸� ��ġ)
    vector<VertexPC>    m_vecPCVertex;      // �ٴ� �浹 ���ؽ�

    LPD3DXMESH              m_pSphere2;         // �浹 ������ �׸� ��
    vector<tagSphere>       m_vecSphere;        // �� ���� ����

public:
    cPicking();
    ~cPicking();

    void Setup();
    void Render();
    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

/*
������ ����ü �Ǵ� Ŭ���� ����
�� ����ü ����

���콺 Ŭ���� ���� ������ ����
��ũ�� => �� �����̽� (å ���� 15�� ��ŷ)
�� �����̽� => ���� �����̽�

�ٴ��� �����ϰ� �ٴڰ� �浹 ������ �� �׸���
(��� �������� �浹 �Լ� ��� : D3DXIntersectTri)

�������� �� �浹 (�� ����ü�� ��ŷ ���� �߰�)
�������� ���� ���� ����

/*
Q = �������� �����
V = �������� ����

������: P(t) = Q + Vt
==> P(x) = Qx + Vxt, P(y) = Qy + Vyt, P(z) = Qz + Vzt

��: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2
��: x^2 + y^2 + z^2 = r^2

���� �������� ���� ������: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

(Qx + Vxt)^2 => (Qx)^2 + 2(Qx)(Vxt) + (Vxt)^2
=> ((Vx)^2)(t^2) + 2(Qx)(Vx)t + (Qx)^2

���� �������� t�� ���� 2�� ���������� ����
(Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
(VV)t^2 + 2(QV)t + QQ - rr = 0

�Ǻ��� = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
�Ǻ��� >= 0 ������ �����Ѵ�(�浹)

���� �������� �浹�� ���� ������ ����(0, 0, 0)���� ó���Ͽ� ����ϴ� ���� ���ϴ�.
*/
