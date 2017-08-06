#include "global.h"
#include "decompress.h"
#include "palette.h"
#include "event_data.h"
#include "main.h"
#include "text_window.h"
#include "menu.h"
#include "strings2.h"
#include "sound.h"
#include "berry.h"
#include "string_util.h"
#include "link.h"
#include "task.h"
#include "rom4.h"
#include "items.h"
#include "rng.h"

//needed to match Blender_ControlHitPitch
struct MusicPlayerInfo
{
    struct SongHeader *songHeader;
    u32 status;
    u8 trackCount;
    u8 priority;
    u8 cmd;
    u8 unk_B;
    u32 clock;
    u8 gap[8];
    u8 *memAccArea;
    u16 tempoD;
    u16 tempoU;
    u16 tempoI;
    u16 tempoC;
    u16 fadeOI;
    u16 fadeOC;
    u16 fadeOV;
    struct MusicPlayerTrack *tracks;
    struct ToneData *tone;
    u32 ident;
    u32 func;
    u32 intp;
};

// other files functions
void m4aMPlayPitchControl(struct MusicPlayerInfo *mplayInfo, u16 trackBits, s16 pitch);
void sub_80A6978(void);
u8 sub_80A7DEC(u8 berryId, u8 x, u8 y, bool8 animate);

#define BLENDER_MAX_PLAYERS 4

#define FLAVOUR_SPICY       0
#define FLAVOUR_DRY         1
#define FLAVOUR_SWEET       2
#define FLAVOUR_BITTER      3
#define FLAVOUR_SOUR        4

struct BlenderBerry
{
    u16 field_0;
    u8 name[7];
    u8 flavours[5];
    u8 smoothness;
};

