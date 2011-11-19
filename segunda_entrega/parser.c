/******************************************************************
 Este es el archivo correspondiente al PARSER
 *******************************************************************/

#define EXTERNA extern
#define EMPTY_STR ""


#define TIPO_CHAR_SIST_EJEC		0
#define TIPO_INT_SIST_EJEC		1
#define TIPO_FLOAT_SIST_EJEC	2

#define TAMANIO_CHAR	tc
#define TAMANIO_INT		te
#define TAMANIO_FLOAT	tf

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codigos.h"
#include "var_globales.h"
#include "ts.h"
#include "error.h"
#include "set.h"
#include "sistejec.c"

/*********** prototipos *************/
void unidad_traduccion(set folset);
void declaraciones(set folset);
int especificador_tipo(set folset);
void especificador_declaracion(set folset, int ath_tipo);
void definicion_funcion(set folset, int ath_tipo);
void declaracion_variable(set folset, int ath_tipo);
void lista_declaraciones_param(set folset);
void declaracion_parametro(set folset);
int declarador_init(set folset, int ath_tipo);
int lista_declaraciones_init(set folset, int ath_tipo);
int constante(set folset, int ath_tipo);
int lista_inicializadores(set folset, int ath_tipo);
void lista_proposiciones(set folset, int ath_tipo);
int lista_declaraciones(set folset);
int declaracion(set folset);
void proposicion(set folset, int ath_tipo);
void proposicion_expresion(set folset);
void proposicion_compuesta(set folset, int ath_tipo);
void proposicion_seleccion(set folset, int ath_tipo);
void proposicion_iteracion(set folset, int ath_tipo);
void proposicion_e_s(set folset);
void proposicion_retorno(set folset, int ath_tipo);
int variable(set folset, int ath_tipo);
int expresion(set folset);
void expresion_asignacion(set folset); // NO HAY METODO EN EL PARSER -- INLINE EN expresion()
void expresion_relacional(set folset); // NO HAY METODO EN EL PARSER -- INLINE EN expresion()
int expresion_simple(set folset);
void relacion(set folset); // NO HAY METODO EN EL PARSER -- INLINE EN expresion()
int termino(set folset, int ath_tipo);
int factor(set folset, int ath_tipo);
void llamada_funcion(set folset);
void lista_expresiones(set folset, char lexema[]);

/*
void unidad_traduccion(set folset);
void declaraciones(set folset);
long especificador_tipo(set folset);
void especificador_declaracion(set folset);
void definicion_funcion(set folset);
void declaracion_variable(set folset);
void lista_declaraciones_param(set folset);
void declaracion_parametro(set folset);
void declarador_init(set folset);
void lista_declacion_init(set folset);
void constante(set folset);
void lista_inicializadores(set folset);
void lista_proposiciones(set folset);
void lista_declaraciones(set folset);
void declaracion(set folset);
void proposicion(set folset);
void proposicion_expresion(set folset);
void proposicion_compuesta(set folset);
void proposicion_seleccion(set folset);
void proposicion_iteracion(set folset);
void proposicion_e_s(set folset);
void proposicion_retorno(set folset);
void variable(set folset);
void expresion(set folset);
void expresion_asignacion(set folset);
void expresion_relacional(set folset);
void expresion_simple(set folset);
void relacion(set folset);
void termino(set folset);
void factor(set folset);
void llamada_funcion(set folset);
void lista_expresiones(set folset);
 */



void inic_set();

/* Funciones agregadas */
/*Chequea si existe la funcion main, el tipo de
 retorno que sea void y que tenga parametros en su invocación*/
void existFuncionMain();

void insertarEnTSFuncionError(char lexema[]);

void insertarEnTSVariableError(char lexema[]);

void insertarEnTSArregloError(char lexema[]);

void insertarEnTSVariable();

void scanner();

void test(set c1, set c2, int ne);

/********** variables globales ************/

token *sbol;

extern FILE *yyin;


// tipos para el chequeo de tipos
int TIPO_INIT, TIPO_VOID, TIPO_CHAR, TIPO_INT, TIPO_FLOAT, TIPO_ERROR, TIPO_STRING;

// para el chequeo de los parametros enviados a una funcion
typedef enum {EXPRESION_INIT, EXPRESION_VARIABLE, EXPRESION_ARREGLO, EXPRESION_OTRO, EXPRESION_FIN} estadosDeExpresion;
estadosDeExpresion expresionActual;		// para saber si un parametro es una variable

/**
 variable para verificar retornon de función*/
int checkreturn;
int got_return = 0;

//almacena si el identificador corresponde a un arreglo
int es_arreglo = 0;
//almacena tam. variable arreglo
int tam_arreglo = 0;

int const_int = 0;
float const_float = 0;
char const_char;

set first[60];

int tipoDeRetornoDeclaracion;
int tipoParam;
/*

 */
int es_parametro = 0;
int tipo_id = NIL;


// para chequear si las constantes string no estan en lugares invalidos
int enProposicionSalida = 0;

// archivos de salida
FILE * salida; // archivo de salida codigo generado objeto
FILE * code; // archivo de salida codigo generado string para test
char nombreSalida[40]; // nombre del archivo de salida codigo generado objeto
char nombreCode[40]; // nombre del archivo de salida codigo generado string para test

// para localizar variables en la memoria de programa
int nivel = 0;
int desplazamiento = 0;

// para saber donde almacenar luego de invocar a variable()
int variableNivel = -1;
int variableDesplaz = -1;

//revisa si ya hice push de nivel
int pushie_func = 0;
tipo_inf_res **ptr_inf_res;
int *ptr_cant_params;

//chequeo por nombre de funcion main
char nbre_func[];

int esArregloDeclaracion;
int inicializacionDeclaracion = 0;
int inicializacionDeclaracionTipo = -1;

/**
 * 
 */

void inic_set() {

    first[UNIDAD_TRADUCCION] = cons(CVOID | CCHAR | CINT | CFLOAT | CEOF);
    first[DECLARACIONES] = cons(CVOID | CCHAR | CINT | CFLOAT);
    first[ESPECIFICADOR_TIPO] = cons(CVOID | CCHAR | CINT | CFLOAT);
    first[ESPECIFICADOR_DECLARACION] = cons(
            CCOR_ABR | CCOMA | CPYCOMA | CASIGNAC | CPAR_ABR);
    first[DEFINICION_FUNCION] = cons(CPAR_ABR);
    first[DECLARACION_VARIABLE] = cons(CCOR_ABR | CCOMA | CASIGNAC);
    first[LISTA_DECLARACIONES_PARAM] = first[ESPECIFICADOR_TIPO];
    first[DECLARACION_PARAMETRO] = first[ESPECIFICADOR_TIPO];
    first[DECLARADOR_INIT] = first[DECLARACION_VARIABLE];
    first[CONSTANTE] = cons(CCONS_ENT | CCONS_FLO | CCONS_CAR);
    first[LISTA_DECLARACIONES_INIT] = cons(NADA | CIDENT);
    first[LISTA_INICIALIZADORES] = first[CONSTANTE];
    first[LISTA_DECLARACIONES] = first[ESPECIFICADOR_TIPO];
    first[DECLARACION] = first[ESPECIFICADOR_TIPO];
    first[PROPOSICION_COMPUESTA] = cons(CLLA_ABR | NADA);
    first[PROPOSICION_SELECCION] = cons(CIF | NADA);
    first[PROPOSICION_ITERACION] = cons(CWHILE | NADA);
    first[PROPOSICION_E_S] = cons(CIN | COUT | NADA);
    first[PROPOSICION_RETORNO] = cons(NADA | CRETURN);
    first[VARIABLE] = CIDENT;
    first[RELACION] = cons(
            NADA | CDISTINTO | CIGUAL | CMENOR | CMEIG | CMAYOR | CMAIG);
    first[FACTOR] = une(cons(CPAR_ABR | CNEG | CIDENT | CCONS_STR),
            first[CONSTANTE]);
    first[LLAMADA_FUNCION] = cons(NADA | CIDENT);
    first[TERMINO] = first[FACTOR];
    first[EXPRESION_SIMPLE] = une(cons(NADA | CMAS | CMENOS), first[TERMINO]);
    first[EXPRESION] = first[EXPRESION_SIMPLE];
    first[PROPOSICION_EXPRESION] = une(cons(CPYCOMA | NADA), first[EXPRESION]);
    first[PROPOSICION] = (first[PROPOSICION_EXPRESION]
            | first[PROPOSICION_COMPUESTA] | first[PROPOSICION_SELECCION]
            | first[PROPOSICION_ITERACION] | first[PROPOSICION_E_S]
            | first[PROPOSICION_RETORNO]);
    first[LISTA_PROPOSICIONES] = first[PROPOSICION];
    first[LISTA_EXPRESIONES] = first[EXPRESION];

}

/*
 
 FUNCIONES DE GENERACION DE CODIGO
 
 */

void generarCast(int tipoBaseEjec, int tipoDestEjec) {
	if (P[lp-3] != CAST) {	// para evitar recastear un cast futuro
		if (tipoBaseEjec < tipoDestEjec) {
			P[lp++]= CAST;
			P[lp++]= tipoBaseEjec;
			P[lp++]= tipoDestEjec;
			fprintf(code,"CAST\t%d\t%d\n", tipoBaseEjec, tipoDestEjec);
		}
	}
}

