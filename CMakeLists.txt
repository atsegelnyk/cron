cmake_minimum_required(VERSION 3.25)
project(cron C)

set(CMAKE_C_STANDARD 17)

add_executable(cron
        main.c
        parser/parser.c
        task/task.c
        cron/cron.c
)

target_include_directories(cron PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_SOURCE_DIR}/task
        ${CMAKE_CURRENT_SOURCE_DIR}/cron
)

target_compile_options(cron PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
)