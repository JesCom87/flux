// Kernel function for parallel computation
pub fn kernel(index: i32) -> i32 {
    // Your computation here
    index * 2
}

// Main entry point
pub fn main() {
    let size = 1000
    let result = map(kernel, size)
}

// Map function to apply kernel across data
pub fn map(f: (i32) -> i32, n: i32) -> [i32] {
    let result = array(n)
    for i in 0..<n {
        result[i] = f(i)
    }
    result
}