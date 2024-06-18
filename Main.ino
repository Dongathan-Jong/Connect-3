

// include libraries
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

// define pins
#define ledStrip 6
#define leftButton 3
#define middleButton 4
#define rightButton 5

// create and initialize booleans
bool player1Won = false;
bool player2Won = false;
bool player1Turn = true;
bool player2Turn = false;
bool emptySpace = true;
bool startGame = true;

// create and initialize integers
int replaceSlot;
int current1stSlot = 0;
int current2ndSlot = 0;
int draw = 0;

// create arrays
int winCase[3];
int slots[3][4];
int slotSelection[4];

// state lcd and leds
LiquidCrystal_I2C lcd(0x20, 16, 2);
Adafruit_NeoPixel pixels(12, 6, NEO_GRB + NEO_KHZ800);

/*
setup()

Use: startup for the program
*/
void setup(){
  // set each input and outputs pinmode
  pinMode(leftButton, INPUT);
  pinMode(middleButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(ledStrip, OUTPUT);
  
  // start the lcd and leds
  lcd.init();
  lcd.backlight();
  pixels.begin();
}

/*
loop()

Use: main loop of the program
*/
void loop(){
  if(startGame){ // check if its the start of the game
    //clear the lcd and welcome user
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Welcome to");
  	lcd.setCursor(0,1);
  	lcd.print("Connect 3!");
  	delay(500);
  	lcd.clear();
    startGame = false;
    
    // reset each pixel in case game was already played
    for(int i = 0; i < 3;i++){
    	for(int j = 0; j < 4;j++){
      	  slots[i][j] = 0;
          pixels.setPixelColor(i*4 + j, pixels.Color(0, 0, 0));
          pixels.show();
    	}
  	}
    updateBoard();
  }
  
  // run if players did not win and doesnt draw and enter function for each player
  if(player1Won == false || player2Won == false || draw < 12){
    if(player1Turn){
      player1();
      checkLink();
    }
    else if(player2Turn){
      player2();
      checkLink();
    }
  }
  
  // check if the game is drawn and reset game
  if(draw == 12){
    lcd.clear();
    lcd.print("Draw!!!");
    startGame = true;
    draw = 0;
    delay(300);
  }
  
  // check if the first player won
  if(player1Won){
    // tell users and show winning chain
    lcd.clear();
    lcd.print("Player 1 wins!");
    pixels.clear();
      for(int j = 0; j < 3; j++){
        pixels.setPixelColor(winCase[j], pixels.Color(0, 150, 0));
 	    pixels.show();
      }
      delay(700);
  }
  // check if the second player won
  if(player2Won){
    // tell the users and show winning chain
    lcd.clear();
    lcd.print("Player 2 wins!");
    pixels.clear();
      for(int j = 0; j < 3; j++){
        pixels.setPixelColor(winCase[j], pixels.Color(150, 0, 0));
 	    pixels.show();
      }
      delay(700);
  }
  
  // check if anyone has won
  if(player1Won || player2Won){
    // reset all variables for the next time
    startGame = true;
    player1Won = false;
    player2Won = false;
    player1Turn = true;
    player2Turn = false;
    current1stSlot = 0;
    current2ndSlot = 0;
    draw = 0;
  }
}

/*
player1()

Use: main method for player 1
*/
void player1(){
  // set lcd screen
  lcd.print("Player 1 turn:");
  lcd.setCursor(0,0);
  
  // change the top left pixel colour
  pixels.setPixelColor(current2ndSlot, pixels.Color(0, 150, 0));
  pixels.show();
  
  // store current pixel position
  slotSelection[current2ndSlot] = 1;
    
  // if the right button is pressed change the pixel to one slot right
  if(digitalRead(rightButton) == 1){
    if(current2ndSlot < 3){
      slotSelection[current2ndSlot] = 0;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0,0, 0));
      pixels.show();
      current2ndSlot++;
      slotSelection[current2ndSlot] = 1;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0, 150, 0));
      pixels.show();
      updateBoard();
    }
  }
  
  // if the left button is pressed change the pixel to one slot left
  if(digitalRead(leftButton) == 1){
    if(current2ndSlot > 0){
      slotSelection[current2ndSlot] = 0;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0, 0, 0));
      pixels.show();
      current2ndSlot--;
      slotSelection[current2ndSlot] = 1;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0, 150, 0));
      pixels.show();
      updateBoard();
    }
  }
  
  // if the middle button is pressed reset variables and set other players turn
  if(digitalRead(middleButton) == 1){
    dropPiece();
    player1Turn = false;
  	player2Turn = true;
    current1stSlot = 0;
    current2ndSlot = 0;
    emptySpace = true;
    updateBoard();
  } 
}

/*
player2()

Use: main method for player 2
*/
void player2(){
  // set lcd screen
  lcd.print("Player 2 turn:");
  lcd.setCursor(0,0);
  
  // set top left pixel to red 
  pixels.setPixelColor(current2ndSlot, pixels.Color(150, 0, 0));
  pixels.show();
  
  // store current selection
  slotSelection[current2ndSlot] = 2;
    
  //if right button is pressed move current pixel one pixel right
  if(digitalRead(rightButton) == 1){
    if(current2ndSlot < 3){
      slotSelection[current2ndSlot] = 0;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0, 0, 0));
      pixels.show();
      current2ndSlot++;
      slotSelection[current2ndSlot] = 2;
      pixels.setPixelColor(current2ndSlot, pixels.Color(150, 0, 0));
      pixels.show();
      updateBoard();
    }
  }
  // if left button is pressed move current pixel one pixel left
  if(digitalRead(leftButton) == 1){
    if(current2ndSlot > 0){
      slotSelection[current2ndSlot] = 0;
      pixels.setPixelColor(current2ndSlot, pixels.Color(0, 0, 0));
      pixels.show();
      current2ndSlot--;
      slotSelection[current2ndSlot] = 2;
      pixels.setPixelColor(current2ndSlot, pixels.Color(150, 0, 0));
      pixels.show();
      updateBoard();
    }
  }
  // if middle button is pressed reset varibles for next user
  if(digitalRead(middleButton) == 1){
    dropPiece();
    player1Turn = true;
  	player2Turn = false;
    current1stSlot = 0;
    current2ndSlot = 0;
    emptySpace = true;
    updateBoard();
  }
}

/*
updateBoard()

Use: updates the pixels from the slots[][] array
*/
void updateBoard(){
  // for each pixel slot update the colour of each pixel to the respective colour
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 4; j++){
      if(slots[i][j] == 1){
        pixels.setPixelColor(i*4 + j, pixels.Color(0, 150, 0));
        pixels.show();
      }
      else if(slots[i][j] == 2){
        pixels.setPixelColor(i*4 + j, pixels.Color(150, 0, 0));
        pixels.show();
      }
    }
  }
}


/*
dropPiece()

Use: drops the piece when the user clicks the middle button
*/
void dropPiece(){
  for(int i = 2; i >= 0; i--){
    // check if the slot below it is empty and update the piece in the array
  	if(slots[current1stSlot + i][current2ndSlot] == 0 && emptySpace){
      if(player1Turn){
        slots[current1stSlot][current2ndSlot] = 0;
      	slots[current1stSlot + i][current2ndSlot] = 1;
        pixels.clear();
        updateBoard();
        draw++;
      }
      else if(player2Turn){
        slots[current1stSlot][current2ndSlot] = 0;
        slots[current1stSlot + i][current2ndSlot] = 2;
        pixels.clear();
        updateBoard();
        draw++;
      }
      emptySpace = false;
    }
  }
  lcd.clear();
}










