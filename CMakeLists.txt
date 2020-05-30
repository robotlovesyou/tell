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
        compiler/DocCommentContext.cpp
        compiler/DocCommentContext.h
        compiler/ConsoleErrorReporter.cpp
        compiler/ConsoleErrorReporter.h
        compiler/TestErrorReporter.cpp
        compiler/TestErrorReporter.h
        compiler/Declaration.h
        compiler/MessageDeclaration.cpp
        compiler/MessageDeclaration.h
        compiler/AST.cpp compiler/AST.h
        compiler/DirectiveDeclaration.cpp
        compiler/DirectiveDeclaration.h
        compiler/ServiceDeclaration.cpp
        compiler/ServiceDeclaration.h
        compiler/Parser.cpp
        compiler/Parser.h
        compiler/ParsingException.cpp
        compiler/ParsingException.h
        compiler/Field.cpp
        compiler/Field.h
        compiler/TypeDef.cpp
        compiler/TypeDef.h
        compiler/ScalarTypeDef.cpp
        compiler/ScalarTypeDef.h
        compiler/MessageTypeDef.cpp
        compiler/MessageTypeDef.h
        compiler/ListTypeDef.cpp
        compiler/ListTypeDef.h
        compiler/MapTypeDef.cpp
        compiler/MapTypeDef.h
        compiler/SubTypeDef.cpp
        compiler/SubTypeDef.h
        compiler/Argument.cpp
        compiler/Argument.h
        compiler/Call.cpp
        compiler/Call.h compiler/ASTValidator.cpp compiler/ASTValidator.h)

add_executable(tellc main.cpp)
target_link_libraries(tellc tell)

add_subdirectory(tests)