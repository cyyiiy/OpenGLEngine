#pragma once
#include <Rendering/Hud/hudComponent.h>
#include <string>

class Font;


/** Text Renderer Component
* Component for rendering a 2D text on the screen.
* This component doesn't use the transform of the Entity.
*/
class TextRendererComponent : public HudComponent
{
public:
	void setTextDatas(const std::string& text_, const Font& textFont_, const Vector2& screenAnchor_, const Vector2& pivot_, const Vector2& pos_, const Vector2& scale_, const float rotAngle_, const Color& tintColor_);

	void setText(const std::string& text_);
	const std::string& getText() const;

	void setTextFont(const Font& textFont_);
	const Font& getTextFont() const;

	const Vector2& getRawTextSize() const;

	Vector2 getSize() const override;

protected:
	virtual void registerComponent() override;
	virtual void unregisterComponent() override;

	void init() override;
	void exit() override;

	bool needToComputeMatrix() const override;

private:
	std::string text{ "" };
	const Font* textFont{ nullptr };
	Vector2 textSize{ Vector2::zero };

	void recomputeTextSize();
	void computeTextLineSize(std::string textLine, int& textWidth, int& textHeight);
};