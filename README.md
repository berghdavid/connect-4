# connect-4
Model for a connect-4 bot

## How to run
Enter the `src` directory and run `make run-bb` to run the program against itself.
Run `make help` to view all commands.

The C program also allows setting custom parameters for the game. Run 
`./connect-4` to view all the available parameters. For example, the following 
command plays two bots against eachother in a 9x9 board with 2.3 seconds 
thinking time and where the second bot starts: `./connect-4 0 9 9 2.3 -1`