struct BerryBlenderData
{
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    u8 field_F;
    u8 field_10;
    u8 field_11;
    u8 field_12;
    u8 field_13;
    u8 field_14;
    u8 field_15;
    u8 field_16;
    u8 field_17;
    u8 field_18;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
    u8 field_24;
    u8 field_25;
    u8 field_26;
    u8 field_27;
    u8 field_28;
    u8 field_29;
    u8 field_2A;
    u8 field_2B;
    u8 field_2C;
    u8 field_2D;
    u8 field_2E;
    u8 field_2F;
    u8 field_30;
    u8 field_31;
    u8 field_32;
    u8 field_33;
    u8 field_34;
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
    u8 field_3A;
    u8 field_3B;
    u8 field_3C;
    u8 field_3D;
    u8 field_3E;
    u8 field_3F;
    u8 field_40;
    u8 field_41;
    u8 field_42;
    u8 field_43;
    u8 field_44;
    u8 field_45;
    u8 field_46;
    u8 field_47;
    u8 field_48;
    u8 field_49;
    u8 field_4A;
    u8 field_4B;
    u8 field_4C;
    u8 field_4D;
    u16 field_4E;
    u8 field_50;
    u8 field_51;
    u8 field_52;
    u8 field_53;
    u16 arrowPos;
    s16 hitPitch;       //56
    u8 field_58;
    u8 field_59;
    u16 field_5A;
    u8 SyncArrowSpriteID[BLENDER_MAX_PLAYERS];
    u8 SyncArrowSprite2ID[BLENDER_MAX_PLAYERS];
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    u8 field_68;
    u8 field_69;
    u8 field_6A;
    u8 field_6B;
    u8 field_6C;
    u8 field_6D;
    u8 field_6E;
    u8 field_6F;
    u16 field_70[BLENDER_MAX_PLAYERS];
    u8 field_78;
    u8 field_79;
    u8 field_7A;
    u8 field_7B;
    u16 field_7C;
    u8 field_7E;
    u8 field_7F;
    u16 field_80[BLENDER_MAX_PLAYERS];
    u8 field_88;
    u8 field_89;
    u8 field_8A;
    u8 field_8B;
    u8 field_8C;
    u8 field_8D;
    u8 field_8E;
    u8 field_8F;
    u8 field_90;
    u8 field_91;
    u8 field_92;
    u8 field_93;
    u8 field_94;
    u8 field_95;
    u8 field_96;
    u8 field_97;
    u8 field_98;
    u8 field_99;
    u16 field_9A[BLENDER_MAX_PLAYERS];
    u16 field_A2[BLENDER_MAX_PLAYERS];
    u8 field_AA;
    u8 field_AB;
    u8 field_AC;
    u8 field_AD;
    u8 field_AE;
    u8 field_AF;
    u8 field_B0;
    u8 field_B1;
    u8 field_B2;
    u8 field_B3;
    u8 field_B4;
    u8 field_B5;
    u8 field_B6;
    u8 field_B7;
    u8 field_B8;
    u8 field_B9;
    u8 field_BA;
    u8 field_BB;
    u8 field_BC;
    u8 field_BD;
    u8 field_BE;
    u8 field_BF;
    u8 field_C0;
    u8 field_C1;
    u8 field_C2;
    u8 field_C3;
    u8 field_C4;
    u8 field_C5;
    u8 field_C6;
    u8 field_C7;
    u8 field_C8;
    u8 field_C9;
    u8 field_CA;
    u8 field_CB;
    u8 field_CC;
    u8 field_CD;
    u8 field_CE;
    u8 field_CF;
    u8 field_D0;
    u8 field_D1;
    u8 field_D2;
    u8 field_D3;
    u8 field_D4;
    u8 field_D5;
    u8 field_D6;
    u8 field_D7;
    u8 field_D8;
    u8 field_D9;
    u8 field_DA;
    u8 field_DB;
    u8 field_DC;
    u8 field_DD;
    u8 field_DE;
    u8 field_DF;
    u8 field_E0;
    u8 field_E1;
    u8 field_E2;
    u8 field_E3;
    u8 field_E4;
    u8 field_E5;
    u8 field_E6;
    u8 field_E7;
    u8 field_E8;
    u8 field_E9;
    u8 field_EA;
    u8 field_EB;
    u8 field_EC;
    u8 field_ED;
    u8 field_EE;
    u8 field_EF;
    u8 field_F0;
    u8 field_F1;
    u8 field_F2;
    u8 field_F3;
    u8 field_F4;
    u8 field_F5;
    u8 field_F6;
    u8 field_F7;
    u8 field_F8;
    u8 field_F9;
    u8 field_FA;
    u8 field_FB;
    u8 field_FC;
    u8 field_FD;
    u8 field_FE;
    u8 field_FF;
    u8 field_100;
    u8 field_101;
    u8 field_102;
    u8 field_103;
    u8 field_104;
    u8 field_105;
    u8 field_106;
    u8 field_107;
    u8 field_108;
    u8 field_109;
    u8 field_10A;
    u8 field_10B;
    u8 field_10C;
    u8 field_10D;
    u8 field_10E;
    u8 field_10F;
    u8 field_110;
    u8 field_111;
    u8 field_112;
    u8 field_113;
    u8 field_114;
    u8 field_115;
    u8 field_116;
    u8 field_117;
    u8 field_118;
    u8 field_119;
    u8 field_11A;
    u8 field_11B;
    u8 field_11C;
    u8 field_11D;
    u8 field_11E;
    u8 field_11F;
    u8 field_120;
    u8 field_121;
    u8 field_122;
    u8 field_123;
    u8 field_124;
    u8 field_125;
    u8 field_126;
    u8 field_127;
    u8 field_128;
    u8 field_129;
    u8 field_12A;
    u8 field_12B;
    u32 field_12C;
    s32 field_130;
    u32 field_134;
    u8 field_138;
    u8 field_139;
    u8 field_13A;
    u8 field_13B;
    u8 field_13C;
    u8 field_13D;
    u16 field_13E;
    u16 field_140;
    u16 field_142;
    u16 field_144;
    u16 field_146;
    u8 field_148[3];
    u8 field_14B;
    u16 field_14C[BLENDER_MAX_PLAYERS][3];
    u8 field_164;
    u8 field_165;
    u8 field_166;
    u8 field_167;
    u8 field_168;
    u8 field_169;
    u8 field_16A;
    u8 field_16B;
    u8 field_16C;
    u8 field_16D;
    u8 field_16E;
    u8 field_16F;
    u8 field_170;
    u8 field_171;
    u8 field_172;
    u8 field_173;
    u8 field_174;
    u8 field_175;
    u8 field_176;
    u8 field_177;
    u16 field_178;
    u8 field_17A;
    u8 field_17B;
    struct BlenderBerry blendedBerries[BLENDER_MAX_PLAYERS];
};

extern struct BerryBlenderData* sBerryBlenderData;

extern struct MusicPlayerInfo gMPlay_SE2;
extern u8 ewram[];

// this file's functions:
void sub_80514A4(void);
void sub_80514F0(void);
void sub_804E56C(void);
void sub_804E884(u8 a0);
void sub_8051474(void);
void sub_804E9F8(void);
void sub_804F378(void);
void sub_8051414(void*);
void sub_804F238(void);
void sub_80501FC(void);
bool8 sub_8051B8C(void);
void sub_80516C4(u8 a0, u16 itemID);
void sub_804F2A8(void);
void sub_804F81C(void);

void Blender_ControlHitPitch(void)
{
    m4aMPlayPitchControl(&gMPlay_SE2, 0xFFFF, (sBerryBlenderData->hitPitch - 128) * 2);
}

