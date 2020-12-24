#ifndef ITEM_00_H
#define ITEM_00_H

#include "z2.h"

void item00_set_gi_index(z2_en_item00_t *actor, u16 gi_index);
u16 item00_get_gi_index(z2_en_item00_t *actor);
void item00_set_draw_gi_index(z2_en_item00_t *actor, u16 draw_gi_index);
u16 item00_get_draw_gi_index(z2_en_item00_t *actor);
void item00_check_and_set_gi_index(z2_en_item00_t *item, z2_game_t *game, u16 gi_index);

#endif // ITEM_00_H