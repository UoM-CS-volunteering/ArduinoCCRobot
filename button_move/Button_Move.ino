/*Author: Ze Feng Huang and Taonga Chabva-Shoperai
  Date: 13/02/19
  This program takes an input of movements from the buttons on an Aduino. 
  CC/Robot. The top button make it move forwards for a second with the bottom 
  making it reverse for the same duration. The right button makes it rotate
  90 degress on the spot from which ever way its orientated and the left
  button make it rotate -90 degrees. The middle button brings up a menu 
  that allow you to run the set of instructions by pressing it again or
  allows you to delete your last move by pressing the bottom button.
  A list of moves in there running order is shown left to right, top
  to bottom on the screen as you enter each move.
  
  Useful values to be considered when reading and/or editing the code.
  BUTTON_MIDDLE = 0
  BUTTON_LEFT = -1
  BUTTON_NONE = 3
  BUTTON_RIGHT = 2
  BUTTON_UP = 1
  BUTTON_DOWN = 4
  Screen Dimension 
  X = 120,
  Y = 150
*/

#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

//Stores the list of moves in order for execution
int moves[100];
//index of the next move to store
int moves_index = 0;

void setup() {
  Robot.begin();
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.text("Yeah Aight", 0, 0);
  //The constants for the buttons take the values -1 through to 4 so
  //initialise it to any other integer value to represent NULL.
  for (int index = 0; index < 100; index ++)
  {
    moves[index] = 10;
  }
}

void loop()
{
  Robot.fill(255,255,255);
  //Adds the next move into the moves array
  recordKey(Robot.keyboardRead());
}

//Adds the next move into the moves array
void recordKey(int key) {
  switch (key) {
    //Calls the middle button menu if the middle button is pressed
    case BUTTON_MIDDLE:
      menu();
      break;
    //If no button is pressed then do nothing
    case BUTTON_NONE:
      break;
    //Store which direction was pressed, incriment the moves_index and
    //display the new set of moves
    default:
      moves[moves_index++] = key;
      disp();
      break;
  }
}

//Display the current contents of the moves array
void disp()
{
  Robot.clearScreen();
  //Integers to store the x and y coordinates of the next instruction to be printed
  int x = 5;
  int y = 5;
  //For loop  goes through every enry in moves and if its is not 10 prints out
  //and "arrow" signifying the direction it should go in
  for (int index = 0; index < 100; index ++)
  {
    //Makes sure the prints to the screen dont go beyond its size
    if (x > 120)
    {
      x = 5;
      y += 10;
    }  
    //Chooses the correct the arrow to draw.
    if (moves[index] != 10);
    {
      switch (moves[index]) {
        case BUTTON_LEFT:
          Robot.text("<", x, y);
          break;
        case BUTTON_RIGHT:
          Robot.text(">", x, y);
          break;
        case BUTTON_UP:
          Robot.text("^", x, y);
          break;
        case BUTTON_DOWN:
          Robot.text("v", x, y);
          break;
      }
      //Move the x so the next print is to the right
      x += 10;
    }  
  }
}

//Menu used to decided if the user wants to delete their last move or 
//start the robot
void menu()
{
  //clear the screen for the new text
  Robot.clearScreen();
  //Print a prompt to ask them what move they should make
  Robot.text("Press center to go", 5, 5);
  Robot.text("\n Or press down to \n erase last command", 5, 15);
  //int to hold key pressed in this menu
  int key;
  //Runs until the user presses the correct button to leave the menu
  while (true)
  {
    key = Robot.keyboardRead();
    //If they press the middle button again start the robot.
    if (key == BUTTON_MIDDLE)
    {
      runProgram();
      break;
    }   
    //If they press the down button delete the latest entry in the moves.
    else if (key == BUTTON_DOWN)
    {
      moves_index--;
      moves[moves_index] = 10;
      break;
    }
  }
  //redisplay after the action
  disp();
}

//Runs the programmed instructions
void runProgram()
{
  //clear the screen for the count down to run
  Robot.clearScreen();
  //Show the count down on the screen
  for (int count = 5; count > 0; count--)
  {
    Robot.text(count, 5, 5);
    //delay for a second
    delay(1000);
    Robot.clearScreen();
  };
  //Go through each instruction until the array is finished or the 
  //program is over and perform each move
  for (int index = 0; index < 100; index ++)
  {
    //If this index is 10 then the program is over.
    if (moves[index] == 10)
      break;
    //move for that instruction
    moveUnit(moves[index]);
  }
  
  //Empty the moves array for it to be reprogrammed
  for (int index = 0; index < 100; index ++)
  {
    moves[index] = 10;
  }
  //Reset the moves index to restart it
  moves_index = 0;
}

//Moves the robot for each instruction
void moveUnit(int movement)
{
  switch (movement) {
    case BUTTON_LEFT:
      Robot.turn(-90);
      break;
    case BUTTON_RIGHT:
      Robot.turn(90);
      break;
    case BUTTON_UP://Go forwards 
      Robot.motorsWrite(100, 100);
      //Run for 1 second then stop the action
      delay(1000);
      break;
    case BUTTON_DOWN://Reverse
      Robot.motorsWrite(-100, -100);
      //Run for 1 second then stop the action
      delay(1000);
      break;
  }
  Robot.motorsStop();
}
