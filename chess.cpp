#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

const int BOARD_SIZE = 8;

struct CastlingRights {
    bool wk = false, bk = false;
    bool wrA = false, wrH = false;
    bool brA = false, brH = false;
};

void printBoard(const vector<vector<char>>& board) {
    cout << "\n  a b c d e f g h\n";
    for (int row = 0; row < BOARD_SIZE; ++row) {
        cout << 8 - row << " ";
        for (int col = 0; col < BOARD_SIZE; ++col) {
            cout << board[row][col] << " ";
        }
        cout << 8 - row << "\n";
    }
    cout << "  a b c d e f g h\n\n";
}

bool isPathClear(const vector<vector<char>>& board, int r1, int c1, int r2, int c2) {
    int dr = (r2 > r1) ? 1 : ((r2 < r1) ? -1 : 0);
    int dc = (c2 > c1) ? 1 : ((c2 < c1) ? -1 : 0);
    
    int r = r1 + dr;
    int c = c1 + dc;
    
    while (r != r2 || c != c2) {
        if (board[r][c] != '.') return false;
        r += dr;
        c += dc;
    }
    return true;
}

bool isValidPieceMove(char piece, int r1, int c1, int r2, int c2, bool isCapture, const vector<vector<char>>& board) {
    char p = tolower(piece);
    int dr = r2 - r1;
    int dc = c2 - c1;
    int abs_dr = abs(dr);
    int abs_dc = abs(dc);

    if (p == 'r') {
        return (dr == 0 || dc == 0) && isPathClear(board, r1, c1, r2, c2);
    } else if (p == 'b') {
        return (abs_dr == abs_dc) && isPathClear(board, r1, c1, r2, c2);
    } else if (p == 'q') {
        return (dr == 0 || dc == 0 || abs_dr == abs_dc) && isPathClear(board, r1, c1, r2, c2);
    } else if (p == 'n') {
        return (abs_dr == 2 && abs_dc == 1) || (abs_dr == 1 && abs_dc == 2);
    } else if (p == 'k') {
        return abs_dr <= 1 && abs_dc <= 1;
    } else if (p == 'p') {
        int dir = isupper(piece) ? -1 : 1;
        int startRow = isupper(piece) ? 6 : 1;
        
        if (!isCapture && dc == 0) {
            if (dr == dir) return true;
            if (r1 == startRow && dr == 2 * dir && board[r1 + dir][c1] == '.') return true;
        } else if (isCapture && abs_dc == 1 && dr == dir) {
            return true;
        }
        return false;
    }
    return false;
}

bool isSquareAttacked(int targetRow, int targetCol, bool byWhite, const vector<vector<char>>& board) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            char piece = board[r][c];
            if (piece == '.') continue;
            
            bool isWhitePiece = isupper(piece);
            if (isWhitePiece == byWhite) {
                if (isValidPieceMove(piece, r, c, targetRow, targetCol, true, board)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isKingInCheck(bool whiteKing, const vector<vector<char>>& board) {
    char targetKing = whiteKing ? 'K' : 'k';
    int kr = -1, kc = -1;
    
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == targetKing) {
                kr = r;
                kc = c;
                break;
            }
        }
        if (kr != -1) break;
    }
    
    return isSquareAttacked(kr, kc, !whiteKing, board);
}

bool makeMove(vector<vector<char>>& board, string move, bool& isWhiteTurn, CastlingRights& cr) {
    if (move.length() != 4) return false;

    int fromCol = move[0] - 'a';
    int fromRow = 8 - (move[1] - '0');
    int toCol = move[2] - 'a';
    int toRow = 8 - (move[3] - '0');

    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }

    char piece = board[fromRow][fromCol];
    if (piece == '.') return false;

    if (isWhiteTurn && islower(piece)) return false;
    if (!isWhiteTurn && isupper(piece)) return false;

    char targetPiece = board[toRow][toCol];
    bool isCapture = (targetPiece != '.');

    if (isCapture) {
        if (isWhiteTurn && isupper(targetPiece)) return false;
        if (!isWhiteTurn && islower(targetPiece)) return false;
    }

    bool isCastling = false;
    if (tolower(piece) == 'k' && abs(toCol - fromCol) == 2) {
        if (fromRow != toRow) return false;
        if (isWhiteTurn ? cr.wk : cr.bk) return false;
        
        if (toCol == 6) {
            if (isWhiteTurn ? cr.wrH : cr.brH) return false;
            if (!isPathClear(board, fromRow, 4, fromRow, 7)) return false;
            if (isSquareAttacked(fromRow, 4, !isWhiteTurn, board) || 
                isSquareAttacked(fromRow, 5, !isWhiteTurn, board) || 
                isSquareAttacked(fromRow, 6, !isWhiteTurn, board)) return false;
        } else if (toCol == 2) {
            if (isWhiteTurn ? cr.wrA : cr.brA) return false;
            if (!isPathClear(board, fromRow, 4, fromRow, 0)) return false;
            if (isSquareAttacked(fromRow, 4, !isWhiteTurn, board) || 
                isSquareAttacked(fromRow, 3, !isWhiteTurn, board) || 
                isSquareAttacked(fromRow, 2, !isWhiteTurn, board)) return false;
        } else {
            return false;
        }
        isCastling = true;
    } else if (!isValidPieceMove(piece, fromRow, fromCol, toRow, toCol, isCapture, board)) {
        return false;
    }

    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = '.';

    if (isCastling) {
        if (toCol == 6) {
            board[toRow][5] = board[toRow][7];
            board[toRow][7] = '.';
        } else {
            board[toRow][3] = board[toRow][0];
            board[toRow][0] = '.';
        }
    }

    if (isKingInCheck(isWhiteTurn, board)) {
        board[fromRow][fromCol] = piece;
        board[toRow][toCol] = targetPiece;
        if (isCastling) {
            if (toCol == 6) {
                board[toRow][7] = board[toRow][5];
                board[toRow][5] = '.';
            } else {
                board[toRow][0] = board[toRow][3];
                board[toRow][3] = '.';
            }
        }
        return false;
    }

    if (piece == 'K') cr.wk = true;
    if (piece == 'k') cr.bk = true;
    if (piece == 'R' && fromCol == 0) cr.wrA = true;
    if (piece == 'R' && fromCol == 7) cr.wrH = true;
    if (piece == 'r' && fromCol == 0) cr.brA = true;
    if (piece == 'r' && fromCol == 7) cr.brH = true;

    if (tolower(piece) == 'p' && (toRow == 0 || toRow == 7)) {
        board[toRow][toCol] = isWhiteTurn ? 'Q' : 'q';
    }

    isWhiteTurn = !isWhiteTurn;
    return true;
}

int main() {
    vector<vector<char>> board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    bool isWhiteTurn = true;
    CastlingRights cr;
    string move;

    cout << "Welcome to SilichChess!\n";
    printBoard(board);

    while (true) {
        if (isKingInCheck(isWhiteTurn, board)) {
            cout << "CHECK! ";
        }
        cout << (isWhiteTurn ? "White's turn: " : "Black's turn: ");
        cin >> move;

        if (move == "q") {
            cout << "Game over!\n";
            break;
        }

        if (makeMove(board, move, isWhiteTurn, cr)) {
            printBoard(board);
        } else {
            cout << "Invalid move! Try again.\n";
        }
    }

    return 0;
}