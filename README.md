# Computer Science (Revised)

### Computer Science
#### Layers of Abstraction

#### Number Systems

- Number Systems: Decimal = 0-9, Binary = 0, 1, Octal = 0-8, Hexidecimal = 1-9 and a-f.

- There are Conversions for each pair.

- Each Number System has a: Radix, Base, Weight.

- The Radix is the number of Decimals in each Position.

- The Base is the same as the Radix.

- The Weight is the Base and the Power Of.

- The Power Of is the number of the Position.

- Integers to the left, Fractionals to the right.

- Decimal: Has no Value.

- Binary: [1 = (on, yes, true, high)], or, [0 = (off, no, false, low)]. = 1 bit.

- Nibble = 4 bits.

- Octal: 8 bits = 1 byte.

- 2 bytes (16 bits) = Word.

- Hexadecimal: Prefix = 0x, Suffix = h.

- Little Endian = Least significant byte first.

- Big Endian = Most significant byte first.

```
Dec.  Bin.  Hex.
0       0      0
1         1       1
2        10     2
3        11      3
4        100  4
5        101    5
6        110    6
7         111     7
8        1000  8
9        1001  9
10      1010  a
11       1011    b
12       1100  c
13       1101   d
14       1110   e
15       1111     f
```

```
16 bit (4 bytes):
Bin: 0100 0111 1110 1101
Hex: 47ed
```

```
Dec Weight: 10*
Bin Weight: 2*
Oct Weight: 8*
Hex Weight: 16*
```

```
Hex: 0x55aa, 55aah.
```

```
Little Endian: 0x55aa, 55aah.
Big Endian: 0xaa55, aa55h.
```

#### Quantum Computing

- Normal computers are Binary. 1, or, 0. A bit.

- Bit stands for Binary Digit.

- Quantum computers use Qubits, for Quantum Binary Digits.

- Qubits can be in Superposition, both 1, and 0, simultaneously.

- This is called Quantum Entanglement.

#### Electronics

- Basic Electronics has: Resistor, Capacitor, Inductor, Transformer, Diode, Transistor.

- The transistor, a silicon semiconductor, was invented in 1956.

- They won a Nobel Prize in Physics for it.

- We use the letters N, and P to describe a Transistor. NPN, PNP.

- A Transistor has an Emitter, Base, Collector.

- It is a Current Controlled Switch.

- The Electrons would typically flow from the Emitter (N), to the Base (P), to the Collector (N).

#### Analog Circuits

- An electric current is Analog and in Decimal.

- It's measured in Volts.

- Wires carry the Voltage.

- Three basic circuits are: Series, Parallel, Series/Parallel.

#### Digital Circuits

- (The Conversion, Analog to Digital, is through ElectroMechanics.)

- A Digital Circuit is Binary.

- It's measured in bits.

- A single wire can carry 1 bit.

- (1 bit is roughly around 0.6V) (off < 0.6V < on). (Current Control Switch)

- There are two types of Digital Circuits: Combinational, Sequential.

#### Adders

- A Half-Adder is a XOR and AND Gate.

- It has A, B Inputs, and Sum and Carry Outputs.

- A Full Adder is two Half-Adders.

- It has A, B, C Inputs, and Sum, Carry Outputs.

- An 8 Bit Ripple Carry Adder, starts with a Half-Adder, and chains Full-Adders.

- Adding Full Adders to an 8 bit with 16 and 32 bit numbers prevents Overflow.

- There are circuits for Adding, Subtracting, Multiplying, and Division.

#### Digital Circuits wih Logic Gates

- They use Boolean Algebra and the Truth Table. (Mainly due to Logic.)

- They can have Input/Output.

- They can have Read/Write capabilities.

- 1 or more Digital Circuits can form a Latch.

- There are Gated Latches.

- A Latch can store 1 bit. Memory.

#### Memory

- Memory is Octal.

- A Group of Latches form a Register.

- A grid of Latches with MultiPlexors form a Matrix.

- An Address is the Intersection of a Matrix. Row and Column.

- An Address is Hexadecimal.

#### Registers

- 32 bit General Register example.

```
32 bit  8 bit  16 bit  8 bit,
EAX      AH      AX     AL,
EBX      BH      BX     BL,
ECX      CH      CX     CL,
EDX      DH      DX     DL,

A = Accumulator
B = Base
C = Counter
D = Data

H = High
L = Low
```

- 32 bit Index and Pointer Registers example.

```
32 bit  16 bit,
ESI     SI,
EDI     DI,
EBP     BP,
ESP     SP,

SI: Source Index.
DI: Destination Index.
BP: Base Pointer.
SP: Stack Pointer.
```

```
E: 32 bit.
R: 64 bit.
```

#### Buses

- There are Data, Address, and Control Buses.

- They are Job-Specific High-Speed Wires.

- They carry Data.

#### Ports

- Ports connect outside In/Out Devices.

#### Mainboard

- The Mainboards connects all components and a computer together.

#### Clock

- A Chip with a Crystal that Vibrates when Electricity is applied.

- It Oscillates, Physcics.

- And Generates Pulses.

- It is the smallest Frequency of a computer.

- It is measured in Hz.

- It Syncronizes all Components that require Time for their Operation.

- There are two types: Real-Time, Software Clocks.

