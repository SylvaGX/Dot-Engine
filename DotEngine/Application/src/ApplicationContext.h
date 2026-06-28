#pragma once

#include "EngineContext.h"
#include "GameInputTypes.h"

#ifdef DOT_EDITOR
    #include "EditorContext.h"
#endif

namespace DotEngine {

    struct ApplicationContext {
        EngineContext  engine;
        GameInputState gameInput;

#ifdef DOT_EDITOR
        EditorContext editor;
#endif
    };

}
