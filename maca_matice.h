#ifndef __rf_maca_matice__
#define __rf_maca_matice__

#include <stdarg.h>
#include <stdio.h>

/** matrix content type */
typedef double field_t;

/** matrix structure */
typedef struct 
{
	int height;
	int width;
	field_t *field;
} maca_matice_t ;

/** alocate new I x J matrix */
int maca_init ( maca_matice_t **m, int, int );

/** sets all items to 0 */
int maca_null ( maca_matice_t *m );

/** add d to all items */
int maca_add_d( maca_matice_t *m, double d);

/** add matrix n to matrix m */
int maca_add_m( maca_matice_t *m, maca_matice_t *n);

/** multiple matrix m by matrix n and return it */
maca_matice_t * maca_mul_m( maca_matice_t *m, maca_matice_t *n);

/** multiple all items with d */
int maca_mul_d( maca_matice_t *m, double d);

/** print matrix */
int maca_print( maca_matice_t *m );

/** return matrix specific item, 0 if out of bounds */
field_t maca_item( maca_matice_t *m, int x, int y);

/** sets matrix items */
int maca_set( maca_matice_t *m, int x, int y, field_t f);

/** fill matrix by optional */
int maca_fill( maca_matice_t *m, field_t, ... );

/** transpose matrix */
int maca_transpose( maca_matice_t *m );

/** read matrix from standard input */
int maca_read_file(FILE * ifile, maca_matice_t **m );
int maca_read( maca_matice_t **m );


#endif
