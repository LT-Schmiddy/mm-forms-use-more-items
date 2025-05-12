#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];
extern u8 gPlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];

extern void Player_SetUpperAction(PlayState* play, Player* this, PlayerUpperActionFunc upperActionFunc);
extern void PlayerAnimation_PlayOnce(PlayState* play, SkelAnime* skelAnime, PlayerAnimationHeader* animation);
extern s32 func_808306F8(Player* this, PlayState* play);
extern s32 Player_IsHoldingHookshot(Player* player);
extern void Player_AnimationPlayLoop(PlayState* play, Player* this, PlayerAnimationHeader* anim);
extern s32 Player_UpperAction_12(Player* this, PlayState* play);
extern LinkAnimationHeader gPlayerAnim_pz_cutterwaitB;
extern LinkAnimationHeader gPlayerAnim_pz_cutterwaitA;
extern LinkAnimationHeader gPlayerAnim_pz_cutterwaitC;
extern LinkAnimationHeader gPlayerAnim_link_hook_shot_ready;
extern LinkAnimationHeader gPlayerAnim_pn_tamahakidf;
extern LinkAnimationHeader gPlayerAnim_link_bow_bow_ready;
extern LinkAnimationHeader gPlayerAnim_link_uma_anim_walk;
extern PlayerAnimationHeader* func_8082ED20(Player* this);

static u8 overwrite_PlayerFormItemRestrictions[PLAYER_FORM_MAX][114];
static u8 overwrite_PlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS];

// Helper functions:
void set_item_restriction(PlayerTransformation transform, ItemId item, u8 allowed) {
    gPlayerFormItemRestrictions[transform][item] = allowed;
    // overwrite_PlayerFormItemRestrictions[transform][item] = allowed;
    overwrite_PlayerFormSlotRestrictions[transform][SLOT(item)] = allowed;
    
}



RECOMP_PATCH s32 func_80830E30(Player* this, PlayState* play) {
    if (
        (this->heldItemAction == PLAYER_IA_11) 
        || (
            this->transformation == PLAYER_FORM_ZORA 
            && (this->heldItemAction != PLAYER_IA_BOW) 
            && (this->heldItemAction != PLAYER_IA_BOW_FIRE) 
            && (this->heldItemAction != PLAYER_IA_BOW_ICE) 
            && (this->heldItemAction != PLAYER_IA_BOW_LIGHT) 
            && (this->heldItemAction != PLAYER_IA_HOOKSHOT) 
        )
    ) {
        Player_SetUpperAction(play, this, Player_UpperAction_12);

        PlayerAnimation_PlayOnce(play, &this->skelAnimeUpper,
                                 (this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_LEFT)
                                     ? &gPlayerAnim_pz_cutterwaitA
                                     : ((this->meleeWeaponAnimation == PLAYER_MWA_ZORA_PUNCH_COMBO)
                                            ? &gPlayerAnim_pz_cutterwaitB
                                            : &gPlayerAnim_pz_cutterwaitC));
        this->unk_ACC = 0xA;
    } else {
        if (!func_808306F8(this, play)) {
            return false;
        }

        PlayerAnimation_PlayOnce(play, &this->skelAnimeUpper,
                                 (Player_IsHoldingHookshot(this))
                                     ? &gPlayerAnim_link_hook_shot_ready
                                     : ((this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_tamahakidf
                                                                                   : &gPlayerAnim_link_bow_bow_ready));
    }

    if (this->stateFlags1 & PLAYER_STATE1_800000) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_uma_anim_walk);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->transformation != PLAYER_FORM_ZORA)) {
        Player_AnimationPlayLoop(play, this, func_8082ED20(this));
    }

    return true;
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
