%{

/*
 * soelim.l: eliminate .so includes within *roff source
 *  
 * Copyright (C), 1994, 1995, Graeme W. Wilford. (Wilf.)
 *
 * You may distribute under the terms of the GNU General Public
 * License as specified in the file COPYING that comes with this
 * distribution.
 *
 * Added functionality over gsoelim to allow for compressed .so includes.
 *
 * Wed Oct 12 18:46:11 BST 1994  Wilf. (G.Wilford@ee.surrey.ac.uk) 
 *
 * Tue, 14 Oct 1997 Fabrizio Polacco <fpolacco@debian.org>
 * - added changes that were done to .c instead of -l source
 * - added new start condition to avoid execution of .so requests
 *   inside a macro definition.
 */

#define MAX_SO_DEPTH 	10		/* max .so recursion depth */
#undef ACCEPT_QUOTES			/* accept quoted roff requests */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#if defined(STDC_HEADERS)
#  include <string.h>
#  include <stdlib.h>
#elif defined(HAVE_STRING_H)
#  include <string.h>
#elif defined(HAVE_STRINGS_H)
#  include <strings.h>
#else /* no string(s) header */
extern char *strchr(), *strcat();
extern int strncmp();
#endif /* STDC_HEADERS */

#if defined(HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifndef STDC_HEADERS
extern int errno;
#endif

#define STATIC_VER	/* zsoelim has a static ver() */

#define NAME	so_name[so_stack_ptr]
#define LINE	so_line[so_stack_ptr]

#include "manconfig.h"
#include "lib/error.h"

#ifdef COMP_SRC
#include "comp_src.h"
static char* so_delete[MAX_SO_DEPTH];
#endif /* COMP_SRC */

static int open_file(char *filename);

#ifdef ACCEPT_QUOTES
#  define ZAP_QUOTES	zap_quotes()
static void zap_quotes(void);
#else
#  define ZAP_QUOTES
#endif

char *program_name;

static YY_BUFFER_STATE so_stack[MAX_SO_DEPTH];
static char *so_name[MAX_SO_DEPTH];
static char temp_name[1024];
static int so_line[MAX_SO_DEPTH];
static int so_stack_ptr;
static int no_newline;
static int status = OK;

extern int optind;
%}

%x so
%x de
%x end_request
%x lfnumber
%x lfname

W	[ \t]

%%

^\.de{W}*.+	{	
			no_newline = 1;
			ECHO;
			BEGIN(de);	/* Now we're inside of a macro definition: ends with a comment */
		}

^\.so{W}*	{	
			no_newline = 1;
			BEGIN(so);	/* Now we're in the .so environment */
		}

^\.lf{W}*	{
			no_newline = 1;
			ECHO;		/* Now we're in the .lf environment */
			BEGIN(lfnumber);
		}

^[^\.\n].*		|	/* fallback */
^\.[^sl].*		|
^\.l[^f].*		|
^\.s[^o].*		|
^\.s			|
^\.l			|
.			{
				no_newline = 1;
				ECHO;
			}

\n		{
			no_newline = 0;
			putchar('\n');
			LINE++;
		}
		

<so>\"?[^ \t\n\"]+\"?	{ 	/* file names including whitespace ?  */
			if (so_stack_ptr == MAX_SO_DEPTH - 1) 
				error (FATAL, 0, 
				       "%s:%d: .so requests nested too deeply or are recursive",
				       NAME, LINE);

			ZAP_QUOTES;
			so_stack[so_stack_ptr++] = YY_CURRENT_BUFFER;
			LINE = 1;

			no_newline = 0;

			sprintf( temp_name, "%s.gz", yytext);
			if (open_file(xstrdup(yytext)) && open_file(temp_name)) {
#ifndef __alpha
				error(OK, 0, 
				      "%s:%d: warning: failed .so request", NAME, LINE);
				printf(".so %s\n", yytext);
#endif
				BEGIN(end_request);
			} else {
				printf(".lf 1 %s\n", yytext);
				yy_switch_to_buffer( yy_create_buffer(yyin, 
							      YY_BUF_SIZE) );
				BEGIN(INITIAL);
			}

		}

<end_request>{W}*\n	{
			no_newline = 0;
			BEGIN(INITIAL);
		}
		
<so>\n		{
			no_newline = 0;
			error(OK, 0,
			      "%s:%d: warning: newline in .so request, ignoring",
			      NAME, LINE);
			putchar('\n');
			LINE++;
			BEGIN(INITIAL);
		}

<de>^\.\..*	{
			no_newline = 1;
			ECHO;
			BEGIN(INITIAL);
		}

<de>.*		{
			no_newline = 1;
			ECHO;
		}

<de>\n		{
			no_newline = 0;
			putchar('\n');
			LINE++;
		}


<lfnumber>\"?[0-9]+\"?	{
			no_newline = 1;
			ECHO;
			ZAP_QUOTES;
			LINE = atoi(yytext);
			BEGIN(lfname);
		}

<lfname>\"?[^ \t\n\"]+\"?	{	/* file names including whitespace ?? */
			no_newline = 1;
			ECHO;
			putchar('\n');
			ZAP_QUOTES;
			NAME = xstrdup(yytext);
			BEGIN(end_request); 
		}

<lfname>{W}+	{
			no_newline = 1;
			ECHO;
		}

<lfnumber,lfname>.	{
			no_newline = 1;
			error(OK, 0,
			      "%s:%d: warning: malformed .lf request, ignoring",
			      NAME, LINE);
			putchar(*yytext);
			BEGIN(INITIAL);
		}
		
