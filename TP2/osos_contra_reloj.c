#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <time.h>
#include "utiles.h"
#include "osos_contra_reloj.h"

#define MAX_OBSTACULOS 600
#define MAX_HERRAMIENTAS 600

#define ALTO 20
#define ANCHO 30

const char VACIO = '.';
const char ARBOL = 'A', PIEDRA = 'R', KOALAS = 'K'; 
const int MAX_ARBOLES = 350, MAX_PIEDRAS = 80, CANTIDAD_KOALA = 1;

const char LINTERNA = 'L', VELA = 'V', BENGALA = 'E', PILA = 'B';
const int MAX_VELAS = 30, MAX_BENGALAS = 10, MAX_PILAS = 30;

const int MOCHILA_VELAS = 4, MOCHILA_VELAS_POLAR = 6, MOCHILA_BENGALAS_PANDA = 2, MOCHILA_LINTERNA = 1;
const int LINTERNA_MOVIMIENTOS = 10, LINTERNA_MOVIMIENTOS_PARDO = 15, VELA_MOVIMIENTOS = 5, BENGALA_MOVIMIENTOS = 3, PILA_MOVIMIENTOS = 5;

static const char POLAR = 'I', PARDO ='G', PANDA = 'P', CHLOE = 'C';

static const int ESTA_JUGANDO = 0, JUEGO_TERMINADO = -1;

const double TIEMPO_POR_ARBOL_PARDO = 0.5, TIEMPO_POR_PIEDRA = 2, TIEMPO_PANDA_VE_CHLOE = 30;

const int DISTANCIA_MANHATTAN = 3;

const int NINGUNA_HERRAMIENTA_EN_USO = -1;

/*Movimientos*/
#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'
#define ENCENDER_LINTERNA 'L'
#define ENCENDER_VELA 'V'
#define ENCENDER_BENGALA 'E'
#define TIEMPO_RESTANTE 'T'

#define LINTERNA_S "\U0001f526"
#define VELA_S "\U0001f56f\uFE0F"
#define BENGALA_S "\U0001f387"
#define TIEMPO_RESTANTE_S "\u23F3"
#define CHLOE_S "\U0001f467"

/*
* PRE: Recibe dos coordenadas validas.
* POST: Devuelve TRUE si las coordenadas son iguales y FALSE, en caso contrario.
*/
bool posiciones_iguales (coordenada_t posicion_uno, coordenada_t posicion_dos) {
    return (posicion_uno.fil == posicion_dos.fil && posicion_uno.col == posicion_dos.col);
}

/*
* PRE: Recibe elementos que el oso puede llevar en su mochila LINTERNA, VELAS y BENGALAS, con sus cantidades correspondientes.
* POST: Guarda en el vector mochila las herramientas con las que el oso comienza el juego.
*/
void guardar_elemento (elemento_mochila_t mochila[MAX_HERRAMIENTAS], int* tope_herramientas, int numero_de_herramientas, char tipo_de_herramienta, int movimientos) {
    int tope_inicial = (*tope_herramientas);
    (*tope_herramientas) = (*tope_herramientas) + numero_de_herramientas;
    
    for (int n = tope_inicial; n < (*tope_herramientas); n++) {
        mochila[n].tipo = tipo_de_herramienta;
        mochila[n].movimientos_restantes = movimientos; 
    }
}

/*
* PRE: Recibe un oso de tipo personaje_t previamente inicializado.
* POST: Segun el tipo de personaje, carga la mochila con sus herramientas iniciales.
*/
void cargar_mochila (personaje_t* oso) {
    if (oso->tipo == POLAR) {
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_LINTERNA, LINTERNA, LINTERNA_MOVIMIENTOS);
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_VELAS_POLAR, VELA, VELA_MOVIMIENTOS);

    } else if (oso->tipo == PARDO) {
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_LINTERNA, LINTERNA, LINTERNA_MOVIMIENTOS_PARDO);
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_VELAS, VELA, VELA_MOVIMIENTOS);

    } else {
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_LINTERNA, LINTERNA, LINTERNA_MOVIMIENTOS);
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_VELAS, VELA, VELA_MOVIMIENTOS);
        guardar_elemento(oso->mochila, &oso->cantidad_elementos, MOCHILA_BENGALAS_PANDA, BENGALA, BENGALA_MOVIMIENTOS);
    }
}

