# Simple C Logger (qlog)

A simple, lightweight logging library for C projects.

## Building the Library

### Prerequisites
Make sure you have the standard build tools installed:
```bash
sudo apt install build-essential
```

### Build Instructions

You can build either a static or a shared (dynamic) library depending on your needs.

**To build a static library:**
```bash
make release
```
*Output:* `bin/qlog.a`

**To build a shared (dynamic) library:**
```bash
make release_shared
```
*Output:* `bin/qlog.so`

## Usage

1. Build the library using the instructions above.
2. Add the include directory to your compiler flags (e.g., `-IIncludes/`).
3. Link the compiled library (`bin/qlog.a` or `bin/qlog.so`) to your project.

### Example

To enable logging, define the `LOG_ON` macro before including the header file.

```c
#define LOG_ON
#include "log.h"

int main() {
    LOGI("HELLO LOGS");
    return 0;
}
```

## Configuration (`qlog.config`)

You can configure the logger by creating a `qlog.config` file in your working directory. 

Here is an example configuration:

```ini
FMT="_%l_ [%f] %p:%n\n\t"
DATE_FMT="[%H:%M:%S]"
LEVEL="DEBUG"
WR_STDERR=1
WR_FILE=0
LOG_ON=1
FILEPATH="test.log"
```

### Configuration Parameters:

*   **`FMT`** — The log output format. Available format specifiers:
    *   `%l` — Log level (e.g., DEBUG, INFO)
    *   `%f` — Function name
    *   `%p` — File path
    *   `%n` — Line number
*   **`DATE_FMT`** — The date and time format.
*   **`LEVEL`** — Minimum log level to record (e.g., `DEBUG`).
*   **`WR_STDERR`** — Write logs to `stderr` (`1` to enable, `0` to disable).
*   **`WR_FILE`** — Write logs to a file (`1` to enable, `0` to disable).
*   **`LOG_ON`** — Global switch to enable (`1`) or disable (`0`) logging entirely.
*   **`FILEPATH`** — Path to the log file (used if `WR_FILE=1`).