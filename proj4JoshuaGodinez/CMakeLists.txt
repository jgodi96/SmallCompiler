cmake_minimum_required(VERSION 3.12)
project(proj4JoshuaGodinez)

set(CMAKE_CXX_STANDARD 14)

add_executable(proj4JoshuaGodinez inputbuf.cc inputbuf.h compiler.cc compiler.h lexer.h lexer.cc ir_debug.cc ir_debug.h parser.cc)