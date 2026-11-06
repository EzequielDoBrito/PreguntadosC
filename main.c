#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///  ESTRUCTURAS
typedef struct
{
    int ID;
    char nombreJugador[50];
    char nombreUsuario[50];
    char claveUsuario[50];
    int puntaje;
    int partidasJugadas;
} Jugador;

typedef struct
{
    char pregunta[100];
    char respuesta[100];
    int puntaje;
} PreguntasVoF;

typedef struct
{
    char pregunta[100];
    char opciones[3][100];
    char respuesta[100];
    int puntaje;
} PreguntasMulti;

///  PROTOTIPADO DE FUNCIONES
int obtenerSiguienteID(const char archivo[]);
int buscarJugadorEnArchivo(const char archivo[], char nombreBuscado[]);
Jugador crearJugador(const char archivo[]);
void guardarJugadorEnArchivo(const char archivo[], Jugador nuevoJugador);
int validarLogin(const char archivo[], char usuarioIngresado[], char claveIngresada[]);
void mostrarJugadores(const char archivo[]);

int main()
{
    const char archivo[] = "jugadores.dat";
    int opcionMenuLogin;
    int opcionMenuJugador;
    int logueado = 0;
    char usuario[50], clave[50];

    do
    {
        printf("\n\t MENU PREGUNTADOS \t\n");
        printf("Elija una opcion: \n");
        printf("1_ Iniciar Sesion (Login)\n");
        printf("2_ Registrar Nuevo Jugador\n");
        printf("0_ Salir\n");
        scanf(" %d", &opcionMenuLogin);

        switch(opcionMenuLogin)
        {
        case 1:
            printf("\n\t LOGIN \t\n");
            printf("Usuario: ");
            fflush(stdin);
            fgets(usuario, 50, stdin);
            usuario[strcspn(usuario, "\n")] = 0;

            printf("Clave: ");
            fflush(stdin);
            fgets(clave, 50, stdin);
            clave[strcspn(clave, "\n")] = 0;

            logueado = validarLogin(archivo, usuario, clave);
            if(logueado == 1)
            {
                printf("\nBienvenido de vuelta, %s!\n", usuario);
                printf("Elija una opcion: \n");
                printf("1_ Jugar \n");
                printf("2_ Ver historiales \n");
                printf("3_ Ver Jugadores Registrados \n");
                printf("0_ Volver al menu Login \n");
                scanf(" %d", &opcionMenuJugador);

                switch(opcionMenuJugador)
                {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    mostrarJugadores(archivo);
                    break;
                case 0:
                    printf("Saliendo del menu de Jugador... \n");
                    break;
                default:
                    printf("\n\tOpcion invalida. \t\n");

                }
            }
            else
            {
                printf("\nError: Usuario o clave incorrectos.\n");
            }
            break;

        case 2:
            printf("\nREGISTRO\n");
            Jugador nuevo = crearJugador(archivo);
            guardarJugadorEnArchivo(archivo, nuevo);
            break;
        case 0:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("\n\tOpcion invalida.\t\n");
        }
    }
    while(opcionMenuLogin != 0);

    return 0;
}

///Funcion para calcular el ID segun la cantidad de jugadores dentro del archivo
int obtenerSiguienteID(const char archivo[])
{
    FILE *archi = fopen(archivo, "rb");

    if (archi == NULL)
    {
        return 1; ///Como el archivo no existe se le asigna el primer ID
    }

    fseek(archi, 0, SEEK_END);
    long bytesTotales = ftell(archi);
    fclose(archi);

    int cantidadRegistros = bytesTotales / sizeof(Jugador);

    return cantidadRegistros + 1;
}

int buscarJugadorEnArchivo(const char archivo[], char nombreBuscado[])
{
    FILE *archi = fopen(archivo, "rb");
    int hallado = 0;
    Jugador player;

    if(archi == NULL)
    {
        return 0;
    }

    while(!hallado && fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        if(strcmp(nombreBuscado, player.nombreUsuario) == 0)
        {
            hallado = 1;
        }
    }

    fclose(archi);
    return hallado;
}

Jugador crearJugador(const char archivo[])
{
    Jugador nuevoJugador;
    int existe = 1;

    while(existe == 1)
    {
        printf("\nIngrese su nombre de Usuario (Login): ");
        fflush(stdin);
        fgets(nuevoJugador.nombreUsuario, 50, stdin);

        nuevoJugador.nombreUsuario[strcspn(nuevoJugador.nombreUsuario, "\n")] = 0;

        existe = buscarJugadorEnArchivo(archivo, nuevoJugador.nombreUsuario);

        if(existe == 1)
        {
            printf("Error: El nombre de usuario ya esta en uso. Intente con otro.\n");
        }
    }

    printf("Ingrese su Nombre de Jugador: ");
    fflush(stdin);
    fgets(nuevoJugador.nombreJugador, 50, stdin);
    nuevoJugador.nombreJugador[strcspn(nuevoJugador.nombreJugador, "\n")] = 0;

    printf("Ingrese su clave: ");
    fflush(stdin);
    fgets(nuevoJugador.claveUsuario, 50, stdin);
    nuevoJugador.claveUsuario[strcspn(nuevoJugador.claveUsuario, "\n")] = 0;

    nuevoJugador.puntaje = 0;
    nuevoJugador.partidasJugadas = 0;
    nuevoJugador.ID = obtenerSiguienteID(archivo);

    return nuevoJugador;
}

void guardarJugadorEnArchivo(const char archivo[], Jugador nuevoJugador)
{
    FILE *archi = fopen(archivo, "ab");

    if (archi != NULL)
    {
        fwrite(&nuevoJugador, sizeof(Jugador), 1, archi);
        fclose(archi);
        printf("\nUsuario '%s' registrado exitosamente con el ID %d.\n", nuevoJugador.nombreUsuario, nuevoJugador.ID);
    }
    else
    {
        printf("\nError: No se pudo acceder al archivo %s para guardar.\n", archivo);
    }
}

int validarLogin(const char archivo[], char usuarioIngresado[], char claveIngresada[])
{
    FILE *archi = fopen(archivo, "rb");
    Jugador player;
    int loginExitoso = 0;

    if(archi == NULL)
    {
        return 0; ///Si el archivo no existe no se puede iniciar el login de nadie
    }

    while(fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        if(strcmp(usuarioIngresado, player.nombreUsuario) == 0)
        {
            if(strcmp(claveIngresada, player.claveUsuario) == 0)
            {
                loginExitoso = 1;
            }
            break;
        }
    }

    fclose(archi);
    return loginExitoso;
}

///Funcion para leer archivo Jugadores
void mostrarJugadores(const char archivo[])
{
    FILE *archi = fopen(archivo, "rb");
    Jugador player;

    if(archi == NULL)
    {
        printf("\nNo hay usuarios registrados aun.\n");
        return;
    }

    printf("\n\tLISTADO DE JUGADORES EN SISTEMA\t\n");
    printf("-%5s | %-15s | %-20s | %-8s | %-10s\n", "ID", "Usuario", "Nombre Real", "Puntaje", "Partidas");

    while(fread(&player, sizeof(Jugador), 1, archi) == 1)
    {
        printf(" %-5d | %-15s | %-20s | %-8d | %-10d\n",
               player.ID,
               player.nombreUsuario,
               player.nombreJugador,
               player.puntaje,
               player.partidasJugadas);
    }

    fclose(archi);
}
