#include "signal.hh"

int main()
{
	Signal signal;
	Signal<>::Connection connection = signal.connect([]() { printf("fire!\n"); });
	signal.fire();
	return 0;
}
