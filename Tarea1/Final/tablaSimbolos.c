#include "anlex.h"

/********************* HASH **********************/
entrada *tabla;             // declarar la tabla de símbolos
int tamTabla = TAMHASH;     // tamaño inicial de la tabla
int elems = 0;              // cantidad de elementos en la tabla

// Función hash para calcular la posición en la tabla
int h(const char *k, int m) {
    unsigned h = 0, g;
    int i;
    for (i = 0; i < strlen(k); i++) {
        h = (h << 4) + k[i];
        if ((g = h & 0xf0000000)) {
            h ^= (g >> 24);
            h ^= g;
        }
    }
    return h % m;
}

// Inicializar la tabla de símbolos
void initTabla() {
    int i;
    
    tabla = (entrada *)malloc(tamTabla * sizeof(entrada));
    for (i = 0; i < tamTabla; i++) {
        tabla[i].compLex = -1;  // marca la entrada como vacía
    }
}

// Verificar si un número es primo
int esprimo(int n) {
    int i;
    for (i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

// Encontrar el siguiente número primo después de n
int siguiente_primo(int n) {
    if (n % 2 == 0)
        n++;
    for (; !esprimo(n); n += 2);
    return n;
}

// Rehashing: duplicar el tamaño de la tabla
void rehash() {
    entrada *vieja;
    int i;
    
    vieja = tabla;
    tamTabla = siguiente_primo(2 * tamTabla);
    initTabla();
    for (i = 0; i < tamTabla / 2; i++) {
        if (vieja[i].compLex != -1)
            insertar(vieja[i]);
    }
    free(vieja);
}

// Insertar una entrada en la tabla de símbolos
void insertar(entrada e) {
    int pos;
    
    if (++elems >= tamTabla / 2)
        rehash();
    
    pos = h(e.lexema, tamTabla);
    while (tabla[pos].compLex != -1) {
        pos++;
        if (pos == tamTabla)
            pos = 0;
    }
    tabla[pos] = e;
}

// Buscar una clave en la tabla de símbolos
entrada *buscar(const char *clave) {
    int pos;
    
    pos = h(clave, tamTabla);
    while (tabla[pos].compLex != -1 && strcmp(tabla[pos].lexema, clave) != 0) {
        pos++;
        if (pos == tamTabla)
            pos = 0;
    }
    return &tabla[pos];
}

// Insertar tokens y palabras clave de JSON en la tabla de símbolos
void insertarTokensJSON() {
    const char *tokens[] = {
        "{", "}", "[", "]", ":", ",", "true", "false", "null"
    };
    int i, compLex;
    entrada e;
    
    for (i = 0; i < sizeof(tokens) / sizeof(tokens[0]); i++) {
        if (strcmp(tokens[i], "{") == 0) {
            compLex = L_LLAVE;
        } else if (strcmp(tokens[i], "}") == 0) {
            compLex = R_LLAVE;
        } else if (strcmp(tokens[i], "[") == 0) {
            compLex = L_CORCHETE;
        } else if (strcmp(tokens[i], "]") == 0) {
            compLex = R_CORCHETE;
        } else if (strcmp(tokens[i], ":") == 0) {
            compLex = DOS_PUNTOS;
        } else if (strcmp(tokens[i], ",") == 0) {
            compLex = COMA;
        } else if (strcmp(tokens[i], "true") == 0) {
            compLex = PR_TRUE;
        } else if (strcmp(tokens[i], "false") == 0) {
            compLex = PR_FALSE;
        } else if (strcmp(tokens[i], "null") == 0) {
            compLex = PR_NULL;
        }
        
        strcpy(e.lexema, tokens[i]);
        e.compLex = compLex;
        insertar(e);
    }
}

// Inicializar la tabla de símbolos con tokens de JSON
void initTablaSimbolos() {
    initTabla();
    insertarTokensJSON();
}

