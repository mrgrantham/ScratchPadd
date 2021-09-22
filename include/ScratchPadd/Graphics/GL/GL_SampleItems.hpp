#pragma once
#include <imgui.h>

float positions[] = {
     -0.5f, -0.5f,
      0.0f, 0.5f,
      0.5f, -0.5f
};

std::vector<Graphics::VertexPair> vertices = {
      {{-0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 0.0f}},
      {{0.0f, 0.5f, 0.0f},{0.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f, 0.0f},{0.0f, 0.0f, 0.0f}}
};

std::vector<uint32_t> indices = {
  0, 1, 2
};

namespace ScratchPadd {
      namespace Color {
      ImVec4 Light_Blue = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      ImVec4 Light_Gray = ImVec4(0.90f, 0.90, 0.90, 1.00f);
      ImVec4 Blue = ImVec4(0.20f, 0.20f, 0.70f, 1.00f);
      ImVec4 Red = ImVec4(0.90f, 0.35f, 0.30f, 1.00f);
      ImVec4 App_Background = Light_Gray;
      ImVec4 Shape_Color = Red;
      }
}
