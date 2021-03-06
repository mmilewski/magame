#ifndef MAGAME_TYPES_H_INCLUDED
#define MAGAME_TYPES_H_INCLUDED
#include "StdAfx.h"

namespace PS {
    enum PlayerState {
        Stand,
        GoLeft,
        GoRight,
        TurnLeft,   // postać zwrócona w lewo
        TurnRight   // postać zwrócona w prawo
    };
}

namespace DL {
    enum DisplayLayer {
        Foreground = 1,  // przedni plan
        Player = 2,      // plan z graczem
        Entity = 2,      // plan z jednostkami
        Background = 3   // tło
    };
}

namespace FT {
    enum FieldType {

        // NOTE: jeżeli chcesz zmienić poniższą listę, upewnij się, że funkcja
        // Level::IsFieldCollidable zwróci poprawną wartość

        None = 0,
        EndOfLevel = 1,

        COLLIDING_START = 10000,

        PlatformTopLeft, // 10001
        PlatformLeft, // 10002
        PlatformMid, // 10003
        PlatformTop, // 10004
        PlatformLeftTopRight, // 10005
        PlatformLeftRight, // 10006
        PlatformTopRight, // 10007
        PlatformRight, // 10008

        COLLIDING_END,

        NON_COLLIDING_START = 20000,

        NcPlatformTopLeft,
        NcPlatformLeft,
        NcPlatformMid,
        NcPlatformTop,
        NcPlatformLeftTopRight,
        NcPlatformLeftRight,
        NcPlatformTopRight,
        NcPlatformRight,
        NcCandleWithBg,
        NcCandleNoBg,
        NcCrackWithBg,
        NcCrackNoBg,

        NON_COLLIDING_END,

        COUNT
    };
}

// stan jednostki
namespace ES {
    enum EntityState {
        Stand,
        GoLeft,
        GoRight
    };
}

// typ jednostki
namespace ET {
    enum EntityType {
        UNKNOWN,

        Mush,
        PlayerBullet,
        SingleShot,
        TwinShot,
        HigherJump,
        Orb,

        SavePoint,
        Thorns,
        Arrow,
        ArrowTrigger,
        Column,

        COUNT
    };
}

std::string EntityTypeAsString(ET::EntityType et);
ET::EntityType StringAsEntityType(std::string str);

// typ efekty przejscia
namespace TransitionEffectType {
    enum Type {
        UNKNOWN,

        FadeIn,
        FadeOut,
        PinWheelOut,

        COUNT
    };
}

#endif
