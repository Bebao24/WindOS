include build/config.mk

.PHONY: all disk kernel bootloader clean always run

all: disk

disk: $(BUILD_DIR)/disk.img
$(BUILD_DIR)/disk.img: kernel bootloader
	@ chmod +x tools/disk/make_disk.sh
	@ ./tools/disk/make_disk.sh $@ /mnt/WindOSMnt target || (chmod +x tools/disk/cleanup.sh && \
		./tools/disk/cleanup.sh /mnt/WindOSMnt)

kernel: $(BUILD_DIR)/kernel/kernel.bin
$(BUILD_DIR)/kernel/kernel.bin: always
	@ $(MAKE) -C kernel BUILD_DIR=$(abspath $(BUILD_DIR))

bootloader:
	@ chmod +x bootloader/fetch.sh
	@ ./bootloader/fetch.sh

run:
	qemu-system-x86_64 -drive file=$(BUILD_DIR)/disk.img,format=raw -m 256M

always:
	@ mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
