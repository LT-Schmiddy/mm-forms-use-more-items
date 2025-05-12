#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];
extern u8 gPlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];



static u8 overwrite_PlayerFormItemRestrictions[PLAYER_FORM_MAX][114];
static u8 overwrite_PlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];

// Helper functions:
void set_item_restriction(PlayerTransformation transform, ItemId item, u8 allowed) {
    gPlayerFormItemRestrictions[transform][item] = allowed;
    // overwrite_PlayerFormItemRestrictions[transform][item] = allowed;
    overwrite_PlayerFormSlotRestrictions[transform][SLOT(item)] = allowed;
    
}





RECOMP_HOOK("KaleidoScope_Update") void set_menu_allowed_ammo(PlayState* this){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 24; j++) {
            gPlayerFormItemRestrictions[i][j] = overwrite_PlayerFormItemRestrictions[i][j];
            gPlayerFormSlotRestrictions[i][j] = overwrite_PlayerFormSlotRestrictions[i][j];
        }
    }
}

RECOMP_CALLBACK("*", recomp_after_play_init) void modify_item_restriction_table(PlayState* this) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 24; j++) {
            overwrite_PlayerFormItemRestrictions[i][j] = gPlayerFormItemRestrictions[i][j];
            overwrite_PlayerFormSlotRestrictions[i][j] = gPlayerFormSlotRestrictions[i][j];
        }
    }

    // Deku Scrub
    set_item_restriction(PLAYER_FORM_DEKU, ITEM_DEKU_STICK, 1);
    set_item_restriction(PLAYER_FORM_DEKU, ITEM_MAGIC_BEANS, 1);
    set_item_restriction(PLAYER_FORM_DEKU, ITEM_SWORD_GREAT_FAIRY, 1);
    
    // Goron
    set_item_restriction(PLAYER_FORM_GORON, ITEM_BOMB, 1);
    set_item_restriction(PLAYER_FORM_GORON, ITEM_BOMBCHU, 1);
    set_item_restriction(PLAYER_FORM_GORON, ITEM_DEKU_NUT, 1);
    set_item_restriction(PLAYER_FORM_GORON, ITEM_MAGIC_BEANS, 1);

    // Zora
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOMB, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOMBCHU, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_DEKU_STICK, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_DEKU_NUT, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_MAGIC_BEANS, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOW, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOW_FIRE, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOW_ICE, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_BOW_LIGHT, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_HOOKSHOT, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_SWORD_GREAT_FAIRY, 1);

    recomp_printf("Item Restrictions Updated.\n");
}
