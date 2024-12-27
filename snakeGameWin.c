// 音频基于winapi,仅适用于windows
// using winapi to play music, windows only
// And modified by 谢建民
/*
加入多线程、信号、重新开始、暂停功能、难度选择、各类bgm播放、增加重新开始
同时添加部分暂停窗口透明度、难度遂等级变化
遂得分增加难度速度
*/
// Published in GNU GENERAL PUBLIC LICENSE Version 2
// 2024-12-23
#define PTW32_STATIC_LIB

#include <stdio.h>
#include <io.h>
#include <windows.h>

// windows多线程api
#include <process.h>

#include <mmsystem.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>

// 跨平台多线程api
// #include <pthread.h>

#include <gtk/gtk.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 25

// pre-defined winApiLib
// windows api function to play music
#pragma comment(lib, "WINMM.LIB")

typedef struct Player
{
    int position[FIELD_HEIGHT][FIELD_WIDTH];
    int direction; // valid values are 0: up, 1: right, 2: down, 3: left
    int maxValue;
    int isGrowing;
} player;

typedef struct Snake
{
    int score;
    int field[FIELD_HEIGHT][FIELD_WIDTH];
    player snake;
    int isSnakePlaced;
    int isApplePlaced;
    int level;
    int gameFieldRefreshTimeout;
    int isGameOver;
    int isGameStop;
} snake;

// game field widgets
GtkWidget *gameFieldSquare[FIELD_HEIGHT][FIELD_WIDTH];
// top label widget
GtkWidget *gameStateLabel;
// window
GtkWidget *window;
// initialize visible widgets
GtkWidget *gameStopBox;
GtkWidget *startBox;
GtkWidget *mainBox;
GtkWidget *containerBox;
GtkWidget *StopOverlay;
GtkWidget *gameOverBox;
GtkWidget *gameStateBox;
GtkWidget *gameFieldBox;
GtkWidget *gameFieldChildrenBoxes[FIELD_HEIGHT];
// game
snake game;
// initialize variables
gboolean TipWinvisible = FALSE;
gboolean TopWinvisible = FALSE;
int SpeedAtStart;
// initialize function
void Start();
void EasyStartGame();
void HardStartGame();
unsigned int __stdcall LoopThread(PVOID pM);

// 通过命令调用Windows的API播放音频
char winCommand[100];
// music file path
char MUSIC_FILE_PATH[] = "skaterswaltz_8bit.mp3";
char ATE_FILE_PATH[] = "ate_8bit.mp3";
char GAME_OVER_FILE_PATH[] = "Rockman3Shortend.mp3";

// 多线程保证事件堵塞，游戏正常运行
HANDLE startThread;
// 初始化事件为未触发状态
HANDLE waitEvent;
HANDLE EmptyFieldEvent;

// css style
// That will make changing the game style easier and more readable.
GdkDisplay *display;
char *gamecss = ".gameOverExitButton { margin-top: 10px; } \
                .gameOverTitle { font-size: 48px; } \
                .gameStopTitle { font-size: 48px; } \
                .gameFieldSquare { border-radius: 0px; } \
                .gameFieldSquare-even { background-color: green; } \
                .gameFieldSquare-odd { background-color: forestgreen; } \
                .apple { background-color: red; } \
                .snake { background-color: blue; } \
                .snakeHead { border: 4px solid yellow; } \
                .gameStartTitle { font-size: 48px; } \
                .gameStartTip { font-size: 24px; } \
                .easyGameButton { margin-top: 30px; } \
                .HardGameButton { margin-top: 30px; } ";

// different snake skin
char *hardgamecss = ".gameOverExitButton { margin-top: 10px; } \
                .gameOverTitle { font-size: 48px; } \
                .gameStopTitle { font-size: 48px; } \
                .gameFieldSquare { border-radius: 0px; } \
                .gameFieldSquare-even { background-color: green; } \
                .gameFieldSquare-odd { background-color: forestgreen; } \
                .apple { background-color: Gold; } \
                .snake { background-color: DarkViolet; } \
                .snakeHead { border: 3px solid Black; } \
                .gameStartTitle { font-size: 48px; } \
                .gameStartTip { font-size: 24px; } \
                .easyGameButton { margin-top: 30px; } \
                .HardGameButton { margin-top: 30px; } ";

