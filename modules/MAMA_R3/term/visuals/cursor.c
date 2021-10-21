#include <lib/out.h>

/**
 * Moves the visual cursor to the left a specified number of steps.
 *
 * @param steps The number of steps to move the cursor to the left.
 */
void cursor_left(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\b", 1);
}

/**
 * Moves the visual cursor to the right a specified number of steps.
 *
 * @param steps The number of steps to move the cursor to the right.
 */
void cursor_right(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[C", 3);
}

/**
 * Moves the visual cursor down a specified number of steps.
 *
 * @param steps The number of steps to move the cursor down.
 */
void cursor_down(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		println("", 0);
		//print("\e[B", 3);
}

/**
 * Moves the visual cursor up a specified number of steps.
 *
 * @param steps The number of steps to move the cursor up.
 */
void cursor_up(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[A", 3);
}

/**
 * Moves the visual cursor to the beginning of the line.
 */
void cursor_return() {
	print("\b\r", 2);
}
