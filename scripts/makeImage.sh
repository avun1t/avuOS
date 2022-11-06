#!/bin/sh
set -e

if [ "$(id -u)" != 0 ]; then
	echo "Please run as root"
	exit 1
fi

echo "Creating a 50MB image..."
dd if=/dev/zero of=avuOS.img count=102400 bs=512 status=none
echo "Made blank image."

echo "Making loopback device..."
dev=$(losetup --find --partscan --show avuOS.img)
if [ -z "$dev" ]; then
	echo "Couldn't mount loopback."
	exit 1
fi
echo "Loopback device mount at ${dev}"

echo "Creating partition table..."
parted -s "${dev}" mklabel msdos mkpart primary ext2 32k 100% -a minimal set 1 boot on || (echo "Couldn't partition image" && exit 1)
echo "Created partition table."

echo "Zeroing out filesystem..."
dd if=/dev/zero of="${dev}p1" bs=1M count=1 status=none || (echo "Couldn't zero out." && exit 1)
echo "Created filesystem."

echo "Creating ext2 filesystem..."
mke2fs -q -I 128 -b 1024 "${dev}p1" || (echo "Couldn't create filesystem." && exit 1)
echo "Created filesystem."

echo "Mounting filesystem to mnt/ ..."
mount "${dev}p1" /mnt || (echo "Couldn't mount." && exit 1)
echo "Mounted."

echo "Copying base and kernel to filesystem..."
cp -r "${SOURCE_DIR}/base/"* /mnt || (echo "Couldn't copy." && exit 1)
mkdir -p /mnt/boot || (echo "Couldn't make mnt/boot." && exit 1)
cp src/kernel/yuku32 /mnt/boot || (echo "Couldn't copy kernel." && exit 1)
echo "Copied."

echo "Installing grub..."
grub-install --boot-directory=/mnt/boot --target=i386-pc --modules="ext2 part_msdos" "${dev}" || (echo "Couldn't install grub." && exit 1)
cp "${SOURCE_DIR}/scripts/grub.cfg" /mnt/boot/grub || (echo "Couldn't copy grub.cfg." && exit 1)
echo "Installed grub!"

echo "Unmounting and cleaning up..."
(umount /mnt && sync)|| (echo "Couldn't unmount." && exit 1)
losetup -d "${dev}" || (echo "Couldn't delete loopback device." && exit 1)
echo "Done! Saved to avuOS.img"
