cmake_minimum_required(VERSION 3.13)
project(sdn C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(controller)
include_directories(node)

add_executable(sdn
        controller/controller-conf.h
        controller/sdn-controller.c
        controller/symbols.c
        controller/symbols.h
        node/sdn-udp-client-repo.c
        node/sdn-udp-client.c
        node/sdn-udp-client_nfv.c
        node/sdn-udp-client_udp.c
        node/symbols.c
        node/symbols.h
        project-conf.h)
