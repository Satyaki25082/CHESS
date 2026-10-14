#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;

const int INF = 1000000;

struct Move {
    int fromX, fromY;
    int toX, toY;
    char captured;
};

class Chess {
private:
    char board[8][8];
    bool whiteTurn;

public:
    Chess() {
        initializeBoard();
        whiteTurn = true;
    }

    void initializeBoard() {
        char initial[8][8] = {
            {'r','n','b','q','k','b','n','r'},
            {'p','p','p','p','p','p','p','p'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'P','P','P','P','P','P','P','P'},
            {'R','N','B','Q','K','B','N','R'}
        };

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                board[i][j] = initial[i][j];
    }

    void printBoard() {
        cout << "\n   0 1 2 3 4 5 6 7\n";

        for(int i = 0; i < 8; i++) {
            cout << i << "  ";
            for(int j = 0; j < 8; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool isWhite(char p) {
        return p >= 'A' && p <= 'Z';
    }

    bool isBlack(char p) {
        return p >= 'a' && p <= 'z';
    }

    bool inBounds(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    int pieceValue(char p) {
        switch(tolower(p)) {
            case 'p': return 100;
            case 'n': return 320;
            case 'b': return 330;
            case 'r': return 500;
            case 'q': return 900;
            case 'k': return 20000;
        }
        return 0;
    }

    int evaluateBoard() {
        int score = 0;

        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                char p = board[i][j];

                if(p == '.') continue;

                if(isWhite(p))
                    score += pieceValue(p);
                else
                    score -= pieceValue(p);
            }
        }

        return score;
    }

    vector<Move> generateMoves(bool white) {
        vector<Move> moves;

        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 8; y++) {
                char piece = board[x][y];

                if(piece == '.') continue;

                if(white && !isWhite(piece)) continue;
                if(!white && !isBlack(piece)) continue;

                switch(tolower(piece)) {
                    case 'p': generatePawnMoves(x, y, moves); break;
                    case 'r': generateRookMoves(x, y, moves); break;
                    case 'n': generateKnightMoves(x, y, moves); break;
                    case 'b': generateBishopMoves(x, y, moves); break;
                    case 'q': generateQueenMoves(x, y, moves); break;
                    case 'k': generateKingMoves(x, y, moves); break;
                }
            }
        }

        return moves;
    }

    void generatePawnMoves(int x, int y, vector<Move>& moves) {
        char p = board[x][y];
        int dir = isWhite(p) ? -1 : 1;

        int nx = x + dir;

        if(inBounds(nx, y) && board[nx][y] == '.') {
            moves.push_back({x,y,nx,y,'.'});
        }

        if(inBounds(nx, y-1)) {
            char target = board[nx][y-1];
            if(target != '.' && isWhite(p) != isWhite(target)) {
                moves.push_back({x,y,nx,y-1,target});
            }
        }

        if(inBounds(nx, y+1)) {
            char target = board[nx][y+1];
            if(target != '.' && isWhite(p) != isWhite(target)) {
                moves.push_back({x,y,nx,y+1,target});
            }
        }
    }

    void generateRookMoves(int x, int y, vector<Move>& moves) {
        int dx[] = {-1,1,0,0};
        int dy[] = {0,0,-1,1};

        for(int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            while(inBounds(nx, ny)) {
                char target = board[nx][ny];

                if(target == '.') {
                    moves.push_back({x,y,nx,ny,'.'});
                }
                else {
                    if(isWhite(board[x][y]) != isWhite(target))
                        moves.push_back({x,y,nx,ny,target});
                    break;
                }

                nx += dx[d];
                ny += dy[d];
            }
        }
    }

    void generateBishopMoves(int x, int y, vector<Move>& moves) {
        int dx[] = {-1,-1,1,1};
        int dy[] = {-1,1,-1,1};

        for(int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            while(inBounds(nx, ny)) {
                char target = board[nx][ny];

                if(target == '.') {
                    moves.push_back({x,y,nx,ny,'.'});
                }
                else {
                    if(isWhite(board[x][y]) != isWhite(target))
                        moves.push_back({x,y,nx,ny,target});
                    break;
                }

                nx += dx[d];
                ny += dy[d];
            }
        }
    }

