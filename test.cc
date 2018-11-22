#include "WinterGUI.hh"

void printCB()
{
	printf("Callback\n");
}

int main()
{
	using GUIElement_t = GUIElement<int>; //The user must specialize the base GUIElement class with their input libraries' keyboard keys enum to create widgets with
	//SP<Button<GUIElement_t>> testButton = MS<Button<GUIElement_t>>(); //all widgets need to be specialized with the using statement's specialized GUIElement base class
	SP<Button<GUIElement_t>> testButton = Button<GUIElement_t>::create(nullptr);
	
	testButton->stateChanged.connect(testButton->connectionObserver, [](bool down) { printf("Lambda\n"); }); //Connect this signal to a lambda
	testButton->onHover.connect(testButton->connectionObserver, &printCB); //Connect this signal to a function pointer
	
	testButton->stateChanged.fire(true); //Manually test the signals.  fire() is called internally by the widget on an event, don't manually fire in a real application
	testButton->onHover.fire();
	return 0;
}
