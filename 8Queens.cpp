#include <iostream>
#include <iomanip>
#include <unistd.h>

const int DIMENSION = 8;
bool DEBUG = false;
bool DEBUGTURNEDOFF = false;
bool boardPositions[DIMENSION * DIMENSION];
int colPositions[DIMENSION];
int rowPositions[DIMENSION];
int leftDiagnol[(2 * DIMENSION) - 1];
int rightDiagnol[(2 * DIMENSION) - 1];
int numCalls = 0;
int numSolutions = 0;

// Define all functions, prevents issues with forward declarations
void clearPosition(int row, int col);
void doDebug(char *msg, int row, int col);
void doWork(int currRow, int currCol);
void dumpPositions();
int getPosition(int row, int col);
bool goodBoard();
void init();
void setLeftDiagnol(int row, int col, int incValue);
void setRightDiagnol(int row, int col, int incValue);
void setPos(int row, int col, bool placingPiece);
void setPosition(int row, int col);
void writeSep();
void writeBoard();
void writePosValues();

// Clear the piece from the current position
void clearPosition(int row, int col) {
  setPos(row, col, false);
}

// Routine for debugging
void doDebug(char *msg, int row, int col) {
  std::cout << std::endl
            << "Debug: " << msg << " row: " << row << " col: " << col << std::endl;
  writeBoard();
  dumpPositions();
  writePosValues();
  std::cout << "goodBoard: " << (goodBoard() ? "true" : "false") << std::endl
            << "q (ENTER) quits debugger" << std::endl;
  char aChar;
  std::cin >> aChar;
  if (aChar == 'q') {
    DEBUG = false;
    DEBUGTURNEDOFF = true; // Don't want it to get turned back on (useful when board position triggerd debug on (see doWork))
  }
  // sleep(2);
}

// Workhorse for the 8Queens, you pass the row/col to process...
void doWork(int currRow, int currCol) {
  // Turn debugging on for a specific board layout... wanted to see how it proceeds from here
  if (boardPositions[0] && boardPositions[13] && boardPositions[23] &&
      boardPositions[26] && boardPositions[38] && boardPositions[43] &&
      !DEBUGTURNEDOFF)
    DEBUG = true;

  numCalls++;
  if ((numCalls % 500) == 0) {
    std::cout << "Number of calls: " << numCalls << " solutions: " << numSolutions << std::endl;
  }

  // If called with a row past our dimension we know the board currently has
  // a solution
  if (currRow > (DIMENSION - 1)) {
    std::cout << std::endl
              << "Solution: " << std::endl;
    numSolutions++;
    writeBoard();
    return;
  }

  // At end of row return
  if (currCol > (DIMENSION - 1)) {
    return;
  }

  setPosition(currRow, currCol);
  if (DEBUG) doDebug("after setPosition", currRow, currCol);

  if (goodBoard()) {
    doWork(currRow + 1, 0); // call to process next row
  }

  clearPosition(currRow, currCol);
  if (DEBUG) doDebug("after clearPosition", currRow, currCol);

  doWork(currRow, currCol + 1);
}

// Write out the positions in boardPositions that hold a queen
void dumpPositions() {
  std::cout << "boardPositions: ";
  for (int i = 0; i < (DIMENSION * DIMENSION); i++) {
    if (boardPositions[i])
      std::cout << i << ", ";
  }
  std::cout << std::endl;
}

// Helper - converts row/col to it's position in boardPositions (a 1 dimensional array)
int getPosition(int row, int col) {
  return (row * DIMENSION) + col;
}

// Return boolean that board is good, meaning the queens on it can't take each other
bool goodBoard() {
  bool isGood = true;
  int idx = 0;
  while ((idx < DIMENSION) && isGood) {
    isGood = (rowPositions[idx] < 2) &&  // 2 queens can't share a row
             (colPositions[idx] < 2) &&  // 2 queens can't share a column
             (leftDiagnol[idx]  < 2) &&  // 2 queens can't share the same left diagnol
             (rightDiagnol[idx] < 2);    // 2 queens can't be on the same right diagnol
    idx++;
  }

  // Check remaining diagnol positions (from DIMENSION->2*DIMENSION-1) 
  while ((idx < ((2 * DIMENSION) - 1)) && isGood) {
    isGood = (leftDiagnol[idx] < 2) &&
             (rightDiagnol[idx] < 2);
    idx++;
  }
  return isGood;
}

