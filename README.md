# C-Shell

A custom Unix shell implementation in C with support for process management, job control, piping, I/O redirection, and command history.

## Features

### Core Shell Features
- **Custom Prompt**: Displays username, hostname, and current directory
- **Command Execution**: Execute both built-in and system commands
- **Background/Foreground Jobs**: Run processes in background with `&` or foreground
- **Job Control**: Full support for process suspension, resumption, and management
- **Signal Handling**: Proper handling of `Ctrl+C` (SIGINT) and `Ctrl+Z` (SIGTSTP)
- **Piping**: Chain multiple commands with `|` operator
- **I/O Redirection**: Redirect input (`<`) and output (`>`) of commands
- **Command Chaining**: Execute multiple commands with `;` (sequential) and `&&` (conditional)

### Built-in Commands

#### Navigation
- **`hop <path>`**: Change directory with support for:
  - `~` - Home directory
  - `.` - Current directory
  - `..` - Parent directory
  - `-` - Previous directory
  - Multiple paths in sequence

#### File Operations
- **`reveal <flags> <path>`**: List directory contents
  - `-a` - Show hidden files (starting with `.`)
  - `-l` - Long format with detailed information
  - Can combine flags: `reveal -la <path>`

#### Process Management
- **`activities`**: Display all running/stopped background processes
  - Shows job number, PID, process state, and command name
  - Sorted by command name lexicographically

- **`ping <pid> <signal>`**: Send signals to processes
  - Signal number modulo 32 is applied
  - Example: `ping 1234 9` sends SIGKILL

- **`fg <job_number>`**: Bring background job to foreground
  - Without argument: brings most recently backgrounded job
  - With job number: brings specific job to foreground

- **`bg <job_number>`**: Resume stopped background job
  - Without argument: resumes most recently stopped job
  - With job number: resumes specific job

#### Command History
- **`log`**: Display command history (last 15 commands)
- **`log purge`**: Clear command history
- **`log execute <index>`**: Execute command at given index
- Command log is persistent across shell sessions
- Stored in `shell_log.txt` in home directory

## Installation

### Prerequisites
- GCC compiler
- Linux/Unix environment
- GNU Make

### Building the Shell

1. Clone or navigate to the project directory:
```bash
cd C-Shell
```

2. Build the project:
```bash
make
```

This will compile all source files and create the `shell.out` executable.

3. To build without running:
```bash
gcc -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 \
    -Wall -Wextra -Werror -Wno-unused-parameter \
    -fno-asm -Iinclude src/*.c -o shell.out
```

## Running the Shell

### Start the Shell
```bash
./shell.out
```

Or build and run in one command:
```bash
make
```

### Exiting the Shell
Use `Ctrl+D` or type `exit` to quit the shell.

## Usage Examples

### Basic Commands
```bash
# Navigate to home directory
hop ~

# List files in current directory
reveal .

# List all files including hidden, with details
reveal -la /home/user

# Change to multiple directories
hop dir1 dir2 dir3

# Return to previous directory
hop -
```

### Process Management
```bash
# Run command in background
sleep 100 &

# View all background processes
activities

# Bring job 1 to foreground
fg 1

# Resume stopped job 2 in background
bg 2

# Send SIGTERM (15) to process
ping 1234 15
```

### Piping and Redirection
```bash
# Pipe commands
cat file.txt | grep "pattern" | wc -l

# Input redirection
sort < input.txt

# Output redirection
echo "Hello World" > output.txt

# Combine redirection and piping
cat < input.txt | grep "error" > errors.txt
```

### Command Chaining
```bash
# Sequential execution (runs both regardless of success)
mkdir test ; cd test

# Conditional execution (runs second only if first succeeds)
make && ./a.out

# Mixed operations
hop /tmp && reveal -l && hop -
```

### Command History
```bash
# Show command history
log

# Execute 5th command from history
log execute 5

# Clear history
log purge
```

### Signal Handling
- **`Ctrl+C`**: Send SIGINT to foreground process (does not terminate shell)
- **`Ctrl+Z`**: Send SIGTSTP to foreground process (suspend to background)
- **`Ctrl+D`**: Exit shell (EOF)

## Project Structure

```
C-Shell/
├── makefile           # Build configuration
├── README.md          # This file
├── include/           # Header files
│   ├── activities.h   # Process/job management
│   ├── file.h         # File operations
│   ├── hop.h          # Directory navigation
│   ├── input.h        # User input handling
│   ├── intrinsic.h    # Built-in commands (ping, fg, bg)
│   ├── log.h          # Command history
│   ├── parser.h       # Command parsing
│   ├── pipe.h         # Pipeline execution
│   ├── prompt.h       # Shell prompt
│   ├── reveal.h       # Directory listing
│   └── shell.h        # Core shell definitions
└── src/               # Source files
    ├── activities.c   # Process tracking and job control
    ├── file.c         # File I/O operations
    ├── hop.c          # cd command implementation
    ├── input.c        # Input reading
    ├── intrinsic.c    # Built-in command implementations
    ├── log.c          # Command history management
    ├── main.c         # Main shell loop and signal handlers
    ├── parser.c       # Command line parsing
    ├── pipe.c         # Pipeline and command execution
    ├── prompt.c       # Prompt generation
    └── reveal.c       # ls command implementation
```

## Implementation Details

### Process Management
- Maintains a global process list (up to 1024 processes)
- Tracks job number, PID, command name, state, and background status
- Process states: Running (1), Stopped (0), Terminated (2)
- Automatic cleanup of terminated background processes

### Signal Handling
- Custom handlers for SIGINT and SIGTSTP
- Signals are forwarded to foreground process group only
- Shell remains unaffected by these signals
- SIGTTOU is ignored for proper terminal control

### Command Parsing
- Supports complex command structures with pipes, redirection, and chaining
- Handles both `&` (background), `&&` (conditional), and `;` (sequential)
- Robust parsing of quoted strings and special characters

### Command History
- Stores last 15 unique commands (excluding `log` commands)
- Persistent across shell sessions in `~/shell_log.txt`
- Automatically removes duplicate consecutive commands

## Technical Specifications

- **Language**: C (C99 standard)
- **POSIX Compliance**: POSIX.1-2008
- **Compiler Flags**: 
  - `-Wall -Wextra -Werror`: Strict error checking
  - `-D_POSIX_C_SOURCE=200809L`: POSIX 2008 features
  - `-D_XOPEN_SOURCE=700`: X/Open 7 features
- **Maximum Input Size**: 4096 characters
- **Maximum Path Length**: 4096 characters
- **Maximum Processes**: 1024 concurrent jobs
- **Log Capacity**: 15 commands

## Limitations

- No support for globbing (wildcard expansion)
- No command line editing (arrow keys)
- No tab completion
- Limited to 1024 concurrent jobs
- Command history limited to 15 entries
- No scripting support

## Error Handling

The shell provides informative error messages for:
- Invalid syntax
- File/directory not found
- Permission denied
- Invalid process IDs
- Signal errors
- Memory allocation failures

## License

This project is created for educational purposes.

## Author

Developed as part of an Operating Systems course project.
