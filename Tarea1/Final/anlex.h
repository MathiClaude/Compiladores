/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/

// Codigos de componentes léxicos para JSON
#define L_LLAVE         301   // {
#define R_LLAVE         302   // }
#define L_CORCHETE      303   // [
#define R_CORCHETE      304   // ]
#define DOS_PUNTOS      305   // :
#define COMA            306   // ,
#define LITERAL_CADENA  307   // "string"
#define LITERAL_NUM     308   // number
#define PR_TRUE         309   // true
#define PR_FALSE        310   // false
#define PR_NULL         311   // null
#define EOF_JSON        312   // Fin de archivo JSON
// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX  	50   // Tamaño máximo de un lexema
#define TAMHASH 	101  // Tamaño de la tabla hash para la tabla de símbolos

/************* Estructuras ********************/

typedef struct entrada {
    int compLex;
    char lexema[TAMLEX];
    // Pueden añadirse más atributos según sea necesario para JSON
} entrada;

typedef struct {
    int compLex;
    char lexema[TAMLEX];
} token;

/************* Prototipos ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void getToken();
void error(const char *mensaje);

/************* Constantes de Token para JSON ********************/
const char *tokens_json[] = {
    "L_LLAVE",
    "R_LLAVE",
    "L_CORCHETE",
    "R_CORCHETE",
    "DOS_PUNTOS",
    "COMA",
    "LITERAL_CADENA",
    "LITERAL_NUM",
    "PR_TRUE",
    "PR_FALSE",
    "PR_NULL",
    "EOF_JSON"
};

/************* Función para imprimir tokens ********************/
void imprimirToken(int compLex) {
    if (compLex >= L_LLAVE && compLex <= EOF_JSON) {
        printf("%s ", tokens_json[compLex - L_LLAVE]);
    } else {
        printf("DESCONOCIDO ");
    }
}
