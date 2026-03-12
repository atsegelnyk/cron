cmake_minimum_required(VERSION 3.25)
project(crond C)

set(CMAKE_C_STANDARD 17)

add_executable(crond
        src/main.c
        src/parser/parser.c
        src/cron/cron.c
        src/cron/loader.c
        src/cron/loader.h
        src/cron/executor.h
        src/cron/executor.c
        src/cron/loader.h
)

target_include_directories(crond PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        src/parser
        src/task
        src/cron
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
