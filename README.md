# Subasta UDP
Proyecto que consiste en un simulador de subastas usando sockets en C
## Info General
Aplicación Cliente/Servidor que simula una subasta. 
El servidor inicia la subasta, y los clientes pueden ofertar. La subasta termina a los 10 segundos de la última oferta
y se informa el valor número ganador	
## Tecnologías
Proyecto creado y compilado usando:
* Lenguaje C
* GCC (Ubuntu 11.3.0)	
## Setup
Para ejecutar este proyecto:

1- Compilar las dos aplicaciones usando los siguientes comandos
```
$ cc servidor.c -o servidorSubasta
$ cc cliente.c -o clienteSubasta
```
2- Ejecutar las aplicaciones
```
./servidorSubasta 
./clienteSubasta 
```