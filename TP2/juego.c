#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include "utiles.h"
#include "osos_contra_reloj.h"
#include "test_de_personalidad.h"

const int ESTA_JUGANDO = 0, TERMINADO = -1; 

const double TIEMPO_MAXIMO = 120;
/*
* PRE: -
* POST: Imprime por pantalla un saludo de bienvenida y una introduccion de que se trata el juego.
*/
void imprimir_introduccion() {
    printf("¡Bienvenidx a OSOS CONTRA RELOJ! \n"
            "\n"
            "Un día, aburridos de estar encerrados, los Osos y Chloe decidieron salir al bosque y jugar a las escondidas.\n" 
            "Sin embargo, se olvidaron del toque de queda que rige debido a los aumentos de casos de Covid en la ciudad\n"
            "y ya está oscureciendo. Además tienen otro problema: Chloe sigue escondida y no sabe que ya tienen que volver a casa.\n"
            "\n");
    
}

/*
* PRE: -
* POST: Imprime por pantalla las intrucciones del juego.
*/
void imprimir_intrucciones() {
    printf("\n"
            "Ayudá a tu oso a encontrar a su amiga antes de que comience el confinamiento obligatorio.\n"
            "¡Pero hay un problema más! Ya se hizo de noche en el bosque y el personaje no podrá ver nada de lo que hay a su\n" 
            "alrededor, y deberá ayudarse de su linterna, velas y bengalas para poder llegar a encontrarse con su amiga.\n"
            "Cuidado con Nom Nom y sus sekoalaces, si te topas con uno volveras al inico del bosque!\n"
            "\n");
}

/*
* PRE: Recibe un juego con toda su estructura previamente inicializada
* POST: Si el juego esta TERMINADO, imprime por pantalla si el jugador ganó o perdio dependiendo el tiempo que le llevó encontrar a CHLOE.
*/
void terminar_juego(juego_t juego) {
    if (estado_juego(juego) == TERMINADO) {
        double tiempo_total = detener_cronometro() + juego.personaje.tiempo_perdido;
        
        if (tiempo_total > TIEMPO_MAXIMO) {
            printf("\n"
                    " ######  ####### ######  ######  ###  #####  ####### ####### \n"
                    " #     # #       #     # #     #  #  #     #    #    #       \n"
                    " #     # #       #     # #     #  #  #          #    #       \n"
                    " ######  #####   ######  #     #  #   #####     #    #####   \n"
                    " #       #       #   #   #     #  #        #    #    #       \n"
                    " #       #       #    #  #     #  #  #     #    #    #       \n"
                    " #       ####### #     # ######  ###  #####     #    ####### \n"
                    "\n");
            printf("Lamentablemente tardaste mucho! :( \n");

        } else {
            printf("\n"
                    "  #####     #    #     #    #     #####  ####### ####### \n"
                    " #     #   # #   ##    #   # #   #     #    #    #       \n"
                    " #        #   #  # #   #  #   #  #          #    #       \n"
                    " #  #### #     # #  #  # #     #  #####     #    #####   \n"
                    " #     # ####### #   # # #######       #    #    #       \n"
                    " #     # #     # #    ## #     # #     #    #    #       \n"
                    "  #####  #     # #     # #     #  #####     #    ####### \n" 
                    "\n");
            printf("Bien! encontraste a Chloe a tiempo, FELICITACIONES esa es la personalidad ESCANDALOSA QUE QUEREMOS!! ＼(^o^)／ \n");    
        }
    }
}

int main () {
    juego_t juego;
    char tipo_de_oso;
    
    iniciar_cronometro();
    srand ((unsigned)time(NULL));

    imprimir_introduccion();
    
    test_de_personalidad(&tipo_de_oso);
    
    imprimir_intrucciones();

    inicializar_juego(&juego, tipo_de_oso);

    mostrar_juego(juego);

    char jugada;

    while (estado_juego(juego) == ESTA_JUGANDO){
        printf("Indica tu proximo movimiento: ");
        scanf (" %c", &jugada);

        if (es_jugada_valida(jugada)){
            system("clear");

            realizar_jugada (&juego, jugada);
            mostrar_juego(juego);
        }
    }

    terminar_juego(juego);

    return 0;
}
