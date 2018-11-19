#pragma once

#include "api.hh"
#include "signal.hh"
#include "definitions.hh"
#include "pixmap.hh"

#include <functional>
#include <cstdint>
#include <vector>
#include <iris/vec2.hh>
#include <iris/shapes.hh>

#include <glad/glad.h>

enum MouseButtons
{
	NONE = 0,
	LEFT = 1,
	MIDDLE = 2,
	RIGHT = 3,
	X1 = 4,
	X2 = 5,
};

struct Layout
{
protected:
	Layout() = default;

public:
	virtual ~Layout() = default;
};

struct GUIElement
{
	virtual ~GUIElement() = default;
	
	//The pure virtual functions need to be called by the user of this library at appropriate times
	WINTERGUI_API virtual void render() = 0;
	
	//Events
	WINTERGUI_API virtual void onResize(uint32_t newWidth, uint32_t newHeight) = 0;
	WINTERGUI_API virtual void onMouseUp(MouseButtons button) {}
	WINTERGUI_API virtual void onMouseDown(MouseButtons button) {}
	WINTERGUI_API virtual void onMouseMove(IR::vec2<int32_t> const &newPos) {}
	WINTERGUI_API template <typename T> virtual void onKeyDown(T key) {}
	WINTERGUI_API template <typename T> virtual void onKeyUp(T key) {}
	WINTERGUI_API virtual void onTextInput(std::string const &input) {}
	WINTERGUI_API virtual void onFocus() {}
	
	WINTERGUI_API inline void addLayout(SP<Layout> const &layout)
	{
		this->layout = layout;
	}
	
	WINTERGUI_API inline void addElement(SP<GUIElement> const &element)
	{
		this->childElements.push_back(element);
	}
	
	IR::vec2<int32_t> pos, size;

private:
	inline IR::aabb2D<int32_t> getHitbox()
	{
		return IR::aabb2D<int32_t>(this->pos.x(), this->pos.x() + this->size.x(), this->pos.y() - this->size.y(), this->pos.y());
	}
	
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
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void customTex(Pixmap const &pixmap)
	{
		this->pixmap = pixmap;
	}
	
	Signal clicked = {};

private:
	Pixmap pixmap;
};

/// 
struct Button : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		//render based on state
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		if(button == LEFT) this->pressing = false;
		this->stateChanged.fire(button, this->pressing);
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		if(button == LEFT) this->pressing = true;
		this->stateChanged.fire(button, this->pressing);
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		auto hitbox = IR::aabb2D<float>(this->pos.x(), this->pos.x() + this->size.x(), this->pos.y() - this->size.y(), this->pos.y());
		if(hitbox.containsPoint(newPos.x(), newPos.y()))
		{
			if(!this->hovering)
			{
				this->hovering = true;
				this->onHover.fire();
			}
		}
		else
		{
			this->hovering = false;
		}
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, PRESSED
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::NORMAL:
				this->pixmapNormal = pixmap;
				break;
			case PixmapElem::HOVER:
				this->pixmapHover = pixmap;
				break;
			case PixmapElem::PRESSED:
				this->pixmapPressed = pixmap;
				break;
		}
	}
	
	bool hovering = false, pressing = false;
	Signal stateChanged = {};
	Signal onHover = {};

private:
	Pixmap pixmapNormal, pixmapHover, pixmapPressed;
};

/// 
struct Checkbox : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		this->value = !this->value;
		this->stateChanged.fire(this->value);
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		//change frame?
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, CHECKED
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::NORMAL:
				this->pixmapNormal = pixmap;
				break;
			case PixmapElem::HOVER:
				this->pixmapHover = pixmap;
				break;
			case PixmapElem::CHECKED:
				this->pixmapChecked = pixmap;
				break;
		}
	}
	
	bool value = false;
	Signal stateChanged = {};
	Signal keyPressed = {};
	
