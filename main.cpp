#include <iostream>
#include <array>
#include <conio.h>

#define CLEAR_SCREEN system("CLS");

class TicTacToe
{
private:
    static const int SIZE = 3;
    static const int BLOCK_END = 8;
    static const int BLOCK_START = 0;

    static const char DRAW = 'D';
    static const char QUIT_KEY = 'Q';
    static const char INTIAL_FILL = ' ';

    static const char JUNCTION;
    static const char STRAIGHT_LINE;
    static const char SLEEPING_LINE;
    static const char COLUMN_SEPERATOR;

    typedef std::array<std::array<char, SIZE>, SIZE> singleBoard;

    int currentCellToPlay;
    bool isGameDone;
    char currentPlayer;
    int currentBlockToPlay;
    int blocksCompletelyfilled;
    std::string player1Name, player2Name;

    singleBoard blockWinner;
    std::array<std::array<int, SIZE>, SIZE> frequencyCount;
    std::array<std::array<singleBoard, SIZE>, SIZE> gameBoard;

    void moveInput();
    void printTable();
    void intializeFullBoard();
    void intializeSingleBoard(singleBoard &);
    void drawLine(const char &, const char &);

    bool isBoardSolved(const singleBoard &);

    inline bool validateCurrentCellToPlay();
    inline bool validateCurrentBlockToPlay();

public:
    TicTacToe(const std::string &, const std::string &);
    void startGame();
};

int main()
{
    TicTacToe one("a", "b");
}

const char TicTacToe::JUNCTION = 197;
const char TicTacToe::STRAIGHT_LINE = 179;
const char TicTacToe::SLEEPING_LINE = 196;
const char TicTacToe::COLUMN_SEPERATOR = ' ';

TicTacToe::TicTacToe(const std::string &player1Name, const std::string &player2Name)
{
    this->isGameDone = false;
    this->currentPlayer = 'X';
    this->currentBlockToPlay = -1;
    this->currentCellToPlay = -1;
    this->blocksCompletelyfilled = 0;
    this->player1Name = player1Name;
    this->player2Name = player2Name;

    intializeFullBoard();
    startGame();
}

void TicTacToe::startGame()
{
    while (!isGameDone)
    {
        moveInput();

        CLEAR_SCREEN;
        printTable();

        gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE][currentCellToPlay / SIZE][currentCellToPlay % SIZE] = currentPlayer;
        frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE]++;

        if (isBoardSolved(gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE]))
        {
            blockWinner[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] = currentPlayer;
        }

        if (isBoardSolved(blockWinner))
        {
            isGameDone = true;
            continue;
        }
        else if (frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] == SIZE * SIZE)
        {
            if (++blocksCompletelyfilled == SIZE * SIZE)
            {
                isGameDone = true;
                currentPlayer = DRAW;
                continue;
            }
            blockWinner[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] = DRAW;
        }

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        currentBlockToPlay = currentCellToPlay;
        currentCellToPlay = -1;
    }
}

void TicTacToe::moveInput()
{
    bool is_block_valid = validateCurrentBlockToPlay();
    bool is_cell_valid = validateCurrentCellToPlay();

    while (!is_block_valid)
    {
        CLEAR_SCREEN;
        printTable();

        std::cout << "Enter current to paly - ";
        std::cin >> currentBlockToPlay;
        currentBlockToPlay--;

        is_block_valid = (currentBlockToPlay >= BLOCK_START && currentBlockToPlay <= BLOCK_END) && validateCurrentBlockToPlay();

        std::cout << "Invalid block number\n";
    }

    while (!is_cell_valid)
    {
        CLEAR_SCREEN;
        printTable();
        std::cout << "Enter cell to play in block " << currentBlockToPlay << " - ";
        std::cin >> currentCellToPlay;
        currentCellToPlay--;

        is_cell_valid = (currentCellToPlay >= BLOCK_START && currentCellToPlay <= BLOCK_END) && validateCurrentCellToPlay();
        std::cout << "Invalid block number\n";
    }
}

inline bool TicTacToe::validateCurrentBlockToPlay()
{
    return (currentBlockToPlay != -1 && frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] < SIZE * SIZE);
}

inline bool TicTacToe::validateCurrentCellToPlay()
{
    return (currentCellToPlay != -1 && gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE][currentCellToPlay / SIZE][currentCellToPlay % SIZE] == INTIAL_FILL);
}

