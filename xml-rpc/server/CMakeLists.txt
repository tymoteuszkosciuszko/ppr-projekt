cmake_minimum_required(VERSION 3.30)
project(ppr_lab2 C)

set(CMAKE_C_STANDARD 17)

include_directories(/opt/homebrew/Cellar/xmlrpc-c/1.60.05/include)
link_directories(/opt/homebrew/Cellar/xmlrpc-c/1.60.05/lib)

add_executable(ppr_lab2 main.c)

target_link_libraries(ppr_lab2
        xmlrpc
        xmlrpc_client
        xmlrpc_util
        xmlrpc_server
        xmlrpc_server_abyss
)