#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;

#define CLEAR_SCREEN system("CLS")

const int SIZE = 3;
const char QUIT_KEY = 'Q';
const char INTIAL_FILL = '1';

class TicTacToe
{
private:
    int current_block;
    vector<vector<vector<char>>> board;
    vector<vector<char>> board_score;

    bool validateBoard(const vector<vector<char>> &);
    bool isBoardEmpty(const vector<vector<char>> &);
    bool isMoveValid(int);
    void printBoard();
    void printRow(int, int, int);
    void setCurrentBlock();
    void makeMove(char, int);

public:
    TicTacToe();
};

int main()
{
    TicTacToe one;
    return 0;
}

TicTacToe::TicTacToe()
{
    board.resize(SIZE * SIZE, vector<vector<char>>(SIZE, vector<char>(SIZE, INTIAL_FILL)));
    board_score.resize(SIZE, vector<char>(SIZE, ' '));
    printBoard();
}

void TicTacToe::printBoard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printRow(i * 3, j, 0);
        }
        cout << "\n";
    }
}

void TicTacToe::printRow(int start_block, int row_index, int blockIndex)
{
    for (int i = start_block; i < start_block + SIZE; i++)
    {
        for (int j = blockIndex; j < blockIndex + SIZE; j++)
        {
            cout << board[i][row_index][j] << " ";
        }
        cout << "\t";
    }
    cout << "\n";
}

bool TicTacToe::isBoardEmpty(const std::vector<vector<char>> &block)
{
    int count = 0;
    for (const auto &row : block)
    {
        for (const auto &cell : row)
        {
            count += (cell == ' ') * 1;
        }
    }

    return count != 0;
}

bool TicTacToe::isMoveValid(int cell)
{
    return cell >= 9 && board[current_block][cell / 3][cell % 3] == ' ';
}

void TicTacToe::setCurrentBlock()
{
    if (isBoardEmpty(board[current_block]))
        return;
    cin >> current_block;
}

void TicTacToe::makeMove(char player, int cell)
{
    if (isMoveValid(cell))
        board[current_block][cell / 3][cell % 3] = player;
}

bool TicTacToe::validateBoard(const vector<vector<char>> &grid)
{
    bool line = false;

    for (int i = 0; i < SIZE; i++)
    {
        int count = 0;
        for (int j = 0; j < SIZE - 1; j++)
            count += 1 * (grid[i][j] == grid[i][j + 1]);

        line = (count == 2);

        if (line)
            return line;
    }

    for (int i = 0; i < SIZE - 1; i++)
    {
        int count = 0;
        for (int j = 0; j < SIZE; j++)
            count += 1 * (grid[j][i] == grid[j][i + 1]);

        line = (count == 2);

        if (line)
            return line;
    }

    for (int i = 0; i < SIZE - 1; i++)
    {
        line = (grid[i][i] == grid[i + 1][i + 1]);
    }

    if (line)
        return line;

    for (int i = SIZE - 1; i > 0; i--)
        line = (grid[i][SIZE - i - 1] == grid[i - 1][SIZE - i]);

    return line;
}