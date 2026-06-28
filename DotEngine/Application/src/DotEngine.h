#pragma once

// --- EngineCore ---
#include "CoreMacros.h"
#include "Log.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"
#include "EngineContext.h"
#include "Events/EventTypes.h"
#include "Events/EventQueue.h"
#include "Events/EventSystem.h"

// --- Renderer ---
#include "RenderTypes.h"
#include "BufferSystem.h"
#include "ShaderSystem.h"
#include "RenderSystem.h"

// --- GameInput ---
#include "GameInputTypes.h"
#include "GameInputSystem.h"

#ifdef DOT_EDITOR
    // --- Editor ---
    #include "EditorContext.h"
    #include "EditorSystem.h"
    #include "EditorPanelSystem.h"
    #include "EditorEventSystem.h"
#endif

// --- Application ---
#include "ApplicationContext.h"
#include "Application.h"

// --- Entry Point ---
#include "EntryPoint.h"
