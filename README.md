# avuOS
### The stupid Operating System

### What's working
- Booting via grub multiboot off of a hard drive
- Typing commands into the kernel shell
- Reading files off of the hard drive
- Reading devices as files(`/dev/hda`, `/dev/zero`, `/dev/random`, etc.)
- Basic multitasking (no executables yet)

### Known Issues / Limitations
- It kinda sucks
- Only works with ext2 filesystems with a block size of 1024
- Uses BIOS interrupts (ATA PIO) to read/write to disk instead of AHCI/IDE (very slow)

### Building / Running
- See [INSTRUCTIONS.md](BUILDING.md) for instructions.