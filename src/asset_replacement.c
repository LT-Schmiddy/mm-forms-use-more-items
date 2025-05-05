#include "modding.h"
#include "global.h"
#include "gLinkZoraBowSkel.h"
#include "gLinkZoraHookshotSkel.h"
#include "gLinkZoraBow3rdPersonSkel.h"

extern Gfx* sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_MAX];
extern Gfx* sPlayerFirstPersonRightHandDLs[PLAYER_FORM_MAX];
extern Gfx* sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_MAX];
extern Gfx* gPlayerRightHandClosedDLs[2 * PLAYER_FORM_MAX];
extern Gfx gLinkHumanHookshotDL[];
extern Gfx gLinkHumanBowDL[];

extern Gfx gLinkZoraRightHandClosedDL[];
extern Gfx* gPlayerRightHandHookshotDLs[2 * PLAYER_FORM_MAX];
extern Gfx* gPlayerRightHandBowDLs[2 * PLAYER_FORM_MAX];

Gfx gLinkZoraRightHandHoldingHookshotDL[] = {
    gsSPDisplayList(gLinkZoraRightHandClosedDL),
    gsSPBranchList(gLinkHumanHookshotDL),
};

Gfx gLinkZoraRightHandShoulderAndForearm[] = {
    gsSPDisplayList(gLinkZoraBowSkel_bone016_gLinkZoraRightShoulderLimb_mesh_layer_Opaque),
    gsSPBranchList(gLinkZoraBowSkel_bone017_gLinkZoraRightForearmLimb_mesh_layer_Opaque),
};

RECOMP_HOOK("Player_Init") void on_Player_Init(Actor* thisx, PlayState* play) {
    sPlayerFirstPersonRightShoulderDLs[PLAYER_FORM_ZORA] = gLinkZoraRightHandShoulderAndForearm;
    sPlayerFirstPersonRightHandDLs[PLAYER_FORM_ZORA] = gLinkZoraBowSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandHookshotDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraRightHandHoldingHookshotDL;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 0] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    gPlayerRightHandBowDLs[PLAYER_FORM_ZORA * 2 + 1] = gLinkZoraBow3rdPersonSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
    sPlayerFirstPersonRightHandHookshotDLs[PLAYER_FORM_ZORA] = gLinkZoraHookshotSkel_bone018_gLinkZoraRightHandLimb_mesh_layer_Opaque;
}

