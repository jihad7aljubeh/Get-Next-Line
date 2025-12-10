# get_next_line

`get_next_line` is a function written in C that reads from a file descriptor **line by line**, returning one line per call, including the newline character (`'\n'`) when present.

This project is part of the 42 curriculum and focuses on:

- File descriptors and low-level I/O
- Static variables and state management between function calls
- Buffer management and memory allocation
- Handling multiple file descriptors at the same time (bonus)

---

## 📚 Table of Contents

- [Overview](#overview)
- [Function Prototype](#function-prototype)
- [How It Works](#how-it-works)
- [Features](#features)
  - [Mandatory Part](#mandatory-part)
  - [Bonus Part](#bonus-part)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Usage](#usage)
- [Examples](#examples)
- [Norme & Constraints](#norme--constraints)
- [Testing](#testing)
- [Author](#author)

---

## Overview

The goal of **get_next_line** is to implement a function that returns a line read from a file descriptor at each call.

A “line” is defined as:
- Any sequence of characters ending with `'\n'`, or
- The end of file if there is no trailing newline.

The function must:
- Work with any valid file descriptor (file, stdin, etc.).
- Handle any `BUFFER_SIZE` defined at compile time.
- Correctly free all allocated memory and avoid leaks.

---

## Function Prototype

```c
char    *get_next_line(int fd);
```

### Return value

- **On success**:  
  - A pointer to a `char *` string containing the line (including `'\n'` if present).
- **If there is nothing more to read or an error occurs**:  
  - `NULL`.

---

## How It Works

At a high level, the function:

1. **Reads from the file descriptor** into a buffer of size `BUFFER_SIZE`.
2. **Stores leftover data** between calls using a static variable.
3. **Searches for a newline** in the accumulated data:
   - If found, it extracts the line up to and including the newline.
   - Keeps the rest as leftover for the next call.
4. **At EOF**:
   - Returns the last chunk if there is any remaining data.
   - Then returns `NULL` on the next call.

---

## Features

### Mandatory Part

- Read **one line at a time** from a given file descriptor.
- Correctly handle files with:
  - Multiple lines
  - Very long lines
  - No newline at the end of file
- Manage memory so there are **no leaks**.
- Work with a configurable `BUFFER_SIZE` (defined when compiling, e.g. `-D BUFFER_SIZE=42`).

Typical mandatory files:

- `get_next_line.c`
- `get_next_line_utils.c`
- `get_next_line.h`

### Bonus Part

If you implemented the bonus:

- Handle **multiple file descriptors at the same time**:
  - For example, you can call `get_next_line` alternately on `fd1` and `fd2` and each will keep its own reading state.
- Use an independent “stash” or buffer per file descriptor.

Typical bonus files (varies by repo style):

- `get_next_line_bonus.c`
- `get_next_line_utils_bonus.c`
- `get_next_line_bonus.h`

---

## Project Structure

A common `get_next_line` repository layout:

```text
.
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line.h
├── get_next_line_bonus.c        # (bonus)
├── get_next_line_utils_bonus.c  # (bonus)
├── get_next_line_bonus.h        # (bonus)
├── main.c                       # (your own tests, not evaluated)
└── README.md
```

You can adapt this section if your filenames are slightly different.

---

## Compilation

### Mandatory

Compile your project with:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c main.c -o gnl
```

You can choose any positive `BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1   ...
cc -Wall -Wextra -Werror -D BUFFER_SIZE=100 ...
```

### Bonus

If you have bonus files:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

> The 42 evaluation will recompile your files with different `BUFFER_SIZE` values, so make sure your code does not depend on a fixed value except through the macro.

---

## Usage

1. **Include the header** in your code:

   ```c
   #include "get_next_line.h"
   ```

2. **Open a file** and call `get_next_line` in a loop:

   ```c
   #include <fcntl.h>
   #include <stdio.h>
   #include "get_next_line.h"

   int main(void)
   {
       int   fd;
       char  *line;

       fd = open("file.txt", O_RDONLY);
       if (fd < 0)
           return (1);
       while ((line = get_next_line(fd)) != NULL)
       {
           printf("%s", line);
           free(line);
       }
       close(fd);
       return (0);
   }
   ```

3. **Always free** the line returned by `get_next_line` after you are done with it.

---

## Examples

### Reading from a file

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
    int   fd;
    char  *line;
    int   i = 1;

    fd = open("example.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", i, line);
        free(line);
        i++;
    }
    close(fd);
    return (0);
}
```

### Bonus: Multiple file descriptors

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line_bonus.h"

int main(void)
{
    int   fd1 = open("file1.txt", O_RDONLY);
    int   fd2 = open("file2.txt", O_RDONLY);
    char  *line1;
    char  *line2;

    if (fd1 < 0 || fd2 < 0)
        return (1);
    line1 = get_next_line(fd1);
    line2 = get_next_line(fd2);
    printf("file1: %s", line1);
    printf("file2: %s", line2);
    free(line1);
    free(line2);
    close(fd1);
    close(fd2);
    return (0);
}
```

---

## Norme & Constraints

- Must respect the **42 Norme**.
- No memory leaks or invalid free.
- No forbidden functions (only those allowed by the subject).
- Use of `static` variables is allowed and expected to keep state between calls.
- Works correctly for:
  - `BUFFER_SIZE` of 1
  - Large `BUFFER_SIZE`
  - Empty files
  - Files without `'\n'` at the end

---

## Testing

You can test your implementation by:

- Writing your own `main.c` with various test cases.
- Using community testers:

  - Some examples (search on GitHub):
    - `gnlTester`
    - `42TESTERS-GNL`
    - `gnl-lover` (names may vary)

Typical flow:

```bash
# In your gnl folder
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

---

## Author

- **Login:** `jalju-be`  
- **School:** 42  
- **GitHub:** [jihad7aljubeh](https://github.com/jihad7aljubeh)
