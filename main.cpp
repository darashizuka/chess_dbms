#include <bitset>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;

// Define a structure for a move using bit fields
struct Move {
  unsigned int wfile : 4;    // 4 bits for file
  unsigned int wrank : 4;    // 4 bits for rank
  unsigned int wpiece : 4;   // 4 bits for piece
  unsigned int wcapture : 1; // 1 bit for capture

  unsigned int bfile : 4;    // 4 bits for file
  unsigned int brank : 4;    // 4 bits for rank
  unsigned int bpiece : 4;   // 4 bits for piece
  unsigned int bcapture : 1; // 1 bit for capture
};

int charToIndex(char c) { return c - 'a'; }

Move parseInputToMove(const string &input) {
  Move move;
  // Extract the file and rank from the input
  char file = input[0];
  int rank = input[1] - '0'; // Convert char to int

  // Set the file and rank based on the input
  if (file >= 'a' && file <= 'h' && rank >= 1 && rank <= 8) {
    if (input[input.length() - 1] == 'w') {
      // White piece
      move.wfile = charToIndex(file);
      move.wrank = rank - 1;
      move.wcapture = 0; // Not capturing
      move.wpiece = 1;

      // Mask black fields
      move.bfile = 0;
      move.brank = 0;
      move.bpiece = 0;
      move.bcapture = 0;
      cout << move.wfile << move.wrank << move.wpiece << move.wcapture << endl;
      cout << move.bfile << move.brank << move.bpiece << move.bcapture << endl;

    } else if (input[input.length() - 1] == 'b') {
      // Black piece
      move.bfile = charToIndex(file);
      move.brank = rank - 1;
      move.bcapture = 0; // Not capturing
      move.bpiece = 1;

      // Mask white fields
      move.wfile = 0;
      move.wrank = 0;
      move.wpiece = 0;
      move.wcapture = 0;
      cout << move.wfile << move.wrank << move.wpiece << move.wcapture << endl;
      cout << move.bfile << move.brank << move.bpiece << move.bcapture << endl;
    } else {
      cerr << "Invalid input format. Please specify piece color as 'white' or "
              "'black'."
           << endl;
    }
  } else {
    cerr << "Invalid input. Please enter a valid move in the format 'e4 white' "
            "or 'd7 black'."
         << endl;
  }

  return move;
}

vector<int> searchForMove(const string &input) {
  Move moveToFind = parseInputToMove(input);
  vector<int> foundGameIds; // List to store game IDs where the move is found

  if (moveToFind.wfile != 0 || moveToFind.bfile != 0) {
    // Iterate over possible game IDs
    for (int gameId = 1; gameId <= 5; ++gameId) {
      string filename = "game" + to_string(gameId) + ".bin";
      ifstream file(filename, ios::binary);
      if (file.is_open()) {
        size_t numMoves;
        file.read(reinterpret_cast<char *>(&numMoves), sizeof(numMoves));
        for (size_t i = 0; i < numMoves; ++i) {
          Move move;
          file.read(reinterpret_cast<char *>(&move), sizeof(move));

          if (input[input.length() - 1] == 'w') {
            // Mask black fields
            move.bfile = 0;
            move.brank = 0;
            move.bpiece = 0;
            move.bcapture = 0;
            cout << move.wfile << move.wrank << move.wpiece << move.wcapture
                 << endl;
            cout << move.bfile << move.brank << move.bpiece << move.bcapture
                 << endl;
          } else if (input[input.length() - 1] == 'b') {
            // Mask white fields
            move.wfile = 0;
            move.wrank = 0;
            move.wpiece = 0;
            move.wcapture = 0;
            cout << move.wfile << move.wrank << move.wpiece << move.wcapture
                 << endl;
            cout << move.bfile << move.brank << move.bpiece << move.bcapture
                 << endl;
          }

          // Compare the masked move with the move to find
          if (memcmp(&move, &moveToFind, sizeof(Move)) == 0) {
            // Move found, add the game ID to the list
            foundGameIds.push_back(gameId);
            break; // No need to continue searching in the same game
          }
        }
        file.close();
      }
    }
  }

  if (foundGameIds.empty()) {
    cout << "Move not found in any game." << endl;
  }

  return foundGameIds;
}

