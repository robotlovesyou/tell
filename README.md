 - [Til/Tell](#tiltell)
    - [Key outputs](#key-outputs)
    - [Status](#status)
    - [Included Open Source Libraries](#a-note-on-included-open-source-libraries)
    - [Til Interface Language](#til-interface-language)
        - [Comments and Doc Comments](#comments-and-doc-comments)
        - [Directives](#directives)
        - [Messages](#messages)
            - [Types](#types)
                - [Scalar Types](#scalar-types)
                - [Compound Types](#compound-types)
                    - [Messages](#message)
                    - [List](#list)
                    - [Map](#map)
                - [Optional Types](#optional-types)
        - [Services](#services)
    - [Tell RPC](#tell-rpc)
        - [Wire Encoding of Messages](#wire-encoding-of-messages)
        - [RPC Calls](#rpc-calls)
        - [Call paths](#call-paths)
    - [Further Work](#further-work)
    - [Building](#building)
    - [Testing](#testing)
        - [Running the til Test Suite](#running-the-rpc-test-code)
        - [Running the RPC Test Code](#running-the-rpc-test-code)
        - [Viewing the generated HTML documentation](#viewing-the-generated-html-documentation)
    - [Code Organisation](#code-organisation)
        - [libtil](#libtil)
            - [AST](#ast)
            - [Lexing and Parsing](#lexing-and-parsing)
            - [Serialization and Deserialization](#serialization-and-deserialization)
        - [libgen](#libgen)
        - [tilc](#tilc)
        - [tell](#tell)
 - [Udacity Rubric Compliance](#udacity-rubric-compliance)
    - [Loops. Functions, IO](#loops-functions-io)
    - [Object Oriented Programming](#object-oriented-programming)
    - [Memory Management](#memory-management)
    - [Concurrency](#concurrency)
                
 

# Til/Tell

Til/Tell is an experimental interface definition language (til) and rpc generator (tell) with two key goals. 

**1. Optional**

Rather than excluding languages and platforms which do not have an available tell generator, the RPC utilises widely
used technologies (HTTP/JSON) and a very simple implementation pattern, to make hand coding compliant servers and 
clients trivial.

**2. Simple to Adopt**

The til compiler (tilc) outputs JSON as an intermediate representation. Those wishing to create a new generator 
can simply parse the intermediate representation and generate code using the language of their choice. Alternatively
the provided libtil and libgen libraries can be utilised to create generators using C++. Additionally the simple RPC pattern
makes the development of new generators quite easy.

## Key outputs

tilc, a compiler for til code, and tell, a generator which transforms compiled til code into server stubs or documentation
are the key outputs of this project. 

Examples of their operation can be found in the [Testing](#testing) section, specifically under 
[Running the RPC Test Code](#running-the-rpc-test-code) and 
[Viewing the generated HTML Documentation](#viewing-the-generated-html-documentation)

## Status

While the project is not yet production ready it has a functioning compiler for the til interface language and working
generators for the Go language and for basic HTML documentation. See the section on Further Work for more detail.  

## A Note on Included Open Source Libraries.

To simplify building the project a number of dependencies are included as header only, meaning their source is integrated with this project.

Those libraries are:
1. Catch2 Testing Framework. The header file and license are included as catch.hpp and LICENSE.txt in the include/thirdparty/catch2 directory. See https://github.com/catchorg/Catch2
2. {fmt} string formatting library. The header and source files are included in the include/thirdparty/fmt directory. See https://fmt.dev/6.2.0/index.html
3. nlohmann/json json parsing/serializing/deserializing library included in include/thirdparty/nlohmann directory. See https://github.com/nlohmann/json
4. CLI11 is a command line parsing library. It is included in include/thirdparty/cli11. See https://github.com/CLIUtils/CLI11 for details
5. inja is a string templating library. It is included in include/thirdparty/inja. See https://github.com/pantor/inja for details

In addition to the libraries mentioned above, the gen library includes a partial port of the strcase library from Go to C++. 
See https://github.com/iancoleman/strcase for the original source code.   

## Til Interface Language

Til is an interface definition language. A til file consists of directives, message definitions and service definitions.


### Comments and Doc Comments

Ordinary comments are prefixed with a double forward slash '//'. The entire line will be ignored by the compiler.

Doc comments are prefixed with a triple forward slash '///'. They are automatically associated with the item which 
follows them in the source, be it a directive, a message, a field a service or a call. The are emitted in generated code
and can also be used for documentation generation

### Directives

A til directive consists of a bang symbol '!' followed by a name followed by a value surrounded by double quotes '"', eg

```
!a_directive "the_directive_value"
```

The key use of directives is to pass generator specific information, such as the name of a package in go. 

### Messages

Messages definte the data types passed to and from RPCs and appear as follows

```
message MyMessage {
    a_field: int
    another_field: float?
}
```

They keyword "message" must appear at the beginning of each message definition, followed by the message name, followed by 
opening and closing curly braces.

Each field must appear on a separate line. The name of the field appears first, followed by a colon, followed by the type
of the field. 

#### Types

The types supported by tell are:

#### Scalar Types

- **float**: A floating point number.
- **int**: An integer type. 
- **bool** A boolean scalar type.
- **string** A string type. 
- **time** A date time with millisecond precision. Transmitted over the wire as an RFC3339 formatted DateTime string. 

Scalar types are only valid as members of a message. 

#### Compound Types**

##### Message

A field of a message may be another message. Infinitely recursive types are disallowed, eg

```
message IllegalMessage {
    a_field: IllegalMessage
}
```

However, recursive structures are allowed when at least one link in the path is optional. (see below for more details on
optional values). Eg

```
message LegalRecursiveMessage {
    a_field: LegalRecursiveMessage?
}
```

---

##### List
A field can be a list of another type, be it a scalar or compound type. 

```
message MyListMessage {
    a_list_of_ints_field: list[int]
    a_list_of_messages_field: list[MyMessage]
    an_optional_list_of_messages_field: list[MyMessage]?
}
```

In the example above there is a field which is a list of integers, a field which is a list of another message type, 
and a field which is an optional list of another message type


##### Map

A field can be a map type. The key of map types is always a string. The value can be any other type. 

```
message MyMapMessage {
    a_map_of_ints: map[int]
    a_map_of_messages: map[MyMessage]
    an_optional_map_of_messages: map[MyMessage]?
}
```

In the example above there is a field which is a map from string to int, a field which is a map from string to MyMessage
and an optional field which is a map from string to MyMessage

Maps and lists can be nested inside one another, eg

```
message NestedMapsAndLists {
    a_field: list[map[list[int?]]]
}
```

#### Optional Types

Any field of a message may be marked as optional by appending a ? to the type. Eg
```
message MyChildMessage {
    name: string
    nickname: string?
}

message MyParentMessage {
    name: string
    nickname: string?
    child: ChildMessage?
}
```

In the example above the `nickname` field in both the MyChildMessage and MyParentMessage is marked as optional.
Additionally the child field in the MyParentMessage is marked as optional.

In host languages which support optional types such as Typescript, Kotlin, Rust or Scala, the appropriate optional or nullable type should be used. 
In host languages which do not support optional types pointer or nullable representations should be used. For example string? in tell would map to a *string in Go.

### Services

A til file can also define services. A service appears as follows

```
service my_service {
    first_call[MyArgumentType]: MyReturnType
    second_call[MyOtherArgumentType]: MyOtherReturnType
}
```

Each service starts with the keyword "service", followed by the name of the service, followed by opening and closing
curly braces. The calls published by the service appear between the braces.

Each call consists of the name, followed by the name of the argument message surrounded by square brackets, followed by 
a colon, followed by the name of the return message. 

Each call has exactly one argument, which must be a message, and exactly one return type, which must also be a message.
If a call should return a list or a map or a scalar type, they must be exposed as fields of a message. They cannot be
used directly as argument or return values.

## Tell RPC

The Tell RPC is an HTML/JSON RPC with a very simple implementation pattern. 

### Wire encoding of messages. 

Messages are encoded as JSON objects. The fields are converted to object properties with appropriately encoded values. 

The scalar types are encoded as follows

- **float**: Encoded as the JSON number type
- **int**: Encoded as the JSON number type
- **bool** Encoded as the JSON boolean type
- **string** Encoded as the JSON string type 
- **time** Encoded as a string in RFC3339 Date Time format. RPC implementations are responsible for encoding and decoding this to and from native DateTime types

Message types are encoded as JSON objects, as described here.

Map types are also encoded as JSON objects, with the keys becoming property names of the object and the values appropriately encoded following this specification.

List types are encoded as JSON arrays with the members encoded appropriately following this specification.

### RPC Calls

All calls are made as HTTP POST calls, with an application/json content type header for both the request and response.
The body of the request is a JSON wire encoding of the call argument message. The body of the response, if successful is a
JSON wire encoding of the call response. 

Successful calls always return a 200 status.

Errors should always return a 400 status code. Expressed as a til message, the body of an error has the following format.

```
message Error {
    code: string
    messages: map[string]
}
```

The code is taken from a fixed, predefined list (see below). The messages field is a map from string to string. It is 
used for providing further detail about errors. For example it could contain a map from field names to validation errors.

The error codes in this implementation are as follows.

- malformed_request
- invalid_argument
- internal

This is clearly not a complete enough list for a production RPC, but it contains enough for the example code included
with this project 

### Call paths

Each call defined by a service is exposed on a path constructed as follows: "/service_name/call_name".
The service name and the call name are both coerced to lower snake case. 

Taking the following service

```
service Calculator {
    add[Operand]: Result
    subtract[Operand]: Result
}
```

the RPCs would be exposed on the paths

/calculator/add

and

/calculator/subtract

## Further Work

The status of til/tell is entirely experimental. It is not ready for production use.

Further work could include:
- Imports: The current implementation skips the import of packages for the sake of simplicity. A real world implementation would certainly require this.
- Enums: Very useful for describing things such as error codes in a single location.
- OneOf types: ie a field which can be one of several predefined message types. Useful for encoding polymorphic types.
- Better testing. While the til compiler has a reasonable starting set of tests, the generators do not. A good approach for testing the generators could be to generate code in the target languages and then create and run tests agains the generated code in said languages
- Formal language specification for til. Currently the language is only specified by the compiler code which does not make for good documentation.
- RPC Improvements. At a minimum the RPC generators should provide interfaces to enable concepts such as authorization, metric gathering or logging. Additonally the core RPC code should be published as shared libraries. The approach of including things such as error codes in the generated output would likely cause issues.   
- Streams: Streams could be implemented using websockets, which are widely supported.
- System installation instructions.
- Editor plugins: Code highlighting can make development less error prone and faster. 
 

## Building.

The following build instructions are for the Udacity Capstone Workspace. 

If the project is not already present in the workspace then please clone it from github

```
git clone https://github.com/robotlovesyou/tell.git
```

The project builds with the default toolset included in the workspace (gcc-7 and cmake 3.05). All dependencies are
included header only, so there is no additional software to build or install.

From the workspace directory:
```
cd tell
mkdir build
cd build
cmake ..
make
```

## Testing

The compiler library (til) has a set of tests written using catch2. 

Additionally, a go source file and a til interface file are included in the gen_go
folder which can be used to test the Go code output by the RPC generator (tell)

### Running the til Test Suite

**NB The til Test Suite relies on files which are copied to the test build directory by cmake. The tests must be run from
the test build directory, otherwise they will fail**

From the workspace directory, assuming the build instructions above have been followed

```
cd tell/build/tests
./Tests_run -s
```

### Running the RPC Test code

In order to demonstrate the operation of the compiler (tilc) and generator (tell) the project includes an example
til interface file, and some Go code which makes use of the Go source which can be generated from the til interface file. 

The example interface file is found in gen_go/calculator.til

The supporting go code is found in gen_go/main.go.

The instructions below detail how to compile the til source file, and then generate Go server stubs and a client
from the compiled til source file.

An up to date Go toolchain is required to run the generated code. Details of how to do so are provided here for your convenience.

First you need to uninstall the very outdated version of Go in the Udacity Capstone Workspace. The included Go source and
generated output will not compile using the installed version of go. 

```
rm /usr/local/bin/go
rm -rf /usr/local/go
```  

Then you can download an up to date version
```
# download the go binary package
wget https://dl.google.com/go/go1.14.4.linux-amd64.tar.gz

# check the sha checksum for the download. It should be aed845e4185a0b2a3c3d5e1d0a35491702c55889192bb9c30e67a3de6849c067
sha256sum go1.14.4.linux-amd64.tar.gz

# unpack the binary package
tar -C /usr/local -xzf go1.14.4.linux-amd64.tar.gz

# add go to the path
export PATH=$PATH:/usr/local/go/bin
```

Next you can compile the example til file and use it to generate go source.

These instructions assume you have followed the build steps above.

From the workspace directory
```
# create the directory the source will be generated into
mkdir tell/gen_go/calculator
cd tell/build
./tilc -f ../gen_go/calculator.til -o a.json
./tell -f a.json --go_out ../gen_go/calculator/calculator.go
```

Finally you can run the example go code. The file main.go imports the calculator.go file you
just generated.

```
cd ../gen_go
go run main.go
```

You will get output which looks something like this:
```
2020/06/24 13:37:50 after adding 10 the result is: 10
2020/06/24 13:37:50 after multiplcation by 100 the result is: 1000
2020/06/24 13:37:50 after division by 2 the result is: 500
2020/06/24 13:37:50 after subtracting 1 the result is: 499
2020/06/24 13:37:50 memory first: {first 2020-06-24 13:37:50.381189063 +0000 UTC 499}
2020/06/24 13:37:50 after division by 2 the result is: 249
2020/06/24 13:37:50 memory second: {second 2020-06-24 13:37:50.381623473 +0000 UTC 249}
2020/06/24 13:37:50 memory dump: {[{first 2020-06-24 13:37:50.381189063 +0000 UTC 499} {second 2020-06-24 13:37:50.381623473 +0000 UTC 249}]}
2020/06/24 13:37:50 reading calculator value: 249
2020/06/24 13:37:50 calculator is reset to: 0
```

The main.go file implements a struct which meets the CalculatorServer interface requirements defined in
the generated calculator.go file. It creates an instance of that struct, registers it with the Tell server and
then starts the server in a goroutine.

Next it creates an instance of the generated CalculatorClient and uses it to make calls to the Calculator server. 
It logs the result of each call to the command line. 


### Viewing the generated HTML documentation

Assuming you have followed all the build and test steps to this point, from the workspace directory

```
cd tell/build
./tell -f a.json --html_out doc.html
```

You can then activate the VNC desktop, open firefox and open the generated file by visiting the url file:///home/workspace/tell/build/doc.html


## Code Organisation

The project consists of two shared libraries (libtil and libgen), two executables (tilc and tell) and an executable
test suite (Tests_run)

### libtil

All code for libtil is found in the til directory and under the til namespace.

Classes and structs found in the til namespace falls into the following categories

#### AST

The classes Argument, AST, Call, Declaration, DirectiveDeclaration, DocCommentContext, Field, ListTypeDef, MapTypeDef, 
MessageDeclaration, MessageTypeDef, ScalarTypeDef, ServiceDeclaration, SubTypeDef and TypeDef are all used to define the 
AST of compiled til code. 

The class AST sits at the root of the AST. 
Declaration is a virtual class used to represent all top level declarations. DirectiveDeclaration, MessageDeclaration and
ServiceDeclaration are all concrete implementations of the Declaration class and represent their respective top level 
declarations. 

The Field class represents a field within a message declaration. TypeDef is a virtual class used to represent all field types. 
ScalarTypeDef, MessageTypeDef, MapTypeDef and ListType def are concrete implementations of TypeDef used to represent the respective 
field types.

SubTypeDef is in interface implemented by MapTypeDef and ListTypeDef to give access to the type of the members of the map or list

Call is used to represent a call within a service declaration. Argument is used to represent the parameter or return types of a call. 

DocCommentContext is used to represent comments which should be preserved in the AST at all levels of the structure.

#### Lexing and Parsing
ASTValidator, Cursor, StringCursor, FileCursor, Lexer, Parser, ErrorReporter, TestErrorReporter, ConsoleErrorReporter, 
ParsingException and Token are all used in the lexing and parsing of til source files.

ASTValidator examines the AST after parsing to ensure it is valid. It checks that all messages used as field types or 
call arguments exist within the AST, and that none of the structures declared would be infinitely recursive. 

Cursor is a virtual template class used to represent all iterable types (StringCursor, FileCursor and Lexer). 
StringCursor and FileCursor both consume til source and produce an iterable source of char. FileCursor wraps a StringCursor and 
passes it the source of the file it is asked to iterate over. 
Lexer consumes a Cursor<Char> and produces an iterable source of Tokens.

Parser transforms an iterable source of tokens into an AST

ErrorReporter is a virtual class used to represent all types of ErrorReporter.
TestErrorReporter and ConsoleErrorReporter are both concrete instances of the ErrorReporter class. They only differ in
the appearance of the test output. 

Reporting errors will prevent the compilation process from proceeding to the next stage.

#### Serialization and Deserialization

The structs SerializableAST, SerializableCall, SerializableDirectiveDeclaration, SerializableField, SerializableMessageDeclaration,
SerializableServiceDeclaration and SerializableTypeDef are used for a representation of the AST with two purposes. 
Firstly serialization to and from the JSON format (making use of the nlohmann/json library) and secondly representation of the AST
during code generation, as the template library (inja) also makes use of the nlohmann/json library.

Each struct implements the methods (to_json and from_json) required by the nlohmann/json library.

### libgen

libgen and the gen namespace contain code used to generate source code and documentation from a serialized AST. All code in the
gen namespace is gen directory

Generator is a virtual class which represents all code generators. GoGenerator and HTMLGenerator are both concrete 
implementations of the Generator class for generating Go and HTML respectively.

GoTemplate contains the inja template for Go generation
HTMLTemplate contains the inja template for HTML generation

MissingDirectiveException is thrown if a directive required by a generator is missing

Strcase is a partial port of the Golang strcase library (see the note on included open source code for details)

### tilc

Code for the tilc executable is found in the compiler directory. tilc depends on libgen for parsing and serialization of
til source code. 

The Compiler class is responsible for using the libgen library to read and compile til source before writing it to an
output file.

The main file uses the included CLI11 library to parse command line arguments in order to correctly configure the Compiler class

### tell

Code for the tell executable is found in the tell directory. It simply consists of a main file which again utilises the
CLI11 library to parse the command line arguments before utilising the libtil and libgen libraries to deserialize a
til JSON representation and use it to generate the required source (HTML or Go)

### Tests_run

The tests directory contains a suite of tests for the til library. The tests utilise the catch2 testing library 

# Udacity Rubric Compliance

## Loops, Functions, IO

if and swich statements are used in several places. An example of an if statement can be found in the 
til::DocCommentContext::append method in til/DocCommentContext.cpp. An example of a switch statement can be found in 
the std::string gen::HTMLGenerator::MapType method in gen/HTMLGenerator.cpp

All the code is organised into functions. 

For loops (both over iterators and using indexes) are used in many places. An example of the former is found in
the gen::GoGenerator::EnsureDirective method in gen/GoGenerator.cpp and an example of the latter is found in the 
til::ASTValidator::ResolveMessages method in til/ASTValidator.go. 

While loops are also used. An example is found in the til::Lexer::read_next_token method in til/Lexer.cpp

The programs accept input from users in the form of command line arguments. See either compiler/main.cpp or 
tell/main.cpp for examples of this. 

The program also reads from and writes to files. Examples of this can be found in compiler/Compiler.cpp and in
tell/main.cpp

## Object Oriented Programming

All the code except test code and the two main.cpp files is organised into classes and structs.

All the classes use public and private or protected modifiers for data and methods. The serializable structs expose all
their data members by choice.

All classes use member initialization lists where appropriate.

All public methods have comments documenting their function and operation, and appropriate names have been chosen. 

All classes encapsulate their data. All member access is via accessor functions. The serializable structs use public data 
members by choice.

The inheritance hierarchy of the project is fairly flat. There are virtual classes such as TypeDef, with concrete implementations
such as ScalarTypeDef or MapTypeDef. Implementation inheritance is largely avoided. 

An example of a function overload can be found in the gen::GoGenerator class where the MapType method has implementations for
each variant of a SerializableTypeDef

Examples of virtual member overrides can be found in any of the concrete implementations of TypeDef, such as ScalarTypeDef
or MapTypeDef where the t() method is overridden.

There are no examples of templated functions in the project

## Memory Management

The gen::GoGenerator::Generate method takes a reference to a SerializableAST

There are no examples of classes with a customised destructor because all dynamically allocated memory is managed via
std::unique_ptr or std::shared_ptr instances. 

There are no examples of RAII, for the same reason

There are not any strong examples of use of the rule of 5 for the same reason. There are classes which delete their copy constructors and allocators
and so explicitly set their move allocators and constructors, as well as destructors to default but this probably doesn't qualify.

The project extensively uses move semantics to move data instead of copying it. There are many examples of the use of std::move throughout the project

The project makes extensive use of smart pointers such as std::unique_ptr and std::shared_ptr. All the structures in the AST manage their children via
unique pointers. The error reporter class utilised by the lexer and parser is passed around as a shared pointer.

## Concurrency

The project does not make any use of concurrency

