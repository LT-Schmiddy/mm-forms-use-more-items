#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"
RECOMP_IMPORT("*", void recomp_set_fd_anywhere(bool new_val));

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];
extern u8 gPlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];

static u8 overwrite_PlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];

// Helper functions:
void set_item_restriction(PlayerTransformation transform, ItemId item, u8 allowed) {
    gPlayerFormItemRestrictions[transform][item] = allowed;
    overwrite_PlayerFormSlotRestrictions[transform][SLOT(item)] = allowed;
    
}

// Hooks:
RECOMP_HOOK("KaleidoScope_Update") void set_menu_allowed_ammo(PlayState* this){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 24; j++) {
            gPlayerFormSlotRestrictions[i][j] = overwrite_PlayerFormSlotRestrictions[i][j];
        }
    }
}

RECOMP_CALLBACK("*", recomp_after_play_init) void modify_item_restriction_table(PlayState* this) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 24; j++) {
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
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_ARROW_FIRE , 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_ARROW_ICE, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_ARROW_LIGHT, 1);
    set_item_restriction(PLAYER_FORM_ZORA, ITEM_HOOKSHOT, 1);
    // Extra declarations for the elemental arrows:
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_FIRE] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_ICE] = 1;
    gPlayerFormItemRestrictions[PLAYER_FORM_ZORA][ITEM_BOW_LIGHT] = 1;

    // Fierce Deity
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_BOMB, 1);
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_BOMBCHU, 1);
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_DEKU_NUT, 1);
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_MAGIC_BEANS, 1);
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_LENS_OF_TRUTH, 1);
    set_item_restriction(PLAYER_FORM_FIERCE_DEITY, ITEM_POWDER_KEG, 1);

    // recomp_set_fd_anywhere(true);

    recomp_printf("Item Restrictions Updated.\n");
}
