#!/bin/bash
#Script to run QEMU for buildroot as the default configuration qemu_aarch64_virt_defconfig
#Host forwarding: Host Port 10022 ->> QEMU Port 22 
#Author: Siddhant Jajoo.

#-netdev user,id=eth0,hostfwd=tcp::10022-:22 \
# -device virtio-net-device,netdev=eth0 \
qemu-system-aarch64 \
    -M virt  \
    -cpu cortex-a53 -nographic -smp 1 \
    -kernel buildroot/output/images/Image \
    -append "rootwait root=/dev/vda console=ttyAMA0" \
    -drive file=buildroot/output/images/rootfs.ext4,if=none,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 -device virtio-rng-pci
#qemu-system-arm \
#  -M raspi2b \
#  -cpu cortex-a7 \
#  -smp 4 \
#  -m 1024 \
#  -kernel buildroot/output/images/Image \
#  -dtb buildroot/output/images/bcm2710-rpi-zero-2-w.dtb \
#  -sd buildroot/output/images/sdcard.img \
#  -append "root=/dev/mmcblk0p2 rootfstype=ext4 rootwait console=ttyAMA0" \
#  -drive if=none,file=buildroot/output/images/rootfs.ext4,id=hd \
#  -device virtio-blk-device,drive=hd \
#  -nographic \
#  -netdev user,id=net0,hostfwd=tcp::10022-:22 \
#  -device virtio-net-device,netdev=net0