void generarAlmacenar(int niv, int desp, int tipo) {
	P[lp++]= ALM;
	P[lp++]= niv;
	P[lp++]= desp;
	P[lp++]= tipo;
	fprintf(code,"ALM\t%d\t%d\t%d\n", niv, desp, tipo);
}

void generarPop(int tipo) {
	P[lp++]= POP;
	P[lp++]= tipo;
	fprintf(code,"POP\t%d\n", tipo);
}

void generarLeer(int tipo) {
	P[lp++]= LEER;
	P[lp++]= tipo;
	fprintf(code,"LEER\t%d\n", tipo);
}

void generarImprimir(int tipo) {
	P[lp++]= IMPR;
	P[lp++]= tipo;
	fprintf(code,"IMPR\t%d\n", tipo);
}

void generarImprimirString() {
	P[lp++]= IMPCS;
	fprintf(code,"IMPCS\n");
}

void generarInvertir(int tipo) {
	P[lp++]= INV;
	P[lp++]= tipo;
	fprintf(code,"INV\t%d\n", tipo);
}

void generarNegacion(int tipo) {
	P[lp++]= NEG;
	P[lp++]= tipo;
	fprintf(code,"NEG\t%d\n", tipo);
}

// retorna la direccion donde se debe ubicar el tipo al que se castea
int generarCastFuturo(int tipoBaseEjec) {
	int ret;
	P[lp++]= CAST;
	P[lp++]= tipoBaseEjec;
	ret = lp;
	P[lp++]= -1;	// TIPO FUTURO
	fprintf(code,"CAST\t%d\tFUTURO\n", tipoBaseEjec);
	return ret;
}

// verifica si a la izaquierda de una asignacion, hay una variable
int esVarIzqAsignacion() {
	char str[200];
	char aux[TAM_LEXEMA];
	char id[TAM_LEXEMA];
	int i,j=0,k;
	int esVar;
	
	strcpy(str,linea);
	strcpy(aux,"");
	strcpy(id,"");
	i = strlen(str) - 1;
	
	while (i>=0 && !isalnum(str[i]) && str[i]!=']') {
		i--;
	}
	if (i>=0 && str[i]==']') {
		while (i>=0 && str[i]!='[') {
			i--;
		}
		i--;
	}
	while (i>=0 && isalnum(str[i])) {
		aux[j] = str[i];
		i--;
		j++;
	}
	k = i;
	aux[j] = '\0';
	for (i=strlen(aux)-1,j=0; i>=0; i--,j++) {
		id[j] = aux[i];
	}
	id[j] = '\0';
	
	esVar = en_tabla(id) != NIL && (Clase_Ident(id) == CLASVAR || Clase_Ident(id) == CLASPAR);
	if (esVar && k>=0) {
		for (i=0; i<=k; k--) {
			if (str[k] == '(' || str[k] == ')' || str[k] == '=' || str[k] == ';' || str[k] == '{' || str[k] == '}' || 
					(str[k]=='<' && k-1>=0 && str[k-1]=='<')) {
				break;
			}
			else if (str[k]!=' ' && str[k]!='\t' && str[k]!='\n') {
				esVar = 0;
				break;
			}
		}
	}
	
	//printf(">>> STR IZQ: %s --- ES VAR: %d\n", id, esVar);
	return esVar;
}


int tipoSistEjec(int tipo) {
    if (tipo == TIPO_CHAR) {
        return TIPO_CHAR_SIST_EJEC;
    } else if (tipo == TIPO_INT) {
        return TIPO_INT_SIST_EJEC;
    } else if (tipo == TIPO_FLOAT) {
        return TIPO_FLOAT_SIST_EJEC;
    } else {
        // ERROR
        return TIPO_FLOAT_SIST_EJEC;
    }
}

// si los tipos a cohertir son char, int o float, retorna la cohersion (el tipo mas grande),
// caso contrario devuelve TIPO_ERROR.
// para ser usado dentro de expresiones (operaciones relacionales/aritmeticas/logicas).
int cohersion(int t1, int t2) {
	int ret;
	
	if (t1 == TIPO_INIT) {	// cuando la herencia de tipos fue inicializada, retorna el segundo tipo
		ret = t2;
	}
	else if ((t1 == TIPO_ERROR) || (t2 == TIPO_ERROR) || 
			(t1 == TIPO_VOID) || (t2 == TIPO_VOID) || 
			(t1 == TIPO_STRING) || (t2 == TIPO_STRING) || 
			(t2 == TIPO_INIT)) {	// tipos incorrectos para realizar la cohersion
		ret = TIPO_ERROR;
	}
	else {	// tipos correctos para realizar la cohersion
		if (t1 == TIPO_FLOAT) {
			ret = TIPO_FLOAT;
		}
		else if (t1 == TIPO_INT) {
			if (t2 == TIPO_FLOAT) {
				ret = TIPO_FLOAT;
			}
			else { 
				ret = TIPO_INT;
			}
		}
		else if (t1 == TIPO_CHAR) {
			ret = t2;
		}
		else {	// no deberia alcanzarse nunca
			ret = TIPO_ERROR;
		}
	}
	
	return ret;
}



// chequea que el tipo t1 pueda almacenar el tipo t2 (sea mas grande);
// para usar en asignaciones (donde la parte izquierda debe poder alojar a la parte derecha)
// y en proposicion_retorno() para verificar que el valor retornado sea del tipo correcto.
// Retorna el tipo de la parte izquierda (o el tipo de la funcion) en caso de exito, o
// TIPO_ERROR en caso de error de tipos.

int chequear_tipos(int t1, int t2, int ne) {
    int ret;

    if (t1 == TIPO_CHAR) {
        //VALIDAR
        if (t2 != TIPO_CHAR && (t2 != TIPO_INT)) {
            error_handler(ne);
            ret = TIPO_ERROR;
        } else {
            ret = TIPO_CHAR;
        }
    } else if (t1 == TIPO_INT) {
        if ((t2 != TIPO_CHAR) && (t2 != TIPO_INT)) {
            error_handler(ne);
            ret = TIPO_ERROR;
        } else {
            ret = TIPO_INT;
        }
    } else if (t1 == TIPO_FLOAT) {
        if ((t2 != TIPO_CHAR) && (t2 != TIPO_INT) && (t2 != TIPO_FLOAT)) {
            error_handler(ne);
            ret = TIPO_ERROR;
        } else {
            ret = TIPO_FLOAT;
        }
    } else {
        error_handler(ne);
        ret = TIPO_ERROR;
    }

    return ret;
}

void test(set c1, set c2, int ne) {
    if (!in(sbol->codigo, c1)) {
        error_handler(ne);
        c1 = une(c1, c2);
        while (!in(sbol->codigo, c1)) {
            scanner();
        }
    }

}

void scanner() {
    int i;

    for (; (i = yylex()) != NADA && sbol->codigo == SEGUIR;)
        ;
    if (i == NADA)
        sbol->codigo = CEOF;
    /* yylex retorna 0 si llego a fin de archivo */

    liberar = linea;
    linea = (char *) malloc(strlen(linea) + strlen(token1.lexema) + 3);
    strcpy(linea, liberar);
    strcat(linea, token1.lexema);
    free((void *) liberar);
    /* El alumno debera dar una solucion al problema que las constantes
     enteras y flotantes son entregadas como strings al parser */
}

void init_tipos() {
    TIPO_INIT = -1;
    TIPO_VOID = en_tabla("void");
    TIPO_CHAR = en_tabla("char");
    TIPO_INT = en_tabla("int");
    TIPO_FLOAT = en_tabla("float");
    TIPO_ERROR = en_tabla("error");
    TIPO_STRING = en_tabla("error") + 1;
    /*printf("\n%d=TIPO_INIT  %d=TIPO_VOID  %d=TIPO_CHAR  %d=TIPO_INT  %d=TIPO_FLOAT  %d=TIPO_ERROR  %d=TIPO_STRING\n\n", 
                    TIPO_INIT, TIPO_VOID, TIPO_CHAR, TIPO_INT, TIPO_FLOAT, TIPO_ERROR, TIPO_STRING);*/
}

void init_generacion(char nbreArchivo[]) {
    hayError = 0;

    strcpy(nombreSalida, nbreArchivo);
    nombreSalida[strlen(nombreSalida) - 1] = 'o';

    strcpy(nombreCode, nbreArchivo);
    nombreCode[strlen(nombreCode) - 1] = '\0';
    strcat(nombreCode, "code");

    //printf("\nnombreSalida %s --- nombreCode %s\n\n", nombreSalida, nombreCode);

    if ((code = fopen(nombreCode, "w")) == NULL) {
        error_handler(103);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    }
}

void generar_salida() {
    P[lp++] = PARAR;
    fprintf(code, "PARAR\n");

    if (!hayError) {
        // GENERAR ARCHIVO .o
        if ((salida = fopen(nombreSalida, "w")) == NULL) {
            error_handler(102);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        }

        fwrite(P, sizeof (float), TAM_PROG, salida);
        fwrite(C, sizeof (char), TAM_CTES, salida);
        fclose(salida);
    } else {
        // como hay errores no genero la salida;
        // si hay una version vieja, la borro porque esta desactualizada
        if ((salida = fopen(nombreSalida, "r")) != NULL) {
            fclose(salida);
            remove(nombreSalida);
        }
    }

    fclose(code);
    //remove(nombreCode);	// TODO descomentar para la entrega para que no les deje el archivo .code
}

