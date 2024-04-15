// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <vector>
// using namespace std;

// class Move {
// public:
//   int file : 4;
//   int rank : 4;
//   int piece : 4;
//   int capture : 1;
//   int player : 1;

//   Move(int file, int rank, int piece, int capture, int player)
//       : file(file), rank(rank), piece(piece), capture(capture), player(player) {
//   }
// };

// vector<Move> parseMoves(const string &movesSection) {
//     istringstream iss(movesSection);
//     vector<Move> moves;

//     string moveToken;
//     while (getline(iss, moveToken, ' ')) {
//         // Skip move tokens with scores like "0-0", "1-0", or "1/2-1/2"
//         if (moveToken.find('-') != string::npos) {
//             continue;
//         }

//         Move move;

//         // Determine player (0 for white, 1 for black)
//         if (isdigit(moveToken[0])) {
//             move.player = 0; // White
//         } else {
//             move.player = 1; // Black
//         }

//         // Determine piece
//         if (isalpha(moveToken[0])) {
//             switch (moveToken[0]) {
//                 case 'N':
//                     move.piece = 2; // Knight
//                     break;
//                 case 'B':
//                     move.piece = 3; // Bishop
//                     break;
//                 case 'R':
//                     move.piece = 4; // Rook
//                     break;
//                 case 'Q':
//                     move.piece = 5; // Queen
//                     break;
//                 case 'K':
//                     move.piece = 6; // King
//                     break;
//                 default:
                   
//                     move.piece = 1; 
//                     break;
//             }
//         }

//         // Determine file and rank
//         if (moveToken.length() >= 3) {
//             move.file = moveToken[moveToken.length() - 2] - 'a' + 1;
//             move.rank = moveToken[moveToken.length() - 1] - '0';
//         }

//         // Determine capture
//         if (moveToken.find('x') != string::npos) {
//             move.capture = 1; // Capture
//         } else {
//             move.capture = 0; // No capture
//         }

//         // Add the move to the moves vector
//         moves.push_back(move);
//     }

//     return moves;
// }



// vector<vector<Move>> parsePGN(const string &pgnFileName) {
//   ifstream pgnFile(pgnFileName);
//   if (!pgnFile.is_open()) {
//     cerr << "Error: Unable to open PGN file." << endl;
//     exit(EXIT_FAILURE);
//   }

//   vector<vector<Move>> allGamesMoves;
//   string line;
//   vector<Move> currentGameMoves;

//   while (getline(pgnFile, line)) {
//     if (isdigit(line[0])) {
//       stringstream movesSection;
//       movesSection << line;
//       while (getline(pgnFile, line) && !line.empty()) {
//         movesSection << " " << line;
//       }
//       cout << movesSection.str() << endl;
//       vector<Move> moves = parseMoves(movesSection.str());
//       allGamesMoves.push_back(moves);
//     }
//   }

//   pgnFile.close();

//   return allGamesMoves;
// }

// int main() {
//   string pgnFileName = "chess.pgn";
//   vector<vector<Move>> allGamesMoves = parsePGN(pgnFileName);

//   return 0;
// }
