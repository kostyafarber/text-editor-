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
- 'ioctl()' used to get window sizes of a terminal, not always accurate though.
## Escape sequences
`ctrl-m` should expect 13 but we get 10. This is because the terminal is turning carriage returns `\r` (13) into `\n` (10).

## ctrl 
When we press control and any of the alphabet characters we start counting from 0...n, e.g `ctrl-a` is 1.

This is because `ctrl` strips the 5th and 6th bit from the character pressed in combination with `ctrl`.

For example the character `a` is 97 in ascii and 
```
1100001
```
in binary. If we strip the 5th of 6th bits we get the number 1.

If we do the same with `b` we get 2 and so on.
