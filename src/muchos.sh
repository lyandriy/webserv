#!/bin/bash

# Configuración
NUM_CONEXIONES=10  # Número de conexiones simultáneas
SERVER="localhost"
PORT=8080

# Función para mantener una conexión al servidor
mantener_conexion() {
    echo -e "GET / HTTP/1.1\r\nHost: $SERVER\r\nConnection: keep-alive\r\n\r\n" | nc $SERVER $PORT &
}

# Crear múltiples conexiones simultáneas
for i in $(seq 1 $NUM_CONEXIONES); do
    mantener_conexion
    echo "Conexión $i establecida"
done

# Esperar a que todas las conexiones terminen
wait

echo "Se han realizado $NUM_CONEXIONES conexiones simultáneas al servidor."