int main(int argc, char *argv[]) {

    /* el alumno debera inicializar la variable yyin segun corresponda */

    linea = (char *) malloc(2);
    strcat(linea, "");

    nro_linea = 0;

    if (argc == 3 && strcmp(argv[1], "-c") == 0) { // ./ucc -c programa.c
        if (argv[2][strlen(argv[2]) - 2] != '.' || argv[2][strlen(argv[2]) - 1] != 'c') {
            error_handler(100);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        }
    
    if (strcmp(argv[1], "-c") == 0) {
        if ((yyin = fopen(argv[2], "r")) == NULL) {
            error_handler(7);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        }

        sbol = &token1; /* la variable token */

        
        init_generacion(argv[2]);
        inic_tablas();
        init_tipos();
        inic_set();
        
        scanner();
        unidad_traduccion(CEOF);

        if (sbol->codigo != CEOF)
            error_handler(41);
        /*Chequemos funcion main*/
        existFuncionMain();

        generar_salida();
        /*Chequemos que no haya errores, sino imprimos errores*/
        if (cant_errores_x_linea > 0) {
            print_aux();
        }
    }
    }
    else if (argc == 3 && strcmp(argv[1],"-e") == 0) {	// ./ucc -e programa.o
		if (argv[2][strlen(argv[2])-2] != '.' || argv[2][strlen(argv[2])-1] != 'o') {
			error_handler(101);
			error_handler(COD_IMP_ERRORES);
			exit(1);
		}
		if ((yyin = fopen(argv[2], "r" )) == NULL) {
			error_handler(7);
			error_handler(COD_IMP_ERRORES);
			exit(1);
		}
		
		// OK, INTERPRETAR ---------------------------------
		
		// CARGAR ARCHIVO EN ZONA PROGRAMA Y ZONA CONSTANTES STRING
		fread(P, sizeof(float), TAM_PROG, yyin);
		fread(C, sizeof(char),  TAM_CTES, yyin);
		
		interprete();
		
  }
	else {		// PARAMETROS INVALIDOS
		error_handler(6);
		error_handler(COD_IMP_ERRORES);
		exit(1);  
	}

}

/********* funciones del parser ***********/

void unidad_traduccion(set folset) {
    
    P[lp++]= INPP;
    fprintf(code,"INPP\n");
    
    test(first[DECLARACIONES] | folset, folset, 50);
    while (sbol->codigo == CVOID || sbol->codigo == CCHAR
            || sbol->codigo == CINT || sbol->codigo == CFLOAT)
        declaraciones(folset | first[DECLARACIONES]);
}

void declaraciones(set folset) {
    tipoDeRetornoDeclaracion = especificador_tipo(folset | CIDENT | first[ESPECIFICADOR_DECLARACION]);
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre, sbol->lexema);
        strcpy(nbre_func, sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }
    especificador_declaracion(folset, tipoDeRetornoDeclaracion);
}

/**
 * 
 * @return el codigo del tipo del identificador 
 */
int especificador_tipo(set folset) {
    test(first[ESPECIFICADOR_TIPO], folset, 51);
    checkreturn = 1;
    switch (sbol->codigo) {
        case CVOID:
            checkreturn = 0;
            inf_id->ptr_tipo = en_tabla("void");
            scanner();
            break;
        case CCHAR:
            inf_id->ptr_tipo = en_tabla("char");
            scanner();
            break;
        case CINT:
            inf_id->ptr_tipo = en_tabla("int");
            scanner();
            break;
        case CFLOAT:
            inf_id->ptr_tipo = en_tabla("float");
            scanner();
            break;
        default:
            inf_id->ptr_tipo = en_tabla("error");
            error_handler(17);
            break;
    }
    tipo_id = inf_id->ptr_tipo;
    test(folset, NADA, 52);
    return tipo_id;
}

void especificador_declaracion(set folset, int ath_tipo) {
    //	test(first[DEFINICION_FUNCION] | first[DECLARACION_VARIABLE], folset, 53);
    switch (sbol->codigo) {
        case CPAR_ABR:
            if (ath_tipo != TIPO_VOID && ath_tipo != TIPO_CHAR &&
                    ath_tipo != TIPO_INT && ath_tipo != TIPO_FLOAT) {
                error_handler(85);
            }
            definicion_funcion(folset, ath_tipo);
            break;
        case CASIGNAC:
        case CCOR_ABR:
        case CCOMA:
        case CPYCOMA:
            if (ath_tipo != TIPO_CHAR && ath_tipo != TIPO_INT && ath_tipo != TIPO_FLOAT) {
                error_handler(86);
            }
            declaracion_variable(folset, ath_tipo);
            break;
        default:
            error_handler(18);
            break;
    }

}

void definicion_funcion(set folset, int ath_tipo) {
    int check_return = checkreturn;

    if (sbol->codigo == CPAR_ABR) {
        inf_id->clase = CLASFUNC;
        scanner();
    } else
        error_handler(19);

    //Revisar esta inicializacion
    ptr_inf_res   = &(inf_id ->desc.part_var.sub.ptr_inf_res);
    ptr_cant_params = &(inf_id ->desc.part_var.sub.cant_par);
    *ptr_inf_res = NULL;
    (*ptr_cant_params) = 0;
    insertarTS();
    pushTB(); //nueevo bloque de la funcion
    pushie_func = 1;
    
    nivel++;

    P[lp++] = ENBL;
    P[lp++] = nivel;
    fprintf(code, "ENBL\t%d\n", nivel);

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT
            || sbol->codigo == CFLOAT || sbol->codigo == CIDENT) {
        lista_declaraciones_param(
                folset | CPAR_CIE | first[PROPOSICION_COMPUESTA]);
    }
    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20);

    proposicion_compuesta(folset, ath_tipo);

    if (strcmp("main", nbre_func) == 0) {
        check_return = 0;
    }
    if (check_return && !got_return) {
        error_handler(37);
        print_aux();
    }
    got_return = 0;
    checkreturn = 0;
}

void lista_declaraciones_param(set folset) {

    declaracion_parametro(folset | CCOMA | first[DECLARACION_PARAMETRO]);

    while (sbol->codigo == CCOMA
            || in(sbol->codigo, first[DECLARACION_PARAMETRO])) {
        if (sbol->codigo != CCOMA) {
            error_handler(75);
        } else {
            scanner();
        }

        declaracion_parametro(folset | CCOMA | first[DECLARACION_PARAMETRO]);
    }
}

void declaracion_parametro(set folset) {
    //	test(first[DECLARACION_PARAMETRO],
    //			folset | CAMPER | CIDENT | CLLA_ABR | CLLA_CIE, 55);
    tipoParam = especificador_tipo(folset | CAMPER | CIDENT | CLLA_ABR | CLLA_CIE);
    es_arreglo = 0;

    *ptr_inf_res = (tipo_inf_res *)calloc(1, sizeof(tipo_inf_res));
    if (*ptr_inf_res == NULL){
        error_handler(10);
        exit(1);
    }
    if (sbol->codigo == CAMPER){
        (*ptr_inf_res) -> tipo_pje = 'r';
        scanner();
    }else{
        (*ptr_inf_res) ->tipo_pje = 'v';
    }

    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre, sbol->lexema);
        scanner();
    } else
        error_handler(16);

    inf_id->clase = CLASPAR;
    //ptr_inf_res->ptero_tipo = inf_id->ptr_tipo;
    if (sbol->codigo == CCOR_ABR) {
        //controla que no ocurra un parametro como &ident[]
        if ((*ptr_inf_res)->tipo_pje != 'r') {
            scanner();
            if (sbol->codigo == CCOR_CIE) {
                scanner();
            } else {
                error_handler(21);
            }
            inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
            inf_id->ptr_tipo = en_tabla("array");
            inf_id->desc.part_var.arr.cant_elem = 0;
            es_arreglo = 1;
        } else {
            //printf("no se permite pasaje por referncia de un arreglo");
            error_handler(42);
        }
    }
    (*ptr_inf_res)->ptero_tipo = inf_id->ptr_tipo;
    (*ptr_inf_res)->arreglo = es_arreglo;
    if (es_arreglo) {
        (*ptr_inf_res)->ptr_tipo_base = inf_id->desc.part_var.arr.ptero_tipo_base;
    } else {
        (*ptr_inf_res)->ptr_tipo_base = NIL;
    }
    ptr_inf_res= &((*ptr_inf_res)->ptr_sig);
    
    (*ptr_cant_params)++;
    (*ptr_inf_res) = NULL;
    //insertarTS();
    insertarEnTSVariable();
    test(folset, NADA, 56);

}

int lista_declaraciones_init(set folset, int ath_tipo){
int bytes = 0;
    test(first[LISTA_DECLARACIONES_INIT],
            folset | first[DECLARADOR_INIT] | CCOMA | CIDENT, 57);

    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre, sbol->lexema);
        inf_id->clase = CLASVAR;
        scanner();
    } else
        error_handler(16);

   bytes +=  declarador_init(folset | (CCOMA | CIDENT) | first[DECLARADOR_INIT], ath_tipo);
    insertarEnTSVariable();

    while (sbol->codigo == CCOMA || sbol->codigo == CIDENT
            || in(sbol->codigo, first[DECLARADOR_INIT])) {

        if (sbol->codigo == CCOMA) {
            scanner();
        } else {
            error_handler(75);
        }

        if (sbol->codigo == CIDENT) {
            strcpy(inf_id->nbre, sbol->lexema);
            inf_id->clase = CLASVAR;
            inf_id->ptr_tipo = tipo_id;
            scanner();
        } else
            error_handler(16);

       bytes +=  declarador_init(folset | (CCOMA | CIDENT) | first[DECLARADOR_INIT], ath_tipo);
        insertarEnTSVariable();
    }
