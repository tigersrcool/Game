Game
====
Object of the game is to move the asterisk to the lower right corner of the LCD screen. If two seconds is exceeded between button pushes, the game will end.
Functions
====
unsigned char initPlayer();
  set location of the first asterisk
void printPlayer(unsigned char player);
  sets asterisk to the location on the screen known as player
void clearPlayer(unsigned char player);
  clears asterisk from place on LCD screen
unsigned char movePlayer(unsigned char player, unsigned char direction);
  updates asterisk position based on direction passed in
char didPlayerWin(unsigned char player);
  checks whether the player reached the lower right corner
void testAndRespondToButtonPush(char buttonToTest);
  tests whether a button is pushed and moves asterisk based on pushed button
Interrupts
====
Timer Interrupt
  as soon as a button is pushed, counts up to 2 seconds; if button is pushed while counting it clears; if button isn't pushed int ENDGAME=1
Button Interrupt
  uses testAndRespondToButtonPush function and interrupts if a button is pushed
License
====
Anyone is free to use and modify this for any purpose


