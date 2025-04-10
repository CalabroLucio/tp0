#include "client.h"
#include <readline/readline.h>


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	
	log_info(logger,"HOLA SOY UN LOG");
	//log_destroy(logger);
	

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	

	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config

	log_info(logger,ip);
	log_info(logger,valor);
	log_info(logger,puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */
	
	
	log_info(logger,"PARTE 3");
	log_destroy(logger);
	
	
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log","tp0_log",true,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{

	// La primera te la dejo de yapa
	

//	char *linea;
//	linea = "no";
//	while (strcmp(linea,"\0"))
//	{
//		linea = readline("> ");
//	    log_info(logger,linea);
//	}

	char* leido;
//	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete

    t_list* lineas = list_create();
    
    while ((leido = readline("> ")) != NULL && strcmp(leido, "") != 0) {
        list_add(lineas, leido);
    }
    
    // Aquí podrías convertir la lista en un paquete
    list_destroy(lineas); // Liberar memoria

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!


	t_paquete* paquete = crear_paquete();
    char* leido;
    
    while ((leido = readline("> ")) != NULL && strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido) + 1); // +1 para '\0'
        free(leido);
    }
    
    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
    log_destroy(logger);
    config_destroy(config);
}
