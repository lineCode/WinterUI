#pragma once

#include "signal.hh"
#include "definitions.hh"
#include "pixmap.hh"
#include "WinterGUI.hh"
#include "sharedAssets.hh"

#include <functional>
#include <cstdint>
#include <vector>
#include <iris/vec2.hh>
#include <iris/mat4.hh>
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

template <typename Key> struct Widget
{
	using Key_t = Key;
	
	virtual ~Widget() = default;
	
	//The pure virtual functions need to be called by the user of this library at appropriate times
	virtual void render() = 0;
	virtual void onResize(uint32_t newWidth, uint32_t newHeight) = 0;
	virtual void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) {}
	virtual void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) {}
	virtual void onMouseMove(IR::vec2<int32_t> const &newPos) {}
	virtual void onKeyDown(Key_t key) {}
	virtual void onKeyUp(Key_t key) {}
	virtual void onTextInput(std::string const &input) {}
	virtual void onFocus() {}
	
	inline void addLayout(SP<Layout> const &layout)
	{
		this->layout = layout;
	}
	
	inline void addElement(SP<Widget> const &element)
	{
		this->childElements.push_back(element);
	}
	
	IR::vec2<int32_t> pos, size;
	Observer connectionObserver;
	SP<SharedAssets> assets;
	uint64_t layer = 1;

private:
	inline IR::aabb2D<int32_t> getHitbox()
	{
		return IR::aabb2D<int32_t>(this->pos.x(), this->pos.x() + this->size.x(), this->pos.y() - this->size.y(), this->pos.y());
	}
	
	std::vector<SP<Widget>> childElements;
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
	inline static SP<Pane> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<Pane>(assets);
	}
	
	inline Pane(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render(IR::vec2<uint32_t> const &contextSize) override
	{
		IR::mat4x4<float> MVP = IR::mat4x4<float>::modelViewProjectionMatrix(IR::mat4x4<float>::modelMatrix({this->pos, this->layer}, {}, {this->size, 1}),
		                                                                     IR::mat4x4<float>::viewMatrix({}, {}),
		                                                                     IR::mat4x4<float>::orthoProjectionMatrix(0, contextSize.x(), contextSize.y(), 0, 1, 100));
		this->assets->shader->bind();
		this->assets->mesh->bind();
		this->pixmap->bind();
		this->assets->shader->sendMat4f("mvp", &MVP[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void customPixmap(SP<Pixmap> const &pixmap)
	{
		this->pixmap = pixmap;
	}

private:
	SP<Pixmap> pixmap;
};

/// 
template <typename BASE> struct Button : public BASE
{
	inline static SP<Button> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<Button>(assets);
	}
	
	inline Button(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		//render based on state
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		if(button == LEFT) this->pressing = false;
		this->stateChanged.fire(this->pressing);
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		if(button == LEFT) this->pressing = true;
		this->stateChanged.fire(this->pressing);
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
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, PRESSED
	};
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<bool> stateChanged = {};
	Signal<> onHover = {};
	Signal<bool, typename BASE::Key_t> keyPressed = {};

private:
	SP<Pixmap> pixmapNormal, pixmapHover, pixmapPressed;
};

/// 
template <typename BASE> struct Checkbox : public BASE
{
	inline static SP<Checkbox> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<Checkbox>(assets);
	}
	
	inline Checkbox(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		this->value = !this->value;
		this->stateChanged.fire(this->value);
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		//change frame?
	}
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		NORMAL, HOVER, CHECKED
	};
	
	inline void customPixmap(PixmapElem elem, Pixmap const &pixmap)
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
	Signal<bool, typename BASE::Key_t> keyPressed = {};
	
private:
	PixmapElem current;
	SP<Pixmap> pixmapNormal, pixmapHover, pixmapChecked;
};