return bytes;
}

void declaracion_variable(set folset, int ath_tipo) {

    declarador_init(folset | CCOMA | CPYCOMA | first[LISTA_DECLARACIONES_INIT], ath_tipo);
    insertarEnTSVariable();

    if (sbol->codigo == CCOMA
            || in(sbol->codigo, first[LISTA_DECLARACIONES_INIT])) {

        if (sbol->codigo == CCOMA) {
            scanner();
        } else {
            error_handler(75);
        }

        lista_declaraciones_init(folset | CPYCOMA, ath_tipo);
    }

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22);

    test(folset, NADA, 54);

}

int declarador_init(set folset, int ath_tipo) {
    int tieneAsig = 1;
    int bytes = 0;

    test(
            first[DECLARADOR_INIT] | folset,
            folset | first[CONSTANTE] | CLLA_ABR | CLLA_CIE | CASIGNAC
            | CCOR_CIE | CCOR_ABR | first[LISTA_INICIALIZADORES], 58);
    es_arreglo = 0;
    esArregloDeclaracion = 0;

    P[lp++] = ALOC;
    fprintf(code, "ALOC\t");
    if (ath_tipo == TIPO_CHAR) {
        P[lp++] = TAMANIO_CHAR;
        fprintf(code, "%d\n", TAMANIO_CHAR);
        bytes += TAMANIO_CHAR;
    } else if (ath_tipo == TIPO_INT) {
        P[lp++] = TAMANIO_INT;
        fprintf(code, "%d\n", TAMANIO_INT);
        bytes += TAMANIO_INT;
    } else if (ath_tipo == TIPO_FLOAT) {
        P[lp++] = TAMANIO_FLOAT;
        fprintf(code, "%d\n", TAMANIO_FLOAT);
        bytes += TAMANIO_FLOAT;
    }

    inicializacionDeclaracion = 0;
    inicializacionDeclaracionTipo = TIPO_INIT;


    switch (sbol->codigo) {
        case CCONS_ENT:
        case CASIGNAC:
        {
            if (sbol->codigo == CASIGNAC) {
                scanner();
            } else {
                error_handler(79);
            }
            int tipo_cons = constante(
                    folset | CLLA_ABR | CLLA_CIE | CASIGNAC | CCOR_CIE | CCOR_ABR
                    | first[LISTA_INICIALIZADORES], ath_tipo);


            tipo_cons = chequear_tipos(ath_tipo, tipo_cons, 82);
            
            esArregloDeclaracion = 0;
            inicializacionDeclaracion = 1;
            inicializacionDeclaracionTipo = tipoSistEjec(tipo_cons);

            break;
        }
        case CCOR_ABR:
        {
            scanner();
            es_arreglo = 1;
            esArregloDeclaracion = 1;
            if (sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_FLO || sbol->codigo == CCONS_STR) {
                int tipo_cons = constante(
                        folset | CLLA_ABR | CLLA_CIE | CASIGNAC | CCOR_CIE
                        | first[LISTA_INICIALIZADORES], ath_tipo);
				tipo_cons = chequear_tipos(ath_tipo, tipo_cons, 82);
                inf_id->desc.part_var.arr.ptero_tipo_base = ath_tipo;
            
                inf_id->ptr_tipo = en_tabla("array");
                inf_id->desc.part_var.arr.cant_elem = tipo_cons;
            } else {
                tieneAsig = 0;
                //error_handler(38);
            }
            
            if (sbol->codigo == CCOR_CIE) {
                scanner();
                tam_arreglo = const_int;
            } else
                error_handler(21);

            if (sbol->codigo == CASIGNAC) {
                scanner();

                if (sbol->codigo == CLLA_ABR)
                    scanner();
                else
                    error_handler(23);

                int ats_cant = lista_inicializadores(folset | CLLA_CIE, ath_tipo);
                
                if(ats_cant != tam_arreglo){
                    error_handler(84);
                }
                
                if (ats_cant>0) {
                    tam_arreglo = ats_cant;
                }

                if (sbol->codigo == CLLA_CIE)
                    scanner();
                else
                    error_handler(24);

            } else if (!tieneAsig) {
                error_handler(38);
            }

            break;
        }
    }

    test(folset, NADA, 59);

return bytes;
}

int lista_inicializadores(set folset, int ath_tipo) {
    int ats_cant = 0; // cantidad de valores en la inicializacion del arreglo
    int tipo_cons = constante(folset | CCOMA | first[CONSTANTE], ath_tipo);

    chequear_tipos(ath_tipo, tipo_cons, 81);
    ats_cant++;

    while (sbol->codigo == CCOMA | in(sbol->codigo, first[CONSTANTE])) {

        if (sbol->codigo == CCOMA) {
            scanner();
        } else {
            error_handler(75);
        }


        tipo_cons = constante(folset | CCOMA | first[CONSTANTE], ath_tipo);
        chequear_tipos(ath_tipo, tipo_cons, 81);
        ats_cant++;
    }
    return ats_cant;
}

void proposicion_compuesta(set folset, int ath_tipo) {
    test(
            first[PROPOSICION_COMPUESTA],
            folset | first[LISTA_DECLARACIONES]
            | first[LISTA_PROPOSICIONES] | CLLA_CIE, 60);

    if (!pushie_func) {
        pushTB();

        nivel++;

        P[lp++] = ENBL;
        P[lp++] = nivel;
        fprintf(code, "ENBL\t%d\n", nivel);
    } else {
        pushie_func = 0;
    }
    if (sbol->codigo == CLLA_ABR) {
        scanner();
    } else {
        error_handler(23);
    }

    int bytesDeclarados = 0;
    if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT
            || sbol->codigo == CFLOAT) {

        bytesDeclarados += lista_declaraciones(folset | first[LISTA_PROPOSICIONES] | CLLA_CIE);
    }
    desplazamiento = 0; // ya que en caso de funcion, las variables locales deben seguir a los params
    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS
            || sbol->codigo == CMENOS || sbol->codigo == CIDENT
            || sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
            || sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
            || sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR
            || sbol->codigo == CIF || sbol->codigo == CWHILE
            || sbol->codigo == CIN || sbol->codigo == COUT
            || sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        lista_proposiciones(folset | CLLA_CIE, ath_tipo);

    if (sbol->codigo == CLLA_CIE)
        scanner();
    else
        error_handler(24);

    if (bytesDeclarados > 0) {
        P[lp++] = DMEM;
        P[lp++] = bytesDeclarados;
        fprintf(code, "DMEM\t%d\n", bytesDeclarados);
    }

    P[lp++] = FINB;
    P[lp++] = nivel;
    fprintf(code, "FINB\t%d\n", nivel);

    nivel--;

    // hago pop de todo el nivel en la TS, seguido de pop en la TB
    pop_nivel();
    
    test(folset, NADA, 61);

}

int lista_declaraciones(set folset) {
int bytes = 0;
    bytes += declaracion(folset | first[DECLARACION]);

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR
            || sbol->codigo == CINT || sbol->codigo == CFLOAT) {
        bytes += declaracion(folset | first[DECLARACION]);
    }

    return bytes;

}

int declaracion(set folset) {
    int bytes = 0;
    tipoDeRetornoDeclaracion = especificador_tipo(folset | first[LISTA_DECLARACIONES_INIT] | CPYCOMA);

    bytes += lista_declaraciones_init(folset | CPYCOMA, tipoDeRetornoDeclaracion);

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22);

    test(folset, NADA, 62);

    return bytes;

}

void lista_proposiciones(set folset, int ath_tipo) {

    proposicion(folset | first[PROPOSICION], ath_tipo);

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS
            || sbol->codigo == CMENOS || sbol->codigo == CIDENT
            || sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
            || sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
            || sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR
            || sbol->codigo == CIF || sbol->codigo == CWHILE
            || sbol->codigo == CIN || sbol->codigo == COUT
            || sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        proposicion(folset | first[PROPOSICION], ath_tipo);

}

void proposicion(set folset, int ath_tipo) {

    test(
            first[PROPOSICION],
            folset | first[PROPOSICION_EXPRESION] | first[PROPOSICION_COMPUESTA]
            | first[PROPOSICION_SELECCION]
            | first[PROPOSICION_ITERACION] | first[PROPOSICION_E_S]
            | first[PROPOSICION_RETORNO], 63);
    switch (sbol->codigo) {
        case CLLA_ABR:
            proposicion_compuesta(folset, ath_tipo);
            break;
        case CWHILE:
            proposicion_iteracion(folset, ath_tipo);
            break;
        case CIF:
            proposicion_seleccion(folset, ath_tipo);
            break;
        case CIN:
        case COUT:
            proposicion_e_s(folset);
            break;
        case CMAS:
        case CMENOS:
        case CIDENT:
        case CPAR_ABR:
        case CNEG:
        case CCONS_ENT:
        case CCONS_FLO:
        case CCONS_CAR:
        case CCONS_STR:
        case CPYCOMA:
            proposicion_expresion(folset);
            break;
        case CRETURN:
            proposicion_retorno(folset, ath_tipo);
            got_return = 1;
            break;
        default:
            error_handler(25);
            break;
    }
}

