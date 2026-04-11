
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>
using namespace std;

// Константи кольорів
const int WHITE = 7;
const int GREEN = 10;
const int RED = 12;
const int YELLOW = 14;
const int BLACK = 0;

// КОЛЬОРИ
void setColor(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor + backgroundColor * 16);
}

void resetColor() {
    setColor(WHITE, BLACK);
}


//ТАЙМЕР

time_t gameStartTime;

string getElapsedTime()
{
    time_t now = time(0);
    double elapsed = difftime(now, gameStartTime);
    int minutes = (int)elapsed / 60;
    int seconds = (int)elapsed % 60;

    string result = "";
    if (minutes > 0) {
        result += to_string(minutes) + " min ";
    }
    result += to_string(seconds) + " sec";
    return result;
}

// КОНСТАНТИ
const int BOARD_SIZE = 10;
const char EMPTY = '.';
const char SHIP = 'S';
const char HIT = 'X';
const char MISS = 'O';

// ФУНКЦІЇ
void printBoard(char board[BOARD_SIZE][BOARD_SIZE], bool hideShips)
{
    cout << "  ";
    for (int col = 0; col < BOARD_SIZE; col++) {
        cout << col << " ";
    }
    cout << endl;
    for (int row = 0; row < BOARD_SIZE; row++) {
        cout << row << " ";
        for (int col = 0; col < BOARD_SIZE; col++) {
            char cell = board[row][col];

            if (hideShips && cell == SHIP) {
               
                cout << EMPTY;
                resetColor();
                cout << " ";
            }
            else if (cell == HIT) {
                setColor(RED, BLACK); 
                cout << HIT;
                resetColor();
                cout << " ";
            }
            else if (cell == SHIP) {
                setColor(GREEN, BLACK);
                cout << SHIP;
                resetColor();
                cout << " ";
            }
            else {
                cout << cell << " ";
            }
        }
        cout << endl;
    }
}

void initBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            board[row][col] = EMPTY;
}

bool canPlaceShip(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int length, bool horiz)
{
    for (int i = 0; i < length; i++) {
        int r = row + (horiz ? 0 : i);
        int c = col + (horiz ? i : 0);
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return false;
        for (int dr = -1; dr <= 1; dr++)
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr, nc = c + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE)
                    if (board[nr][nc] == SHIP) return false;
            }
    }
    return true;
}

void placeShip(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int length, bool horiz)
{
    for (int i = 0; i < length; i++) {
        int r = row + (horiz ? 0 : i);
        int c = col + (horiz ? i : 0);
        board[r][c] = SHIP;
    }
}

void placeShipsRandom(char board[BOARD_SIZE][BOARD_SIZE])
{
    int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int numShips = 10;
    for (int i = 0; i < numShips; i++) {
        int length = ships[i];
        while (true) {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            bool horiz = rand() % 2;
            if (canPlaceShip(board, row, col, length, horiz)) {
                placeShip(board, row, col, length, horiz);
                break;
            }
        }
    }
}

void placeShipsManual(char board[BOARD_SIZE][BOARD_SIZE])
{
    int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int numShips = 10;
    string directions[] = {
        "four-deck", "three-deck", "three-deck",
        "two-deck",  "two-deck",   "two-deck",
        "single-deck","single-deck","single-deck","single-deck"
    };

    for (int i = 0; i < numShips; i++) {
        int length = ships[i];
        while (true) {
            cout << "\nYour field:\n";
            printBoard(board, false);

            setColor(YELLOW, BLACK);
            cout << "\nPlace " << directions[i] << " ship (length " << length << ").\n";
            resetColor();

            cout << "Enter row (0-9): ";
            int row; cin >> row;
            cout << "Enter column (0-9): ";
            int col; cin >> col;
            cout << "Horizontally? (1=yes, 0=no): ";
            int h; cin >> h;

            if (canPlaceShip(board, row, col, length, h)) {
                placeShip(board, row, col, length, h);
                break;
            }
            else {
                
                setColor(RED, BLACK);
                cout << "Can't place here! Try again.\n";
                resetColor();
            }
        }
    }
}

