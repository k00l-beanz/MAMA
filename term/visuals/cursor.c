#include <lib/out.h>

void cursor_left(int steps) {
	int i;
	for( i = 0; i < steps; i++)
		print("\b", 1);
}

void cursor_right(int steps) {
	int i;
	for( i = 0; i < steps; i++)
		print("\e[C", 1);
}