/*
* PRE: Recibe las coordenadas del oso sin inicializar.
* POST: Asigna coordenadas válidas para el oso, una fila aleatoria y la columna 0.
*/
void posicionar_oso (coordenada_t* oso) {
    oso->fil = rand () % ALTO;
    oso->col = 0;
}

/*
* PRE: Recibe la posicion de Chloe sin incializar, y la posicion del oso, ya inicializada.
* POST: Asigna una posicion de Chloe aleatoria válida. 
*/
void posicionar_amiga (coordenada_t* chloe, coordenada_t oso) {
    do {
        chloe->fil = rand() % ALTO;
        chloe->col = rand() % ANCHO;
    } while (posiciones_iguales((*chloe), oso));
}

/*
* PRE: Recibe un vector inicializado y un entero (que no necesariamente es el tope del vector)
* POST: Compara posicion a posicion las coordenadas del vector con las coordenadas enviadas y devuelve TRUE si ya hay otra 
*       posicion igual y FALSE, en caso contrario.
*/
bool posicion_es_valida (elemento_del_mapa_t elementos[], int n, coordenada_t coordenada) {
    int i = 0;
    bool hay_otra_posicion_igual = false;

    while (i < n && !hay_otra_posicion_igual) {
        if (posiciones_iguales(elementos[i].posicion, coordenada))
            hay_otra_posicion_igual = true;
        else {
            i++;
        }      
    }
    
    return hay_otra_posicion_igual;
}

/*
* PRE: Recibe un vector de herramientas y su tope, la posicion del oso y la posicion de Chloe, ambas válidas,
*      la cantidad de herramientas previamente establecida y un tipo de herramienta permitido VELA, PILA o BENGALA.
* POST: Asigna posiciones válidas para todo el vector de herramientas.
*/
void posicionar_obstaculos (elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int* tope_obstaculos, coordenada_t oso, 
                            coordenada_t chloe, int numero_de_elementos, char tipo_de_obstaculo) {
    
    int tope_inicial = (*tope_obstaculos);
    (*tope_obstaculos) += numero_de_elementos;
    
    for(int n = tope_inicial; n < (*tope_obstaculos); n++) {
        obstaculos[n].tipo = tipo_de_obstaculo;
        obstaculos[n].visible = false;
        
        do {
            obstaculos[n].posicion.fil = rand() % ALTO;
            obstaculos[n].posicion.col = rand() % ANCHO;      

        } while (posiciones_iguales(obstaculos[n].posicion, oso) || posiciones_iguales(obstaculos[n].posicion, chloe) || 
                posicion_es_valida(obstaculos, n, obstaculos[n].posicion));
    }    
}

/*
* PRE: Recibe un juego con toda su estructura, una cantidad de obstaculos previamente establecida y un tipo de obstaculo permitido 
*      ARBOL, PIEDRA o KOALA.
* POST: Asigna posiciones válidas para todo el vector de obstaculos.
*/
void posicionar_herramientas (juego_t* juego, int numero_de_elementos, char tipo_de_herramientas) {
    
    int tope_inicial = juego->cantidad_herramientas;
    juego->cantidad_herramientas += numero_de_elementos;
    
    for(int n = tope_inicial; n < juego->cantidad_herramientas; n++) {
        juego->herramientas[n].tipo = tipo_de_herramientas;
        juego->herramientas[n].visible = false;
        
        do {
            juego->herramientas[n].posicion.fil = rand() % ALTO;
            juego->herramientas[n].posicion.col = rand() % ANCHO;      

        } while (posiciones_iguales(juego->herramientas[n].posicion, juego->personaje.posicion) || 
                posiciones_iguales(juego->herramientas[n].posicion, juego->amiga_chloe) || 
                posicion_es_valida(juego->herramientas, n, juego->herramientas[n].posicion) ||
                posicion_es_valida(juego->obstaculos, juego->cantidad_obstaculos, juego->herramientas[n].posicion));
    }
}

/*
 * PRE: Recibe un juego sin inicializar, y un tipo de personaje válido PANDA, POLAR o PARDO.
 * POST: Inicializa el juego, cargando toda la información inicial y los datos del personaje, según corresponda. 
 */