/// A bank of radio buttons that are linked together, eg only one can be selected at a time
template <typename BASE> struct RadioButtonBank : public BASE
{
	inline static SP<RadioButtonBank> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<RadioButtonBank>(assets);
	}
	
	inline RadioButtonBank(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	struct RadioButton : public BASE
	{
		inline static SP<RadioButton> create(SP<SharedAssets> assets) //should I do this?
		{
			return MS<RadioButton>(assets);
		}
		
		inline RadioButton(SP<SharedAssets> assets)
		{
			this->assets = assets;
			//TODO create pixmaps
		}
		
		enum struct PixmapElem
		{
			NORMAL, SELECTED,
		};
		
		inline void render() override
		{
			
		}
		
		inline void customPixmap(PixmapElem elem, Pixmap const &pixmap)
		{
			switch(elem)
			{
				case PixmapElem::NORMAL:
					this->pixmapRBNormal = pixmap;
					break;
				case PixmapElem::SELECTED:
					this->pixmapRBSelected = pixmap;
					break;
			}
		}
	
	private:
		PixmapElem current;
		SP<Pixmap> pixmapRBNormal, pixmapRBSelected;
	};
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	uint32_t selected = 0;
	Signal<uint32_t> stateChanged = {};
	Signal<bool, typename BASE::Key_t> keyPressed = {};
};

/// An adjustable slider widget
template <typename BASE> struct Slider : public BASE
{
	inline static SP<Slider> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<Slider>(assets);
	}
	
	inline Slider(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		RAIL, HANDLE, ICON
	};
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<bool, typename BASE::Key_t> keyPressed = {};

private:
	SP<Pixmap> pixmapRail, pixmapHandle, pixmapIcon;
};

/// A dropdown selection widget
template <typename BASE> struct DropdownMenu : public BASE
{
	inline static SP<DropdownMenu> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<DropdownMenu>(assets);
	}
	
	inline DropdownMenu(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		if(!this->extended)
		{
			//if mouse is hitting child category items
		}
		else
		{
			//if mouse is hitting child menu items
		}
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		if(this->extended)
		{
			//check for hits with child menu items
		}
	}
	
	inline void onTextInput(std::string const &input) override
	{
		
	}
	
	enum struct PixmapElem
	{
		FRAME, FRAMEBORDER, EXTFRAME, EXTFRAMEBORDER
	};
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	bool extended = false;
	SP<Pixmap> pixmapFrame, pixmapFrameBorder, pixmapExtendedFrame, pixmapExtendedFrameBorder;
};

/// A menu bar with dropdown menus, eg File Edit Help etc
template <typename BASE> struct MenuBar : public BASE
{
	inline static SP<MenuBar> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<MenuBar>(assets);
	}
	
	inline MenuBar(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	enum struct PixmapElem
	{
		BACKGROUND, BORDER
	};
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	Signal<bool, typename BASE::Key_t> keyPressed = {};

private:
	SP<Pixmap> pixmapBackground, pixmapBorder;
};

/// A selectable item in a dropdown or menu bar
template <typename BASE> struct MenuItem : public BASE
{
	inline static SP<MenuItem> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<MenuItem>(assets);
	}
	
	inline MenuItem(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void onKeyDown(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(true, key);
	}
	
	inline void onKeyUp(typename BASE::Key_t key) override
	{
		this->keyPressed.fire(false, key);
	}
	
	std::string text = "";
	Signal<MouseButtons, bool> clicked {};
	Signal<> hover {};
	Signal<bool, typename BASE::Key_t> keyPressed = {};
};

/// A text label
template <typename BASE> struct Label : public BASE
{
	inline static SP<Label> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<Label>(assets);
	}
	
	inline Label(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseMove(IR::vec2<int32_t> const &newPos) override
	{
		
	}
	
	inline void customPixmap(SP<Pixmap> const &pixmap)
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
	inline static SP<TextLine> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<TextLine>(assets);
	}
	
	inline TextLine(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
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
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
	inline static SP<TextArea> create(SP<SharedAssets> assets) //should I do this?
	{
		return MS<TextArea>(assets);
	}
	
	inline TextArea(SP<SharedAssets> assets)
	{
		this->assets = assets;
		//TODO create pixmaps
	}
	
	inline void render() override
	{
		
	}
	
	inline void onResize(uint32_t newWidth, uint32_t newHeight) override
	{
		
	}
	
	inline void onMouseUp(MouseButtons button, IR::vec2<int32_t> const &pos) override
	{
		
	}
	
	inline void onMouseDown(MouseButtons button, IR::vec2<int32_t> const &pos) override
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
	
	inline void customPixmap(PixmapElem elem, SP<Pixmap> const &pixmap)
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
