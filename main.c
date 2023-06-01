#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

char *ColorizeString(char *text, char *hexRgb)
{
    int r, g, b;
    sscanf(hexRgb, "%02x%02x%02x", &r, &g, &b);

    int length = snprintf(NULL, 0, "\x1b[38;2;%d;%d;%dm%s\x1b[0m", r, g, b, text);
    if (length >= 0)
    {
        char *resultColorizedText = malloc(length + 1);
        if (resultColorizedText != NULL)
        {
            snprintf(resultColorizedText, length + 1, "\x1b[38;2;%d;%d;%dm%s\x1b[0m", r, g, b, text);
            return resultColorizedText;
        }
        else
            return NULL;
    }
}

enum RockPaperScissors
{
    Rock = 0,
    Paper,
    Scissors
};
char *RockPaperScissorsToString(enum RockPaperScissors rockPaperScissors)
{
    switch (rockPaperScissors)
    {
    case Rock:
        return "Rock";
    case Paper:
        return "Paper";
    case Scissors:
        return "Scissors";
    default:
        return "Unknown";
    }
}

enum GameResult
{
    PlayerWinner = 0,
    Draw,
    ComputerWinner
};
char *GameResultToString(enum GameResult gameResult)
{
    switch (gameResult)
    {
    case PlayerWinner:
        return "Winner";
    case Draw:
        return "Draw";
    case ComputerWinner:
        return "Lose";
    default:
        return "Unknown";
    }
}

char *GameResultToColorizeString(enum GameResult gameResult)
{
    switch (gameResult)
    {
    case PlayerWinner:
        return ColorizeString("Winner", "00FF00");
    case Draw:
        return ColorizeString("Draw", "FFFFFF");
    case ComputerWinner:
        return ColorizeString("Lose", "FF0000");
    default:
        return ColorizeString("Unknown", "808080");
    }
}
void SetConsoleCursorPosition(int x, int y)
{
    printf("\033[%d;%df", y, x);
}
void HideConsoleCursor()
{
    printf("\e[?25l");
}
void ShowConsoleCursor()
{
    printf("\e[?25h");
}
int GenerateRandomIntegerInRange(int first, int last)
{
    return (rand() % (last + 1) + first);
}
enum RockPaperScissors GenerateRandomRockPaperScissors()
{
    return (enum RockPaperScissors)(GenerateRandomIntegerInRange(0, 2));
}
enum GameResult DetectGameResult(enum RockPaperScissors player, enum RockPaperScissors computer)
{
    switch (player)
    {
    case Rock:
        if (computer == Paper)
            return ComputerWinner;
        else if (computer == Scissors)
            return PlayerWinner;
        break;

    case Paper:
        if (computer == Rock)
            return PlayerWinner;
        else if (computer == Scissors)
            return ComputerWinner;
        break;

    case Scissors:
        if (computer == Rock)
            return ComputerWinner;
        else if (computer == Paper)
            return PlayerWinner;
        break;
    }

    return Draw;
}
void WaitToPressEnterKey()
{
    while (getch() != 13)
        ;
}
void MoveConsoleCursorUp(unsigned int rows)
{
    printf("\033[%dA", rows);
}
void MoveConsoleCursorDown(unsigned int rows)
{
    printf("\033[%dB", rows);
}
void DelayMilliseconds(unsigned int milliseconds)
{
    if (milliseconds == 0)
        return;

    struct timespec delay;
    delay.tv_sec = milliseconds / 1000;
    delay.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&delay, NULL);
}

int main()
{
    HideConsoleCursor();

    // Learning to play
    printf("%s\r\n", ColorizeString("Use keyboard right and left arrow keys to select item", "FFFFFF"));
    for (size_t i = 0; i < 2; i++)
    {
        printf("Loading   \r");
        DelayMilliseconds(300);
        printf("Loading.  \r");
        DelayMilliseconds(300);
        printf("Loading.. \r");
        DelayMilliseconds(300);
        printf("Loading...\r");
        DelayMilliseconds(300);
    }

    // Set the seed for random number generation
    srand(time(NULL));

    unsigned long long playerScore = 0;
    unsigned long long computerScore = 0;
    printf("---------------------------------\r\n");
    printf("Player = %d  |  Computer = %d\r\n", playerScore, computerScore);
    printf("---------------------------------\r\n");

    const char *greenRockWord = ColorizeString("Rock", "00FF00");
    const char *greenPaperWord = ColorizeString("Paper", "00FF00");
    const char *greenScissorsWord = ColorizeString("Scissors", "00FF00");

    bool firstTime = true;
    char i = 1;
    while (true)
    {
        if (firstTime)
        {
            printf("Rock  |  %s  |  Scissors\r\n", greenPaperWord);
            printf("     <-         ->     ");
            MoveConsoleCursorUp(1);
        }
        else
        {
            MoveConsoleCursorUp(7);
        }

        int key = 0;
        while (key != 13) // Continue until enter is pressed
        {
            key = getch();

            if (key == 75) // Arrow left
                i = (i <= 0) ? 2 : --i;
            else if (key == 77) // Arrow right
                i = (i >= 2) ? 0 : ++i;
            else // Not pressed left or right arrow keys
                continue;

            // Switch to Rock,Paper,Scissors items ( Infinity )
            switch (i)
            {
            case 0:
                printf("\r%s  |  Paper  |  Scissors", greenRockWord);
                break;
            case 1:
                printf("\rRock  |  %s  |  Scissors", greenPaperWord);
                break;
            case 2:
                printf("\rRock  |  Paper  |  %s", greenScissorsWord);
                break;
            }
        }

        MoveConsoleCursorDown(2);
        printf("\r---------------------------------\r\n");

        enum RockPaperScissors playerSelectedItem = (enum RockPaperScissors)i;
        printf("Player : %s        \r\n", RockPaperScissorsToString(playerSelectedItem));

        enum RockPaperScissors computerSelectedItem = GenerateRandomRockPaperScissors();
        printf("Computer : %s        \r\n", RockPaperScissorsToString(computerSelectedItem));

        enum GameResult gameResult = DetectGameResult(playerSelectedItem, computerSelectedItem);
        printf("Game result is %s       \r\n", GameResultToColorizeString(gameResult));

        MoveConsoleCursorUp(8);
        if (gameResult != Draw)
        {
            switch (gameResult)
            {
            case PlayerWinner:
                playerScore++;
                break;
            case ComputerWinner:
                computerScore++;
                break;
            }
            printf("\rPlayer = %llu  |  Computer = %llu", playerScore, computerScore);
        }

        MoveConsoleCursorDown(8);
        printf("\r---------------------------------\r\n");

        firstTime = false;
    }
}