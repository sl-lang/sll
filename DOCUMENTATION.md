# Introduction to Sll

## Overview

### What is Sll?

Sll is a **general-purpose programing language** aimed at **all developers** looking for a **non-OOP** language with heavy compile-time optimization (and therefor **better runtime performance**).

### What can it be used for?

Sll can be used for many different applications, from back-end server-side software development to complex AI-driven path-finding systems.

### Key features of Sll

- Cross-platform (Tested on Windows 10 64-bit and multiple Linux x64 distributions)
- Easy to learn (Similar to Lisp)
- Heavy compile-time optimization

### Sll syntax

The syntax of the Sl Language is similar to Common Lisp (and other Lisp dialects).

It has one notable difference, that is it has no keywords. Sll uses only operators, which makes it easier to learn for people with any spoken language background.

For more specific details, see [Syntax](#syntax)

## Installation

Sll can be download from the [GitHub website](https://github.com/sl-lang/sll/releases/latest). The primary target operating systems are Windows and POISX.

Sll has the ability to be installed in a folder, or as a standalone executable. Here is a list of file names:

- `win.zip`: Windows build of Sll, must be extracted into a folder before use.
- `win_standalone.exe`: Windows standalone version, can be used out-of-the-box.
- `posix.zip`: POSIX build of Sll (platforms such as Linux, Ubuntu, Fedora, FreeBSD, etc.). Must be extracted into a single folder.
- `posix_standalone`: POSIX standalone version of Sll.

### Version Checking

If using the standalone version of Sll, replace `sll` by `sll_standalone`. The most recent version of Sll is `0.6.27` (`0.6.28` is in development).

#### Windows

To check the version of Sll on Windows, type the following expression into a Command Prompt (or Power Shell):

```batch
sll -V
```

#### POSIX

To check the version of Sll on POSIX systems, type the following expression into a Terminal:

```bash
sll -V
```

## Syntax

The syntax of the Sl Language is similar to the syntax of Common Lisp and other Lisp dialects. The only main difference is that Sll does not have any keywords. Sll uses only operators to perform any function. This feature helps people with any language background to learn Sll.

The building-block of the Sl Language is called an [object](#objects). An object can represent anything from an integer to a complex expression.

### Comments

Single-line comments begin with a semicolon, while block comments have to be surrounded with special symbols.

``` sll
; Single-line comment

|# Block comment
   (Multiple lines)
#|
```

### Objects

Each object represent either a [base type](#base-types) or an [operator](#operators). If an object represents the latter, it must be surrounded by parentheses and contain the symbol for the given operator, as well as its operands.

All operands are split by whitespace. If an operator receives more arguments than it requires, it evaluates them as other expressions and not its operands.

#### Base Types

Base types have a different syntax than other objects. Unlike operators, they do not require parentheses in their syntax.

##### Integer

A 64-bit signed integer.

By default, integers are represented in base 10, however, by using prefixes, it is possible to write integers in different bases:

- **B**inary (base 2): `0b` (Valid symbols are `0` and `1`)
- **O**ctal (base 8): `0o` (Valid symbols are digits `0` to `7`)
- He**X**adecimal (base 16): `0x` (Valid symbols are digits `0` to `9` and letters from `a` to `f`)

Integers are case-insensitive. The prefixes, as well as the symbols (in case of base 16) can be both uppercase and lowercase.

For clarity purposes, integers digits can be split by underscores (`_`). The only exception is that a number **can not** begin with an underscore, because it would be recognized as an identifier.

##### Float

A 64-bit IEEE 754 floating-point number.

##### Char

An 8-bit unsigned integer representing a character.

##### String

An array of [characters](#char).

##### Array

An array of objects.

##### Map

Creates a mapping (association) between different keys and values. The keys and values can represent any objects.

##### Identifier

A name of a variable.

An identifier evaluates to the value of the variable pointed to by the identifier.

##### Operation List

A collection of objects to evaluate one by one.

An operation list evaluates to `nil` (zero).

##### Constant

The is the list of all of the constants as well as their corresponding integer values:

| Type | Value | Note |
|---|---|---|
| `false` | `0` | A value which is returned when an expression is not true, ex. by the [equal operator](#equal-) with two different integers |
| `nil` | `0` | This value is returned by an expression which does not return anything |
| `true` | `1` | A value which is returned when an expression is true, ex. by the comparing two equal numbers |

The constants are replaced by their integer value during the parsing stage of the program.

#### Operators

All operators have the same structure:

```sll
(|# operator type #| |# argument 1 #| |# argument 2 #| |# argument 3 #|)
```

The operator type must be one of the following symbols:

##### Access (`:`)

###### Syntax

```sll
(:)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(:)
```

##### Addition (`+`)

###### Syntax

```sll
(+ |# term 1 #| |# term 2 #| |# term 3 #|)
```

###### Return value

The sum of the operands

###### Description

All of the operands are evaluated, added together and the return value is returned.

###### Example

```sll
(:> (+ 1 2 3) "\n")
```

##### And (`&&`)

###### Syntax

```sll
(&&)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(&&)
```

##### Assignment (`=`)

###### Syntax

```sll
(= |# variable #| |# value #|)
```

###### Return value

The value of the second operand.

###### Description

The value of the second operand is evaluated and assigned to the variable pointed by the first operand. If the first operand is not an identifier, the entire object behaves like an [operation list](#operation-list).

###### Example

```sll
(= variable "value" (:> "Extra expression"))
```

##### Bitwise AND (`&`)

###### Syntax

```sll
(& |# term 1 #| |# term 2 #| |# term 3 #|)
```

###### Return value

The bitwise AND product of all of the terms

###### Description

All of the operands are evaluated, and the bitwise AND product of all of the terms is returned.

###### Example

```sll
(:> (& 0b101 0b1111) "\n")
```

##### Bitwise NOT (One's Complement) (`~`)

###### Syntax

```sll
(~ |# expression #|)
```

###### Return value

The bitwise NOT (One's complement) of the value is return

###### Description

The one's complement (bitwise NOT) of the first (and only) operand is returned.

###### Example

```sll
(:> (& (~ 0xa5) 0xff) "\n")
```

##### Bitwise OR (`|`)

###### Syntax

```sll
(| |# term 1 #| |# term 2 #| |# term 3 #|)
```

###### Return value

The bitwise OR product of all of the terms

###### Description

All of the operands are evaluated, and the bitwise OR product of all of the terms is returned.

###### Example

```sll
(:> (| 0x0f 0x80) "\n")
```

##### Bitwise XOR (Exclusive OR) (`^`)

###### Syntax

```sll
(^ |# term 1 #| |# term 2 #| |# term 3 #|)
```

###### Return value

The bitwise XOR (exclusive or) product of all of the terms

###### Description

All of the operands are evaluated, and the bitwise XOR (exclusive or) product of all of the terms is returned.

###### Example

```sll
(:> (^ 0xff 0x80) "\n")
```

##### Boolean Not (`!`)

###### Syntax

```sll
(! |# expression #|)
```

###### Return value

The inverted value

###### Description

The first (and only) operand is evaluated, its value is converted to a boolean and the inverse is returned.

###### Example

```sll
(:> (! 0) ", " (! 1) "\n")
```

##### Break (`@`)

###### Syntax

```sll
(@)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(@)
```

##### Function Call (`<-`)

###### Syntax

```sll
(<- |# function expression #| |# function argument 1 #| |# function argument 2 #| |# function argument 3 #|)
```

###### Return value

The return value of the function called, or `nil`

###### Description

This object calls the function specified by the first operand with the arguments specified by the consecutive operands. If the function operand is an invalid function or internal function ID, a `nil` value is returned.

###### Example

```sll
(<- (... "file_write") stdout "Example Code\n")
```

##### Cast (`::`)

###### Syntax

```sll
(::)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(::)
```

##### Comma (`,`)

###### Syntax

```sll
(,)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(,)
```

##### Continue (`<<<`)

###### Syntax

```sll
(<<<)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(<<<)
```

##### Declaration (`#`)

###### Syntax

```sll
(#)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(#)
```

##### Division (`/`)

###### Syntax

```sll
(/ |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

###### Return value

The quotient

###### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors and the quotient is returned.

###### Example

```sll
(:> (/ 12 4) "\n")
```

##### Equal (`==`)

###### Syntax

```sll
(==)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(==)
```

##### Exit (`@@@`)

###### Syntax

```sll
(@@@)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(@@@)
```

##### Export (`##`)

###### Syntax

```sll
(##)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(##)
```

##### Floor Division (`//`)

###### Syntax

```sll
(// |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

###### Return value

The quotient

###### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors, the result is rounded down and returned.

###### Example

```sll
(:> (// 13 4) "\n")
```

##### For Loop (`->`)

###### Syntax

```sll
(->)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(->)
```

##### Function Declaration (`,,,`)

###### Syntax

```sll
(,,, |# argument 1 #| |# argument 2 #| |# argument 3 #|
	|# function body #|
)
```

###### Return value

An integer, which can be used to call the given function.

###### Description

The object defines a function. The first `n` consecutive identifiers are considered the arguments, and the rest of the operands (`operand_count - n`) are considered the body of the function.

###### Example

```sll
(= mult_func (,,, x y
	(:> x " * " y " = " (* x y) "\n")
	(@@ (* x y))
))
```

##### If (`?`)

###### Syntax

```sll
(? |# condition 1 #| |# code block 1 #|
	|# condition 2 #| |# code block 2 #|
	|# condition 3 #| |# code block 3 #|
	|# 'else' code block #|
)
```

###### Return value

No return value (`nil`)

###### Description

Condition blocks are evaluated in the given order until one evaluates to a non-zero value. The matching code block is executed. If every condition was false and an 'else' block is specified, it is executed.

###### Example

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

##### Import (`--`)

###### Syntax

```sll
(--)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(--)
```

##### Infinite Loop (`><`)

###### Syntax

```sll
(><)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(><)
```

##### Inline Function (`***`)

###### Syntax

```sll
(*** |# function body #|)
```

###### Return value

The value returned by a return object, or `nil`

###### Description

This object works just like a function called without arguments. It can be used to write expressions more complex than [inline-if objects](#inline-if-).

###### Example

```sll
(***
	(-> (= i 0) (< i 10) {
		(? (= i 5) (@@ i))
	})
)
```

##### Inline If (`?:`)

###### Syntax

```sll
(?: |# condition 1 #| |# return value 1 #|
	|# condition 2 #| |# return value 2 #|
	|# condition 3 #| |# return value 3 #|
	|# 'else' return value #|
)
```

###### Return value

The value returned by the given return block.

###### Description

Condition blocks are evaluated in the given order until one evaluates to a non-zero value. The matching code block is evaluated and the return value is returned. If every condition was false and an 'else' block is specified, it is evaluated and returned. Otherwise, if no 'else' block is specified, a `nil` value is returned.

###### Example

```sll
(= x 3)
(:> (?:
	(< x 5) "below"
	(> x 5) "above"
	"equal"
))
```

##### Input (`<:`)

###### Syntax

```sll
(<:)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(<:)
```

##### Internal Function Declaration (`...`)

###### Syntax

```sll
(... "internal_function_name")
```

###### Return value

An integer, which can be used to invoke the given internal function.

###### Description

The object declares an internal function. The operand is a string, which will be used to look-up the ID of the internal function. If the operand is not a string, the object behaves like an [operation list](#operation-list).

###### Example

```sll
(= parse_json (... "json_parse"))
```

##### Left Bit Shift (`<<`)

###### Syntax

```sll
(<<)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(<<)
```

##### Length (`$`)

###### Syntax

```sll
($)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
($)
```

##### Less (`<`)

###### Syntax

```sll
(<)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(<)
```

##### Less Or Equal (`<=`)

###### Syntax

```sll
(<=)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(<=)
```

##### Modulo (`%`)

###### Syntax

```sll
(% |# dividend 1 #| |# divisor 1 #| |# divisor 2 #|)
```

###### Return value

The remainder

###### Description

All of the operands are evaluated, the first operand (the dividend) is divided by all of the divisors and the remainder is returned.

###### Example

```sll
(:> (% 13 4) "\n")
```

##### More (`>`)

###### Syntax

```sll
(>)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(>)
```

##### More Or Equal (`>=`)

###### Syntax

```sll
(>=)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(>=)
```

##### Multiplication (`*`)

###### Syntax

```sll
(* |# factor 1 #| |# factor 2 #| |# factor 3 #|)
```

###### Return value

The product of the operands

###### Description

All of the operands are evaluated, multiplied together and the value is returned.

###### Example

```sll
(:> (* 5 4 3 2 1) "\n")
```

##### Not Equal (`!=`)

###### Syntax

```sll
(!=)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(!=)
```

##### Or (`||`)

###### Syntax

```sll
(||)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(||)
```

##### Print (`:>`)

###### Syntax

```sll
(:> |# expression 1 #| |# expression 2 #| |# expression 3 #|)
```

###### Return value

No return value (`nil`)

###### Description

Converts every argument to a string and writes it to the default output stream.

A print operator can be substituted by the following expression[^2] to obtain the number of bytes written:

```sll
(<- (... "file_write") -2 |# argument 1 #| |# argument 2 #| |# argument 3 #|)
```

###### Example

```sll
(:> "Array: " [1 2 3 4] "\n")
```

##### Reference (`%%`)

###### Syntax

```sll
(%%)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(%%)
```

##### Return (`@@`)

###### Syntax

```sll
(@@)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(@@)
```

##### Right Bit Shift (`>>`)

###### Syntax

```sll
(>>)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(>>)
```

##### String Equal (`===`)

###### Syntax

```sll
(===)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(===)
```

##### String Not Equal (`!==`)

###### Syntax

```sll
(!==)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(!==)
```

##### Subtraction (`-`)

###### Syntax

```sll
(- |# minuend 1 #| |# subtrahend 1 #| |# subtrahend 2 #|)
```

###### Return value

The difference of the operands

###### Description

All of the operands are evaluated, subtracted from the first operand and returned.

###### Example

```sll
(:> (- 6 1 2) "\n")
```

##### Switch (`??`)

###### Syntax

```sll
(?: |# condition #|
	|# case 1 #| |# expression 1 #|
	|# case 2 #| |# expression 2 #|
	|# case 3 #| |# expression 3 #|
	|# default case expression #|
)
```

###### Return value

No return value (`nil`)

###### Description

The condition expression is evaluated. If it is not an integer or character, the default case expression is evaluated (if it exists). Every case expression is evaluated. Any case expressions that are not integers or characters are skipped. The expression block is selected based on the condition value. If no code block is found and the default case expression exists, it is evaluated.

###### Example

```sll
(= x -1)
(?? x
	-1 (:> "below")
	1 (:> "above")
	(:> "equal")
)
```

##### While Loop (`>-`)

###### Syntax

```sll
(>-)
```

###### Return Value

TBD

###### Description

TBD

###### Example

```sll
(>-)
```
