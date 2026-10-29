#include "editorWindows.h"
#include <EditorUtils/editorAssets.h>
#include <ServiceLocator/locator.h>
#include <ServiceLocator/renderer.h>
#include <imgui/imgui.h>

void EditorWindows::ShowViewportWindow()
{
	// Set the window flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// Note: Window position/size can be forced here, docking too?

	// Create the window
	ImGui::Begin("Viewport", NULL, window_flags);

	// Set the window content
	const ImVec2 viewport_size = ImGui::GetContentRegionAvail();
	std::shared_ptr<RenderTargetTexture> viewport_render_target = EditorAssets::GetViewportRenderTarget();

	ImGui::Image(
		(ImTextureID)(intptr_t)viewport_render_target->getColorTexture(),
		viewport_size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	// End the window section
	ImGui::End();

	// Resize the render target (after it has been used)
	viewport_render_target->resize((int)viewport_size.x, (int)viewport_size.y);
}