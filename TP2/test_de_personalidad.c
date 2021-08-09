#include <stdio.h>
#include <stdbool.h>
#include "test_de_personalidad.h"

const char POLAR = 'I', PANDA = 'P', PARDO = 'G';

#define ANIME 'A'
#define MUSICA_POP 'M'
#define LIMPIEZA 'L'

#define BAMBU 'B'
#define PESCADO 'P'
#define FOCAS 'F'

const int LIMPIEZA_NUMERO = 1, ANIME_NUMERO = 2, MUSICA_POP_NUMERO = 3; // Valores numericos asociados a cada canal de television.
const int FOCAS_NUMERO = 3, BAMBU_NUMERO = 6, PESCADO_NUMERO = 9; // Valores numericos asociados a cada alimento.

const int MAXIMO_PISO = 18, MINIMO_PISO = 1; 
const int MAXIMO_GRITO = 18, MINIMO_GRITO = 1;

const int POLAR_MINIMO = 5, POLAR_MAXIMO = 24;
const int PANDA_MINIMO = 25, PANDA_MAXIMO = 43;
const int PARDO_MINIMO = 44, PARDO_MAXIMO = 63;

/*
* PRE: -
* POST: Imprime por pantalla un saludo de bienvenida.
*/
void dar_bienvenida() {
    printf ("Antes de comenzar, hay que conocer tu personalidad escandalosa. \n"
            "Para ello es necesario que respondas algunas preguntas... \n");
}

/*
* PRE: -
* POST: Devolvera TRUE, si el canal de television es ANIME, MUSICA_POP o LIMPIEZA y FALSE, si no es ninguno de ellos.
*/
bool es_canal_valido (char canal_television) {
    return (canal_television == ANIME || canal_television == MUSICA_POP || canal_television == LIMPIEZA);
}

/*
* PRE: -
* POST: Pregunta al usuario que canal de televisión prefiere y repregunta si no es ANIME, MUSICA_POP o LIMPIEZA.
*/
void pedir_canal(char* canal_television) {
    printf ("Vas a ver televisión un rato, pones el canal de Anime (%c), Musica Pop (%c), Limpieza (%c): ", ANIME , MUSICA_POP, LIMPIEZA);
    scanf (" %c", canal_television);

    while (es_canal_valido(*canal_television) == false) {
        printf ("Las únicas opciones posibles son Anime (%c), Musica Pop (%c), Limpieza (%c): ", ANIME, MUSICA_POP, LIMPIEZA);
        scanf(" %c", canal_television);
    }
}

/*
* PRE: -
* POST: Devolvera TRUE, si la vianda es BAMBU, PESCADO o FOCAS y FALSE, si no es ninguna de ellos.
*/
bool es_vianda_valida (char vianda) {
    return (vianda == BAMBU || vianda == PESCADO || vianda == FOCAS);
}

/*
* PRE: -
* POST: Pregunta al usuario que vianda prefiere y repregunta si no es BAMBU, PESCADO o FOCAS.
*/
void pedir_vianda (char* vianda) {
    printf ("Solo podes guardar un alimento en tu vianda Bambu (%c), Pescado (%c), Focas (%c): ", BAMBU, PESCADO, FOCAS);
    scanf (" %c", vianda);

    while (es_vianda_valida(*vianda) == false) {
        printf ("Las únicas opciones posibles son Bambu (%c), Pescado (%c), Focas (%c): ", BAMBU, PESCADO, FOCAS);
        scanf (" %c", vianda);
    }
}

/*
* PRE: -
* POST: Devolvera TRUE, si la variable piso se encuentra dentro de un rango determinado por
*       MINIMO_PISO y MAXIMO_PISO, y FALSE si encuentra fuera de ese rango.
*/
bool es_piso_valido (int piso) {
    return (piso >= MINIMO_PISO) && (piso <= MAXIMO_PISO);
}

/*
* PRE: -
* POST: Pregunta al usuario en que piso prefiere vivir y repregunta si se encuentra fuera del rango determinado por
*       MINIMO_PISO y MAXIMO_PISO.
*/
void pedir_piso (int* piso) {
    printf ("Te compras una torre con tus dos hermanes de %i pisos. ¿En qué piso te gustaria vivir?: ", MAXIMO_PISO);
    scanf ("%i", piso);

    while (es_piso_valido(*piso) == false) {
        printf ("Solo se puede elegir un piso entre %i y %i: ", MINIMO_PISO, MAXIMO_PISO);
        scanf ("%i", piso);
    }
}

/*
* PRE: -
* POST: Devolvera TRUE, si la variable grito se encuentra dentro de un rango determinado por 
*       MINIMO_GRITO y MAXIMO_GRITO, y FALSE si encuentra fuera de ese rango.
*/
bool es_grito_valido (int grito) {
    return (grito >= MINIMO_GRITO) && (grito <= MAXIMO_GRITO);
}

