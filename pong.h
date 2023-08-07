#ifndef PONG_H
#define PONG_H

#include "display.h"
#include "analog.h"
#include "gfx.h"

struct Ball{
  
  int ballX, ballY, ballDirX, ballDirY;
  int ballSize;
  int upperBound, lowerBound, leftBound, rightBound;
  
}ball;

struct Paddle{
  
  uint8_t id;
  int paddle_posY,paddle_posX;
  int upperBound,lowerBound;
  int baseline;
  int paddle_dir;
  int paddle_size;
  int thickness;
  uint8_t score;
  
}paddle_1,paddle_2;


void initialBall();

void calPaddleBoundary(struct Paddle *p);

void calBallBoundary(struct Ball *b);

void initialPong();

uint8_t checkCollision(struct Ball *b, struct Paddle *p)

void checkFieldBoundary(struct Ball *b);

void readJoystick();

void update();

void showField();

void checkScore();

void startPong();


#endif