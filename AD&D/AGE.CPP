#include <stdlib.h>
#include <stdio.h>
#include <bios.h>
#include <conio.h>

#define UWORD unsigned int
#define UBYTE unsigned char
#define randomize srand( (UWORD)(biostime( 0, 0) / 24) )

//--------------------------PROTOTYPE----------------------------
UWORD lancer_des( UBYTE, UBYTE,
		     UBYTE* );

//--------------------------PROGRAMME----------------------------
void main()
{
unsigned long total, score, gtotal;
unsigned char temp;
double moyenne;

randomize;
clrscr();
level = 0;
base_age = 15;
var_age_num = 1;
var_age_des = 4;
base_max_age = 90;
max_age_num = 2;
max_age_des = 20;
age = 0;
for( long a=0; a<500; a++ )
{
  total = 0;
  for( int i=1; i<20; i++ )
  {
    score = lancer_des( 1, 20, &temp );
    total = total + score;
  }
//  printf( "total: %ld\n", total );
  gtotal = gtotal + total;
}
//printf( "gtotal: %ld\n", gtotal ); 
gtotal = gtotal * 2;
moyenne = (double)(gtotal) / 1000;
printf( "Moyenne: %lf", moyenne + 15 );
getch();
}

UWORD lancer_des( UBYTE nombre, UBYTE face,
		  UBYTE* plus_petit )
{
UBYTE i, des;
UWORD total;

total = 0;
*plus_petit = 0;
for( i = 0; i < nombre; i++ )
{
  des = random( face ) + 1;
  if( *plus_petit == 0 )
    (*plus_petit) = des;
  else
    if( *plus_petit > des )
      *plus_petit = des;
  total = total + des;
}
return( total );
}