void playMusic(char *musicName, char *nickName, int loop, int wait)
{
    memset(winCommand, 0, sizeof(winCommand));
    sprintf_s(winCommand, 100, "open res/%s alias %s", musicName, nickName);
    mciSendString(winCommand, NULL, 0, NULL);
    memset(winCommand, 0, sizeof(winCommand));
    sprintf_s(winCommand, 100, "set %s time format milliseconds", nickName);
    mciSendString(winCommand, NULL, 0, NULL);
    memset(winCommand, 0, sizeof(winCommand));
    if (loop)
    {
        sprintf_s(winCommand, 100, "play %s repeat", nickName);
    }
    else if (wait)
    {
        sprintf_s(winCommand, 100, "play %s wait", nickName);
    }
    else
    {
        sprintf_s(winCommand, 100, "play %s", nickName);
    }
    mciSendString(winCommand, NULL, 0, NULL);
    memset(winCommand, 0, sizeof(winCommand));
}

void closeMusic(char *nickName)
{
    sprintf_s(winCommand, 100, "close %s", nickName);
    mciSendString(winCommand, NULL, 0, NULL);
    memset(winCommand, 0, sizeof(winCommand));
}

gboolean StopAteBGM()
{
    closeMusic("atemusic");
    return FALSE;
}

// quit the application
void quitApplication()
{
    gtk_window_close(GTK_WINDOW(window));
}

// set window contents to game over
void finishGame()
{
    gtk_box_remove(GTK_BOX(containerBox), mainBox);

    gameOverBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(gameOverBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(gameOverBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(containerBox), gameOverBox);

    GtkWidget *gameOverLabel;
    gameOverLabel = gtk_label_new("Game over");
    gtk_widget_add_css_class(gameOverLabel, "gameOverTitle");
    gtk_box_append(GTK_BOX(gameOverBox), gameOverLabel);

    GtkWidget *gameOverDetails;
    gameOverDetails = gtk_label_new("Loading details...");
    gtk_box_append(GTK_BOX(gameOverBox), gameOverDetails);
    char scoreLabel[100];
    sprintf(scoreLabel, "蛇长度: %d - Level: %d", game.score, game.level);
    gtk_label_set_label(GTK_LABEL(gameOverDetails), scoreLabel);

    GtkWidget *gameOverExitButton;
    gameOverExitButton = gtk_button_new_with_label("Quit");
    gtk_widget_add_css_class(gameOverExitButton, "gameOverExitButton");
    g_signal_connect(gameOverExitButton, "clicked", G_CALLBACK(quitApplication), NULL);
    gtk_box_append(GTK_BOX(gameOverBox), gameOverExitButton);

    GtkWidget *gameOverRestartButton;
    gameOverRestartButton = gtk_button_new_with_label("Restart");
    gtk_widget_add_css_class(gameOverRestartButton, "gameOverRestartButton");
    g_signal_connect(gameOverRestartButton, "clicked", G_CALLBACK(Start), NULL);
    gtk_box_append(GTK_BOX(gameOverBox), gameOverRestartButton);
}

// game stop tips
void StopGame()
{
    GtkWidget *gameStopLabel;
    GtkWidget *gameStopTips;
    double Tipopacity = 0.9;

    gameStopBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(gameStopBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(gameStopBox, GTK_ALIGN_CENTER);
    gtk_widget_set_opacity(gameStopBox, Tipopacity);

    gtk_overlay_add_overlay(GTK_OVERLAY(StopOverlay), gameStopBox);

    gameStopLabel = gtk_label_new("Game stop");
    gtk_widget_add_css_class(gameStopLabel, "gameStopTitle");
    gtk_box_append(GTK_BOX(gameStopBox), gameStopLabel);

    gameStopTips = gtk_label_new("Push J to continue");
    gtk_box_append(GTK_BOX(gameStopBox), gameStopTips);

    TipWinvisible = TRUE;
    gtk_widget_set_visible(gameStopBox, TipWinvisible);

    printf("stop game\n");
    return;
}

// game resume
// see as https://discourse.gnome.org/t/how-to-not-destroy-a-widget/7449
void ResumeGame()
{
    TipWinvisible = FALSE;
    gtk_widget_set_visible(gameStopBox, TipWinvisible);
    printf("resume game\n");
}

// set direction label
char *generateDirectionLabel(int direction)
{
    switch (direction)
    {
    case 0:
        return "North";
        break;
    case 1:
        return "East";
        break;
    case 2:
        return "South";
        break;
    case 3:
        return "West";
        break;
    }
}

// set game level
void checkLevelScore()
{
    game.level = (game.score / 5) + 1;
}

// update label content
void updateLabels()
{
    char scoreLabel[100];
    sprintf(scoreLabel, "蛇长度: %d - Level: %d - Direction: %s - Speed: %d", game.score, game.level, generateDirectionLabel(game.snake.direction), 199 - game.gameFieldRefreshTimeout);
    gtk_label_set_label(GTK_LABEL(gameStateLabel), scoreLabel);
}

// find value in matrix
void findValueCoordinatesInMatrix(int matrix[FIELD_HEIGHT][FIELD_WIDTH], int value, int *coordinates)
{
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < FIELD_WIDTH; j++)
        {
            if (matrix[i][j] == value)
            {
                coordinates[0] = i;
                coordinates[1] = j;
            }
        }
    }
}

