# General terms 

### What is canonical mode? 
Canonical mode is a configuration in the terminal that only sends keyboard input when the user presses `enter`.

## Flags

- `ECHO`: causes all user input to be echoed back to the terminal.
- `ICANON`: turns of canonical mode, which means our program reads byte by byte as opposed to line by line.

## Further research
`TCSAFLUSH`

## New functions I've learned
- `atexit()`

## Escape sequences
`ctrl-m` should expect 13 but we get 10. This is because the terminal is turning carriage returns `\r` (13) into `\n` (10).
