#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <array>

#define GAME_TITLE std::cout << "\t\t\tSuper TicTacToe\n"
#define TIMEOUT(time) sleep(time)

// Macros for machine specific instructions
#ifdef _WIN32
#define CLEAR_SCREEN system("cls");
#define CHANGE_TERMINAL_COLOR system("color 3F")

inline int OPEN_INSTRUCTIONS()
{
    return system("start Instructions.pdf");
}

#elif __APPLE__
#define CLEAR_SCREEN system("clear");
#define CHANGE_TERMINAL_COLOR ;
inline int OPEN_INSTRUCTIONS()
{
    return system("open Instructions.pdf");
}

#else
#define CLEAR_SCREEN system("clear");
#define CHANGE_TERMINAL_COLOR ;
inline int OPEN_INSTRUCTIONS()
{
    return system("xdg-open Instructions.pdf");
}
#endif

class SuperTicTacToe
{
private:
    // Integer constants
    static const int SIZE = 9;
    const int ROW_SIZE = 3;
    const int COLUMN_SIZE = 3;
    const int DIAGONAL_SIZE = 3;
    const int BLOCK_END = 8;
    const int QUIT_KEY = 0;
    const int BLOCK_START = 0;
    const int WIN_NUMBER = 10;
    const int INITIAL_FREQUENCY = 0;

    // Character constants
    const char DRAW = 'D';
    const char STOP = 'S';
    const char INITIAL_FILL = ' ';
    const unsigned char JUNCTION = 197;
    const unsigned char STRAIGHT_LINE = 179;
    const unsigned char SLEEPING_LINE = 196;
    const unsigned char COLUMN_SEPARATOR = ' ';

    typedef std::array<char, SIZE> singleBoard;
    typedef std::array<int, SIZE> frequencyBoard;
    typedef std::array<singleBoard, SIZE> mainBoard;

    // variables
    bool isGameDone;

    int currentCellToPlay;
    int currentBlockToPlay;
    int restrictedBlocks;

    char currentPlayer;

    std::string player1Name, player2Name;

    mainBoard gameBoard;
    singleBoard blockWinner;
    frequencyBoard frequencyCount;

    // helper methods
    void printTable();
    void drawLine(const int);
    void drawLine(const char &, const char &);

    bool moveInput();
    bool isBoardSolved(const singleBoard &);

    void initializeBoard(mainBoard &);
    inline void initializeBoard(singleBoard &);
    inline void initializeBoard(frequencyBoard &);

    inline bool isCellValid();
    inline bool isBlockValid();

public:
    SuperTicTacToe();
    char playGame();
};

void printMainMenu();
void printInstrctionsMenu();
void printFileOpenError();
void inputPlayersName(std::string &, std::string &);
void printWrongInput();

int main()
{
    CLEAR_SCREEN;
    CHANGE_TERMINAL_COLOR;

    int choice;
    bool playGame = true;
    std::string player1, player2;

    while (playGame)
    {
        CLEAR_SCREEN;
        GAME_TITLE;

        printMainMenu();
        std::cin >> choice;

        CLEAR_SCREEN;
        GAME_TITLE;

        switch (choice)
        {
        case 0:
            playGame = false;
            continue;

        case 1:
            break;

        case 2:
            printInstrctionsMenu();
            TIMEOUT(2);
            if (OPEN_INSTRUCTIONS != 0)
            {
                printFileOpenError();
                TIMEOUT(5);
            }
            continue;

        default:
            printWrongInput();
            continue;
        }

        CLEAR_SCREEN;
        GAME_TITLE;

        inputPlayersName(player1, player2);
        SuperTicTacToe startGame;

        char gameStatus = startGame.playGame();

        if (gameStatus == 'S')
            std::cout << "\n\nGame Stopped :(\n";

        else
            std::cout << "\n\n"
                      << (gameStatus == 'X' ? player1 : player2) << " Won :)\n\n";

        std::cout << "You will be redirected to main menu in 5 seconds....";
        TIMEOUT(5);
    }

    std::cout << "\nCome Back! Play Again :)";
    TIMEOUT(1.5);
}

SuperTicTacToe::SuperTicTacToe()
{

    this->isGameDone = false;
    this->currentPlayer = 'X';
    this->currentBlockToPlay = -1;
    this->currentCellToPlay = -1;
    this->restrictedBlocks = 0;

    initializeBoard(gameBoard);
    initializeBoard(blockWinner);
    initializeBoard(frequencyCount);
}

