# chess_dbms
Our code can be used to perform operations on PGN files which contain all the moves for a specific game. The PGN files are being parsed into bitboards. Each move is represented as bitboards of 64 bits (8 x 8). 
The code also takes moves as input and store them as binary files. 
Additionally, we can search for a particular move by giving a move and the colour of the piece to search for in all the game files. It returns a list of all game ids which contains the input move. 

So broadly, This C++ program allows us to manage and analyze chess games stored in Portable Game Notation (PGN) format. It provides functionality to:

Parse PGN strings into game objects
Save and load games to and from binary files
Search for specific moves within multiple games
Represent chess positions using bitboards 

# Usage:
Parsing PGN Strings
You can create a new game object by parsing a PGN string:
Game game("PGN string", "White Player", "Black Player");


# Saving and Loading Games
Save a game to a binary file:
string filename = game.saveToFile();

# Load a game from a binary file:
Game loadedGame("FEN string", "White Player", "Black Player");
loadedGame.loadFromFile("game1.bin");

# Searching for Moves
Search for a specific move within multiple games:vector<int> foundGameIds = searchForMove("e3w");

# Bitboard Representation
Represent chess positions using bitboards:
BitBoard FINALBITBOARD = makeBitboards(pgn, 2);



