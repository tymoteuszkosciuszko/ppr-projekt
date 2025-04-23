#include <string.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Dla funkcji read

#define NAME "Xmlrpc-c Byte-by-Byte Client"
#define VERSION "1.0"

//======================================================================
static void dieIfFaultOccurred(xmlrpc_env *const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n", envP->fault_string, envP->fault_code);
        exit(1);
    }
}

//======================================================================
int main(int argc, const char **argv) {
    const char *const serverUrl = "http://localhost:12345/RPC2";
    const char *const methodName = "convert.toHex";

    xmlrpc_env env;
    xmlrpc_value *resultP;
    const char *hex_result;
    char buf; // Jednobajtowy bufor
    ssize_t bytesRead;

    xmlrpc_env_init(&env);

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);

    // Odczyt danych w pętli za pomocą read
    while ((bytesRead = read(STDIN_FILENO, &buf, 1)) > 0) {
        // Konwersja char na integer
        int num = (int)buf;
        //printf("%02X\n", num);
        // Wywołanie zdalnej procedury
        resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(i)", num);
        dieIfFaultOccurred(&env);

        // Zwolnienie pamięci
        xmlrpc_DECREF(resultP);
        free((void *)hex_result);
    }

    if (bytesRead < 0) {
        perror("read");
        exit(1);
    }

    xmlrpc_env_clean(&env);
    xmlrpc_client_cleanup();

    return 0;
}