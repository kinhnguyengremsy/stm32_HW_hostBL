# stm32_HW_hostBL
# The communication protocol used by ST bootloader is documented in following ST
# application notes, depending on communication port.

In current version of stm32flash are supported only UART and I2C ports.

* AN3154: CAN protocol used in the STM32 bootloader
  http://www.st.com/web/en/resource/technical/document/application_note/CD00264321.pdf

* AN3155: USART protocol used in the STM32(TM) bootloader
  http://www.st.com/web/en/resource/technical/document/application_note/CD00264342.pdf

* AN4221: I2C protocol used in the STM32 bootloader
  http://www.st.com/web/en/resource/technical/document/application_note/DM00072315.pdf

* AN4286: SPI protocol used in the STM32 bootloader
  http://www.st.com/web/en/resource/technical/document/application_note/DM00081379.pdf

Boot mode selection for STM32 is documented in ST application note AN2606, available in ST website:
  http://www.st.com/web/en/resource/technical/document/application_note/CD00167594.pdf
  
# create bin file in keil c
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin gremsyTestBoard\gremsyTestBoard.axf --output gremsyTestBoard\gremsyTestBoard.bin

# merge hex
- document : https://developer.arm.com/documentation/ka004500/latest
- cai dat moi truong 
- srec_cat.exe "Programmer/Programmer.hex" -Intel "E:\GremsyGremsy\Gremsy_HAL_Project\STM32F0\Blink\MDK-ARM\Blink\Blink.hex" -Intel -offset 0xE0000 -o MergedHexFile.hex -Intel