void inicializar_juego (juego_t* juego, char tipo_personaje) {
    juego->cantidad_obstaculos = 0;
    juego->cantidad_herramientas = 0;
    juego->chloe_visible = false;
    juego->personaje.tipo = tipo_personaje;
    juego->personaje.cantidad_elementos = 0;
    juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;
    juego->personaje.tiempo_perdido = 0;

    cargar_mochila(&juego->personaje);

    posicionar_oso(&juego->personaje.posicion);
    posicionar_amiga(&juego->amiga_chloe, juego->personaje.posicion);

    posicionar_obstaculos(juego->obstaculos, &juego->cantidad_obstaculos, juego->personaje.posicion, juego->amiga_chloe, MAX_ARBOLES, ARBOL);
    posicionar_obstaculos(juego->obstaculos, &juego->cantidad_obstaculos, juego->personaje.posicion, juego->amiga_chloe, MAX_PIEDRAS, PIEDRA);
    posicionar_obstaculos(juego->obstaculos, &juego->cantidad_obstaculos, juego->personaje.posicion, juego->amiga_chloe, CANTIDAD_KOALA, KOALAS);
    
    posicionar_herramientas(juego, MAX_VELAS, VELA);
    posicionar_herramientas(juego, MAX_BENGALAS, BENGALA);
    posicionar_herramientas(juego, MAX_PILAS, PILA);
    

}

/*
* PRE: Recibe un juego con todas sus estructuras válidas.
 * POST: El juego se dará por terminado, si el personaje encontró a Chloe, devolverá JUEGO_TERMINADO, en caso contrario devuelve
 * ESTA_JUGANDO.
 */
int estado_juego (juego_t juego){
    int estado = ESTA_JUGANDO; 

    if (posiciones_iguales(juego.personaje.posicion, juego.amiga_chloe)) {
        estado = JUEGO_TERMINADO;
    }

    return estado;
}

/*
* PRE: Recibe dos vectores uno de obstaculos y uno de herramientas previamentes inicializados con sus respectivos topes y un booleano 
*      que indica si la posicion de Chloe es visible o no.
* POST: Estable como no visibles todas las herramientas, obstaculos y a Chloe que pudieron ser visibles previamente.
*/
void borrar_iluminado (elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos, elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], 
                       int tope_herramientas, bool* chloe_visible) {
    
    for (int n = 0; n < tope_obstaculos; n++){
        obstaculos[n].visible = false;
    }

    for (int k = 0; k < tope_herramientas; k++) {
        herramientas[k].visible = false;
    }

    (*chloe_visible) = false;
}

/*
* PRE: Recibe una coordenada valida del oso y una jugada valida que puede ser ARRIBA, IZQUIERDA, ABAJO o DERECHA
* POST: Desplaza al oso según la jugada recibida, y guarda el ultimo movimiento que este realizó.
*/
void desplazar_oso (coordenada_t* oso, char* ultimo_movimiento, char jugada) {
    switch (jugada) {
    
    case ARRIBA:
        if (oso->fil - 1 >= 0) {
            oso->fil --;
            (*ultimo_movimiento) = ARRIBA;
        }
        break;

    case IZQUIERDA:
        if (oso->col -1 >= 0) {
            oso->col --;
            (*ultimo_movimiento) = IZQUIERDA;
        }
        break;

    case ABAJO:
        if (oso->fil +1 < ALTO) {
            oso->fil ++;
            (*ultimo_movimiento) = ABAJO;
        }
        break;

    case DERECHA:
        if (oso->col +1 < ANCHO) {
            oso->col ++;
            (*ultimo_movimiento) = DERECHA;
        }
        break;
    }   
}

/*
* PRE: Recibe las coordenadas del oso y un vector de elementos del mapa (herramientas u obstaculos), con su respectivo tope, previamente
*      inicializados.
* POST: Devuelve TRUE, si las coordenadas del oso, coinciden con la posicion de un elemntento del vector y FALSE, en caso contrario.
*/
bool oso_encima_de (coordenada_t oso, elemento_del_mapa_t elementos[], int tope_elementos) {
    int i = 0;
    bool oso_sobre_elemento = false;

    while (i < tope_elementos && !oso_sobre_elemento) {
        if (posiciones_iguales(oso, elementos[i].posicion)) {
            oso_sobre_elemento = true;
        } else {
            i++;
        }
    }
    
    return oso_sobre_elemento;
}

