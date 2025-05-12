#include "modding.h"
#include "global.h"
#include "models/gLinkZoraBowSkel.h"
#include "models/gLinkZoraHookshotSkel.h"
#include "models/gLinkZoraBow3rdPersonSkel.h"
#include "models/gGreatFairySword.h"
#include "models/gHookshot.h"

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
extern Gfx* sPlayerFirstPersonLeftHandDLs[PLAYER_FORM_MAX];
extern Gfx* sPlayerFirstPersonLeftForearmDLs[PLAYER_FORM_MAX];

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

RECOMP_HOOK("Player_Init") void on_Player_Init(Actor* thisx, PlayState* play) {
    // Zora link
    sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_ZORA] = gLinkZoraRightHandShoulderAndForearm;
    sPlayerFirstPersonRightHandDLs[PLAYER_FORM_ZORA] = gLinkZoraBowSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_ZORA] = gLinkZoraHookshotSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    sPlayerFirstPersonLeftHandDLs[PLAYER_FORM_ZORA] = gLinkZoraBowSkel_bone015_gLinkZoraLeftHandLimb_mesh_layer_Opaque;

    // Deku Link
    gPlayerLeftHandTwoHandSwordDLs[PLAYER_FORM_DEKU * 2 + 0] = gLinkDekuHoldingGreatFairySwordDL;
}

RECOMP_IMPORT("ProxyMM_ObjDepLoader", bool ObjDepLoader_Load(PlayState* play, u8 segment, s16 objectId));
RECOMP_IMPORT("ProxyMM_ObjDepLoader", void ObjDepLoader_Unload(PlayState* play, u8 segment, s16 objectId));

PlayState* sPlayState;
static bool sShouldUnloadForPostLimbDraw = false;
RECOMP_HOOK("Player_PostLimbDrawGameplay") void on_Player_PostLimbDrawGameplay(PlayState* play, s32 limbIndex, Gfx** dList1, Gfx** dList2, Vec3s* rot, Actor* actor) {
    if (limbIndex == PLAYER_LIMB_RIGHT_HAND) {
        sShouldUnloadForPostLimbDraw = true;
        ObjDepLoader_Load(play, 0x6, OBJECT_LINK_CHILD);
    }
    sPlayState = play;
}

RECOMP_HOOK_RETURN("Player_PostLimbDrawGameplay") void return_PostLimbDrawGameplay(void) {
    if (sShouldUnloadForPostLimbDraw) {
        sShouldUnloadForPostLimbDraw = false;
        ObjDepLoader_Unload(sPlayState, 0x6, OBJECT_LINK_CHILD);
    }
}

static bool sShouldUnloadForArmsHook = false;
RECOMP_HOOK("ArmsHook_Draw") void on_ArmsHook_Draw(Actor* thisx, PlayState* play) {
    sShouldUnloadForArmsHook = true;
    ObjDepLoader_Load(play, 0x6, OBJECT_LINK_CHILD);
    sPlayState = play;
}

RECOMP_HOOK_RETURN("ArmsHook_Draw") void return_ArmsHook_Draw(void) {
    if (sShouldUnloadForArmsHook) {
        sShouldUnloadForArmsHook = false;
        ObjDepLoader_Unload(sPlayState, 0x6, OBJECT_LINK_CHILD);
    }
}

Player *gOriginalPlayer;
u8 gOriginalPlayerTransformation;
u8 gShouldSkipFins = false;
RECOMP_HOOK("func_80126BD0") void on_func_80126BD0(PlayState* play, Player* player, s32 arg2) {
    gOriginalPlayer = player;
    gOriginalPlayerTransformation = player->transformation;
    if (gShouldSkipFins) {
        if (player->transformation == PLAYER_FORM_ZORA) {
            player->transformation = PLAYER_FORM_HUMAN;
        }
    }
}

RECOMP_HOOK_RETURN("func_80126BD0") void return_func_80126BD0(void) {
    if (gShouldSkipFins) {
        gOriginalPlayer->transformation = gOriginalPlayerTransformation;
        gShouldSkipFins = false;
    }
    
}

RECOMP_HOOK("Player_OverrideLimbDrawGameplayFirstPerson") void on_Player_OverrideLimbDrawGameplayFirstPerson(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* actor) {
    gShouldSkipFins = true;
}

// Move drawn arrow's starting point from bow
#define MOVEARROWX 300
#define MOVEARROWY -200
#define MOVEARROWZ -100

RECOMP_HOOK ("Player_PostLimbDrawGameplay") void on_PostLimbDrawGameplay(PlayState* play, s32 limbIndex, Gfx** dList1, Gfx** dList2, Vec3s* rot, Actor* actor) {
    Player* player = (Player*) actor;
    if (limbIndex == PLAYER_LIMB_LEFT_HAND && player->actor.scale.y >= 0.0f) {
        Actor* heldActor;
        MtxF sp230;
        if (!Player_IsHoldingHookshot(player) && ((heldActor = player->heldActor) != NULL)) {
            if ((player->stateFlags3 & PLAYER_STATE3_40) && (player->transformation != PLAYER_FORM_DEKU)) {
                if (player->transformation == PLAYER_FORM_ZORA) {
                    Vec3s* temp_s1;
                    Matrix_Translate(MOVEARROWX, MOVEARROWY, MOVEARROWZ, MTXMODE_APPLY);    // Move arrow's drawn position
    
                    Matrix_Get(&sp230);
                    temp_s1 = &heldActor->world.rot;
                    Matrix_MtxFToYXZRot(&sp230, temp_s1, false);
                    heldActor->shape.rot = *temp_s1;
                }
            }
        }
    }
}


// Enables Zora Link to use the Bow and Hookshot:
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
