#define C8_IMPL
#include "cel8.h"

#define C8_PLATFORM_SOKOL
#include "cel8-sokol.c"

/* std */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
  TETRIS_FIELD_WIDTH = 8,
  TETRIS_FIELD_HEIGHT = 16,
  TETRIS_PIECE_COUNT = 7,
  TETRIS_CLEAR_LINE_TICKS = 25,
};

uint8_t color_table[] = {
    0x90,
    0xC0,
    0xA0,
    0xF0,
    0xE0,
    0x80,
    0xD0,
    0x70,
};

float gravity_table[] = {
    0.01667f,
    0.021017f,
    0.026977f,
    0.035256f,
    0.04693f,
    0.06361f,
    0.0879f,
    0.1236f,
    0.1775f,
    0.2598f,
    0.388f,
    0.59f,
    0.92f,
    1.46f,
    2.36f,
};

uint8_t tetromino[7][16] = {
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

uint16_t b_tetromino[7] = {
    0b0010001000100010,
    0b0010011000100000,
    0b0000011001100000,
    0b0010011001000000,
    0b0100011000100000,
    0b0100010001100000,
    0b0010001001100000,
};

typedef struct
{
  int8_t x, y, r;
} tetromino_t;

struct
{
  uint64_t ticks;
  uint16_t score;
  uint16_t score_offset;
  uint8_t level;
  uint8_t lines;
  int8_t fx_ticks;

  bool should_clear;
  bool gameover;

  char str_buffer[5];

  int cx, cy, cr;
  int cp, np;
  int8_t grid[16][8];
} state;

static int rotate(int x, int y, int r)
{
  switch (r % 4)
  {
  case 0:
    return y * 4 + x;
  case 1:
    return 12 + y - (x * 4);
  case 2:
    return 15 - (y * 4) - x;
  case 3:
    return 3 - y + (x * 4);
  default:
    return 0;
  }
}

static bool does_piece_fit(int i, int r, int px, int py)
{
  uint8_t *t = tetromino[i];
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (t[rotate(x, y, r)] == 0)
      {
        continue;
      }
      if ((px + x) >= 0 && (px + x) < 8 && (py + y) >= 0 && (py + y) < 16)
      {
        if (state.grid[y + py][x + px] != 0)
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

static void drop_piece(void)
{
  while (does_piece_fit(state.cp, state.cr, state.cx, state.cy + 1))
  {
    state.cy++;
  }
}

static void draw_tetromino(int p, int x, int y, int r)
{
  c8_color(color_table[p]);
  for (int j = 0; j < 4; j++)
  {
    for (int i = 0; i < 4; i++)
    {
      if (tetromino[p][rotate(i, j, r)] == 0)
      {
        continue;
      }
      c8_put(i + x, j + y, 0x00);
    }
  }
}

static void lock_piece(void)
{
  uint8_t *t = tetromino[state.cp];
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 4; x++)
    {
      if (t[rotate(x, y, state.cr)] != 0)
      {
        state.grid[y + state.cy][x + state.cx] = color_table[state.cp];
      }
    }
  }
}

static int count_line(int y)
{
  int count = 0;
  for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
  {
    count += (state.grid[y][x] != 0) ? 1 : 0;
  }
  return count;
}

static void flag_for_clear(int y)
{
  state.should_clear = true;
  for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
  {
    state.grid[y][x] = 0x70;
  }
}

static void clear_line(int line)
{
  for (int y = line; y > 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      state.grid[y][x] = state.grid[y - 1][x];
    }
  }
  state.lines++;
}

static void clear_lines(void)
{
  int count;
  for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
  {
    count = 0;
    for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
    {
      count += (state.grid[y][x] != 0) ? 1 : 0;
    }

    /* clear the row */
    if (count == TETRIS_FIELD_WIDTH)
    {
      flag_for_clear(y);
    }
  }
}

static void move_piece(void)
{
  uint8_t *t = tetromino[state.cp];
  if (does_piece_fit(state.cp, state.cr, state.cx, state.cy + 1))
  {
    state.cy++;
  }
  else
  {
    lock_piece();
    clear_lines();

    /* update score, lines and level */
    state.score += 25;
    state.level = (int)floor(state.lines / 10);

    /* new piece */
    state.cx = 2;
    state.cy = 0;
    state.cr = 0;
    state.cp = state.np;
    state.np = rand() % 7;

    /* game over */
    state.gameover = !does_piece_fit(state.cp, state.cr, state.cx, state.cy);
  }
}