/*
* PRE: Recibe un vector de herramientas previamente cargado con su tope y una posicion válida de la herramienta en el vector mismo.
* POST: Elimina del mapa la herramienta que se encuentra en la posicion indicada.
*/
void eliminar_herramienta_mapa (elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int* cantidad_herramientas, int posicion) {
    herramientas[posicion]= herramientas[(*cantidad_herramientas) - 1];
    (*cantidad_herramientas)--;
}

/*
* PRE: Recibe un personaje con toda su estructura previamente inicializada, un vector herramientas, tambien previamente inicializado
*      y su respectivo tope.
* POST: Toma la herramienta del bosque y la guarda en la mochila del personaje.
*/
void agarrar_herramienta (personaje_t* oso, elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int* tope_herramientas) {
    int n = 0;
    bool encontre_linterna = false;
    
    for (int i=0; i < (*tope_herramientas); i++) {
        if (posiciones_iguales(herramientas[i].posicion, oso->posicion)) {
            
            if (herramientas[i].tipo == PILA) {
                while (n < oso->cantidad_elementos && !encontre_linterna){
                    if (oso->mochila[n].tipo == LINTERNA) {
                        oso->mochila[n].movimientos_restantes = oso->mochila[n].movimientos_restantes + PILA_MOVIMIENTOS;
                        eliminar_herramienta_mapa(herramientas, tope_herramientas, i);
                        encontre_linterna = true;
                    } else {
                        n++;
                    }
                }
            
            } else {
                if (herramientas[i].tipo == VELA) {
                    guardar_elemento(oso->mochila, &oso->cantidad_elementos, 1, VELA, VELA_MOVIMIENTOS);
                } else if (herramientas[i].tipo == BENGALA) {
                    guardar_elemento(oso->mochila, &oso->cantidad_elementos, 1, BENGALA, BENGALA_MOVIMIENTOS);
                }

                eliminar_herramienta_mapa(herramientas, tope_herramientas, i);
            }
        }
    }

}

/*
* PRE: Recibe una posicion válida del oso y Chloe, ademas del vector de obstaculos previamente inicializado y su respectivo tope 
* POST: Se fija si el personaje se encuentra en la misma posicion que un koala y modifica su posicion enviandolo a la columna 0 
*       y una fila aleatoria.
*/
void sobre_koala (coordenada_t* oso, coordenada_t chloe, elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos) {
    for (int i = 0; i < tope_obstaculos ; i++) {
        if (posiciones_iguales((*oso), obstaculos[i].posicion)){
            if (obstaculos[i].tipo == KOALAS){
                posicionar_oso(oso);
            }
        }
    }
}

/*
* PRE: Recibe un tipo de oso valido, PANDA, PARDO o POLAR, una posicion válida del oso, un vector de obstaculos previamente 
*      cargado, con su tope y el tiempo perdido del personaje.
* POST: Modifica el tiempo que perdio el personaje, según cual sea, debido a que se encontro con un obstaculo del tipo ARBOL o PIEDRA.
*/
void perder_tiempo (char tipo_oso, coordenada_t oso, double* tiempo_perdido, elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos) {
    int i = 0;

    while (i < tope_obstaculos && !posiciones_iguales(obstaculos[i].posicion, oso)){
        i++;
    }

    if (obstaculos[i].tipo == ARBOL) {
        if (tipo_oso != PARDO) {
            (*tiempo_perdido) ++;
        } else {
            (*tiempo_perdido) += TIEMPO_POR_ARBOL_PARDO;
        }

    } else if (obstaculos[i].tipo == PIEDRA) {
        if (tipo_oso != POLAR) {
            (*tiempo_perdido) +=  TIEMPO_POR_PIEDRA;
        }
    }
}   

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada.
* POST: Se fija si el oso esta posicionado encima de una herramienta o un obstaculo. Si esta sobre una herramienta la toma del mapa
*       y si esta sobre un obstaculo pierde tiempo cuando es ARBOL o PIEDRA, y si es un KOALA, se modifica la posicion del oso.
*/
void obstaculos_o_herramientas (juego_t* juego) {
    if (oso_encima_de(juego->personaje.posicion, juego->herramientas, juego->cantidad_herramientas)) {
        agarrar_herramienta(&juego->personaje, juego->herramientas, &juego->cantidad_herramientas);

    } else if (oso_encima_de(juego->personaje.posicion, juego->obstaculos, juego->cantidad_obstaculos)){
        sobre_koala(&juego->personaje.posicion, juego->amiga_chloe, juego->obstaculos, juego->cantidad_obstaculos);
        perder_tiempo(juego->personaje.tipo, juego->personaje.posicion, &juego->personaje.tiempo_perdido, juego->obstaculos, juego->cantidad_obstaculos);
    }
}

