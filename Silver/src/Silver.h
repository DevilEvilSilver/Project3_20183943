#pragma once

// Core -----------------------
#include "Silver/Application.h"
#include "Silver/Layer.h"
#include "Events/Input.h"
#include "Events/EventCode.h"

// Utils ----------------------
#include "Utils/Utils.h"
#include "Utils/Log.h"
#include "Utils/Timer.h"
#include "Utils/Singleton.h"

// Renderer -------------------
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Camera/CameraLookAt.h"

// Data ------------------------
#include "DataManager/Resources/ResourceManager.h"
#include "DataManager/Scenes/Scene.h"
#include "DataManager/ECS/Components.h"
#include "DataManager/ECS/Model/Model.h"

// Debug -----------------------
#include "ImGui/ImGuiLayer.h"
