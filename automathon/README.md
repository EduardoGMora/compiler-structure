# Automathon — Deterministic Finite State Automaton (FSA)

**Developer:** Eduardo Gómez Mora

A C++ implementation of a Deterministic Finite State Automaton (DFSA) capable of validating strings against regular expression patterns. Currently supports alphabetic (`[A-Za-z]+`) and numeric (`[0-9]+`) expressions.

## How it works

The automaton reads an input string character by character. Each character triggers a **state transition**. If the automaton ends in an accepting state after processing all characters, the string is accepted; otherwise it is rejected.

### States

```
q0 (initial) --[valid symbol]--> q1 (accept)
q0            --[invalid symbol]--> REJECTED
q1            --[valid symbol]--> q1 (stay)
q1            --[invalid symbol]--> REJECTED
```

- **q0** — initial state, no input consumed yet.
- **q1** — accepting state, reached after reading at least one valid symbol.

The definition of "valid symbol" is delegated to the `Regex` subclass in use, making the automaton generic.

## Architecture

```
Pattern
  └── stores the raw pattern string (e.g. "[A-Za-z]+")

Regex  (abstract)
  ├── holds a Pattern*
  ├── isValid(string)        — validates the full input string
  └── isAcceptedSymbol(char) — validates a single character

  ├── AlphaExpression : Regex   — accepts [A-Za-z]
  └── NumericExpression : Regex — accepts [0-9]

FSA
  ├── holds a Regex*
  ├── process(string)   — runs the automaton over the full input
  ├── transition(char)  — advances the state for one symbol
  ├── reset()           — returns to the initial state
  └── getCurrentState() — returns the current state index
```

### Class responsibilities

| Class | Responsibility |
|---|---|
| `Pattern` | Stores the pattern string; no logic. |
| `Regex` | Abstract base: owns a `Pattern`, defines the validation interface. |
| `AlphaExpression` | Accepts letters (`isalpha`). |
| `NumericExpression` | Accepts digits (`isdigit`). |
| `FSA` | Runs the DFA. Owns the `Regex*` and deletes it on destruction. |

## File structure

```
automathon/
├── Pattern.hpp / Pattern.cpp
├── Regex.hpp / Regex.cpp
├── AlphaExpression.hpp / AlphaExpression.cpp
├── NumericExpression.hpp / NumericExpression.cpp
├── FSA.hpp / FSA.cpp
└── main.cpp
```

## Build

```bash
g++ -o automaton main.cpp AlphaExpression.cpp NumericExpression.cpp FSA.cpp Pattern.cpp Regex.cpp
```

## Usage

Run the binary and choose a validation mode from the interactive menu:

```
==========================================
 Deterministic Finite State Automata (FSA)
 1. Validate alphabetic string [A-Za-z]+
 2. Validate numeric string [0-9]+
 3. Exit
==========================================
Choose an option: 1
Enter an alphabetic string: hello
Result: ACCEPTED
Final state: q1
```

### Accepted inputs

| Mode | Accepted | Rejected |
|---|---|---|
| Alphabetic | `hello`, `ABC`, `xYz` | `hello1`, `123`, `` (empty) |
| Numeric | `123`, `0`, `9999` | `12a`, `abc`, `` (empty) |

## Extending with a new expression type

1. Create `MyExpression.hpp` / `MyExpression.cpp` inheriting from `Regex`.
2. Implement `isValid(const std::string&)` and `isAcceptedSymbol(char)`.
3. Instantiate it in `main.cpp` and pass it to `FSA`.
4. Add the new `.cpp` to the build command.
