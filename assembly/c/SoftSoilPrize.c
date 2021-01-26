#include <z64.h>
#include "BaseRupee.h"

ActorEnItem00* SoftSoilPrize_ItemSpawn(GlobalContext* ctxt, Actor* actor, u16 type) {
    ActorEnItem00* item = z2_fixed_drop_spawn(ctxt, &actor->currPosRot.pos, type);
    if (item == NULL) { // TODO items should be made to spawn regardless of inventory
        return item;
    }
    u16 giIndex = 0;
    u8 flag = actor->params & 0x7F;
    switch (ctxt->sceneNum) {
        case 0x07: // Grottos
            giIndex = 0x362; // Bean Grotto
            break;
        case 0x27: // Swamp Spider House
            if (flag == 0) {
                giIndex = 0x37D; // Rock
            } else {
                giIndex = 0x37E; // Gold Room
            }
            break;
        case 0x2B: // Deku Palace
            giIndex = 0x370;
            break;
        case 0x2D: // Termina Field
            if (flag == 0xE) {
                giIndex = 0x37F; // Stump
            } else if (flag == 0x5) {
                giIndex = 0x380; // Observatory
            } else if (flag == 0x14) {
                giIndex = 0x381; // South Wall
            } else {
                giIndex = 0x382; // Pillar
            }
            break;
        case 0x35: // Romani Ranch
            if (gSaveContext.perm.day <= 1) {
                giIndex = 0x37A;
            } else {
                giIndex = 0x374;
            }
            break;
        case 0x37: // Great Bay Coast
            giIndex = 0x372;
            break;
        case 0x41: // Doggy Racetrack
            giIndex = 0x371;
            break;
        case 0x59: // Stone Tower (Inverted)
            if (flag == 0) {
                giIndex = 0x37B; // Lower
            } else {
                giIndex = 0x37C; // Upper
            }
            break;
        case 0x60: // Secret Shrine
            giIndex = 0x373;
            break;
    }
    if (giIndex > 0) {
        Rupee_CheckAndSetGiIndex(&item->base, ctxt, giIndex);
    }
    return item;
}