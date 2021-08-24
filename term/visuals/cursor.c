#include <lib/out.h>

void cursor_left(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\b", 1);
}

void cursor_right(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[C", 3);
}

void cursor_down(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[B", 3);
}

void cursor_up(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[A", 3);
}

void cursor_return() {
	print("\b\r", 2);
}
