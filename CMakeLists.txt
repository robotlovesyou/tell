cmake_minimum_required(VERSION 3.15)
project(tell)
include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/thirdparty)

add_library(
        til
        include/thirdparty/fmt/format.cc
        til/Token.h til/Token.cpp
        til/Cursor.h
        til/StringCursor.cpp til/StringCursor.h
        til/FileCursor.cpp til/FileCursor.h
        til/Lexer.cpp til/Lexer.h
        til/ErrorReporter.h
        til/DocCommentContext.cpp
        til/DocCommentContext.h
        til/ConsoleErrorReporter.cpp
        til/ConsoleErrorReporter.h
        til/TestErrorReporter.cpp
        til/TestErrorReporter.h
        til/Declaration.h
        til/MessageDeclaration.cpp
        til/MessageDeclaration.h
        til/AST.cpp til/AST.h
        til/DirectiveDeclaration.cpp
        til/DirectiveDeclaration.h
        til/ServiceDeclaration.cpp
        til/ServiceDeclaration.h
        til/Parser.cpp
        til/Parser.h
        til/ParsingException.cpp
        til/ParsingException.h
        til/Field.cpp
        til/Field.h
        til/TypeDef.cpp
        til/TypeDef.h
        til/ScalarTypeDef.cpp
        til/ScalarTypeDef.h
        til/MessageTypeDef.cpp
        til/MessageTypeDef.h
        til/ListTypeDef.cpp
        til/ListTypeDef.h
        til/MapTypeDef.cpp
        til/MapTypeDef.h
        til/SubTypeDef.cpp
        til/SubTypeDef.h
        til/Argument.cpp
        til/Argument.h
        til/Call.cpp
        til/Call.h
        til/ASTValidator.cpp
        til/ASTValidator.h
        til/SerializableTypeDef.cpp
        til/SerializableTypeDef.h
        til/SerializableField.cpp
        til/SerializableField.h
        til/SerializableMessageDeclaration.cpp
        til/SerializableMessageDeclaration.h
        til/SerializableAST.cpp
        til/SerializableAST.h
        til/SerializableDirectiveDeclaration.cpp
        til/SerializableDirectiveDeclaration.h
        til/SerializableCall.cpp
        til/SerializableCall.h
        til/SerializableServiceDeclaration.cpp
        til/SerializableServiceDeclaration.h)

add_library(gen gen/Strcase.cpp gen/Strcase.h gen/GoGenerator.cpp gen/GoGenerator.h gen/GoTemplate.h gen/MissingDirectiveException.cpp gen/MissingDirectiveException.h)
target_include_directories(gen PUBLIC til)

add_executable(tilc compiler/main.cpp compiler/Compiler.cpp compiler/Compiler.h)
target_include_directories(tilc PUBLIC til)
target_link_libraries(tilc til)

add_executable(tell tell/main.cpp tell/main.cpp)
target_include_directories(tell PUBLIC til gen)
target_link_libraries(tell til gen)

add_subdirectory(tests)