class Game {
private:
  static int nextId;

public:
  vector<Move> moves;
  int id;
  string white;
  string black;
  string time_control;
  string opening;
  // the game constructor takes an FEN string
  // converts it into a game object
  // with a list of Move objects

  Game(string pgn, string white, string black) {
    // vector <Move> moves, string time_control, string opening = parsePGN(pgn);
    // this->opening = opening;
    // this->timecontrol = time_control;
    this->id = nextId++;
    this->white = white;
    this->black = black;
  }

  // Function to save game to a binary file
  string saveToFile() const {
    string filename = "game" + to_string(id) + ".bin";
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
      // Write number of moves
      size_t numMoves = moves.size();
      file.write(reinterpret_cast<const char *>(&numMoves), sizeof(numMoves));

      // Write moves data
      file.write(reinterpret_cast<const char *>(moves.data()),
                 numMoves * sizeof(Move));

      file.close();
      return filename; // Return the filename
    } else {
      cerr << "Unable to open file: " << filename << endl;
      return ""; // Return an empty string on failure
    }
  }

  // Function to load game from a binary file
  void loadFromFile(const string &filename) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
      // Read number of moves
      size_t numMoves;
      file.read(reinterpret_cast<char *>(&numMoves), sizeof(numMoves));

      // Resize moves vector
      moves.resize(numMoves);

      // Read moves data
      file.read(reinterpret_cast<char *>(moves.data()),
                numMoves * sizeof(Move));

      file.close();
    } else {
      cerr << "Unable to open file: " << filename << endl;
    }
  }

  // Function to print moves
  void printMoves() const {
    cout << "Moves:" << endl;
    for (const auto &move : moves) {
      cout << " White \n File: " << move.wfile << ", Rank: " << move.wrank
           << ", Piece: " << move.wpiece << ", Capture: " << move.wcapture
           << endl;
      cout << " Black \n File: " << move.bfile << ", Rank: " << move.brank
           << ", Piece: " << move.bpiece << ", Capture: " << move.bcapture
           << endl;
    }
  }
};

unordered_map<int, string> gameFiles;

class BitBoard {
private:
  unsigned long long board;
  int gameId;
  int moveno;

public:
  // Function to calculate the index based on row and column
  int getIndex(int row, int col) { return col * 8 + row; }

  BitBoard() : board(UINT64_MAX), gameId(-1), moveno(-1) {}

  void addPiece(int row, int col) {
    int index = getIndex(row, col);
    // Set the bit corresponding to the piece
    board &= ~(1ULL << index);
  }

  void removePiece(int row, int col) {
    int index = getIndex(row, col);
    // Clear the bit corresponding to the piece
    board |= (1ULL << index);
  }

  bool isOccupied(int row, int col) {
    int index = getIndex(row, col);
    // Check if the bit at the index is set
    return (board & (1ULL << index)) != 0;
  }

  void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    // Remove piece from original position
    removePiece(fromRow, fromCol);
    // Add piece to the new position
    addPiece(toRow, toCol);
  }

  void drawBoard() {
    for (int row = 7; row >= 0; row--) {
      for (int col = 0; col < 8; col++) {
        int index = getIndex(row, col);
        std::cout << std::bitset<1>((board >> index) & 1).to_string() << " ";
      }
      std::cout << std::endl;
    }
  }
};

std::vector<std::string> splitString(const std::string &input) {
  std::vector<std::string> result;
  std::string word;
  int spaceCount = 0;

  for (char c : input) {
    if (c == ' ') {
      spaceCount++;
      if (spaceCount % 2 == 0) {
        result.push_back(word);
        word.clear();
      } else {
        word += c; // Preserve space if it's the first space
      }
    } else {
      word += c;
    }
  }

  // Add the wlast word if it's not empty
  if (!word.empty()) {
    result.push_back(word);
  }

  return result;
}

