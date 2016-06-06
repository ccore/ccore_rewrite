To compile and install ccore on Linux (Debian is used as an example):

1. Install the dependencies
2. Setup the makefiles and build it `autoreconf --verbose --install && ./configure && make`
3. The library object is now available from the .libs folder
4. (Optional) Build and install as a Debian package `debuild -us -uc -i -b && sudo dpkg -i ../lib*.deb`
