#include <iostream>
#include <cstdlib>
#include <array>

#define debug \
    int a;    \
    std::cin >> a;

#ifdef _WIN32
#define CLEAR_SCREEN system("cls");
#define OPEN_INSTRUCTIONS system("start Instructions.pdf")

#elif __APPLE__
#define CLEAR_SCREEN system("clear");
#define OPEN_INSTRUCTIONS system("open Instructions.pdf")

#else
#define CLEAR_SCREEN system("clear");
#define OPEN_INSTRUCTIONS system("xdg-open Instructions.pdf")

#endif

class SuperTicTacToe
{
private:
    // Integer constants
    static constexpr int SIZE = 9;
    static constexpr int ROW_SIZE = 3;
    static constexpr int COLUMN_SIZE = 3;
    static constexpr int DIAGONAL_SIZE = 3;
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

    typedef std::array<char, SIZE> singleBoard;
    typedef std::array<int, SIZE> frequencyBoard;
    typedef std::array<singleBoard, SIZE> mainBoard;

    // variables
    bool isGameDone;

    int currentCellToPlay;
    int currentBlockToPlay;
    int blocksCompletelyfilled;

    char currentPlayer;

    std::string player1Name, player2Name;
    std::string clearScreenCommand;

    singleBoard blockWinner;
    frequencyBoard frequencyCount;
    mainBoard gameBoard;

    void printTable();
    void drawLine(const int);
    void drawLine(const char &, const char &);

    bool moveInput();
    bool isBoardSolved(const singleBoard &);

    void initializeBoard(mainBoard &);
    inline void initializeBoard(singleBoard &);
    inline void initializeBoard(frequencyBoard &);

    inline bool validateCurrentCellToPlay();
    inline bool validateCurrentBlockToPlay();

public:
    SuperTicTacToe(const std::string &, const std::string &);
    bool playGame();
};

int main()
{
    SuperTicTacToe one("a", "b");
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
    this->clearScreenCommand = clearScreenCommand;

    initializeBoard(gameBoard);
    initializeBoard(blockWinner);
    initializeBoard(frequencyCount);
    printTable();
}

bool SuperTicTacToe::playGame()
{
    return true;
}

bool SuperTicTacToe::moveInput()
{
    bool is_block_valid = validateCurrentBlockToPlay();
    bool is_cell_valid = validateCurrentCellToPlay();

    while (!is_block_valid)
    {
        std::cout << "Enter block to play - ";
        std::cin >> currentBlockToPlay;

        if (currentBlockToPlay == 0)
            return false;

        currentBlockToPlay--;

        is_block_valid = validateCurrentBlockToPlay();
    }

    while (!is_cell_valid)
    {
        std::cout << "Enter cell to play in block " << currentBlockToPlay + 1 << " - ";
        std::cin >> currentCellToPlay;

        if (currentBlockToPlay == 0)
            return false;

        currentCellToPlay--;

        is_cell_valid = validateCurrentCellToPlay();
    }

    return true;
}

inline bool SuperTicTacToe::validateCurrentCellToPlay()
{
    return (currentBlockToPlay >= BLOCK_START && currentBlockToPlay <= BLOCK_END && gameBoard[currentBlockToPlay][currentCellToPlay] == INITIAL_FILL);
}

inline bool SuperTicTacToe::validateCurrentBlockToPlay()
{
    return (currentBlockToPlay >= BLOCK_START && currentBlockToPlay <= BLOCK_END && blockWinner[currentBlockToPlay] != INITIAL_FILL && currentBlockToPlay != -1 && frequencyCount[currentBlockToPlay] <= SIZE);
}

inline void SuperTicTacToe::initializeBoard(singleBoard &currentBoard)
{
    currentBoard.fill(INITIAL_FILL);
}

inline void SuperTicTacToe::initializeBoard(frequencyBoard &currentBoard)
{
    currentBoard.fill(INITIAL_FREQUENCY);
}

