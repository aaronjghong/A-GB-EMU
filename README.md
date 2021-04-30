# A-GB-EMU

A-GB-EMU (Aaron's Gameboy Emulator) is an ongoing project where I delve deeper into emulation by attempting to make an emulator for the original gameboy.

## What's been done

-   First steps of hardware implementation in code
-   Opcode arrays (for fetch and decode)
-   Implementation of all Opcodes in code
-   Start of proper memory implementation

## Next steps

-   Test Opcodes
-   Continue working with memory (memory control, memory banking, boot rom, etc.)
-   Learn about Gameboy's display (PPU)
-   Get timings correct
-   Implement interrupts
-   Implement graphics
-   Connect to SDL and implement controls
-   Implement sound
-   Clean up inconsistencies within Opcode Functions and look to simplify function calls (for ex. use h_B and h_C instead of getting msb from BC getter function)

## Resources I'm using

If you would like to give this a go, I'd highly recommend the following resources:

-   https://gekkio.fi/files/gb-docs/gbctr.pdf
-   https://izik1.github.io/gbops/index.html
-   https://robdor.com/2016/08/10/gameboy-emulator-half-carry-flag/
-   https://meganesulli.com/generate-gb-opcodes/
-   https://forums.nesdev.com/viewtopic.php?t=15944 (For DAA instruction)
-   https://github.com/AntonioND/giibiiadvance/blob/master/docs/TCAGBD.pdf

## Blargg Test Results

-   01-special
    -   POP AF Failed #5
-   02-interrupts
    -   EI Failed #2
-   03-op sp, hl
    -   09 08 08 08 Failed
-   04-op r, imm
    -   06 0E Failed
-   05-op rp
    -   09 09 09 Failed
-   06-ld r, r
    -   02 03 Failed
-   08-misc instrs
    -   02 02 01 Failed
-   09-op r, r
    -   00 01 02 03 04 05 07 08 09 0A 0B 0C 0D 0F 0F 0B 00 0B 01 0B 02 0B 03 0B 04 0B 05 0B 07 0B 00 0B 01 0B 02 0B 03 0B 04 0B 05 0B 07 Failed
-   10-bit ops
    -   Passed
-   11-op a, (hl)
    -   06 0E 0B 06 0B 06 07 Failed
