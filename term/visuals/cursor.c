#include <lib/out.h>

/*
 * Procedure: cursor_left
 * Description: Moves the visual cursor to the left a specified number of steps
 */
void cursor_left(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\b", 1);
}

/*
 * Procedure: cursor_right
 * Description: Moves the visual cursor to the right a specified number of steps
 */
void cursor_right(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[C", 3);
}

/*
 * Procedure: cursor_down
 * Description: Moves the visual cursor down a specified number of steps
 */
void cursor_down(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		println("", 0);
		//print("\e[B", 3);
}

/*
 * Procedure: cursor_up
 * Description: Moves the visual cursor up a specified number of steps
 */
void cursor_up(int steps) {
	int i;
	for(i = 0; i < steps; i++)
		print("\e[A", 3);
}

/*
 * Procedure: cursor_return
 * Description: Moves the visual cursor to the beginning of the line
 */
void cursor_return() {
	print("\b\r", 2);
}
