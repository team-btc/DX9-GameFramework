#pragma once

// 지형 타입
enum E_GROUND_TYPE {
    E_SOIL_GROUND,      // 흙 지형
    E_WATER_GROUND,     // 물 지형
    E_GRASS_GROUND,     // 풀 지형
    E_STONE_GROUND      // 돌 지형
};

// 이벤트 트랩 타입
enum E_EVENT_TRAP_TYPE {
    E_EVENT_TRAP_NONE,
    E_MONSTER_TRAP
};