#include <stdlib.h>
#include <stdio.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#define NAME "Xmlrpc-c"
#define VERSION "1.0"

static void
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n", envP->fault_string, envP->fault_code);
        exit(1);
    }
}

int main(int const argc, const char ** const argv) {
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <num1> <num2>\n", argv[0]);
        return 1;
    }

    const char * const serverUrl = "http://localhost:12345/RPC2";
    const char * const methodName = "sample.add";

    xmlrpc_env env;
    xmlrpc_value * resultP;
    xmlrpc_int32 sum, num1, num2;

    // pobieranie liczb
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);

    // inicjalizacja środowiska
    xmlrpc_env_init(&env);

    // inicjalizacja klienta XML-RPC
    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);

    // wywołanie zdalnej procedury
    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(ii)", num1, num2);
    dieIfFaultOccurred(&env);

    // pobranie wyniku
    xmlrpc_read_int(&env, resultP, &sum);
    dieIfFaultOccurred(&env);

    printf("Wynik: %d\n", sum);

    // Clean up resources
    xmlrpc_DECREF(resultP);
    xmlrpc_env_clean(&env);
    xmlrpc_client_cleanup();

    return 0;
}