void VBlankCB0_BerryBlender(void)
{
    sub_80514A4();
    sub_80514F0();
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void VBlankCB1_BerryBlender(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

extern const u8 gUnknown_08E6C100[];
extern const u8 gUnknown_08215C2C[];
extern const u8 gUnknown_08E6C920[];
extern const u8 gUnknown_08E6D354[];
extern const u16 gUnknown_08215C0C[];
extern const u16 gUnknown_0821602C[];
extern const struct SpriteSheet gUnknown_082163DC;
extern const struct SpriteSheet gUnknown_082164F4;
extern const struct SpriteSheet gUnknown_08216454;
extern const struct SpriteSheet gUnknown_08216540;
extern const struct SpriteSheet gUnknown_08216574;
extern const struct SpritePalette gUnknown_082163EC;
extern const struct SpritePalette gUnknown_082163E4;

bool8 sub_804E2EC(void)
{
    switch (sBerryBlenderData->field_1)
    {
    case 0:
        sub_800D238(gUnknown_08E6C100, &ewram[0x10000]);
        sBerryBlenderData->field_1++;
        break;
    case 1:
        {
            const void* offsetRead = gUnknown_08215C2C;
            void* offsetWrite = (void*)(VRAM + 0x4000);

            DmaCopy16(3, offsetRead, offsetWrite, 0x400);
            LoadPalette(gUnknown_08215C0C, 0, 0x100);
            sBerryBlenderData->field_1++;
        }
        break;
    case 2:
        {
            void* offsetRead = &ewram[0x10000];
            void* offsetWrite = (void*)(VRAM);
            u32 size = 0x2000;
            while (TRUE)
            {
                DmaCopy16(3, offsetRead, offsetWrite, 0x1000);
                offsetRead += 0x1000;
                offsetWrite += 0x1000;
                size -= 0x1000;
                if (size <= 0x1000)
                {
                    DmaCopy16(3, offsetRead, offsetWrite, size);
                    break;
                }
            }
            sBerryBlenderData->field_1++;
        }
        break;
    case 3:
        sub_800D238(gUnknown_08E6C920, &ewram[0x10000]);
        sBerryBlenderData->field_1++;
        break;
    case 4:
        sub_800D238(gUnknown_08E6D354, &ewram[0x13000]);
        sBerryBlenderData->field_1++;
        break;
    case 5:
        {
            void* offsetRead = &ewram[0x10000];
            void* offsetWrite = (void*)(VRAM + 0xE000);

            DmaCopy16(3, offsetRead, offsetWrite, 0x1000);
            sBerryBlenderData->field_1++;
        }
        break;
    case 6:
        {
            void* offsetRead = &ewram[0x11000];
            void* offsetWrite = (void*)(VRAM + 0xF000);

            DmaCopy16(3, offsetRead, offsetWrite, 0x1000);
            sBerryBlenderData->field_1++;
        }
        break;
    case 7:
        {
            u16 i;
            u16* palStore = (u16*)(&ewram[0x13000]);
            void* offsetRead;
            void* offsetWrite;

            for (i = 0; i < 640; i++)
            {
                *(palStore + i) |= 0x100;
            }
            offsetRead = &ewram[0x13000];
            offsetWrite = (void*)(VRAM + 0x6000);
            DmaCopy16(3, offsetRead, offsetWrite, 0x500);
            LoadPalette(gUnknown_0821602C, 0x80, 0x20);
            sBerryBlenderData->field_1++;
        }
        break;
    case 8:
        LoadSpriteSheet(&gUnknown_082163DC);
        LoadSpriteSheet(&gUnknown_082164F4);
        LoadSpriteSheet(&gUnknown_08216454);
        sBerryBlenderData->field_1++;
        break;
    case 9:
        LoadSpriteSheet(&gUnknown_08216540);
        LoadSpriteSheet(&gUnknown_08216574);
        LoadSpritePalette(&gUnknown_082163EC);
        LoadSpritePalette(&gUnknown_082163E4);
        sBerryBlenderData->field_1 = 0;
        return 1;
    }
    return 0;
}

void sub_804E4FC(void)
{
    REG_DISPCNT = 0x1341;
    REG_BG2CNT = 0x4880;
    REG_BG1CNT = 0xC0D;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
}

void sub_804E538(void)
{
    u8* field6F; //this temp value is needed to match

    sBerryBlenderData = (struct BerryBlenderData*)(&ewram[0x18000]);

    field6F = &sBerryBlenderData->field_6F;
    sBerryBlenderData->field_0 = 0;
    *field6F = 0;

    sub_804E884(gSpecialVar_0x8004);
    SetMainCallback2(sub_804E56C);
}

extern const struct WindowConfig gWindowConfig_81E6F68;
extern const u8 sBlenderSyncArrowsPos[][2];
extern const struct SpriteTemplate sBlenderSyncArrow_SpriteTemplate;

void sub_804E56C(void)
{
    s32 i;
    switch (sBerryBlenderData->field_0)
    {
    case 0:
        REG_DISPCNT = 0;
        ResetSpriteData();
        FreeAllSpritePalettes();
        SetVBlankCallback(NULL);
        SetUpWindowConfig(&gWindowConfig_81E6F68);
        InitMenuWindow(&gWindowConfig_81E6F68);
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_140 = 0;
        sBerryBlenderData->field_13E = 0;
        sBerryBlenderData->field_142 = 0x50;
        sBerryBlenderData->field_144 = 0;
        sBerryBlenderData->field_146 = 0;
        sBerryBlenderData->field_1 = 0;
        sub_8051474();
        break;
    case 1:
        if (sub_804E2EC())
        {
            for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
            {
                sBerryBlenderData->SyncArrowSpriteID[i] = CreateSprite(&sBlenderSyncArrow_SpriteTemplate, sBlenderSyncArrowsPos[i][0], sBlenderSyncArrowsPos[i][1], 1);
                StartSpriteAnim(&gSprites[sBerryBlenderData->SyncArrowSpriteID[i]], i + 8);
            }
            SetVBlankCallback(VBlankCB0_BerryBlender);
            sBerryBlenderData->field_0++;
        }
        break;
    case 2:
        BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
        sub_8051474();
        sBerryBlenderData->field_0++;
        break;
    case 3:
        sub_804E4FC();
        if (!gPaletteFade.active)
        {
            sBerryBlenderData->field_0++;
        }
        break;
    case 4:
        MenuDrawTextWindow(0, 14, 29, 19);
        MenuPrintMessage(gOtherText_BlenderChooseBerry, 1, 15);
        sBerryBlenderData->field_0++;
        break;
    case 5:
        if (MenuUpdateWindowText())
        {
            sBerryBlenderData->field_0++;
            BeginNormalPaletteFade(-1, 0, 0, 0x10, 0);
        }
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            sub_80A6978();
            sBerryBlenderData->field_0 = 0;
        }
        break;
    }
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void sub_804E738(struct Sprite* sprite)
{
    sprite->data1 += sprite->data6;
    sprite->data2 -= sprite->data4;
    sprite->data2 += sprite->data7;
    sprite->data0 += sprite->data7;
    sprite->data4--;

    if (sprite->data0 < sprite->data2)
    {
        sprite->data3 = sprite->data4 = sprite->data3 - 1;
        if (++sprite->data5 > 3)
            DestroySprite(sprite);
        else
            PlaySE(116);
    }
    sprite->pos1.x = sprite->data1;
    sprite->pos1.y = sprite->data2;
}

void sub_804E794(struct Sprite* sprite, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6)
{
    sprite->data0 = a3;
    sprite->data1 = a2;
    sprite->data2 = a3;
    sprite->data3 = a4;
    sprite->data4 = 10;
    sprite->data5 = 0;
    sprite->data6 = a5;
    sprite->data7 = a6;
    sprite->callback = sub_804E738;
}

extern const s16 gUnknown_08216594[][5];

void sub_804E7C0(u16 a0, u8 a1)
{
    u8 spriteID = sub_80A7DEC(a0 + 123, 0, 80, a1 & 1);
    sub_804E794(&gSprites[spriteID], gUnknown_08216594[a1][0], gUnknown_08216594[a1][1], gUnknown_08216594[a1][2], gUnknown_08216594[a1][3], gUnknown_08216594[a1][4]);
}

void Blender_CopyBerryData(struct BlenderBerry* berry, u16 itemID)
{
    const struct Berry *berryInfo = GetBerryInfo(itemID + 124);
    berry->field_0 = itemID;
    StringCopy(berry->name, berryInfo->name);
    berry->flavours[FLAVOUR_SPICY] = berryInfo->spicy;
    berry->flavours[FLAVOUR_DRY] = berryInfo->dry;
    berry->flavours[FLAVOUR_SWEET] = berryInfo->sweet;
    berry->flavours[FLAVOUR_BITTER] = berryInfo->bitter;
    berry->flavours[FLAVOUR_SOUR] = berryInfo->sour;
    berry->smoothness = berryInfo->smoothness;
}

extern u8 gUnknown_03004834;
extern const u8* const gUnknown_082162B8[];

void sub_804E884(u8 a0)
{
    int i;
    if (a0)
    {
        for (i = 0; i < 4; i++)
            gLinkPlayers[i].language = LANGUAGE_ENGLISH;
    }
    switch (a0)
    {
    case 0:
        gUnknown_03004834 = 0;
        break;
    case 1:
        gUnknown_03004834 = 1;
        sBerryBlenderData->field_88 = 2;
        StringCopy(gLinkPlayers[0].name, gSaveBlock2.playerName);
        StringCopy(gLinkPlayers[1].name, gUnknown_082162B8[0]);
        break;
    case 2:
        gUnknown_03004834 = 2;
        sBerryBlenderData->field_88 = 3;
        StringCopy(gLinkPlayers[0].name, gSaveBlock2.playerName);
        StringCopy(gLinkPlayers[1].name, gUnknown_082162B8[0]);
        StringCopy(gLinkPlayers[2].name, gUnknown_082162B8[1]);
        break;
    case 3:
        gUnknown_03004834 = 3;
        sBerryBlenderData->field_88 = 4;
        StringCopy(gLinkPlayers[0].name, gSaveBlock2.playerName);
        StringCopy(gLinkPlayers[1].name, gUnknown_082162B8[0]);
        StringCopy(gLinkPlayers[2].name, gUnknown_082162B8[1]);
        StringCopy(gLinkPlayers[3].name, gUnknown_082162B8[2]);
        break;
    }
}

void sub_804E990(void)
{
    s32 i;

    REG_DISPCNT = 0;
    sBerryBlenderData = (struct BerryBlenderData*)(&ewram[0x18000]);
    sBerryBlenderData->field_0 = 0;
    sBerryBlenderData->field_134 = 0;
    for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
    {
        sBerryBlenderData->field_80[i] = 0;
    }
    sub_804E884(gSpecialVar_0x8004);
    if (gSpecialVar_0x8004 == 0)
        SetMainCallback2(sub_804E9F8);
    else
        SetMainCallback2(sub_804F378);
}

extern u16 gScriptItemId;

extern const u8 gUnknown_082162EC[3][4];
extern const u16 gUnknown_082162F8[];
extern const u8 gUnknown_08216300[];
extern const struct SpriteTemplate gSpriteTemplate_8216548;

void sub_804E9F8(void)
{
    int i, j;
    switch (sBerryBlenderData->field_0)
    {
    case 0:
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetTasks();
        SetVBlankCallback(VBlankCB0_BerryBlender);
        SetUpWindowConfig(&gWindowConfig_81E6F68);
        InitMenuWindow(&gWindowConfig_81E6F68);
        gLinkType = 0x4422;
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_4E = 0;
        sBerryBlenderData->field_7E = 0;
        sBerryBlenderData->field_144 = 0;
        sBerryBlenderData->field_146 = 0;
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            sBerryBlenderData->field_70[i] = 0;
            for (j = 0; j < 3; j++)
            {
                sBerryBlenderData->field_14C[i][j] = 0;
            }
        }
        sBerryBlenderData->field_7C = 0;
        sBerryBlenderData->hitPitch = 0;
        sBerryBlenderData->arrowPos = 0;
        sBerryBlenderData->field_5A = 0;
        sBerryBlenderData->field_1 = 0;
        break;
    case 1:
        if (sub_804E2EC())
        {
            sBerryBlenderData->field_0++;
            sub_8051474();
        }
        break;
    case 2:
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            sBerryBlenderData->SyncArrowSprite2ID[i] = CreateSprite(&sBlenderSyncArrow_SpriteTemplate, sBlenderSyncArrowsPos[i][0], sBlenderSyncArrowsPos[i][1], 1);
            StartSpriteAnim(&gSprites[sBerryBlenderData->SyncArrowSprite2ID[i]], i + 8);
        }
        sBerryBlenderData->field_0++;
        break;
    case 3:
        BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
        sBerryBlenderData->field_0++;
        break;
    case 4:
        sub_804E4FC();
        if (!gPaletteFade.active)
        {
            sBerryBlenderData->field_0++;
        }
        break;
    case 5:
        MenuDrawTextWindow(0, 13, 29, 19);
        MenuPrint(gOtherText_LinkStandby3, 1, 14);
        sBerryBlenderData->field_0 = 8;
        sBerryBlenderData->field_130 = 0;
        break;
    case 8:
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_13C = 0;
        Blender_CopyBerryData(&sBerryBlenderData->blendedBerries[0], gScriptItemId);
        memcpy(gBlockSendBuffer, &sBerryBlenderData->blendedBerries[0], sizeof(struct BlenderBerry));
        sub_80084A4();
        sBerryBlenderData->field_130 = 0;
        break;
    case 9:
        if (sub_8007ECC())
        {
            ResetBlockReceivedFlags();
            if (GetMultiplayerId() == 0)
                sub_8007E9C(4);
            sBerryBlenderData->field_0++;
        }
        break;
    case 10:
        if (++sBerryBlenderData->field_130 > 20)
        {
            MenuZeroFillScreen();
            if (GetBlockReceivedStatus() == sub_8008198())
            {
                for (i = 0; i < GetLinkPlayerCount(); i++)
                {
                    memcpy(&sBerryBlenderData->blendedBerries[i], &gBlockRecvBuffer[i][0], sizeof(struct BlenderBerry));
                    sBerryBlenderData->field_80[i] = sBerryBlenderData->blendedBerries[i].field_0;
                }
                ResetBlockReceivedFlags();
                sBerryBlenderData->field_0++;
            }
        }
        break;
    case 11:
        sBerryBlenderData->field_88 = GetLinkPlayerCount();
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            if (sBerryBlenderData->field_13C == gUnknown_082162EC[sBerryBlenderData->field_88 - 2][i])
            {
                sub_804E7C0(sBerryBlenderData->field_80[sBerryBlenderData->field_13C], i);
                break;
            }
        }
        sBerryBlenderData->field_130 = 0;
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_13C++;
        break;
    case 12:
        if (++sBerryBlenderData->field_130 > 60)
        {
            if (sBerryBlenderData->field_13C >= sBerryBlenderData->field_88)
            {
                sBerryBlenderData->field_0++;
                sBerryBlenderData->arrowPos = gUnknown_082162F8[gUnknown_08216300[sBerryBlenderData->field_88 - 2]] - 22528;
            }
            else
                sBerryBlenderData->field_0--;
            sBerryBlenderData->field_130 = 0;
        }
        break;
    case 13:
        if (sub_8007ECC())
        {
            sBerryBlenderData->field_0++;
            sub_8051414(&sBerryBlenderData->field_168);
        }
        break;
    case 14:
        REG_DISPCNT |= 0x400;
        sBerryBlenderData->arrowPos += 0x200;
        sBerryBlenderData->field_142 += 4;
        if (sBerryBlenderData->field_142 > 255)
        {
            sBerryBlenderData->field_0++;
            sBerryBlenderData->field_142 = 256;
            sBerryBlenderData->arrowPos = gUnknown_082162F8[gUnknown_08216300[sBerryBlenderData->field_88 - 2]];
            REG_BG2CNT = 0x4882;
            sBerryBlenderData->field_130 = 0;
            sub_804F238();
            sub_804F2A8();
        }
        sub_8051414(&sBerryBlenderData->field_168);
        break;
    case 15:
        if (sub_8051B8C())
        {
            sBerryBlenderData->field_130 = 0;
            sBerryBlenderData->field_0++;
        }
        sub_8051414(&sBerryBlenderData->field_168);
        break;
    case 16:
        CreateSprite(&gSpriteTemplate_8216548, 120, -16, 3);
        sBerryBlenderData->field_0++;
        break;
    case 18:
        sBerryBlenderData->field_0++;
        break;
    case 19:
        sub_80084A4();
        sBerryBlenderData->field_0++;
        break;
    case 20:
        if (sub_8007ECC())
        {
            sub_8007E24();
            sBerryBlenderData->field_0++;
        }
        break;
    case 21:
        sBerryBlenderData->hitPitch = 128;
        sBerryBlenderData->field_12C = 0;
        SetMainCallback2(sub_80501FC);
        if (GetCurrentMapMusic() != 403)
        {
            sBerryBlenderData->field_178 = GetCurrentMapMusic();
        }
        PlayBGM(403);
        break;
    case 100:
        MenuDrawTextWindow(0, 13, 29, 19);
        MenuPrintMessage(gOtherText_LinkNotFound, 1, 15);
        sBerryBlenderData->field_0++;
        break;
    case 101:
        if (MenuUpdateWindowText())
            sBerryBlenderData->field_0++;
        break;
    case 102:
        if (!gPaletteFade.active)
            SetMainCallback2(c2_exit_to_overworld_1_continue_scripts_restart_music);
        break;
    }
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void sub_804F0F4(void)
{
    REG_DISPCNT = 0;

    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetTasks();

    SetVBlankCallback(VBlankCB0_BerryBlender);

    SetUpWindowConfig(&gWindowConfig_81E6F68);
    InitMenuWindow(&gWindowConfig_81E6F68);

    gLinkType = 0x4422;

    sBerryBlenderData->field_4E = 0;
    sBerryBlenderData->hitPitch = 0;
    sBerryBlenderData->arrowPos = 0;
    sBerryBlenderData->field_5A = 0;
    sBerryBlenderData->field_144 = 0;
    sBerryBlenderData->field_146 = 0;
    sBerryBlenderData->field_0++;
}