/*
* PRE: -
* POST: Pregunta al usuario que tan fuerte gritaría si ve una rata y repregunta si no esta dentro del rango determinado por 
*       MINIMO_GRITO y MAXIMO_GRITO.
*/
void pedir_grito (int* grito) {
    printf ("¡OH UNA RATA! ¿qué tan fuerte gritas de %i a %i? ", MINIMO_GRITO, MAXIMO_GRITO);
    printf ("Siendo %i no gritar y %i desgarrarse la garganta: ", MINIMO_GRITO, MAXIMO_GRITO);
    scanf ("%i", grito);

    while (es_grito_valido(*grito) == false) {
        printf ("Nuestra escala solo va de %i a %i: ", MINIMO_GRITO, MAXIMO_GRITO);
        scanf ("%i", grito);
    }
}

/*
* PRE: El parametro canal debe ser alguna de las correspondientes 
*      a los canales de televisión: ANIME, MUSICA_POP, LIMPIEZA.
* POST: Devuelve un valor numérico segun el caracter recibido.
*/
int asignar_valor_numerico_canal (char canal) {
    int valor = 0;
    switch (canal) {
        case LIMPIEZA:
            valor = LIMPIEZA_NUMERO;
            break;
        case ANIME:
            valor = ANIME_NUMERO;
            break;
        default:
            valor = MUSICA_POP_NUMERO;
            break;
    }
    return valor;
}

/*
* PRE: El parametro vianda debe ser alguna de las correspondientes a las viandas: BAMBU, PESCADO, FOCAS
* POST: Devuelve un valor numérico segun el caracter recibido..
*/
int asignar_valor_numerico_vianda (char vianda) {
    int valor = 0;    
    switch (vianda) {
        case BAMBU:
            valor = BAMBU_NUMERO;
            break;
        case PESCADO:
            valor = PESCADO_NUMERO;
            break;
        default:
            valor = FOCAS_NUMERO;
            break;
    }
    return valor;
}

/*
* PRE: El parametro canal debe corresponder a ANIME, MUSICA_POP o LIMPIEZA.
*      El parametro vianda debe corresponder a BAMBU, PESCADO  o FOCAS.
*      El parametro piso debe corresponder a un valor dentro del rango MINIMO_PISO y MAXIMO_PISO.
*      El parametro grito debe corresponder a un valor dentro del rango MINIMO_GRITO y MAXIMO_GRITO.
* POST: La función devuelve un puntaje.
*/
int calcular_puntaje (char canal, char vianda, int piso, int grito) {
    int programa = asignar_valor_numerico_canal (canal);
    int comida = asignar_valor_numerico_vianda (vianda);
    return ((programa * comida) + piso + grito);
}   

/*
* PRE: Puntaje debe estar dentro del rango establecido por POLAR_MINIMO y PARDO_MAXIMO
* POST: Imprime por pantalla el puntaje y la personalidad que coincide con POLAR, PANDA o PARDO, según en el rango.
*/
void determinar_personalidad (int puntaje, char* personalidad) {

    if ((puntaje >= POLAR_MINIMO) && (puntaje <= POLAR_MAXIMO)) {
        printf ("\nSos una persona callada, pero responsable y valiente, tu personalidad escandalosa coincide con la de - Polar (%c) -. \n", POLAR);
        printf("Polar por hacer artes marciales y estar muy entrenado, tiene la habilidad de poder saltar piedras.\n");
        (*personalidad) = POLAR;
    }
    if ((puntaje >= PANDA_MINIMO) && (puntaje <= PANDA_MAXIMO)) {
        printf ("\nSos una persona insegura, sensible, tierna y te enamoras rapido, tu personalidad escandalosa coincide con la de - Panda (%C) -. \n", PANDA);
        printf("Panda es un genio usando el GPS, y esto le da la ventaja de saber la ubicación de Chloe, luego de un tiempo podra verla.\n");
        (*personalidad) = PANDA;
    }
    if ((puntaje >= PARDO_MINIMO) && (puntaje <= PARDO_MAXIMO)) {
        printf ("\nSos una persona hiperactiva, ruidosa y optimista, tu personalidad escandalosa coincide con la de - Pardo (%c) -. \n", PARDO);
        printf("Pardo por ser guardabosque y conocer muy bien los caminos, en caso de toparse con un árbol, podrá enfrentar la situación más rápido y perder menos tiempo.\n");
        (*personalidad) = PARDO;
    }
}

/*
* PRE: -
* POST: Deetermina a través de un cuestionario, el tipo de oso del que se trata.
*/
 void test_de_personalidad(char* personalidad_detectada) {
    char canal_television;
    char vianda;
    int piso;
    int grito;
    int puntaje = 0;

    dar_bienvenida ();
    pedir_canal (&canal_television);
    pedir_vianda (&vianda);
    pedir_piso (&piso);
    pedir_grito (&grito);
    
    puntaje = calcular_puntaje (canal_television, vianda, piso, grito);
    determinar_personalidad (puntaje, personalidad_detectada);
}