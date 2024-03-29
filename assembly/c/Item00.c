#include <stdbool.h>
#include <z64.h>
#include "Misc.h"
#include "MMR.h"
#include "Player.h"
#include "BaseRupee.h"

static u16 collectableTable[0x80];

u16 GetTweakedCollectableSceneIndex(u16 sceneIndex) {
    switch (sceneIndex) {
        case 0x1C: // Path to Mountain Village
            if (gSaveContext.perm.weekEventReg.mountainCleared) {
                return 0x71;
            }
            break;
        case 0x5C: // Snowhead
            if (gSaveContext.perm.weekEventReg.mountainCleared) {
                return 0x72;
            }
            break;
    }
    return sceneIndex;
}

void Item00_LoadCollectableTable(GlobalContext* ctxt) {
    if (MISC_CONFIG.internal.vanillaLayout) {
        return;
    }

    u16 sceneIndex = GetTweakedCollectableSceneIndex(ctxt->sceneNum);

    u32 index = MISC_CONFIG.shorts.collectableTableFileIndex;
    DmaEntry entry = dmadata[index];

    u32 start = entry.romStart + (sceneIndex * 0x100);

    z2_RomToRam(start, &collectableTable, sizeof(collectableTable));
}

u16 Item00_CollectableFlagToGiIndex(u16 collectableFlag) {
    if (MISC_CONFIG.internal.vanillaLayout) {
        return 0;
    }

    return collectableTable[collectableFlag];
}

void Item00_Constructor(ActorEnItem00* actor, GlobalContext* ctxt) {
    if (actor->collectableFlag != 0) {
        u16 giIndex = Item00_CollectableFlagToGiIndex(actor->collectableFlag);
        if (giIndex > 0) {
            Rupee_SetGiIndex(&actor->base, giIndex);
            u16 drawGiIndex = MMR_GetNewGiIndex(ctxt, 0, giIndex, false);
            Rupee_SetDrawGiIndex(&actor->base, drawGiIndex);
        }
    }
}

bool Item00_GiveItem(ActorEnItem00* actor, GlobalContext* ctxt) {
    u16 giIndex = Rupee_GetGiIndex(&actor->base);
    if (giIndex == 0) {
        return false;
    }
    actor->pickedUp = true;
    if (!MMR_GiveItem(ctxt, &actor->base, giIndex)) {
        Player_Pause(ctxt);
    }
    return true;
}

u16 Item00_GetDespawnDelayAmount(ActorEnItem00* actor) {
    u16 giIndex = Rupee_GetDrawGiIndex(&actor->base);
    if (giIndex > 0) {
        GetItemEntry* entry = MMR_GetGiEntry(giIndex);
        if (entry->item == 0xB0) {
            // Ice Trap
            return 0x40;
        }
    }
    return 0xF;
}

void Item00_BeforeBeingPickedUp(ActorEnItem00* actor, GlobalContext* ctxt) {
    u16 giIndex = Rupee_GetDrawGiIndex(&actor->base);
    if (giIndex > 0) {
        GetItemEntry* entry = MMR_GetGiEntry(giIndex);
        if (entry->item == 0xB0) {
            // Ice Trap
            z2_PlayLoopingSfxAtActor(&actor->base, 0x31A4);
        }
    }
}

extern bool forceSpawn;
s8 Item00_CanBeSpawned(u16 params) {
    s8 result = params & 0xFF;
    if (forceSpawn) {
        return result;
    }
    u16 collectableFlag = (params >> 8) & 0x7F;
    if (collectableFlag > 0) {
        u16 giIndex = Item00_CollectableFlagToGiIndex(collectableFlag);
        if (giIndex > 0) {
            return result;
        }
    }
    return z2_item_can_be_spawned(params & 0xFF);
}
