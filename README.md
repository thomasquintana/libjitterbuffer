# libjitterbuffer

A library implementation of a fixed jitter buffer.

### Installing Dependencies

#### Install Linux Dependencies (Ubuntu)

```
sudo apt-get install -y build-essential binutils g++ gcc scons
```

#### Install Google Test Framework

```
$] sudo apt-get install -y libgtest-dev
$] cd /usr/src/gtest
$] sudo cmake CMakeLists.txt
$] sudo make
$] sudo cp *.a /usr/lib
```

### Compiling libjitterbuffer (Static Library)

```
$] git clone https://github.com/thomasquintana/libjitterbuffer.git
$] cd libjitterbuffer
$] scons
```

Once, `libjitterbuffer` is compiled run the unit tests.

```
$] tests/test_runner
```

### Generating the Docs

```
$] doxygen Doxyfile
```
