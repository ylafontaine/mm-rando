;==================================================================================================
; Spawn rupee
;==================================================================================================

.headersize G_EN_HIT_TAG_DELTA

; Replaces:
;   or      a1, s1, r0
;   jal     0x800A7730
;   or      a2, r0, r0
.org 0x80BE2158
    lw      a1, 0x0014 (sp)
    jal     HitTag_RupeeSpawn
    or      a2, s0, r0

; Replaces:
;   .dw     0x00000000
.org 0x80BE21EC
    .dw     HitTag_Draw