static void reset()
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
  /* FIXME: api needs to adapt. */
  c8_poke4(C8_MEM_FONT_ADDR + 0x00 + 0x00 * 4, 0x00808080);
  c8_poke4(C8_MEM_FONT_ADDR + 0x00 + 0x01 * 4, 0x8080807E);
  c8_poke4(C8_MEM_FONT_ADDR + 0x08 + 0x00 * 4, 0x00000018);
  c8_poke4(C8_MEM_FONT_ADDR + 0x08 + 0x01 * 4, 0x18000000);

  state.ticks = 0;
  state.fx_ticks = 0;
  state.score = 0;
  state.level = 0;
  state.lines = 0;
  state.gameover = false;
  state.should_clear = false;
  state.cx = 2;
  state.cy = 0;
  state.cr = 0;
  state.cp = rand() % 7;
  state.np = rand() % 7;

  memset(&state.grid, 0x00, sizeof(state.grid));
}

void c8_load(void)
{
  reset();
}

static inline int drop_speed()
{
  return (int)(1 / gravity_table[state.level]);
}

void c8_update(void)
{
  if (c8_btn(C8_INPUT_SELECT))
  {
    reset();
  }

  if (c8_btn(C8_INPUT_UP))
  {
    if (does_piece_fit(state.cp, state.cr + 1, state.cx, state.cy))
    {
      state.cr++;
    }
  }
  else if (c8_btn(C8_INPUT_DOWN))
  {
    drop_piece();
  }
  else if (c8_btn(C8_INPUT_LEFT))
  {
    if (does_piece_fit(state.cp, state.cr, state.cx - 1, state.cy))
    {
      state.cx--;
    }
  }
  else if (c8_btn(C8_INPUT_RIGHT))
  {
    if (does_piece_fit(state.cp, state.cr, state.cx + 1, state.cy))
    {
      state.cx++;
    }
  }

  if (!state.gameover)
  {
    if (state.ticks % drop_speed() == 0)
    {
      move_piece();
    }
    state.ticks++;
  }

  /* apply fx */
  if (state.should_clear)
  {
    if (++state.fx_ticks >= TETRIS_CLEAR_LINE_TICKS)
    {
      for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
      {
        if (state.grid[y][0] == color_table[7])
        {
          clear_line(y);
        }
      }
      state.fx_ticks = 0;
      state.should_clear = false;
    }
  }
}

void c8_draw()
{
  c8_cls(0x00, 0x00);

  /* draw field */
  for (int y = 0; y < TETRIS_FIELD_HEIGHT; y++)
  {
    for (int x = 0; x < TETRIS_FIELD_WIDTH; x++)
    {
      c8_color(state.grid[y][x] == 0x00 ? 0x05 : state.grid[y][x]);
      c8_put(x, y, state.grid[y][x] == 0x00 ? 0x01 : 0x00);
    }
  }

  /* draw current and next tetromino */
  draw_tetromino(state.cp, state.cx, state.cy, state.cr);
  draw_tetromino(state.np, 9, 3, 0);

  /* draw user interface */
  c8_color(0x05);
  c8_print(9, 1, "NEXT:");

  /* draw score */
  int score = (state.score_offset < state.score) ? ++state.score_offset : state.score;
  sprintf(state.str_buffer, "%d", score);
  c8_color(0x05);
  c8_print(9, 8, "SCORE:");
  c8_color(0x06);
  c8_print(9, 9, state.str_buffer);

  /* draw level */
  sprintf(state.str_buffer, "%d", state.level);
  c8_color(0x05);
  c8_print(9, 11, "LEVEL:");
  c8_color(0x06);
  c8_print(9, 12, state.str_buffer);

  /* draw lines */
  sprintf(state.str_buffer, "%d", state.lines);
  c8_color(0x05);
  c8_print(9, 14, "LINES:");
  c8_color(0x06);
  c8_print(9, 15, state.str_buffer);

  if (state.gameover)
  {
    c8_color(0x07);
    c8_print(2, 7, "GAME");
    c8_print(2, 8, "OVER");
    c8_color(0x05);
    c8_print(1, 11, "PRESS ");
    c8_print(1, 12, "'R' TO");
    c8_print(1, 13, " RESET");
  }
}