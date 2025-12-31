#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "maca_matice.h"

#define MACA_COUNT 10

// global matrices field
maca_matice_t *m[MACA_COUNT];
FILE *input_file;

typedef struct {
	int paramcount; 
	int pass_by_ref;
	int (*func)(maca_matice_t, int, int);
} maca_action_t;

void usage()
{ /*{{{*/
	puts(" Usage of maticalc");
	puts("h - help");
	puts("a 1 2 - add matrix 2 to 1");
	puts("m 1 2 3 - multiple matrix 1 by 2 and store it into 3");
	puts("r 1 - read matrix 1");
	puts("p 1 - print matrix 1");
	puts("* 1 x - multiple all items of matrix 1 by number x");
	puts("+ 1 x - add to all items of matrix 1 value x");
	puts("e 1 i j x - set matrix 1 item on i-th row and j-th column to x");
	puts("i 1 i j - print value of matrix 1 item on i-th row and j-th column");
	puts("t 1 - transponse matrix 1");
} /*}}}*/

/** Nacte param_count cisel a zavola prislusnou operaci s maticemi */
int perform_action( char * action, int param_count )
{
	int * indexes;
	int i;
	int quit = 0;
	indexes = (int *) malloc( sizeof(int) * param_count );
	if( !indexes ) {
		perror( "cannot alloc indexes ");
		exit(-1);
	}

	for( i = 0; i < param_count; i++ ) {
		fscanf(input_file, "%d", &indexes[i]);
		if( (indexes[i] < 0) || (indexes[i] >= MACA_COUNT) ) {
			quit = 1;
			fprintf(stderr, "%d is out of range.\n", indexes[i]);
		}
	}

	if( quit ) {
		return -1;
	}

	if( 0 == strcmp( action, "read" ) ) {
		maca_read_file(input_file, &m[indexes[0]] );
	}
	else if( 0 == strcmp( action, "print" ) ) {
		printf("M[%d]: ", indexes[0]);
		maca_print( m[indexes[0]] );
	}
	else if( 0 == strcmp( action, "add" ) ) {
		maca_add_m( m[indexes[0]], m[indexes[1]] );
	}
	else if( 0 == strcmp( action, "mul" ) ) {
		free( m[indexes[2]] );
		m[indexes[2]] = maca_mul_m( m[indexes[0]], m[indexes[1]] );
	}
	else if( 0 == strcmp( action, "item" ) ) {
		printf("M(%d)[%d,%d] = %f\n", 
			indexes[0], indexes[1], indexes[2],
			maca_item( m[indexes[0]], indexes[1], indexes[2] )
		);
	}
	else if( 0 == strcmp( action, "edit" ) ) {
		float f;
		printf("M(%d)[%d,%d] ? ", 
			indexes[0], indexes[1], indexes[2] );
		fscanf(input_file, "%f", &f);
		maca_set( m[indexes[0]], indexes[1], indexes[2], f );
	}
	else if( 0 == strcmp( action, "plus" ) ) {
		float f;
		fscanf(input_file, "%f", &f);
		printf("Adding %f to M(%d)", f, indexes[0]);
		maca_add_d( m[indexes[0]], f);
	}
	else if( 0 == strcmp( action, "times" ) ) {
		float f;
		fscanf(input_file, "%f", &f);
		maca_mul_d( m[indexes[0]], f);
	}
	else if( 0 == strcmp( action, "transpose" ) ) {
		maca_transpose( m[indexes[0]]);
	}
	else {
		printf("Unknown action '%s'!\n", action );
	}

	free(indexes);
	indexes = 0;
	return 0;

}

int main( int argc, char ** argv )
{/*{{{*/
	maca_action_t maca_actions[] = { (1,1,0) };
	int i, mi;
	char c; // command

	// read file from 1st param
	if(argc > 1) {
		input_file = fopen(argv[1],"r");
		if( !input_file ) {
			perror("fopen");
			exit(1);
		}
	} else {
		input_file = stdin;
	}

	// init
	for(i=0; i<MACA_COUNT; i++) {
		maca_init(&m[i], 3, 3);
	}

	if( input_file == stdin )
		printf( "Enter command, h for help > ");

	while( ( c = fgetc(input_file) ) != EOF )
	{
		int quit = 0;
		switch(c) {
			case '#': 
				do { // skip all chars until end of line (or file)
					c = fgetc(input_file);
				} while( c != EOF && c != 10 && c != 13);
				break;
			case 'h': case 'H': usage(); break;
			case 'p': case 'P': perform_action("print", 1); break;
			case 'i': case 'I': perform_action( "item", 3); break;
			case 'e': case 'E': perform_action( "edit", 3); break;
			case 'r': case 'R': perform_action( "read", 1); break;
			case 'a': case 'A': perform_action( "add", 2); break;
			case 'M': case 'm': perform_action( "mul", 3); break;
			case 'T': case 't': perform_action( "transpose", 1); break;
			case 'q': case 'Q' : puts("good bye"); quit = 1; break;
			case '+': perform_action("plus", 1);
			case '*': perform_action("times", 1);
			case 10 : if( input_file == stdin ) printf("> "); break;
			case 32 : break;
			default : fprintf(stderr, "Unknown command %c, code %d\n", c, c);
		}
		if( quit ) {
			break;
		}
	}

	// clean up
	for(i=0; i < MACA_COUNT; i++)
	{
		free(m[i]); m[i] = 0;
	}

	if( argc > 1 ) {
		fclose(input_file);
	}
	return 0;
}/*}}}*/
