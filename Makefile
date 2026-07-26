AS = nasm
CC = x86_64-elf-gcc
LD = x86_64-elf-ld

CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL = kernel.elf
ISO = toy-os.iso
GRUB_MKRESCUE = i686-elf-grub-mkrescue 

SRCS = kernel.c gdt.c
COBJS = kernel.o gdt.o
OBJS = boot.o $(COBJS) 

# Default target
all: $(KERNEL)

# Assemble boot code

boot.o: boot.asm
	$(AS) $(ASFLAGS) boot.asm -o boot.o

# Compile kernel

%.o: %c
	$(CC) $(CFLAGS) $< -o $@

compile: $(COBJS)

# Link kernel
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(KERNEL)
	
# Create GRUB ISO
iso: $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.elf
	cp grub/grub.cfg iso/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO) iso

# Run in QEMU
run: iso
	qemu-system-i386 -cdrom $(ISO) -boot d

# Clean build files
clean:
	rm -rf *.o $(KERNEL) $(ISO) iso

.PHONY: all iso run clean compile
