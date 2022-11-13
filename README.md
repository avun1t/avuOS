# avuOS
## avuOS is defunct, take a look at [Saturn](https://github.com/avun1t/saturn).
### The stupid Operating System

### What's working
- Booting via grub multiboot off of a hard drive
- Typing commands into the kernel shell
- Reading files off of the hard drive
- Reading devices as files(`/dev/hda`, `/dev/zero`, `/dev/random`, etc.)
- A newlib-based C standard library for programs (but not a ton of syscall functionality is implemented yet)
- Multitasking / executing ELFs

### Programs
- mirror (/bin/mirror): A demo program that prompts you to type something and then prints it back to you.
- dummy (/bin/dummy): A demo program that does nothing and exits.
- fork (/bin/fork): A demo program that forks and then prints whether it is the child or the parent process.

### Known Issues / Limitations
- Only works with ext2 filesystems with a block size of 1024
- Uses BIOS interrupts (ATA PIO) to read/write to disk instead of AHCI/IDE (very slow)
- Copy-on-write (CoW) is not implemented, so when processes fork, they copy all working memory which could be slow

### Building / Running
- See [BUILDING.md](BUILDING.md) for instructions.

### License
- See [LICENSE.txt](LICENSE.txt).
