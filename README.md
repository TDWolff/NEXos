## Prerequisites

 - A text editor such as [VS Code](https://code.visualstudio.com/).
 - [Docker](https://www.docker.com/) for creating our build-environment.
 - [Qemu](https://www.qemu.org/) for emulating our operating system.
   - Remember to add Qemu to the path so that you can access it from your command-line. ([Windows instructions here](https://dev.to/whaleshark271/using-qemu-on-windows-10-home-edition-4062))

## Setup

Build an image for our build-environment:
 - `docker build buildenv -t myos-buildenv`

## Build

Enter build environment:
 - Linux or MacOS: `docker run --rm -it -v "$(pwd)":/root/env myos-buildenv`
 - Windows (CMD): `docker run --rm -it -v "%cd%":/root/env myos-buildenv`
 - Windows (PowerShell): `docker run --rm -it -v "${pwd}:/root/env" myos-buildenv`
 - Please use the linux command if you are using `WSL`, `msys2` or `git bash`
 - NOTE: If you are having trouble with an unshared drive, ensure your docker daemon has access to the drive you're development environment is in. For Docker Desktop, this is in "Settings > Shared Drives" or "Settings > Resources > File Sharing".

Build for x86 (other architectures may come in the future):
 - `make build-x86_64`
 - If you are using Qemu, please close it before running this command to prevent errors.

To leave the build environment, enter `exit`.

## Emulate

You can emulate your operating system using [Qemu](https://www.qemu.org/): (Don't forget to [add qemu to your path](https://dev.to/whaleshark271/using-qemu-on-windows-10-home-edition-4062#:~:text=2.-,Add%20Qemu%20path%20to%20environment%20variables%20settings,-Copy%20the%20Qemu)!)

 - `qemu-system-x86_64 -m 1048 -cdrom dist/x86_64/kernel.iso`
 - Note: Close the emulator when finished, so as to not block writing to `kernel.iso` for future builds.

If the above command fails, try one of the following:
 - Windows: [`qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso -L "C:\Program Files\qemu"`](https://stackoverflow.com/questions/66266448/qemu-could-not-load-pc-bios-bios-256k-bin)
 - Linux: [`qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso -L /usr/share/qemu/`](https://unix.stackexchange.com/questions/134893/cannot-start-kvm-vm-because-missing-bios)
 - Alternatively, install a custom BIOS binary file and link it to Qemu using the `-L` option.

Alternatively, you should be able to load the operating system on a USB drive and boot into it when you turn on your computer. (I haven't actually tested this yet, but it appears to be working for others.)

## Cleanup

Remove the build-evironment image:
 - `docker rmi myos-buildenv -f`


## Contributing
[TDWolff](https://github.com/TDWolff)

## Placecholder code
```c
typedef struct {
    char name[11];
    unsigned char attr;
    unsigned char nt_res;
    unsigned char crt_time_tenth;
    unsigned short crt_time;
    unsigned short crt_date;
    unsigned short lst_acc_date;
    unsigned short fst_clus_hi;
    unsigned short wrt_time;
    unsigned short wrt_date;
    unsigned short fst_clus_lo;
    unsigned int file_size;
} __attribute__((packed)) dir_entry_t;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sectors;
    unsigned char number_of_fats;
    unsigned short root_dir_entries;
    unsigned short total_sectors_short; // if zero, later field is used
    unsigned char media_descriptor;
    unsigned short fat_size_sectors;
    unsigned short sectors_per_track;
    unsigned short number_of_heads;
    unsigned int hidden_sectors;
    unsigned int total_sectors_long;

    unsigned int table_size;
    unsigned short extended_flags;
    unsigned short fat_version;
    unsigned int root_cluster;
    unsigned short fat_info;
    unsigned short backup_BS_sector;
    unsigned char reserved_0[12];
    unsigned char drive_number;
    unsigned char reserved_1;
    unsigned char boot_signature;
    unsigned int volume_id;
    char volume_label[11];
    char fat_type_label[8];
} __attribute__((packed)) fat32_BS_struct_t;
```


## Current Issues
```
WARNING: The requested image's platform (linux/amd64) does not match the detected host platform (linux/arm64/v8) and no specific platform was requested
root@8585bd4fc0eb:~/env# make build-x86_64
mkdir -p build/kernel/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/kernel/main.c -o build/kernel/main.o
src/impl/kernel/main.c: In function 'kernel_main':
src/impl/kernel/main.c:12:5: warning: implicit declaration of function 'mem_init' [-Wimplicit-function-declaration]
   12 |     mem_init();
      |     ^~~~~~~~
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/print.c -o build/x86_64/print.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/io.c -o build/x86_64/io.o
src/impl/x86_64/io.c: In function 'sprintf':
src/impl/x86_64/io.c:59:5: warning: implicit declaration of function 'va_start' [-Wimplicit-function-declaration]
   59 |     va_start(args, format);
      |     ^~~~~~~~
src/impl/x86_64/io.c:60:5: warning: implicit declaration of function 'vsprintf'; did you mean 'sprintf'? [-Wimplicit-function-declaration]
   60 |     vsprintf(str, format, args);
      |     ^~~~~~~~
      |     sprintf
src/impl/x86_64/io.c:61:5: warning: implicit declaration of function 'va_end' [-Wimplicit-function-declaration]
   61 |     va_end(args);
      |     ^~~~~~
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/keyboard.c -o build/x86_64/keyboard.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/mem.c -o build/x86_64/mem.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/string.c -o build/x86_64/string.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/filesystem.c -o build/x86_64/filesystem.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/disk.c -o build/x86_64/disk.o
mkdir -p build/x86_64/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/x86_64/md.c -o build/x86_64/md.o
mkdir -p build/x86_64/boot/ && \
nasm -f elf64  src/impl/x86_64/boot/main.asm -o build/x86_64/boot/main.o
mkdir -p build/x86_64/boot/ && \
nasm -f elf64  src/impl/x86_64/boot/main64.asm -o build/x86_64/boot/main64.o
mkdir -p build/x86_64/boot/ && \
nasm -f elf64  src/impl/x86_64/boot/header.asm -o build/x86_64/boot/header.o
mkdir -p build/shell/ && \
x86_64-elf-gcc -c -I src/intf -ffreestanding  src/impl/shell/shell.c -o build/shell/shell.o
mkdir -p dist/x86_64 && \
x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld  build/kernel/main.o  build/x86_64/print.o  build/x86_64/io.o  build/x86_64/keyboard.o  build/x86_64/mem.o  build/x86_64/string.o  build/x86_64/filesystem.o  build/x86_64/disk.o  build/x86_64/md.o  build/x86_64/boot/main.o  build/x86_64/boot/main64.o  build/x86_64/boot/header.o  build/shell/shell.o && \
cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso
x86_64-elf-ld: warning: dist/x86_64/kernel.bin has a LOAD segment with RWX permissions
x86_64-elf-ld: build/x86_64/io.o: in function `sprintf':
io.c:(.text+0x1f3): undefined reference to `va_start'
x86_64-elf-ld: io.c:(.text+0x218): undefined reference to `vsprintf'
x86_64-elf-ld: io.c:(.text+0x22c): undefined reference to `va_end'
make: *** [Makefile:33: build-x86_64] Error 1
root@8585bd4fc0eb:~/env# 
```