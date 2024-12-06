# Webserv

# Tests

## Subject

### Requirements

- [x]  Program name
- [x]  Makefile
    - [x]  Relink
    - [x]  Rules NAME, all, clean, fclean y re
    - [x]  c++
    - [x]  -Wall -Wextra -Werror -std=c++98
    - [x]  No external library and no Boost
- [ ]  Allowed functions
- [ ]  a configuration file as argument, or use a default path
        ./webserver genera: basic_string::_M_construct null not valid, código salida == 1
- [x]  execve another server
- [x]  Non-blocking
        Esto diría que está bien, confirmar con Lyudmyla
- [x]  1 poll for all I/O operations
- [ ]  poll read and write at the same time
        No tengo claro que esto lo estemos cumpliendo
- [ ]  Never read or write without poll()
        El write de Response.cpp:440 hay un write a un fd[1] para el autoindex, comprobar que es correcto
- [x]  Don’t check errno
        Solo CGI.cpp(:157) usa errno, pero no lo checkea
- [x]  Don’t use poll before read config file
- [x]  Don’t write/send in any file descriptor without poll
- [x]  Macros FD_SET, FD_CLR, FD_ISSET, FD_ZERO allowed
        No las usamos porque son necesarias para select(), no para poll()
- [ ]  Request don’t hang forever
        No se cómo probar esto si no es para una petición que pase por el CGI
- [x]  Compatible with any web browser
        Es compatible con firefox y chrome
- [x]  HTTP 1.1
- [ ]  HTTP response status
    - [x]  100
        Esto no es obligatorio, necesita de un header específico en la request que le indica al servidor que cuando le devuelva un 100 el cliente empezará a enviar el body
    - [x]  200
        Las que deben devolver 200 lo devuelven correctamente
    - [ ]  300
        No se qué tipo de petición hacer para probar esto
    - [ ]  400
        Hay peticiones que deben devolver 404 y devuelven 500
            http://localhost:8081/jsbhfjasbdf.html
                http://localhost:8081/favicon.ico  -> deberíamos meter un favicon.ico???
    - [ ]  500
        Creo que estoy haciendo mal las peticiones para el CGI
- [ ]  Default error pages
        Para el 500 no se devuelve la página por defecto.
- [x]  Fork() solo para CGI
- [x]  Serve fully static webstatic
- [ ]  Clients must can upload files
- [ ]  GET, POST, DELETE
    - [x]  GET
    - [x]  POST
    - [ ]  DELETE
        No se cómo probarlo
- [ ]  Test de estres
- [ ]  Listen multiple ports

### Configuration file

- [ ]  Port + host for each server
    - [ ] servernames_port_diff → ambos funcionan
    - [ ] servernames_diff_port_same → solo funciona el primero
    - [ ] no_methods_accepted → todo 404 (debería ser 405 method not accepted)

- [x]  setup server_names or not
    - [ ] not_server_name → sin server name no nombre por defecto, ***es correcto??***
- [x]  Default port
- [ ]  Setup default error pages
    - [ ] páginas definidas, no siempre se devuelven
- [ ]  Limit client body size
    - [ ] Server.cpp:7 client_max_body_size = 1; petición con body > 1, no produce error
- [ ]  Rules / configuración
    - [x]  Define list HTTP methods
    - [ ]  Define HTTP redirection
        - [ ] redirection config → error "Invalid keyword return!"
    - [x]  Define directory root and url
    - [x]  Turn on or off directory listing
        - [ ] autoindex_on_off → todo correcto
    - [ ]  Set default file to answer if request = directory
    - [ ]  CGI based on file extension
    - [ ]  CGI work with POST and GET
    - [ ]  Able to accept uploaded file
        - [ ]  full path = PATH_INFO
        - [ ]  Chunked request → server unchunck → CGI expect EOF as end of the body
        - [ ]  Chunked response → CGI sin content-length → EOF
        - [ ]  Call CGI with file requested as first argument
        - [ ]  CGI run in the correct directory for relative path file access (No entiendo esto)
        - [ ]  Servere work with one CGI at least
- [ ]  Various config files, default basic files
- [ ]  Server never dies
- [ ]  

## Corrección

### Check the code and ask questions

