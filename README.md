# Streaming ASCII-to-floating-point (atof)

The C language library and many other programming environments has a function
to convert ASCII strings to floating point numbers. Typically, the function is
implemented in a manner that it gives the nearest floating point number without
any rounding errors in the algorithm. This results in the desirable property
that if you ever convert a floating point number to ASCII string with enough
precision (let's say using the format specifier `"%.17g"`), the ASCII string
will always convert to the same floating point value.

However, sometimes you may encounter problems where the floating point number
may not be `'\0'`-terminated -- you may have a length instead specifying the
string length, and the string may be followed by any junk data. You also may
encounter problems where the entire floating point value is not in the memory
at the same time, arriving instead one character or larger chunk at a time. The
`atof`/`strtod` interface does not handle these problems.

If programming with integers, it is trivial to create a streaming incremental
parser converting ASCII strings to integers. With floating point numbers, it is
not easy, as you cannot rely on the `long double` data type (some processors
may not support it, if the processor supports it the compiler may not support
it, even if the processor and compiler support it the operating system on the
architecture may have set the FPU to a mode where the full precision is not
available). If `long double` was always available, it would not be a herculean
task to implement a streaming incremental parser converting an incrementally
arriving ASCII string to a floating point number of the `double` precision as
the increased precision allows avoiding rounding errors. Alas, `long double` is
not always available, and the reimplementation of `atof` without rounding
errors is thus very hard.

The streaming atof library attempts to solve these problems. It sets aside a
small chunk of memory. It stores the arriving data into this chunk, but the
chunk may overflow. All three types of overflows are handled:

* A really long integer part in the floating point number, example: `900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001.5`
* A really long floating point part in the floating point number, example: `9.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000015`
* A really long list of zeros at start, example: `0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000015`
* A really long number followed by an exponent, example: `-9.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000015e-2`

The long integer part is handled by discarding digits at the end and keeping
track of how the discarded digits would affect the exponent. The long floating
point part is handled by discarding digits (in this case they can be discarded
with no special processing). The really long list of zeros at start is handled
by starting to store numbers only whenever encountering the first nonzero
number and keeping track of how this zero-discarding at the start would affect
the exponent. The really long number followed by an exponent is handled too by
keeping enough room in the buffer to eventually store the exponent.

One case that is not handled properly is a zero, decimal separator, and a long
list of zeros whose length would overflow a 64-bit signed integer, followed by
a large positive exponent whose value would overflow a 64-bit signed integer.
The two overflows could negate each other, being in different directions, and
thus, could result in a reasonable number like `15.3`. Note that a 100Gbps link
would take over 23 years to transfer the ASCII representation of such a
pathological number. Thus, it is unlikely such numbers will ever be encountered
in practice. Such a number does not crash the library; instead, an incorrect
number will merely be returned with no other ill effects.

## Caveats

It is expected that the user of the library never feeds invalid data into the
streaming atof library. Thus, any user who would like to parse numbers whose
textual representation is not a valid JSON number should validate the data
before feeding in to the library.

Failure to check the validity of data can lead to an `abort()`.

## How to build

Streaming atof is built using Stirmake. How to install it:

```
git clone https://github.com/Aalto5G/stirmake
cd stirmake
git submodule init
git submodule update
cd stirc
make
./install.sh
```

This installs stirmake to `~/.local`. If you want to install to `/usr/local`,
run `./install.sh` by typing `sudo ./install.sh /usr/local` (and you may want
to run `sudo mandb` also).

If the installation told `~/.local` is missing, create it with `mkdir` and try
again. If the installation needed to create `~/.local/bin`, you may need to
re-login for the programs to appear in your `PATH`.

Then build streaming atof by:

```
cd streamingatof
smka
```

and test it by:

```
./test
```

## License

All of the material related to streaming atof is licensed under the following
MIT license:

Copyright (C) 2020 Juha-Matti Tilli

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
