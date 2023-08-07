#include "pong.h"

void initialBall(){
    ball.ballX = 240;
    ball.ballY = 160;
    ball.ballDirX = 5
    ball.ballDirY = 0;
    ball.ballSize = 5;
}

void calPaddleBoundary(struct Paddle *p){
    p->upperBound = p->paddle_posY+p->paddle_size;
    p->lowerBound = p->paddle_posY-p->paddle_size;
    if(p->upperBound > 320)
      p->paddle_posY = 320 - p->paddle_size;
    else if(p->lowerBound < 0)
      p->paddle_posY = 0 + p->paddle_size;
}

void calBallBoundary(struct Ball *b){
    b->upperBound = b->ballY + b->ballSize;
    b->lowerBound = b->ballY - b->ballSize;
    b->leftBound = b->ballX - b->ballSize;
    b->rightBound = b->ballX + b->ballSize;
    checkFieldBoundary(b);
}
void initialPong(){
    initialBall();
    calBallBoundary(&ball);
    
    paddle1.paddle_posX = 0;
    paddle1.paddle_posY = 160;
    paddle1.paddle_dir = 0;
    paddle1.paddle_size = 10;
    paddle1.thickness = 5;
    paddle1.id = 1;
    paddle1.baseline = paddle1.paddle_posX + paddle1.thickness;
    paddle1.score = 0;
    calPaddleBoundary(&paddle1);
    
    paddle2.paddle_posX = 479;
    paddle2.paddle_posY = 160;
    paddle2.paddle_dir = 0;
    paddle2.paddle_size = 10;
    paddle2.thickness = 5;
    paddle2.id = 0;
    paddle2.baseline = paddle2.paddle_posX - paddle2.thickness;
    paddle2.score = 0;
    calPaddleBoundary(&paddle2);
       
    printPattern(BLACK, WHITE);
}

uint8_t checkCollision(struct Ball *b, struct Paddle *p){
  
    uint8_t rtn = 0;
    if(b->upperBound < p->upperBound && b->lowerBound > p->lowerBound){
      rtn = 1;
      b->ballDirY = p->paddle_dir;
      b->ballDirX = - b->ballDirX;
    }
  
    return rtn;
}

void checkFieldBoundary(struct Ball *b){

  if(b->upperBound >= 320){
    b->ballY = 320 - b->ballSize;
    b->ballDirY = - b->ballDirY;
  }
  else if (b->lowerBound <= 0){
    b->ballY = 0 + b->ballSize;
    b->ballDirY = -b->ballDirY;
  }
}

void readJoystick() {
    uint8_t analog11;
    uint8_t analog12;
    
    uint8_t analog13;
    uint8_t analog16;
    uint8_t analog19;
    uint8_t analog23;
    
    uint8_t analog17;
    cppp_getAnalogValues(&analog11, &analog12, &analog13, &analog16, &analog17, &analog19, &analog23);
    const uint32_t sleepTime = 10000; // 0,01s


    // joystick left
    if (analog16 >= 220) {
        paddle1.paddle_dir = 5;
    }
    // joystick middle
    else if (analog16 < 220 && analog16 >= 180) {
        paddle1.paddle_dir = 0;
    }
    // joystick right
    else if (analog16 < 180) {
        paddle1.paddle_dir = -5;
    }
    
    if (analog13 >= 220) {
        paddle2.paddle_dir = -5;
    }
    // joystick middle
    else if (analog13 < 220 && analog13 >= 180) {
        paddle2.paddle_dir = 0;
    }
    // joystick right
    else if (analog13 < 180) {
        paddle2.paddle_dir = 5;
    }
    
    cppp_microDelay(sleepTime);
}

void update(){
    const uint32_t sleepTime = 100000; // 0,1s
    ball.ballX = ball.ballX + ball.ballDirX;
    ball.ballY = ball.ballY + ball.ballDirY;
    
    paddle1.paddle_posY = paddle1.paddle_posY + paddle1.paddle_dir;
    calPaddleBoundary(&paddle1);
    paddle2.paddle_posY = paddle2.paddle_posY + paddle2.paddle_dir;
    cppp_microDelay(sleepTime);
}

void showField(){

    cppp_fillCircle(ball.ballX, ball.ballY, ball.ballSize, YELLOW);
    
    cppp_fillRect(paddle1.paddle_posX, paddle1.paddle_posY - paddle1.paddle_size, paddle1.thickness, 2*paddle1.paddle_size, WHITE);
    
    cppp_fillRect(paddle2.paddle_posX-paddle2.thickness, paddle2.paddle_posY - paddle2.paddle_size, paddle2.thickness, 2*paddle1.paddle_size, WHITE);
    
} 

void checkScore(){
  
    if(ball.leftBound<paddle1.baseline){     
      if(!checkCollision(&ball, &paddle1)){
        paddle2.score +=1;
        initialBall();     
      }   
    }
    else if (ball.leftBound>paddle2.baseline){
      if(!checkCollision(&ball, &paddle2)){
        paddle1.score +=1;
        initialBall();
      }   
    }
}

void startPong(){
  const uint32_t sleepTime = 100000; // 0,1s
  initialPong();
  while(1){
    readJoystick();
    update();
    calBallBoundary(&ball);
    calPaddleBoundary(&paddle1);
    calPaddleBoundary(&paddle2);
    checkScore();
    showField();
    cppp_microDelay(sleepTime);      
  }  
}