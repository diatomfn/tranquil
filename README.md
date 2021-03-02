![tranquil banner](https://raw.githubusercontent.com/ScytheApp/tranquil/master/assets/banner.png)

## tranquil
Tranquil if a free and open source Javascript runtime powered by the ChakraCore engine. Designed to be high performance and easy to use.

### Links
[License](https://github.com/ScytheApp/tranquil/blob/master/LICENSE)

### How does it work?
Tranquil at its core is just a modern C++ wrapper for the ChakraCore API aiming to make it more user friendly. It implements its own eventloop and some other systems that make development faster and easier.

### Compiling
This project has been developed on Ubuntu 20.04 and only tested on Ubuntu 20.04. It is not guaranteed to work on any other platform.
```shell script
git clone https://github.com/ScytheApp/tranquil.git
cd tranquil
./setup.sh
cmake CMakeLists.txt
cmake --build build
```