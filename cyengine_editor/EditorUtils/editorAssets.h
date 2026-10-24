#pragma once
#include <Rendering/renderTargetTexture.h>
#include <memory>

class EditorAssets
{
public:
	[[nodiscard]] static std::shared_ptr<RenderTargetTexture> GetViewportRenderTarget();

private:
	friend class Editor;

	static void LoadEditorAssets();
	static void UnloadEditorAssets();

	static std::shared_ptr<RenderTargetTexture> viewportRenderTarget;
};

