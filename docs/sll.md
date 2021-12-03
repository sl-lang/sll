# Overview

## What is Sll

Sll is a **general-purpose programming language** aimed at **all developers** looking for a **non-OOP** language with heavy compile-time optimization (and therefor **better runtime performance**).

## Key Features of Sll

- Cross-platform (Tested on Windows 10 64-bit and multiple Linux x64 distributions)
- Easy to learn (Similar to Lisp)
- Heavy compile-time optimization

## Sll Syntax

The syntax of the Sl Language is similar to Common Lisp (and other Lisp dialects).

It has one notable difference, that is it has no keywords. Sll uses only operators, which makes it easier to learn for people with any spoken language background.

For more specific details, see [Syntax](#syntax)

# Installation

Sll can be download from the [GitHub website](https://github.com/sl-lang/sll/releases/latest). The primary target operating systems are Windows and POISX.

Sll has the ability to be installed in a folder, or as a standalone executable. Here is a list of file names:

- `win.zip`: Windows build of Sll, must be extracted into a folder before use.
- `win_standalone.exe`: Windows standalone version, can be used out-of-the-box.
- `posix.zip`: POSIX build of Sll (platforms such as Linux, Ubuntu, Fedora, etc.). Must be extracted into a single folder.
- `posix_standalone`: POSIX standalone version of Sll.

## Version Checking

If using the standalone version of Sll, replace `sll` by `sll_standalone`. The most recent version of Sll is `0.6.29` (`0.6.30` is in development).

### Windows

To check the version of Sll on Windows, type the following expression into a Command Prompt (or Power Shell):

```batch
sll -V
```

### POSIX

To check the version of Sll on POSIX systems, type the following expression into a Terminal:

```bash
sll -V
```

# Syntax

The syntax of the Sl Language is similar to the syntax of Common Lisp and other Lisp dialects. The only main difference is that Sll does not have any keywords. Sll uses only operators to perform any function. This feature helps people with any language background to learn Sll.

The building-block of the Sl Language is called an [object](#objects). An object can represent anything from an integer to a complex expression.

## Comments

Single-line comments begin with a semicolon, while block comments have to be surrounded with special symbols.

```sll
; Single-line comment

|# Block comment
   (Multiple lines)
#|
```

## Objects

Each object represent either a [base type](#base-types) or an [operator](#operators). If an object represents the latter, it must be surrounded by parentheses and contain the symbol for the given operator, as well as its operands.

All operands are split by white space. If an operator receives more arguments than it requires, it evaluates them as other expressions and not its operands.

### Base Types

Base types have a different syntax than other objects. Unlike operators, they do not require parentheses in their syntax.

#### Integer

A 64-bit signed integer.

By default, integers are represented in base 10, however, by using prefixes, it is possible to write integers in different bases:

- **B**inary (base 2): `0b` (Valid symbols are `0` and `1`)
- **O**ctal (base 8): `0o` (Valid symbols are digits `0` to `7`)
- He**X**adecimal (base 16): `0x` (Valid symbols are digits `0` to `9` and letters from `a` to `f`)

Integers are case-insensitive. The prefixes, as well as the symbols (in case of base 16) can be both uppercase and lowercase.

For clarity purposes, integer digits can be split by underscores (`_`). The only exception is that a number **cannot** begin with an underscore, because it would be recognized as an identifier.

#### Float

A 64-bit IEEE 754 floating-point number. It consists of an option number followed by a period (`.`) and another number. The second number is not required if the first one is present. The entire expression can be followed by an `e` or `E` (denoting the scientific notation) combined with a signed integer representing an exponent. The sign of the number must be located before the first number (or period).

Likewise, with integers, digits can be split by underscores (`_`) for clarity purposes. The only exceptions are that an underscore cannot begin the number nor the exponent.

#### Char

An 8-bit unsigned integer representing a single byte of data. It is denoted by a single character or [escape sequence](#escape-sequences) enclosed in a pair of apostrophes (`''`).

If the apostrophes are empty or surround more than one character or escape sequence, a compile-time error is raised.

##### Escape Sequences

- `\"`: Quotation marks (`"`, ASCII 34)
- `\'`: Apostrophe (`'`, ASCII 39)
- `\\`: Backslash (`\`, ASCII 92)
- `\f`: Form feed (ASCII 12)
- `\n`: Line feed (ASCII 10)
- `\r`: Carriage return (ASCII 13)
- `\t`: Horizontal tab (ASCII 9)
- `\v`: Vertical tab (ASCII 11)
- `\x{value}`: Any character represented in base 16 padded to two digits. (ex. `\x41` maps to `A` and `\x0a` maps to a line feed (`\n`))

Any other character following a backslash will raise a compile-time error.

#### String

An array of [characters](#char). It has to be surrounded by quotation marks (`""`).

Strings support the same [escape sequences](#escape-sequences) as regular characters.

#### Array

An array of objects. It is denoted by an enclosing pair of square brackets (`[]`).

#### Map

Creates a mapping (association) between different keys and values. The keys and values can represent any objects.

A map is constructed by a pair of angle brackets (`<>`) containing key-value pairs. Every object at an even index (starting with zero) is considered a key, whereas objects at odd indexes are values. There cannot be two (strictly) equal keys in the same map.

#### Identifier

A name of a variable. The name can consist of any combination of ASCII letter (uppercase and lowercase), digits or underscores (`_`). The only exceptions are that the name cannot start with a digit. The other exception is that the name cannot be one of the reserved [constants](#constant).

An identifier evaluates to the object of the variable pointed to by the identifier.

#### Operation List

A collection of objects to evaluate in the given order. An operation list is marked by a pair of curly brackets (`{}`) containing zero or more expressions.

An operation list evaluates to `nil` (zero).

#### Constant

The is the list of all of the constants as well as their corresponding integer values:

| Type | Value | Note |
|---|---|---|
| `false` | `0` | A value which is returned when an expression is not true, ex. by the [equal operator](#equal-) with two different integers |
| `nil` | `0` | This value is returned by an expression which does not return anything |
| `true` | `1` | A value which is returned when an expression is true, ex. by the comparing two equal numbers |

The constants are replaced by their integer value during the parsing stage of the program.

### Operators

All operators have the same structure:

```sll
(|# operator type #| |# argument 1 #| |# argument 2 #| |# argument 3 #|)
```

The operator type must be one of the following symbols:

#### Access (`:`)

##### Syntax

```sll
(: |# object #| |# index #|)

OR

(: |# object #| |# start index #| |# end index #|)

OR

(: |# object #| |# start index #| |# end index #| |# increment #|)
```

##### Return Value

Value selected from the given object by the arguments, or, if the object does not support item indexing, the same object

##### Description

If the object does not support item indexing (i.e. it is not of a storage type) the same object is returned. Otherwise, the return value depends on the number of arguments.

If there is only one argument, the value at that index in the given object is returned.

A container refers to a string if the object is a string, otherwise it is an array.

If there are two arguments, all of the values between the `start index` and `end index`. If `start index` is bigger than or equal to `end index`, an empty container is returned.

Otherwise, when all of the 3 arguments are supplied, every value between `start index` and `end index`, `increment` indexes apart is returned in a container. If `increment` is equal to zero, an empty container is returned.

If the object is a mapping and there is more than one index, a list of values at these indexes is returned. The following two examples evaluate to the same value:
```sll
(= map <'a' 0 'b' 1 'c' 2 'd' 3>)

(= x (: map 'a' 'c'))
|# is the same as #|
(= x [
	(: map 'a')
	(: map 'c')
])
```

##### Example

```sll
(= array [0 1 2 3 4 5 6 7 8 9])
(-> (= i 0) (< i ($ array)) {
	(:> "Array at index " i " is equal to " (: array i) "\n")
	(= i (+ i 1))
})
```

#### Addition (`+`)

##### Syntax

```sll
(+ |# term 1 #| |# term 2 #| |# term 3 #|)
```

##### Return value

The sum of the operands

##### Description

All of the operands are evaluated, added together and the return value is returned.

##### Example

```sll
(:> (+ 1 2 3) "\n")
```

#### And (`&&`)

##### Syntax

```sll
(&& |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

`true` if all of the expression evaluate to `true`, otherwise `false`

##### Description

All expressions are evaluated in the specified order. If all of the expressions evaluate to `true`, then the entire expression returns the value `true`. If any of the expressions evaluate to `false`, the evaluation stops and the value `false` is returned.

##### Example

```sll
(= x 6)
(? (&& (> x 5) (< x 10))
	(:> x " is between 5 and 10!\n")
)
```

#### Assignment (`=`)

##### Syntax

```sll
(= |# variable #| |# value #|)
```

##### Return value

The value of the second operand.

##### Description

The value of the second operand is evaluated and assigned to the variable pointed by the first operand. If the first operand is not an identifier, the entire object behaves like an [operation list](#operation-list).

##### Example

```sll
(= name "John")
```

#### Bitwise AND (`&`)

##### Syntax

```sll
(& |# term 1 #| |# term 2 #| |# term 3 #|)
```

##### Return value

The bitwise AND product of all of the terms

##### Description

All of the operands are evaluated, and the bitwise AND product of all of the terms is returned.

##### Example

```sll
(:> (& 0b101 0b1111) "\n")
```

#### Bitwise NOT (One's Complement) (`~`)

##### Syntax

```sll
(~ |# expression #|)
```

##### Return value

The bitwise NOT (One's complement) of the value is return

##### Description

The one's complement (bitwise NOT) of the first (and only) operand is returned.

##### Example

```sll
(:> (& (~ 0xa5) 0xff) "\n")
```

#### Bitwise OR (`|`)

##### Syntax

```sll
(| |# term 1 #| |# term 2 #| |# term 3 #|)
```

##### Return value

The bitwise OR product of all of the terms

##### Description

All of the operands are evaluated, and the bitwise OR product of all of the terms is returned.

##### Example

```sll
(:> (| 0x0f 0x80) "\n")
```

#### Bitwise XOR (Exclusive OR) (`^`)

##### Syntax

```sll
(^ |# term 1 #| |# term 2 #| |# term 3 #|)
```

##### Return value

The bitwise XOR (exclusive or) product of all of the terms

##### Description

All of the operands are evaluated, and the bitwise XOR (exclusive or) product of all of the terms is returned.

##### Example

```sll
(:> (^ 0xff 0x80) "\n")
```

#### Boolean (`!!`)

##### Syntax

```sll
(!! |# expression #|)
```

##### Return value

`true` if the boolean value of the expression is nonzero, otherwise `false`

##### Description

The first (and only) operand is evaluated, its value is converted to a boolean and returned.

##### Example

```sll
(:> (!! 0.0) ", " (!! 0.1) "\n")
```

#### Boolean Not (`!`)

##### Syntax

```sll
(! |# expression #|)
```

##### Return value

`true` if the boolean value of the expression is zero, otherwise `false`

##### Description

The first (and only) operand is evaluated, its value is converted to a boolean and the inverse is returned.

##### Example

```sll
(:> (! 0) ", " (! 1) "\n")
```

#### Break (`@`)

##### Syntax

```sll
(@)
```

##### Return Value

Moves the execution outside of the current loop

##### Description

Moves to instruction pointer after the current innermost loop.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(:> "Number: " i "\n")
	(? (== i 5) {
		(:> "Early Exit!")
		(@)
	})
	(= i (+ i 1))
})
```

#### Function Call (`<-`)

##### Syntax

```sll
(<- |# function expression #| |# function argument 1 #| |# function argument 2 #| |# function argument 3 #|)
```

##### Return value

The return value of the function called, or `nil`

##### Description

This object calls the function specified by the first operand with the arguments specified by the consecutive operands. If the function operand is an invalid function or internal function ID, a `nil` value is returned.

##### Example

```sll
(<- (... "file_write") stdout "Example Code\n")
```

#### Cast (`::`)

##### Syntax

```sll
(:: |# object #| |# type 1 #| |# type 2 #| |# type 3 #|)
```

##### Return Value

The casted object

##### Description

The given object is consequently casted to each of the given types. The new, casted object is returned.

##### Example

```sll
(# int_type)
(-- "types.sll")

(= value 1.678)
(= floored_value (:: value int_type))
```

#### Comma (`,`)

##### Syntax

```sll
(, |# expression 1 #| |# expression 2 #| |# expression 3 #| |# returning expression #|)
```

##### Return Value

The last value of the expression

##### Description

All expression are evaluated and the value of the last one is returned.

##### Example

```sll
(= value (, (:> "Assigning '5' to variable value...") 5))
```

#### Continue (`<<<`)

##### Syntax

```sll
(<<<)
```

##### Return Value

Execution is directed to the beginning of the loop

##### Description

The instruction pointer is moved to the beginning of the loop, i.e. the just before the condition.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(? (== i 5) {
		(:> "Skipping 5!")
		(= i (+ i 1))
		(<<<)
	})
	(:> "Number: " i "\n")
	(= i (+ i 1))
})
```

#### Declaration (`#`)

##### Syntax

```sll
(# |# variable 1 #| |# variable 2 #| |# variable 3 #|)
```

##### Return Value

No return value (`nil`)

##### Description

The variables are declared and not initialized. Their value is undefined.

##### Example

```sll
(# stdout)
(-- "file.sll")

(:> "Standard output file index: " stdout "\n")
```

#### Division (`/`)

##### Syntax

```sll
(/ |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

##### Return value

The quotient

##### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors and the quotient is returned.

##### Example

```sll
(:> (/ 12 4) "\n")
```

#### Equal (`==`)

##### Syntax

```sll
(== |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression are equal, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If a pair of expressions does not evaluate to the same value, the evaluation ends and `false` is returned. If all of the evaluated expression are equal, `true` is returned.

##### Example

```sll
(= age 20)

(? (== age 20)
	(:> "You are 20 years old!")
)
```

#### Exit (`@@@`)

##### Syntax

```sll
(@@@ |# value #|)
```

##### Return Value

Execution of the program ends with the value as the return code

##### Description

The program is terminated and the `value` is casted to a 32-bit signed integer and returned as the return code.

##### Example

```sll
(@@@ 0)
```

#### Export (`##`)

##### Syntax

```sll
(## |# variable 1 #| |# variable 2 #| |# variable 3 #|)
```

##### Return Value

No return value (`nil`)

##### Description

Each of the variables are internally marked as exported and will not be removed by the optimizer.

##### Example

```sll
(= export_value 12345)

(## export_value)
```

#### Floor Division (`//`)

##### Syntax

```sll
(// |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

##### Return value

The quotient

##### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors, the result is rounded down and returned.

##### Example

```sll
(:> (// 13 4) "\n")
```

#### For Loop (`->`)

##### Syntax

```sll
(-> |# initialization #| |# condition #|
	|# loop body #|
)
```

##### Return Value

No return value (`nil`)

##### Description

First, the initialization code is executed. Then, while the condition evaluates to `true`, the loop body is executed.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(:> "Number: " i '\n')
	(= i (+ i 1))
})
```

#### Function Declaration (`,,,`)

##### Syntax

```sll
(,,, |# argument 1 #| |# argument 2 #| |# argument 3 #|
	|# function body #|
)
```

##### Return value

An integer, which can be used to call the given function.

##### Description

The object defines a function. The first `n` consecutive identifiers are considered the arguments, and the rest of the operands (`operand_count - n`) are considered the body of the function.

##### Example

```sll
(= mult_func (,,, x y
	(:> x " * " y " = " (* x y) "\n")
	(@@ (* x y))
))
```

#### If (`?`)

##### Syntax

```sll
(? |# condition 1 #| |# code block 1 #|
	|# condition 2 #| |# code block 2 #|
	|# condition 3 #| |# code block 3 #|
	|# 'else' code block #|
)
```

##### Return value

No return value (`nil`)

##### Description

Condition blocks are evaluated in the given order until one evaluates to a non-zero value. The matching code block is executed. If every condition was false and an 'else' block is specified, it is executed.

##### Example

```sll
(= x 3)
(? (< x 5) {
	(:> "below")
} (> x 5) {
	(:> "above")
} {
	(:> "equal")
})
```

#### Import (`--`)

##### Syntax

```sll
(-- "file_to_import_1.sll" "file_to_import_2.sll" "file_to_import_3.sll")
```

##### Return Value

No return value (`nil`)

##### Description

Each of the string arguments is treated as a file path. This can either be a built-in module name or an external file. If the file cannot be found, a compile-time error is generated.

##### Example

```sll
(# file stdout)
(-- "file.sll")

(<- file$write stdout "Hello, World!\n")
```

#### Infinite Loop (`><`)

##### Syntax

```sll
(>< |# initialization #|
	|# loop body #|
)
```

##### Return Value

No return value (`nil`)

##### Description

First, the initialization code is executed. Then, the loop body is executed repeatedly.

##### Example

```sll
(>< (= i 0) {
	(? (== i 10) (@))
	(:> "Number: " i '\n')
	(= i (+ i 1))
})
```

#### Inline Function (`***`)

##### Syntax

```sll
(*** |# function body #|)
```

##### Return value

The value returned by a return object, or `nil`

##### Description

This object works just like a function called without arguments. It can be used to write expressions more complex than [inline-if objects](#inline-if-).

##### Example

```sll
(***
	(-> (= i 0) (< i 10) {
		(? (= i 5) (@@ i))
		(= i (+ i 1))
	})
)
```

#### Inline If (`?:`)

##### Syntax

```sll
(?: |# condition 1 #| |# return value 1 #|
	|# condition 2 #| |# return value 2 #|
	|# condition 3 #| |# return value 3 #|
	|# 'else' return value #|
)
```

##### Return value

The value returned by the given return block.

##### Description

Condition blocks are evaluated in the given order until one evaluates to a non-zero value. The matching code block is evaluated, and the return value is returned. If every condition was false and an 'else' block is specified, it is evaluated and returned. Otherwise, if no 'else' block is specified, a `nil` value is returned.

##### Example

```sll
(= x 3)
(:> "The number is " (?:
	(< x 5) "below"
	(> x 5) "above"
	"equal"
) " five.\n")
```

#### Internal Function Declaration (`...`)

##### Syntax

```sll
(... "internal_function_name")
```

##### Return value

An integer, which can be used to invoke the given internal function.

##### Description

The object declares an internal function. The operand is a string, which will be used to look-up the ID of the internal function. If the operand is not a string, the object behaves like an [operation list](#operation-list).

##### Example

```sll
(= parse_json (... "json_parse"))
```

#### Left Bit Shift (`<<`)

##### Syntax

```sll
(<< |# object #| |# amount #|)
```

##### Return Value

The new object

##### Description

The given objects is shifted left by a given amount. For integers, floats and chars, this is equivalent to multiplying by `2**(amount)`. For arrays, this means that `amount` zeros are prepended to the array.

##### Example

```sll
(:> "Bit 16 multiplied by 4: " (<< 16 2) "\n")
```

#### Length (`$`)

##### Syntax

```sll
($ |# object #|)
```

##### Return Value

The length of the object

##### Description

If the object has a storage type, the number of elements (length) of the given object is returned. Otherwise, `0` is returned.

##### Example

```sll
(= arr [0 1 2 3 4 5 6 7 8 9])
(:> "Length of array: " ($ arr) "\n")
```

#### Less (`<`)

##### Syntax

```sll
(< |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression get progressively bigger, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If the first element in a pair of expressions is not smaller than the second one, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(:> "Number: " i '\n')
	(= i (+ i 1))
})
```

#### Less or Equal (`<=`)

##### Syntax

```sll
(<= |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression do not get progressively smaller, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If the first element in a pair of expressions is bigger than the second one, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(-> (= i 0) (<= i 9) {
	(:> "Digit: " i '\n')
	(= i (+ i 1))
})
```

#### Modulo (`%`)

##### Syntax

```sll
(% |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

##### Return value

The remainder

##### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors and the remainder is returned.

##### Example

```sll
(:> (% 13 4) "\n")
```

#### More (`>`)

##### Syntax

```sll
(> |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression get progressively bigger, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If the first element in a pair of expressions is not bigger than the second one, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(?
		(> i 4) (:> "Round up: " i "\n")
		(:> "Round down: " i "\n")
	)
	(= i (+ i 1))
})
```

#### More or Equal (`>=`)

##### Syntax

```sll
(>= |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression do not get progressively bigger, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If the first element in a pair of expressions is smaller than the second one, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(-> (= i 0) (< i 10) {
	(?
		(>= i 5) (:> "Round up: " i "\n")
		(:> "Round down: " i "\n")
	)
	(= i (+ i 1))
})
```

#### Multiplication (`*`)

##### Syntax

```sll
(* |# factor 1 #| |# factor 2 #| |# factor 3 #|)
```

##### Return value

The product of the operands

##### Description

All of the operands are evaluated, multiplied together and the value is returned.

##### Example

```sll
(:> (* 5 4 3 2 1) "\n")
```

#### New (`.`)

##### Syntax

```sll
(. |# type id #|
	|# field value 1 #|
	|# field value 2 #|
	|# field value 3 #|
)
```

##### Return Value

The new object

##### Description

Constructs a new object specified by the given type. If the type is a built-in type, the field values are interpreted as in the syntax of the given type (see [Base Types](#base-types)). Otherwise, the field values are assigned following the structure declaration of the given type. If the `type id` specifies an unknown type, it is assumed to be an integer (`int_type`).

##### Example

```sll
(# float_type)
(-- "type.sll")

(= point_type (&:
	float_type "x"
	float_type "y"
))

(= p (. point_type -5 -4))
(:> "Point: " p "\nUnique object: " (. (&:)) "\n")
```

#### Not Equal (`!=`)

##### Syntax

```sll
(!= |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If each pair of expressions evaluates to two different values, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If a pair of expressions evaluates to the same value, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(= a 1)
(= b 2)

(? (!= a b)
	(:> "Two different numbers!")
)
```

#### Or (`||`)

##### Syntax

```sll
(|| |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

`true` if at least one expression evaluate to `true`, otherwise `false`

##### Description

All expressions are evaluated in the specified order. If one of the expressions evaluate to `true`, the evaluation process ends and `true` is returned. If all of the expressions evaluate to `false`, the value `false` is returned.

##### Example

```sll
(= x 6)
(? (|| (<= x 5) (>= x 10))
	(:> x " is NOT between 5 and 10!\n")
)
```

#### Print (`:>`)

##### Syntax

```sll
(:> |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return value

No return value (`nil`)

##### Description

Converts every argument to a string and writes it to the default output stream.

A print operator can be substituted by the following expression to obtain the number of bytes written:

```sll
(<- (... "file_write") -2 |# argument 1 #| |# argument 2 #| |# argument 3 #|)
```

##### Example

```sll
(:> "Array: " [1 2 3 4] "\n")
```

#### Reference (`%%`)

##### Syntax

```sll
(%%)

OR

(%% |# object #|)
```

##### Return Value

A handle to the object pointer, or a null pointer

##### Description

If there are no arguments, a handle to a null pointer is returned. Otherwise, a handle to the memory location of the given object is returned.

##### Example

```sll
(= x 5)

(:> "Address of 'x' in memory: " (%% x) '\n')
```

#### Return (`@@`)

##### Syntax

```sll
(@@)

OR

(@@ |# object #|)
```

##### Return Value

Execution is returned back to the function call with the return value of `object` or `nil`, if no object is supplied

##### Description

The instruction pointer is moved back to the place of the enclosing function call, which returns the `object` if it is supplied, otherwise returns `nil`.

##### Example

```sll
(= mult (,,, a b {
	(@@ (* a b))
}))
```

#### Right Bit Shift (`>>`)

##### Syntax

```sll
(>> |# object #| |# amount #|)
```

##### Return Value

The new object

##### Description

The given objects is shifted right by a given amount. For integers, floats and chars, this is equivalent to dividing by `2**(amount)`. For arrays, this means that `amount` elements are removed from the beginning of the array.

##### Example

```sll
(:> "Bit 16 divided by 4: " (>> 16 2) "\n")
```

#### Strict Equal (`===`)

##### Syntax

```sll
(=== |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If all of the expression (and their types) are equal, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If a pair of expressions does not evaluate to the same type or the same value, the evaluation ends and `false` is returned. If all of the evaluated expression have the same type and are equal, `true` is returned.

##### Example

```sll
(= var 5.0)

(? (=== var 5.0)
	(:> "Float with a value of 5!")
)
```

#### Strict Not Equal (`!==`)

##### Syntax

```sll
(!== |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

##### Return Value

If each pair of expressions evaluates to two different types or values, `true` is returned, otherwise `false`

##### Description

All of the expressions are evaluated sequentially. If a pair of expressions evaluates to the same type and the same value, the evaluation ends and `false` is returned. Otherwise, `true` is returned.

##### Example

```sll
(= var 5)

(? (!== var 5.0)
	(:> "'var' should be a float with a value of 5!")
)
```

#### Structure Declaration (`&:`)

##### Syntax

```sll
(&:
	|# type 1 #| |# field name 1 #|
	|# type 2 #| |# field name 2 #|
	|# type 3 #| |# field name 3 #|
)
```

##### Return Value

New type ID

##### Description

A new type is created with respect to the field types and names specified by the arguments. If the number of arguments is odd, the last type is ignored.

##### Example

```sll
(# int_type)
(-- "type.sll")

(= point_type (&:
	int_type x
	int_type y
))

(:> "Point: " (. point_type 1 -2) "\n")
```

#### Subtraction (`-`)

##### Syntax

```sll
(- |# minuend 1 #| |# subtrahend 1 #| |# subtrahend 2 #|)
```

##### Return value

The difference of the operands

##### Description

All of the operands are evaluated, subtracted from the first operand and returned.

##### Example

```sll
(:> (- 6 1 2) "\n")
```

#### Switch (`??`)

##### Syntax

```sll
(?? |# condition #|
	|# case 1 #| |# expression 1 #|
	|# case 2 #| |# expression 2 #|
	|# case 3 #| |# expression 3 #|
	|# default case expression #|
)
```

##### Return value

No return value (`nil`)

##### Description

The condition expression is evaluated. If it is not an integer or character, the default case expression is evaluated (if it exists). Every case expression is evaluated. Any case expressions that are not integers or characters are skipped. The expression block is selected based on the condition value. If no code block is found and the default case expression exists, it is evaluated.

##### Example

```sll
(= x -1)
(?? x
	-1 (:> "below")
	1 (:> "above")
	(:> "equal")
)
```

#### Typeof (`:?`)

##### Syntax

```sll
(:? |# object #|)
```

##### Return Value

The type of the object

##### Description

An integer representing the type of the given object is returned.

##### Example

```sll
(:> "The type of an integer has the following ID: " (:? 12345) "\n")
```

#### While Loop (`>-`)

##### Syntax

```sll
(>- |# initialization #| |# condition #|
	|# loop body #|
)
```

##### Return Value

No return value (`nil`)

##### Description

First, the initialization code is executed. Then, the loop body is evaluated once, after which, while the condition evaluates to `true`, the loop body is executed again.

##### Example

```sll
(>- (= i 0) (<= i 9) {
	(:> "Digit: " i '\n')
	(= i (+ i 1))
})
```
