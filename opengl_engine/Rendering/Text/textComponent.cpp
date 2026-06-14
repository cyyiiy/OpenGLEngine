#include "textComponent.h"


void TextComponent::setTextDatas(const std::string& text_, const Font& font_)
{
	text = text_;
	font = &font_;
	recomputeTextSize();
}

void TextComponent::setText(const std::string& text_)
{
	text = text_;
	recomputeTextSize();
}

const std::string TextComponent::getText() const
{
	return text;
}

void TextComponent::setFont(const Font& font_)
{
	font = &font_;
	recomputeTextSize();
}

const Font* TextComponent::getFont() const
{
	return font;
}

const Vector2 TextComponent::getTextSize() const
{
	return textSize * scale;
}


void TextComponent::recomputeTextSize()
{
	if (font == nullptr)
	{
		textSize = Vector2::zero;
		return;
	}

	int text_width = 0, text_height = 0;

	std::string text_line;

	// Iterate through every character of the text
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		if (*c == '\n')
		{
			// If a line return is detected, compute the size of the line and add the line return offset
			FontCharacter ch = font->getCharacter(*c);
			text_height += static_cast<int>(static_cast<float>(ch.size.y) * 0.6f);

			computeTextLineSize(text_line, text_width, text_height);
			text_line.clear();
		}
		else
		{
			text_line.push_back(*c);
		}
	}

	// Compute the last line
	computeTextLineSize(text_line, text_width, text_height);

	textSize = Vector2{ (float)(text_width), (float)(text_height) };
}

void TextComponent::computeTextLineSize(std::string textLine, int& textWidth, int& textHeight)
{
	int line_width = 0, max_line_height = 0;

	// Iterate through every character of the line of text
	std::string::const_iterator c;
	for (c = textLine.begin(); c != textLine.end(); c++)
	{
		FontCharacter ch = font->getCharacter(*c);

		line_width += (ch.advance >> 6); // Bitshift by 6 (2^6 = 64) to advance the character size

		max_line_height = Maths::max<int>(max_line_height, ch.size.y);
	}

	textWidth = Maths::max<int>(textWidth, line_width);
	textHeight += max_line_height;
}