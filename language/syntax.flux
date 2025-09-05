// Single-line comment
/* Multi-line 
   comment */

// Declaration
let x = 42;        // Immutable variable
var y = 10;        // Mutable variable
const z = 3.14;    // Constant

// Functions
fn add(a, b) -> Int {
    return a + b;
}

fn main() -> Void {
    let msg = "Hello Flux!";
    print(msg);
}

// Control Flow
if (x > 0) {
    print("Positive");
} else {
    print("Non-positive");
}

while (count < 10) {
    count = count + 1;
}

for (i in 0..5) {
    print(i);
}

// Types
let n: Int = 10;
let pi: Float = 3.14;
let flag: Bool = true;
let name: String = "Flux";

// Struct Records
struct User {
    id: Int;
    name: String;
}

let u = User { id: 1, name: "Alice" };

// Modules
module math {
    fn square(x: Int) -> Int {
        return x * x;
    }
}

import math;

let result = math.square(5);

// Low Level
machine {
    out 0x60, key;      // Send value to port
    let status = in 0x64;  // Read from port
}

// OS Firmware
entry {
    // BIOS / Bootloader starting point
    init_hardware();
    load_kernel();
    jump os_main;
}


