#include <term/commhand.h>
#include <include/string.h>
#include <modules/mpx_supt.h>
#include <term/visuals/colorize.c>
// #include <lib/out.c>

int is_name_char(char);
void extract_cmd_name_from_cmd(char *, char *, int *);
cmd_func_t fetch_cmd_handler(char *);

int commhand() {
	static int max_cmd_str_len = MAX_CMD_STRING_LEN;	

	//char *cmd_str  = (char *)sys_alloc_mem(MAX_CMD_STRING_LEN * sizeof(char));
	char cmd_str[MAX_CMD_STRING_LEN + 1];
	char cmd_name[MAX_CMD_NAME_LEN + 1];
	//char *cmd_name = (char *)sys_alloc_mem((MAX_CMD_NAME_LEN + 1) * sizeof(char));
	int cmd_name_len = 0;

	int running = 1;
	
	while(running) {
		// print prompt and read input
		display_fg_color(GREEN);
		print("---> ", 2);
		display_reset();

		sys_req(READ, DEFAULT_DEVICE, cmd_str, &max_cmd_str_len);
		println("", 0);

		extract_cmd_name_from_cmd(cmd_str, cmd_name, &cmd_name_len);

		if(strcmp(cmd_name, "shutdown") == 0)
			running = 0;

		cmd_func_t handler = fetch_cmd_handler(cmd_name);
		if(handler != NULL) {
			(*handler)();
		} else {
			print("Unrecognized command: ", 22);
			println(cmd_name, cmd_name_len);
		}
	}

	// (*fetch_cmd_handler("help"))();

	//sys_free_mem(cmd_str);
	//sys_free_mem(cmd_name);
	return 0;
}

cmd_func_t fetch_cmd_handler(char *cmd_name) {	
	int i = 0;
	while(cmd_mappings[i].cmd_name != NULL) {
		if(strcmp(cmd_name, cmd_mappings[i].cmd_name) == 0)
			return (cmd_func_t)cmd_mappings[i].cmd_handler;
		i++;
	}
	return NULL;
}

void extract_cmd_name_from_cmd(char *cmd_str, char *cmd_name, int *cmd_name_len) {
	*cmd_name_len = 0;
        for(int i = 0; i < MAX_CMD_NAME_LEN && is_name_char(cmd_str[i]); i++)
                *cmd_name_len += 1;

	for(int i = 0; i < *cmd_name_len; i++)
		cmd_name[i] = cmd_str[i];
	cmd_name[*cmd_name_len] = '\0';
}

int is_name_char(char c) {
	return  (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '_');
}
