# PES Project 5 Readme


**Team Members:** 
- Prayag Milan Desai
- Rahul Ramaprasad 


---

**Notes**  
This project does not use any KL25Z SDK functions, and relies on the MKL25Z4.h header file.

**Description of repository contents:**  
>This repository contains ten folders, each having multiple source and header files. The description of the folders are as follows:  

**Folder board:**  
This folder contains the library and source files related to the KL25Z FRDM board which have pin init functions and configuration functions. 

**Folder CMSIS:**  
This folder contains source and header files which provide interfaces to processor and peripherals, real-time operating systems, and middleware components.

**Folder drivers:**  
This folder contains drivers for clock, gpio, communication protocols, touch sensor, timer, flash,etc.

**Folder utilities:** 
This folder contains source and header file which facilitate the debugging process.

**Folder startup:**
This folder contains source and header file for the boot process of the freedom board.

**Folder source:** 
This folder contains source files which are common to all build configurations and are compiled based on the macro definitions. The application is made
modular by defining multiple source files. The Project_5_UART.c is defined here, which has the main() function and  is compiled 
for all targets.

**Folder Debug:**
This folder contains the debug target which are created after the compilation of all the source files. This folder also contains the output of the compilation process which is ***'.axf'*** file. The '.axf' file will be executed on the FRDM board.

**Folder Normal:**
This folder contains the normal target, which are created after the compilation of all the source files. This folder also contains the output of the compilation process which is ***'.axf'*** file. The '.axf' file will be executed on the FRDM board.  
**Folder test:**
This folder contains the test target, which are created after the compilation of all the source files. This folder also contains the output of the compilation process which is ***'.axf'*** file. The '.axf' file will be executed on the FRDM board. 

**Folder uCUnit**  
This folder contains functions of uCunit, which is a unit test framework. This unit test framework is used to create and monitor test cases for the program.   
The file `unitTestResults.out` has the output of the unit tests captured in it.

---

**Installation/execution notes for the C code in this repository:**  
All the code in this repository should be executed using the following command:  
`make -r -j3 "target"`  
Where, "target" has three choices
- *Debug*: This target builds the output for the KL25Z. The output LEDs will change according to the states and the messages will be logged.   
- *Normal*: This target builds the output for the KL25Z. The output LEDs will change according to the state changes, and a log output is displayed.
- *Test*: This target builds the output for the PC. This target logs messages after executing uCUnit test cases. 

   

Always run the 'make clean' command before rebuilding other targets.  
Executing the above command results in .axf file which can be executed on FRDM board by clicking the debug option.  
This project has the following requirements, that are fulfilled using the aforementioned command:  
- ARM-GCC cross-compiler : The 'arm-none-eabi-gcc' command is used to compile the FRDM board target code
- Macro flag : The '-D $(target_macro_definition)' to compile the target specific sections of code 
- No errors and warnings: The -Wall flag enables a set of warnings, and the -Werror flag treats the warnings as errors. Since the output of the compiler shows no error, the code in this repo is without any compiler warnings and errors  


**Issues faced**  
The interrupt kept getting fired in the interrupt mode. To solve this issue, the transmit interrupt was enable donly when something had to be sent over UART from KL25Z to PC and the transmit interrupt was disabled at the end of ISR. The application was giving hard faults while testing the circular buffer functionality, by using printf to print the pointer address. This was because the address was unaligned and the printf statement was trying to access an unaligned memory location.  

**Printing Buffer Summary**  
The program works in application or echo mode and in each of these modes it is further split into interrupt or polling mode. The print summary function runs only in the application mode. The "." character is used to trigger the print sumary function, so every time the user types the "." character, the buffer contents along with the frequency of each character in the buffer is printed.  

**Reallocation**  
The buffer starts of with an initial size of of 8 bytes. Every time it reaches the size limit, it realloactes the buffer to twice its original size. If the reallocation fails, the current buffer summary is printed and the buffer reinitialises to 16 bytes.  

**References**  
The UART example from Alexander Dean's github repository was referred, mostly to figure out the flags for polling and interrupt based UART. 




