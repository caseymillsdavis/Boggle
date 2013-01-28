/*
 * File: BogglePlus.cpp
 * ----------------
 * Name: Casey Mills Davis
 * Section: Robert Hintz
 * This file contains functions for playing boggle.
 */

/* Includes */

#include <iostream>
#include <vector>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "random.h"
#include "strlib.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const int STANDARD_CUBES_SIZE = 16;
const int BIG_BOGGLE_CUBES_SIZE = 25;

const int STANDARD_BOGGLE_EDGE = 4;
const int BIG_BOGGLE_EDGE = 5;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

const int FOUR_LETTERS = 4;
const bool UNUSED = true;
const bool USED = false;
const bool NEW_LINE = true;
const bool NO_LINE = false;
const int WELCOME_PAUSE1 = 2000;
const int WELCOME_PAUSE2 = 1500;
const int STANDARD_PAUSE = 800;
const int HIGHLIGHT_TIME = 400;
const int TYPE_SPEED = 40;


const int* SIZE;
const int* EDGES;
const string* CUBES;




/* Function prototypes */

void welcome();
void giveInstructions();
void stringPrinter(string str, bool newline);
bool yesOrNo(string yesNoQuestion);
void fillBoggleGrid(string boggleString);
string manualBoggleGrid(void);
string autoBoggleGrid(void);
vector<string> getTileArrangement(void);
string getLetters(vector<string> tilePlacement);
char getRandomLetterFromTile(string boggleTile);
string getBoardString(void);
Lexicon humansTurn(Grid<char> &boggleGrid);
bool humanWordPreCheck(string userWord, Lexicon &engLex, Lexicon &usedLex);
bool isLessThanFourLetters(string word);
void testBoard(string userWord, Grid<char> &boggleGrid);
void highlightWord(Grid<bool> usageGrid);
void unHighlightGrid(void);
void computersTurn(Grid<char> &boggleGrid, Lexicon &usedLex);
bool onBoardFromPosition(int gridRow, 
                         int gridCol, 
                         string &userWord, 
                         string &boardWord,
                         Grid<char> &boggleGrid,
                         Grid<bool> &usageGrid);
void findAllWords(int gridRow, 
                  int gridCol,
                  string &boardWord,
                  Grid<char> &boggleGrid,
                  Grid<bool> &usageGrid,
                  Lexicon &engWords,
                  Lexicon &allWords,
                  Lexicon &usedLex);
bool inRange(int y, int x);
Grid<bool> getCleanUsageGrid(int numRows, int numCols);
Grid<char> getBoardGrid(string boardString);
void boggleProgram(void);
void bigBoggleSwitch(void);

/* Main program */

int main1() {
    //boggleProgram();
    while (true) {
        bigBoggleSwitch();
    }
    return 0;
}


void bigBoggleSwitch(void){
    if (yesOrNo("Would you like to play big boggle?")) {
        SIZE = &BIG_BOGGLE_CUBES_SIZE;
        EDGES = &BIG_BOGGLE_EDGE;
        CUBES = &BIG_BOGGLE_CUBES[0];
    } else {
        SIZE = &STANDARD_CUBES_SIZE;
        EDGES = &STANDARD_BOGGLE_EDGE;
        CUBES = &STANDARD_CUBES[0];
    }
    for (int i = 0; i < *SIZE; i++) {
        cout << CUBES[i] << endl;
    }
    cout << *SIZE << endl;
    cout << *EDGES << endl;
}


/*
 * Function: yesOrNo
 * -----------------
 * Recursive interface for asking user a yes or no
 * question.
 */
bool yesOrNo(string yesNoQuestion) {
    string response = getLine(yesNoQuestion);
    response = toLowerCase(response);
    if (response[0] == 'n') {
        return false;
    } else if (response[0] == 'y') {
        return true;
    } else {
        cout << "Please answer yes or no." << endl;
        return yesOrNo(yesNoQuestion);
    }
}


/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    stringPrinter("Do you know what time it is???", NO_LINE);
    pause(WELCOME_PAUSE1);
    stringPrinter("IT'S BOGGLE:30!", NEW_LINE);
    pause(WELCOME_PAUSE2);
    stringPrinter("So pull your pants up, take a drag off your cig, ", NEW_LINE);
    stringPrinter("and tell your lady-friend to make some sandwiches.....", NO_LINE);
    stringPrinter("It's gonna be a long night.", NEW_LINE);
    cout << endl << endl;
}


