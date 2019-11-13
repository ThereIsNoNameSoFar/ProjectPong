//Wykonał Michał Warzecha I rok Informatyka EAIIB grupa 6b
#include "pong.h"

_pong_ PONG = {
  .x = 0,
  .y = 0,
  .prev_x = 0,
  .prev_y = 0,
  .vel_x = 0,
  .vel_y = 0,
  .acc_x = 0,
  .acc_y = 0,
  .gravity = 7,
  .rezistance = 0.9,
  .bouncines = 1
};

_paddle_ PADDLE = {
  .x = 0,
  .y = 12,
  .prev_x = 0,
  .prev_y = 0,
  .max_y = 20,
  .min_y = 10,
  .height = 3
};

_paddle_ PADDLE_AI = {
  .x = X_TO_SCREEN_SPACE(30),
  .y = Y_TO_SCREEN_SPACE(12),
  .prev_x = 0,
  .prev_y = 0,
  .max_y = 20,
  .min_y = 10,
  .height = 3
};

_table_ TABLE = {
  .min_x = 0,
  .min_y = 0,
  .max_x = X_TO_SCREEN_SPACE(30.0),
  .max_y = Y_TO_SCREEN_SPACE(20),
  .left_net = X_TO_SCREEN_SPACE(15.0) - 1,
  .right_net = X_TO_SCREEN_SPACE(15.0) + 1,
  .net_hight = Y_TO_SCREEN_SPACE(2)
};

void pong_physics(_pong_* PONG);
void paddle_collision(_paddle_* PADDLE, _pong_* PONG, int side);
void net_collision(_pong_* PONG, _table_* TABLE);
void table_collision(_pong_* PONG, _table_* TABLE);
void ai_controll(_pong_* PONG, _paddle_* PADDLE_AI);

const wchar_t* block_char = L"\x2588";
const char pong_char = 'o';

void Initialise(void)
{
  setlocale(LC_ALL, "");
  initscr();
  raw();
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  curs_set(FALSE);
}

void GamePhysics(_pong_* PONG, _paddle_* PADDLE, _paddle_* PADDLE_AI, _table_* TABLE)
{
  paddle_collision(PADDLE, PONG, 1);
  paddle_collision(PADDLE_AI, PONG, -1);
  net_collision(PONG, TABLE);
  table_collision(PONG, TABLE);
  pong_physics(PONG);
  
  paddle_movement(PADDLE);
  ai_controll(PONG, PADDLE_AI);
}

void pong_physics(_pong_* PONG)
{
  PONG->rezistance = pow(0.9, DELAY);
  PONG->acc_y += PONG->gravity;
  PONG->vel_x += PONG->acc_x*SPEED;
  PONG->vel_y += PONG->acc_y*SPEED;
  PONG->vel_x *= PONG->rezistance;
  PONG->vel_y *= PONG->rezistance;
  PONG->x += PONG->vel_x*SPEED*2;
  PONG->y += PONG->vel_y*SPEED*2;
  PONG->acc_x *= 0;
  PONG->acc_y *= 0;
}

void paddle_movement(_paddle_* PADDLE)
{
  PADDLE->y += check_keys()[1];
  correct_paddle(PADDLE); 
}

void net_collision(_pong_* PONG, _table_* TABLE)
{
  if(((int)PONG->x==TABLE->left_net || ((int)PONG->x==TABLE->left_net+1 && PONG->vel_x>0))
     &&(PONG->y>=(int)TABLE->max_y-1 && PONG->y<=(int)TABLE->max_y+1))
  {
    PONG->vel_x = -abs(PONG->vel_x)*PONG->bouncines;
  }
  if(((int)PONG->x==TABLE->right_net || ((int)PONG->x==TABLE->right_net-1 && PONG->vel_x<0))
     &&(PONG->y>=(int)TABLE->max_y-1 && PONG->y<=(int)TABLE->max_y+1))
  {
    PONG->vel_x = abs(PONG->vel_x)*PONG->bouncines;
  }
}

void table_collision(_pong_* PONG, _table_* TABLE)
{
  if(PONG->x<TABLE->min_x || PONG->x>TABLE->max_x)
  {
    GameOver(winner);
  }
  if(PONG->y<TABLE->min_y)
  {
    PONG->vel_y = abs(PONG->vel_y)*PONG->bouncines;
    PONG->y = TABLE->min_y;
  }
  if(PONG->y>TABLE->max_y)
  {
    if(PONG->x>=TABLE->right_net)
    {
      if(winner == 1)
      {
        GameOver(winner);
      }
      else if(winner == -1)
      {
        winner = 1;
      }
    }
    if(PONG->x<TABLE->right_net-1)
    {
      if(winner == -1)
      {
        GameOver(winner);
      }
      else if(winner == 1)
      {
       winner = -1;
      }
    }
    PONG->vel_y = (-abs(PONG->vel_y)*PONG->bouncines);
    PONG->y = TABLE->max_y;
  }
}

void paddle_collision(_paddle_* PADDLE, _pong_* PONG, int side)
{
  if(PONG->y>=(int)PADDLE->y && PONG->y<((int)PADDLE->y+PADDLE->height+1))
  {
    if(side == 1 && PONG->x<PADDLE->x)
    {
      if(winner == 1)
      {
        GameOver(winner);
      }
      PONG->x = PADDLE->x;
      PONG->vel_x = X_TO_SCREEN_SPACE(PADDLE_BOUNCE_SPEED);
      PONG->vel_y = Y_TO_SCREEN_SPACE(-5+PONG->y-PADDLE->y);
    }
    else if(side == -1 && PONG->x>PADDLE->x)
    {
      if(winner == -1)
      {
        GameOver(winner);
      }
      PONG->x = PADDLE->x;
      PONG->vel_x = X_TO_SCREEN_SPACE(-PADDLE_BOUNCE_SPEED);
      PONG->vel_y = Y_TO_SCREEN_SPACE(-5+PONG->y-PADDLE->y);
    }
  }
}

