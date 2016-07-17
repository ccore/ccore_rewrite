To compile and install ccore on Linux (Debian is used as an example):

1. Install the dependencies
2. Setup the makefiles and build it `autoreconf --verbose --install && ./configure && make`
3. The library object is now available from the .libs folder
4. (Optional) Build and install as a Debian package `debuild -us -uc -i -b && sudo dpkg -i ../lib*.deb`

To compile on Linux for Windows:
1. Install the MinGW32 toolchain
	- For mingw-w64: `apt-get install gcc-mingw-w64-i686 mingw-w64-tools gcc-mingw-w64-x86-64`
	- For mingw32: `apt-get install gcc-mingw32 mingw32-binutils`
2. Build
	- For mingw-w64: `autoreconf --verbose --install && ./configure --host=i686-w64-mingw32 && make`
	- For mingw32: TODO
