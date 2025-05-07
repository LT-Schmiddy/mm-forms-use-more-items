#include "modding.h"
#include "global.h"



#include "recomputils.h"

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];

// Patches a function in the base game that's used to check if the player should quickspin.
RECOMP_CALLBACK("*", recomp_after_play_init) void modify_item_restriction_table(PlayState* this) {

    // Deku Scrub
    gPlayerFormItemRestrictions[PLAYER_FORM_DEKU][ITEM_DEKU_STICK] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_DEKU][ITEM_MAGIC_BEANS] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_DEKU][ITEM_SWORD_GREAT_FAIRY] = 1;
    
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
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_FIRE] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_ICE] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_LIGHT] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_HOOKSHOT] = 1;

}
