#ifndef __TYPES_H__
#define __TYPES_H__
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
        None = 0,
        PlatformLeftEnd = 1,
        PlatformMidPart = 2,
        PlatformRightEnd = 3,

        EndOfLevel = 4,

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
        TwinShot,

        COUNT
    };
}

std::string EntityTypeAsString(ET::EntityType et);

// typ fade effectu
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
