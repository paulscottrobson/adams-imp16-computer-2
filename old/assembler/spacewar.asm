; **** created from the disassembled code. DO NOT EDIT IT *****

; ***************************************************************************************************************************************
; ***************************************************************************************************************************************
;
;                                                           SCOTT'S SPACE WAR
;                                                           -----------------
;
;   Original Program Code by Scott Adams (C) 1975
;   IMP-16 Home Computer Hardware design by Richard Adams
;   Transcription, Coding and Comment expansion by Paul Robson 2014.
;
;   For more information see : http://exoticsciences.com
;
;   This document is publicly distributable, however ownwership of the Software and Hardware design of the IMP-16C Computer
;   remain with Scott and Richard Adams.
;
;   Paul Robson (paul@robsons.org.uk)
;
; ***************************************************************************************************************************************
; ***************************************************************************************************************************************

Start:
                    jmp     @StartAddress                                                       ; go to startup code.

Interrupt:                                                                              ; go to interrupt routine
                    jmp     InterruptRoutine

StartAddress:                                                                           ; address of startup code.
                    word    Main

AKeyboard:  word    ReadKeyboard
AInit:      word    Initialise

TorpedoID:  word    0                                                                   ; currently moving torpedo.

ConstFF80:  word    $FF80                                                               ; constants.
ConstZero:  word    0

AReadCrt:   word    ReadCRT                                                             ; routine addreses.
AWriteCrt:  word    WriteCRT
AClear:     word    Clear
AShowCollision:word ShowCollision
ACanMove:   word    CanMove
AMoveShip:  word    MoveShip
AMoveTorpedo:word   MoveTorpedo

CharShip1:  word    $43                                                                 ; C graphics characters used in game.
CharShip2:  word    $45                                                                 ; E
CharTorpedo:word    $6B                                                                 ; +
CharPlanet: word    $3B                                                                 ; O with horizontal bar, from 2513 L/C chip
CharAsteroid:word   $6A                                                                 ; *
CharBlank:  word    $60                                                                 ; space.


Const3:     word    3                                                                   ; more constants

APlayerWin2:word    PlayerWin2
Speed:      word    $10                                                                 ; torpedo speed.
AClearChar: word    ClearToCharacter

CanFire1:   word    1
CanFire2:   word    1
MaxTorpedoes:word   $10                                                                 ; max number of torpedos of all types

Hold:       word    0
Freeze:     word    0

Main:       jmp     @InitVector
InitVector: word    NewGame

Ship1:      word    0                                                                   ; Line << 12 + Col << 7
            word    $1000                                                               ; current direction
            word    0                                                                   ; timer
            word    0                                                                   ; speed

Ship2:      word    0                                                                   ; Line << 12 + Col << 7
            word    $1000                                                               ; current direction
            word    0                                                                   ; timer
            word    0                                                                   ; speed

Torpedos:   word    $1100                                                               ; Line << 12 + Col << 7 *PLANETS*
            word    0                                                                   ; Direction
            word    0                                                                   ; Timer
            word    $7FFF                                                               ; Number of moves

            word    $7F00
            word    0
            word    0
            word    $7FFF

            word    $D500
            word    0
            word    0
            word    $7FFF

            word    $6A00
            word    0
            word    0
            word    $7FFF

            word    $A600
            word    0
            word    0
            word    $7FFF

            word    $F700                                                               ; heading NE  * ASTEROIDS *
            word    $F080
            word    0
            word    $7FFF

            word    $FF80
            word    $EF80                                                               ; heading NW
            word    0
            word    $7FFF

            word    $9900
            word    $1000                                                               ; heading S
            word    0
            word    $7FFF

            word    $0210                                                               ; heading N
            word    $F000
            word    0
            word    $7FFF

;
;                       4C-70 more torpedoes same layout, these are user torpedoes e.g. the ones that are fired.
;
;                       actually $67 - $10 torpedoes, 4 bytes each, start at $28
;

                    org     $6D
SpaceKeyChar:word   $20                                                                 ; *keyboard* space code.
APlayerWin: word    PlayerWin                                                           ; code vector.
Ship:       word    0                                                                   ; current ship.

Speeds:     word    0                                                                   ; speed rates
            word    $20
            word    $12
            word    0


DirUp:      word    $F000                                                               ; directional constants
DirRight:   word    $0080
DirDown:    word    $1000
DirLeft:    word    $FF80

