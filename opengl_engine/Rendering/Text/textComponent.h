#pragma once
#include <ECS/component.h>
#include <Rendering/Hud/hudElement.h>
#include "font.h"


/** Text Component
* Component for rendering hud text.
* Note: HUD elements don't use their entity's transform.
*/
class TextComponent : public Component, public HudElement
{
public:
	void setTextDatas(const std::string& text_, const Font& font_);

	void setText(const std::string& text_);
	const std::string getText() const;

	void setFont(const Font& font_);
	const Font* getFont() const;

	const Vector2 getTextSize() const;

// Note: text and font aren't public because 'textSize' need to be re-computed each time they are changed.
private:
	std::string text{ "" };
	const Font* font{ nullptr };
	Vector2 textSize{ Vector2::zero };

	void recomputeTextSize();
	void computeTextLineSize(std::string textLine, int& textWidth, int& textHeight);
};


// Specify sublist size for 'TextComponent'
template<>
struct ComponentSublistSize<TextComponent>
{
	static constexpr size_t value = 8;
};