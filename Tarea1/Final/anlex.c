/*********** Inclusión de cabecera **************/
#include "anlex.h"

/************* Variables globales **************/

int consumir = 1;            // Indica al analizador léxico que debe devolver el siguiente componente léxico
char lexema[TAMLEX];         // Utilizado por el analizador léxico
token t;                     // Token global para recibir componentes del Analizador Léxico

FILE *archivo;               // Archivo JSON de entrada
char buff[TAMBUFF];          // Buffer para lectura de archivo JSON
int numLinea = 1;            // Número de línea

/**************** Funciones **********************/

// Rutinas del analizador léxico

void error(const char *mensaje) {
    printf("Lín %d: Error léxico. %s.\n", numLinea, mensaje);    
}

void getToken() {
    int i = 0;
    char c = 0;

    while ((c = fgetc(archivo)) != EOF) {
        if (c == ' ' || c == '\t') {
            continue; // Eliminar espacios en blanco
        } else if (c == '\n') {
            numLinea++; // Incrementar el número de línea
            continue;
        } else if (c == '{') {
            t.compLex = L_LLAVE;
            strcpy(t.lexema, "{");
            break;
        } else if (c == '}') {
            t.compLex = R_LLAVE;
            strcpy(t.lexema, "}");
            break;
        } else if (c == '[') {
            t.compLex = L_CORCHETE;
            strcpy(t.lexema, "[");
            break;
        } else if (c == ']') {
            t.compLex = R_CORCHETE;
            strcpy(t.lexema, "]");
            break;
        } else if (c == ':') {
            t.compLex = DOS_PUNTOS;
            strcpy(t.lexema, ":");
            break;
        } else if (c == ',') {
            t.compLex = COMA;
            strcpy(t.lexema, ",");
            break;
        } else if (c == '"') {
            // Es una cadena (string)
            i = 0;
            lexema[i] = c;
            i++;
            do {
                c = fgetc(archivo);
                lexema[i] = c;
                i++;
            } while (c != '"' && i < TAMLEX - 1);
            lexema[i] = '\0';
            strcpy(t.lexema, lexema);
            t.compLex = LITERAL_CADENA;
            break;
        } else if (isdigit(c)) {
            // Es un número
            i = 0;
            lexema[i] = c;
            i++;
            while (isdigit(c = fgetc(archivo)) || c == '.') {
                lexema[i] = c;
                i++;
            }
            lexema[i] = '\0';
            strcpy(t.lexema, lexema);
            t.compLex = LITERAL_NUM;
            ungetc(c, archivo); // Devolver último caracter leído (no numérico) al buffer
            break;
        } else if (c == 't' || c == 'f' || c == 'n') {
            // Puede ser true, false o null
            i = 0;
            lexema[i] = c;
            i++;
            while (isalpha(c = fgetc(archivo)) && i < TAMLEX - 1) {
                lexema[i] = c;
                i++;
            }
            lexema[i] = '\0';
            ungetc(c, archivo); // Devolver último caracter leído (no alfabético) al buffer
            if (strcmp(lexema, "true") == 0) {
                t.compLex = PR_TRUE;
            } else if (strcmp(lexema, "false") == 0) {
                t.compLex = PR_FALSE;
            } else if (strcmp(lexema, "null") == 0) {
                t.compLex = PR_NULL;
            } else {
                error("Token inválido");
            }
            strcpy(t.lexema, lexema);
            break;
        } else {
            error("Caracter inválido");
            break;
        }
    }

    if (c == EOF) {
        t.compLex = EOF_JSON;
        strcpy(t.lexema, "EOF");
    }
}
/*
void imprimirToken(int compLex) {
    switch (compLex) {
        case L_LLAVE:
        case R_LLAVE:
        case L_CORCHETE:
        case R_CORCHETE:
        case DOS_PUNTOS:
        case COMA:
        case LITERAL_CADENA:
        case LITERAL_NUM:
        case PR_TRUE:
        case PR_FALSE:
        case PR_NULL:
        case EOF_JSON:
            printf("%s ", tokens_json[compLex - L_LLAVE]);
            break;
        default:
            printf("DESCONOCIDO ");
            break;
    }
}*/

int main(int argc, char *args[]) {
    // Inicializar analizador léxico
    initTabla();
    initTablaSimbolos();
    
    if (argc > 1) {
        if (!(archivo = fopen(args[1], "rt"))) {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        
        while (t.compLex != EOF_JSON) {
            getToken();
            imprimirToken(t.compLex);
        }
        
        fclose(archivo);
    } else {
        printf("Debe pasar como parámetro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
