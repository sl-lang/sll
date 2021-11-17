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

If using the standalone version of Sll, replace `sll` by `sll_standalone`. The most recent version of Sll is `0.6.27` (`0.6.28` is in developement).

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

The syntax of the Sl Language is simmilar to the syntax of Common Lisp and other Lisp dialects. The only main difference is that Sll does not have any keywords. Sll uses only operators to perform any function. This feature helps people with any language background to learn Sll.

The building-block of the Sl Language is called an [object](#objects). An object can represent anything from an integer to a complex expression.

### Comments

Single-line comments begin with a semicolon (`; comment`), while block comments have to be surrounded with special symbols (`|# comment #|`).

### Objects

Each object represent either a [base type](#base-types) or an [operator](#operator-types). If an object represents the latter, it must be surrounded by parentheses and contain the symbol for the given operator, as well as its operands.

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

The sign of the integer must by placed before the prefix. Otherwise, a compilation error will be raised.# Introduction to Sll

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

If using the standalone version of Sll, replace `sll` by `sll_standalone`. The most recent version of Sll is `0.6.27` (`0.6.28` is in developement).

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

The syntax of the Sl Language is simmilar to the syntax of Common Lisp and other Lisp dialects. The only main difference is that Sll does not have any keywords. Sll uses only operators to perform any function. This feature helps people with any language background to learn Sll.

The building-block of the Sl Language is called an [object](#objects). An object can represent anything from an integer to a complex expression.

### Comments

Single-line comments begin with a semicolon (`; comment`), while block comments have to be surrounded with special symbols (`|# comment #|`).

### Objects

Each object represent either a [base type](#base-types) or an [operator](#operator-types). If an object represents the latter, it must be surrounded by parentheses and contain the symbol for the given operator, as well as its operands.

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

An 8-bit unsigned integer.

##### String

An array of [characters](#char).

##### Array

An array of objects.

##### Map

Creates an mapping (association) between different keys and values. The keys and values can represent any object.

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
| `false` | `0` [^1] | A value which is returned when an expression is not true, ex. by the [equal operator](#equal-) with two different integers |
| `nil` | `0` [^1] | This value is returned by an expression which does not return anything |
| `true` | `1` [^1] | A value which is returned when an expression is true, ex. by the comparing two equal numbers |

[^1]: This constant is replaced by its integer value during the parsing stage