private:
	Pixmap pixmapNormal, pixmapHover, pixmapChecked;
};

/// 
struct RadioButtonBank : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		
	}
	
	enum struct PixmapElem
	{
		NORMAL, SELECTED
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::NORMAL:
				this->pixmapNormal = pixmap;
				break;
			case PixmapElem::SELECTED:
				this->pixmapSelected = pixmap;
				break;
		}
	}
	
	uint32_t selected = 0;
	Signal stateChanged = {};

private:
	Pixmap pixmapNormal, pixmapSelected;
};

/// 
struct Slider : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		
	}
	
	enum struct PixmapElem
	{
		RAIL, HANDLE, ICON
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::RAIL:
				this->pixmapRail = pixmap;
				break;
			case PixmapElem::HANDLE:
				this->pixmapHandle = pixmap;
				break;
			case PixmapElem::ICON:
				this->pixmapIcon = pixmap;
				break;
		}
	}
	
	float value = 0.0f;
	Signal stateChanged = {};

private:
	Pixmap pixmapRail, pixmapHandle, pixmapIcon;
};

/// 
struct DropdownMenu : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		FRAME, FRAMEBORDER, EXTFRAME, EXTFRAMEBORDER
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::FRAME:
				this->pixmapFrame = pixmap;
				break;
			case PixmapElem::FRAMEBORDER:
				this->pixmapFrameBorder = pixmap;
				break;
			case PixmapElem::EXTFRAME:
				this->pixmapExtendedFrame = pixmap;
				break;
			case PixmapElem::EXTFRAMEBORDER:
				this->pixmapExtendedFrameBorder = pixmap;
				break;
		}
	}
	
	uint32_t selected = 0;
	Signal onOpen = {};
	Signal stateChanged = {};

private:
	Pixmap pixmapFrame, pixmapFrameBorder, pixmapExtendedFrame, pixmapExtendedFrameBorder;
};

/// 
struct MenuBar : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::BACKGROUND:
				this->pixmapBackground = pixmap;
				break;
			case PixmapElem::BORDER:
				this->pixmapBorder = pixmap;
				break;
		}
	}
	
	Signal onOpen = {};
	Signal hover {};

private:
	Pixmap pixmapBackground, pixmapBorder;
};

/// 
struct MenuItem : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyDown(T key) override
	{
		
	}
	
	WINTERGUI_API template <typename T> void onKeyUp(T key) override
	{
		
	}
	
	std::string text = "";
	Signal clicked {};
	Signal hover {};
};

/// 
struct Label : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void customTex(Pixmap const &pixmap)
	{
		this->pixmapBackground = pixmap;
	}
	
	std::string text = "";
	Signal hover {};

private:
	Pixmap pixmapBackground;
};

/// 
struct TextLine : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER,
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::BACKGROUND:
				this->pixmapBackground = pixmap;
				break;
			case PixmapElem::BORDER:
				this->pixmapBorder = pixmap;
				break;
		}
	}
	
	std::string text = "";
	std::string selectedText = "";
	Signal textChanged {};
	Signal hover {};
	Signal drag {};

private:
	Pixmap pixmapBackground, pixmapBorder;
};

/// 
struct TextArea : public GUIElement
{
	WINTERGUI_API inline void render() override
	{
		
	}
	
	WINTERGUI_API inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	WINTERGUI_API inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	WINTERGUI_API inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER,
	};
	
	WINTERGUI_API inline void customTex(PixmapElem elem, Pixmap const &pixmap)
	{
		switch(elem)
		{
			case PixmapElem::BACKGROUND:
				this->pixmapBackground = pixmap;
				break;
			case PixmapElem::BORDER:
				this->pixmapBorder = pixmap;
				break;
		}
	}
	
	std::string text = "";
	std::string selectedText = "";
	Signal textChanged {};
	Signal hover {};
	Signal drag {};

private:
	Pixmap pixmapBackground, pixmapBorder;
};
