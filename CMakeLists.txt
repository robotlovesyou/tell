cmake_minimum_required(VERSION 3.15)
project(tell)
include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/thirdparty)

add_library(
        tell
        compiler/MessageDescriptor.cpp compiler/MessageDescriptor.h
        compiler/FieldDescriptor.cpp compiler/FieldDescriptor.h
        include/thirdparty/fmt/format.cc
        compiler/SourceDescriptor.cpp compiler/SourceDescriptor.h
        compiler/Token.h compiler/Token.cpp
        compiler/Cursor.h
        compiler/StringCursor.cpp compiler/StringCursor.h
        compiler/FileCursor.cpp compiler/FileCursor.h
        compiler/Lexer.cpp compiler/Lexer.h
        compiler/ErrorReporter.h
        compiler/DocCommentContext.cpp compiler/DocCommentContext.h compiler/ConsoleErrorReporter.cpp compiler/ConsoleErrorReporter.h compiler/SilentErrorReporter.cpp compiler/SilentErrorReporter.h compiler/Declaration.h compiler/MessageDeclaration.cpp compiler/MessageDeclaration.h compiler/AST.cpp compiler/AST.h compiler/DirectiveDeclaration.cpp compiler/DirectiveDeclaration.h)

add_executable(tellc main.cpp)
target_link_libraries(tellc tell)

add_subdirectory(tests)