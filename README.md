# Simple Shell

## Description
The **simple_shell** is a simple UNIX command line interpreter written in C language.  
It mimics the basic functionality of the **sh** shell by reading commands from the user, executing them, and displaying the output.

## Features
- Display a prompt and wait for the user to type a command.
- Handle simple commands with arguments.
- Handle the PATH variable to locate executables.
- Support built-in commands such as `exit` and `env`.
- Handle EOF (Ctrl+D) and command execution errors.
- Works in both interactive and non-interactive modes.

## Requirements
- Allowed editors: `vi`, `vim`, `emacs`
- Compilation:
  ```bash
  gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
