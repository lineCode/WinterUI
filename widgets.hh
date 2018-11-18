#pragma once

#include <functional>
#include <cstdint>
#include <vector>
#include <beast/def.hh>
#include <iris/vec2.hh>

struct Layout
{
	virtual ~Layout() = default;
};

struct GUIElement
{
	virtual ~GUIElement() = default;
	virtual void render() = 0;
	virtual void onResize(uint32_t newWidth, uint32_t newHeight) = 0;
	void addLayout(SP<Layout> const &layout);
	void addElement(SP<GUIElement> const &element);
	
	IR::vec2<float> pos, size, margin, padding;

private:
	std::vector<SP<GUIElement>> childElements;
	SP<Layout> layout;
};

/*Layouts--------------------------------------------------------------------------------------------------------------------------------*/

/// 
struct VBoxLayout : public Layout
{
	
};

/// 
struct HBoxLayout : public Layout
{
	
};

/// 
struct FlexBoxLayout : public Layout
{
	
};

/*Components-----------------------------------------------------------------------------------------------------------------------------*/

/// 
struct Pane : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
};

/// 
struct Button : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	std::function<void(bool down)> callback;
};

/// 
struct Checkbox : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	bool value;
};

/// 
struct RadioButtonBank : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	uint32_t selected = 0;
};

/// 
struct Slider : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	float value = 0.0f;
};

/// 
struct DropdownMenu : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	uint32_t selected = 0;
};

/// 
struct MenuBar : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
};

/// 
struct MenuItem : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	std::string text;
};

/// 
struct Label : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	std::string text;
};

/// 
struct TextLine : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	std::string text;
};

/// 
struct TextArea : public GUIElement
{
	void render() override;
	void onResize(uint32_t newWidth, uint32_t newHeight) override;
	std::string text;
};
