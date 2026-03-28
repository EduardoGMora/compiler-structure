# Máquina Virtual — Simulación del Ciclo de CPU

**Proyecto para la clase de Compiladores**  
**Profesor:** JOSE ANTONIO AVIÑA MENDEZ  
**Desarrollador:** José Eduardo Gómez Mora  

---

## Descripción general

Este proyecto implementa en C++ una **Máquina Virtual (VM)** que simula el ciclo fundamental de ejecución de una CPU real: **Fetch → Decode → Execute**. Su propósito es demostrar, desde el nivel de compiladores, cómo un procesador obtiene instrucciones de un programa, las interpreta y las ejecuta sobre un conjunto de registros a través de una Unidad Aritmético-Lógica (ALU).

El programa no lee código fuente de texto todavía; en cambio, las instrucciones se construyen directamente en código C++ para poder demostrar el ciclo completo de la VM de forma funcional. Las clases `Scanner` y `Token` están presentes en el repositorio como base para trabajo futuro.

---

## Arquitectura del proyecto

```
┌────────────────────────────────────────────────┐
│                      CPU                       │
│                                                │
│  ┌─────────────┐   ┌───────┐   ┌───────────┐   │
│  │ControlUnit  │   │  ALU  │   │ Registers │   │
│  │  fetch()    │   │ add() │   │  R0–R15   │   │
│  │  decode()   │   │ sub() │   │  PC,SP,FP │   │
│  │  execute()  │   │ mul() │   └───────────┘   │
│  └─────────────┘   │ div() │                   │
│                    └───────┘                   │
│  ┌─────────────────────────────────────────┐   │
│  │                Program                  │   │
│  │  [ MOV | ADD | SUB | MUL | ... ]        │   │
│  └─────────────────────────────────────────┘   │
└────────────────────────────────────────────────┘
```

---

## Componentes implementados

### `Instruction` — Clase base abstracta
Clase padre de todas las instrucciones. Define los atributos comunes:

| Atributo | Tipo | Descripción |
|---|---|---|
| `name` | `string` | Texto legible de la instrucción (ej. `"MOV R0, 10"`) |
| `code` | `short int` | Código de operación (*opcode*) |
| `length` | `short int` | Longitud en bytes de la instrucción |

Expone el método virtual `execute(Registers&, ALU&)` que cada instrucción concreta sobreescribe — lo que permite el **polimorfismo** que usa la `ControlUnit` al ejecutar.

---

### `MOV` — Carga inmediata
```
MOV Rdest, valor_inmediato
```
Carga un valor entero directamente en el registro destino.

```cpp
new MOV("MOV R0, 10", 1, 1, /*operand=*/10, /*registerIndex=*/0)
```

---

### `ADD` — Suma de registros
```
ADD Rdest, Rsrc   →   Rdest = Rdest + Rsrc
```
Delega la operación a `ALU::add()` y almacena el resultado en `Rdest`.

---

### `SUB` — Resta de registros
```
SUB Rdest, Rsrc   →   Rdest = Rdest - Rsrc
```
Delega la operación a `ALU::subtract()`.

---

### `MUL` — Multiplicación de registros
```
MUL Rdest, Rsrc   →   Rdest = Rdest * Rsrc
```
Delega la operación a `ALU::multiply()`.

---

### `ALU` — Unidad Aritmético-Lógica
Proporciona las operaciones aritméticas básicas:

| Método | Operación |
|---|---|
| `add(a, b)` | `a + b` |
| `subtract(a, b)` | `a - b` |
| `multiply(a, b)` | `a * b` |
| `divide(a, b)` | `a / b` (lanza excepción si `b == 0`) |

---

### `Registers` — Banco de registros
Simula el banco de registros de un procesador:

| Campo | Descripción |
|---|---|
| `R[16]` | 16 registros de propósito general (R0 – R15) |
| `PC` | *Program Counter* — dirección de la siguiente instrucción |
| `SP` | *Stack Pointer* — tope de la pila |
| `FP` | *Frame Pointer* — base del marco de activación actual |

---

### `Program` — Contenedor de instrucciones
Arreglo estático de hasta **10 punteros** a `Instruction`. El uso de punteros es lo que habilita el polimorfismo: la `ControlUnit` no necesita saber qué tipo concreto de instrucción va a ejecutar.

---

### `ControlUnit` — Unidad de Control
Implementa el ciclo **Fetch → Decode → Execute**:

