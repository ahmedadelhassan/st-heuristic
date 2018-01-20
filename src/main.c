#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

/* command line arguments */
typedef struct
{
  int   seed;       /* -s option  */
  int   verbosity;  /* -v option  */
  char* filename;   /* input file */
} command_line_args;

static const char* opt_string = "s:vh?";

static const struct option long_opts[] = {
    { "seed",    required_argument, NULL, 's' },
    { "verbose", no_argument,       NULL, 'v' },
    { "help",    no_argument,       NULL, 'h' },
    { NULL,      no_argument,       NULL, 0 }
};

void usage() {
    printf("Usage: st-heuistic -s num filename\n");
}

void here_we_are(command_line_args args)
{
  printf("report. seed=%d, verbosity=%d, filename=%s\n", args.seed, args.verbosity, args.filename);
}

int main( int argc, char *argv[] )
{
  /* Initialize args before we get to work. */
  command_line_args args;
	args.seed      = -1;
  args.verbosity = 0;
  args.filename  = NULL;

  /* parse command line arguments */
  int opt = 0;
  int long_index = 0;
	while ((opt = getopt_long( argc, argv, opt_string, long_opts, &long_index)) != -1) {
		switch (opt) {
			case 's':
        if (sscanf(optarg, "%i", &args.seed) != 1) {
          fprintf(stderr, "error. could not parse \"%s\" as a positive integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        if (args.seed < 0) {
          fprintf(stderr, "error. could not parse \"%s\" as a positive integer\n", optarg);
          exit(EXIT_FAILURE);
        }
				break;

			case 'v':
				args.verbosity++;
				break;

			case 'h':	/* fall-through is intentional */
			case '?':
				usage();
				break;

			default:
				/* You won't actually get here. */
				break;
		}
	}

  /* parse filename */
  if (optind >= argc)
  {
    usage();
    exit(EXIT_FAILURE);
  }
  args.filename = strdup(argv[optind]);

  /* here we are */
  here_we_are(args);

	return(EXIT_SUCCESS);
}
