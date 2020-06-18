# Tell

## Included Open Source Libraries.

To simplify building the project a number of dependencies are included as header only, meaning their source is integrated with this project.

Those libraries are:
1. Catch2 Testing Framework. The header file and license are included as catch.hpp and LICENSE.txt in the include/thirdparty/catch2 directory. See https://github.com/catchorg/Catch2
2. {fmt} string formatting library. The header and source files are included in the include/thirdparty/fmt directory. See https://fmt.dev/6.2.0/index.html
3. nlohmann/json json parsing/serializing/deserializing library included in include/thirdparty/nlohmann directory. See https://github.com/nlohmann/json
4. CLI11 is a command line parsing library. It is included in include/thirdparty/cli11. See https://github.com/CLIUtils/CLI11 for details

In addition to the libraries mentioned above, the gen library includes a partial port of the strcase library from Go to C++. 
See https://github.com/iancoleman/strcase for the original source code.   

## Til Interface Language

### Types

The types supported by tell are:

**Scalar Types**

- **float**: A floating point number. Should map to a 64 bit float or fall back to highest available floating point scalar type.
- **int**: An integer type. Should map to a 64 bit int or fall back to the highest available integer scalar type
- **bool** A boolean scalar type.
- **string** A string type. 
- **time** A date time with millisecond precision. Transmitted over the wire as an RFC3339 DateTime string. Should be parsed to and from an appropriate DateTime type.

Scalar types are only valid as members of a message. 

**Compound Types**

- **message**: A structure composed of one or more scalar or compound types.

```
message MyMessage {
    a_string_field: string
    an_integer_field: int
    a_bool_field: bool
}
```

---

### List types.
Any field can be a list of another type, be it a scalar or compound type. 

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

Any field can be a map type. The key of map types is always a string. The value can be any other type. 

```
message MyMapMessage {
    a_map_of_ints: map[int]
    a_map_of_messages: map[MyMessage]
    an_optional_map_of_messages: map[MyMessage]?
}
```

In the example above there is a field which is a map from string to int, a field which is a map from string to MyMessage
and an optional field which is a map from string to MyMessage

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

In host languages which support optional types such as Typescript, Kotlin, Rust or Scala, the appropriate optional type should be used. 
In host languages which do not support optional types pointer or nullable representations should be used. For example string? in tell would map to a *string in Go.

## Further Work

The status of til/tell is entirely experimental. It is not ready for production use.

Further work could include:
- Imports: The current implementation skips the import of packages for the sake of simplicity. A real world implementation would certainly require this.
- Formal language specification for til. Currently the language is only specified by the compiler code which does not make for good documentation. 
- Enums: Very useful for describing things such as error codes in a single location.
- Streams: Streams could be implemented using websockets, which are widely supported.
- Editor plugins: Code highlighting can make development less error prone and faster. 


1. Create the types to describe the parsed .til file. Ensure they can be serialized as json
2. Write tests for the descriptor serialization
3. Define the tokens
4. Write the lexer + tests
5. Define the AST + write tests
6. Write the parser + tests
7. Write a tree walking interpreter which will generate a descriptor from the AST. 