/*
* PRE: Recibe un juego con toda su estructura inicializada.
* POST: Vuelve visible obstaculos, herramientas y Chloe según en la dirección del ultimo movimiento del personaje.
*/
void linterna_iluminar (juego_t* juego) {
    switch (juego->personaje.ultimo_movimiento) {
    
    case ARRIBA:
        for (int j = 0; j < juego->cantidad_herramientas; j++) {
            if (juego->personaje.posicion.col == juego->herramientas[j].posicion.col && juego->personaje.posicion.fil > juego->herramientas[j].posicion.fil) {
                juego->herramientas[j].visible = true;
            }
        }

        for (int n = 0; n < juego->cantidad_obstaculos; n++) {
            if (juego->personaje.posicion.col == juego->obstaculos[n].posicion.col && juego->personaje.posicion.fil > juego->obstaculos[n].posicion.fil) {
                juego->obstaculos[n].visible = true;
            }
        } 

        if (juego->personaje.posicion.col == juego->amiga_chloe.col && juego->personaje.posicion.fil > juego->amiga_chloe.fil) {
            juego->chloe_visible = true;
        }

        break;

    case IZQUIERDA:
        for (int n = 0; n < juego->cantidad_herramientas; n++) {
            if (juego->personaje.posicion.fil == juego->herramientas[n].posicion.fil && juego->personaje.posicion.col > juego->herramientas[n].posicion.col) {
                juego->herramientas[n].visible = true;
            }
        }

        for (int i = 0; i < juego->cantidad_obstaculos; i++) {
            if (juego->personaje.posicion.fil == juego->obstaculos[i].posicion.fil && juego->personaje.posicion.col > juego->obstaculos[i].posicion.col) {
                juego->obstaculos[i].visible = true;      
            }
        } 

        if (juego->personaje.posicion.fil == juego->amiga_chloe.fil && juego->personaje.posicion.col > juego->amiga_chloe.col) {
            juego->chloe_visible = true;
        }
        break;

    case ABAJO:
        for (int j = 0; j < juego->cantidad_herramientas; j++) {
            if (juego->personaje.posicion.col == juego->herramientas[j].posicion.col && juego->personaje.posicion.fil < juego->herramientas[j].posicion.fil) {
                juego->herramientas[j].visible = true;
            }
        }

        for (int n = 0; n < juego->cantidad_obstaculos; n++) {
            if (juego->personaje.posicion.col == juego->obstaculos[n].posicion.col && juego->personaje.posicion.fil < juego->obstaculos[n].posicion.fil) {
                juego->obstaculos[n].visible = true;
            }
        }

        if (juego->personaje.posicion.col == juego->amiga_chloe.col && juego->personaje.posicion.fil < juego->amiga_chloe.fil) {
            juego->chloe_visible = true;
        }
        break;  

    default:
        for (int n = 0; n < juego->cantidad_herramientas; n++) {
            if (juego->personaje.posicion.fil == juego->herramientas[n].posicion.fil && juego->personaje.posicion.col < juego->herramientas[n].posicion.col) {
                juego->herramientas[n].visible = true;
            }
        }

        for (int i = 0; i < juego->cantidad_obstaculos; i++) {
            if (juego->personaje.posicion.fil == juego->obstaculos[i].posicion.fil && juego->personaje.posicion.col < juego->obstaculos[i].posicion.col) {
                juego->obstaculos[i].visible = true;
            }
        }

        if (juego->personaje.posicion.fil == juego->amiga_chloe.fil && juego->personaje.posicion.col < juego->amiga_chloe.col) {
            juego->chloe_visible = true;
        }
        break;
    }
}

