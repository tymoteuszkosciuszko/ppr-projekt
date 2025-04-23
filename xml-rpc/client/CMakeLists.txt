cmake_minimum_required(VERSION 3.30)
project(ppr_lab2_client C)

set(CMAKE_C_STANDARD 11)
include_directories(/opt/homebrew/Cellar/xmlrpc-c/1.60.05/include)
link_directories(/opt/homebrew/Cellar/xmlrpc-c/1.60.05/lib)

add_executable(ppr_lab2_client main.c)
target_link_libraries(ppr_lab2_client
        xmlrpc
        xmlrpc_client
        xmlrpc_util
        xmlrpc_server
        xmlrpc_server_abyss
)