#include <lib/out.h>

#define START_SEQ "\e["

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

void display_fg_color(enum Color color) {
	print(START_SEQ, 2);
	print("3", 1);
	print_color_code(color);
	print(";1m", 1);
}

void display_bg_color(enum Color color) {
	print(START_SEQ, 2);
	print("4", 1);
	print_color_code(color);
	print("m", 1);
}

void display_reset() {
	print("\e[0m", 4);
}

void display_italicize() {
	print("\e[3m", 3);
}

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
