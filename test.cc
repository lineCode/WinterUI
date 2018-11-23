#include "WinterGUI.hh"

void printCB()
{
	printf("Callback\n");
}

int main()
{
	using Widget_t = Widget<int>; //The user should specialize the base Widget class with their input libraries' scancode enum to create widgets with
	using Button_t = Button<Widget_t>; //Defining this for every widget will end up being much cleaner, especially when the application is large
	SP<Button_t> testButton = Button_t::create(nullptr); //Widgets should be contained in shared_ptrs, include definitions.hh for SP
	
	testButton->stateChanged.connect(testButton->connectionObserver, [](bool down) { printf("Lambda\n"); }); //Connect this signal to a lambda
	testButton->onHover.connect(testButton->connectionObserver, &printCB); //Connect this signal to a function pointer
	
	testButton->stateChanged.fire(true); //Manually test the signals.  fire() is called internally by the widget on an event, don't manually fire any signal in a real application
	testButton->onHover.fire();
	return 0;
}
