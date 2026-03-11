cmake_minimum_required(VERSION 3.25)
project(crond C)

set(CMAKE_C_STANDARD 17)

add_executable(crond
        main.c
        parser/parser.c
        task/task.c
        cron/cron.c
        cron/loader.c
        cron/loader.h
        cron/executor.h
        cron/executor.c
        cron/loader.h
)

target_include_directories(crond PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/task
        ${CMAKE_CURRENT_SOURCE_DIR}/cron
)

target_compile_options(crond PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
)
