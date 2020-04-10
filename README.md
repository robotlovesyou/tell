# Tell

## Included Open Source Libraries.

To simplify building the project a number of dependencies are included as header only, meaning their source is integrated with this project.

Those libraries are:
1. Catch2 Testing Framework. The header file and license are included as catch.hpp and LICENSE.txt in the include/thirdparty/catch2 directory. See https://github.com/catchorg/Catch2
2. {fmt} string formatting library. The header and source files are included in the include/thirdparty/fmt directory. See https://fmt.dev/6.2.0/index.html

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
Additonally the child field in the MyParentMessage is also marked as optional.

In host languages which support optional types such as Typescript, Kotlin, Rust or Scala, the appropriate optional type should be used. 
In host languages which do not support optional types pointer or nullable representations should be used. For example in Go a string? in tell would map to a *string


1. Create the types to describe the parsed .til file. Ensure they can be serialized as json
2. Write tests for the descriptor serialization
3. Define the tokens
4. Write the lexer + tests
5. Define the AST + write tests
6. Write the parser + tests
7. Write a tree walking interpreter which will generate a descriptor from the AST. 