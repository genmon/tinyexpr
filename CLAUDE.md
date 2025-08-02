# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This project uses a Makefile for building. Key commands:

- `make all` - Builds all targets (smoke tests, shader tests, examples, benchmarks, REPL)
- `make smoke` - Builds and runs the smoke test suite
- `make smoke_pr` - Builds and runs smoke tests with right-associative power and natural log
- `make shader_test` - Builds and runs comprehensive shader function tests
- `make clean` - Removes all built files and executables
- `make repl` - Builds the interactive REPL calculator
- `make bench` - Builds the benchmark program
- `make example` - Builds basic usage example
- `make example2` - Builds variable binding example  
- `make example3` - Builds custom function example

The build system automatically runs tests after building them to verify correctness.

## Testing

The repository includes multiple test suites:

### Primary Test Suite (`smoke.c`)
Uses the `minctest.h` testing framework. Tests cover:
- Basic arithmetic operations and operator precedence
- Mathematical functions (sin, cos, sqrt, etc.)
- Variable binding and evaluation
- Custom function registration
- Error handling and parse error detection
- Compile-time optimization verification
- Logical operators (&&, ||, !)
- Comparison operators (<, <=, >, >=, ==, !=)
- Ternary operator (condition ? true_expr : false_expr)

Run with `make smoke` for standard configuration or `make smoke_pr` for right-associative power and natural log configuration.

### Shader Function Tests (`shader_test.c`)
Comprehensive tests using practical shader functions that exercise all TinyExpr features:
- 48 test cases covering real-world mathematical expressions
- Tests smoothstep, mix, clamp, distance, and other common shader functions
- Validates complex nested expressions and operator precedence
- Includes reference implementations for accuracy verification

Run with `make shader_test`.

## Core Architecture

TinyExpr is a single-file math expression parser and evaluator consisting of:

### Main API (tinyexpr.h)
- `te_interp(expression, error)` - One-shot parse and evaluate
- `te_compile(expression, variables, var_count, error)` - Parse into reusable expression tree
- `te_eval(expr)` - Evaluate compiled expression
- `te_free(expr)` - Free compiled expression memory
- `te_print(expr)` - Debug print expression tree

### Core Implementation (tinyexpr.c)
- Recursive descent parser that builds abstract syntax trees
- Expression evaluation engine with variable binding support
- Built-in mathematical functions and constants (pi, e)
- Automatic constant folding optimization during compilation
- Memory management for expression trees

### Expression Tree Structure
- `te_expr` - Node in expression tree containing type, value/function pointer, and parameters
- Supports variables, constants, functions (0-7 arguments), and closures
- Type system distinguishes between pure functions and those with side effects

## Compile-Time Configuration

Key preprocessor flags (defined in tinyexpr.c):
- `TE_POW_FROM_RIGHT` - Makes exponentiation right-associative (a^b^c = a^(b^c))
- `TE_NAT_LOG` - Makes `log()` function use natural log instead of log10

## Examples and Usage Patterns

- `example.c` - Basic usage with `te_interp()`
- `example2.c` - Variable binding with `te_compile()` and `te_eval()`
- `example3.c` - Custom function registration
- `repl.c` - Interactive calculator implementation
- `benchmark.c` - Performance testing against native C code

The library is designed to be embedded as just two files (`tinyexpr.c` and `tinyexpr.h`) in larger projects.