/*
 * Function: stringPrinter
 * --------------------------
 * Outputs the given string with a type-writer effect.
 */
void stringPrinter(string str, bool newline) {
    if (newline) cout << endl;
    string::iterator it;
    for (it = str.begin(); it != str.end(); it++) {
        cout << *it;
        pause(TYPE_SPEED);
    }
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

/*
 * Function: manualBoggleGrid
 * -----------------
 * wrapper function for generating the boggle grid
 * from user input
 */
string manualBoggleGrid(void) {
    string boggleString = getBoardString();
    fillBoggleGrid(boggleString);
    return boggleString;
}

/*
 * Function: autoBoggleGrid
 * -----------------
 * wrapper function for generating the boggle grid
 * automatically using standard boggle cubes and
 * randomization functions
 */
string autoBoggleGrid(void) {
    vector<string> newTiles = getTileArrangement();
    string boggleString = getLetters(newTiles);
    fillBoggleGrid(boggleString);
    return boggleString;
}

/*
 * Function: fillBoggleGrid
 * -----------------
 * graphically updates the boggle grid using
 * the given string, which should have length = 16
 */
void fillBoggleGrid(string boggleString) {
    int k = 0;
    for (int i = 0; i < STANDARD_BOGGLE_EDGE; i++) {
        for (int j = 0; j < STANDARD_BOGGLE_EDGE; j++) {
            labelCube(i, j, boggleString[k]);
            k++;
        }
    }
}

/*
 * Function: getTileArrangement
 * -----------------
 * using the cube arrays provided,
 * returns them in a randomized order
 */
vector<string> getTileArrangement(void) {
    vector<string> tilePlacement;
    tilePlacement.assign(STANDARD_CUBES, STANDARD_CUBES + STANDARD_CUBES_SIZE);
    for (int i = 0; i < tilePlacement.size(); i++) {
        int randIndex = randomInteger(i, tilePlacement.size() - 1);
        string tempString = tilePlacement[i];
        tilePlacement[i] = tilePlacement[randIndex];
        tilePlacement[randIndex] = tempString;
    }
    return tilePlacement;
}

/*
 * Function: getLetters
 * -----------------
 * returns a string populated by randomly
 * selecting one letter from each of the 
 * given tiles
 */
string getLetters(vector<string> tilePlacement) {
    string letters;
    for (vector<string>::iterator it = tilePlacement.begin(); it != tilePlacement.end(); it++) {
        letters += getRandomLetterFromTile(*it);
    }
    return letters;
}

/*
 * Function: getRandomLetterFromTile
 * -----------------
 * returns a random char from the given string
 */
char getRandomLetterFromTile(string boggleTile) {
    int randIndex = randomInteger(0, boggleTile.size() - 1);
    return boggleTile[randIndex];
}

/*
 * Function: getBoardString
 * -----------------
 * forces user to enter a string of length at least 16.
 * returns the first 16 chars of the string, all uppercase
 */
string getBoardString(void) {
    string boardString = getLine("Enter Boggle string: ");
    if (boardString.size() < STANDARD_CUBES_SIZE) {
        cout << "Please enter a sufficently long string (MIN " 
        << STANDARD_CUBES_SIZE << " chars)." << endl;
        return getBoardString();
    } else {
        return toUpperCase(boardString.substr(0, STANDARD_CUBES_SIZE));
    }
}

/*
 * Function: humansTurn
 * -----------------
 * prompts user for word entries
 * wraps functions for checking these entries
 * returns all words successful user words in a lexicon
 */
Lexicon humansTurn(Grid<char> &boggleGrid) {
    Lexicon engWords("EnglishWords.dat");
    Lexicon usedLex;
    cout << "Ok, take all the time you want and";
    cout << " find all the words you can!";
    cout << " Signal that you're finished by entering an empty line.";
    cout << endl << endl;
    while (true) {
        string userWord = getLine("Enter a word:");
        userWord = toUpperCase(userWord);
        if (userWord == "") break;
        if (humanWordPreCheck(userWord, engWords, usedLex)) {
            usedLex.add(userWord);
            testBoard(userWord, boggleGrid);
        } else {
            cout << "Invalid word." << endl;
        }
    }
    return usedLex;
}

/*
 * Function: testBoard
 * -----------------
 * loops over the board and calls
 * the recursive word checker at each position
 */
void testBoard(string userWord, Grid<char> &boggleGrid) {
    Grid<bool> usageGrid;
    string boardWord;
    bool foundWord = false;
    int gcount = 0;
    int grow;
    int gcol;
    foreach(char ch in boggleGrid){
        grow = gcount / STANDARD_BOGGLE_EDGE;
        gcol = gcount % STANDARD_BOGGLE_EDGE;
        boardWord = string() + ch;
        usageGrid = getCleanUsageGrid(STANDARD_BOGGLE_EDGE, STANDARD_BOGGLE_EDGE);
        usageGrid[grow][gcol] = USED;
        if (boardWord == userWord.substr(0, boardWord.size())) {
            if (onBoardFromPosition(grow, gcol, userWord, boardWord, boggleGrid, usageGrid)) {
                recordWordForPlayer(userWord, HUMAN);
                highlightWord(usageGrid);
                pause(HIGHLIGHT_TIME);
                unHighlightGrid();
                foundWord = true;
                break;
            }
        }
        gcount++;
    }
    foundWord ? (cout << "Way to go!" << endl) : (cout << "Invalid word." << endl);
}

/*
 * Function: highlightWord
 * -----------------
 * uses usageGrid to highlight a successful word
 * on the boggle board.
 */
void highlightWord(Grid<bool> usageGrid) {
    int gcount = 0;
    int grow;
    int gcol;
    foreach(bool lightUp in usageGrid) {
        grow = gcount / STANDARD_BOGGLE_EDGE;
        gcol = gcount % STANDARD_BOGGLE_EDGE;
        highlightCube(grow, gcol, !lightUp);
        gcount++;
    }
}

/*
 * Function: unHighlightWord
 * -----------------
 * un-highlights the boggle board
 */
void unHighlightGrid(void) {
    int gcount = 0;
    int grow;
    int gcol;
    while (gcount <= (STANDARD_CUBES_SIZE - 1)) {
        grow = gcount / STANDARD_BOGGLE_EDGE;
        gcol = gcount % STANDARD_BOGGLE_EDGE;
        highlightCube(grow, gcol, false);
        gcount++;
    }
}

/*
 * Function: humanWordPreCheck
 * -----------------
 * ensures that the entered word meets
 * basic criteria
 */
bool humanWordPreCheck(string userWord, Lexicon &engLex, Lexicon &usedLex) {
    return !((userWord.size() < FOUR_LETTERS) |
             !engLex.contains(userWord) | 
             usedLex.contains(userWord));
}

/*
 * Function: onBoardFromPosition
 * -----------------
 * the meat of the human-turn
 * this is the recursive function which returns
 * true if the userWord is found on the board
 * gridRow and gridCol track its current board position
 * userWord is the word it is looking for
 * boardWord is the string represented by the current level of 
 * recursion
 * boggleGrid is the characters of the boggle board
 * usageGrid tracks which boggle positions have already been used
 */
bool onBoardFromPosition(int gridRow, 
                         int gridCol, 
                         string &userWord, 
                         string &boardWord,
                         Grid<char> &boggleGrid,
                         Grid<bool> &usageGrid) {
    if (boardWord == userWord) {
        return true;
    } else {
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                int newRow = gridRow + y;
                int newCol = gridCol + x;
                if (!inRange(newRow, newCol)) continue;
                if (usageGrid[newRow][newCol] == UNUSED) {
                    boardWord.push_back(boggleGrid[newRow][newCol]);
                    if (boardWord == userWord.substr(0, boardWord.size())) {
                        usageGrid[newRow][newCol] = USED;
                        if (onBoardFromPosition(newRow, newCol, userWord, boardWord, boggleGrid, usageGrid)) {
                            return true;
                        }
                        usageGrid[newRow][newCol] = UNUSED;
                    }
                    boardWord = boardWord.substr(0, boardWord.size() - 1);
                }
            }
        }
        return false;
    }
}

