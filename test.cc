#include "WinterGUI.hh"

void printCB()
{
	printf("Callback\n");
}

int main()
{
	using Widget_t = Widget<int>; //The user should specialize the base Widget class with their input libraries' scancode enum to create widgets with
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
	
	SP<Button_t> testButton = Button_t::create(nullptr); //Widgets should be contained in shared_ptrs, include definitions.hh to use SP
	
	testButton->stateChanged.connect(testButton->connectionObserver, [](bool down) { printf("Lambda\n"); }); //Connect this signal to a lambda
	testButton->onHover.connect(testButton->connectionObserver, &printCB); //Connect this signal to a function pointer
	
	testButton->stateChanged.fire(true); //Manually test the signals.  fire() is called internally by the widget on an event, don't manually fire any signal in a real application
	testButton->onHover.fire();
	return 0;
}