- [ ]  Launch siege
- [ ]  Explicación básica de HTTP
- [ ]  Función usada para operaciones de entrada/salida
- [ ]  Explicación de cómo funciona poll()
- [ ]  Si el servidor solo usa una función poll, cómo hace el servidor para manejar múltiples clientes para escrbir/leer
- [ ]  Check fd para lectura y escritura AL MISMO TIEMPO
- [ ]  Solo un read o write para cada cliente
- [ ]  Buscar todas las read/recv/write/send en el socket y comprobar que si un error ocurre, el cliente is eliminado
- [ ]  Buscar todas las read/recv/write/send y comprobar si el valor retornado es comprobado correctamente )comprobar solo -1 o valores no es suficiente, se deben de comproabr ambos)
- [ ]  Si se comprueba errno después de read/recv/write/send suspender
- [ ]  Leer o escribir en cualquier file descriptor sin usar poll → suspender
- [ ]  Sin relink

### Configuration

- [ ]  HTTP status code de respuesta. Proporcionar lista al corrector
- [ ]  Múltiples servidores con con diferentes puertos
    - [ ]  Probar puertos repetidos
    - [ ]  Probar puertos en uso
- [ ]  Múltiples servidores con diferentes hostnames

curl --resolve example.com:80:127.0.0
[http://example.com/](http://example.com/) ([http://example.com/](http://example.com/))

- [ ]  Setup default error page
- [ ]  Limitar el body del cliente
- [ ]  Establecer rutas en un servidor para diferentes directorios
- [ ]  Establecer listas de métodos aceptados para una determinada ruta
    - [ ]  borrar algo con y sin permiso

### Basic checks

Usar telnet, curl, archivos preparados para demostrar que funciona:

- [ ]  GET
- [ ]  POST
- [ ]  DELETE
- [ ]  Código de respuesta adecuado
    - [ ]  200
    - [ ]  300
    - [ ]  400
    - [ ]  500
- [ ]  Subir archivo al servidor y bajarlo de nuevo
    - [ ]  (opcional mío) usar un formulario para subir el archivo

### Check CGI

- [ ]  El servidor funciona bien usando CGI
- [ ]  EL CGI se ejecuta en el directorio correcto para rutas de acceso relativas
- [ ]  Ayudar al corrector a verificar que todo funciona correctamente
    - [ ]  Probar con GET
    - [ ]  Probar con POST
- [ ]  Probar scripts con errores para ver si se manejan correctamente
    - [ ]  Loops infinitos
    - [ ]  Errores de ejecución
- [ ]  Servidor nunca debe romperse

### Check con navegador

- [ ]  Usar el navegador de referencia
    - [ ]  Abrir Herramientas de desarrollo (F12) → Red
    - [ ]  Comprobar headers
        - [ ]  Request
        - [ ]  Response
- [ ]  URL incorrecta
- [ ]  Compatible con una página estática
- [ ]  Listar un directorio
- [ ]  URL redirigida
- [ ]  Otras cosas

### Port issues

- [ ]  Establecer múltiples puertos y usar diferentes sitios web.
    - [ ]  Usar el navegador para asegurarse de que la configuración funciona como se espera y enseñe el sitio adecuado
- [ ]  Poner mismo puerto varias veces. Debe fallar
- [ ]  Múltiples servidores a la vez con diferentes config pero con puertos en común.
    - [ ]  Si funciona preguntar xq el servidor funciona si una de las configuraciones no es funcional. (Continuar, no lo marca como error)

### Siege y test de estrés

- [ ]  Usar Siege para ejecutar test de estrés
- [ ]  La disponibilidad debe permanecer por encima de 99,5%
    - [ ]  Para peticiones GET simples de una web vacía
    - [ ]  con siege -b en esa web
- [ ]  Verificar memory leaks (memoria usada no crece indefinidamente)
- [ ]  Verificar que no hay conexiones colgadas
- [ ]  Se debe de poder usar siege indefinidamente sin tener que reiniciar el servidor
- [ ]  Limitar los valores de siege de acuerdo con el corrector
    - [ ]  Número de conexiones por segundo (-c)
    - [ ]  Máximo tiempo de espera entre reconexiones del cliente (-d)
    - [ ]  Número de reintentos (-r)

### Bonus

- [ ]  Más de 1 CGI

### Tests propios

- [ ]  Leaks
- [ ]  
___
`curl http://wololo:8081/test/hola --resolve wololo:8081:127.0.0.1`

***
