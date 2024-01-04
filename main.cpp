#include <iostream>
#include <array>

#define CLEAR_SCREEN system("CLS");

class SuperTicTacToe
{
private:
    // Integer constants
    static constexpr int SIZE = 3;
    static constexpr int BLOCK_END = 8;
    static constexpr int QUIT_KEY = 0;
    static constexpr int BLOCK_START = 0;
    static constexpr int WIN_NUMBER = 10;
    static constexpr int INITIAL_FREQUENCY = 0;

    // Character constants
    static constexpr char DRAW = 'D';
    static constexpr char INITIAL_FILL = ' ';
    static constexpr unsigned char JUNCTION = 197;
    static constexpr unsigned char STRAIGHT_LINE = 179;
    static constexpr unsigned char SLEEPING_LINE = 196;
    static constexpr unsigned char COLUMN_SEPARATOR = ' ';

    typedef std::array<std::array<char, SIZE>, SIZE> singleBoard;

    // variables
    bool isGameDone;

    int currentCellToPlay;
    int currentBlockToPlay;
    int blocksCompletelyfilled;

    char currentPlayer;

    std::string player1Name, player2Name;

    singleBoard blockWinner;
    std::array<std::array<int, SIZE>, SIZE> frequencyCount;
    std::array<std::array<singleBoard, SIZE>, SIZE> gameBoard;

    // Helper methods
    void moveInput();
    void printTable();
    void intializeFullBoard();
    void intializeSingleBoard(std::array<std::array<int, SIZE>, SIZE> &);
    void intializeSingleBoard(singleBoard &);
    void drawLine(const char &, const char &);

    bool isBoardSolved(const singleBoard &);

    inline bool validateCurrentCellToPlay();
    inline bool validateCurrentBlockToPlay();

public:
    void startGame();
    SuperTicTacToe(const std::string &, const std::string &);
};

int main()
{
    std::string name1, name2;
    std::cout << "Enter name1 - ";
    std::cin >> name1;
    std::cout << "Enter name2 - ";
    std::cin >> name2;

    SuperTicTacToe one(name1, name2);
}

SuperTicTacToe::SuperTicTacToe(const std::string &player1Name, const std::string &player2Name)
{
    this->isGameDone = false;
    this->currentPlayer = 'X';
    this->currentBlockToPlay = -1;
    this->currentCellToPlay = -1;
    this->blocksCompletelyfilled = 0;
    this->player1Name = player1Name;
    this->player2Name = player2Name;

    intializeSingleBoard(frequencyCount);
    intializeSingleBoard(blockWinner);

    intializeFullBoard();
    startGame();
}

void SuperTicTacToe::startGame()
{
    int count = 0;
    while (!isGameDone)
    {
        count++;
        moveInput();

        CLEAR_SCREEN;
        printTable();
        if (count == 6)
        {
            std::cout << "Now";
        }
        gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE][currentCellToPlay / SIZE][currentCellToPlay % SIZE] = currentPlayer;
        frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE]++;

        if (isBoardSolved(gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE]))
        {
            blockWinner[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] = currentPlayer;
            frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] = WIN_NUMBER;

            if (isBoardSolved(blockWinner))
            {
                isGameDone = true;
                continue;
            }
        }

        if (frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] == SIZE * SIZE)
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

void SuperTicTacToe::moveInput()
{
    bool is_block_valid = validateCurrentBlockToPlay();
    bool is_cell_valid = validateCurrentCellToPlay();

    while (!is_block_valid)
    {
        CLEAR_SCREEN;
        printTable();

        std::cout << "Enter block to play - ";
        std::cin >> currentBlockToPlay;
        currentBlockToPlay--;

        is_block_valid = (currentBlockToPlay >= BLOCK_START && currentBlockToPlay <= BLOCK_END);
        is_block_valid &= validateCurrentBlockToPlay();

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

inline bool SuperTicTacToe::validateCurrentBlockToPlay()
{
    return (currentBlockToPlay != -1 && frequencyCount[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE] < SIZE * SIZE);
}

inline bool SuperTicTacToe::validateCurrentCellToPlay()
{
    return (currentCellToPlay != -1 && gameBoard[currentBlockToPlay / SIZE][currentBlockToPlay % SIZE][currentCellToPlay / SIZE][currentCellToPlay % SIZE] == INITIAL_FILL);
}

bool SuperTicTacToe::isBoardSolved(const singleBoard &currentBoard)
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

void SuperTicTacToe::intializeFullBoard()
{

    for (std::array<singleBoard, SIZE> &row : gameBoard)
    {
        for (singleBoard &singleGame : row)
        {
            intializeSingleBoard(singleGame);
        }
    }
}

void SuperTicTacToe::intializeSingleBoard(singleBoard &currentBoard)
{
    for (std::array<char, SIZE> &row : currentBoard)
    {
        for (char &block : row)
        {
            block = INITIAL_FILL;
        }
    }
}

void SuperTicTacToe::intializeSingleBoard(std::array<std::array<int, SIZE>, SIZE> &currentBoard)
{
    for (std::array<int, SIZE> &row : currentBoard)
    {
        for (int &block : row)
        {
            block = INITIAL_FREQUENCY;
        }
    }
}

void SuperTicTacToe::drawLine(const char &lineChar, const char &columnSeparator)
{
    for (int sperations = 0; sperations < SIZE; sperations++)
    {
        for (int print_char = 0; print_char < SIZE * 2 + 2; print_char++)
        {
            std::cout << lineChar;
        }

        if (sperations != SIZE - 1)
            std::cout << columnSeparator;
    }

    std::cout << std::endl;
}

void SuperTicTacToe::printTable()
{
    drawLine(COLUMN_SEPARATOR, STRAIGHT_LINE);

    for (int block_row = 0; block_row < SIZE; block_row++)
    {
        for (int row = 0; row < SIZE; row++)
        {
            std::cout << COLUMN_SEPARATOR;

            for (int first_col = 0; first_col < SIZE; first_col++)
            {
                std::cout << gameBoard[block_row][0][row][first_col] << (first_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int second_col = 0; second_col < SIZE; second_col++)
            {
                std::cout << gameBoard[block_row][1][row][second_col] << (second_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int third_col = 0; third_col < SIZE; third_col++)
            {
                std::cout << gameBoard[block_row][2][row][third_col] << (third_col != SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << std::endl;
            if (row != SIZE - 1)
            {

                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPARATOR << COLUMN_SEPARATOR << STRAIGHT_LINE;
                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPARATOR << COLUMN_SEPARATOR << STRAIGHT_LINE;
                std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
                std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE << COLUMN_SEPARATOR << COLUMN_SEPARATOR;
                std::cout << std::endl;
            }
        }

        drawLine(COLUMN_SEPARATOR, STRAIGHT_LINE);

        if (block_row < SIZE - 1)
        {
            drawLine(SLEEPING_LINE, JUNCTION);
        }

        drawLine(COLUMN_SEPARATOR, STRAIGHT_LINE);
    }
}