/*
* PRE: Recibe dos enteros
* POST: Devuelve TRUE si el entero elemento coincide con el entero oso, o si se encuentra inmediatamente al lado, FALSE en caso contrario.
*/
bool zona_iluminable_vela (int elemento, int oso) {
    return (elemento == oso || elemento == oso - 1 || elemento == oso + 1);
}

/*
* PRE: Recibe un juego con toda su estructura inicializada.
* POST: Vuelve visible obstaculos, herramientas y a Chloe si se encuentran alrededor del oso.
*/
void vela_iluminar (juego_t* juego) {
    for (int i = 0; i < juego->cantidad_obstaculos; i++) {
        if (zona_iluminable_vela (juego->obstaculos[i].posicion.fil, juego->personaje.posicion.fil) &&
            zona_iluminable_vela (juego->obstaculos[i].posicion.col, juego->personaje.posicion.col)) {
            
            juego->obstaculos[i].visible = true;
        }
    }

    for (int i = 0; i < juego->cantidad_herramientas; i++) {
        if(zona_iluminable_vela (juego->herramientas[i].posicion.fil, juego->personaje.posicion.fil) &&
           zona_iluminable_vela (juego->herramientas[i].posicion.col, juego->personaje.posicion.col)) {
            
            juego->herramientas[i].visible = true;
        }
    }

    if (zona_iluminable_vela (juego->amiga_chloe.fil, juego->personaje.posicion.fil) &&
       zona_iluminable_vela (juego->amiga_chloe.col, juego->personaje.posicion.col)) {
 
        juego->chloe_visible = true;
    }

}

/*
* PRE: Recibe dos posiciones válidas
* POST: Devuelve la distancia manhattan entre dos posiciones.
*/
int distancia_manhattan (coordenada_t posicion_uno, coordenada_t posicion_dos) {
    return (abs(posicion_uno.fil - posicion_dos.fil) + abs(posicion_uno.col - posicion_dos.col));
}

/*
* PRE: Recibe un juego con toda su estructura inicializada
* POST: Vuelve visible obstaculos, herramientas y a Chloe si se encuentran a una DISTANCIA_MANHATTAN menor o igual entre un "centro" 
*       aleatorio y los elementos.
*/
void bengala_iluminar (juego_t* juego) {
    coordenada_t centro_bengala;
    centro_bengala.fil = rand() % ALTO;
    centro_bengala.col = rand() % ANCHO;

    for (int i = 0; i < juego->cantidad_obstaculos; i++) {
        if (distancia_manhattan(centro_bengala, juego->obstaculos[i].posicion) <= DISTANCIA_MANHATTAN){
            juego->obstaculos[i].visible = true;
        }
    }

    for (int j = 0; j < juego->cantidad_herramientas; j++) {
        if (distancia_manhattan(centro_bengala, juego->herramientas[j].posicion) <= DISTANCIA_MANHATTAN) {
            juego->herramientas[j].visible = true;
        }
    }

    if (distancia_manhattan(centro_bengala, juego->amiga_chloe) <= DISTANCIA_MANHATTAN) {
        juego->chloe_visible = true;
    }
}

/*
* PRE: Recibe un vector mochila previamente inicializado, con su respectivo tope y un tipo de herramienta válido VELA, BENGALA
*      o LINTERNA.
* POST: Devuelve la posicion en la que se encuentra la herramienta en uso dentro del vector mochila.
*/
int determinar_herramienta_en_uso (elemento_mochila_t mochila[MAX_HERRAMIENTAS], int tope_mochila, char tipo_herramienta) {
    int n = 0;
    bool encontre_herramienta_en_uso = false;

    while (n < tope_mochila && !encontre_herramienta_en_uso) {
        if (mochila[n].tipo == tipo_herramienta) {
            encontre_herramienta_en_uso = true;
        } else {
            n++;
        }
    }

    if (!encontre_herramienta_en_uso){
        n = NINGUNA_HERRAMIENTA_EN_USO; 
    }

    return n;
}

