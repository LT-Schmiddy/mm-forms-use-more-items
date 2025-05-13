#include "modding.h"
#include "global.h"

#include "overlays/actors/ovl_En_Syateki_Man/z_en_syateki_man.h"
#include "overlays/actors/ovl_En_Syateki_Crow/z_en_syateki_crow.h"
#include "overlays/actors/ovl_En_Syateki_Dekunuts/z_en_syateki_dekunuts.h"
#include "overlays/actors/ovl_En_Syateki_Okuta/z_en_syateki_okuta.h"
#include "overlays/actors/ovl_En_Syateki_Wf/z_en_syateki_wf.h"

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10 | ACTOR_FLAG_CANT_LOCK_ON)

#define THIS ((EnSyatekiMan*)thisx)

void EnSyatekiMan_Init(Actor* thisx, PlayState* play);
void EnSyatekiMan_Destroy(Actor* thisx, PlayState* play);
void EnSyatekiMan_Update(Actor* thisx, PlayState* play);
void EnSyatekiMan_Draw(Actor* thisx, PlayState* play);

void EnSyatekiMan_SetupIdle(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_Idle(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_Talk(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_Idle(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_Talk(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_SetupGiveReward(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_GiveReward(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_SetupGiveReward(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_GiveReward(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_MovePlayerAndExplainRules(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_StartGame(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_RunGame(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_EndGame(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Swamp_AddBonusPoints(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_MovePlayerAndSayHighScore(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_StartGame(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_RunGame(EnSyatekiMan* this, PlayState* play);
void EnSyatekiMan_Town_EndGame(EnSyatekiMan* this, PlayState* play);

#define TALK_FLAG_NONE 0
#define TALK_FLAG_TOWN_HAS_SPOKEN_WITH_HUMAN (1 << 0)
#define TALK_FLAG_TOWN_HAS_SPOKEN_WITH_DEKU (1 << 1)
#define TALK_FLAG_TOWN_HAS_SPOKEN_WITH_GORON (1 << 2)
#define TALK_FLAG_TOWN_HAS_SPOKEN_WITH_ZORA (1 << 3)
#define TALK_FLAG_TOWN_HAS_EXPLAINED_THE_RULES (1 << 4)
#define TALK_FLAG_SWAMP_HAS_SPOKEN_WITH_HUMAN (1 << 0)
#define TALK_FLAG_SWAMP_HAS_EXPLAINED_THE_RULES (1 << 1)

typedef enum {
    /* 0 */ SG_MAN_ANIM_HANDS_ON_TABLE,
    /* 1 */ SG_MAN_ANIM_HEAD_SCRATCH_LOOP,
    /* 2 */ SG_MAN_ANIM_HEAD_SCRATCH_END
} ShootingGalleryManAnimation;

static AnimationInfo sAnimationInfo[] = {
    { &gBurlyGuyHandsOnTableAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },    // SG_MAN_ANIM_HANDS_ON_TABLE
    { &gBurlyGuyHeadScratchLoopAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f }, // SG_MAN_ANIM_HEAD_SCRATCH_LOOP
    { &gBurlyGuyHeadScratchEndAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -8.0f },  // SG_MAN_ANIM_HEAD_SCRATCH_END
};

// Enabling the Zora Link to use the Swamp Shooting Gallery:
RECOMP_PATCH void EnSyatekiMan_Swamp_Idle(EnSyatekiMan* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        u16 faceReactionTextId;

        Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, SG_MAN_ANIM_HEAD_SCRATCH_END);
        faceReactionTextId = Text_GetFaceReaction(play, FACE_REACTION_SET_SWAMP_SHOOTING_GALLERY_MAN);
        if (faceReactionTextId != 0) {
            Message_StartTextbox(play, faceReactionTextId, &this->actor);
            this->prevTextId = faceReactionTextId;
        } else if (player->transformation == PLAYER_FORM_HUMAN || player->transformation == PLAYER_FORM_ZORA) {
            if (this->talkFlags == TALK_FLAG_NONE) {
                this->talkFlags = TALK_FLAG_SWAMP_HAS_SPOKEN_WITH_HUMAN;
                // How are you? Wanna play?
                Message_StartTextbox(play, 0xA28, &this->actor);
                this->prevTextId = 0xA28;
            } else {
                // Won't you play?
                Message_StartTextbox(play, 0xA29, &this->actor);
                this->prevTextId = 0xA29;
            }
        } else {
            switch (CURRENT_DAY) {
                case 1:
                    // You can't play if you don't have a bow! (Day 1)
                    Message_StartTextbox(play, 0xA38, &this->actor);
                    this->prevTextId = 0xA38;
                    break;

                case 2:
                    // You can't play if you don't have a bow! (Day 2)
                    Message_StartTextbox(play, 0xA39, &this->actor);
                    this->prevTextId = 0xA39;
                    break;

                case 3:
                    // You can't play if you don't have a bow! (Day 3)
                    Message_StartTextbox(play, 0xA3A, &this->actor);
                    this->prevTextId = 0xA3A;
                    break;
            }
        }
        this->actionFunc = EnSyatekiMan_Swamp_Talk;
    } else {
        Actor_OfferTalk(&this->actor, play, 120.0f);
    }

    if (player->actor.world.pos.z < 135.0f) {
        player->actor.world.pos.z = 135.0f;
    }
}

// A little cursed, NGL.
RECOMP_PATCH void EnSyatekiMan_Town_StartIntroTextbox(EnSyatekiMan* this, PlayState* play) {
    switch (GET_PLAYER_FORM) {
        case PLAYER_FORM_HUMAN:
            Flags_SetAllTreasure(play, Flags_GetAllTreasure(play) + 1);
            if (CURRENT_DAY != 3) {
                if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_HUMAN)) {
                    this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_HUMAN;
                    // Why don't you give it a try?
                    Message_StartTextbox(play, 0x3E8, &this->actor);
                    this->prevTextId = 0x3E8;
                } else {
                    // Wanna try?
                    Message_StartTextbox(play, 0x3E9, &this->actor);
                    this->prevTextId = 0x3E9;
                }
            } else if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_HUMAN)) {
                this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_HUMAN;
                // If you still have time, why don't you try it?
                Message_StartTextbox(play, 0x3EA, &this->actor);
                this->prevTextId = 0x3EA;
            } else {
                // How about it? Wanna try?
                Message_StartTextbox(play, 0x3EB, &this->actor);
                this->prevTextId = 0x3EB;
            }
            break;

        case PLAYER_FORM_DEKU:
            if (CURRENT_DAY != 3) {
                if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_DEKU)) {
                    this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_DEKU;
                    // When I saw your fairy, I thought you were that masked troublemaker.
                    Message_StartTextbox(play, 0x3EC, &this->actor);
                    this->prevTextId = 0x3EC;
                } else {
                    // You can't play because you don't have a bow.
                    Message_StartTextbox(play, 0x3ED, &this->actor);
                    this->prevTextId = 0x3ED;
                }
            } else if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_DEKU)) {
                this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_DEKU;
                // I thought you were a customer, but I guess I can't expect any...
                Message_StartTextbox(play, 0x3EE, &this->actor);
                this->prevTextId = 0x3EE;
            } else {
                // Stop hanging around and go home!
                Message_StartTextbox(play, 0x3EF, &this->actor);
                this->prevTextId = 0x3EF;
            }
            break;

        case PLAYER_FORM_ZORA:
            if (CURRENT_DAY != 3) {
                // By using the same this->prevTextId = 0x3E8 assignments as human form,
                // We allow Zora Link to play the game while preserving the dialog unique to him.
                if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_ZORA)) {
                    this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_ZORA;
                    // I swear I've seen you before...
                    Message_StartTextbox(play, 0x3F0, &this->actor);
                    // using the previus
                    this->prevTextId = 0x3E8;
                } else {
                    // Wanna try?
                    Message_StartTextbox(play, 0x3E9, &this->actor);
                    this->prevTextId = 0x3E9;
                }
            } else if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_ZORA)) {
                this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_ZORA;
                // Huh? You're still here?
                Message_StartTextbox(play, 0x3F4, &this->actor);
                this->prevTextId = 0x3EA;
            } else {
                // Haven't you heard the news?
                Message_StartTextbox(play, 0x3F5, &this->actor);
                this->prevTextId = 0x3EB;
            }
            break;

        case PLAYER_FORM_GORON:
            if (CURRENT_DAY != 3) {
                if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_GORON)) {
                    this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_GORON;
                    // You have quite the build!
                    Message_StartTextbox(play, 0x3F2, &this->actor);
                    this->prevTextId = 0x3F2;
                } else {
                    // Sorry...you don't have a bow.
                    Message_StartTextbox(play, 0x3F3, &this->actor);
                    this->prevTextId = 0x3F3;
                }
            } else if (!(this->talkFlags & TALK_FLAG_TOWN_HAS_SPOKEN_WITH_GORON)) {
                this->talkFlags |= TALK_FLAG_TOWN_HAS_SPOKEN_WITH_GORON;
                // Huh? You're still here?
                Message_StartTextbox(play, 0x3F4, &this->actor);
                this->prevTextId = 0x3F4;
            } else {
                // Haven't you heard the news?
                Message_StartTextbox(play, 0x3F5, &this->actor);
                this->prevTextId = 0x3F5;
            }
            break;

        default:
            break;
    }
}