#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

/* std */
#include <stdio.h>
#include <stdlib.h>

enum
{
  MIN_SNAKE_SIZE = 3,
  MAX_SNAKE_SIZE = 256,
  SNAKE_HEAD_SEG = 0x00,
  SNAKE_BODY_SEG = 0x01,
  GRID_PATTERN = 0x02,
  FRUIT_SCORE = 50,
};

typedef struct
{
  uint8_t x;
  uint8_t y;
} pos_t;

struct
{
  pos_t pos;
} apple;

struct
{
  pos_t pos[256];
  uint8_t size;
} snake;

int8_t dx = 0;
int8_t dy = 0;

static struct
{
  uint16_t score;
  uint16_t score_offset;
  int32_t timer;
  bool gameover;
  char str_buffer[5];
} state;

static inline bool check_overlap(const pos_t a, const pos_t b)
{
  return (a.x == b.x) && (a.y == b.y);
}

static inline bool check_overlaps(const pos_t a, const pos_t *b, size_t len)
{
  bool ret = false;
  for (int32_t i = 0; i < len; i++)
  {
    ret |= check_overlap(a, *(b + i));
  }
  return ret;
}

static inline void place_snake(void)
{
  snake.size = MIN_SNAKE_SIZE;
  for (int32_t i = 0; i < snake.size; i++)
  {
    snake.pos[i].x = 8;
    snake.pos[i].y = 8;
  }
}

static inline void place_apple(void)
{
  do
  {
    apple.pos.x = rand() % 0x0f;
    apple.pos.y = rand() % 0x0f;
  } while (check_overlaps(apple.pos, &snake.pos, snake.size));
}

void c8_load(void)
{
/* cel8 */
#include "embed/font.h"
#include "embed/palette.h"
  c8_init(&(c8_desc_t){
      .roms = {
          .chars = (c8_range_t){.ptr = font_h, .size = sizeof(font_h)},
          .palette = (c8_range_t){.ptr = palette_h, .size = sizeof(palette_h)},
      },
  });

  /* create sprites */
  /* snake head */
  /* 01111110  -- 0x7E */
  /* 11111111  -- 0xFF */
  /* 11011011  -- 0xDB */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 01111110  -- 0x7E */
  c8_poke4(C8_MEM_FONT_ADDR + 0x00 * 4, 0x7EFFDBFF);
  c8_poke4(C8_MEM_FONT_ADDR + 0x01 * 4, 0xFFFFFF7E);

  /* snake segment */
  /* 01111110  -- 0x7E */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 11111111  -- 0xFF */
  /* 01111110  -- 0x7E */
  c8_poke4(C8_MEM_FONT_ADDR + 0x08 + 0x00 * 4, 0x7EFFFFFF);
  c8_poke4(C8_MEM_FONT_ADDR + 0x08 + 0x01 * 4, 0xFFFFFF7E);

  /* grid pattern */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00011000  -- 0x18 */
  /* 00011000  -- 0x18 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  /* 00000000  -- 0x00 */
  c8_poke4(C8_MEM_FONT_ADDR + 0x10 * 4, 0x00000018);
  c8_poke4(C8_MEM_FONT_ADDR + 0x11 * 4, 0x18000000);

  place_snake();
  place_apple();

  /* game state */
  state.gameover = false;
  state.timer = 0;
}

static void move_snake(void)
{
  if (c8_btn(C8_INPUT_UP))
  {
    dx = 0, dy = -1;
  }
  else if (c8_btn(C8_INPUT_DOWN))
  {
    dx = 0, dy = +1;
  }
  else if (c8_btn(C8_INPUT_LEFT))
  {
    dx = -1, dy = 0;
  }
  else if (c8_btn(C8_INPUT_RIGHT))
  {
    dx = +1, dy = 0;
  }

  pos_t temp;
  pos_t prev = snake.pos[0];
  snake.pos[0].x += dx;
  snake.pos[0].y += dy;

  /* move snake segments */
  for (int i = 1; i < snake.size; i++)
  {
    temp = snake.pos[i];
    snake.pos[i] = prev;
    prev = temp;
  }

  /* check for collisions with body */
  for (int i = 1; i < snake.size; i++)
  {
    if (check_overlap(snake.pos[0], snake.pos[i]))
    {
      state.gameover = true;
    }
  }

  /* check for collision with apple */
  if (check_overlap(snake.pos[0], apple.pos))
  {
    /* increae snake size */
    snake.size++;
    snake.pos[snake.size - 1] = snake.pos[0];
    state.score += FRUIT_SCORE;

    place_apple();
  }
}

void c8_update(void)
{
  if ((--state.timer) <= 0)
  {
    state.timer = 5 - (snake.size / 12);
    if (state.timer < 2)
    {
      state.timer = 2;
    }
    move_snake();
  }
}

void c8_draw(void)
{
  c8_cls(0x00, 0x00);

  /* draw board */
  c8_color(0x01);
  c8_fill(0, 0, 16, 16, GRID_PATTERN);

  /* draw the score */
  int score = (state.score_offset < state.score) ? ++state.score_offset : state.score;
  sprintf(state.str_buffer, "%d", score);
  c8_color(0x06);
  c8_print(1, 1, "SCORE:");
  c8_print(8, 1, state.str_buffer);

  /* draw the apple and stem */
  c8_color(0x08);
  c8_print(apple.pos.x, apple.pos.y, "@");
  c8_color(0x0B);
  c8_print(apple.pos.x, apple.pos.y - 1, "c");

  /* draw the snake */
  c8_color(0x0C);
  c8_put(snake.pos[0].x, snake.pos[0].y, SNAKE_HEAD_SEG);
  for (int32_t i = 1; i < snake.size; i++)
  {
    c8_put(snake.pos[i].x, snake.pos[i].y, SNAKE_BODY_SEG);
  }
}