/*
* PRE: Recibe un vector mochila previamente cargado con su tope y una posicion válida de la herramienta en el vector mochila.
* POST: Elimina de la mochila la herramienta que se encuentra en la posicion indicada.
*/
void eliminar_herramienta_mochila (elemento_mochila_t mochila[MAX_HERRAMIENTAS], int* tope_mochila, int posicion) {
    mochila[posicion] = mochila [(*tope_mochila) -1];
    (*tope_mochila) --;
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada.
* POST: Vuelve visible objetos, herramientas y a Chloe con la bengala, si esta no tiene movimientos disponibles, la saca de uso.
*/
void bengala_en_uso (juego_t* juego) {
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes > 0) {
        bengala_iluminar(juego);
        juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes --;      

    } else {
        eliminar_herramienta_mochila(juego->personaje.mochila, &juego->personaje.cantidad_elementos, juego->personaje.elemento_en_uso);
        juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;  
    }
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada.
* POST: Vuelve visible objetos, herramientas y a Chloe con la linterna, si esta no tiene movimientos disponibles, la saca de uso.
*/
void linterna_en_uso (juego_t* juego) {
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes > 0) {
        linterna_iluminar(juego); 
        juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes --;

    } else {
        juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;
    }
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada.
* POST: Vuelve visible objetos, herramientas y a Chloe con la vela, si esta no tiene movimientos disponibles, la saca de uso.
*/
void vela_en_uso (juego_t* juego) {
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes > 0) {
        vela_iluminar(juego);
        juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes --;

    } else {
        eliminar_herramienta_mochila(juego->personaje.mochila, &juego->personaje.cantidad_elementos, juego->personaje.elemento_en_uso);
        juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;
    }
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada
* POST: Verifica que si la herramienta en uso es LINTERNA, VELA o BENGALA, e ilumina según corresponda.
*/
void herramienta_en_uso (juego_t* juego) {
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA) {
        bengala_en_uso(juego);

    }else if(juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA) {
        linterna_en_uso(juego);

    } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA) {
        vela_en_uso(juego);
    }     
}

/* 
 * Mueve el personaje en la dirección indicada por el usuario o habilita 
 * cualquiera de las herramientas y actualiza el juego según los elementos 
 * que haya en el camino del personaje.
 * El juego quedará en un estado válido al terminar el movimiento. 
 * El movimiento será:
 * -> W: Si el personaje debe moverse para la arriba. 
 * -> A: Si el personaje debe moverse para la izquierda.
 * -> S: Si el personaje debe moverse para la abajo.
 * -> D: Si el personaje debe moverse para la derecha.
 * -> L: Si el personaje quiere encender una linterna.
 * -> V: Si el personaje quiere encender una vela. 
 * -> E: Si el personaje quiere encender la bengala.
 * -> T: Si el personaje quiere ver el tiempo restante.
 * En caso de que querer activar una herramienta, y no tenga mas movimientos, no deberá 
 * activarse ninguna ventaja. 
 * Si se aprieta una tecla de iluminación y esta ya está siendo usada, se desactivará colocando
 * el int elemento_en_uso en -1.
 */
