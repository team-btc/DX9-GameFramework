#pragma once

#define WHITE   XColor(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK   XColor(0.0f, 0.0f, 0.0f, 1.0f)
#define RED     XColor(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN   XColor(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE    XColor(0.0f, 0.0f, 1.0f, 1.0f)
#define YELLOW  XColor(1.0f, 1.0f, 0.0f, 1.0f)

namespace D3DUTILITY
{
    MATERIAL9 InitMtrl(XColor a, XColor d, XColor s, XColor e, float p);

    const MATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
    const MATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
    const MATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
    const MATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
    const MATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);

    void ComputeNormal(Vector3* out, Vector3* p0, Vector3* p1, Vector3* p2);

    D3DLIGHT9 InitDirectional(Vector3* dir, XColor* c);
    D3DLIGHT9 InitPoint(Vector3* pos, XColor* c);
    D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, XColor* c);
}