char SuperTicTacToe::playGame()
{
    while (!isGameDone)
    {
        CLEAR_SCREEN;
        printTable();

        if (!moveInput())
            return STOP;

        gameBoard[currentBlockToPlay][currentCellToPlay] = currentPlayer;
        frequencyCount[currentBlockToPlay]++;

        CLEAR_SCREEN;
        printTable();

        if (isBoardSolved(gameBoard[currentBlockToPlay]))
        {
            blockWinner[currentBlockToPlay] = currentPlayer;
            frequencyCount[currentBlockToPlay] = WIN_NUMBER;

            if (isBoardSolved(blockWinner))
            {
                isGameDone = true;
                continue;
            }
        }
        else if (frequencyCount[currentBlockToPlay] == SIZE)
        {
            blockWinner[currentBlockToPlay] = DRAW;
            frequencyCount[currentBlockToPlay] = WIN_NUMBER;
            std::cout << "Tie\n";
        }

        restrictedBlocks = frequencyCount[currentBlockToPlay] > SIZE ? restrictedBlocks + 1 : restrictedBlocks;

        if (restrictedBlocks == SIZE)
        {
            isGameDone = true;
            currentPlayer = DRAW;
            continue;
        }

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        currentBlockToPlay = currentCellToPlay;
        currentCellToPlay = -1;
    }

    return currentPlayer;
}

bool SuperTicTacToe::moveInput()
{
    while (!isBlockValid())
    {
        std::cout << "Enter block to play - ";
        std::cin >> currentBlockToPlay;

        if (currentBlockToPlay == 0)
            return false;

        currentBlockToPlay--;
    }

    while (!isCellValid())
    {
        std::cout << "Enter cell to play in block " << currentBlockToPlay + 1 << " - ";
        std::cin >> currentCellToPlay;

        if (currentCellToPlay == 0)
            return false;

        currentCellToPlay--;
    }

    return true;
}

inline bool SuperTicTacToe::isCellValid()
{
    return (currentCellToPlay >= BLOCK_START && currentCellToPlay <= BLOCK_END && gameBoard[currentBlockToPlay][currentCellToPlay] == INITIAL_FILL);
}

inline bool SuperTicTacToe::isBlockValid()
{
    return (currentBlockToPlay >= BLOCK_START && currentBlockToPlay <= BLOCK_END && blockWinner[currentBlockToPlay] == INITIAL_FILL && frequencyCount[currentBlockToPlay] < SIZE);
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
        bool is_solved = true;

        for (int row = 0; row < ROW_SIZE; row++)
        {
            is_solved = true;
            for (int col = 0; col < COLUMN_SIZE && is_solved; col++)
            {
                is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + col % SIZE]);
            }

            if (is_solved)
                return is_solved;
        }

        return is_solved;
    };

    auto check_column = [&]()
    {
        bool is_solved = true;

        for (int col = 0; col < COLUMN_SIZE; col++)
        {
            is_solved = true;
            for (int row = 0; row < ROW_SIZE && is_solved; row++)
            {
                is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + col % SIZE]);
            }

            if (is_solved)
                return is_solved;
        }

        return is_solved;
    };

    auto check_left_diagonal = [&]()
    {
        bool is_solved = true;

        for (int row = 0; row < ROW_SIZE && is_solved; row++)
        {
            is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + row]);
        }

        return is_solved;
    };

    auto check_right_diagonal = [&]()
    {
        bool is_solved = true;

        for (int row = 0; row < ROW_SIZE && is_solved; row++)
        {
            is_solved &= (currentPlayer == currentBoard[row * ROW_SIZE + ROW_SIZE - row - 1]);
        }

        return is_solved;
    };

    return check_row() || check_column() || check_left_diagonal() || check_right_diagonal();
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
                std::cout << gameBoard[block_row * ROW_SIZE][ROW_SIZE * row + first_col] << (first_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int second_col = 0; second_col < ROW_SIZE; second_col++)
            {
                std::cout << gameBoard[block_row * ROW_SIZE + 1][ROW_SIZE * row + second_col] << (second_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
            }

            std::cout << COLUMN_SEPARATOR << STRAIGHT_LINE << COLUMN_SEPARATOR;

            for (int third_col = 0; third_col < ROW_SIZE; third_col++)
            {
                std::cout << gameBoard[block_row * ROW_SIZE + 2][ROW_SIZE * row + third_col] << (third_col != ROW_SIZE - 1 ? STRAIGHT_LINE : COLUMN_SEPARATOR);
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

void printMainMenu()
{
    std::cout << "1. Play game \n2. Read Instructions\n0.Exit";
    std::cout << "\n\tChoose your option - ";
}

void printInstrctionsMenu()
{
    std::cout << "Trying to open Instructions of the game\n";
    std::cout << "If opened sucessfully game will be redirected to main menu\n";
}

void printFileOpenError()
{
    std::cout << "Error in opening Instructions pdf\n";
    std::cout << "Make sure that pdf is present in the same directory and necessary dependencies installed if any\n";
    std::cout << "You will be redirected to main menu in 5 seconds.......\n";
}

void inputPlayersName(std::string &player1, std::string &player2)
{
    std::cout << "Enter Player 1 name - ";
    std::cin >> player1;
    std::cout << "Enyer Player 2 name - ";
    std::cin >> player2;
}

void printWrongInput()
{
    std::cout << "Invalid option\n";
    std::cout << "Redirecting to Main Menu...";
    TIMEOUT(3);
}