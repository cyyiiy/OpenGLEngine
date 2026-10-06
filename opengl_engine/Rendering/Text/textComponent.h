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
	void setTextDatas(const std::string& text_, std::shared_ptr<Font> font_);

	void setText(const std::string& text_);
	const std::string getText() const;

	void setFont(std::shared_ptr<Font> font_);
	const Font& getFont() const;

	const Vector2 getTextSize() const;

	/** Return true if this component has a valid font and a non null text. */
	bool isValid() const;

// Note: text and font aren't public because 'textSize' need to be re-computed each time they are changed.
private:
	std::string text{ "" };
	std::shared_ptr<Font> font{ nullptr };
	Vector2 textSize{ Vector2::zero };

	void recomputeTextSize();
	void computeTextLineSize(std::string textLine, int& textWidth, int& textHeight);
};


// Specify sublist size for 'TextComponent'
template<>
struct ComponentSublistSize<TextComponent>
{
	static constexpr size_t value = 16;
};