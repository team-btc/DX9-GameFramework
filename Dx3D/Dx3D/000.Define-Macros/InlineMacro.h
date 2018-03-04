#pragma once

inline float DwordToFloat(DWORD DWORD_Num)
{
    return *((FLOAT*)&DWORD_Num);
}

inline DWORD FloatToDword(float FLOAT_Num)
{
    // 플롯의 주소를 DWORD포인터로 만들고 얘의 실체를 보냄
    return  *((DWORD*)&FLOAT_Num);
    // 포인터는 비트 자체가 의미 있는 값이므로, 가리키는 대상이 다른 포인터 사이의 형 변환은 
    // 이진 비트열을 파괴하지 않는다.
    // long : 4바이트 –2,147,483,648 ~ 2,147,483,647
    // unsigned long : 4바이트 0 ~ 4,294,967,295
    // float : 4바이트 3.4E+/-38(7개의 자릿수)
}

inline float GetRandomFloat(float fMin, float fMax)
{
    // rand() 0 ~ 32676 사이의 난수(int)정수 발생
    // RAND_MAX == 32767
    // 4.0f ~ 4.5f
    // 랜덤이 4가 나왔을 때
    // ( (0.5f) * 4 / 32676)    
    return ((fMax - fMin) * ((float)rand() / RAND_MAX)) + fMin;
}

inline Vector3 GetRandomVector3(Vector3 vMin, Vector3 vMax)
{
    float x = GetRandomFloat(vMin.x, vMax.x);
    float y = GetRandomFloat(vMin.y, vMax.y);
    float z = GetRandomFloat(vMin.z, vMax.z);

    return Vector3(x, y, z);
}