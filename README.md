# libjitterbuffer

A library implementation of a fixed jitter buffer.

### Installing Dependencies

#### Install Linux Dependencies (Ubuntu)

```
sudo apt-get install -y build-essential binutils g++ gcc scons
```

### Compiling libjitterbuffer (Static Library)

```
$] git clone https://github.com/thomasquintana/libjitterbuffer.git
$] cd libjitterbuffer
$] scons
```

Once, `libspectrum` is compiled run the unit tests.

```
$] tests/test_runner
```

### Generating the Docs

```
$] doxygen Doxyfile
```