    void generateKnightMoves(int x, int y, vector<Move>& moves) {
        int dx[] = {-2,-2,-1,-1,1,1,2,2};
        int dy[] = {-1,1,-2,2,-2,2,-1,1};

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(!inBounds(nx, ny)) continue;

            char target = board[nx][ny];

            if(target == '.' || isWhite(board[x][y]) != isWhite(target)) {
                moves.push_back({x,y,nx,ny,target});
            }
        }
    }

    void generateQueenMoves(int x, int y, vector<Move>& moves) {
        generateRookMoves(x, y, moves);
        generateBishopMoves(x, y, moves);
    }

    void generateKingMoves(int x, int y, vector<Move>& moves) {
        for(int dx = -1; dx <= 1; dx++) {
            for(int dy = -1; dy <= 1; dy++) {
                if(dx == 0 && dy == 0) continue;

                int nx = x + dx;
                int ny = y + dy;

                if(!inBounds(nx, ny)) continue;

                char target = board[nx][ny];

                if(target == '.' || isWhite(board[x][y]) != isWhite(target)) {
                    moves.push_back({x,y,nx,ny,target});
                }
            }
        }
    }

    void makeMove(Move m) {
        board[m.toX][m.toY] = board[m.fromX][m.fromY];
        board[m.fromX][m.fromY] = '.';
    }

    void undoMove(Move m) {
        board[m.fromX][m.fromY] = board[m.toX][m.toY];
        board[m.toX][m.toY] = m.captured;
    }

    int minimax(int depth, bool maximizingPlayer, int alpha, int beta) {
        if(depth == 0)
            return evaluateBoard();

        vector<Move> moves = generateMoves(maximizingPlayer);

        if(moves.empty())
            return evaluateBoard();

        if(maximizingPlayer) {
            int best = -INF;

            for(auto move : moves) {
                makeMove(move);
                int value = minimax(depth - 1, false, alpha, beta);
                undoMove(move);

                best = max(best, value);
                alpha = max(alpha, best);

                if(beta <= alpha)
                    break;
            }

            return best;
        }
        else {
            int best = INF;

            for(auto move : moves) {
                makeMove(move);
                int value = minimax(depth - 1, true, alpha, beta);
                undoMove(move);

                best = min(best, value);
                beta = min(beta, best);

                if(beta <= alpha)
                    break;
            }

            return best;
        }
    }

    Move findBestMove(int depth) {
        vector<Move> moves = generateMoves(false);

        int bestValue = INF;
        Move bestMove = moves[0];

        for(auto move : moves) {
            makeMove(move);

            int value = minimax(depth - 1, true, -INF, INF);

            undoMove(move);

            if(value < bestValue) {
                bestValue = value;
                bestMove = move;
            }
        }

        return bestMove;
    }

    bool playerMove(int fx, int fy, int tx, int ty) {
        vector<Move> moves = generateMoves(true);

        for(auto move : moves) {
            if(move.fromX == fx && move.fromY == fy &&
               move.toX == tx && move.toY == ty) {
                makeMove(move);
                return true;
            }
        }

        return false;
    }

    void aiMove() {
        cout << "AI is thinking...\n";

        Move best = findBestMove(3);

        makeMove(best);

        cout << "AI moved from ("
             << best.fromX << ","
             << best.fromY << ") to ("
             << best.toX << ","
             << best.toY << ")\n";
    }

    bool hasKing(bool white) {
        char king = white ? 'K' : 'k';

        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                if(board[i][j] == king)
                    return true;
            }
        }

        return false;
    }

    void gameLoop() {
        while(true) {
            printBoard();

            if(!hasKing(true)) {
                cout << "Black Wins!\n";
                break;
            }

            if(!hasKing(false)) {
                cout << "White Wins!\n";
                break;
            }

            int fx, fy, tx, ty;

            cout << "Your move\n";
            cout << "From X Y: ";
            cin >> fx >> fy;

            cout << "To X Y: ";
            cin >> tx >> ty;

            if(!playerMove(fx, fy, tx, ty)) {
                cout << "Invalid move!\n";
                continue;
            }

            printBoard();

            if(!hasKing(false)) {
                cout << "White Wins!\n";
                break;
            }

            aiMove();
        }
    }
};



int main() {
    Chess game;

    cout << "========================\n";
    cout << "  Satyaki's Chess Board     \n";
    cout << "========================\n";

    cout << "\nBoard Coordinates Example:\n";
    cout << "Top Left = (0,0)\n";
    cout << "Bottom Right = (7,7)\n";

    game.gameLoop();

    return 0;
}