extern const u8 gUnknown_08216303[];

u8 sub_804F16C(u16 arrowPos, u8 a1)
{
    u32 var1 = (arrowPos / 256) + 24;
    u8 arrID = sBerryBlenderData->field_A2[a1];
    u32 var2 = gUnknown_08216303[arrID];

    if (var1 >= var2 && var1 < var2 + 48)
    {
        if (var1 >= var2 + 20 && var1 < var2 + 28)
            return 2;
        else
            return 1;
    }
    else
        return 0;
}

extern const u8 gUnknown_082165BC[][3];

void sub_804F1BC(u16 itemID, u8 a1, struct BlenderBerry* berry)
{
    u16 r4 = 0;
    u16 i;
    if (itemID == ITEM_ENIGMA_BERRY)
    {
        for (i = 0; i < 5; i++)
        {
            if (berry->flavours[r4] > berry->flavours[i])
                r4 = i;
        }
        r4 += 5;
    }
    else
    {
        r4 = itemID - 133;
        if (r4 >= 5)
            r4 = (r4 % 5) + 5;
    }
    for (i = 0; i < a1 - 1; i++)
    {
        sub_80516C4(i + 1, gUnknown_082165BC[r4][i] + 133);
    }
}

void sub_804F238(void)
{
    s32 i, j;
    for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
    {
        sBerryBlenderData->field_A2[i] = 0xFF;
        sBerryBlenderData->field_9A[i] = gUnknown_082162EC[sBerryBlenderData->field_88 - 2][i];
    }
    for (j = 0; j < BLENDER_MAX_PLAYERS; j++)
    {
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            if (sBerryBlenderData->field_9A[i] == j)
                sBerryBlenderData->field_A2[j] = i;
        }
    }
}

