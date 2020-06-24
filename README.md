# Til/Tell

Til/Tell is an experimental interface definition language (til) and rpc generator (tell) with two key goals.

## 1. Optional

Rather than excluding languages and platforms which do not have an available tell generator, the RPC utilises widely
used technologies (HTTP/JSON) and a very simple implementation pattern, to make hand coding compliant servers and 
clients trivial.

## 2. Simple to Adopt

The til compiler (tilc) outputs JSON as an intermediate representation. Those wishing to create a new generator 
can simply parse the intermediate representation and generate code using the language of their choice. Alternatively
the provided libtil and libgen libraries can be utilised to create generators using C++. Additionally the simple RPC pattern
makes the development of new generators quite easy.

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

### Types

The types supported by tell are:

**Scalar Types**

- **float**: A floating point number.
- **int**: An integer type. 
- **bool** A boolean scalar type.
- **string** A string type. 
- **time** A date time with millisecond precision. Transmitted over the wire as an RFC3339 formatted DateTime string. 

Scalar types are only valid as members of a message. 

**Compound Types**

**Message**: a field of a message may be another message. Infinitely recursive types are disallowed, eg

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

### List types.
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


### Map Types

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

### Optional Types

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

## Services

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

### Calls

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

This requires installation of the Go toolchain. Instructions are included here for your convenience

From the workspace directory:

```
wget https://dl.google.com/go/go1.14.4.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.14.4.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin
mkdir tell/gen_go/calculator
```
