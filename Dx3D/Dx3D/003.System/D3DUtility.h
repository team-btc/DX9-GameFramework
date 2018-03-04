#pragma once


namespace D3DUTILITY
{
    MATERIAL9 InitMtrl(XColor a, XColor d, XColor s, XColor e, float p);

    const MATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
    const MATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
    const MATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
    const MATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
    const MATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);

    D3DLIGHT9 InitDirectional(Vector3* dir, XColor* c);
    D3DLIGHT9 InitPoint(Vector3* pos, XColor* c);
    D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, XColor* c);
}