extern const u8 gUnknown_082162C4[];
extern const u8 gUnknown_082162D4[][2];

void sub_804F2A8(void)
{
    int i;
    for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
    {
        if (sBerryBlenderData->field_9A[i] != 0xFF)
        {
            u8* stringPtr = gStringVar1;

            sBerryBlenderData->SyncArrowSpriteID[sBerryBlenderData->field_9A[i]] = sBerryBlenderData->SyncArrowSprite2ID[i];
            StartSpriteAnim(&gSprites[sBerryBlenderData->SyncArrowSpriteID[sBerryBlenderData->field_9A[i]]], i);
            if (GetMultiplayerId() == sBerryBlenderData->field_9A[i])
                stringPtr = StringCopy(stringPtr, gUnknown_082162C4);
            StringCopy(stringPtr, gLinkPlayers[sBerryBlenderData->field_9A[i]].name);
            MenuPrint_PixelCoords(gStringVar1, gUnknown_082162D4[i][0] * 8 + 1, gUnknown_082162D4[i][1] * 8, 1);
        }
    }
}

extern const TaskFunc gUnknown_08216308[];

void sub_804F378(void)
{
    s32 i, j;
    switch (sBerryBlenderData->field_0)
    {
    case 0:
        sub_804F0F4();
        sub_80516C4(0, gScriptItemId);
        Blender_CopyBerryData(&sBerryBlenderData->blendedBerries[0], gScriptItemId);
        sub_804F1BC(gScriptItemId, sBerryBlenderData->field_88, &sBerryBlenderData->blendedBerries[0]);
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            sBerryBlenderData->field_70[i] = 0;
            for (j = 0; j < 3; j++)
            {
                sBerryBlenderData->field_14C[i][j] = 0;
            }
        }
        sBerryBlenderData->field_7C = 0;
        sBerryBlenderData->field_1 = 0;
        break;
    case 1:
        if (sub_804E2EC())
        {
            sBerryBlenderData->field_0++;
            sub_8051474();
        }
        break;
    case 2:
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            sBerryBlenderData->SyncArrowSprite2ID[i] = CreateSprite(&sBlenderSyncArrow_SpriteTemplate, sBlenderSyncArrowsPos[i][0], sBlenderSyncArrowsPos[i][1], 1);
            StartSpriteAnim(&gSprites[sBerryBlenderData->SyncArrowSprite2ID[i]], i + 8);
        }
        sBerryBlenderData->field_0++;
        break;
    case 3:
        BeginNormalPaletteFade(-1, 0, 0x10, 0, 0);
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_130 = 0;
        break;
    case 4:
        if (++sBerryBlenderData->field_130 == 2)
            sub_804E4FC();
        if (!gPaletteFade.active)
            sBerryBlenderData->field_0 = 8;
        break;
    case 8:
        sBerryBlenderData->field_0 = 11;
        sBerryBlenderData->field_13C = 0;
        break;
    case 11:
        for (i = 0; i < BLENDER_MAX_PLAYERS; i++)
        {
            u32 var = gUnknown_082162EC[sBerryBlenderData->field_88 - 2][i];
            if (sBerryBlenderData->field_13C == var)
            {
                sub_804E7C0(sBerryBlenderData->field_80[sBerryBlenderData->field_13C], i);
                break;
            }
        }
        sBerryBlenderData->field_130 = 0;
        sBerryBlenderData->field_0++;
        sBerryBlenderData->field_13C++;
        break;
    case 12:
        if (++sBerryBlenderData->field_130 > 60)
        {
            if (sBerryBlenderData->field_13C >= sBerryBlenderData->field_88)
            {
                sBerryBlenderData->arrowPos = gUnknown_082162F8[gUnknown_08216300[sBerryBlenderData->field_88 - 2]] - 22528;
                sBerryBlenderData->field_0++;
            }
            else
                sBerryBlenderData->field_0--;
            sBerryBlenderData->field_130 = 0;
        }
        break;
    case 13:
        sBerryBlenderData->field_0++;
        sub_804F238();
        PlaySE(43);
        sub_8051414(&sBerryBlenderData->field_168);
        break;
    case 14:
        REG_DISPCNT |= 0x400;
        sBerryBlenderData->arrowPos += 0x200;
        sBerryBlenderData->field_142 += 4;
        if (sBerryBlenderData->field_142 > 255)
        {
            sBerryBlenderData->field_0++;
            sBerryBlenderData->field_142 = 256;
            sBerryBlenderData->arrowPos = gUnknown_082162F8[gUnknown_08216300[sBerryBlenderData->field_88 - 2]];
            REG_BG2CNT = 0x4882;
            sBerryBlenderData->field_130 = 0;
            PlaySE(52);
            sub_804F2A8();
        }
        sub_8051414(&sBerryBlenderData->field_168);
        break;
    case 15:
        if (sub_8051B8C())
        {
            sBerryBlenderData->field_0++;
        }
        sub_8051414(&sBerryBlenderData->field_168);
        break;
    case 16:
        CreateSprite(&gSpriteTemplate_8216548, 120, -16, 3);
        sBerryBlenderData->field_0++;
        break;
    case 18:
        sBerryBlenderData->field_0++;
        break;
    case 19:
        sBerryBlenderData->field_0++;
        break;
    case 20:
        sBerryBlenderData->field_0++;
        break;
    case 21:
        sub_804F81C();
        sBerryBlenderData->hitPitch = 128;
        sBerryBlenderData->field_12C = 0;
        sBerryBlenderData->field_14B = 0;
        sBerryBlenderData->field_7E = 0;
        SetMainCallback2(sub_80501FC);

        for (i = 0; i < gSpecialVar_0x8004; i++)
        {
            sBerryBlenderData->field_148[i] = CreateTask(gUnknown_08216308[i], 10 + i);
        }

        if (GetCurrentMapMusic() != 403)
        {
            sBerryBlenderData->field_178 = GetCurrentMapMusic();
        }
        PlayBGM(403);
        PlaySE(53);
        Blender_ControlHitPitch();
        break;
    }
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void sub_804F81C(void)
{
    s32 i;
    for (i = 0; i < 4; i++)
    {
        gSendCmd[0] = 0;
        gSendCmd[2] = 0;
        gRecvCmds[0][i] = 0;
        gRecvCmds[2][i] = 0;
    }
}

