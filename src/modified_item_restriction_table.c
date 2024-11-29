#include "modding.h"
#include "global.h"

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

// Patches a function in the base game that's used to check if the player should quickspin.
RECOMP_CALLBACK("*", recomp_after_play_init) void modify_item_restriction_table(PlayState* this) {

    // Deku Scrub
    gPlayerFormItemRestrictions[PLAYER_FORM_DEKU][ITEM_DEKU_STICK] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_DEKU][ITEM_MAGIC_BEANS] = 1;
    
    // Goron
    gPlayerFormItemRestrictions[PLAYER_FORM_GORON][ITEM_BOMB] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_GORON][ITEM_BOMBCHU] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_GORON][ITEM_DEKU_NUT] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_GORON][ITEM_MAGIC_BEANS] = 1;

    // Zora
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOMB] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOMBCHU] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_DEKU_STICK] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_DEKU_NUT] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_MAGIC_BEANS] = 1;

}