<lfnumber,lfname>\n	{
			no_newline = 0;
			error(OK, 0,
			      "%s:%d: warning: newline in .lf request, ignoring",
			      NAME, LINE);
			putchar('\n');
			LINE++;
			BEGIN(INITIAL);
			}

<<EOF>>	{
		fclose(yyin);

#ifdef COMP_SRC
		if (so_delete[so_stack_ptr])
			remove(so_delete[so_stack_ptr]);
#endif
		if (no_newline)
			putchar('\n');

		if ( --so_stack_ptr < 0 ) {
			yyterminate();
		} else {
			yy_delete_buffer( YY_CURRENT_BUFFER );
			yy_switch_to_buffer( so_stack[so_stack_ptr] );
			printf(".lf %d %s\n", LINE += 1, NAME);
		}
		no_newline = 0;
		BEGIN(end_request);
	}
%%

#ifdef ACCEPT_QUOTES
/* remove leading and trailing quotes in requests */
static void zap_quotes(void)
{
	if (*yytext == '"') {
		if (yytext[yyleng - 1] == '"') {
			yytext[yyleng - 1] = '\0';
			yytext++;
		} else
			error(OK, 0, "%s:%d: unterminated quote in roff request",
			      NAME, LINE);
	}
}
#endif

/* print the usage message, then exit */
static void usage (int status)
{
	printf("usage: %s [ -VCh ] [ file ... ]\n", program_name);
	exit (status);
}

/* print the version, then exit */
static __inline__ void ver(void)
{
	printf("%s, version %s, %s (G.Wilford@ee.surrey.ac.uk)\n",
	       program_name, VERSION, DATE);
	exit(OK);
}

/* initialise the stack and call the parser */
static void parse_file(void)
{
	so_stack_ptr = 0;
	printf(".lf 1 %s\n", NAME);
	LINE = 1;
	yylex();
}

int main(int argc, char *argv[])
{
	int c;

	program_name = basename(argv[0]);

	while ( (c = getopt(argc, argv, "hVC")) != EOF ) {
		switch (c) {
			case 'V':
				ver();
				break;
			case 'C': 
				break; /* compatibility with GNU soelim */
			case 'h':
				usage(OK);
				break;
			default:
				usage(FAIL);
				break;
		}
	}

	/* if we have any arguments, parse them in command line order, else
	   open stdin */
	if (optind == argc) {
		so_name[0] = "-";
		parse_file();
	} else {
		while (optind < argc) {
			if (open_file(argv[optind++]))
				continue;
			parse_file();
		}
	}

	return status;
}

/* This routine is used to open the specified file or uncompress a compressed
   version and open that instead */
static int open_file(char *filename)
{
	FILE *oldyyin;
	char *ext;
	char *compfile;
	int is_hpux=0;
	int is_compr=0;
	struct compression *comp;
	struct stat buf;

	oldyyin = yyin;
	
/* #ifdef __hpux */
	ext = strrchr(filename, '.');
	if (strstr(filename, ".Z/")) { /* The file is in an HPUX directory */
		compfile = filename;
		is_hpux = 1;
		is_compr = 0;
	} else if (ext) {
		ext++;
		for (comp = comp_list; comp->ext && !is_compr; comp++) {
			if (strcmp(comp->ext, ext) == 0) {
				if (stat(filename, &buf) == 0) {
					comp->file = --ext;
					is_compr = 1;
					compfile = filename;
					break;
				}
			}
		}
		is_hpux = 0;
	}
/* #endif __hpux */

	if( is_hpux || is_compr || !(yyin = fopen( filename, "r" ))) {

#ifdef COMP_SRC
		size_t len;

		if (is_hpux) {
			struct compression hpux_comp = {GUNZIP "  -S \"\"", 
							"", NULL};
			comp = &hpux_comp;
		} else if ( ! is_compr) {
			compfile = strappend(NULL, filename, ".", NULL);
			len = strlen(compfile);
  		
			for (comp = comp_list; comp->ext; comp++) {
				compfile = strappend(compfile, comp->ext, NULL);
				if (stat(compfile, &buf) == 0)
					break;
				*(compfile + len) = '\0';
			}
		}

		if (comp->ext) {
			char *command;
			int exit_status;
			int file_fd;

			file_fd = create_tempfile ("zsman", &filename);
			if (file_fd < 0)
				error (FATAL, errno, "can't create a temporary filename");
			command = strappend(NULL, comp->prog, " ", compfile, " > ", filename, NULL);
			exit_status = system(command);
			close( file_fd);

			if (exit_status != 0) {
				error (OK, 0, "%s exited with status %d",
				       command, exit_status);
				free(command);
				yyin = oldyyin;
				return 1;
			}

			free(command);

			if( !(yyin = fopen( filename, "r" )) )  {
				error (OK, errno, "open decompressed file: %s",
				       filename);
				yyin = oldyyin;
				return 1;
			}
			
			so_delete[so_stack_ptr] = filename;
			NAME = compfile;
			return 0; 
		}
#endif /* COMP_SRC */
		if (so_stack_ptr > 0) {
			so_stack_ptr--;
#ifndef __alpha
			error (OK, errno, "%s:%d: %s", NAME, LINE, filename);
#endif
		} else {
			error (OK, errno, "%s", filename);
			status = FAIL;
		}
		yyin = oldyyin;
		return 1;
	}
	NAME = filename;
	return 0;
}

/* for compatibility with systems not having libfl, can't be static */
int yywrap(void)
{
	return 1;
}
