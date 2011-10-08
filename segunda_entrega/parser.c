/******************************************************************
 Este es el archivo correspondiente al PARSER
 *******************************************************************/

#define EXTERNA extern
#define EMPTY_STR ""

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigos.h"
#include "var_globales.h"
#include "ts.h"
#include "error.h"
#include "set.h"

/*********** prototipos *************/

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
char const_char[];

set first[60];

int es_parametro = 0;
int tipo_id = NIL;

//revisa si ya hice push de nivel
int pushie_func = 0;
tipo_inf_res *ptr_inf_res;
int *ptr_cant_params;

//chequeo por nombre de funcion main
char nbre_func[];

/**
 * 
 */

void inic_set() {

	first[UNIDAD_TRADUCCION] = cons(CVOID | CCHAR | CINT | CFLOAT | CEOF);
	first[DECLARACIONES] = cons(CVOID | CCHAR | CINT | CFLOAT | NADA);
	first[ESPECIFICADOR_TIPO] = cons(CVOID | CCHAR | CINT | CFLOAT | NADA);
	first[ESPECIFICADOR_DECLARACION] = cons(
			CCOR_ABR | CCOMA | CPYCOMA | CASIGNAC | CPAR_ABR | NADA);
	first[DEFINICION_FUNCION] = cons(CPAR_ABR | NADA);
	first[DECLARACION_VARIABLE] = cons(
			CCOR_ABR | CCOMA | CPYCOMA | CASIGNAC | NADA);
	first[LISTA_DECLARACIONES_PARAM] = first[ESPECIFICADOR_TIPO];
	first[DECLARACION_PARAMETRO] = first[ESPECIFICADOR_TIPO];
	first[DECLARADOR_INIT] = first[DECLARACION_VARIABLE];
	first[CONSTANTE] = cons(NADA | CCONS_ENT | CCONS_FLO | CCONS_CAR);
	first[LISTA_DECLARACIONES_INIT] = cons(NADA | CIDENT);
	first[LISTA_INICIALIZADORES] = first[CONSTANTE];
	first[LISTA_DECLARACIONES] = first[ESPECIFICADOR_TIPO];
	first[DECLARACION] = first[ESPECIFICADOR_TIPO];
	first[PROPOSICION_COMPUESTA] = cons(CLLA_ABR | NADA);
	first[PROPOSICION_SELECCION] = cons(CIF | NADA);
	first[PROPOSICION_ITERACION] = cons(CWHILE | NADA);
	first[PROPOSICION_E_S] = cons(CIN | COUT | NADA);
	first[PROPOSICION_RETORNO] = cons(NADA | CRETURN);
	first[VARIABLE] = cons(NADA | CIDENT);
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

int main(int argc, char *argv[]) {

	/* el alumno debera inicializar la variable yyin segun corresponda */

	linea = (char *) malloc(2);
	strcat(linea, "");

	nro_linea = 0;

	if (argc != 3) {
		printf("Argumentos: %d \n", argc);
		error_handler(6);
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

		inic_tablas();

		inic_set();

		scanner();
		unidad_traduccion();

		if (sbol->codigo != CEOF)
			error_handler(41);
		/*Chequemos funcion main*/
		existFuncionMain();
		/*Chequemos que no haya errores, sino imprimos errores*/
		if (cant_errores_x_linea > 0) {
			print();
		}
	}

}

/********* funciones del parser ***********/

void unidad_traduccion(set folset) {
	//TODO test();
	test(first[DECLARACIONES] | folset, folset);
	while (sbol->codigo == CVOID || sbol->codigo == CCHAR
			|| sbol->codigo == CINT || sbol->codigo == CFLOAT)
		declaraciones(folset | first[DECLARACIONES]);
}

void declaraciones(set folset) {
	especificador_tipo(folset | CIDENT | first[ESPECIFICADOR_DECLARACION]);
	if (sbol->codigo == CIDENT) {
		strcpy(inf_id->nbre, sbol->lexema);
		strcpy(nbre_func, sbol->lexema);
		scanner();
	} else {
		error_handler(16);
	}
	especificador_declaracion(folset);
}

/**
 * 
 * @return el codigo del tipo del identificador 
 */
long especificador_tipo(set folset) {
	//TODO test();
	test(first[ESPECIFICADOR_TIPO], folset);
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
	//TODO test();
	test(folset);
}

void especificador_declaracion(set folset) {
	//TODO test();
	test(first[DEFINICION_FUNCION] | first[DECLARACION_VARIABLE], folset);
	switch (sbol->codigo) {
	case CPAR_ABR:
		definicion_funcion(folset);
		break;
	case CASIGNAC:
	case CCOR_ABR:
	case CCOMA:
	case CPYCOMA:
		declaracion_variable(folset);
		break;
	default:
		error_handler(18);
		break;
	}

}

void definicion_funcion(set folset) {
	int check_return = checkreturn;

	if (sbol->codigo == CPAR_ABR) {
		inf_id->clase = CLASFUNC;
		scanner();
	} else
		error_handler(19);

	//Revisar esta inicializacion
	ptr_inf_res = &(inf_id->desc.part_var.sub.ptr_inf_res);
	ptr_cant_params = &(inf_id->desc.part_var.sub.cant_par);
	// ptr_inf_res = NULL;
	(*ptr_cant_params) = 0;
	insertarTS();
	pushTB(); //nueevo bloque de la funcion
	pushie_func = 1;

	if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT
			|| sbol->codigo == CFLOAT) {
		lista_declaraciones_param(
				folset | CCOMA | first[PROPOSICION_COMPUESTA]);
	}
	if (sbol->codigo == CPAR_CIE)
		scanner();
	else
		error_handler(20);

	proposicion_compuesta(folset);

	if (strcmp("main", nbre_func) == 0) {
		check_return = 0;
	}
	if (check_return && !got_return) {
		error_handler(37);
		print();
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
	test(first[DECLARACION_PARAMETRO],
			folset | CAMPER | CIDENT | CLLA_ABR | CLLA_CIE);
	especificador_tipo(folset | CAMPER | CIDENT | CLLA_ABR | CLLA_CIE);
	int es_referencia = 0;
	//int es_arreglo = 0;

	ptr_inf_res = (tipo_inf_res *) calloc(1, sizeof(tipo_inf_res));
	if (ptr_inf_res == NULL) {
		error_handler(10);
		exit(1);
	}
	if (sbol->codigo == CAMPER) {
		ptr_inf_res->tipo_pje = 'r';
		scanner();
	} else {
		ptr_inf_res->tipo_pje = 'v';
	}

	if (sbol->codigo == CAMPER) {
		es_referencia = 1;
		scanner();
	}

	if (sbol->codigo == CIDENT) {
		strcpy(inf_id->nbre, sbol->lexema);
		scanner();
	} else
		error_handler(16);

	inf_id->clase = CLASPAR;
	ptr_inf_res->ptero_tipo = inf_id->ptr_tipo;
	if (sbol->codigo == CCOR_ABR) {
		//controla que no ocurra un parametro como &ident[]
		if (ptr_inf_res->tipo_pje != 'r') {
			scanner();
			if (sbol->codigo == CCOR_CIE) {
				scanner();
			} else {
				error_handler(21);
			}
			inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
			inf_id->ptr_tipo = en_tabla("array");
			inf_id->desc.part_var.arr.cant_elem = 0;
		} else {
			//printf("no se permite pasaje por referncia de un arreglo");
			error_handler(42);
		}
	}
	//ptr_inf_res= &ptr_inf_res->ptr_sig;
	ptr_inf_res = ptr_inf_res->ptr_sig;
	(*ptr_cant_params)++;
	ptr_inf_res = NULL;
	insertarTS();
	test(folset);

}

void lista_declaraciones_init(set folset) {
//TODO test
	test(first[LISTA_DECLARACIONES_INIT],
			folset | first[DECLARADOR_INIT] | CCOMA | CIDENT);

	if (sbol->codigo == CIDENT) {
		strcpy(inf_id->nbre, sbol->lexema);
		inf_id->clase = CLASVAR;
		scanner();
	} else
		error_handler(16);

	declarador_init(folset | (CCOMA | CIDENT) | first[DECLARADOR_INIT]);
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

		declarador_init(folset | (CCOMA | CIDENT) | first[DECLARADOR_INIT]);
		insertarEnTSVariable();
	}
}

