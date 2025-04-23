#include <string.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include <stdlib.h>
#include <stdio.h>

//======================================================================
static xmlrpc_value *convert_to_hex(
        xmlrpc_env *   const envP,
        xmlrpc_value * const paramArrayP,
        void *         const serverInfo,
        void *         const channelInfo) {

    xmlrpc_int32 input;
    char hex_output[4] = {0};

    // Parse the input as an integer
    xmlrpc_decompose_value(envP, paramArrayP, "(i)", &input);

    if (envP->fault_occurred)
        return NULL;

    unsigned char byte = (unsigned char)input;
    snprintf(hex_output, sizeof(hex_output), "%02X", byte);

    printf("%s ", hex_output);

    return xmlrpc_build_value(envP, "s", hex_output);
}

//======================================================================
int main(void) {
    int port = 12345;

    struct xmlrpc_method_info3 const methodInfo = {
            .methodName     = "convert.toHex",
            .methodFunction = &convert_to_hex,
    };

    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry *registryP;
    xmlrpc_env env;

    xmlrpc_env_init(&env);

    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_new(): %s\n", env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &methodInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3(): %s\n", env.fault_string);
        exit(1);
    }

    serverparm.config_file_name = NULL;
    serverparm.registryP = registryP;
    serverparm.port_number = port;
    serverparm.log_file_name = "/tmp/xmlrpc_log";

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if (env.fault_occurred) {
        printf("xmlrpc_server_abyss(): %s\n", env.fault_string);
        exit(1);
    }

    return 0;
}