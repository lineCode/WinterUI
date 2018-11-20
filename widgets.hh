#pragma once

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

template <typename Key> struct GUIElement
{
	using Key_t = Key;
	
	virtual ~GUIElement() = default;
	
	//The pure virtual functions need to be called by the user of this library at appropriate times
	virtual void render() = 0;
	
	//Events
	virtual void onResize(uint32_t newWidth, uint32_t newHeight) = 0;
	virtual void onMouseUp(MouseButtons button) {}
	virtual void onMouseDown(MouseButtons button) {}
	virtual void onMouseMove(IR::vec2<int32_t> const &newPos) {}
	virtual void onKeyDown(Key_t key) {}
	virtual void onKeyUp(Key_t key) {}
	virtual void onTextInput(std::string const &input) {}
	virtual void onFocus() {}
	
	inline void addLayout(SP<Layout> const &layout)
	{
		this->layout = layout;
	}
	
	inline void addElement(SP<GUIElement> const &element)
	{
		this->childElements.push_back(element);
	}
	
	IR::vec2<int32_t> pos, size;
	Observer connectionObserver;

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
template <typename BASE> struct Pane : public BASE
{
	inline static SP<Pane> create()
	{
		
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void customTex(SP<Pixmap> const &pixmap)
	{
		this->pixmap = pixmap;
	}

private:
	SP<Pixmap> pixmap;
};

/// 
template <typename BASE> struct Button : public BASE
{
	inline void render() override
	{
		//render based on state
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		if(button == LEFT) this->pressing = false;
		this->stateChanged.fire(button, this->pressing);
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		if(button == LEFT) this->pressing = true;
		this->stateChanged.fire(button, this->pressing);
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
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
	
	void onKeyDown(typename BASE::Key_t key) override
	{
		
	}
	
	virtual void onKeyUp(typename BASE::Key_t key) override
	{
		
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, PRESSED
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<MouseButtons, bool> stateChanged = {};
	Signal<> onHover = {};

private:
	SP<Pixmap> pixmapNormal, pixmapHover, pixmapPressed;
};

/// 
template <typename BASE> struct Checkbox : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		this->value = !this->value;
		this->stateChanged.fire(this->value);
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		//change frame?
	}
	
	void onKeyDown(typename BASE::Key_t key) override
	{
		//this->keyPressed.fire(true, key);
	}
	
	virtual void onKeyUp(typename BASE::Key_t key) override
	{
		//this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, CHECKED
	};
	
	inline void customTex(PixmapElem elem, Pixmap const &pixmap)
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
	Signal<bool> stateChanged = {};
	Signal<> keyPressed = {};
	
private:
	SP<Pixmap> pixmapNormal, pixmapHover, pixmapChecked;
};

/// 
template <typename BASE> struct RadioButtonBank : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	template <typename T> void onKeyDown(T key)
	{
		
	}
	
	template <typename T> void onKeyUp(T key)
	{
		
	}
	
	enum struct PixmapElem
	{
		NORMAL, SELECTED
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<uint32_t> stateChanged = {};

private:
	SP<Pixmap> pixmapNormal, pixmapSelected;
};

/// 
template <typename BASE> struct Slider : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	template <typename T> void onKeyDown(T key)
	{
		
	}
	
	template <typename T> void onKeyUp(T key)
	{
		
	}
	
	enum struct PixmapElem
	{
		RAIL, HANDLE, ICON
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<float> stateChanged = {};

private:
	SP<Pixmap> pixmapRail, pixmapHandle, pixmapIcon;
};

/// 
template <typename BASE> struct DropdownMenu : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		FRAME, FRAMEBORDER, EXTFRAME, EXTFRAMEBORDER
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<> onOpen = {};
	Signal<uint32_t> stateChanged = {};

private:
	SP<Pixmap> pixmapFrame, pixmapFrameBorder, pixmapExtendedFrame, pixmapExtendedFrameBorder;
};

/// 
template <typename BASE> struct MenuBar : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	template <typename T> void onKeyDown(T key)
	{
		
	}
	
	template <typename T> void onKeyUp(T key)
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	
	Signal<> onOpen = {};
	Signal<> hover {};

private:
	SP<Pixmap> pixmapBackground, pixmapBorder;
};

/// 
template <typename BASE> struct MenuItem : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	template <typename T> void onKeyDown(T key)
	{
		
	}
	
	template <typename T> void onKeyUp(T key)
	{
		
	}
	
	std::string text = "";
	Signal<MouseButtons, bool> clicked {};
	Signal<> hover {};
};

/// 
template <typename BASE> struct Label : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void customTex(SP<Pixmap> const &pixmap)
	{
		this->pixmapBackground = pixmap;
	}
	
	std::string text = "";
	Signal<> hover {};

private:
	SP<Pixmap> pixmapBackground;
};

/// 
template <typename BASE> struct TextLine : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER,
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<std::string> textChanged {};
	Signal<> hover {};
	Signal<uint64_t> drag {};

private:
	SP<Pixmap> pixmapBackground, pixmapBorder;
};

/// 
template <typename BASE> struct TextArea : public BASE
{
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER,
	};
	
	inline void customTex(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<std::string> textChanged {};
	Signal<> hover {};
	Signal<uint64_t> drag {};

private:
	SP<Pixmap> pixmapBackground, pixmapBorder;
};
