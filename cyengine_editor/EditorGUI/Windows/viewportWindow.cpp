#include "editorWindows.h"
#include <EditorUtils/editorAssets.h>
#include <EditorUtils/editorStatics.h>
#include <Core/window.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>


void EditorWindows::ShowViewportWindow()
{
	// Set the window flags
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	// Create the window
	ImGui::Begin(VIEWPORT_WINDOW_NAME, NULL, window_flags);

	// Set the window content
	const ImVec2 viewport_size = ImGui::GetContentRegionAvail();
	std::shared_ptr<RenderTargetTexture> viewport_render_target = EditorAssets::GetViewportRenderTarget();

	ImGui::Image(
		(ImTextureID)(intptr_t)viewport_render_target->getColorTexture(),
		viewport_size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	// Check if the viewport window is focused
	Window& window_ref = *EditorStatics::window;

	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		EditorStatics::isViewportFocused = true;
		glfwSetInputMode(window_ref.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (EditorStatics::isViewportFocused && 
		(ImGui::IsMouseReleased(ImGuiMouseButton_Right) ||
			!glfwGetWindowAttrib(window_ref.getGLFWwindow(), GLFW_FOCUSED)))
	{
		EditorStatics::isViewportFocused = false;
		glfwSetInputMode(window_ref.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// End the window section
	ImGui::End();

	// Resize the render target (after it has been used)
	viewport_render_target->resize((int)viewport_size.x, (int)viewport_size.y);
}