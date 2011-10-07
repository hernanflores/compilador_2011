/****************************************************************************

		       	C O M P I L A D O R E S
						   

Modelo para la implementacion de tipo set en 'C'.

En este fuente, se ve en primer termino, la definicion de las constantes 
que representan los tokens devueltos por el scanner(); luego las funciones 
que implementan operaciones sobre conjuntos de acuerdo a esas definiciones

****************************************************************************/


# include <stdio.h>
# include "codigos.h"
# include "set.h"


/****************************************************************************
						une()
****************************************************************************/
set une(set c1, set c2)
{
	return c1|c2;
}
/****************************************************************************
						in()
****************************************************************************/
int in(long e,set A) { /* x pertenece a A ? */
  long r;

       r= (e & A);    
       if (e == r) return 1;
       else  return 0;

}

/****************************************************************************
						cons()
****************************************************************************/
set cons(set x)
{
	set nuevo_set;

	nuevo_set = x;
	return nuevo_set;
}
/***********************************************************************
                            imprime set
***********************************************************************/
void imp_set(set v){
  printf("%X\n",v);
};


/*
int main () {
  int result;

  long l1, l2, e;
  set s1;
  set s2;

  l2= 0x40000030;
  l1= 0x00000001;

  s1=cons(l1, l2);

  e=0x40000002;
  result= in(e, s1);

  imp_set(s1);

  printf("esta %X en el set ? %d\n",e,result);
}

*/