void printPGN(const std::vector<std::string> &pgn) {
  // Print the parts
  for (const std::string &move : pgn) {
    std::cout << move << std::endl;
  }
}

int charToIndex(char c) {
  // Subtract the ASCII value of 'a' from the character and convert to integer
  return c - 'a';
}

bool isValidKnightMove(int currentRow, int currentCol, int targetRow,
                       int targetCol) {
  int rowDiff = std::abs(targetRow - currentRow);
  int colDiff = std::abs(targetCol - currentCol);

  return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

std::string removeChar(const std::string &str, char ch) {
  std::string result;
  for (char c : str) {
    if (c != ch) {
      result += c;
    }
  }
  return result;
}

std::string removeFirstTwoChars(string &str) {
  // Check if the string has at least two characters
  if (str.length() >= 2) {
    // Extract substring starting from index 2 (third character)
    return str.substr(2);
  } else {
    // Return the original string if it has less than two characters
    return str;
  }
}

std::vector<std::string> splitString2(string &str) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  while (std::getline(iss, token, ' ')) {
    tokens.push_back(token);
  }
  return tokens;
}
int char_to_int(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else {
    // Handle invalid characters
    std::cerr << "Invalid character: " << c << std::endl;
    return 0; // Or handle error as needed
  }
}

BitBoard makeBitboards(string PGN) {
  vector<BitBoard> gameboards;
  PGN = removeChar(PGN, '+');
  vector<string> parts = splitString(PGN);

  array<int, 2> wlastKing = {-1, -1};
  array<int, 2> wlastQueen = {-1, -1};
  array<int, 2> wlastKnight1 = {-1, -1};
  array<int, 2> wlastKnight2 = {-1, -1};
  array<int, 2> wlastRook1 = {-1, -1};
  array<int, 2> wlastRook2 = {-1, -1};
  array<int, 2> wlastBishopw = {-1, -1};
  array<int, 2> wlastBishopb = {-1, -1};

  // array<int, 2> blastKing={-1,-1};
  // array<int, 2> blastQueen={-1,-1};
  // array<int, 2> blastKnight1={-1,-1};
  // array<int, 2> blastKnight2={-1,-1};
  // array<int, 2> blastRook1={-1,-1};
  // array<int, 2> blastRook2={-1,-1};
  // array<int, 2> blastBishopw={-1,-1};
  // array<int, 2> blastBishopb={-1,-1};
  BitBoard BITBOARD;
  int i =0;
  for (string &move : parts) {
    i++;
    // cout << move << endl;
    move = removeFirstTwoChars(move);
    move = removeChar(move, '.');
    // cout << move <<endl;
    vector<string> moves = splitString2(move);
    move = moves[0];
    move = removeChar(move, ' ');
    int prev_row = 0;
    int prev_column = 0;
    cout << move << endl;

    int row = char_to_int(move[move.length() - 1]) - 1;
    int column = charToIndex(move[move.length() - 2]);
    cout << row << endl << column << endl;
    // cout << move << endl;
    //  moves looks like e4 e5
    switch (move[0]) {
    case 'N':
      cout << move.length();
      if (wlastKnight1[0] == -1 || wlastKnight2[0] == -1) {
        if (row == 2 && column == 2 || row == 2 && column == 0 ||
            row == 1 && column == 3) {
          prev_row = 0;
          prev_column = 1;
          wlastKnight1[0] = row;
          wlastKnight1[1] = column;
        } else if (row == 2 && column == 5 || row == 2 && column == 7) {
          prev_row = 0;
          prev_column = 6;
          wlastKnight2[0] = row;
          wlastKnight2[1] = column;
        }
      } else if (move.length() == 3 || move.find('x') != std::string::npos) {
        cout << "true";
        if (isValidKnightMove(wlastKnight1[0], wlastKnight1[1], row, column)) {
          prev_row = wlastKnight1[0];
          prev_column = wlastKnight1[1];
          wlastKnight1[0] = row;
          wlastKnight1[1] = column;
        } else if (isValidKnightMove(wlastKnight2[0], wlastKnight2[1], row,
                                     column)) {
          cout << "true";
          prev_row = wlastKnight2[0];
          prev_column = wlastKnight2[1];
          wlastKnight2[0] = row;
          wlastKnight2[1] = column;
        }
      } else {
        if (isalpha(move[1])) {
          prev_column = charToIndex(move[1]);
          if (wlastKnight1[1] == prev_column) {
            prev_row = wlastKnight1[0];
            wlastKnight1[0] = row;
            wlastKnight1[1] = column;
          } else {
            prev_row = wlastKnight2[0];
            wlastKnight2[0] = row;
            wlastKnight2[1] = column;
          }
        } else {
          prev_row = move[0];
          if (wlastKnight1[0] == prev_row) {
            prev_column = wlastKnight1[1];
            wlastKnight1[0] = row;
            wlastKnight1[1] = column;
          } else {
            prev_column = wlastKnight2[1];
            wlastKnight2[0] = row;
            wlastKnight2[1] = column;
          }
        }
      }
      break;
    case 'O':
      if (move == "O-O") {
        wlastKing[0] = 0;
        wlastKing[1] = 6;
        prev_row = 0;
        prev_column = 4;
        BITBOARD.movePiece(prev_row, prev_column, 0, 6);
        wlastRook1[0] = 0;
        wlastRook1[1] = 5;
        prev_row = 0;
        prev_column = 0;
        row = 0;
        column = 5;
      } else {
        wlastKing[0] = 0;
        wlastKing[1] = 2;
        prev_row = 0;
        prev_column = 4;
        BITBOARD.movePiece(prev_row, prev_column, 0, 2);
        wlastRook2[0] = 0;
        wlastRook2[1] = 3;
        prev_row = 0;
        prev_column = 7;
        row = 0;
        column = 5;
      }

    case 'B':
      if (row + column % 2 == 0) {
        if (wlastBishopw[0] == -1) {
          prev_row = 0;
          prev_column = 2;
        } else {
          prev_row = wlastBishopw[0];
          prev_column = wlastBishopw[1];
        }
        wlastBishopw[0] = row;
        wlastBishopw[1] = column;
      } else {
        if (wlastBishopb[0] == -1) {
          prev_row = 0;
          prev_column = 5;
        } else {
          prev_row = wlastBishopb[0];
          prev_column = wlastBishopb[1];
        }
        wlastBishopb[0] = row;
        wlastBishopb[1] = column;
      }
      break;
    case 'R':
      if (move.length() == 4) {

        if (isalpha(move[1])) {
          prev_column = charToIndex(move[1]);
          if (wlastRook1[1] == prev_column) {
            wlastRook1[0] = row;
          } else {
            wlastRook1[0] = row;
          }
        } else {
          prev_row = move[1];
          if (wlastRook1[0] == prev_row) {
            wlastRook1[1] = column;
          } else {
            wlastRook1[1] = column;
          }
        }
      }
      if (wlastRook1[0] == -1) {
        prev_row = 0;
        prev_column = 2;
        wlastRook1[0] = row;
        wlastRook1[1] = column;
      }
      if (wlastRook2[0] == -1) {
        prev_row = 0;
        prev_column = 2;
        wlastRook2[0] = row;
        wlastRook2[1] = column;
      }

      break;
    case 'Q':
      if (wlastQueen[0] == -1) {
        prev_row = 0;
        prev_column = 3;
      } else {
        prev_row = wlastQueen[0];
        prev_column = wlastQueen[1];
      }
      wlastQueen[0] = row;
      wlastQueen[1] = column;
      break;
    case 'K':
      if (wlastKing[0] == -1) {
        prev_row = 0;
        prev_column = 4;
      } else {
        prev_row = wlastKing[0];
        prev_column = wlastKing[1];
        wlastKing[0] = row;
        wlastKing[1] = column;
      }
      break;
    default:
      prev_row = row - 1;
      if (move.length() == 4) {
        if (charToIndex(move[0]) - 1 < column) {
          prev_column = column - 1;
        } else {
          prev_column = column + 1;
        }
      } else {
        prev_column = column;
      }
      break;
    }
    cout << prev_row << endl << prev_column << endl;
    BITBOARD.movePiece(prev_row, prev_column, row, column);
    gameboards.push_back(BITBOARD, game_id, i);
    BITBOARD.drawBoard();
  }
  return BITBOARD;
}
int Game::nextId = 1;

