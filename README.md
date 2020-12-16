![alt text](https://github.com/aroyer-qc/GRBL_Digini_STM32Fx/blob/master/Digini.png)![alt text](https://github.com/aroyer-qc/GRBL_Digini_STM32Fx/blob/master/Grbl.png)

# GRBL_Digini_STM32Fx

  ## Note(s):
  
>   * GRBL is a great application for me to develop the graphic DIGINI platform.
>   * Widget are easy to add in Digini.
    
  ## Feature:

>  `* Code is C++ (following embedded rules found on IAR C++ document Inefficiencies of C++: Fact or Fiction?).
>   * Run on nOS from Jim Tremblay. See [RTOS for microcontrollers](https://github.com/jimtremblay)
>   * Digini graphics run parallel and independant of the main application, all graphics are handles via service.
>   * Data are display by hooking to variables, input are also done via variables. no graphic code exist in application.
>   * Control over the application can be done via state machine and/or message at the discretion of the application user.
  
  ## Widget list so far:
  
>    * Background widget to display background layer.
>    * Basic box.
>    * Basic button with text (no graphic).
>    * Basic rectangle.
>    * Button with text, icon and support for multiple graphic so button can have different state. ( EX. normal, pressed, gray out)
>    * Exchange is a non grahical special widget to exchange data between graphic page.
>    * Gif to display gif as animation.
>    * Icon support for multiple graphic to display different state.
>    * Label to display static/formatted label. also support for different language.
>    * Label list, like label but with a list different label according to state.
>    * Meter, WIP.
>    * Panel, can add touch zone with graphics under other widget.
>    * Progress bar.
>    * Spectrum analyser display (Use data from application, no processing).
>    * Terminal basic terminal screen.
>    * Virtual is a non grahical special widget to decide whart page to display according to application setting.
>    * Virtual windows (WIP).
  
![alt text](https://github.com/aroyer-qc/GRBL_Digini_STM32Fx/blob/master/Preview/IMG_20201118_112744192.jpg)