void SuperTicTacToe::initializeBoard(mainBoard &currentBoard)
{
    for (singleBoard &each_board : currentBoard)
    {
        initializeBoard(each_board);
    }
}

bool SuperTicTacToe::isBoardSolved(const singleBoard &currentBoard)
{
    auto check_row = [&]()
    {
        bool is_solved = false;
        int row_count = 0;

        for (int row = 0; row < ROW_SIZE; row++)
        {
            row_count = 0;
            for (int col = 0; col < COLUMN_SIZE; col++)
            {
                row_count = row_count * (currentPlayer == currentBoard[row * ROW_SIZE + col % SIZE]) + 1;
            }

            is_solved |= (row_count == ROW_SIZE);
        }

        return is_solved;
    };

    auto check_column = [&]()
    {
        bool is_solved = false;
        int column_count = 0;

        for (int col = 0; col < COLUMN_SIZE; col++)
        {
            column_count = 0;
            for (int row = 0; row < ROW_SIZE; row++)
            {
                column_count = column_count * (currentPlayer == currentBoard[row * ROW_SIZE + col % SIZE]) + 1;
            }

            is_solved |= (column_count == COLUMN_SIZE);
        }

        return is_solved;
    };

    auto check_left_diagonal = [&]()
    {
        bool is_solved = true;

        for (int row = 0; row < ROW_SIZE; row++)
        {
            is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + row]);
        }

        return is_solved;
    };

    auto check_right_diagonal = [&]()
    {
        bool is_solved = true;

        for (int row = 0; row < ROW_SIZE; row++)
        {
            is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + ROW_SIZE - row - 1]);
        }

        return is_solved;
    };

    return check_row() || check_row() || check_left_diagonal() || check_right_diagonal();
}

void SuperTicTacToe::printTable()
{

    drawLine(COLUMN_SEPARATOR, COLUMN_SEPARATOR);

    for (int block_row = 0; block_row < ROW_SIZE; block_row++)
    {
        for (int row = 0; row < ROW_SIZE; row++)
        {
            std::cout << COLUMN_SEPARATOR;

            for (int first_col = 0; first_col < ROW_SIZE; first_col++)
            {
                std::cout << gameBoard[block_row][ROW_SIZE * row + first_col] << (first_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int second_col = 0; second_col < ROW_SIZE; second_col++)
            {
                std::cout << gameBoard[block_row + 1][ROW_SIZE * row + second_col] << (second_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int third_col = 0; third_col < ROW_SIZE; third_col++)
            {
                std::cout << gameBoard[block_row + 2][ROW_SIZE * row + third_col] << (third_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << std::endl;
            if (row != ROW_SIZE - 1)
            {
                drawLine(1);
                drawLine(2);
                drawLine(3);
            }
        }

        drawLine(COLUMN_SEPARATOR, STRAIGHT_LINE);

        if (block_row < ROW_SIZE - 1)
        {
            drawLine(SLEEPING_LINE, JUNCTION);
        }

        drawLine(COLUMN_SEPARATOR, STRAIGHT_LINE);
    }
}

void SuperTicTacToe::drawLine(const int lineNumber)
{
    std::cout << SLEEPING_LINE << SLEEPING_LINE << JUNCTION;
    std::cout << SLEEPING_LINE << JUNCTION << SLEEPING_LINE;
    std::cout << COLUMN_SEPARATOR << COLUMN_SEPARATOR;
    if (lineNumber != 3)
    {
        std::cout << STRAIGHT_LINE;
    }
    else
    {
        std::cout << std::endl;
    }
}

void SuperTicTacToe::drawLine(const char &lineChar, const char &columnSeparator)
{
    for (int sperations = 0; sperations < ROW_SIZE; sperations++)
    {
        for (int print_char = 0; print_char < ROW_SIZE * 2 + 2; print_char++)
        {
            std::cout << lineChar;
        }

        if (sperations != ROW_SIZE - 1)
            std::cout << columnSeparator;
    }

    std::cout << std::endl;
}