void proposicion_iteracion(set folset, int ath_tipo) {

    if (sbol->codigo == CWHILE)
        scanner();
    else
        error_handler(26);

    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19);

    int tipo_exp = expresion(folset | CPAR_CIE | first[PROPOSICION]);

    if (tipo_exp != TIPO_CHAR && tipo_exp != TIPO_INT && tipo_exp != TIPO_FLOAT) {
        error_handler(94);
    }

    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20);

    proposicion(folset, ath_tipo);

}

void proposicion_seleccion(set folset, int ath_tipo) {
    int dirElse = -1, dirFin = -1;

    if (sbol->codigo == CIF)
        scanner();
    else
        error_handler(27);

    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19);

    int tipo_exp = expresion(folset | CPAR_CIE | first[PROPOSICION] | CELSE);

    if (tipo_exp != TIPO_CHAR && tipo_exp != TIPO_INT && tipo_exp != TIPO_FLOAT) {
        error_handler(94);
    }

    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20);

    P[lp++] = BIFF;
    P[lp++] = tipoSistEjec(tipo_exp);
    dirElse = lp;
    lp++; // reservo lugar para la direccion del salto
    fprintf(code, "BIFF\t%d\tDIRELSE\n", tipoSistEjec(tipo_exp));

    proposicion(folset | first[PROPOSICION] | CELSE, ath_tipo);

    if (sbol->codigo == CELSE) {
        // si entro a la rama verdadera, ahora debe saltar la falsa hasta el fin
        P[lp++] = BIFS;
        dirFin = lp;
        lp++; // reservo lugar para la direccion del salto
        fprintf(code, "BIFS\tDIRFIN\n");

        // sino, que ejecute la rama falsa, desde aqui
        P[dirElse] = lp - dirElse - 1;

        scanner();

        proposicion(folset, ath_tipo);
    } else {
        // no hay else, si la condicion no se cumple debe saltar al final
        P[dirElse] = lp - dirElse - 1;
    }

}

void proposicion_e_s(set folset) {

    switch (sbol->codigo) {
        case CIN:
        {
            scanner();
            if (sbol->codigo == CSHR) {
                scanner();
            } else {
                error_handler(28);
            }

            int tipo_var = variable(folset | CSHR | first[VARIABLE] | CPYCOMA, TIPO_INIT);

            if (tipo_var != TIPO_CHAR && tipo_var != TIPO_INT && tipo_var != TIPO_FLOAT) {
                error_handler(95);
            }

            int tipo_var_ejec = tipoSistEjec(tipo_var);
            // primero debo hacer POP, porque variable() cargo el valor y no se usa
            generarPop(tipo_var_ejec);
            generarLeer(tipo_var_ejec);
            generarAlmacenar(variableNivel, variableDesplaz, tipo_var_ejec);
            generarPop(tipo_var_ejec);

            while (sbol->codigo == CSHR || in(sbol->codigo, first[VARIABLE])) {
                if (sbol->codigo == CSHR) {
                    scanner();
                } else
                    error_handler(76);

                tipo_var = variable(folset | CSHR | first[VARIABLE] | CPYCOMA, TIPO_INIT);

                if (tipo_var != TIPO_CHAR && tipo_var != TIPO_INT && tipo_var != TIPO_FLOAT) {
                    error_handler(95);
                }

                tipo_var_ejec = tipoSistEjec(tipo_var);
                // primero debo hacer POP, porque variable() cargo el valor y no se usa
                generarPop(tipo_var_ejec);
                generarLeer(tipo_var_ejec);
                generarAlmacenar(variableNivel, variableDesplaz, tipo_var_ejec);
                generarPop(tipo_var_ejec);
            }
            if (sbol->codigo == CPYCOMA)
                scanner();
            else
                error_handler(22);
            break;
        }
        case COUT:
        {
            enProposicionSalida = 1;
            scanner();
            if (sbol->codigo == CSHL) {
                scanner();
            } else {
                error_handler(29);
            }

            int tipo_exp = expresion(folset | CSHL | first[EXPRESION] | CPYCOMA);

            if (tipo_exp != TIPO_CHAR && tipo_exp != TIPO_INT && tipo_exp != TIPO_FLOAT && tipo_exp != TIPO_STRING) {
                error_handler(96);
            }

            if (tipo_exp == TIPO_STRING) {
                generarImprimirString();
            } else {
                int tipo_exp_ejec = tipoSistEjec(tipo_exp);
                generarImprimir(tipo_exp_ejec);
            }

            while (sbol->codigo == CSHL || in(sbol->codigo, first[EXPRESION])) {
                if (sbol->codigo == CSHL) {
                    scanner();
                } else {
                    error_handler(77);
                }

                tipo_exp = expresion(folset | CSHL | first[EXPRESION] | CPYCOMA);

                if (tipo_exp != TIPO_CHAR && tipo_exp != TIPO_INT && tipo_exp != TIPO_FLOAT && tipo_exp != TIPO_STRING) {
                    error_handler(96);
                }

                if (tipo_exp == TIPO_STRING) {
                    generarImprimirString();
                } else {
                    int tipo_exp_ejec = tipoSistEjec(tipo_exp);
                    generarImprimir(tipo_exp_ejec);
                }

            }
            if (sbol->codigo == CPYCOMA) {
                scanner();
            } else {
                error_handler(22);
            }
            enProposicionSalida = 0;
            break;

        }
        default:
            error_handler(30);
            break;
    }
    test(folset, NADA, 64);
}

void proposicion_retorno(set folset, int ath_tipo) {

    scanner();
    int ats_tipo = expresion(folset | CPYCOMA);

    if (ath_tipo == TIPO_VOID) {
        error_handler(89);
    } else {
        chequear_tipos(ath_tipo, ats_tipo, 88);
    }

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22);

    test(folset, NADA, 65);

}

void proposicion_expresion(set folset) {

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == CIDENT
            || sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
            || sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
            || sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        expresion(folset | CPYCOMA);

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22);

    test(folset, NADA, 66);
}

int expresion(set folset) {

    if ((expresionActual == EXPRESION_VARIABLE) || (expresionActual == EXPRESION_ARREGLO)) {
        expresionActual = EXPRESION_OTRO;
    }

    int ats_tipo = expresion_simple(
            folset | CASIGNAC | first[EXPRESION_SIMPLE] | first[RELACION]);

    switch (sbol->codigo) {
        case CASIGNAC:
        {
            if (!esVarIzqAsignacion()) {
                error_handler(98);
            }
            scanner();
            // hago backup de la ubicacion de la variable en el stack
            int varAlmNivel = variableNivel;
            int varAlmDesp = variableDesplaz;

            int tipo_izq_ejec = tipoSistEjec(ats_tipo);
            // primero debo hacer POP, porque variable() cargo el valor y no se usa
            generarPop(tipo_izq_ejec);

            int tipo2 = expresion(folset | first[EXPRESION_SIMPLE] | first[RELACION]);

            int tipo_der_ejec = tipoSistEjec(tipo2);
            // si la parte derecha es de un tipo menor al de la izquierda, hay que hacer cast
            if (tipo_der_ejec < tipo_izq_ejec) {
                generarCast(tipo_der_ejec, tipo_izq_ejec);
            }
            // almaceno el valor
            generarAlmacenar(varAlmNivel, varAlmDesp, tipo_izq_ejec);
            // no hay que hacer pop, porque la asignacion retorna el valor asignado

            ats_tipo = chequear_tipos(ats_tipo, tipo2, 87);
            break;
        }
        case CDISTINTO:
        case CIGUAL:
        case CMENOR:
        case CMEIG:
        case CMAYOR:
        case CMAIG:
        {
            long operador = sbol->codigo;
            scanner();

            int tipo_izq_ejec = tipoSistEjec(ats_tipo);

            // Cast futuro, reservo el lugar para el tipo destino del cast y luego lo lleno
            int dirCastFuturo = generarCastFuturo(tipo_izq_ejec);

            int tipo2 = expresion(folset | first[EXPRESION_SIMPLE]);

            int tipo_oper = tipo_izq_ejec;
            
            int tipo_der_ejec = tipoSistEjec(tipo2);
            // Completo el tipo adeudado en el cast futuro
            if (tipo_der_ejec > tipo_izq_ejec) {
                // el cast es significativo, casteo al tipo mayor
                P[dirCastFuturo] = tipo_der_ejec;
                tipo_oper = tipo_der_ejec;
                //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_der_ejec);
            } else {
                // no es necesario el cast, asi que pongo el mismo tipo para que no se realice ninguna accion de cast
                P[dirCastFuturo] = tipo_izq_ejec;
                //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_izq_ejec);
            }

            // si la parte derecha es de un tipo menor al de la izquierda, hay que hacer cast
            if (tipo_der_ejec < tipo_izq_ejec) {
                generarCast(tipo_der_ejec, tipo_izq_ejec);
            }
            switch (operador) {
                case CDISTINTO: P[lp++] = CMNI;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMNI\t%d\n", tipo_oper);
                    break;
                case CIGUAL: P[lp++] = CMIG;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMIG\t%d\n", tipo_oper);
                    break;
                case CMENOR: P[lp++] = CMME;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMME\t%d\n", tipo_oper);
                    break;
                case CMEIG: P[lp++] = CMEI;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMEI\t%d\n", tipo_oper);
                    break;
                case CMAYOR: P[lp++] = CMMA;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMMA\t%d\n", tipo_oper);
                    break;
                case CMAIG: P[lp++] = CMAI;
                    P[lp++] = tipo_oper;
                    fprintf(code, "CMAI\t%d\n", tipo_oper);
                    break;
            }

            ats_tipo = cohersion(ats_tipo, tipo2);

            break;
        }
            /*
                    default:
                    {
                        if (in(sbol->codigo, first[EXPRESION])) {
                            error_handler(78);
                            expresion(folset);
                        }
                    }
             */
    }

    return ats_tipo;
}

