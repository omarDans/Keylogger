# Keylogger
Simple Hand-made keylogger

Just a simple keylogger in C++ i did for fun and practice. It has a lot of flaws and a lot of things to implement to get a "reliable performance" but has the essence and i had a lot of fun doing it :)

*I am not responsible for any misuse of the program, this program was created for educational purposes only*

I know noone is going to actually use it but i always wanted to write that ;)

COMPILING:
-You will need libcurl for compiling -> https://curl.se/download.html
-After downloading you need to build curl, you will find information in daddy google :)
-Finally you are probably going to need to reference the include header curl.h and the libcurl.lib package to the compiler and linker respectively, you can do that with this command: cl.exe -o keylogger.exe /I\path\to\folder\curl /link \path\to\libcurl.lib

The curl.h and libcurl.lib should be in the build folder that is created after building curl.

Oh, also you will need the libcurl.dll for executing the keylogger.exe, that file you can also find it in the build folder 