/*
 * Function: computersTurn
 * -----------------
 * scores the computer for all words not
 * found by the user. wraps the recursive
 * function that does the searching.
 */
void computersTurn(Grid<char> &boggleGrid, Lexicon &usedLex) {
    Grid<bool> usageGrid;
    string boardWord;
    Lexicon engWords("EnglishWords.dat");
    Lexicon allWords;
    int gcount = 0;
    int grow;
    int gcol;
    foreach(char ch in boggleGrid){
        grow = gcount / STANDARD_BOGGLE_EDGE;
        gcol = gcount % STANDARD_BOGGLE_EDGE;
        boardWord = string() + ch;
        usageGrid = getCleanUsageGrid(STANDARD_BOGGLE_EDGE, STANDARD_BOGGLE_EDGE);
        usageGrid[grow][gcol] = USED;
        findAllWords(grow, gcol, boardWord, boggleGrid, usageGrid, engWords, allWords, usedLex);
        gcount++;
    }
    foreach(string word in allWords){
        recordWordForPlayer(word, COMPUTER);
    }
}

/*
 * Function: findAllWords
 * -----------------
 * the meat of the computer turn
 * this finds all remaining words not found
 * by the user
 * works similarly to onBoardFromPosition(...)
 * instead of returning when a word is found
 * it explores all word paths that have potential
 * and populates a lexicon with all successes
 */
