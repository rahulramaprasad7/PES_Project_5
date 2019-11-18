# PES Project 4 Readme


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
modular by defining multiple source files. The PES Project 4.c is defined here, which has the main() function and  is compiled 
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
- *Normal*: This target builds the output for the KL25Z. The output LEDs will change according to the state changes, and a log output is displayed (The temperature read, and the average calculated, as required).
- *Test*: This target builds the output for the PC. This target logs messages after executing uCUnit test cases. 

   

Always run the 'make clean' command before rebuilding other targets.  
Executing the above command results in .axf file which can be executed on FRDM board by clicking the debug option.  
This project has the following requirements, that are fulfilled using the aforementioned command:  
- ARM-GCC cross-compiler : The 'arm-none-eabi-gcc' command is used to compile the FRDM board target code
- Macro flag : The '-D $(target_macro_definition)' to compile the target specific sections of code 
- No errors and warnings: The -Wall flag enables a set of warnings, and the -Werror flag treats the warnings as errors. Since the output of the compiler shows no error, the code in this repo is without any compiler warnings and errors  


**Issues faced**  
Tried the interrupt based approach for I2C communication with the temperature sensor, ran into some errors. So, switched to polling approach to read the temperature. Ran into errors while reading the THIGH register and realised that we had not put a WAIT signal after writing to TLOW and before writing to THIGH. 





