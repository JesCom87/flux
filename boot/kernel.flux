// kernel.flux — Tiny monolithic kernel core: CPU/Paging init, sched stub, syscalls, drivers.

module Kernel v0.3 {
  export fn kentry(bi:&BootInfo): never
  export fn syscall(n:u64, a:u64, b:u64, c:u64, d:u64): u64
}

use BootInfo from BootLoader
const KSTACK_TOP:u64 = 0xFFFF800000200000
const HEAP_BASE:u64  = 0xFFFF900000000000
const HEAP_SZ:  u64  = 128 * 1024 * 1024

// Page tables / MMU
fn mmu_init(): void {
  mmu.enable_nx(true)
  mmu.map_identity(0, 16*1024*1024) // identity small window
  mmu.map(HEAP_BASE, HEAP_SZ, flags::RW)
  mmu.activate()
}

// Simple heap
fn heap_init(): void { heap.init(HEAP_BASE, HEAP_SZ) }

// Driver init
fn drivers_init(): void {
  console.init()
  timer.init(100) // 100 Hz
  keyboard.init()
  storage.init()
  net.init()
}

// Syscall table (OPD/CC1 layer can alias to this table)
const SYS_WRITE:u64 = 1
const SYS_EXIT: u64 = 2
const SYS_TIME: u64 = 3

fn sys_write(ptr:ptr<u8>, len:u64): u64 { console.write(ptr, len); return len }
fn sys_exit(code:u64): u64 { scheduler.exit_current(code); return code }
fn sys_time(): u64 { return timer.ticks() }

fn syscall(n:u64, a:u64, b:u64, c:u64, d:u64): u64 {
  match n {
    SYS_WRITE => return sys_write(cast(ptr<u8>) a, b)
    SYS_EXIT  => return sys_exit(a)
    SYS_TIME  => return sys_time()
    _ => return u64(-1)
  }
}

// Minimal scheduler
fn user_spawn(entry:u64, stack:u64): pid {
  return scheduler.spawn(entry, stack, priority::Normal)
}

// Kernel entry (ELF/KIMG header sets this symbol as entry)
fn kentry(bi:&BootInfo): never {
  console.write_str("[Kernel] Hello from Flux kernel\n")
  mmu_init()
  heap_init()
  drivers_init()

  // Map init process from OS blob (loaded right after kernel image in AbleWare)
  let init_entry = os.image.entry()
  let init_stack = scheduler.alloc_stack(64*1024)
  let pid0 = user_spawn(init_entry, init_stack)
  scheduler.run() // never returns
  halt()
}