void declaracion_variable(set folset) {

	declarador_init(folset | CCOMA | CPYCOMA | first[LISTA_DECLARACIONES_INIT]);
	insertarEnTSVariable();

	if (sbol->codigo == CCOMA
			|| in(sbol->codigo, first[LISTA_DECLARACIONES_INIT])) {

		if (sbol->codigo == CCOMA) {
			scanner();
		} else {
			error_handler(75);
		}

		lista_declaraciones_init(folset | CPYCOMA);
	}

	if (sbol->codigo == CPYCOMA)
		scanner();
	else
		error_handler(22);

	//TODO test();
	test(folset);

}

void declarador_init(set folset) {
	test(
			first[DECLARADOR_INIT] | folset,
			folset | first[CONSTANTE] | CLLA_ABR | CLLA_CIE | CASIGNAC
					| CCOR_CIE | CCOR_ABR | first[LISTA_INICIALIZADORES]);
	es_arreglo = 0;
	switch (sbol->codigo) {
	case CASIGNAC: {
		scanner();
		constante(
				folset | CLLA_ABR | CLLA_CIE | CASIGNAC | CCOR_CIE | CCOR_ABR
						| first[LISTA_INICIALIZADORES]);
		break;
	}
	case CCOR_ABR: {
		scanner();
		es_arreglo = 1;
		if (sbol->codigo == CCONS_ENT) {
			constante(
					folset | CLLA_ABR | CLLA_CIE | CASIGNAC | CCOR_CIE
							| first[LISTA_INICIALIZADORES]);
		} else {
			error_handler(38);
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

			lista_inicializadores(folset | CLLA_CIE);

			if (sbol->codigo == CLLA_CIE)
				scanner();
			else
				error_handler(24);

		}

		break;
	}
	}

	test(folset);
}

