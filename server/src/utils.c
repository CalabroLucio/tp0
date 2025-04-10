#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{


	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	logger = log_create("tp0.log","tp0_log",true,LOG_LEVEL_INFO);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, PUERTO, &hints, &servinfo);

	// Creamos el socket de escucha del servidor

	for (p = servinfo; p != NULL; p = p->ai_next){
		socket_servidor = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		
		if (socket_servidor == -1){
			log_warning(logger,"Fallo socket(),probando siguiente");
			continue;
		}

		// Asociamos el socket a un puerto
		// Asociar (bind) el socket a la dirección IP + puerto

		if (bind(socket_servidor,p->ai_addr,p->ai_addrlen) == -1){
			close(socket_servidor);
			log_warning(logger,"Fallo bind(),probando siguiente");
			continue;
		}

		break; // Si llegamos aca ya tenemos un socket como queremos
	}
	
	// Escuchamos las conexiones entrantes

	if (listen(socket_servidor,10) == -1){ //10 -> maximo de conexiones en cola
		log_error(logger,"Error con el listen()");
		close(socket_servidor);
		freeaddrinfo(servinfo);
		exit(1);
	}



	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");
	log_destroy(logger);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{

	// Aceptamos un nuevo cliente
	int socket_cliente;
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