void sub_804F844(u8 taskID)
{
   if(++gTasks[taskID].data[0] > gTasks[taskID].data[1])
   {
        gRecvCmds[2][gTasks[taskID].data[2]] = 0x2345;
        DestroyTask(taskID);
   }
}

void sub_804F890(u8 a0, u8 a1)
{
    u8 taskID = CreateTask(sub_804F844, 80);
    gTasks[taskID].data[1] = a1;
    gTasks[taskID].data[2] = a0;
}

void sub_804F8C8(u8 taskID)
{
    if (sub_804F16C(sBerryBlenderData->arrowPos, 1) == 2)
    {
        if (gTasks[taskID].data[0] == 0)
        {
            if (sBerryBlenderData->field_14B == 0)
            {
                u8 rand = Random() / 655;
                if (sBerryBlenderData->hitPitch < 500)
                {
                    if (rand > 75)
                        gRecvCmds[2][1] = 0x4523;
                    else
                        gRecvCmds[2][1] = 0x5432;
                    gRecvCmds[2][1] = 0x5432; // ???
                }
                else if (sBerryBlenderData->hitPitch < 1500)
                {
                    if (rand > 80)
                        gRecvCmds[2][1] = 0x4523;
                    else
                    {
                        u8 value = rand - 21;
                        if (value < 60)
                            gRecvCmds[2][1] = 0x5432;
                        else if (rand < 10)
                            sub_804F890(1, 5);
                    }
                }
                else if (rand <= 90)
                {
                    u8 value = rand - 71;
                    if (value < 20)
                        gRecvCmds[2][1] = 0x5432;
                    else if (rand < 30)
                        sub_804F890(1, 5);
                }
                else
                   gRecvCmds[2][1] = 0x4523;
            }
            else
                gRecvCmds[2][1] = 0x4523;

            gTasks[taskID].data[0] = 1;
        }
    }
    else
        gTasks[taskID].data[0] = 0;
}

