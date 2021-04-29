# A-GB-EMU

A-GB-EMU (Aaron's Gameboy Emulator) is an ongoing project where I delve deeper into emulation by attempting to make an emulator for the original gameboy.

## What's been done

-   First steps of hardware implementation in code
-   Opcode arrays (for fetch and decode)
-   Implementation of all Opcodes in code
-   Start of proper memory implementation

## Next steps

-   Continue working with memory (memory control, memory banking, boot rom, etc.)
-   Learn about Gameboy's display (PPU)
-   Get timings correct
-   Implement interrupts
-   Implement graphics
-   Connect to SDL and implement controls
-   Test Opcodes and implementation
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
