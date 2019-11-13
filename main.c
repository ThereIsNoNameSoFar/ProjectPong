//Wykonał Michał Warzecha I rok Informatyka EAIIB grupa 6b
#include <stdlib.h>
#include <time.h>
#include "pong.h"


void Start(void);
void FixedUpdate(void);
void Update(void);

void Start()
{
  isPlay = 0;
  isEnd = 0;
  round_counter = 0;
  winner = -1;
  player_points = 0;
  opponent_points = 0;
}

void FixedUpdate()
{
  if(isPlay)
  {
    GamePhysics(&PONG, &PADDLE, &PADDLE_AI, &TABLE);
  }
  else
  {
    if((round_counter/2)%2==0)
    {
      winner = -1;
      PONG.x = PADDLE.x;
      PONG.y = PADDLE.y+1;
      PONG.vel_x = X_TO_SCREEN_SPACE(SERV_SPEED);
    }
    else if((round_counter/2)%2==1)
    {
      winner = 1;
      PONG.x = PADDLE_AI.x;
      PONG.y = PADDLE_AI.y+1;
      PONG.vel_x = X_TO_SCREEN_SPACE(-SERV_SPEED);
    }
    PONG.vel_y = -3;
  }
  draw_UI();
}

void Update()
{
  draw_table(&TABLE);
  draw_paddle(&PADDLE);
  draw_paddle(&PADDLE_AI);
  draw_pong(&PONG);
  paddle_movement(&PADDLE);
  check_keys();
}

int CheckDeltaTime(double* delta, double* last_time, double* current_time, struct timespec* tstart)
{
  clock_gettime(CLOCK_MONOTONIC, tstart);
  *current_time = ((double)(*tstart).tv_sec + 1.0e-9*(*tstart).tv_nsec);
  *delta = *current_time - *last_time;
  if(*delta > DELAY)
  {
    *last_time = *current_time;
    return 1;
  }
  return 0;
}

void RunGame()
{
  struct timespec tstart={0,0};
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  double delta = 0;
  double last_time = ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
  double current_time = last_time;

  Start();
  while(1)
  {
//    clear();
    Update();
    if(CheckDeltaTime(&delta, &last_time, &current_time, &tstart))
    {
      FixedUpdate();
    }
    refresh();
  }
}

int main()
{
  Initialise();
  RunGame();

  endwin();
  return 0;
}