void sub_804F9F4(u8 taskID)
{
    u32 var1 = (sBerryBlenderData->arrowPos + 0x1800) & 0xFFFF;
    u32 var2 = sBerryBlenderData->field_A2[2] & 0xFF;
    if ((var1 >> 8) > gUnknown_08216303[var2] + 20 && (var1 >> 8) < gUnknown_08216303[var2] + 40)
    {
        if (gTasks[taskID].data[0] == 0)
        {
            if (sBerryBlenderData->field_14B == 0)
            {
                u8 rand = Random() / 655;
                if (sBerryBlenderData->hitPitch < 500)
                {
                    if (rand > 66)
                        gRecvCmds[2][2] = 0x4523;
                    else
                        gRecvCmds[2][2] = 0x5432;
                }
                else
                {
                    u8 value;
                    if (rand > 65)
                        gRecvCmds[2][2] = 0x4523;
                    value = rand - 41;
                    if (value < 25)
                        gRecvCmds[2][2] = 0x5432;
                    if (rand < 10)
                        sub_804F890(2, 5);
                }

                gTasks[taskID].data[0] = 1;
            }
            else
            {
                gRecvCmds[2][2] = 0x4523;
                gTasks[taskID].data[0] = 1;
            }
        }
    }
    else
        gTasks[taskID].data[0] = 0;
}