; ***************************************************************************************************************************************
;
;                                               List of Keyboard Commands
;
;   Note: these are not standard ASCII. The computer CRT display has a pair of 2513 chips, chars 40-6F are mapped on to the standard
;         chip, and chars 00-3F are mapped on to the lower case chip. This is quite a rare chip AFAICS but was used on some Apple IIs.
;         The ASCII -> IMP translation is (charCode & 0x3F) | 0x40 (not for lower case !)
;
; ***************************************************************************************************************************************

Commands1:  word    'E'                                                                 ; speed controls.
            word    'D'
            word    'X'
            word    'Q'                                                                 ; fire
            word    'W'                                                                 ; directions
            word    'S'
            word    'Z'
            word    'A'

Commands2:  word    'U'                                                                 ; speed controls.
            word    'H'
            word    'N'
            word    'P'                                                                 ; fire
            word    'I'                                                                 ; directions
            word    'K'
            word    'M'
            word    'J'

Const7F:    word    $7F                                                                 ; constants
Const7:     word    7
FireDelay:  word    $80                                                                 ; time between firing.
AFireTorpedo:word   FireTorpedo
Const2:     word    2

; ***************************************************************************************************************************************
;
;                                                               Interrupt Routine.
;
; ***************************************************************************************************************************************

        InterruptRoutine:
            pflg    6,0                                                                 ; pulse 6 is clocked AR $00 (clear irq latch/disable vsync ?)
            pflg    6,-64                                                               ; pulse 6 is clocked AR $40 (enable vsync interrupt ?)
            jmp     IncFrameCount                                                       ; go bump frame counter and exit

                    org     $95
        CmdListEnd:
            word    Commands1+15                                                        ; used by keyboard in routine.

; ***************************************************************************************************************************************
;
;                                                               Move ship R1.
;
; ***************************************************************************************************************************************

                    org     $B0
            word    MoveShip                                                            ; not actually used but left for completeness.

        MoveShip:
            st      1,Ship                                                              ; Save ship number
            jsr     @ACanMove                                                           ; can we move the ship ? (sets R2)
            boc     5,MoveShip2                                                         ; if returns non zero then we can and it has.
            rts                                                                         ; otherwise we can't

        MoveShip2:
            ld      1,3(2)                                                              ; copy speed to timer
            st      1,2(2)                                                              ; resetting it for the next time round
        UpdateShip:
            ld      3,0(2)                                                              ; read character at current position.
            jsr     @AReadCrt
            aisz    0,-$60                                                              ; skip if $60 read (e.g. it is blank)
            jsr     @AShowCollision                                                     ; if not blank, then call the hit routine to check.

            ld      2,Ship                                                              ; read the ship number into R2
            ld      0,CharShip1-1(2)                                                    ; read the character to use in
            jsr     @AWriteCrt                                                          ; and write to CRT.
            rts

; ***************************************************************************************************************************************
;
;                               Come here when hit. Fills the screen with the winning character
;
; ***************************************************************************************************************************************

                    org     $C0

PlayerWin:  ld      2,Ship                                                              ; read ship number
            ld      0,CharShip1-1(2)                                                    ; read its associated character
        PlayerWin2:
            jsr     @AClearChar                                                         ; fill the screen with the character
            li      0,1                                                                 ; set the freeze flag.
            st      0,Freeze
            jsr     @AKeyboard                                                          ; read keyboard, which will wait till unfreeze
            pull    0                                                                   ; fix the stack up (asm is pull 1, code is pull 0)
                                                                                                ; but it doesn't actually matter :)
            jmp     Start                                                               ; and restart.

; ***************************************************************************************************************************************
;
;                                                   Print R0 on the CRT at location R3
;
; ***************************************************************************************************************************************

                    org     $C9
        WriteCrt:

            push    0                                                                   ; save R0 on the stack
            ld      0,ConstFF80                                                         ; get mask for address
            rand    3,0                                                                 ; and location with mask (e.g. clear lower 7 bits)
            pflg    7,3                                                                 ; set to write to CRT.
            pull    0                                                                   ; restore the old R0
            rout    $2A                                                                 ; sets the AR to address + $2A (TODO: Why $2A)
                                                                                                ; then outputs the character in R0 to it.
            rts                                                                         ; return.

; ***************************************************************************************************************************************
;
;                                                           Exit Interrupt Routine
;
; ***************************************************************************************************************************************

                    org     $F0
        IncFrameCount:
            isz     FrameCount                                                          ; increment frame counter
            rti                                                                         ; and exit (RTI twice because of skip)
            rti

FrameCount: word    0                                                                   ; count of vsyncs

