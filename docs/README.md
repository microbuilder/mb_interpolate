# Interpolation for Apache Mynewt

This package contains a number of functions to interpolate between two
values based on the following algorithms:

- Nearest neighbour (AKA 'piecewise constant') interpolation
- Linear (AKA 'piecewise linear') interpolation
- Natural cubic spline interpolation

It is designed to work with the [Apache Mynewt](https://mynewt.apache.org/) operating system, and was written using version 1.4.1.

## Dependencies

### Package Dependencies

This package has no package dependencies outside of
[mynewt-core](https://github.com/apache/mynewt-core).

### Libraries

This packages requires **libm** (math lib) support, with libm added as part
of the normal package build process.

## Using this Package

The easiest way to make use of this package is to add it as a project
dependency. This will pull the latest version of the library from Github:

    $ newt TODO

## Development Status

Check [issue tracking](https://github.com/microbuilder/mb_interpolate/issues) for the project on Github, or look at the README file in the root of this repo.