void findAllWords(int gridRow, 
                  int gridCol,
                  string &boardWord,
                  Grid<char> &boggleGrid,
                  Grid<bool> &usageGrid,
                  Lexicon &engWords,
                  Lexicon &allWords,
                  Lexicon &usedLex) {
    if (engWords.contains(boardWord) && (boardWord.size() >= FOUR_LETTERS) && (!usedLex.contains(boardWord))) {
        allWords.add(boardWord);
    }
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int newRow = gridRow + y;
            int newCol = gridCol + x;
            if (!inRange(newRow, newCol)) continue;
            if (usageGrid[newRow][newCol] == UNUSED) {
                boardWord.push_back(boggleGrid[newRow][newCol]);
                if (engWords.containsPrefix(boardWord)) {
                    usageGrid[newRow][newCol] = USED;
                    findAllWords(newRow, newCol, boardWord, boggleGrid, usageGrid, engWords, allWords, usedLex);
                    usageGrid[newRow][newCol] = UNUSED;
                }
                boardWord = boardWord.substr(0, boardWord.size() - 1);
            }
        }
    }
}

/*
 * Function: inRange
 * -----------------
 * returns true if y, x are in range
 * of the boggle board, false otherwise
 */
bool inRange(int y, int x) {
    if (y < 0 | y > (STANDARD_BOGGLE_EDGE - 1) |
        x < 0 | x > (STANDARD_BOGGLE_EDGE - 1) ) {
        return false;
    }
    return true;
}

/*
 * Function:getCleanUsageGrid
 * -----------------
 * returns a boolean grid with values all set
 * to UNUSED
 */
Grid<bool> getCleanUsageGrid(int numRows, int numCols) {
    Grid<bool> usageGrid(numRows, numCols);
    for(int row = 0; row < usageGrid.numRows(); row++) {
        for (int col = 0; col < usageGrid.numCols(); col++) {
            usageGrid[row][col] = UNUSED;
        }
    }
    return usageGrid;
}

/*
 * Function:getBoardGrid
 * -----------------
 * generates a grid which is the dual
 * of the current boggle board
 */
Grid<char> getBoardGrid(string boardString) {
    Grid<char> boardGrid(STANDARD_BOGGLE_EDGE, STANDARD_BOGGLE_EDGE);
    int k = 0;
    for (int i = 0; i < STANDARD_BOGGLE_EDGE; i++) {
        for (int j = 0; j < STANDARD_BOGGLE_EDGE; j++) {
            boardGrid[i][j] = boardString[k];
            k++;
        }
    }
    return boardGrid;
}


/*
 * Function: boggleProgram
 * -----------------
 * wraps all functions for playing Boggle!
 */
void boggleProgram(void) {
    initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    drawBoard(STANDARD_BOGGLE_EDGE, STANDARD_BOGGLE_EDGE);
    welcome();
    if (yesOrNo("Would you like instructions?")) giveInstructions();
    while (true) {
        string boggleString;
        if (yesOrNo("Would you like to input your own Boggle characters?")) {
            boggleString = manualBoggleGrid();
        } else {
            boggleString = autoBoggleGrid();
        }
        Grid<char> boardGrid = getBoardGrid(boggleString);
        Lexicon usedLex = humansTurn(boardGrid);
        computersTurn(boardGrid, usedLex);
        if (yesOrNo("Would you like to play again?")) {
            initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
            drawBoard(STANDARD_BOGGLE_EDGE, STANDARD_BOGGLE_EDGE);
        } else {
            break;
        }
    }
}