void sub_804FB1C(u8 taskID)
{
    u32 var1, var2;

    #ifndef FAKEMATCHING
    asm("":::"r6");
    #endif // FAKEMATCHING

    var1 = (sBerryBlenderData->arrowPos + 0x1800) & 0xFFFF;
    var2 = sBerryBlenderData->field_A2[3] & 0xFF;
    if ((var1 >> 8) > gUnknown_08216303[var2] + 20 && (var1 >> 8) < gUnknown_08216303[var2] + 40)
    {
        if (gTasks[taskID].data[0] == 0)
        {
            if (sBerryBlenderData->field_14B == 0)
            {
                u8 rand = (Random() / 655);
                if (sBerryBlenderData->hitPitch < 500)
                {
                    if (rand > 88)
                        gRecvCmds[2][3] = 0x4523;
                    else
                        gRecvCmds[2][3] = 0x5432;
                }
                else
                {
                    u8 value;
                    if (rand > 60)
                        gRecvCmds[2][3] = 0x4523;
                    else if ((value = rand - 56) < 5)
                        gRecvCmds[2][3] = 0x5432;
                    if (rand < 5)
                        sub_804F890(3, 5);
                }
                gTasks[taskID].data[0] = 1;
            }
            else
            {
                gRecvCmds[2][3] = 0x4523;
                gTasks[taskID].data[0] = 1;
            }
        }
    }
    else
        gTasks[taskID].data[0] = 0;
}
