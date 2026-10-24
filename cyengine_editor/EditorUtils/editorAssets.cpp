#include "editorAssets.h"
#include <Assets/assetManager.h>

std::shared_ptr<RenderTargetTexture> EditorAssets::viewportRenderTarget;


std::shared_ptr<RenderTargetTexture> EditorAssets::GetViewportRenderTarget()
{
    return viewportRenderTarget;
}

void EditorAssets::LoadEditorAssets()
{
    viewportRenderTarget = RenderTargetTexture::Create(RenderTargetTexture::LoadParams{ 1280, 720 });
}

void EditorAssets::UnloadEditorAssets()
{
    viewportRenderTarget = nullptr;
}
