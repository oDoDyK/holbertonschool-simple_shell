# ⭐️ C - Simple Shell
The Simple Shell project is a basic implementation of a UNIX command interpreter written in C.
It aims to mimic the behavior of the standard shell by handling user commands, executing programs,
and supporting built-in functions such as `cd`, `exit`, `env`, `setenv`, and `unsetenv`.
This project provides hands-on experience with processes, system calls, memory management,
and the inner workings of a command-line environment.

## 📑 Table of Contents  

- ⚙️ Description
- 🧩 Project Files  
- 💻 Built-in Commands
- 🧠 Learning Objectives  
- 🏗️ Requirements  
- 🚀 Compilation & Usage
- ✍️ Authors
- 💡 Reflection    

---
## ⚙️ Description

This project is part of the *Holberton School Low-Level Programming curriculum*,  
and focuses on building a simple command-line interpreter.

It replicates the core features of a Unix shell:
- Displaying a prompt and waiting for user input  
- ⁠Executing commands found in the system ⁠ *PATH* ⁠  
- Managing built-in commands like ⁠ cd ⁠, ⁠ exit ⁠, ⁠ env ⁠, ⁠ setenv ⁠, and ⁠ unsetenv ⁠  
- ⁠Handling command chaining `(⁠ ; ⁠)`, logical operators `(⁠ && ⁠, ⁠ || ⁠)`.  

The project deepens understanding of *process creation, system calls, memory management*,  
and overall *shell behavior* from the inside.

---

## 🧠 Learning Objectives

By completing this project, we learned to:
- Use system calls such as ⁠ `fork` ⁠, ⁠ `execve` ⁠, and ⁠ `wait` ⁠.
- ⁠Manage processes and file descriptors.
- Handle user input efficiently with buffers.
- ⁠Manage environment variables dynamically.
- Implement recursion, parsing, and error handling.
- ⁠Understand logical operators ⁠ `&&` ⁠, ⁠ `||` ⁠, and command chaining ⁠ `;` ⁠.

---

## 🏗️ Requirements

- Allowed editors: `vi`, `vim`, `emacs`
- All your files will be compiled on Ubuntu 20.04 LTS using
  `gcc, using the options -Wall -Werror -Wextra -pedantic -std=gnu89`
- All your files should end with a new line
- A `README.md` file, at the root of the folder of the project is mandatory
- Your code should use the Betty style. It will be checked using `betty-style.pl` and `betty-doc.pl`
- Your shell should not have any memory leaks
- more than 5 functions per file
- All your header files should be include guarded
- Use system calls only when you need to (why?)

--- 

## 🧩 Project Files

| File | Description |
|------|--------------|
| ⁠ main.c ⁠ | Entry point of the shell; initializes the environment, starts the main loop, and manages program execution. |
| ⁠ shell_logic.c ⁠ | Core logic for parsing commands, handling syntax, and executing built-in or external programs. |
| ⁠ io.c ⁠ | Handles input and output, displays the shell prompt, and reads user commands efficiently. |
| ⁠ env_builtins.c ⁠ | Implements ⁠ env ⁠, ⁠ setenv ⁠, and ⁠ unsetenv ⁠ built-ins to manage environment variables dynamically. |
| ⁠ builtin_cd.c ⁠ | Implements the ⁠ cd ⁠ command, managing directory navigation and path updates. |
| ⁠ shell_exit.c ⁠ | Handles the ⁠ exit ⁠ command and performs proper memory cleanup before termination. |
| ⁠ signal_handler.c ⁠ | Manages system signals like ⁠ Ctrl+C ⁠ to ensure stable and smooth shell operation. |
| ⁠ memory.c ⁠ | Provides custom memory utilities for safe allocation, freeing, and overall stability. |
| ⁠ vector.c ⁠ | Handles dynamic string vectors and command argument management. |
| ⁠ simple_shell.h ⁠ | Header file containing all prototypes, macros, and structures used across the project. |

---

## 💻 Built-in Commands

| Command | Description |
|----------|--------------|
| ⁠ exit⁠ | Exits the shell with an optional status code. Performs cleanup before termination. |
| ⁠ env ⁠ | Prints the current environment variables available to the shell. |
| ⁠ setenv⁠ | Creates a new environment variable or updates an existing one. |
| ⁠ unsetenv ⁠ | Removes an existing environment variable from the shell environment. |
| ⁠ cd | Changes the current working directory to the specified path. |

---

# 🚀 Compilation & Usage 

```
$ gcc -Wall -Wextra -Werror -pedantic -std=gnu89 *.c -o hsh
$ ./hsh
$ echo Hello, Nada!
Hello, Nada!
$ alias greet='echo Welcome Nada!'
$ greet
Welcome Nada!
$ exit
👋
```
---

## ✍️ Authors

## 👨🏽‍💻 Mohammed Abdulrahman Basuliman
📍 Riyadh, Saudi Arabia  
🎓 IT Student at Saudi Electronic University  
🎓 Student at Holberton School  
📧 4thkiller2005@gmail.com  
🔗 GitHub: [oDoDyK](https://github.com/oDoDyK)

## 👩🏻‍💻 Nada Ghannam Al-Mutairi
📍 Riyadh, Saudi Arabia  
🎓 IS Student at AlMaarefa University  
🎓 Student at Holberton School  
📧 Nada.ghannam19@gmail.com  
🔗 GitHub: [NadaGhannam25](https://github.com/NadaGhannam25)

---

## 💡 Reflection
```
Building a shell from scratch changes the way you think about how commands, processes, and systems truly work.
Every line of code here brings you one step closer to mastering the core of the operating system.🧠⚡️
```