; ***************************************************************************************************************************************
;
;                                                   Patch to start of Fire Torpedo code
;
; ***************************************************************************************************************************************

        PatchFireTorpedo:
            ld      2,MaxTorpedoes                                                      ; check get the maximum number of torpedoes
            aisz    2,9                                                                 ; add 9 to that. (?)
            jmp     @GoPatchEnd                                                         ; and go back to the main code.
GoPatchEnd: word    EndPatchFireTorpedo

; ***************************************************************************************************************************************
;
;                                                       Patch to end of Ship Hit Code
;
; ***************************************************************************************************************************************

        HitPatch:
            ld      3,Ship1                                                             ; get the location of ship1
            ld      0,CharShip1                                                         ; and redisplay it
            jsr     @AWriteCrt

            ld      3,Ship2                                                             ; same for ship 2
            ld      0,CharShip2
            jsr     @AWriteCrt

            rts


; ***************************************************************************************************************************************
;
;                                                               Clear Screen
;
; ***************************************************************************************************************************************

                    org     $103
        Clear:
            li      0,$60                                                               ; code for blank
        ClearToCharacter:
            li      3,0                                                                 ; add 128 to 0 32 x 16 times .....
        ClearLoop:
            jsr     @AWriteCrt                                                          ; write blanks to the display
            add     3,x80                                                               ; the address is stepped by 0x80 between characters
                                                                                                ; e.g. the upper 9 bits address 32 x 16 characters.
            skne    3,ConstZero                                                         ; exit if zero (e.g. whole screen cleared)
            rts
            jmp     ClearLoop                                                           ; otherwise keep clearing the screen.

x80:        word    $80                                                                 ; constant used above.

