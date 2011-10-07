/***********************************************************
           CODIGOS PARA UTILIZAR EN EL SCANNER
***********************************************************/
/*  39 TOKEN DE USO EN EL SCANNER */

#define NADA  0LL
#define UNO   1LL
//#define FLAG  (UNO<<30)

#define CVOID       UNO       /* 1 */
#define CCHAR      (UNO<<1)   /* 2 */
#define CINT       (UNO<<2)   /* 4 */
#define CFLOAT     (UNO<<3)   /* 8 */
#define CAMPER     (UNO<<4)   /* 16 */
#define CCOR_ABR   (UNO<<5)   /* 32 */
#define CCOR_CIE   (UNO<<6)   /* 64 */
#define CCOMA      (UNO<<7)   /* 128 */
#define CPYCOMA    (UNO<<8)   /* 256 */
#define CASIGNAC   (UNO<<9)   /* 512 */
#define CLLA_ABR   (UNO<<10)  /* 1024 */
#define CLLA_CIE   (UNO<<11)  /* 2048 */
#define CWHILE     (UNO<<12)  /* 4096 */
#define CPAR_ABR   (UNO<<13)  /* 8192 */
#define CPAR_CIE   (UNO<<14)  /* 16384 */
#define CIF        (UNO<<15)  /* 32768 */
#define CELSE      (UNO<<16)  /* 65536 */
#define CIN       (UNO<<17)   /* 131072 */
#define COUT      (UNO<<18)   /* 262144 */
#define CSHL      (UNO<<19)	/* c<< */  /* 524288 */
#define CSHR      (UNO<<20)     /* c>> */  /* 1048576 */

#define CDISTINTO  (UNO<<21)     /* */
#define CIGUAL     (UNO<<22)  /* */ 
#define CMENOR     (UNO<<23)  /* */ 
#define CMEIG      (UNO<<24)  /* */
#define CMAYOR     (UNO<<25)
#define CMAIG      (UNO<<26)
#define CMAS       (UNO<<27)
#define CMENOS     (UNO<<28)
#define COR        (UNO<<29)
#define CMULT      (UNO<<30)
#define CDIV       (UNO<<31)
#define CAND       (UNO<<32)
#define CIDENT     (UNO<<33)
#define CNEG       (UNO<<34)
#define CCONS_ENT  (UNO<<35)
#define CCONS_FLO  (UNO<<36)
#define CCONS_CAR  (UNO<<37)
#define CCONS_STR  (UNO<<38)
#define CRETURN    (UNO<<39)
  
#define CEOF       (UNO<<40)
#define SEGUIR     (UNO<<41)





/* Para calcular los conjutos FIRST */

#define UNIDAD_TRADUCCION            0
#define DECLARACIONES                1
#define ESPECIFICADOR_TIPO           2
#define ESPECIFICADOR_DECLARACION    3
#define DEFINICION_FUNCION           4
#define DECLARACION_VARIABLE         5
#define LISTA_DECLARACIONES_PARAM    6 
#define DECLARACION_PARAMETRO        7
#define DECLARADOR_INIT              8
#define LISTA_DECLARACIONES_INIT     9
#define CONSTANTE                    10
#define LISTA_INICIALIZADORES        11
#define LISTA_PROPOSICIONES          12
#define LISTA_DECLARACIONES          13
#define DECLARACION                  14
#define PROPOSICION                  15
#define PROPOSICION_EXPRESION        16
#define PROPOSICION_COMPUESTA        17
#define PROPOSICION_SELECCION        18
#define PROPOSICION_ITERACION        19
#define PROPOSICION_E_S              20
#define PROPOSICION_RETORNO          21
#define VARIABLE                     22
#define EXPRESION                    23
#define EXPRESION_SIMPLE             24
#define RELACION                     25
#define TERMINO                      26
#define FACTOR                       27
#define LLAMADA_FUNCION              28
#define LISTA_EXPRESIONES            29
