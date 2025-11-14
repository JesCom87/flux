import std

pub fn main() {
    // MBR is 512 bytes total
    let mbr: [u8; 512] = [0; 512]
    
    // Boot code (446 bytes, offset 0x000)
    // This is the bootloader code that executes first
    let boot_code: [u8; 446] = [0; 446]
    
    // Partition table (64 bytes, offset 0x1BE)
    // 4 partition entries, 16 bytes each
    let partition_table: [u8; 64] = [0; 64]
    
    // Boot signature (2 bytes, offset 0x1FE)
    // Must be 0x55 0xAA for valid MBR
    let boot_signature: [u8; 2] = [0x55, 0xAA]
    
    // Combine into full MBR
    std::mem::copy(&boot_code, &mbr[0..446])
    std::mem::copy(&partition_table, &mbr[446..510])
    std::mem::copy(&boot_signature, &mbr[510..512])
}