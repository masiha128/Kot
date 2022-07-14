SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
qemu-system-x86_64 -no-reboot -no-shutdown -M smm=off -serial stdio -machine q35 -cpu qemu64 -smp 8 -cdrom $SCRIPTPATH/../Bin/kot.iso -m 256M