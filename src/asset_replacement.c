#include "modding.h"
#include "global.h"
#include "gLinkZoraBowSkel.h"
#include "gLinkZoraHookshotSkel.h"
#include "gLinkZoraBow3rdPersonSkel.h"
#include "gGreatFairySword.h"
#include "gHookshot.h"

#include "prevent_bss_reordering.h"
#include "global.h"
#include "z64horse.h"
#include "z64quake.h"
#include "z64rumble.h"
#include "z64shrink_window.h"

#include "functions.h"

extern Gfx* sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_MAX];
extern Gfx* sPlayerFirstPersonRightHandDLs[PLAYER_FORM_MAX];
extern Gfx* sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_MAX];
extern Gfx* gPlayerRightHandClosedDLs[2 * PLAYER_FORM_MAX];

extern Gfx gLinkZoraRightHandClosedDL[];
extern Gfx* gPlayerRightHandHookshotDLs[2 * PLAYER_FORM_MAX];
extern Gfx* gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX];

extern Gfx* gPlayerLeftHandTwoHandSwordDLs[2 * PLAYER_FORM_MAX];

extern Gfx gLinkDekuLeftHandDL[];
extern Gfx gLinkHumanGreatFairysSwordDL[];

Gfx gLinkZoraRightHandHoldingHookshotDL[] = {
    gsSPDisplayList(gLinkZoraRightHandClosedDL),
    gsSPBranchList(gHookshot),
};

Gfx gLinkZoraRightHandShoulderAndForearm[] = {
    gsSPDisplayList(gLinkZoraBowSkel_bone016_gLinkZoraRightShoulderLimb_mesh_layer_Opaque),
    gsSPBranchList(gLinkZoraBowSkel_bone017_gLinkZoraRightForearmLimb_mesh_layer_Opaque),
};

Gfx gLinkDekuHoldingGreatFairySwordDL[] = {
    gsSPDisplayList(gLinkDekuLeftHandDL),
    gsSPBranchList(gGreatFairySword),    
};

RECOMP_HOOK("Player_Init") void on_Player_Init(Actor* thisx, PlayState* play) {
    // Zora link
    sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_ZORA] = gLinkZoraRightHandShoulderAndForearm;
    sPlayerFirstPersonRightHandDLs[PLAYER_FORM_ZORA] = gLinkZoraBowSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_ZORA] = gLinkZoraHookshotSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;

    // Deku Link
    gPlayerLeftHandTwoHandSwordDLs[PLAYER_FORM_DEKU * 2 + 0] = gLinkDekuHoldingGreatFairySwordDL;
}

extern PlayerAnimationHeader gPlayerAnim_link_hook_shot_ready;
extern PlayerAnimationHeader gPlayerAnim_pn_tamahakidf;
extern PlayerAnimationHeader gPlayerAnim_link_bow_bow_ready;
extern PlayerAnimationHeader gPlayerAnim_link_uma_anim_walk;
extern PlayerAnimationHeader gPlayerAnim_pz_cutterwaitA;
extern PlayerAnimationHeader gPlayerAnim_pz_cutterwaitB;
extern PlayerAnimationHeader gPlayerAnim_pz_cutterwaitC;

s32 func_808306F8(Player* this, PlayState* play);
void Player_AnimationPlayLoop(PlayState* play, Player* this, PlayerAnimationHeader* anim);
PlayerAnimationHeader* func_8082ED20(Player* this);
s32 Player_UpperAction_12(Player* this, PlayState* play);
void Player_SetUpperAction(PlayState* play, Player* this, PlayerUpperActionFunc upperActionFunc);

// Allows use of bow and hookshot as Zora Link:
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
/*
s32 func_80830E30_2(Player* this, PlayState* play) {
    if ((this->heldItemAction == PLAYER_IA_11) || (this->transformation == PLAYER_FORM_ZORA)) {
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
}*/