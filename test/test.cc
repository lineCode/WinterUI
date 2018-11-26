#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#define WUI_GL33
#include "wui/WinterUI.hh"

bool exiting = false;
SDL_Window *window;
SDL_GLContext context;
uint32_t windowWidth = 800, windowHeight = 600;

void defaultOpenGLSettings()
{
	glDisable(GL_DEPTH_TEST); //for 2D games we dont use depth sorting because sprites use alpha transparency, instead we need to draw sprites from back to front
	glEnable(GL_SCISSOR_TEST); //allow GL to cut off anything that extends beyond the bounds of the context
	glEnable(GL_BLEND); //enable blending (transparency)
	glDisable(GL_CULL_FACE); //allow double sided rendering, geometry isnt invisible on one side
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //sets how pixels are blended together
	glViewport(0, 0, windowWidth, windowHeight); //sets the renderable area of the context
	glScissor(0, 0, windowWidth, windowHeight); //cut off anything beyond the bounds of the context
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clear the screen to black
}

void printCB()
{
	printf("Callback\n");
}

int main()
{
	using Widget_t = Widget<SDL_Scancode>; //The user should specialize the base Widget class with their input libraries' scancode enum to create widgets with
	using Pane_t = Pane<Widget_t>; //Defining this for every widget will end up being cleaner, especially when the application gets larger
	using Button_t = Button<Widget_t>;
	using Checkbox_t = Checkbox<Widget_t>;
	using RadioButtonBank_t = RadioButtonBank<Widget_t>;
	using RadioButton_t = RadioButtonBank_t::RadioButton;
	using Slider_t = Slider<Widget_t>;
	using DropdownMenu_t = DropdownMenu<Widget_t>;
	using MenuBar_t = MenuBar<Widget_t>;
	using MenuItem_t = MenuItem<Widget_t>;
	using Label_t = Label<Widget_t>;
	using TextLine_t = TextLine<Widget_t>;
	using TextArea_t = TextArea<Widget_t>;
	
	//SDL setup
	window = SDL_CreateWindow("UITest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	if(!window) throw std::runtime_error("Window Setup: Failed to create a window");
	SDL_GL_LoadLibrary(nullptr);
	
	context = SDL_GL_CreateContext(window);
	if(!context) throw std::runtime_error("Window context setup: Failed to create an OpenGL context");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	if(!GLAD_GL_VERSION_3_3) throw std::runtime_error("Your system does not support OpenGL 3.3");
	SDL_GL_SetSwapInterval(1);
	defaultOpenGLSettings();
	
	SP<Button_t> testButton = Button_t::create(nullptr); //Widgets should be contained in shared_ptrs, include definitions.hh to use SP
	
	testButton->stateChanged.connect(testButton->connectionObserver, [](bool down) { printf("Lambda\n"); }); //Connect this signal to a lambda
	testButton->onHover.connect(testButton->connectionObserver, &printCB); //Connect this signal to a function pointer
	
	testButton->stateChanged.fire(true); //Manually test the signals.  fire() is called internally by the widget on an event, don't manually fire any signal in a real application
	testButton->onHover.fire();
	
	SDL_Event event;
	do
	{
		while(SDL_PollEvent(&event) != 0)
		{
			switch(event.type)
			{
				case SDL_QUIT: exiting = true; break;
				case SDL_KEYUP: if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) exiting = true;
					break;
				case SDL_KEYDOWN: //update the 
					break;
				default: break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//render
		SDL_GL_SwapWindow(window);
	} while(!exiting);
	
	return 0;
}
