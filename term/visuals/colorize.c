#include <lib/out.h>

#define START_SEQ "\e[" /// The start sequence of all ANSI escape codes.

enum Color {
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

void print_color_code(enum Color);

/**
 * Switches the text color in the terminal so that subsequent text written to the screen will be the specified color.
 *
 * @param color The color to switch to.
 */
void display_fg_color(enum Color color) {
	print(START_SEQ, 2);
	print("3", 1);
	print_color_code(color);
	print(";1m", 1);
}

/**
 * Switches the background color in the terminal so that subsequent text written to the screen will appear on a backdrop of the specified color.
 *
 * @param color The color to switch to.
 */
void display_bg_color(enum Color color) {
	print(START_SEQ, 2);
	print("4", 1);
	print_color_code(color);
	print("m", 1);
}

/**
 * Resets any formatting so that subsequent text written to the screen will use the default appearance.
 */
void display_reset() {
	print("\e[0m", 4);
}

/**
 * Description: Causes subsequent text written to the screen to be displayed in italics.
 */
void display_italicize() {
	print("\e[3m", 3);
}

/**
 * Description: Prints part of the escape sequence needed to switch the foreground or background color to the specified color. Used internally by display_fg_color and display_bg_color.
 *
 * @param color The color being switched to.
 */
void print_color_code(enum Color color) {
	switch(color) {
		case BLACK:
			print("0", 1);
			break;
		case RED:
			print("1", 1);
			break;
		case GREEN:
			print("2", 1);
			break;
		case YELLOW:
			print("3", 1);
			break;
		case BLUE:
			print("4", 1);
			break;
		case MAGENTA:
			print("5", 1);
			break;
		case CYAN:
			print("6", 1);
			break;
		case WHITE:
		default:
			print("7", 1);
			break;
	}
}
