// Honey and Darling Dialog:

/* Reasons you can't play Honey and Darling.
Dumping Game Message ID 0x286F (10351):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_I
        Textbox Y Position: 0
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "|1EiqWhat shall we do, Honey?|11From what I can see, he doesn't|11seem to be able to carry |01bombs|00.|19|BF"

Dumping Game Message ID 0x2870 (10352):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_I
        Textbox Y Position: 0
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "|1Ei,Oh my...Then he can't play, can he,|11Darling?|BF"


Dumping Game Message ID 0x2841 (10305):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_I
        Textbox Y Position: 0
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "|1EiqBut from what I see, he doesn't|11have a bow, Honey.|19|BF"

Dumping Game Message ID 0x2842 (10306):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_I
        Textbox Y Position: 0
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "|1Ei,Oh...Then he can't play, can he,|11Darling?|BF"

Bomb Shop:

Dumping Game Message ID 0x064B (1611):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_I
        Textbox Y Position: 0
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "You can't use any of them, but|11feel free to look around.|19|BF"

Dumping Game Message ID 0x0644 (1604):
MsgBuffer:
        Textbox Type: EZTR_STANDARD_TEXT_BOX_II
        Textbox Y Position: 48
        Display Icon: EZTR_ICON_NO_ICON
        Next Message ID: EZTR_NO_VALUE
        First Item Rupees: EZTR_NO_VALUE
        Second Item Rupees: EZTR_NO_VALUE
        Content: "...Welcome.|19|BF"
*/

#include "modding.h"
#include "global.h"
#include "z64player.h"
#include "eztr_api.h"
#include "x_eztr_msg_macros.h"

const char* goron_excuse = "|1EiqWhat shall we do, Honey?|11From what I can see, he's too|11heavy for the platform to support.|19|BF";
const char* zora_excuse = "|1EiqHe's a celebrity, Honey.|11You wouldn't want me to think|11I have competiton, do you?|19|BF";

EZTR_MSG_CALLBACK(day_1_and_2_callback) {
    Player* player = GET_PLAYER(play);
    if (player->transformation == PLAYER_FORM_GORON || player->transformation == PLAYER_FORM_FIERCE_DEITY) {
        EZTR_MsgSContent_Sprintf(buf->data.content, goron_excuse);
    } else if (player->transformation == PLAYER_FORM_ZORA) {
        EZTR_MsgSContent_Sprintf(buf->data.content, zora_excuse);
    } else {
        // Vanilla Dialog:
        EZTR_MsgSContent_Sprintf(buf->data.content, "|1EiqWhat shall we do, Honey?|11From what I can see, he doesn't|11seem to be able to carry |01bombs|00.|19|BF");
    }
}

EZTR_MSG_CALLBACK(day_3_callback) {
    Player* player = GET_PLAYER(play);
    if (player->transformation == PLAYER_FORM_GORON || player->transformation == PLAYER_FORM_FIERCE_DEITY) {
        EZTR_MsgSContent_Sprintf(buf->data.content, goron_excuse);
    } else if (player->transformation == PLAYER_FORM_ZORA) {
        EZTR_MsgSContent_Sprintf(buf->data.content, zora_excuse);
    } else {
        // Vanilla Dialog:
        EZTR_MsgSContent_Sprintf(buf->data.content, "|1EiqBut from what I see, he doesn't|11have a bow, Honey.|19|BF");
    }
}

EZTR_MSG_CALLBACK(bombshop_nonhuman_callback) {
    Player* player = GET_PLAYER(play);
    if (player->transformation != PLAYER_FORM_DEKU) {
        buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II;
        EZTR_MsgSContent_Sprintf(buf->data.content, "...Welcome.|19|BF");
    } else {
        // Vanilla Dialog:
        EZTR_MsgSContent_Sprintf(buf->data.content, "You can't use any of them, but|11feel free to look around.|19|BF");
    }
}

EZTR_ON_INIT void setup_messages() {

    // Replacements for Honey & Darling
    EZTR_Basic_ReplaceText(
        0x2841,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xbf",
        day_1_and_2_callback
    );

    EZTR_Basic_ReplaceText(
        0x286F,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xbf",
        day_3_callback
    );

    // Replacements for Bomb Shop
    EZTR_Basic_ReplaceText(
        0x064B,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xbf",
        bombshop_nonhuman_callback
    );
}