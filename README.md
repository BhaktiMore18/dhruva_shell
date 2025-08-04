# Dhruva shell

The `dhruva_shell` project is a basic command-line program, acting like a *mini-operating system interface*. It works by continuously *listening* for your commands, *understanding* what you want to do, and then *running* those actions for you. Think of it as a **simple but functional tool** that lets you interact with your computer by typing instructions.


## Visual Overview

```mermaid
flowchart TD
    A0["Shell Main Loop
"]
    A1["Input Handler
"]
    A2["Command Parser
"]
    A3["Command Dispatcher
"]
    A4["Built-in Commands
"]
    A5["External Program Launcher
"]
    A6["Dynamic Memory Manager
"]
    A0 -- "Reads input" --> A1
    A0 -- "Parses input" --> A2
    A0 -- "Executes commands" --> A3
    A1 -- "Manages memory" --> A6
    A2 -- "Manages memory" --> A6
    A3 -- "Handles built-ins" --> A4
    A3 -- "Launches programs" --> A5
```

## Chapters

1. [Shell Main Loop
](01_shell_main_loop_.md)
2. [Input Handler
](02_input_handler_.md)
3. [Command Parser
](03_command_parser_.md)
4. [Command Dispatcher
](04_command_dispatcher_.md)
5. [Built-in Commands
](05_built_in_commands_.md)
6. [External Program Launcher
](06_external_program_launcher_.md)
7. [Dynamic Memory Manager
](07_dynamic_memory_manager_.md)

---
