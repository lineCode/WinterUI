#include "signal.hh"
#include "widgets.hh"

void printCB()
{
	printf("Callback\n");
}

int main()
{
	using GUIElement_t = GUIElement<int>; //You must specialize the base class GUIElement with your input libraries keyboard keys enum to create widgets
	SP<Button<GUIElement_t>> testButton = MS<Button<GUIElement_t>>(); //all widgets need to be specialized with the using statement
	testButton->stateChanged.connect(testButton->connectionObserver, [](MouseButtons button, bool down) { printf("Fire!\n"); }); //connect this signal to a lambda
	testButton->onHover.connect(testButton->connectionObserver, &printCB); //connect this signal to a function pointer
	testButton->stateChanged.fire(LEFT, true); //manually test the signal
	testButton->onHover.fire();
	return 0;
}