void addGame(string PGN){

  Game game2("PGN string", "White Player", "Black Player");

  PGN = removeChar(PGN, '+');
  vector<string> parts = splitString(PGN);
  
  string filename2 = game2.saveToFile();
  makeBitboards(PGN);
  
}

int main() {
  // Example usage
  Game game("PGN string", "White Player", "Black Player");
  // Populate game.moves with some moves...
  game.moves.push_back({4, 2, 1, 0, 4, 4, 1, 0});
  // Save the game to a binary file
  string filename = game.saveToFile();
  game.printMoves();

  // Load the game from the binary file
  Game loadedGame("FEN string", "White Player", "Black Player");
  loadedGame.loadFromFile("game1.bin");

  // Print the loaded moves
  loadedGame.printMoves();

  // if (!filename.empty()) {
  //   // Store the filename in the hash table
  //   gameFiles[game.id] = filename;
  //   cout << "Game added " << filename << endl;
  // } else {
  //   cout << "Failed to save the game." << endl;
  // }
  // // so accessing filepath
  // int gameId = 1;
  // if (gameFiles.find(gameId) != gameFiles.end()) {
  //   cout << "Binary file path for Game ID " << gameId << ": "
  //        << gameFiles[gameId] << endl;
  // } else {
  //   cout << "Game ID " << gameId << " not found in the hash table." << endl;
  // }

  string pgn =
      "1.e4 c6 2.d4 d5 3.exd5 cxd5 4.c3 Nf6 5.Bf4 Nc6 6.Nf3 g6 7.Nbd2 Bg7 "
      "8.Bd3 Bf5 9.Bxf5 gxf5 10.Ne5 Qb6 11.O-O O-O 12.Nb3 e6 13.Qe2 Rfc8 "
      "14.Nc5 Nxe5 15.Bxe5 Ne4 16.Bxg7 Kxg7 17.Nd3 Qc7 18.Qe3 Kh8 19.f3 Nf6 "
      "20.Ne5 Rg8 21.Kh1 Rg7 22.g4 fxg4 23.Qf4 g3 24.Rg1 Qe7 25.Qh4 gxh2 "
      "26.Rxg7 Kxg7 27.Kxh2 Rg8 28.Rg1+ Kh8 29.Rxg8+ Kxg8 30.Qg5+ Kf8 31.Qh6+ "
      "Kg8 32.Qg5+ Kf8 33.Qh6+ Kg8 34.Qg5+";

  BitBoard FINALBITBOARD = makeBitboards(pgn);
  FINALBITBOARD.drawBoard();

  Game game2("PGN string", "White Player", "Black Player");
  game2.moves.push_back({3, 2, 1, 0, 5, 7, 1, 0});
  string filename2 = game2.saveToFile();
  game2.printMoves();
  cout << "Game id:" << game2.id << endl;

  vector<int> foundGameIds = searchForMove("e3w");
  cout << "Games where the move is found:" << endl;
  for (int gameId : foundGameIds) {
    cout << gameId << endl;
  }
}
