#include <iostream>
#include <array>

#define CLEAR_SCREEN system("CLS");

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

    singleBoard blockWinner;
    frequencyBoard frequencyCount;
    mainBoard gameBoard;

    void moveInput();
    void initializeBoard(mainBoard &);
    inline void initializeBoard(singleBoard &);
    inline void initializeBoard(frequencyBoard &);
    inline bool validateCurrentCellToPlay();
    inline bool validateCurrentBlockToPlay();

    bool isBoardSolved(const singleBoard &);

public:
    SuperTicTacToe(const std::string &, const std::string &);
};

SuperTicTacToe::SuperTicTacToe(const std::string &player1Name, const std::string &player2Name)
{
    this->isGameDone = false;
    this->currentPlayer = 'X';
    this->currentBlockToPlay = -1;
    this->currentCellToPlay = -1;
    this->blocksCompletelyfilled = 0;
    this->player1Name = player1Name;
    this->player2Name = player2Name;

    initializeBoard(gameBoard);
    initializeBoard(blockWinner);
    initializeBoard(frequencyCount);
}

void SuperTicTacToe::moveInput()
{
    bool is_cell_valid;
    bool is_cell_valid;
}

inline bool SuperTicTacToe::validateCurrentCellToPlay()
{
}

inline bool SuperTicTacToe::validateCurrentBlockToPlay()
{
    return (blockWinner[currentBlockToPlay] != INITIAL_FILL && currentBlockToPlay != -1 && frequencyCount[currentBlockToPlay] <= SIZE);
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