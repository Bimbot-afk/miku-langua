# Miku Language

````mikuTextMain
talk "HELLO WORLD!!! :D";
````


Miku is an interpreted, dynamically typed, and linearly executed programming language, designed with a syntax and thematic commands inspired by MIKU! It is written entirely in C and allows for direct evaluation from plain text scripts[cite: 1].

tl:dr Its a cool langua

## Main Commands

-   **`note`**: Declares or updates a variable, saving a direct numeric value, the result of a calculation, or external input.
    -   Syntax: `note <variable> = <value>;`
    -   Example: `note miku = 39;`

-   **`sing`**: Solves mathematical operations (addition, subtraction, multiplication, division, modulo) or prints the numeric value of variables, projecting the result right to the console.
    -   Syntax: `sing <calculation_or_variable>;`
    -   Example: `sing miku + 1;`
    -   Example: `sing miku % 2;`

-   **`talk`**: Prints plain text strings directly to the console.
    -   Syntax: `talk "<text>";`
    -   Example: `talk "Starting concert!";`

-   **`check`**: A single-line conditional instruction. It compares two values (using `==`, `!=`, `<`, or `>`) and executes the command or block that follows *only* if the condition is met.
    -   Syntax: `check <value1> (== | != | < | >) <value2> <command_or_block>;`
    -   Example: `check miku == 39 talk "It's Miku!";`

-   **`listen`**: Asks the user to type in a number from the keyboard during the program's execution and saves it directly into a specified variable.
    -   Syntax: `listen <variable>;`
    -   Example: `listen age;`

-   **`gacha`**: Random number generator. Creates a random number between 0 and the indicated maximum value. It is used in combination with the `note` command to assign the result.
    -   Syntax: `note <variable> = gacha <maximum>;`
    -   Example: `note luck = gacha 100;`

-   **`encore`**: for loop, repeats a single command or an entire code block during the amount of times its asked.
    -   Syntax: `encore <amount of times> <action_to_repeat_or_block>;`
    -   Example: `encore 67 talk "67";`

## Blocks & Structure

-   **`{ }`**: Code blocks. Use curly braces to group multiple commands together so they can be executed sequentially within conditional checks (`check`) or loops (`encore`).
    -   Example:
        ```miku
        check score > 10 {
            talk "High score!";
            note bonus = 5;
        }
        ```

## ATTENTION!

-   **`;`**: Instruction terminator. It is a mandatory delimiter at the end of each individual command (`note`, `sing`, `talk`, `listen`) to indicate to the interpreter that the line has finished.

## VS Code Extension (Syntax Highlighting)

To get colors and syntax support for `.miku` files:

-   Copy the **`miku-theme`** folder into your VS Code extensions directory:
    -   **Windows**: `C:\Users\<your_user>\.vscode\extensions\`
    -   **Linux / macOS**: `~/.vscode/extensions/`
-   Restart VS Code (or press `Ctrl + Shift + P` and run **`Developer: Reload Window`**).
-   If you use another IDE, just search the extension folder and paste the folder there.