int expresion_simple(set folset) {

    if ((expresionActual == EXPRESION_VARIABLE) || (expresionActual == EXPRESION_ARREGLO)) {
        expresionActual = EXPRESION_OTRO;
    }
    test(first[EXPRESION_SIMPLE], folset | CMAS | CMENOS | COR | first[TERMINO],
            67);

    int invertirSigno = 0;
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
        if (sbol->codigo == CMENOS) {
            invertirSigno = 1;
        }
        scanner();
    }

    int ats_tipo = termino(folset | CMAS | CMENOS | COR | first[TERMINO], TIPO_INIT);

    if (invertirSigno) {
        generarInvertir(tipoSistEjec(ats_tipo));
    }

    int tipo_izq_ejec;
    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR
            || (in(sbol->codigo, first[TERMINO]))) {

        long operador = -1;
        if (sbol->codigo == CMAS || sbol->codigo == CMENOS
                || sbol->codigo == COR) {
            operador = sbol->codigo;
            scanner();

            tipo_izq_ejec = tipoSistEjec(ats_tipo);

            // Cast futuro, reservo el lugar para el tipo destino del cast y luego lo lleno
            //int dirCastFuturo = generarCastFuturo(tipo_izq_ejec);

        } else {
            error_handler(78);
        }
        // Cast futuro, reservo el lugar para el tipo destino del cast y luego lo lleno
		int dirCastFuturo = generarCastFuturo(tipo_izq_ejec);
        int ats_tipo2 = termino(folset, ats_tipo);
        int tipo_der_ejec = tipoSistEjec(ats_tipo2);

        int tipo_oper = tipo_izq_ejec;
        // Completo el tipo adeudado en el cast futuro
        if (tipo_der_ejec > tipo_izq_ejec) {
            // el cast es significativo, casteo al tipo mayor
            P[dirCastFuturo] = tipo_der_ejec;
            tipo_oper = tipo_der_ejec;
            //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_der_ejec);
        } else {
            // no es necesario el cast, asi que pongo el mismo tipo para que no se realice ninguna accion de cast
            P[dirCastFuturo] = tipo_izq_ejec;
            //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_izq_ejec);
        }

        // si la parte derecha es de un tipo menor al de la izquierda, hay que hacer cast
        if (tipo_der_ejec < tipo_izq_ejec) {
            generarCast(tipo_der_ejec, tipo_izq_ejec);
        }
        switch (operador) {
            case CMAS: P[lp++] = SUM;
                P[lp++] = tipo_oper;
                fprintf(code, "SUM\t%d\n", tipo_oper);
                break;
            case CMENOS: P[lp++] = SUB;
                P[lp++] = tipo_oper;
                fprintf(code, "SUB\t%d\n", tipo_oper);
                break;
            case COR: P[lp++] = OR;
                P[lp++] = tipo_oper;
                fprintf(code, "OR\t%d\n", tipo_oper);
                break;
        }

        ats_tipo = cohersion(ats_tipo, ats_tipo2);
    }

    return ats_tipo;
}

int termino(set folset, int ath_tipo) {
    if ((expresionActual == EXPRESION_VARIABLE) || (expresionActual == EXPRESION_ARREGLO)) {
        expresionActual = EXPRESION_OTRO;
    }

    int ats_tipo = factor(folset | CMULT | CDIV | CAND | first[FACTOR], ath_tipo);
    ats_tipo = cohersion(ath_tipo, ats_tipo);

    while (sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND
            || in(sbol->codigo, first[FACTOR])) {
        long operador = -1;
        if (sbol->codigo == CMULT || sbol->codigo == CDIV
                || sbol->codigo == CAND) {
            operador = sbol->codigo;
            scanner();
        } else {
            error_handler(78);
        }
        int tipo_izq_ejec = tipoSistEjec(ats_tipo);

        // Cast futuro, reservo el lugar para el tipo destino del cast y luego lo lleno
        int dirCastFuturo = generarCastFuturo(tipo_izq_ejec);

        int ats_tipo2 = factor(folset | CMULT | CDIV | CAND | first[FACTOR], ath_tipo);


        int tipo_der_ejec = tipoSistEjec(ats_tipo2);

        int tipo_oper = tipo_izq_ejec;
        // Completo el tipo adeudado en el cast futuro
        if (tipo_der_ejec > tipo_izq_ejec) {
            // el cast es significativo, casteo al tipo mayor
            P[dirCastFuturo] = tipo_der_ejec;
            tipo_oper = tipo_der_ejec;
            //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_der_ejec);
        } else {
            // no es necesario el cast, asi que pongo el mismo tipo para que no se realice ninguna accion de cast
            P[dirCastFuturo] = tipo_izq_ejec;
            //fprintf(code, ">>> TIPO CAST FUTURO: %d\n", tipo_izq_ejec);
        }

        // si la parte derecha es de un tipo menor al de la izquierda, hay que hacer cast
        if (tipo_der_ejec < tipo_izq_ejec) {
            generarCast(tipo_der_ejec, tipo_izq_ejec);
        }
        switch (operador) {
            case CMULT: P[lp++] = MUL;
                P[lp++] = tipo_oper;
                fprintf(code, "MUL\t%d\n", tipo_oper);
                break;
            case CDIV: P[lp++] = DIV;
                P[lp++] = tipo_oper;
                fprintf(code, "DIV\t%d\n", tipo_oper);
                break;
            case CAND: P[lp++] = AND;
                P[lp++] = tipo_oper;
                fprintf(code, "AND\t%d\n", tipo_oper);
                break;
        }

        ats_tipo = cohersion(ats_tipo, ats_tipo2);
    }
    return ats_tipo;
}

int factor(set folset, int ath_tipo) {
    if ((expresionActual == EXPRESION_VARIABLE) || (expresionActual == EXPRESION_ARREGLO)) {
        expresionActual = EXPRESION_OTRO;
    }

    test(first[FACTOR], folset, 68);

    int ats_tipo = TIPO_ERROR;
    switch (sbol->codigo) {
        case CIDENT:
        {
            char token_actual[TAM_LEXEMA];
            strcpy(token_actual, sbol->lexema);
            switch (Clase_Ident(token_actual)) {
                case CLASFUNC:
                    ats_tipo = Tipo_Ident(token_actual);
                    llamada_funcion(folset);
                    break;
                case CLASPAR:
                case CLASVAR:
                    ats_tipo = Tipo_Ident(token_actual);
                    if (ats_tipo == en_tabla("array") ){
                        // sintetizo el tipo base del arreglo
                        ats_tipo = ts[en_tabla(token_actual)].ets->desc.part_var.arr.ptero_tipo_base;
                    }

                    variable(folset | CPAR_CIE, ath_tipo);
                    break;


                default:
                    //Identificador no declarado
                    ats_tipo = TIPO_ERROR;
                    error_handler(33);
                    scanner();
                    if (sbol->codigo == CCOR_ABR) { //es arreglo
                        scanner();
                        expresion(folset | CCOR_CIE);
                        if (sbol->codigo == CCOR_CIE) {
                            scanner();
                        } else {
                            error_handler(21);
                        }
                        insertarEnTSArregloError(token_actual);
                        //doy de alta en tabla de simbolos
                    } else if (sbol->codigo == CPAR_ABR) { //es funcion
                        scanner();
                        if (sbol->codigo == CMAS || sbol->codigo == CMENOS
                                || sbol->codigo == CIDENT || sbol->codigo == CPAR_ABR
                                || sbol->codigo == CNEG || sbol->codigo == CCONS_ENT
                                || sbol->codigo == CCONS_FLO
                                || sbol->codigo == CCONS_CAR
                                || sbol->codigo == CCONS_STR) {
                            lista_expresiones(folset | CPAR_CIE,"");
                        }
                        if (sbol->codigo == CPAR_CIE) {
                            scanner();
                        } else {
                            error_handler(20);
                        }
                        insertarEnTSFuncionError(token_actual);
                        //doy de alta en tabla de simbolos
                    } else { //es variable
                        //doy de alta en tabla de simbolos
                        insertarEnTSVariableError(token_actual);
                    }
                    break;
            }
        }
            break;
        case CCONS_ENT:
        case CCONS_FLO:
        case CCONS_CAR:
            ats_tipo = constante(folset, ath_tipo);
            break;
        case CCONS_STR:
            if (!enProposicionSalida) {
                error_handler(97);
            }
            P[lp++] = CRCTS;
            P[lp++] = lc;
            fprintf(code, "CRCTS\t%d\n", lc);

            char lexema[100];
            strcpy(lexema, sbol->lexema);

            // si el string termina con "\\n" (\n como 2 caracteres) los reemplazo por "\n\0"
            if (lexema[strlen(lexema) - 2] == '\\' && lexema[strlen(lexema) - 1] == 'n') {
                lexema[strlen(lexema) - 2] = '\n';
                lexema[strlen(lexema) - 1] = '\0';
            }// si el string termina con "\\t" (\t como 2 caracteres) los reemplazo por "\t\0"
            else if (lexema[strlen(lexema) - 2] == '\\' && lexema[strlen(lexema) - 1] == 't') {
                lexema[strlen(lexema) - 2] = '\t';
                lexema[strlen(lexema) - 1] = '\0';
            }

            // guardo el lexema en la zona de constantes string
            int i;
            for (i = 0; i < strlen(lexema); i++) {
                C[lc++] = lexema[i];
            }
            C[lc++] = '\0'; // para delimitar el final del string en la zona de constantes string

            ats_tipo = TIPO_STRING;

            scanner();
            break;
        case CPAR_ABR:
        {
            scanner();
            ats_tipo = expresion(folset | CPAR_CIE);
            if (sbol->codigo == CPAR_CIE)
                scanner();
            else
                error_handler(20);
            break;
        }
        case CNEG:
        {
            scanner();
            ats_tipo = expresion(folset);
            generarNegacion(tipoSistEjec(ats_tipo));
            break;
        }
        default:
            ats_tipo = TIPO_ERROR;
            error_handler(31);
    }

    test(folset, NADA, 69);
    return ats_tipo;
}

