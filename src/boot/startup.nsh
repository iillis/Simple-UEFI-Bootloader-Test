@echo -off

;foundimage section is simply to locate the correct drive
cls
if exist .\EFI\BOOT\BOOTX64.EFI then
echo "found the bootloader"
goto FOUNDIMAGE
endif
if exist fs0:\EFI\BOOT\BOOTX64.EFI then
fs0:
echo "found the bootloader on fs0:"
goto FOUNDIMAGE
endif
if exist fs1:\EFI\BOOT\BOOTX64.EFI then
fs1:
echo "found the bootloader on fs1:"
goto FOUNDIMAGE
endif
if exist fs2:\EFI\BOOT\BOOTX64.EFI then
fs2:
echo "found bootloader on fs2:"
goto FOUNDIMAGE
endif
echo "Unable to find the bootloader"
echo "Please mount the drive with the adov operating system configuration"
goto END
:FOUNDIMAGE
echo "booting the bootloader..."
.\EFI\BOOT\BOOTX64.EFI
:END
echo "exiting..."