bool TicTacToe::isBoardSolved(const singleBoard &currentBoard)
{
    static auto check_row = [&]()
    {
        bool is_solved = false;
        int row_count = 0;

        for (int row = 0; row < SIZE; row++)
        {
            row_count = 0;
            for (int col = 0; col < SIZE; col++)
            {
                row_count = row_count * (currentBoard[row][col] == currentPlayer) + 1;
            }

            is_solved |= (row_count == SIZE);
        }

        return is_solved;
    };

    static auto check_column = [&]()
    {
        bool is_solved = false;
        int column_count = 0;
        for (int col = 0; col < SIZE; col++)
        {
            column_count = 0;
            for (int row = 0; row < SIZE; row++)
            {
                column_count = column_count * (currentBoard[row][col] == currentPlayer) + 1;
            }

            is_solved |= (column_count == SIZE);
        }

        return is_solved;
    };

    static auto check_left_diagonal = [&]()
    {
        int left_diagonal_count = 0;

        for (int row = 0; row < SIZE; row++)
        {
            left_diagonal_count = left_diagonal_count * (currentBoard[row][row] == currentPlayer) + 1;
        }

        return (left_diagonal_count == SIZE);
    };

    static auto check_right_diagonal = [&]()
    {
        int right_diagonal_count = 0;

        for (int row = 0; row < SIZE; row++)
        {
            right_diagonal_count = right_diagonal_count * (currentBoard[row][SIZE - row - 1] == currentPlayer) + 1;
        }

        return (right_diagonal_count == SIZE);
    };

    return check_row() || check_column() || check_left_diagonal() || check_right_diagonal();
}

void TicTacToe::intializeFullBoard()
{
    static auto intializeSingleBoard = [](singleBoard &currentBoard)
    {
        for (std::array<char, SIZE> &row : currentBoard)
        {
            for (char &block : row)
            {
                block = INTIAL_FILL;
            }
        }
    };

    for (std::array<singleBoard, SIZE> &row : gameBoard)
    {
        for (singleBoard &singleGame : row)
        {
            intializeSingleBoard(singleGame);
        }
    }
}

void TicTacToe::drawLine(const char &lineChar, const char &columnSeperator)
{
    for (int sperations = 0; sperations < SIZE; sperations++)
    {
        for (int print_char = 0; print_char < SIZE * 2 + 2; print_char++)
        {
            std::cout << lineChar;
        }

        if (sperations != SIZE - 1)
            std::cout << columnSeperator;
    }

    std::cout << std::endl;
}

void TicTacToe::printTable()
{
    drawLine(TicTacToe::COLUMN_SEPERATOR, TicTacToe::STRAIGHT_LINE);

    for (int block_row = 0; block_row < SIZE; block_row++)
    {
        for (int row = 0; row < SIZE; row++)
        {
            std::cout << COLUMN_SEPERATOR;

            for (int first_col = 0; first_col < SIZE; first_col++)
            {
                std::cout << gameBoard[block_row][0][row][first_col] << (first_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPERATOR);
            }

            std::cout << COLUMN_SEPERATOR << STRAIGHT_LINE << COLUMN_SEPERATOR;

            for (int second_col = 0; second_col < SIZE; second_col++)
            {
                std::cout << gameBoard[block_row][1][row][second_col] << (second_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPERATOR);
            }

            std::cout << COLUMN_SEPERATOR << STRAIGHT_LINE << COLUMN_SEPERATOR;

            for (int third_col = 0; third_col < SIZE; third_col++)
            {
                std::cout << gameBoard[block_row][1][row][third_col] << (third_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPERATOR);
            }

            std::cout << std::endl;
            if (row != SIZE - 1)
            {

                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPERATOR << COLUMN_SEPERATOR << STRAIGHT_LINE;
                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPERATOR << COLUMN_SEPERATOR << STRAIGHT_LINE;
                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPERATOR << COLUMN_SEPERATOR;
                std::cout << std::endl;
            }
        }

        drawLine(TicTacToe::COLUMN_SEPERATOR, TicTacToe::STRAIGHT_LINE);

        if (block_row < SIZE - 1)
        {
            drawLine(TicTacToe::SLEEPING_LINE, TicTacToe::JUNCTION);
        }

        drawLine(TicTacToe::COLUMN_SEPERATOR, TicTacToe::STRAIGHT_LINE);
    }
}