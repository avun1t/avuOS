# avuOS
### The stupid Operating System

### What's working
- Booting via grub multiboot off of a hard drive
- Typing commands into the kernel shell
- Reading files off of the hard drive
- Reading devices as files(`/dev/hda`, `/dev/zero`, `/dev/random`, etc.)
- A newlib-based C standard library for programs (but not a ton of syscall functionality is implemented yet)
- Basic multitasking (executing ELFs)

### Programs
- mirror (/bin/mirror): A demo program that prompts you to type something and then prints it back to you.
- dummy (/bin/dummy): A demo program that does nothing and exits.

### Known Issues / Limitations
- Only works with ext2 filesystems with a block size of 1024
- Uses BIOS interrupts (ATA PIO) to read/write to disk instead of AHCI/IDE (very slow)
- Since dynamic memory allocation (brk/sbrk) isn't yet implemented in my newlib implementation, there are many spurious crashes when running programs

### Building / Running
- See [BUILDING.md](BUILDING.md) for instructions.

### License
- See [LICENSE.txt](LICENSE.txt).