// handle keypressed event
gboolean key_pressed(GtkEventControllerKey *self, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
    printf("key pressed\n");
    switch (keyval)
    { // valid values are 0: up, 1: right, 2: down, 3: left
    case GDK_KEY_w:
    case GDK_KEY_Up:
        if (game.snake.direction != 2)
        {
            game.snake.direction = 0;
        }
        break;
    case GDK_KEY_s:
    case GDK_KEY_Down:
        if (game.snake.direction != 0)
        {
            game.snake.direction = 2;
        }
        break;
    case GDK_KEY_a:
    case GDK_KEY_Left:
        if (game.snake.direction != 1)
        {
            game.snake.direction = 3;
        }
        break;
    case GDK_KEY_d:
    case GDK_KEY_Right:
        if (game.snake.direction != 3)
        {
            game.snake.direction = 1;
        }
        break;
    case GDK_KEY_j:
    case GDK_KEY_J:
        if (game.isGameStop)
        {
            printf("out\n");
            ResumeGame();
            game.isGameStop = 0;
        }
        else
        {
            printf("in\n");
            game.isGameStop = 1;
            StopGame();
        }
        break;
    }
    return TRUE;
}

// empty game field
void emptyField()
{
    ResetEvent(EmptyFieldEvent);
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < FIELD_WIDTH; j++)
        {
            game.field[i][j] = 0;
        }
    }
    SetEvent(EmptyFieldEvent);
}