void lista_inicializadores(set folset) {

	constante(folset | CCOMA | first[CONSTANTE]);

	while (sbol->codigo == CCOMA | in(sbol->codigo, first[CONSTANTE])) {

		if (sbol->codigo == CCOMA) {
			scanner();
		} else {
			error_handler(75);
		}

		constante(folset | CCOMA | first[CONSTANTE]);
	}

}

void proposicion_compuesta(set folset) {
	test(
			first[PROPOSICION_COMPUESTA],
			folset | first[LISTA_DECLARACIONES]
					| first[LISTA_PROPOSICIONES] | CLLA_CIE);

	if (!pushie_func) {
		pushTB();
	} else {
		pushie_func = 0;
	}
	if (sbol->codigo == CLLA_ABR) {
		scanner();
	} else {
		error_handler(23);
	}

	if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT
			|| sbol->codigo == CFLOAT)

		lista_declaraciones(folset | first[LISTA_PROPOSICIONES] | CLLA_CIE);

	if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS
			|| sbol->codigo == CMENOS || sbol->codigo == CIDENT
			|| sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
			|| sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
			|| sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR
			|| sbol->codigo == CIF || sbol->codigo == CWHILE
			|| sbol->codigo == CIN || sbol->codigo == COUT
			|| sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

		lista_proposiciones(folset | CLLA_CIE);

	if (sbol->codigo == CLLA_CIE)
		scanner();
	else
		error_handler(24);

	pop_nivel();
	test(folset);

}

void lista_declaraciones(set folset) {

	declaracion(folset | first[DECLARACION]);

	while (sbol->codigo == CVOID || sbol->codigo == CCHAR
			|| sbol->codigo == CINT || sbol->codigo == CFLOAT)

		declaracion(folset | first[DECLARACION]);

}

