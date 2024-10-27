ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# Compiler settings
CFLAGS = -Wall -Isrc/include -nostdlib \
-ffreestanding -fno-stack-protector -fno-pic -fno-pie -static -fno-strict-aliasing \
-fno-builtin -fno-omit-frame-pointer -msoft-float -m32 -Wextra -Wall -Werror -T src/linker.ld -lc

GRUB_CFG = menuentry \"yawnix\" {\
	multiboot /boot/yawnix\
}\


TARGET_EXEC := build/iso/boot/yawnix
BUILD_DIR := ./build
SRC_DIRS := ./src
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s' -or -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CC := gcc

exec: $(TARGET_EXEC)

run: iso
	$(V_AT)qemu-system-i386 -cdrom os.iso

iso: $(TARGET_EXEC)
	$(V_AT)grub-mkrescue -o os.iso build/iso

$(TARGET_EXEC): $(OBJS)
	@echo "Linking $@"
	$(V_AT)$(CXX) -g $(OBJS) -o $@ $(LDFLAGS) $(CFLAGS)


$(BUILD_DIR)/%.c.o: %.c iso_dir
	@echo "Compiling $<"
	$(V_AT)mkdir -p $(dir $@)
	$(V_AT)$(CC) -g $(CPPFLAGS) -c $< -o $@ $(CFLAGS)

iso_dir:
	@echo making iso directory
	$(V_AT)mkdir -p ${BUILD_DIR}/iso/boot/grub
	$(V_AT)echo ${GRUB_CFG} > ${BUILD_DIR}/iso/boot/grub/grub.cfg

disassm:
	$(V_AT)objdump -s --disassemble /home/vic/Desktop/C/build/iso/boot/yawnix

clean:
	$(V_AT)rm -rf build/* *.iso
	$(V_AT)rm -rf doc/doxygen

.PHONY: all doc clean