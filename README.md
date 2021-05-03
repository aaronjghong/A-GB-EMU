# A-GB-EMU

A-GB-EMU (Aaron's Gameboy Emulator) is an ongoing project where I delve deeper into emulation by attempting to make an emulator for the original gameboy.

## What's been done

-   First steps of hardware implementation in code
-   Opcode arrays (for fetch and decode)
-   Implementation of all Opcodes in code
-   Start of proper memory implementation
-   Implemented Interrupts (not confident in my implementation)
-   Implemented Timer (not confident in my implementation)
-   First steps of PPU

## Next steps

-   Continue working on PPU
-   Connect to SDL and implement controls
-   Get timings correct
-   Continue working with memory (memory control, memory banking, boot rom, etc.)
-   Test Opcodes (ON HOLD)
    -   After testing Opcode implementations, I've realized that there were many discrepancies between a working GB emulator and my own due to the lack implemenation of the PPU and proper memory control. This will be returned to once the PPU and Memory are sufficiently implemented to do so.
-   Clean up inconsistencies within Opcode Functions and look to simplify function calls
-   Implement sound

## Resources I'm using

If you would like to give this a go, I'd highly recommend the following resources:

-   https://gekkio.fi/files/gb-docs/gbctr.pdf
-   https://izik1.github.io/gbops/index.html
-   https://robdor.com/2016/08/10/gameboy-emulator-half-carry-flag/
-   https://meganesulli.com/generate-gb-opcodes/
-   https://forums.nesdev.com/viewtopic.php?t=15944 (For DAA instruction)
-   https://github.com/AntonioND/giibiiadvance/blob/master/docs/TCAGBD.pdf
-   https://hacktix.github.io/GBEDG
-   https://codeslinger.co.uk

## Blargg Test Results

-   01-special
    -   09 06 06 0C Failed
-   02-interrupts
    -   ? -> no output
-   03-op sp, hl
    -   08 08 Failed
-   04-op r, imm
    -   ? -> no output
-   05-op rp
    -   ? -> no output
-   06-ld r, r
    -   02 03 Failed
-   07-jr,jp,call,ret,rst
    -   04 0c 04 0c Failed
-   08-misc instrs
    -   02 02 Failed
-   09-op r, r
    -   0F 0B 00 0B 01 0B 02 0B 03 0B 04 0B 05 0B 07 0B 00 0B 01 0B 02 0B 03 0B 04 0B 05 0B 07 Failed
-   10-bit ops
    -   Passed?
-   11-op a, (hl)
    -   0B 06 0B 06 07 Failed

_No output either means a pass or the emulator ran into an infinite loop_
