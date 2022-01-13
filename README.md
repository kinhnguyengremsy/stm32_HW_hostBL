# stm32_HW_hostBL

# create bin file in keil c
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin gremsyTestBoard\gremsyTestBoard.axf --output gremsyTestBoard\gremsyTestBoard.bin

# merge hex
- document : https://developer.arm.com/documentation/ka004500/latest
- cai dat moi truong 
- srec_cat.exe "Programmer/Programmer.hex" -Intel "E:\GremsyGremsy\Gremsy_HAL_Project\STM32F0\Blink\MDK-ARM\Blink\Blink.hex" -Intel -offset 0xE0000 -o MergedHexFile.hex -Intel