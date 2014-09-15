Beruben Interpreter
===================

An interpreter for the Beruben language.
See docs/beruben_specification.md for details on syntax.

License: LGPL 2.1 or later

Written by rubenwardy.

Building
========

Windows builds will be provided at a later date.

Linux
-----

```
sudo apt-get install build-essentials
cmake .
make -j3
./bin/befunge examples/test.br -s