int variable(set folset, int ath_tipo) {
    test(first[VARIABLE], folset | CLLA_ABR | first[EXPRESION] | CLLA_CIE, 70);
    char ident_actual[TAM_LEXEMA];
    int pos;
    int ats_tipo = 99; // NO ES ERROR, PONER CONSTANTE
    int desconocida = 0, esArreglo = 0, sinIndice = 1;
    strcpy(ident_actual, "");
    if (sbol->codigo == CIDENT) {
        if (en_tabla(sbol->lexema) == NIL) {
            ats_tipo = TIPO_ERROR;
            error_handler(33);
            inf_id->clase = CLASVAR;
            inf_id->ptr_tipo = en_tabla("error");
            /* el alumno debera verificar con una consulta a TS
             si, siendo la variable un arreglo, corresponde o no
             verificar la presencia del subindice */
            if (sbol->codigo == CCOR_ABR) {
                sinIndice = 0;
                inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
                inf_id->ptr_tipo = en_tabla("array");
            }
            desconocida =1;
            //insertarTS();
            insertarEnTSVariable();
        }
        strcpy(ident_actual, sbol->lexema);
        scanner();
    } else {
        error_handler(16);
        desconocida =1;
        strcpy(ident_actual, "");
    }

    /* el alumno debera verificar con una consulta a TS
     si, siendo la variable un arreglo, corresponde o no
     verificar la presencia del subindice */
    if (en_tabla(ident_actual) != NIL
            && ts[en_tabla(ident_actual)].ets->ptr_tipo == en_tabla("array")) {
        if (sbol->codigo == CCOR_ABR) {
            if (es_parametro
                    && (sbol->codigo == CCOMA || sbol->codigo == CPAR_CIE)) {
                error_handler(43);
            }
            sinIndice = 0;
            scanner();
            expresion(folset | CCOR_CIE);
            if (sbol->codigo == CCOR_CIE)
                scanner();
            else
                error_handler(21);
        } else {
            if (!es_parametro) {
                error_handler(42);
            }
        }
    } else {
        if (sbol->codigo == CCOR_ABR) {
            scanner();
            error_handler(32);
            expresion(folset | CCOR_CIE);
            if (sbol->codigo == CCOR_CIE)
                scanner();
        }
    }
    if (ats_tipo == 99) {
        pos = en_tabla(ident_actual);
        if (Tipo_Ident(ident_actual) == en_tabla("array") && Clase_Ident(ident_actual) == CLASVAR) { // no se puede usar esArreglo, porque en las llamadas a funcion, si es conocido no se setea
            // sintetizo el tipo base
            ats_tipo = ts[pos].ets->desc.part_var.arr.ptero_tipo_base;
            //printf(">>> DEBUG >>> tipo rama A %d\n", ats_tipo);
        } else if (Tipo_Ident(ident_actual) == en_tabla("array") && Clase_Ident(ident_actual) == CLASPAR) { // no se puede usar esArreglo, porque en las llamadas a funcion, si es conocido no se setea
            // sintetizo el tipo base
            ats_tipo = ts[pos].ets->desc.part_var.arr.ptero_tipo_base;
            //printf(">>> DEBUG >>> tipo rama B %d\n", ats_tipo);
        } else { // es una variable simple
            ats_tipo = ts[pos].ets->ptr_tipo;
            //printf(">>> DEBUG >>> tipo rama C %d\n", ats_tipo);
        }
    }

    
        if (expresionActual == EXPRESION_INIT) {
                    if (desconocida) {
                            expresionActual = EXPRESION_OTRO;
                    }
                    else {
                            if (Tipo_Ident(ident_actual) == en_tabla("array")) {	// no se puede usar esArreglo, porque en las llamadas a funcion, si es conocido no se setea
                                    if (sinIndice) {
                                            expresionActual = EXPRESION_ARREGLO;
                                    }
                                    else {
                                            expresionActual = EXPRESION_VARIABLE;
                                    }
                            }
                            else {
                                    expresionActual = EXPRESION_VARIABLE;
                            }
                    }
            }
            else {
                    expresionActual = EXPRESION_OTRO;
            }
     

    if (ats_tipo != TIPO_ERROR) {
        int tipoVarEjec = tipoSistEjec(ats_tipo);
        int tipoHerEjec = tipoSistEjec(ath_tipo);

        if (ath_tipo == TIPO_INIT || tipoVarEjec == tipoHerEjec) {
            P[lp++] = CRVL;
            P[lp++] = ts[pos].ets->desc.nivel;
            P[lp++] = ts[pos].ets->desc.despl;
            P[lp++] = tipoVarEjec;
            fprintf(code, "CRVL\t%d\t%d\t%d\n", ts[pos].ets->desc.nivel, ts[pos].ets->desc.despl, tipoVarEjec);
        } else if (tipoVarEjec < tipoHerEjec) {
            P[lp++] = CRVL;
            P[lp++] = ts[pos].ets->desc.nivel;
            P[lp++] = ts[pos].ets->desc.despl;
            P[lp++] = tipoVarEjec;
            fprintf(code, "CRVL\t%d\t%d\t%d\n", ts[pos].ets->desc.nivel, ts[pos].ets->desc.despl, tipoVarEjec);
            generarCast(tipoVarEjec, tipoHerEjec);
        } else if (tipoVarEjec > tipoHerEjec) {
            generarCast(tipoHerEjec, tipoVarEjec);
            P[lp++] = CRVL;
            P[lp++] = ts[pos].ets->desc.nivel;
            P[lp++] = ts[pos].ets->desc.despl;
            P[lp++] = tipoVarEjec;
            fprintf(code, "CRVL\t%d\t%d\t%d\n", ts[pos].ets->desc.nivel, ts[pos].ets->desc.despl, tipoVarEjec);
        }

        variableNivel = ts[pos].ets->desc.nivel;
        variableDesplaz = ts[pos].ets->desc.despl;
    } else {
        variableNivel = -1;
        variableDesplaz = -1;
    }
    test(folset, NADA, 71);

    return ats_tipo;
}

void llamada_funcion(set folset) {
    char lexema[TAM_LEXEMA];
    int desconocida=0;

    if (sbol->codigo == CIDENT) {
        strcpy(lexema, sbol->lexema);
        if (en_tabla(lexema) == NIL) {
                error_handler(33);
		desconocida=1;
	}
        scanner();
    } else {
        strcpy(lexema, "");
        error_handler(16);
        desconocida=1;
    }

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == CIDENT
            || sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
            || sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
            || sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR) {
        lista_expresiones(folset | CPAR_CIE, lexema);
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    test(folset, NADA, 72);
}

