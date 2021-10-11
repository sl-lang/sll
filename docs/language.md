
# The Sl-Lang File Syntax

The building-block of Sll is called an object. An object can represent anything from an integer to a complex expression.

Each object represent either a [base type](#base-types) or an [operator](#operator-types). If an object represents the latter, it must be surrounded by parentheses and contain the symbol for the given operator, as well as its operands.

Single-line comments begin with a semicolon (`; comment`), while block comments have to be surrounded with special symbols (`|# comment #|`).

## Base Types

### Integer

A 64-bit signed integer.

The C equivalents of this type are `sll_integer_t` and `int64_t`.

### Float

A 64-bit IEEE 754 floating-point number.

The C equivalents of this type are `sll_float_t` and `double`.

### Char

A 8-bit unsigned integer.

The C equivalents of this type are `sll_char_t` and `uint8_t`.

### String

An array of **CHAR**s.

The C equivalents of this type are `sll_string_t` and `uint8_t*`.

### Array

An array of objects.

The C equivalent of this type is `sll_array_t`.

### Map

Creates an mapping (association) between different keys and values. The keys and values can represent any object.

The C equivalent of this type is `sll_map_t`.

### Identifier

A name of a variable.

An identifier evaluates to the value of the variable pointed to by the identifier.

### Operation List

A collection of objects to evaluate one by one.

An operation list evaluates to `nil` (zero).

### Constant

A constant is replaced by an integer during the parsing stage.

Here are all of the constants as well as their corresponding integer values:

| Type | Value | Note |
|---|---|---|
| `array_type` | [^1] | Array type (used for casting) |
| `char_type` | [^1] | Character type (used for casting) |
| `false` | `0` | A value which is returned when an expression is not true, ex. by the [equals operator](#equal-) with two different integers |
| `float_type` | [^1] | Float type (used for casting) |
| `handle_type` | [^1] | Handle type (used for casting) |
| `int_type` | [^1] | Integer type (used for casting) |
| `map_type` | [^1] | Map type (used for casting) |
| `map_key_type` | [^1] | Map key type (used for casting) |
| `map_value_type` | [^1] | Map value type (used for casting) |
| `nil` | `0` | This value is returned by an object which does not return anything |
| `stderr` | `-3` | A type recognized by the `file_write` internal function (and other file-related function). It is a handle to the default error stream |
| `stdin` | `-1` | A type recognized by the `file_write` internal function (and other file-related function). It is a handle to the default input stream |
| `stdout` | `-2` | A type recognized by the `file_write` internal function (and other file-related function). It is a handle to the default output stream |
| `string_type` | [^1] | String type (used for casting) |
| `true` | `1` | A value which is returned when an expression is true, ex. by the comparing two equal numbers |

## Operator Types

### Print (`:>`)

#### Arguments

Variable number of arguments (`0` - `2^32-1`)

#### Return value

No return value (`nil`)

#### Description

Converts every argument to a string and writes it to the default output stream.

A print operator can be substituted by the following expression[^2] to obtain the number of bytes written:

```sll
(<- (... "file_write") stdout |# argument 1 #| |# argument 2 #| |# argument 3 #|)
```

### Input (`<:`)

Currently unimplemented.

### Boolean AND (`&&`)

Currently unimplemented.

### Boolean OR (`||`)

Currently unimplemented.

### Boolean NOT (`!`)

### Assignment (`=`)

### Function (`,,,`)

### Internal Function (`...`)

### Inline Function (`***`)

### Function Call (`<-`)

### If (`?`)

### Inline If (`?:`)

### Switch (`??`)

### For-Loop (`->`)

### While-Loop (`>-`)

### Infinite Loop (`><`)

### Addition (`+`)

### Subtraction (`-`)

### Multiplication (`*`)

### Division (`/`)

### Floor Division (`//`)

### Modulo (`%`)

### Bitwise AND (`&`)

### Bitwise OR (`|`)

### Bitwise XOR (Exclusive OR) (`^`)

### Bitwise NOT (`~`)

### Right Shift (`>>`)

### Left Shift (`<<`)

### Less (`<`)

### Less or Equal (`<=`)

### Equal (`==`)

### Not Equal (`!=`)

### More (`>`)

### More or Equal (`>=`)

### Length (`$`)

### Access (`:`)

### Cast (`::`)

### Break (`@`)

### Continue (`<<<`)

### Reference (`%%`)

### Return (`@@`)

### Exit (`@@@`)

### Comma (`,`)

[^1]: The value is determined by a C compile-time constant
[^2]: There is a possible performance penalty when using this method
