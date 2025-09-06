// os.flux — Userland 0 (init), basic libc, TTY app, and demo.

module FluxOS v0.2 {
  export fn image.entry(): u64
}

// Mini “libflux” userland
extern fn syscall(n:u64, a:u64, b:u64, c:u64, d:u64): u64

const SYS_WRITE:u64 = 1
const SYS_EXIT: u64 = 2
const SYS_TIME: u64 = 3

fn write(s:str) {
  let p = &s[0]
  let n = len(s)
  syscall(SYS_WRITE, cast(u64) p, cast(u64) n, 0, 0)
}

fn sleep_ticks(t:u64) {
  let start = syscall(SYS_TIME,0,0,0,0)
  while (syscall(SYS_TIME,0,0,0,0) - start < t) {}
}

fn demo_shell(): never {
  write("[init] FluxOS userland up\n")
  var i:u64 = 0
  loop {
    write("tick "); 
    // (very tiny itoa omitted)
    sleep_ticks(100)
    i = i + 1
    if (i == 10) { syscall(SYS_EXIT, 0,0,0,0) }
  }
}

// Image header shim; the loader treats this as the entry address.
fn image.entry(): u64 {
  return addr_of(demo_shell)
}