bool shoot(char board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        return true;
    }
    else {
        board[row][col] = MISS;
        return false;
    }
}

bool allShipsSunk(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            if (board[row][col] == SHIP) return false;
    return true;
}

void computerShoot(char board[BOARD_SIZE][BOARD_SIZE])
{
    while (true) {
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;
        if (board[row][col] != HIT && board[row][col] != MISS) {
            cout << "Computer shoots: " << row << " " << col << "\n";
            if (shoot(board, row, col)) {
                setColor(RED, BLACK);
                cout << "Computer hit!\n";
                resetColor();
            }
            else {
                cout << "Computer missed.\n";
            }
            break;
        }
    }
}
// MAIN
int main()
{
    srand((unsigned int)time(0));

    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    char computerBoard[BOARD_SIZE][BOARD_SIZE];
    initBoard(playerBoard);
    initBoard(computerBoard);

    setColor(YELLOW, BLACK);
    cout << "----- BATTLESHIP -----\n\n";
    resetColor();

    cout << "Choose ship placement:\n";
    cout << "1 - Manually\n";
    cout << "2 - Random\n";
    cout << "Your choice: ";
    int choice; cin >> choice;

    if (choice == 1) {
        placeShipsManual(playerBoard);
    }
    else {
        placeShipsRandom(playerBoard);
        setColor(GREEN, BLACK);
        cout << "Ships placed randomly.\n";
        resetColor();
    }
    placeShipsRandom(computerBoard);
    gameStartTime = time(0);

    setColor(YELLOW, BLACK);
    cout << "\nThe game has begun! Good luck!\n";
    resetColor();
    while (true) {
        setColor(YELLOW, BLACK);
        cout << "\n===== YOUR TURN =====\n";
        resetColor();

        cout << "Time: ";
        setColor(YELLOW, BLACK);
        cout << getElapsedTime();
        resetColor();
        cout << "\n";

        cout << "\nEnemy field:\n";
        printBoard(computerBoard, true);
        cout << "\nYour field:\n";
        printBoard(playerBoard, false);

        int row, col;
        while (true) {
            cout << "\nEnter row for the shot (0-9): ";    cin >> row;
            cout << "Enter column for the shot (0-9): ";  cin >> col;
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                setColor(RED, BLACK);
                cout << "Out of range! Try again.\n";
                resetColor();
            }
            else if (computerBoard[row][col] == HIT || computerBoard[row][col] == MISS) {
                setColor(RED, BLACK);
                cout << "Already shot here! Choose another cell.\n";
                resetColor();
            }
            else {
                break;
            }
        }
        if (shoot(computerBoard, row, col)) {
            setColor(RED, BLACK);
            cout << "HIT!\n";
            resetColor();
        }
        else {
            cout << "Miss!\n";
        }

        if (allShipsSunk(computerBoard)) {
            cout << "\nEnemy field:\n";
            printBoard(computerBoard, false);
            setColor(GREEN, BLACK);
            cout << "\nCONGRATULATIONS! YOU WIN!\n";
            resetColor();
            cout << "Your time: " << getElapsedTime() << "\n";
            break;
        }

        setColor(YELLOW, BLACK);
        cout << "\n---- COMPUTER'S TURN -----\n";
        resetColor();
        computerShoot(playerBoard);

        if (allShipsSunk(playerBoard)) {
            cout << "\nYour field:\n";
            printBoard(playerBoard, false);
            setColor(RED, BLACK);
            cout << "\nComputer won! All your ships are sunk.\n";
            resetColor();
            cout << "Game lasted: " << getElapsedTime() << "\n";
            break;
        }
    }

    cout << "\nThanks for playing!\n";
    return 0;
}