// Initialization - setup global arrays
void init() {
  int temp = 0;
  for (int i = 0; i < DIMENSION; i++) {
    // Set row/col arrays that hold positions used to 0
    colPositions[i] = 0;
    rowPositions[i] = 0;
    for (int j = 0; j < DIMENSION; j++) {
      // Set boardPositions to false
      boardPositions[temp] = false;
      // We'll initialize left/right diagnol arrays as well... but we only have
      // 2*DIMENSION - 1 values to set
      if (temp < ((2 * DIMENSION) - 1)) {
        leftDiagnol[temp] = 0;
        rightDiagnol[temp] = 0;
      }
      temp++;
    }
  }
}

// Helper to set value in left diagnol, what we do is incriment the position by one when
// a queen is on the diagnol and decrement by one when it's removed.  That way we know
// how many queens are on a specific diagnol.  The 'incValue' arg is either 1 or -1 here
// Diagnols are numbered counter clockwise from (0,0) to (dimension-1,dimension-1)
void setLeftDiagnol(int row, int col, int incValue) {
  int diagPos = row + col;
  leftDiagnol[diagPos] += incValue;
}

// Setup value for right diagnol, the diagnols are numbered counter clockwise from 
// position (dimension-1,0) to (0,dimension-1)
void setRightDiagnol(int row, int col, int incValue) {
  int diagPos = (DIMENSION - row - 1) + col;
  rightDiagnol[diagPos] += incValue;
}

// Set the state of the piece at the position passed in, if last arg is true
// then we're placing a queen at that position, otherwise we're removing one
void setPos(int row, int col, bool placingPiece) {
  int thePos = getPosition(row, col);     // map row/col to position in boardPositions
  int incValue = placingPiece ? 1 : -1;   // if placing piece we increment otherwise we decrement
  boardPositions[thePos] = placingPiece;  // set value in boardPositions
  setLeftDiagnol(row, col, incValue);     // set left diagnol
  setRightDiagnol(row, col, incValue);    // set right diagnol
  colPositions[col] += incValue;          // set number of queens in the specific column
  rowPositions[row] += incValue;          // set number of queens in the row
  return;
}

// Set a piece at the position passed in, we just call the helper with 'true' to identify putting
// piece on the board
void setPosition(int row, int col) {
  setPos(row, col, true);
}

// Little helper to write separator for 'writeBoard'
void writeSep() {
  for (int i = 0; i < DIMENSION; i++) {
    std::cout << "----";
  }
  std::cout << "-" << std::endl;
}

// Write out the current chess board in a 'nice' way :)
void writeBoard() {
  writeSep();
  // We want to output rows decending... that's how user would see the board
  for (int i = DIMENSION - 1; i >= 0; i--) {
    std::cout << "| ";
    for (int j = 0; j < DIMENSION; j++) {         
      std::cout << ((boardPositions[getPosition(i,j)]) ? "Q" : " ") << " | ";
    }
    std::cout << std::endl;
    writeSep();
  }
}

// Write out the number of queens in each of the positional arrays
void writePosValues() {
  std::cout << "rowPositions: ";
  for (int i = 0; i < DIMENSION; i++)
    std::cout << rowPositions[i] << ", ";

  std::cout << std::endl
            << "colPositions: ";
  for (int i = 0; i < DIMENSION; i++)
    std::cout << colPositions[i] << ", ";

  std::cout << std::endl
            << "leftDiagnol: ";
  for (int i = 0; i < ((2 * DIMENSION) - 1); i++)
    std::cout << leftDiagnol[i] << ", ";

  std::cout << std::endl
            << "rightDiagnol: ";
  for (int i = 0; i < ((2 * DIMENSION) - 1); i++)
    std::cout << rightDiagnol[i] << ", ";

  std::cout << std::endl;
}

// Mainline
int main(int argc, char *argv[]) {

  // Not related to 8 queens... just outputs arguments
  std::cout << "argc is " << argc << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << "argv[" << i << "]:" << argv[i] << std::endl;
  }

  std::cout << "Dimension of board is " << DIMENSION << " X " << DIMENSION << std::endl;

  init();        // Initialize 
  doWork(0, 0);  // Call routine to kick it off, we'll start at row 0, col 0 :)

  std::cout << "Number of calls: " << numCalls << " solutions: " << numSolutions << std::endl;
  return 0;
}