```
FETCH   → Obtiene el puntero a la instrucción en la posición actual del programa.
DECODE  → Lee el opcode y el nombre de la instrucción (registro en consola).
EXECUTE → Llama al método virtual execute() de la instrucción.
```

---

### `CPU` — Procesador principal
Orquesta todos los componentes. Su método `run()` itera sobre las instrucciones del programa y aplica el ciclo de la `ControlUnit` instrucción por instrucción. Al finalizar, `printRegisters()` muestra el estado final de los 8 primeros registros.

---

## Ciclo de ejecución — Ejemplo del `main`

El programa de demostración codificado en `Main.cpp` realiza la siguiente secuencia:

```
R0 = 10          (MOV R0, 10)
R1 = 5           (MOV R1, 5)
R0 = R0 + R1     (ADD R0, R1)  →  R0 = 15
R0 = R0 - R1     (SUB R0, R1)  →  R0 = 10
R0 = R0 * R1     (MUL R2, R1)  →  R0 = 50
```

Salida esperada en consola:

```
=== CPU started ===
[FETCH]   position 0
[DECODE]  MOV R0, 10 (code=1)
[EXECUTE] MOV R0, 10

[FETCH]   position 1
[DECODE]  MOV R1, 5 (code=1)
[EXECUTE] MOV R1, 5

[FETCH]   position 2
[DECODE]  ADD R0, R1 (code=2)
[EXECUTE] ADD R0, R1

[FETCH]   position 3
[DECODE]  SUB R0, R1 (code=3)
[EXECUTE] SUB R0, R1

[FETCH]   position 4
[DECODE]  MUL R2, R1 (code=4)
[EXECUTE] MUL R2, R1

=== CPU halted ===
=== Registers ==
  R0 = 50
  R1 = 5
  R2 = 0
  ...
```

---

## Compilación y ejecución

```bash
# Desde el directorio vm/
g++ -std=c++17 -o vm \
    Main.cpp \
    CPU.cpp ALU.cpp ControlUnit.cpp Registers.cpp \
    Program.cpp Instruction.cpp \
    MOV.cpp ADD.cpp SUB.cpp MUL.cpp \
    Token.cpp Scanner.cpp

./vm
```

---

## Trabajo futuro

Las clases `Scanner` y `Token` ya están definidas estructuralmente pero **no están integradas** al flujo de ejecución actual. Son la base para las siguientes etapas del compilador que se tienen pendientes de implementar:

| Etapa | Descripción |
|---|---|
| **Analizador Léxico (Lexer/Scanner)** | Leer código fuente como texto y dividirlo en tokens (palabras clave, identificadores, operadores, literales). La clase `Scanner` y el enum `TokenType` en `Token.hpp` ya definen los tipos necesarios. |
| **Tokens** | Representación estructurada de cada unidad léxica del lenguaje. La clase `Token` ya contiene `type` y `value`. |
| **Analizador Sintáctico (Parser)** | Verificar que la secuencia de tokens produzca una gramática válida y construir el Árbol Sintáctico Abstracto (AST). |
| **Generación de instrucciones desde código fuente** | Traducir el AST a instrucciones de la VM (`MOV`, `ADD`, `SUB`, `MUL`, etc.) en lugar de codificarlas manualmente en `main`. |

Con estas etapas, la VM podría leer un archivo de texto escrito en un lenguaje de programación personalizado, compilarlo internamente y ejecutarlo a través del mismo ciclo Fetch → Decode → Execute que ya está implementado.

---

## Estructura de archivos

```
vm/
├── Main.cpp            # Punto de entrada: programa de demostración
├── CPU.hpp / .cpp      # Procesador principal (orquesta todo)
├── ControlUnit.hpp / .cpp  # Ciclo Fetch-Decode-Execute
├── ALU.hpp / .cpp      # Unidad Aritmético-Lógica
├── Registers.hpp / .cpp    # Banco de 16 registros + PC, SP, FP
├── Program.hpp / .cpp  # Contenedor de instrucciones
├── Instruction.hpp / .cpp  # Clase base abstracta de instrucciones
├── MOV.hpp / .cpp      # Instrucción: carga inmediata
├── ADD.hpp / .cpp      # Instrucción: suma de registros
├── SUB.hpp / .cpp      # Instrucción: resta de registros
├── MUL.hpp / .cpp      # Instrucción: multiplicación de registros
├── Token.hpp / .cpp    # [Futuro] Representación de tokens léxicos
└── Scanner.hpp / .cpp  # [Futuro] Analizador léxico
```
