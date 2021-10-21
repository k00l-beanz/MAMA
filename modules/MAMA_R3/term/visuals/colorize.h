#ifndef COLORIZE_H
#define COLORIZE_H

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

void display_fg_color(enum Color);
void display_bg_color(enum Color);
void display_italicize();
void display_reset();

#endif
