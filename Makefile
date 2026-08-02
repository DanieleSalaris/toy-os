AS = nasm
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
OUTDIR = build

CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -c -g
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld
QEMUFLAGS = -display cocoa,zoom-to-fit=on

KERNEL = $(OUTDIR)/kernel.elf
ISO = $(OUTDIR)/toy-os.iso
GRUB_MKRESCUE = i686-elf-grub-mkrescue 

C_SRCS := $(wildcard *.c)
ASM_SRCS := $(wildcard *.asm)
C_OBJS := $(patsubst %.c, $(OUTDIR)/%.o, $(C_SRCS))
ASM_OBJS := $(patsubst %.asm, $(OUTDIR)/%.o, $(ASM_SRCS))

OBJS = $(C_OBJS) $(ASM_OBJS) 

# Default target
all: $(KERNEL)

# Assemble boot code

$(OUTDIR):
	mkdir $@

po: $(OBJS)
	echo $(OBJS)
poc: $(C_OBJS)
	echo $(C_OBJS)
pac: $(ASM_OBJS)
	echo $(ASM_OBJS)

$(OUTDIR)/%.o: %.asm | $(OUTDIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile kernel

$(OUTDIR)/%.o: %.c | $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $@

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
	qemu-system-i386 -cdrom $(ISO) -boot d $(QEMUFLAGS)
dbg: iso
	qemu-system-i386 -cdrom $(ISO) -boot d -S -s $(QEMUFLAGS)

# Clean build files
clean:
	rm -rf $(OUTDIR) $(ISO) iso

.PHONY: all iso run clean compile