#### CPU

- There are three main parts to the Central Processing Unit (CPU): The Control Unit (CU), the Arithmetic and Logic Unit (ALU), and the Memory Unit.

- It's four Main Functions: Input, Processing, Memory, Output.

#### CU

- The Control Unit manages computer ressources.

- It directs the flow of data between the CPU and other Devices.

- It will configure a CPU to manipulate the data correctly between instructions.

#### ALU

- Performs Mathematical Operations: Arithmitic and Logical Computation.

- Arithmitic handles all numerical operations. +, -, x, /.

- Logical handles all Logical Operations. AND, OR, NOT, XOR.

- It has two 8 bit Inputs. A 4 bit OpCode. An 8 bit Output. Overflow, Zero, Negative Flags.

#### Memory Unit

- The Memory Unit consists of Registers.

- There are 8 General Purpose Registers.

- The Accumultor, Base, Counter, Data Registers.

- The Base, Stack Pointer Registers.

- The Source, Destination Index Registers.

- It also has Flag Registers.

#### CPU Purpose

- The CPU's purpose is to execute Programs.

- Programs are made up of Instructions.

- The Instruction Cycle has four cycles: Fetch, Decode, Execute, Store.

- It takes a certain amount of Machine Cycles to execute an Instruction Cycle.

- Speed depends on the number of Clock Cycles needed to execute an Instruction.

- A Clock Cycle is the time taken between two Pulses of an Oscillator.

- A CPU performs a Clock Cycle, Machine Cycle, Instruction Cycle, respectively.

- Then the Instruction is executed.

#### Instruction Format

- 16 bit Instruction Format:

```
Prefix, OpCode, Mod-R/M, SIB, Disp, Imm.
```
 
  - Required:

```
Opcode, Disp, Imm.
```

#### Instruction Set

- A group of commands for a CPU in Machine Language.

- Can refer to all possible or subset instructions for a CPU.

#### Language Processing System

- Process: Pre-Processor, Compiler, Assembler, Linker, Loader, Memory.

- Pre-Processor: Produces Input for the Compiler.

- Compiler Phases: Lexical, Syntax, and Semantic Analyzer, Intermediate Code Generator, Machine independant Code Optimizer, Code Generator, Machine Dependant Code Optimizer.

- Assembler: Translates Assembly Language into Machine Code, and creates an Object file.

- Linker: Links and Merges various Object files to create an Executable.

- Loader/Memory: Loads Executable files into Memory and executes them.

#### Programming Languages

- There are Low Level and High Level Programming.

- They are used to make Intructions that make up Programs.

- They are written in Code.

- Each Programming Language's Intruction Code have their own Syntax.

- Incorrect Syntax will generate an Error.

- They guide the rules that guide the Logic of Hardware and Software Design.

#### Cyber Security

- Reverse-Engineered Debugging techniques.

- Ethical Hacking.

#### Data Structures and Algorithms

- Data Structures and Algorithms are an important part of Computer Science.

- Data Structures: Primitive, Non Primitive.

- Primitive: Numeric, Non Numeric.

- Non Primitive: Linear, Non Linear.

- Algorithms: Input, Output, Finiteness, Definiteness, Effectiveness.

- They help CPUs solve Problems.

#### Operating System

- An Operating System is an Interface between User Applications and Hardware.

- It helps manage the CPU's Input, Processing, Memory, Output.

- Four of it's Functions: Memory Management, Processor Management, Device Management, File Management.

- It has four Processes: Stack, Heap, Data, Text.

- It has Scheduling.

- It has Threads.

#### Cloud

- Cloud is a Network.

- It refers to manipulating, configurating, and accessing Applications online.

- It offers online data storage, infrastructure and application.

- Deployment Models: Public, Private, Hybrid, Community.

- Service Models: Infrastructure, Platform, Software, Identity, and Network as a Service.

#### Artificial Intelligence

- We are trying to get CPUs to think like Humans.

- Types of Intelligence: Linguistic, Musical, Logical, Mathematical, Spatial, Bodily-Kinesthetic, Intra-Personal, Interpersonal.

- It is composed of: Reasoning, Learning, Problem Solving, Perception, Linguistic.

- Task classification: Formal, Mundane, Expert.

- Machine Learning is a sub of Artificial Intelligence.

#### Math

- Theory, Analysis.

- Set, Field, Space, Structure, Object, Element, Function, Expression, Ring, Group, Symbol, Operation, Formula, Solution.

#### Algebra

- Elementary, Linear, Abstract, Universal.

#### Calculus

- Integral, Differentiation, Inverse.

- Convergence, Sequence, Series.

- Normal, Reverse.

- Limit.

#### Probability

- Theoretical, Experimental, Axiomatic.

- Description, Event, Occurrence.

- Mean, Deviation, Distribution.

- 0-1.

#### Discrete Math

- Sets, Combinatorics, Relations.

#### Linear Algebra

- Map, Matrix, Tensors, Vectors, Scalars.

#### Limit Calculus

- Continuity, Derivatives, Integrals, Function, Argument, Value.

#### Axiomatic Probability

- First, Second, Third.

#### Example with Layers of Abstraction

- Pseudocode Sorting and Classification Programs from Data Structures and Algorithms for Deep Learning of Neural Networks of Artificial Intelligence.