void declaracion(set folset) {
	test(first[DECLARACION],
			folset | first[LISTA_DECLARACIONES_INIT] | CPYCOMA);
	especificador_tipo(folset | first[LISTA_DECLARACIONES_INIT] | CPYCOMA);

	lista_declaraciones_init(folset | CPYCOMA);

	if (sbol->codigo == CPYCOMA)
		scanner();
	else
		error_handler(22);

	test(folset);

}

void lista_proposiciones(set folset) {

	proposicion(folset | first[PROPOSICION]);

	while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS
			|| sbol->codigo == CMENOS || sbol->codigo == CIDENT
			|| sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
			|| sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
			|| sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR
			|| sbol->codigo == CIF || sbol->codigo == CWHILE
			|| sbol->codigo == CIN || sbol->codigo == COUT
			|| sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

		proposicion(folset | first[PROPOSICION]);

}

void proposicion(set folset) {

	test(
			first[PROPOSICION],
			folset | first[PROPOSICION_EXPRESION] | first[PROPOSICION_COMPUESTA]
					| first[PROPOSICION_SELECCION]
					| first[PROPOSICION_ITERACION] | first[PROPOSICION_E_S]
					| first[PROPOSICION_RETORNO]);
	switch (sbol->codigo) {
	case CLLA_ABR:
		proposicion_compuesta(folset);
		break;
	case CWHILE:
		proposicion_iteracion(folset);
		break;
	case CIF:
		proposicion_seleccion(folset);
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
		proposicion_retorno(folset);
		got_return = 1;
		break;
	default:
		error_handler(25);
		break;
	}
}

void proposicion_iteracion(set folset) {

	if (sbol->codigo == CWHILE)
		scanner();
	else
		error_handler(26);

	if (sbol->codigo == CPAR_ABR)
		scanner();
	else
		error_handler(19);

	expresion(folset | CPAR_CIE | first[PROPOSICION]);

	if (sbol->codigo == CPAR_CIE)
		scanner();
	else
		error_handler(20);

	proposicion(folset);

}

void proposicion_seleccion(set folset) {

	if (sbol->codigo == CIF)
		scanner();
	else
		error_handler(27);

	if (sbol->codigo == CPAR_ABR)
		scanner();
	else
		error_handler(19);

	expresion(folset | CPAR_CIE | first[PROPOSICION] | CELSE);

	if (sbol->codigo == CPAR_CIE)
		scanner();
	else
		error_handler(20);

	proposicion(folset | first[PROPOSICION] | CELSE);

	if (sbol->codigo == CELSE) {
		scanner();

		proposicion(folset);
	}

}

void proposicion_e_s(set folset) {

	switch (sbol->codigo) {
	case CIN: {
		scanner();
		if (sbol->codigo == CSHR)
			scanner();
		else
			error_handler(28);
		variable(folset | CSHR | first[VARIABLE] | CPYCOMA);
		while (sbol->codigo == CSHR || in(sbol->codigo, first[VARIABLE])) {
			if (sbol->codigo == CSHR) {
				scanner();
			} else
				error_handler(28);

			variable(folset | CSHR | first[VARIABLE] | CPYCOMA);
		}
		if (sbol->codigo == CPYCOMA)
			scanner();
		else
			error_handler(22);
		break;
	}
	case COUT: {
		scanner();
		if (sbol->codigo == CSHL)
			scanner();
		else
			error_handler(29);
		expresion();
		while (sbol->codigo == CSHL) {
			scanner();
			expresion();
		}
		if (sbol->codigo == CPYCOMA)
			scanner();
		else
			error_handler(22);
		break;
	}
	default:
		error_handler(30);
	}
}

void proposicion_retorno(set folset) {

	scanner();
	expresion();
	if (sbol->codigo == CPYCOMA)
		scanner();
	else
		error_handler(22);

}

