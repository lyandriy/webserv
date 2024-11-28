#include <stdio.h>
#include <stdlib.h>

int main() {
    // Obtener el valor de las variables de entorno
    char *user = getenv("user");     // Obtener la variable de entorno USER
    char *age = getenv("age");     // Obtener la variable de entorno HOME
    char *answer = getenv("answer"); // Obtener la variable de entorno MY_VAR

    // Mostrar los valores de las variables de entorno
    if (user != NULL) {
        printf("user: %s\n", user);
    } else {
        printf("user no está definida.\n");
    }

    if (age != NULL) {
        printf("age: %s\n", age);
    } else {
        printf("age no está definida.\n");
    }

    if (answer != NULL) {
        printf("answer: %s\n", answer);
    } else {
        printf("answer no está definida.\n");
    }

    return 0;
}