; ***************************************************************************************************************************************
;
;                                                     Read In Character at R3 to R0
;
; ***************************************************************************************************************************************

        ReadCrt:
            pflg    7,3                                                                 ; set to access the CRT Memory (TODO: Why $05 below ?)
            rin     $05                                                                 ; read in ($05 is the 6 bit 1's complement of $2A, mode = read)
            and     0,x7F                                                               ; mask off lower 7 bits
            rts

x7F:        word    $007F                                                               ; word used to mask keyboard input.


; ***************************************************************************************************************************************
;
;                                   Deal with Hits R3 contains location hit - show collision
;
; ***************************************************************************************************************************************

                    org     $112
        ShowCollision:
            st      3,Hold                                                              ; save location in holding register
            li      0,$30                                                               ; probably ~ . Could be 3C, but that's a box graphic
            jsr     @AClearChar                                                         ; clear the whole screen to that graphic.

            ld      3,Hold                                                              ; get location to clear
            ld      0,CharBlank                                                         ; and clear it.
            jsr     @AWriteCrt

            li      0,$40                                                               ; delay about 1 s
        HitDelay:
            skg     0,FrameCount                                                        ; by waiting for the frame count to reach 64
            jmp     EndHitDelay
            Jmp     HitDelay
        EndHitDelay:
            jsr     @AClear                                                             ; clear the screen
            jmp     HitPatch                                                            ; go to the patch to write stuff back out

; ***************************************************************************************************************************************
;
;                                                   Move current torpedo (ID in torpedoID)
;
; ***************************************************************************************************************************************

                    org     $120
        MoveTorpedo:
            ld      1,TorpedoID                                                         ; get current torpedo number
            aisz    1,2                                                                 ; add 2 to torpedo number, can't skip
                                                                                                ; the torpedos are 2 4 bit records after the ship
            jsr     @ACanMove                                                           ; can the torpedo move (sets up R2)
            boc     5,MoveTorpedo2                                                      ; if R0 != 0 it has moved
            rts

        MoveTorpedo2:
            dsz     3(2)                                                                ; decrement the number of moves, skip if reached zero.
            jmp     TorpedoAlive
            rts                                                                         ; if zero, the torpedo is dead.

        TorpedoAlive:
            ld      3,0(2)                                                              ; where has it moved to ?
            jsr     @AReadCrt                                                           ; read the screen there.
            aisz    0,-$60                                                              ; is it blank ?
            jmp     TorpedoDead                                                         ; if not, then it's dead and may have killed something

            ld      0,TorpedoID                                                         ; get the torpedo ID.
            aisz    0,-5                                                                ; is it one of the first five ? (planets)
            boc     2,NotPlanet                                                         ; if >= 0 then it isn't a planet

            ld      0,CharPlanet                                                        ; write a planet back there

        TorpedoForever:
            isz     3(2)                                                                ; make sure the asteroids always move, their counters
                                                                                                ; would reach zero eventually.
            jmp     EndMoveTorpedo

        NotPlanet:
            aisz    0,-4                                                                ; is it an asteroid ?
            boc     2,NotAsteroid

            ld      0,CharAsteroid                                                      ; write the asteroid back
            jmp     TorpedoForever                                                      ; and make it live forever.

        NotAsteroid:                                                                            ; write out the torpedo character.
            ld      0,CharTorpedo

        EndMoveTorpedo:
            jsr     @AWriteCrt                                                          ; write to the position on the CRT
            nop
            nop
            ld      1,Speed                                                             ; copy speed into timer
            st      1,2(2)                                                              ; is this right QUERY - set to seperate speeds.
            jmp     TorpedoDone

        TorpedoDead:
            li      1,0                                                                 ; reduce moves to zero, e.g. kill it
            st      1,3(2)


            aisz    0,$60                                                               ; fix character back after AISZ at $12A
            skne    0,CharShip1                                                         ; check if it a ship
            jsr     @APlayerWin2                                                        ; if so display winner (QUERY: Jump/Stack ?)
            skne    0,CharShip2                                                         ; Hits2 does a pull to fix up the stack but
            jsr     @APlayerWin2                                                        ; after the JSR we will be two deep ?
            jmp     Dnz

                    org     $148
        Dnz:
            jsr     @AShowCollision                                                     ; check to see if hit
            rts                                                                         ; and exit.


        TorpedoDone:
            st      2,Hold                                                              ; save pointer.
            ld      0,ConstFF80                                                         ; and it with $FF80
            skaz    0,Hold                                                              ; checking addresses haven't gone haywire.
            halt                                                                        ; if they have, stop.
            rts

; ***************************************************************************************************************************************
;
;                                           Fire torpedo, Index of source in R2
;
; ***************************************************************************************************************************************

        FireTorpedo:
            jmp     PatchFireTorpedo                                                    ; go to the torpedo patch.
        EndPatchFireTorpedo:                                                                    ; puts MaxTorpedoes + 9 into R2 (QUERY -9)

            st      2,Hold                                                              ; save this in holding store
            li      2,Torpedos+$20                                                      ; point R2 to the last of the 'fixed' torpedoes
                                                                                                ; so AISZ will go to first unused record.
        FindUnusedTorpedo:
            aisz    2,4                                                                 ; go to the next record
            ld      0,3(2)                                                              ; read the life, e.g. is torpedo in use
            boc     1,FoundFreeTorpedo                                                  ; if AC0 zero, then create new torpedo
            dsz     Hold                                                                ; checked all of them (which makes +9 unlikely !)
            jmp     FindUnusedTorpedo
            rts                                                                         ; so cannot fire.

        FoundFreeTorpedo:
            st      1,0(2)                                                              ; save location
            st      3,1(2)                                                              ; save direction
            li      1,1                                                                 ; set timer to 1
            st      1,2(2)
            li      1,$0F                                                               ; how many moves it has (argument $0F or $20)
            st      1,3(2)
            rts                                                                         ; and return.

; ***************************************************************************************************************************************
;
;  Check if movement of object ID R1 is possible, if not return zero, if so delete old, move it, and return non-zero (actually CharBlank)
;
; ***************************************************************************************************************************************

        CanMove:
            rcpy    2,1                                                                 ; copy R1 (index) to R2
            shl     2,2                                                                 ; multiply r2 x 4
            aisz    2,Ship1-4                                                           ; now R2 points to the record

            li      0,0
            li      3,0
            skne    3,3(2)                                                              ; skip if lives counter > 0 e.g. can move
            rts                                                                         ; can not move.

            dsz     2(2)                                                                ; decrement move counter, skip if that becomes zero.
            rts

            ld      3,0(2)                                                              ; get the address into R3
            ld      0,CharBlank                                                         ; and write a blank character there.
            jsr     @AWriteCrt

            ld      1,0(2)                                                              ; get physical address on screen
            add     1,1(2)                                                              ; add the direction to it
            and     1,ConstFF80                                                         ; push into possible range.
            st      1,0(2)                                                              ; update the physical address
            rts

; ***************************************************************************************************************************************
;
;                                                                   Initialise Game
;
; ***************************************************************************************************************************************

                    org     $17C
        Initialise:
            ld      0,$00
            st      0,Ship1
            add     0,$8D                                                               ; QUERY adds the contents of $80  li 0,$80 == bottom right
            st      0,Ship2                                                             ; if sign extended.

            jsr     @AShowCollision                                                     ; QUERY AClear - Hit call makes no sense.

            ld      1,MaxTorpedoes                                                      ; set a counter of the number of torpedoes.
            st      1,Hold

            li      2,Torpedos                                                          ; point R2 to torpedoes
            li      1,1
            li      3,2
        AInitLoop1:
            st      1,2(2)                                                              ; save 1 to torpedo speed.
            st      3,3(2)                                                              ; save 2 to torpedo moves
            aisz    2,4
            skne    2,torpedoEndPreset                                                  ; skip if reset all the preset torpedoes (up to $4C)
            li      3,0                                                                 ; moves will be zero for the rest
            aisz    1,$10                                                               ; but bump up the speed (QUERY too much ?JSv has it $01)
            dsz     Hold                                                                ; done all the torpedoes ?
            jmp     AInitLoop1                                                          ; keep on going.
            st      3,Ship1+3                                                           ; set the ship speeds to zero
            st      3,Ship2+3
            st      3,Freeze                                                            ; clear the freeze flag.
            rts

torpedoEndPreset: word $4C                                                              ; address of end of preset torpedoes

; ***************************************************************************************************************************************
;
;                                                               Begin a new game.
;
; ***************************************************************************************************************************************

                    org     $19A
        NewGame:
            pflg    6,-64                                                               ; pulse interrupt on.
            li      0,0                                                                 ; clear the frame count.
            st      0,FrameCount
            sflg    1,0                                                                 ; turn IMP-16 interrupts on.
        WaitFrame:
            skne    0,FrameCount                                                        ; skip if frame count has been incremented
            jmp     WaitFrame                                                           ; otherwise keep waiting.

            jsr     @AInit                                                              ; set everything up

; ***************************************************************************************************************************************
;
;                                                               Main Loop
;
; ***************************************************************************************************************************************

        Begin:
            jsr     @AKeyboard                                                          ; read the keyboard.

            ld      1,MaxTorpedoes                                                      ; get the number of torpedoes
            st      1,TorpedoID                                                         ; save as current
        MoveTorpedoLoop:
            jsr     @AMoveTorpedo                                                       ; move torpedo.
            dsz     TorpedoID                                                           ; do all the torpedos
            jmp     MoveTorpedoLoop

            li      1,1                                                                 ; move both ships.
            jsr     @AMoveShip
            li      1,2
            jsr     @AMoveShip

            dsz     CanFire1                                                            ; is p1 allowed to fire yet
            jmp     TestFire2
            isz     CanFire1                                                            ; if so, leave 1 in there to show this
        TestFire2:
            dsz     CanFire2                                                            ; is p2 allowed to fire yet
            jmp     TestFireOver
            isz     CanFire2                                                            ; again, if can fire leave as 1.
        TestFireOver:
            jmp     Begin                                                               ; and go round again .....

; ***************************************************************************************************************************************
;
;                                                               Keyboard reading routine
;
; ***************************************************************************************************************************************

                    org     $1B9
        ReadKeyboardPatch:
            skne    0,SpaceKeyChar                                                      ; go to freeze if key is blank (e.g. space)
            jmp     FreezeCheck
            jmp     EInt
        ReadKeyboard:
            li      0,0                                                                 ; clear the data read value
            li      1,0                                                                 ; clear the 60Hz frame count
            st      1,FrameCount
        WaitKeyLoop:
            skne    1,FrameCount                                                        ; if frame count clear (no 60Hz) go to pulse flag.
            jmp     ReadKeyboard2                                                       ; no interrupt yet, so go read the keyboard
            jmp     ReadKeyboardPatch                                                   ; otherwise check for blank (e.g. space) pressed.

        ReadKeyboard2:
            pflg    7,1                                                                 ; pulse the keyboard flag
            boc     14,WaitKeyLoop                                                      ; if no key press found, go round again.

            nop                                                                         ; no-op, short delay to settle.
            rin                                                                         ; read the key into R0.
            and     0,Const7f                                                           ; clear bit 7 (and any other upper bits)
            jmp     WaitKeyLoop                                                         ; and go round again.

        EInt:
            jmp     EIntNext
        EIntNext:
            boc     1,Done                                                              ; if no key pressed (R0 == 0) then exit.
            ld      2,CmdListEnd                                                        ; point to last command.
            li      1,$10                                                               ; count of commands to check
            st      1,Hold
        CheckNextCommand:
            skne    0,0(2)                                                              ; character found ?
            jmp     Match                                                               ; yes, process it.
            aisz    2,-1                                                                ; point to previous character
            nop                                                                         ; ignore skip
            dsz     Hold                                                                ; if checked all of them.
            jmp     CheckNextCommand                                                    ; skip, otherwise do the next.
            jmp     Done                                                                ; no command match

        Match:
            dsz     Hold                                                                ; decrement hold (e.g. the command number goes 00-0F)
            nop                                                                         ; no skip
            ld      2,Hold                                                              ; read that into 2.
            shr     2,3                                                                 ; divide by 8
            shl     2,2                                                                 ; now 2 is 0 or 4, depending

            li      1,7                                                                 ; mask for command
            ld      3,Hold                                                              ; re read the key
            rand    3,1                                                                 ; 3 now contains the lower 3 bits of the command
                                                                                                ; 2 the player, 0 or 4 (e.g. offset into the Ship1/Ship2)

            skg     3,Const2                                                            ; if command > 2 (e.g. a speed command)
            jmp     SetSpeed                                                            ; then set the speed.

            skne    3,Const3                                                            ; if command = 3 (e.g. fire)
            jmp     Fire

            nop                                                                         ; direction code 4-7
            ld      1,DirUp-4(3)                                                        ; access the relevant entry in the direction table
            st      1,Ship1+1(2)                                                        ; store in the direction word
            jmp     Done                                                                ; and exit

        SetSpeed:
            nop                                                                         ; speed 0-2
            ld      1,Speeds(3)                                                         ; get the speed.
            st      1,Ship1+3(2)                                                        ; store in both speed and move down counter
            nop
            st      1,Ship1+2(2)
            jmp     Done                                                                ; and exit

        Fire:
            ld      3,Ship1+1(2)                                                        ; AC3 = Direction
            ld      1,Ship1(2)                                                          ; AC1 = Location
            jmp     MovementPatch                                                       ; go to the patch code

        EndMovementPatch:
            ld      0,CanFire1(2)                                                       ; look at the fire counter
            aisz    0,-1                                                                ; is it 1, skip if so
            jmp     Done                                                                ; if > 1, cannot fire

            ld      0,FireDelay                                                         ; set the fire counter rate
            st      0,CanFire1(2)                                                       ; in the fire delay counter
            jsr     @AFireTorpedo                                                       ; fire torpedo, current player.
            jmp     Done                                                                ; and end.

        FreezeCheck:
            ld      0,Freeze                                                            ; toggle freeze bit.
            li      1,1
            rxor    0,1
            st      0,Freeze
        Done:
            ld      0,Freeze                                                            ; read freeze into 0.
            boc     5,ReadKeyboard                                                      ; if non-zero, then keep reading, this is wait mode.
            rts

        MovementPatch:
            radd    1,3                                                                 ; this is the location of the position moving 2
            shr     2,2                                                                 ; convert R2 back from offset to ID (divide by 4)
            jmp     EndMovementPatch

; ***************************************************************************************************************************************
;
;   Version     Completed       Notes
;   -------     ---------       -----
;   1.0         23-3-2014       This is the first version that can be split into binary and source and will re-assemble to itself using
;                               the python IMP16-C Assembler.
;   1.01        24-3-2014       Very minor changes to labels and comments.
;   1.02        25-3-2014       AND at $16D missing :)
;
; ***************************************************************************************************************************************

;   Questions
;   ---------
;
; the screen fills with '@' characters. Appears to be a transcription bug. In Initialize (0x17C) there is a jsr *ASomething that is transcribed as 2C0B
; but this is the 'hit' test routine (probably why the other emulator fills the screen with at signs). There is another routine which clears the screen
; which would be addressed via 2C0A (on the 0x0C9 page) which is never used except after a hit and logically fits here.

; did the torpedoes move at the same rate ? initially their speeds are set to different values, but they are all set to the same speed in the move loop
; (Richard thinks the rate varied) - the JSON version has this as '1'. Leave for the time being.

; the start positions of the ships. If you replace the LD/ADD with LI and AISZ they will go in to the top left and bottom right.

; why when writing or reading memory are the lower bits of AR set to $05 and $2A which are 6 bit complements of each other - is this setting R/W gating ?

; is 15 an event driven keyboard latch ? pulse 15 clocks the contents of that latch in and then clears it, e.g. it is returning key depressions not the
; current keyboard state ?

; +9/-9 torpedoes bug ?
