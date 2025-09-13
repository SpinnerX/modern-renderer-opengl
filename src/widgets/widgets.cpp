#include <widgets/widgets.hpp>
#include <imgui.h>

void custom_healthbard(float& p_progress, const framebuffer_extent& p_extent) {
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(p_extent.width) * 0.5f, static_cast<float>(p_extent.height) * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(200, 20), ImGuiCond_Always);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
    ImGui::SetNextWindowBgAlpha(0.f);

    ImGui::Begin("Testing", nullptr, flags);
    ImGui::ProgressBar(10.f);
    ImGui::End();
}