void realizar_jugada(juego_t* juego, char jugada) {
    borrar_iluminado (juego->obstaculos, juego->cantidad_obstaculos, juego->herramientas, juego->cantidad_herramientas, &juego->chloe_visible);

    switch (jugada) {
    
    case ARRIBA:
    case IZQUIERDA:
    case ABAJO:
    case DERECHA:
        desplazar_oso(&juego->personaje.posicion, &juego->personaje.ultimo_movimiento, jugada);
        obstaculos_o_herramientas(juego);

        if (juego->personaje.tipo == PANDA && juego->personaje.tiempo_perdido >= TIEMPO_PANDA_VE_CHLOE) {
            juego->chloe_visible = true;
        }

        herramienta_en_uso(juego);
          
        break;
    
    case ENCENDER_LINTERNA:
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
            bengala_en_uso(juego);
        
        } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA) {
            juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;

        } else {
            posicionar_obstaculos(juego->obstaculos, &juego->cantidad_obstaculos, juego->personaje.posicion, juego->amiga_chloe, CANTIDAD_KOALA, KOALAS);
            juego->personaje.elemento_en_uso = determinar_herramienta_en_uso(juego->personaje.mochila, juego->personaje.cantidad_elementos, LINTERNA);
            linterna_en_uso(juego);
        }

        break;

    case ENCENDER_VELA:
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
            bengala_en_uso(juego);
        
        } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA) {
            juego->personaje.elemento_en_uso = NINGUNA_HERRAMIENTA_EN_USO;

        }else {
            juego->personaje.elemento_en_uso = determinar_herramienta_en_uso(juego->personaje.mochila, juego->personaje.cantidad_elementos, VELA); 
            vela_en_uso(juego);
        }

        break;
    
    case ENCENDER_BENGALA:
        juego->personaje.elemento_en_uso = determinar_herramienta_en_uso(juego->personaje.mochila, juego->personaje.cantidad_elementos, BENGALA);
        
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA) {
            bengala_en_uso(juego);
        }
        break;

    case TIEMPO_RESTANTE:
        printf("Tiempo transcurrido %2.f segundos \n", juego->personaje.tiempo_perdido + tiempo_actual());
        break;
    }
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada y una matriz sin inicializar.
* POST: Carga la matriz con los obstaculos, las herramientas recolectables, Chloe y el oso, si es que estos son visibles, sino con VACIO.
*/
void verificar_visibilidad(juego_t juego, char bosque[ALTO][ANCHO]) {
    for (int i = 0; i < ALTO; i++) {
        for (int j = 0; j < ANCHO; j++) {
            bosque[i][j] = VACIO;
            
            for (int n = 0; n < juego.cantidad_obstaculos; n++) {
                if((i == juego.obstaculos[n].posicion.fil) && (j == juego.obstaculos[n].posicion.col)){
                    if (juego.obstaculos[n].visible == true) {
                        bosque[i][j] = juego.obstaculos[n].tipo;
                    }
                }
            }
            
            for (int n = 0; n < juego.cantidad_herramientas; n++) {
                if((i == juego.herramientas[n].posicion.fil) && (j == juego.herramientas[n].posicion.col)){ 
                    if (juego.herramientas[n].visible == true) {
                        bosque[i][j] = juego.herramientas[n].tipo;
                    }
                }
            }

            if ((i == juego.amiga_chloe.fil) && (j == juego.amiga_chloe.col)){
                if (juego.chloe_visible == true) {
                    bosque[i][j] = CHLOE;
                }
            }
            if ((i == juego.personaje.posicion.fil) && (j == juego.personaje.posicion.col)){
                bosque[i][j] = juego.personaje.tipo;
            }
        }
    }
}

/*
 * Mostrará el juego por pantalla.
 * Se recomienda mostrar todo lo que sea de utilidad para el jugador.
 */
void mostrar_juego(juego_t juego) {
    char bosque[ALTO][ANCHO];

    verificar_visibilidad(juego, bosque);

    for (int i = 0; i < ALTO; i++) {
        for (int j = 0; j < ANCHO; j++) { 
            if (bosque[i][j] == CHLOE) {
                printf("%s ", CHLOE_S);
            } else {   
                printf(" %c ", bosque[i][j]);
            }
        }
        printf(" \n");
    }
    printf("\n");

    printf( "REGLAS DEL JUEGO: \n");
    printf("Movimientos:\n"); 
    printf("Arriba: %c                                  Encender linterna: %s (%c) \n", ARRIBA, LINTERNA_S, LINTERNA);
    printf("Izquierda: %c                               Encender vela: %s (%c) \n", IZQUIERDA, VELA_S, VELA);
    printf("Abajo: %c                                   Encender bengala: %s (%c) \n", ABAJO, BENGALA_S, BENGALA);
    printf("Derecha: %c                                 Mostrar tiempo restante: %s (%c) \n", DERECHA, TIEMPO_RESTANTE_S, TIEMPO_RESTANTE);
}

/*
 * Devolvera true en caso de ser una jugada valida (W,A,S,D,L,V,E o T)
 * Devolvera false en caso contrario.
 */
bool es_jugada_valida(char jugada) {
    return (jugada == ARRIBA || jugada == IZQUIERDA || jugada == ABAJO || jugada == DERECHA || jugada == ENCENDER_LINTERNA 
    || jugada == ENCENDER_VELA || jugada ==ENCENDER_BENGALA || jugada == TIEMPO_RESTANTE);
}
