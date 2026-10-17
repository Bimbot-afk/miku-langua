# Miku Language

Miku is an interpreted, dynamically typed, and linearly executed programming language, designed with a syntax and thematic commands inspired by MIKU! It is written entirely in C and allows for direct evaluation from plain text scripts.

tl:dr Its a cool langua
## Main Commands

*   **`note`**: Declares or updates a variable, saving a direct numeric value, the result of a calculation, or external input.
    *   Syntax: `note <variable> = <value>;`
    *   Example: `note miku = 39;`

*   **`sing`**: Solves mathematical operations (addition, subtraction, multiplication, division) or prints the numeric value of variables, projecting the result right to the console.
    *   Syntax: `sing <calculation_or_variable>;`
    *   Example: `sing miku + 1;`

*   **`talk`**: Prints plain text strings directly to the console.
    *   Syntax: `talk "<text>";`
    *   Example: `talk "Starting concert!";`

*   **`check`**: A single-line conditional instruction. It compares two values (using `==` or `!=`) and executes the command that follows *only* if the condition is met.
    *   Syntax: `check <value1> (== | !=) <value2> <command>;`
    *   Example: `check miku == 39 talk "It's Miku!";`

*   **`listen`**: Asks the user to type in a number from the keyboard during the program's execution and saves it directly into a specified variable.
    *   Syntax: `listen <variable>;`
    *   Example: `listen age;`

*   **`gacha`**: Random number generator. Creates a random number between 0 and the indicated maximum value. It is used in combination with the `note` command to assign the result.
    *   Syntax: `note <variable> = gacha <maximum>;`
    *   Example: `note luck = gacha 100;`

## ATTENTION!

*   **`;`**: Instruction terminator. It is a mandatory delimiter at the end of each individual command (`note`, `sing`, `talk`, `listen`) to indicate to the interpreter that the line has finished.