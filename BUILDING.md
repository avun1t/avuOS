# avuOS Build / Run instructions

## Prerequisites

### Dependencies
- A Linux environment (WSL2 will work)
- GCC
- G++
- Make
- CMake
- Bison
- Flex
- GMP
- MPFR
- MPC
- TexInfo
- ISL
- CLooG
- QEmu

### Installing dependencies
- Ubuntu/Debian: `sudo apt install build-essential cmake bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libcloog-isl-dev libisl-dev qemu-system-i386 qemu-utils`
- Arch: `sudo pacman -Syu base-devel cmake gmp libmpc mpfr base-devel qemu qemu-arch-extra`

## Building the toolchain
1. Open the `toolchain` directory in your terminal and run `build-toolchain.sh`.
2. Make a cup of coffee and wait for the toolchain to compile.

### Editing the toolchain
If you'd like to edit the toolchain, you can run the `edit-toolchain.sh` script to download patches and set up a git repository for each one. Then, use the `gen-patches.sh` script to generate patch files for each one.

**DO NOT** git commit in the repositories created by `edit-toolchain.sh` or else `gen-patches.sh` won't work properly.

To build something from the `edit` directory, pass the `edited-[thing]` to the `build-toolchain.sh` script. (ex.: `build-toolchain.sh edited-newlib` to build newlib from the edit directory)

## Configuring CMake
1. Make sure you've built the toolchain first.
2. Make a build folder somewhere outside of this project where you'd like to build the kernel.
3. From that directory, run `cmake avuos_dir`, substituting `avuos_dir` for the directory in which the avuOS repository is located on your computer.

## Building and running avuOS
1. Either run the following commands as root, or use sudo su to open a root shell in the build directory you made earlier.
2. In your build directory, run `make` and then `make install` to build the kernel and programs.
3. Run `make image` to make the root filesystem image.
4. Run `make qemu-image` to run qemu with the image you just made.
5. Enjoy!