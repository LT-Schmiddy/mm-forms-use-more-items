#include "modding.h"
#include "global.h"



#include "recomputils.h"

extern u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114];

// Patches a function in the base game that's used to check if the player should quickspin.

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



RECOMP_PATCH s32 func_80830E30(Player* this, PlayState* play) {

        if (!func_808306F8(this, play)) {
            return false;
        }

        PlayerAnimation_PlayOnce(play, &this->skelAnimeUpper,
                                 Player_IsHoldingHookshot(this)
                                     ? &gPlayerAnim_link_hook_shot_ready
                                     : ((this->transformation == PLAYER_FORM_DEKU) ? &gPlayerAnim_pn_tamahakidf
                                                                                   : &gPlayerAnim_link_bow_bow_ready));
    

    if (this->stateFlags1 & PLAYER_STATE1_800000) {
        Player_AnimationPlayLoop(play, this, &gPlayerAnim_link_uma_anim_walk);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->transformation != PLAYER_FORM_ZORA)) {
        Player_AnimationPlayLoop(play, this, func_8082ED20(this));
    }

    return true;
}

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