// refreshes the game field
gboolean refreshField()
{
    if (!game.isGameStop)
    {
        // if the player was not placed, it its now placed
        if (!game.isSnakePlaced)
        {
            game.snake.position[FIELD_HEIGHT / 2][FIELD_WIDTH / 2] = 1;
            game.snake.maxValue = 1;
            game.isSnakePlaced = 1;
            game.snake.direction = 1;
            game.snake.isGrowing = 0;
        }

        // if the apple was not placed, it is now placed
        if (!game.isApplePlaced)
        {
            int appleX;
            int appleY;
            do
            {
                appleX = rand() % FIELD_WIDTH;
                appleY = rand() % FIELD_HEIGHT;
            } while (game.snake.position[appleY][appleX] > 0);
            game.field[appleY][appleX] = 1;
            game.isApplePlaced = 1;
        }

        if (game.isSnakePlaced)
        {
            int snakeHeadCoordinates[2];
            findValueCoordinatesInMatrix(game.snake.position, game.snake.maxValue, snakeHeadCoordinates);
            // check field boundaries
            if (snakeHeadCoordinates[0] > 0 && game.snake.direction == 0)
            {
                if (game.field[snakeHeadCoordinates[0] - 1][snakeHeadCoordinates[1]] == 1)
                {
                    game.snake.isGrowing = 1;
                }
                if (game.snake.position[snakeHeadCoordinates[0] - 1][snakeHeadCoordinates[1]] > 0)
                {
                    game.isGameOver = 1;
                }
            }
            if (snakeHeadCoordinates[1] > 0 && game.snake.direction == 1)
            {
                if (game.field[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] + 1] == 1)
                {
                    game.snake.isGrowing = 1;
                }
                if (game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] + 1] > 0)
                {
                    game.isGameOver = 1;
                }
            }
            if (snakeHeadCoordinates[0] < FIELD_HEIGHT - 1 && game.snake.direction == 2)
            {
                if (game.field[snakeHeadCoordinates[0] + 1][snakeHeadCoordinates[1]] == 1)
                {
                    game.snake.isGrowing = 1;
                }
                if (game.snake.position[snakeHeadCoordinates[0] + 1][snakeHeadCoordinates[1]] > 0)
                {
                    game.isGameOver = 1;
                }
            }
            if (snakeHeadCoordinates[1] < FIELD_WIDTH - 1 && game.snake.direction == 3)
            {
                if (game.field[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] - 1] == 1)
                {
                    game.snake.isGrowing = 1;
                }
                if (game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] - 1] > 0)
                {
                    game.isGameOver = 1;
                }
            }
            if (!game.snake.isGrowing)
            {
                switch (game.snake.direction)
                {
                case 0:
                    if (snakeHeadCoordinates[0] > 0)
                    {
                        game.snake.position[snakeHeadCoordinates[0] - 1][snakeHeadCoordinates[1]] = game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1]];
                        snakeHeadCoordinates[0]--;
                    }
                    else
                    {
                        game.isGameOver = 1;
                    }
                    break;
                case 1:
                    if (snakeHeadCoordinates[1] < FIELD_WIDTH - 1)
                    {
                        game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] + 1] = game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1]];
                        snakeHeadCoordinates[1]++;
                    }
                    else
                    {
                        game.isGameOver = 1;
                    }
                    break;
                case 2:
                    if (snakeHeadCoordinates[0] < FIELD_HEIGHT - 1)
                    {
                        game.snake.position[snakeHeadCoordinates[0] + 1][snakeHeadCoordinates[1]] = game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1]];
                        snakeHeadCoordinates[0]++;
                    }
                    else
                    {
                        game.isGameOver = 1;
                    }
                    break;
                case 3:
                    if (snakeHeadCoordinates[1] > 0)
                    {
                        game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1] - 1] = game.snake.position[snakeHeadCoordinates[0]][snakeHeadCoordinates[1]];
                        snakeHeadCoordinates[1]--;
                    }
                    else
                    {
                        game.isGameOver = 1;
                    }
                    break;
                }
            }
            for (int i = 0; i < FIELD_HEIGHT; i++)
            {
                for (int j = 0; j < FIELD_WIDTH; j++)
                {
                    if (((i != snakeHeadCoordinates[0] || j != snakeHeadCoordinates[1])) && (game.snake.position[i][j] > 0) && (!game.snake.isGrowing))
                    {
                        game.snake.position[i][j]--;
                    }
                }
            }
            if (game.snake.isGrowing)
            {
                int appleCoordinates[2];
                game.score++;
                findValueCoordinatesInMatrix(game.field, 1, appleCoordinates);
                game.snake.maxValue++;
                game.snake.position[appleCoordinates[0]][appleCoordinates[1]] = game.snake.maxValue;
                game.field[appleCoordinates[0]][appleCoordinates[1]] = 0;
                game.isApplePlaced = 0;
                game.snake.isGrowing = 0;
                playMusic(ATE_FILE_PATH, "atemusic", 0, 0);
                // use timeout to stop the bgm of ate
                g_timeout_add(444, StopAteBGM, NULL);
            }
        }

        checkLevelScore();
        updateLabels();
        for (int i = 0; i < FIELD_HEIGHT; i++)
        {
            for (int j = 0; j < FIELD_WIDTH; j++)
            {
                switch (game.field[i][j])
                {
                case 0:
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "apple");
                    break;
                case 1:
                    gtk_widget_add_css_class(gameFieldSquare[i][j], "apple");
                    break;
                }

                if (game.snake.position[i][j] > 0)
                {
                    gtk_widget_add_css_class(gameFieldSquare[i][j], "snake");
                }
                else
                {
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "snake");
                }

                if (game.snake.position[i][j] == game.snake.maxValue)
                {
                    gtk_widget_add_css_class(gameFieldSquare[i][j], "snakeHead");
                }
                else
                {
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "snakeHead");
                }
            }
        }
        if (game.gameFieldRefreshTimeout != SpeedAtStart - game.level * game.level)
        {
            game.gameFieldRefreshTimeout = SpeedAtStart - game.level * game.level;
            SetEvent(waitEvent);
            return FALSE;
        }
        if (game.isGameOver)
        {
            SetEvent(waitEvent);
            finishGame();

            // WinApi关闭主要背景音乐文件，打开游戏结束音乐文件
            closeMusic("backgroundmusic");
            playMusic(GAME_OVER_FILE_PATH, "gameovermusic", 0, 1);

            return FALSE;
        }
    }
    return TRUE;
}