void lista_expresiones(set folset, char lexema[]) {
   int cont_params = 0;
    es_parametro = 1;
	
	tipo_inf_res *params = NULL;
	int cant_params_formales = 0, cant_params_formales_aux = 0;
	int pos = en_tabla(lexema);
	int tipo_param_formal;
	int tipo_base_param_formal;
	char pasaje_param_formal;
	int param_es_arreglo;
	int hayMasParamsFormales = 0;
	if (pos != NIL) {
		params = ts[pos].ets->desc.part_var.sub.ptr_inf_res;
		cant_params_formales = ts[pos].ets->desc.part_var.sub.cant_par;
		cant_params_formales_aux = cant_params_formales;
		if (cant_params_formales_aux > 0) {
			hayMasParamsFormales = 1;
			tipo_param_formal = params->ptero_tipo;
			pasaje_param_formal = params->tipo_pje;
			tipo_base_param_formal = params->ptr_tipo_base;
			param_es_arreglo = params->arreglo;
		}
	}
    expresionActual = EXPRESION_INIT;
    int ats_tipo = expresion(folset | CCOMA);
    cont_params++;
    
    //chequear_tipos(tipo_param, ats_tipo, 91);
    
    if (pos != NIL && hayMasParamsFormales) {
		if (tipo_param_formal == en_tabla("array")) {
			if (expresionActual != EXPRESION_ARREGLO) {	// estos IFs deben estar anidados !!! (no combinar la condicion)
				error_handler(93);
			}
		}
		else {
			if (expresionActual == EXPRESION_ARREGLO) {
				error_handler(91);
			}
			else {
				if (param_es_arreglo) {
					chequear_tipos(tipo_base_param_formal, ats_tipo, 91);
					//printf(">>> DEBUG >>> cheq 1 tipo param formal %d -- tipo param actual %d\n", tipo_base_param_formal, ats_tipo);
				}
				else {
					chequear_tipos(tipo_param_formal, ats_tipo, 91);
					//printf(">>> DEBUG >>> cheq 2 tipo param formal %d -- tipo param actual %d\n", tipo_param_formal, ats_tipo);
				}
			}
			
			if ((pasaje_param_formal == 'r') && (expresionActual != EXPRESION_VARIABLE)) {
				error_handler(92);
			}
		}
	}
	expresionActual = EXPRESION_FIN;
	cant_params_formales_aux--;
	hayMasParamsFormales = cant_params_formales_aux > 0;

    while (sbol->codigo == CCOMA || (in(sbol->codigo, first[EXPRESION]))) {
        if (sbol->codigo == CCOMA) {
            scanner();
        } else {
            error_handler(75);
        }
        
        if (pos != NIL && hayMasParamsFormales) {
			params = params->ptr_sig;
			tipo_param_formal = params->ptero_tipo;
			pasaje_param_formal = params->tipo_pje;
			tipo_base_param_formal = params->ptr_tipo_base;
			param_es_arreglo = params->arreglo;
		}
		
		expresionActual = EXPRESION_INIT;
        ats_tipo = expresion(folset | CCOMA);
        
         cont_params++;
		
		if (pos != NIL && hayMasParamsFormales) {
			if (tipo_param_formal == en_tabla("array")) {
				if (expresionActual != EXPRESION_ARREGLO) {	// estos IFs deben estar anidados !!! (no combinar la condicion)
					error_handler(93);
				}
			}
			else {
				if (expresionActual == EXPRESION_ARREGLO) {
					error_handler(91);
				}
				else {
					if (param_es_arreglo) {
						chequear_tipos(tipo_base_param_formal, ats_tipo, 91);
					}
					else {
						chequear_tipos(tipo_param_formal, ats_tipo, 91);
					}
				}
				
				if ((pasaje_param_formal == 'r') && (expresionActual != EXPRESION_VARIABLE)) {
					error_handler(92);
				}
			}
		}
		expresionActual = EXPRESION_FIN;
		cant_params_formales_aux--;
		hayMasParamsFormales = cant_params_formales_aux > 0;
	}
	
	if ((cant_params_formales != cont_params) && (pos != NIL)) {
		error_handler(90);
	}
         es_parametro = 0;
}



int constante(set folset, int ath_tipo) {
    char tmp[TAM_LEXEMA];

    int ats_tipo;
    test(first[CONSTANTE], folset, 73);

    switch (sbol->codigo) {
        case CCONS_ENT:
            ats_tipo = TIPO_INT;
            const_int = atoi(sbol->lexema);
            scanner();
            break;
        case CCONS_FLO:
            ats_tipo = TIPO_FLOAT;
            const_float = atof(sbol->lexema);
            scanner();
            break;
        case CCONS_CAR:
            ats_tipo = TIPO_CHAR;
            //strcpy(const_char, sbol->lexema);
            const_char = sbol->lexema[1];
            scanner();
            break;
        case CCONS_STR:
            ats_tipo = TIPO_STRING;
            //strcpy(const_char, sbol->lexema);
            //const_char = sbol->lexema[1];
            scanner();
            break;
        default:
            ats_tipo = TIPO_ERROR;
            error_handler(38);
            break; /*f_error(); aca va f_error, faltan los algoritmos de conversion a las constantes numericas. */
    }

    int tipoConsEjec = tipoSistEjec(ats_tipo);
    int tipoHerEjec = tipoSistEjec(ath_tipo);

    if (ath_tipo == TIPO_INIT || tipoConsEjec == tipoHerEjec) {
        P[lp++] = CRCT;
        P[lp++] = tipoConsEjec;
        fprintf(code, "CRCT\t%d\t", tipoConsEjec);
        switch (tipoConsEjec) {
            case TIPO_CHAR_SIST_EJEC: P[lp++] = const_char;
                fprintf(code, "%c\n", const_char);
                break;
            case TIPO_INT_SIST_EJEC: P[lp++] = const_int;
                fprintf(code, "%d\n", const_int);
                break;
            case TIPO_FLOAT_SIST_EJEC: P[lp++] = const_float;
                fprintf(code, "%.2f\n", const_float);
                break;
        }
    } else if (tipoConsEjec < tipoHerEjec) {
        P[lp++] = CRCT;
        P[lp++] = tipoConsEjec;
        fprintf(code, "CRCT\t%d\t", tipoConsEjec);
        switch (tipoConsEjec) {
            case TIPO_CHAR_SIST_EJEC: P[lp++] = const_char;
                fprintf(code, "%c\n", const_char);
                break;
            case TIPO_INT_SIST_EJEC: P[lp++] = const_int;
                fprintf(code, "%d\n", const_int);
                break;
            case TIPO_FLOAT_SIST_EJEC: P[lp++] = const_float;
                fprintf(code, "%.2f\n", const_float);
                break;
        }
        generarCast(tipoConsEjec, tipoHerEjec);
    } else if (tipoConsEjec > tipoHerEjec) {
        generarCast(tipoHerEjec, tipoConsEjec);
        P[lp++] = CRCT;
        P[lp++] = tipoConsEjec;
        fprintf(code, "CRCT\t%d\t", tipoConsEjec);
        switch (tipoConsEjec) {
            case TIPO_CHAR_SIST_EJEC: P[lp++] = const_char;
                fprintf(code, "%c\n", const_char);
                break;
            case TIPO_INT_SIST_EJEC: P[lp++] = const_int;
                fprintf(code, "%d\n", const_int);
                break;
            case TIPO_FLOAT_SIST_EJEC: P[lp++] = const_float;
                fprintf(code, "%.2f\n", const_float);
                break;
        }
    }

    test(folset, NADA, 74);
    return ats_tipo;
}

void existFuncionMain() {

    if (en_tabla("main") == NIL) {
        /*No existe función main*/
        error_print(15);
        return;
    }

    if (ts[en_tabla("main")].ets->ptr_tipo != en_tabla("void")) {
        /* no de tipo vod*/
        error_print(35);
    }

    if (Clase_Ident("main") != CLASFUNC) {
        /* no es clase  funcion */
        error_print(34);
    }

    if (ts[en_tabla("main")].ets->desc.part_var.sub.cant_par > 0) {
        /* la funcion main no lleva parametros*/
        error_print(36);
    }

}

void insertarEnTSFuncionError(char lexema[]) {
    strcpy(inf_id->nbre, lexema);
    inf_id->clase = CLASFUNC;
    inf_id->ptr_tipo = en_tabla("error");
    insertarTS();
}

void insertarEnTSVariableError(char lexema[]) {
    strcpy(inf_id->nbre, lexema);
    inf_id->clase = CLASVAR;
    inf_id->ptr_tipo = en_tabla("error");
    insertarTS();
}

void insertarEnTSArregloError(char lexema[]) {
    strcpy(inf_id->nbre, lexema);
    inf_id->clase = CLASVAR;
    inf_id->ptr_tipo = en_tabla("array");
    inf_id->desc.part_var.arr.ptero_tipo_base = en_tabla("error");
    insertarTS();
}

void insertarEnTSVariable() {

    if (strcmp(inf_id->nbre, EMPTY_STR) != 0) {

        inf_id->clase = CLASVAR;
        if (es_arreglo) {
            inf_id->ptr_tipo = en_tabla("array");
            inf_id->cant_byte = ts[inf_id->desc.part_var.arr.ptero_tipo_base].ets->cant_byte
                    * tam_arreglo;
            //inf_id->desc.part_var.arr.ptero_tipo_base = tipoDeRetornoDeclaracion;
            inf_id->desc.part_var.arr.cant_elem = tam_arreglo;

        } else {
            inf_id->ptr_tipo = tipo_id;
            inf_id->cant_byte = ts[inf_id->ptr_tipo].ets->cant_byte;
        }

        inf_id->desc.nivel = nivel;
        inf_id->desc.despl = desplazamiento;

        insertarTS();

        if (inicializacionDeclaracion && inicializacionDeclaracionTipo != TIPO_INIT) {
            generarAlmacenar(nivel, desplazamiento, inicializacionDeclaracionTipo);
            generarPop(inicializacionDeclaracionTipo);
            inicializacionDeclaracion = 0;
            inicializacionDeclaracionTipo = TIPO_INIT;
        }

        incDesplaz(tipoDeRetornoDeclaracion);

    }
}

void insertarFuncionEnTS() {
    if (strcmp(inf_id->nbre, EMPTY_STR) != 0) {
        inf_id->clase = CLASFUNC;
        inf_id->ptr_tipo = tipoDeRetornoDeclaracion;
        if ((*ptr_cant_params) > 0) { // si hay parametros
            inf_id->desc.part_var.sub.cant_par = (*ptr_cant_params);
            inf_id->desc.part_var.sub.ptr_inf_res = ptr_inf_res;
        }
        insertarTS();

        //tipoDeRetornoDeclaracionFuncion = tipoDeRetornoDeclaracion;
        //strcpy(identificadorDeclaracion, STR_VACIO);
    }
}

void incDesplaz(int tipo) {
    if (tipo == TIPO_CHAR) {
        desplazamiento += TAMANIO_CHAR;
    } else if (tipo == TIPO_INT) {
        desplazamiento += TAMANIO_INT;
    } else if (tipo == TIPO_FLOAT) {
        desplazamiento += TAMANIO_FLOAT;
    }
}