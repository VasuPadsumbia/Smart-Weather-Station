#!/bin/bash
# Script to run QEMU for Buildroot as the default configuration (qemu_aarch64_virt_defconfig or similar)
# Host forwarding: Host Port 10022 ->> QEMU Port 22
# Author: Vasu Padsumbia (reference: Siddhant Jajoo, assignment-9)

KERNEL_IMG=buildroot/output/images/Image
ROOTFS_IMG=buildroot/output/images/rootfs.ext4

if [ ! -f "$KERNEL_IMG" ]; then
    echo "Kernel image $KERNEL_IMG not found!"
    exit 1
fi

if [ ! -f "$ROOTFS_IMG" ]; then
    echo "Root filesystem $ROOTFS_IMG not found!"
    exit 1
fi

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a53 -nographic -smp 1 \
    -kernel "$KERNEL_IMG" \
    -append "rootwait root=/dev/vda console=ttyAMA0" \
    -netdev user,id=eth0,hostfwd=tcp::10022-:22,hostfwd=tcp::9000-:9000 \
    -device virtio-net-device,netdev=eth0 \
    -drive file="$ROOTFS_IMG",if=none,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -device virtio-rng-pci
