QEMUFLAGS =	-no-reboot 										\
			-no-shutdown 									\
			-boot d 										\
			-M smm=off 										\
			-serial stdio 									\
			-machine q35 									\
			-cpu qemu64 									\
			-smp 8 											\
			-cdrom ./Bin/kot.iso							\
			-drive file=./Build/kot.img						\
			-m 3G											\
			-netdev user,id=n0						 		\
			-device e1000,netdev=n0							\
			-usb                                            \
    		-device usb-ehci,id=ehci                        \
			-soundhw pcspk

build:
	bash ./Build/build.sh 

run:
	sudo qemu-system-x86_64 $(QEMUFLAGS)

debug:
	qemu-system-x86_64 $(QEMUFLAGS) -s -S

deps-llvm:
	wget https://apt.llvm.org/llvm.sh
	chmod +x llvm.sh
	sudo ./llvm.sh 14 all
	rm -f llvm.sh

deps-debian: deps-llvm
	sudo apt update
	sudo apt install nasm xorriso mtools grub-common grub-efi-amd64 grub-pc-bin build-essential qemu-system-x86 ovmf  -y

clean:
	sudo rm -rf ./Bin ./Sysroot ./Sources/*/*/*/*/*/Lib ./Sources/*/*/*/*/Lib ./Sources/*/*/*/Lib ./Sources/*/*/Lib ./Sources/*/Lib

deps-github-action: deps-llvm
	sudo apt update
	sudo apt install nasm xorriso mtools

github-action: deps-github-action build

.PHONY: build run deps-llvm deps-debian
