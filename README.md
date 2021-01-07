![alt text](https://github.com/aroyer-qc/GRBL_Digini_STM32Fx/blob/master/Digini.png)

# GRBL_Digini_STM32Fx

Grbl is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. This version will run on a stm32f7-discovery and later on other STM32 family.
The controller is written in highly optimized C utilizing every clever feature of the STM32F7 to achieve precise timing and asynchronous operation. It will be able to maintain more than the original 30kHz from Arduino of stable, jitter free control pulses.

Digini is a HAL and GUI library written in embedded C++. It is highly optimized and is also a highly configurable graphic library. GUI is based on Widget/Service concept. What this mean is, the application like GRBL run on his own loop, and is not aware the GUI even exist. the GUI on his side is running multiple task to display graphic page. Those page are a chain list of widget, and those widget are configured with information about position, graphic to use, service to call to display the proper information and link to other page. all information is processed via variables and/or call to users functions.

Digini run on a RTOS for microcontrollers called nOS (nano OS).
[nOS](https://github.com/jimtremblay/nOS)


Preview until ready to commit

![alt text](https://github.com/aroyer-qc/GRBL_Digini_STM32Fx/blob/master/Preview/IMG_20201118_112744192.jpg)
