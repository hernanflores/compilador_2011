
typedef long long set;
 
/* p1 define el long conteniendo los codigos SIN el 1 en el primer bits */
/* p2 define el long conteniendo los codigos CON el 1 en el primer bits */


set  une(set,set);              /* retorna la union de dos conjuntos */
int in(long long,set);               /* si un elemento pertenece al conjunto */
set  cons(long long);           /* construye un set */
void imp_set(set);

