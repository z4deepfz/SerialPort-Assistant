Serial Port Assistant v0.5a
====
Free, open source project for serial communication
----

## Begin

The project is quite **imperfect**, welcome to commit or open issus.

***Windows version* has been tested and released. Please go to *release* page.**

## Features

 - Cross-platform
 - Adaptive layout
 - (Other features are under developing...)
 
## TODO

*Following features are under consideration for next versions*

 - [ ] CMakeLists.txt
 - [ ] Switch ASCII and HEX mode for sending and recieving
 - [ ] Tool bar (but I think that's useless...)
 - [ ] Multiple Language
 - [ ] English notes
 - [ ] Fix the compile problem under Linux

## Compile 

Dependencies:

 - wxWidgets v3.0.4
 - wxMathPlot v0.1.2
 - Boost v1.73
 
Remark: lower versions may also be available.

### Windows

You can choose any of following:

1. mingw32-make msw
2. mingw32-make msw_x86_64

Please copy or create the links of the library file into ./lib and include file in ./include folder.

 - head files of wxWigets & wxMathPlot -> ./include/wx
 - head files of Boost -> ./include/boost
 - static x86 library files of wxWidgets -> ./lib/i386
 - static x86_64 libraray files of wxWidgets -> ./lib/x86_64


### Linux

**Not avalible yet, the source code needs to be modified to pass the build. I'll fix it in the future.**

1. ~~make linux~~

~~Please make sure the libraray *boost* *wxWidgets* and *wxMathPlot* have been installed correctly and the path has been added into environment varible. ~~

Remark: **This project has not been tested under Linux**

~~Remark: You can easily build 32-bit version on linux. But I don't recommand to do so.~~