// handle activate event
static void activate(GtkApplication *app, gpointer user_data)
{
    display = gdk_display_get_default();
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, gamecss);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_FALLBACK);
    g_object_unref(provider);

    GtkIconTheme *icon_theme = gtk_icon_theme_get_for_display(display);
    gtk_icon_theme_add_search_path(icon_theme, "app");

    if (gtk_icon_theme_has_icon(icon_theme, "micon") != 1)
    {
        // manage error
        printf("icon not found\n");
    }

    // window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SnakeGame");
    gtk_window_set_default_icon_name("micon");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_icon_name(GTK_WINDOW(window), "micon");
    GtkEventController *keyPressEventController = gtk_event_controller_key_new();
    g_signal_connect_object(keyPressEventController, "key-pressed", G_CALLBACK(key_pressed), window, G_CONNECT_SWAPPED);
    gtk_widget_add_controller(GTK_WIDGET(window), keyPressEventController);

    // overlay
    StopOverlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(StopOverlay));

    // container box
    containerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(containerBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(containerBox, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(GTK_WIDGET(containerBox), "containerBox");
    gtk_overlay_set_child(GTK_OVERLAY(StopOverlay), GTK_WIDGET(containerBox));

    // Start Game box
    startBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(startBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(startBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(containerBox), startBox);

    // Game Title Show
    GtkWidget *gameTitleLabel;
    gameTitleLabel = gtk_label_new("Snake Game");
    gtk_widget_add_css_class(gameTitleLabel, "gameStartTitle");
    gtk_box_append(GTK_BOX(startBox), gameTitleLabel);

    // Start Tip Show
    GtkWidget *StartTipLabel;
    StartTipLabel = gtk_label_new("Chose a difficulty to enjoy the game!");
    gtk_widget_add_css_class(StartTipLabel, "gameStartTip");
    gtk_box_append(GTK_BOX(startBox), StartTipLabel);

    // Easy Start button
    GtkWidget *easyGameStartButton;
    easyGameStartButton = gtk_button_new_with_label("easyGame");
    gtk_widget_add_css_class(easyGameStartButton, "easyGameButton");
    g_signal_connect(easyGameStartButton, "clicked", G_CALLBACK(EasyStartGame), NULL);
    gtk_box_append(GTK_BOX(startBox), easyGameStartButton);

    // Hard Start button
    GtkWidget *hardGameStartButton;
    hardGameStartButton = gtk_button_new_with_label("HardGame");
    gtk_widget_add_css_class(hardGameStartButton, "HardGameButton");
    g_signal_connect(hardGameStartButton, "clicked", G_CALLBACK(HardStartGame), NULL);
    gtk_box_append(GTK_BOX(startBox), hardGameStartButton);

    // show the window
    TopWinvisible = TRUE;
    gtk_widget_set_visible(window, TopWinvisible);
}

// Whether the difficulty is easy or hard, it will ever be called.
void Start()
{
    // End chosing and start game!
    game.score = 0;
    game.isGameStop = 0;

    if (game.isGameOver)
    {
        closeMusic("gameovermusic");
        gtk_box_remove(GTK_BOX(containerBox), gameOverBox);
    }
    else
    {
        game.isGameOver = 0;
        gtk_box_remove(GTK_BOX(containerBox), startBox);
    }

    emptyField();
    WaitForSingleObject(EmptyFieldEvent, INFINITE);

    // WinApi加载音乐文件
    playMusic(MUSIC_FILE_PATH, "backgroundmusic", 1, 0);

    // main ui box
    mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(mainBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(mainBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(containerBox), mainBox);

    // top bar box
    gameStateBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(gameStateBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(gameStateBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(mainBox), gameStateBox);

    // game field
    gameFieldBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(gameFieldBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(gameFieldBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(mainBox), gameFieldBox);

    // game field objects
    memset(gameFieldChildrenBoxes, 0, sizeof(gameFieldChildrenBoxes));

    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        gameFieldChildrenBoxes[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_halign(gameFieldChildrenBoxes[i], GTK_ALIGN_CENTER);
        gtk_widget_set_valign(gameFieldChildrenBoxes[i], GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(gameFieldBox), gameFieldChildrenBoxes[i]);
        for (int j = 0; j < FIELD_WIDTH; j++)
        {
            gameFieldSquare[i][j] = gtk_label_new("");
            gtk_widget_set_size_request(gameFieldSquare[i][j], 25, 25);
            gtk_widget_add_css_class(gameFieldSquare[i][j], "gameFieldSquare");
            gtk_widget_add_css_class(gameFieldSquare[i][j], (i + j) % 2 == 0 ? "gameFieldSquare-even" : "gameFieldSquare-odd");
            gtk_box_append(GTK_BOX(gameFieldChildrenBoxes[i]), gameFieldSquare[i][j]);
        }
    }

    // Css won't remove even the widget removed.
    // It recorded as a array, so program need to reset it after the fields are set.
    if (game.isGameOver)
    {
        for (int i = 0; i < FIELD_HEIGHT; i++)
        {
            for (int j = 0; j < FIELD_WIDTH; j++)
            {
                if (game.snake.position[i][j] != game.snake.maxValue)
                {
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "snakeHead");
                }
                if (game.snake.position[i][j] > 0)
                {
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "snake");
                }
                if (game.field[i][j])
                {
                    gtk_widget_remove_css_class(gameFieldSquare[i][j], "apple");
                    break;
                }
                memset(game.snake.position, 0, sizeof(game.snake.position));
            }
        }

        game.isApplePlaced = 0;
        game.isGameOver = 0;
        game.isSnakePlaced = 0;
        game.gameFieldRefreshTimeout = SpeedAtStart;
    }

    // about game
    gameStateLabel = gtk_label_new("Starting game...");
    gtk_box_append(GTK_BOX(gameStateBox), gameStateLabel);

    SpeedAtStart = game.gameFieldRefreshTimeout;

    // 创建线程
    startThread = (HANDLE)_beginthreadex(NULL, 0, LoopThread, NULL, 0, NULL);
}

unsigned int __stdcall LoopThread(PVOID pM)
{
    // use timeout to reload field
    while (!game.isGameOver)
    {
        printf("game Start\n");
        g_timeout_add(game.gameFieldRefreshTimeout, refreshField, NULL);
        // 阻塞循环并等待事件被触发
        WaitForSingleObject(waitEvent, INFINITE);
        // 将事件设为末触发
        ResetEvent(waitEvent);
        printf("Speed changed.\n");
    }
    return 0;
}

void EasyStartGame()
{
    game.gameFieldRefreshTimeout = 200;
    Start();
}

void HardStartGame()
{
    game.gameFieldRefreshTimeout = 100;

    display = gdk_display_get_default();
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, hardgamecss);
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_FALLBACK);
    g_object_unref(provider);

    Start();
}

int main(int argc, char **argv)
{
    waitEvent = CreateEvent(NULL, false, false, NULL);

    EmptyFieldEvent = CreateEvent(NULL, false, false, NULL);

    // 设置控制台编码方式为utf-8
    system("chcp 65001");
    SetConsoleOutputCP(CP_UTF8);

    srand(time(NULL));
    game.gameFieldRefreshTimeout = 200;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0))
            {
                printf("snakeGame\nOptions:\n  --refreshTimeout / -rt | Set game tick interval in milliseconds (default is 200)\n  --help / -h            | Show this help message\n");
                exit(0);
            }
            if ((strcmp(argv[i], "--refreshTimeout") == 0) || (strcmp(argv[i], "-rt") == 0))
            {
                if (atoi(argv[i + 1]) == 0)
                {
                    printf("Refresh timeout value should be a positive integer.\n");
                    exit(1);
                }
                game.gameFieldRefreshTimeout = atoi(argv[i + 1]);
            }
        }
    }

    // app
    GtkApplication *app;
    int status;

    // app initialization and activation
    app = gtk_application_new("gd.rf.dowtyblog", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);

    // close event
    CloseHandle(waitEvent);
    CloseHandle(EmptyFieldEvent);

    // quit application
    g_object_unref(app);
    return status;
}
