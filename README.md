# FairLogger

Lightweight and fast C++ Logging Library

| Branch | Build Status |
| :---: | :--- |
| `master` | ![build status master branch](https://alfa-ci.gsi.de/buildStatus/icon?job=FairRootGroup/FairLogger/master) |
| `dev` | ![build status dev branch](https://alfa-ci.gsi.de/buildStatus/icon?job=FairRootGroup/FairLogger/dev) |

## Installation

```bash
git clone https://github.com/FairRootGroup/FairLogger
mkdir FairLogger_build && cd FairLogger_build
cmake -DCMAKE_INSTALL_PREFIX=./FairLogger_install ../FairLogger
cmake --build . --target install
```

FairLogger bundles a version of the [fmt](https://github.com/fmtlib/fmt) library. You can override this with your own fmt installation via the `-DUSE_EXTERNAL_FMT=ON` and `-DFMT_ROOT=/fmt/location` CMake switches.

## Usage

In your `CMakeLists.txt`:

```cmake
find_package(FairLogger)
```

If FairLogger is not installed in system directories, you can hint the installation location:

```cmake
set(CMAKE_PREFIX_PATH /path/to/FairLogger/installation ${CMAKE_PREFIX_PATH})
find_package(FairLogger)
```

`find_package(FairLogger)` will define an imported target `FairLogger::FairLogger`.

If FairLogger is built with `-DUSE_EXTERNAL_FMT=ON`, your project needs to find the external dependencies, too, e.g.

```cmake
find_package(FairLogger)
foreach(dep IN LISTS FairLogger_PACKAGE_DEPENDENCIES)
￼  find_package(${dep} ${FairLogger_${dep}_VERSION})
endforeach()
```

## CMake options

On command line:

  * `-DDISABLE_COLOR=ON` disables coloured console output.
  * `-DBUILD_TESTING=OFF` disables building of unit tests.
  * `-DUSE_EXTERNAL_FMT=ON` uses external fmt instead of the bundled one.

## Documentation

## 1. General

All log calls go through the provided LOG(severity) macro. Output through this macro is thread-safe. Logging is done to cout.

## 2. Additional macros

A number of additional logging macros are provided:

- `LOGF(severity, ...)` The arguments are given to `fmt::format` and the result is logged, e.g. `LOGF(info, "Hello {}!", "world");`
- `LOGP(severity, ...)` The arguments are given to `fmt::printf` and the result is logged, e.g. `LOGP(info, "Hello %s!", "world");`

## 3. Severity

The log severity is controlled via:
```C++
fair::Logger::SetSeverity("<severity level>");
```

where severity level is one of the following:

```C++
"nolog",
"fatal",
"error",
"warn",
"state",
"info",
"debug",
"trace"
```

Logger will log the chosen severity and all above it (except "nolog", which deactivates logging for that sink completely). Fatal severity is always logged.

## 4. Verbosity

The log verbosity is controlled via:
```C++
fair::Logger::SetVerbosity("<verbosity level>");
```

it is same for all sinks, and is one of the following values: `verylow`, `low`, `medium`, `high`, `veryhigh`, which translates to following output:

```
verylow:  message
low:      [severity] message
medium:   [HH:MM:SS][severity] message
high:     [process name][HH:MM:SS][severity] message
veryhigh: [process name][HH:MM:SS:µS][severity][file:line:function] message
```

When running a FairMQ device, the log severity can be simply provided via `--verbosity <level>` cmd option.

## License

GNU Lesser General Public Licence (LGPL) version 3, see [LICENSE](LICENSE).

Copyright (C) 2017-2018 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
