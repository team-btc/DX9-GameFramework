#pragma once
#include "cObject.h"


class cPicking : public cObject
{
private:
    LPMESH              m_pSphere;          // 충돌 지점에 그릴 구
    Vector3             m_vPickedPos;       // 충돌 지점(구를 그릴 위치)
    vector<VertexPC>    m_vecPCVertex;      // 바닥 충돌 버텍스

    LPD3DXMESH              m_pSphere2;         // 충돌 지점에 그릴 구
    vector<tagSphere>       m_vecSphere;        // 구 정보 벡터

public:
    cPicking();
    ~cPicking();

    void Setup();
    void Render();
    void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

/*
반직선 구조체 또는 클래스 생성
구 구조체 생성

마우스 클릭에 의한 반직선 생성
스크린 => 뷰 스페이스 (책 참조 15장 픽킹)
뷰 스페이스 => 월드 스페이스

바닥을 생성하고 바닥과 충돌 지점에 구 그리기
(면과 반직선의 충돌 함수 사용 : D3DXIntersectTri)

반직선과 구 충돌 (구 구조체에 픽킹 정보 추가)
랜덤으로 구를 많이 띠우기

/*
Q = 반직선의 출발점
V = 반직선의 방향

반직선: P(t) = Q + Vt
==> P(x) = Qx + Vxt, P(y) = Qy + Vyt, P(z) = Qz + Vzt

구: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2
구: x^2 + y^2 + z^2 = r^2

구와 반직선의 연립 방정식: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

(Qx + Vxt)^2 => (Qx)^2 + 2(Qx)(Vxt) + (Vxt)^2
=> ((Vx)^2)(t^2) + 2(Qx)(Vx)t + (Qx)^2

연립 방정식을 t에 대한 2차 방적식으로 정리
(Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
(VV)t^2 + 2(QV)t + QQ - rr = 0

판별식 = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
판별식 >= 0 교점이 존재한다(충돌)

구와 반직선의 충돌시 구의 중점을 원점(0, 0, 0)으로 처리하여 계산하는 것이 편하다.
*/
