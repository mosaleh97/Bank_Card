cmake_minimum_required(VERSION 3.24)
project(payment_app C)

set(CMAKE_C_STANDARD 11)

add_executable(payment_app main.c card/card.h card/card.c terminal/terminal.h terminal/terminal.c server/server.c server/server.h)
