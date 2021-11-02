#include <term/visuals/cursor.h>
#include <term/visuals/clear.h>

int cmd_clear(char *args) {
	// this kinda works? you can still scroll 'back up' after the screen has been cleared but that might just be a quirk of qemu that we have no control over
	(void)args;
	cursor_return();
	cursor_up(999);
	display_clear();
	return 0;
}
