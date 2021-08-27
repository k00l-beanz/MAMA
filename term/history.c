#include "commhand.h"
#include "visuals/cursor.c"
#include "visuals/syntax.h"
#include <lib/out.h>

static char cmd_hist[MAX_CMD_HIST_LEN][MAX_CMD_STRING_LEN + 1];
static int cmd_hist_most_recent_index = -1; // will be set properly on first call to hist_next_frame
static int cmd_hist_current_index = 0;
static int cmd_hist_oldest_index = 0;
static int initialized = 0;
static int last_frame_discarded = 0;

int circular_next_index(int);
int circular_prev_index(int);
void write_hist_to_buf(char *, int *, int *);

void hist_rewind(char *internal_buf, int *internal_index, int *internal_buf_len) {
        if(cmd_hist_current_index != cmd_hist_oldest_index) {
                cmd_hist_current_index = circular_prev_index(cmd_hist_current_index);
		write_hist_to_buf(internal_buf, internal_index, internal_buf_len);
        }
}

void hist_forward(char *internal_buf, int *internal_index, int *internal_buf_len) {
	if(cmd_hist_current_index != cmd_hist_most_recent_index) {
		cmd_hist_current_index = circular_next_index(cmd_hist_current_index);
		// as a special case, the user should be greeted with a blank prompt if returning to the bottom of the history queue
		// instead of writing this entry to the buffer
		// not doing so causes the last entry to remain in the buffer (we would just write the buffer to itself) and makes it
		// feel like there's a duplicate entry in the queue
		if(cmd_hist_current_index == cmd_hist_most_recent_index) {
			cursor_left(*internal_index);
			int i;
			for( i = 0; i < *internal_buf_len; i++)
				print(" ", 1);
			cursor_left(*internal_buf_len);

			internal_buf[0] = '\0';
			*internal_index = 0;
			*internal_buf_len = 0;
		} else {
			write_hist_to_buf(internal_buf, internal_index, internal_buf_len);
		}
	}
}

void hist_discard_last_frame() {
	last_frame_discarded = 1;
}

void write_hist_to_buf(char *buf, int *index, int *len) {
        int orig_buf_len = *len;
	int orig_index = *index;
        *len = 0;
        *index = 0;
        int i = 0;
        while(cmd_hist[cmd_hist_current_index][i]) {
                buf[i] = cmd_hist[cmd_hist_current_index][i];
                *len += 1;
                *index += 1;
        	i++;
        }
	// Null-terminating the buffer here shouldn't be necessary, since polling doesn't care about the null-terminator
	// and instead keeps track of the cursor and total length when doing everything.
	// HOWEVER, sys_req_read doesn't actually listen to the length parameter you pass in, even though it's supposed to.
	// For this reason, if we write a small history entry to a buffer containing a longer entry and don't null-terminate,
	// extraneous garbage chars will be printed to the screen during the following print, even though no problems should
	// occur internally. Null-termination here prevents this.
	buf[*len] = '\0';

	// adjust visually
        cursor_left(orig_index);
	
	// there's probably a more efficient way to do this, but for now this makes history play nice with syntax highlighting
	for(i = 0; i < *len; i++) {
		syntax_handle_char(buf[i], i);
		printc(buf[i]);
	}
	
	for(i = 0; i < orig_buf_len - *len; i++)
		print(" ", 1);
	cursor_left(orig_buf_len - *len);

}

char *hist_next_frame() {
	if(last_frame_discarded) {
		last_frame_discarded = 0;
		cmd_hist_current_index = cmd_hist_most_recent_index;
		return cmd_hist[cmd_hist_most_recent_index];
	}

	cmd_hist_most_recent_index = circular_next_index(cmd_hist_most_recent_index);
	cmd_hist_current_index = cmd_hist_most_recent_index;
	if(cmd_hist_oldest_index == cmd_hist_most_recent_index && initialized)
		cmd_hist_oldest_index = circular_next_index(cmd_hist_oldest_index);
	initialized = 1;
	return cmd_hist[cmd_hist_most_recent_index];
}

int circular_next_index(int i) {
	return (i + 1) % MAX_CMD_HIST_LEN;
}

int circular_prev_index(int i) {
	return i - 1 < 0 ? MAX_CMD_HIST_LEN - 1 : i - 1;
}


