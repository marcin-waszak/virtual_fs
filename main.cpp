#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

int cmdparser(int argc, char **argv);
int optparser(int argc, char **argv, option* long_options, size_t* o_size,
	char* o_filesystem, char* o_localfile, char* o_virtualfile);
int print_manual();
int print_cmderr(char* cmd);

int main(int argc, char **argv) {

	cmdparser(argc, argv);
	return 0;
}

int cmdparser(int argc, char **argv) {

	option create_options[] = {
		{"size",		required_argument,	nullptr, 's'},
		{"filesystem",	required_argument,	nullptr, 'f'}
	};

	option import_options[] = {
		{"filesystem",	required_argument,	nullptr, 'f'},
		{"source",		required_argument,	nullptr, 'l'},
		{"destination",	required_argument,	nullptr, 'v'}
	};

	option export_options[] = {
		{"filesystem",	required_argument,	nullptr, 'f'},
		{"source",		required_argument,	nullptr, 'v'},
		{"destination",	required_argument,	nullptr, 'l'}
	};

	option details_options[] = {
		{"filesystem",	required_argument,	nullptr, 'f'}
	};

	option remove_options[] = {
		{"filesystem",	required_argument,	nullptr, 'f'},
		{"name",		required_argument,	nullptr, 'v'}
	};

	option delete_options[] = {
		{"filesystem",	required_argument,	nullptr, 'f'}
	};

	char*	s_command;

	size_t	o_size = 0;
	char	o_filesystem[128] = "";
	char	o_localfile[128] = "";
	char	o_virtualfile[128] = "";

	if(argc < 2) {
		print_cmderr(nullptr);
		print_manual();
		return 0;
	}
	
	s_command = argv[1];

	// Parse command
	
	if(strcmp(s_command, "create") == 0) {
		optparser(argc, argv, create_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Size: %zu\n", o_size);
		printf("Filesystem path: %s\n", o_filesystem);
	}
	else if(strcmp(s_command, "import") == 0) {
		optparser(argc, argv, import_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Filesystem path: %s\n", o_filesystem);
		printf("Local file path: %s\n", o_localfile);
		printf("Virtual file path: %s\n", o_virtualfile);
	}
	else if(strcmp(s_command, "export") == 0) {
		optparser(argc, argv, export_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Filesystem path: %s\n", o_filesystem);
		printf("Local file path: %s\n", o_localfile);
		printf("Virtual file path: %s\n", o_virtualfile);
	}
	else if(strcmp(s_command, "details") == 0) {
		optparser(argc, argv, details_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Filesystem path: %s\n", o_filesystem);
	}
	else if(strcmp(s_command, "remove") == 0) {
		optparser(argc, argv, remove_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Filesystem path: %s\n", o_filesystem);
		printf("Virtual file path: %s\n", o_virtualfile);
	}
	else if(strcmp(s_command, "delete") == 0) {
		optparser(argc, argv, delete_options, &o_size, o_filesystem,
			o_localfile, o_virtualfile);

		printf("Filesystem path: %s\n", o_filesystem);
	}
	else if(strcmp(s_command, "help") == 0 || strcmp(s_command, "--help") == 0) {
		print_manual();
	}
	else {
		print_cmderr(s_command);
		print_manual();
	}

	return 0;
}

int optparser(int argc, char **argv, option* long_options, size_t* o_size,
	char* o_filesystem, char* o_localfile, char* o_virtualfile) {

	bool f_break;
	int opt_result;
	int opt_index;

	f_break = false;
	optind++; // skip the command argument

	while(!f_break) {
		opt_result = getopt_long(argc, argv, "s:f:l:v:", long_options, &opt_index);

		switch(opt_result) {
		case 's':
			*o_size = atoi(optarg);
			break;

		case 'f':
			strcpy(o_filesystem, optarg);
			break;

		case 'l':
			strcpy(o_localfile, optarg);
			break;

		case 'v':
			strcpy(o_virtualfile, optarg);
			break;

		case ':':
		case '?':
			print_manual();
			break;

		case -1:
		default:
			f_break = true;
			break;
		}
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf("Non-option arguments: ");

		while(optind < argc)
			printf("%s ", argv[optind++]);

		putchar('\n');
	}

	return 0;
}

int print_cmderr(char* cmd) {
	if(cmd)
		printf("Incorrect command \"%s\". ", cmd);
	else
		printf("Incorrect command. ");
	return 0;
}

int print_manual() {
	printf("Syntax:\n fs [command] [options]\n");
	return 0;
}
