#include "maca_matice.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int maca_init ( maca_matice_t **m, int h, int w )
{
	(*m) = (maca_matice_t *) malloc( sizeof(maca_matice_t) );
	if( !(*m) )
		return -1;
	(*m)->height = h;
  (*m)->width  = w;
  (*m)->field  = (field_t *)
		malloc(sizeof(field_t) * h * w);

	if( !( (*m)->field ) )
	{
		free(*m);
		return -2;
	}
	return maca_null( *m );
}

int maca_null ( maca_matice_t *m )
{
	int i, j;
	if( !m )
		return -3;
	for( j=0; j < m->height; j++ )
	for( i=0; i < m->width; i++ )
		m->field[i+j*m->width] = 0;

	return 0;
}

int maca_add_d( maca_matice_t *m, double d)
{
	int i,j;
	if( !m ) return -3;
	for( j=0; j < m->height; j++ )
	for( i=0; i < m->width; i++ )
		m->field[i+j*m->width] += d;
	return 0;
}

int maca_add_m( maca_matice_t *m, maca_matice_t *n)
{
	/*
	int maxh, maxw;
	maxh = max( m->height, n->height );
	maxw = max( m->width, n->width );
	*/
	int i,j;

	for( j=0; j < m->height; j++ )
	for( i=0; i < m->width; i++ )
		m->field[i + j*m->width] +=
			maca_item(n, i, j);
		//	n->field[i+j*n->height];

	return 0;
}

maca_matice_t * maca_mul_m( maca_matice_t *m, maca_matice_t *n)
{
	maca_matice_t *out;
	int i,j,k, ret;
/*
	m = / 1, 2 \  n = / 10, 20 \
      \ 3, 4 /      \ 30, 40 /

	m * n = / 1*10+2*30, 1*20+2*40 \
          \ 3*10+4*30, 3*20+4*40 /
*/
	
	// m musi mit tolik sloupcu, kolik ma n radku
	if( m->width != n->height )
		return 0;

	// radky, sloupce
	if( ret = maca_init( &out, m->height, n->width) )
		return 0;
	
  // nasob 1 radek m s 1 sloupcem n
	for( i=0; i < m->height; i++ ) // radky v m
	for( j=0; j < n->width; j++ ) // sloupce v n
  for( k=0; k < m->width; k++ ) // sloupce v m nebo radky v n
		out->field[ j + i*n->width ] +=
			maca_item(m,k,i) * maca_item(n,j,k);

	return out;
}

int maca_mul_d( maca_matice_t *m, double d)
{
	int i,j;
	for( j=0; j < m->height; j++ )
	for( i=0; i < m->width; i++ )
		m->field[i+j*m->width] *= d;
	return 0;
}

int maca_print( maca_matice_t *m )
{
	int i,j; // i - column, j - row
	printf("%d %d\n", m->height, m->width);
	for( j = 0; j < m->height; j++ ) {
		for(i = 0; i < m->width; i++)
			printf("%10.3f, ", maca_item(m,i,j));
		puts("");
	}
	return 0;
}

/**
 * x - column
 * y - row
 */
field_t maca_item( maca_matice_t *m, int x, int y)
{
	if( x >= m->width || y >= m->height )
		return 0;
	return m->field[x + y * m->width];
}


int maca_set( maca_matice_t *m, int x, int y, field_t f)
{
	if( x >= m->width || y >= m->height )
		return 1;

	m->field[x + y * m->width] = f;
	return 0;
}

int maca_fill( maca_matice_t *m, field_t field_args, ... )
{
	va_list ap;
	int loop;

	va_start(ap, field_args);
	m->field[0] = field_args;
	for( loop=1; loop < (m->height * m->width); loop++ )
	{	
		m->field[loop] = va_arg(ap, field_t);
		// printf("%f, ", m->field[loop]);
	}

	va_end(ap);
	return 0;
}

int maca_transpose( maca_matice_t *m )
{
	int w, h, i, j, wide;
	int *links; // transition matrix
	int cur = 0; // links cursor
	int next; // next field to
	field_t tmp;
	int tmpi;
	w = m->width;
	h = m->height;
	// wide = max(h,w);
	links = (int *) malloc( h*w*sizeof(int) );

	for( j=0; j < h; j++ )
		for( i=0; i < w; i++ )
			links[j+i*h] = i+j*w;

	/* for( j=0; j < h; j++ ) // print links like matrix
	{
		for( i=0; i < w; i++ ) {
			printf("%4d", links[i+j*w] + 1);
		}
		puts("");
	} */

	while( cur < h*w ) {
		// find first non zero item
		while( links[cur] == 0 )
		{
			// printf("skipping %d\n", cur);
			cur++;
		}

		if( cur >= h*w )
			break;

		// do cycle in links
		// printf("backing up %d position\n", cur+1);
		tmp = m->field[cur];
		tmpi = cur;
		while( links[cur] != 0 ) {
			next = links[cur];
			links[cur] = 0;
			m->field[cur] = m->field[next];
			// printf("%d <- %d\n", cur+1, next+1);
			if( next == tmpi )
				tmpi = cur;
			cur = next;
		}
		// printf("recoverying %d: %f\n", tmpi+1, tmp);
		m->field[tmpi] = tmp;
	}

	m->width  = h;
	m->height = w;

	return 0;
}


int maca_read( maca_matice_t **m )
{
	return maca_read_file( stdin, m);
}

int maca_read_file(FILE * ifile, maca_matice_t **m )
{
	int h, w, count;
	// field_t f;
	float f; // scanf cannot read double
	if( ifile == stdin )
		puts("Enter height and width: ");
	fscanf(ifile, "%d", &h);
	fscanf(ifile, "%d", &w);

	if( 0 == h*w )
		return 1;

	h = abs(h); w = abs(w);
	maca_init( m, h, w );
	maca_null( *m );

	if( ifile == stdin )
		puts("Enter items by rows: ");
	count = 0;
	while( count < h*w )
	{
		fscanf(ifile, "%f", &f);
		(*m)->field[count] = f;
		count++;
	}

	return 0;
}