void proposicion_expresion(set folset) {

	if (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == CIDENT
			|| sbol->codigo == CPAR_ABR || sbol->codigo == CNEG
			|| sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO
			|| sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

		expresion();

	if (sbol->codigo == CPYCOMA)
		scanner();
	else
		error_handler(22);
}

void expresion(set folset) {

	expresion_simple();

	switch (sbol->codigo) {
	case CASIGNAC: {
		scanner();
		expresion();
		break;
	}
	case CDISTINTO:
	case CIGUAL:
	case CMENOR:
	case CMEIG:
	case CMAYOR:
	case CMAIG: {
		scanner();
		expresion();
		break;
	}
	}
}

void expresion_simple(set folset) {

	if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
		scanner();
	}

	termino();

	while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR) {
		scanner();
		termino();
	}

}

void termino(set folset) {

	factor();

	while (sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND) {
		scanner();
		factor();
	}

}

void factor(set folset) {

	switch (sbol->codigo) {
	case CIDENT: {
		char token_actual[TAM_LEXEMA];
		strcpy(token_actual, sbol->lexema);
		switch (Clase_Ident(token_actual)) {
		case CLASFUNC:
			llamada_funcion();
			break;
		case CLASPAR:
		case CLASVAR:
			variable();
			break;
		default:
			//Identificador no declarado
			error_handler(33);
			scanner();
			if (sbol->codigo == CCOR_ABR) { //es arreglo
				scanner();
				expresion();
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
					lista_expresiones();
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
		constante();
		break;
	case CCONS_STR:
		scanner();
		break;
	case CPAR_ABR: {
		scanner();
		expresion();
		if (sbol->codigo == CPAR_CIE)
			scanner();
		else
			error_handler(20);
		break;
	}
	case CNEG: {
		scanner();
		expresion();
		break;
	}
	default:
		error_handler(31);
	}

}

void variable(set folset) {
	char ident_actual[TAM_LEXEMA];
	if (sbol->codigo == CIDENT) {
		strcpy(ident_actual, sbol->lexema);
		scanner();
	} else
		error_handler(16);

	/* el alumno debera verificar con una consulta a TS
	 si, siendo la variable un arreglo, corresponde o no
	 verificar la presencia del subindice */
	if (ts[en_tabla(ident_actual)].ets->ptr_tipo == en_tabla("array")) {
		if (sbol->codigo == CCOR_ABR) {
			if (es_parametro
					&& (sbol->codigo == CCOMA || sbol->codigo == CPAR_CIE)) {
				error_handler(43);
			}
			scanner();
			expresion();
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
			expresion();
			if (sbol->codigo == CCOR_CIE)
				scanner();
		}
	}

}

void llamada_funcion(set folset) {

	if (sbol->codigo == CIDENT) {
		scanner();
	} else {
		error_handler(16);
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
		lista_expresiones();
	}

	if (sbol->codigo == CPAR_CIE) {
		scanner();
	} else {
		error_handler(20);
	}

}

void lista_expresiones(set folset) {
	es_parametro = 1;
	expresion();

	while (sbol->codigo == CCOMA) {
		scanner();

		expresion();
	}
	es_parametro = 0;
}

void constante(set folset) {
	char tmp[TAM_LEXEMA];
	switch (sbol->codigo) {
	case CCONS_ENT:
		const_int = atoi(sbol->lexema);
		scanner();
		break;
	case CCONS_FLO:
		const_float = atof(sbol->lexema);
		scanner();
		break;
	case CCONS_CAR:
		strcat(tmp, "\"");
		strcat(tmp, sbol->lexema);
		strcat(tmp, "\"");
		strcpy(sbol->lexema, tmp);
		strcpy(const_char, sbol->lexema);
		scanner();
		break;
	default:
		//scanner();
		break;/*f_error(); aca va f_error, faltan los algoritmos de conversion a las constantes numericas. */
	}

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
			inf_id->cant_byte = ts[inf_id->ptr_tipo].ets->cant_byte
					* tam_arreglo;
			inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
			inf_id->desc.part_var.arr.cant_elem = tam_arreglo;

		} else {
			inf_id->ptr_tipo = tipo_id;
			inf_id->cant_byte = ts[inf_id->ptr_tipo].ets->cant_byte;
		}
		insertarTS();
	}
}
