# Object_processing_engine
A project created during the C++ course for training purposes.

Sorts a list of objects, and interacts with files.
The program uses Russian as the language of its interface.

# Compiling
You need: 
* Qt 5.12 and above
  
`$ git clone https://github.com/FLev-Dev/Object_processing_engine`

`$ cd Object_processing_engine`

`$ mkdir build && cd build`

`$ qmake ../Object_processing_engine.pro`

`$ mingw32-make` or, if you want to use it more quickly, you can use `$ mingw32-make -j8`
This will increase the number of processes to 8.

 ***NOTE:*** You should use the compiler provided by Qt. You can find it here: "Qt\Qt5.12.12\Tools\mingw\bin\mingw32-make.exe".

 # Running
The executable file can be found in the build/release directory.

In order for the program to correctly process the files, they must be formatted according to a specific structure. 
Each object must be on its own line. The line must contain data separated by spaces: the name of the object, its x and y coordinates, the type of object, and the time it was created in the format of a time_t value.
**For example** you can create an object in program and save it to a file.
![](https://sun119-1.userapi.com/impg/EbfZQij8wPWIBPSeshU8-RTk5C8vzcMobH7Pqw/3uwyf5msBrI.jpg?size=368x259&quality=95&sign=44d0b1a5aabb42341e3c7c6f82e46d0d&c_uniq_tag=pjp82MIBdyDa82-kbHKU0B67dDAqpFU8xWDHOetfGqI&type=album)
 
***NOTE:*** At startup, the program looks for a file named "data.txt" in the "build" directory and downloads it if it exists. The file's name can be customized in the     File_Manager class by setting the default_file variable. And also, this name change(until the next launch) occurs after loading a file from the UI menu of the program.