void draw_pong(_pong_* PONG)
{
  mvprintw((int)PONG->prev_y,(int)PONG->prev_x," ");
  mvprintw((int)PONG->y,(int)PONG->x,"%c",pong_char);
  PONG->prev_x = PONG->x;
  PONG->prev_y = PONG->y;
}

void draw_paddle(_paddle_* PADDLE)
{
  for(int i=0;i<PADDLE->height;++i)
  {
    mvprintw((int)PADDLE->prev_y+i,(int)PADDLE->prev_x," ");
  }
  for(int i=0;i<PADDLE->height;++i)
  {
    mvprintw((int)PADDLE->y+i,(int)PADDLE->x,"|");
  }
  PADDLE->prev_x = PADDLE->x;
  PADDLE->prev_y = PADDLE->y;
}

void draw_table(_table_* TABLE)
{
  for(int i=0;i<=TABLE->max_x+1;++i)
  {
    if(i==TABLE->right_net-1)
    {
      for(int j=0;j<TABLE->net_hight;++j)
      {
        mvprintw(TABLE->max_y-j,i,"|");
      }
    }
	
    mvaddwstr(TABLE->max_y+1,TABLE->max_x-i,block_char);
    mvaddwstr(8,i,block_char);
  }
}

int ai_delta = 0;

void ai_controll(_pong_* PONG, _paddle_* PADDLE_AI)
{
  ++ai_delta;
  if(ai_delta<10)
  {
    return;
  }
  ai_delta = 0;
  if((int)PONG->y > (int)PADDLE_AI->y)
  {
    PADDLE_AI->y += AI_MOV_VALUE;
  }
  if((int)PONG->y < (int)PADDLE_AI->y)
  {
    PADDLE_AI->y += -AI_MOV_VALUE;
  }
  correct_paddle(PADDLE_AI);
}

void correct_paddle(_paddle_* PADDLE)
{
  if(PADDLE->y > PADDLE->max_y)
  {
    PADDLE->y = PADDLE->max_y;
  }
  if(PADDLE->y < PADDLE->min_y)
  {
    PADDLE->y = PADDLE->min_y;
  }
}

void draw_UI(void)
{
  char* line_to_print = "                                                            ";

  mvprintw(5,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);	
  mvprintw((int)TABLE.max_y/2,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);

  if(opponent_points >= 11 && (opponent_points-player_points) >= 2)
  {
    isPlay = 0;
    isEnd = 1;
    round_counter = 0;
	
    line_to_print = "Przegrales";
    mvprintw(5,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
	
    line_to_print = "Przycisnij spacje by zagrac ponownie";
    mvprintw((int)TABLE.max_y/2,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  }
  else if(player_points >= 11 && (player_points-opponent_points) >= 2)
  {
    isPlay = 0;
    isEnd = 1;
    round_counter = 0;
	
    line_to_print = "Wygrales";
    mvprintw(5,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
	
    line_to_print = "Przycisnij spacje by zagrac ponownie";
    mvprintw((int)TABLE.max_y/2,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  }
  else if(isPlay==0)
  {
    if((round_counter/2)%2==0)
    {
      line_to_print = "Przycisnij spacje by zaserwowac";
    }
    else if((round_counter/2)%2==1)
    {
      line_to_print = "Przycisnij spacje by przeciwnik zaserwowal";
    }
    mvprintw((int)TABLE.max_y/2,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  }
  
  line_to_print = " _   _        _ ";
  mvprintw(0,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  line_to_print = "|_| | | |\\ | |  ";
  mvprintw(1,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  line_to_print = "|   |_| | \\| |_|";
  mvprintw(2,(int)(TABLE.max_x/2)-(int)(strlen(line_to_print)/2),"%s",line_to_print);
  
  mvprintw(TABLE.max_y/2-6,TABLE.max_x/2-20,"    ");
  mvprintw(TABLE.max_y/2-6,TABLE.max_x/2+20,"    ");
  mvprintw(TABLE.max_y/2-6,TABLE.max_x/2-20,"%d",player_points);
  mvprintw(TABLE.max_y/2-6,TABLE.max_x/2+20,"%d",opponent_points);
}

void GameOver(int winner)
{
  isPlay = 0;
  ++round_counter;
  PADDLE_AI.y = rand()%5+11;
  if(winner == -1)
  {
    ++opponent_points;
  }
  else if(winner == 1)
  {
    ++player_points;
  }
}

int direction[2];

int* check_keys(void)
{
  memset(direction, 0, 2*sizeof(int));
  char input = getch();
  if (input == 'w') 
  {
    direction[1] = -1;
  }
  if (input == 's') 
  {
    direction[1] = 1;
  }
  if (input == 'a') 
  {
    direction[0] = -1;
  }
  if (input == 'd') 
  {
    direction[0] = 1;
  }
  if (input == ' ') 
  {
    isPlay = 1;
    if(isEnd)
    {
      player_points = 0;
      opponent_points = 0;
      isEnd = 0;
    }
  }
  if (input == 'q') 
  {
    endwin();
    exit(0);
  }
  return direction;
}
