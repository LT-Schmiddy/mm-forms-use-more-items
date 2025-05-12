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
