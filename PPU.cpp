#include "PPU.h"

PPU::PPU(CPU* cpu)
    :ref_cpu(cpu),
    r_LCDC(ref_cpu->h_MEMORY.memory[0xFF40]),
    r_STAT(ref_cpu->h_MEMORY.memory[0xFF41]),
    r_SCY(ref_cpu->h_MEMORY.memory[0xFF42]),
    r_SCX(ref_cpu->h_MEMORY.memory[0xFF43]),
    r_LY(ref_cpu->h_MEMORY.memory[0xFF44]),
    r_LYC(ref_cpu->h_MEMORY.memory[0xFF45]),
    r_DMA(ref_cpu->h_MEMORY.memory[0xFF46]),
    r_BGP(ref_cpu->h_MEMORY.memory[0xFF47]),
    r_OBP0(ref_cpu->h_MEMORY.memory[0xFF48]),
    r_OBP1(ref_cpu->h_MEMORY.memory[0xFF49]),
    r_WX(ref_cpu->h_MEMORY.memory[0xFF4A]),
    r_WY(ref_cpu->h_MEMORY.memory[0xFF4B])
{
    //h_VRAM = &cpu->h_MEMORY.memory[0x8000];
    //h_OAM = &cpu->h_MEMORY.memory[0xFE00];
    //g_REGISTERS = &cpu->h_MEMORY.memory[0xFF40];
    //ref_cpu = cpu;

    // r_LCDC = ref_cpu->h_MEMORY.memory[0xFF40];
    // r_STAT = ref_cpu->h_MEMORY.memory[0xFF41];
    // r_SCY = ref_cpu->h_MEMORY.memory[0xFF42];
    // r_SCX = ref_cpu->h_MEMORY.memory[0xFF43];
    // r_LY = ref_cpu->h_MEMORY.memory[0xFF44];
    // r_LYC = ref_cpu->h_MEMORY.memory[0xFF45];
    // r_DMA = ref_cpu->h_MEMORY.memory[0xFF46];
    // r_BGP = ref_cpu->h_MEMORY.memory[0xFF47];
    // r_OBP0 = ref_cpu->h_MEMORY.memory[0xFF48];
    // r_OBP1 = ref_cpu->h_MEMORY.memory[0xFF49];
    // r_WX = ref_cpu->h_MEMORY.memory[0xFF4A];
    // r_WY = ref_cpu->h_MEMORY.memory[0xFF4B];
}

PPU::~PPU(){

}

void PPU::init(){
    // SDL_Init(SDL_INIT_EVERYTHING);
    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    // SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer);
    // SDL_SetWindowSize(window, 480, 432);
    // SDL_SetWindowResizable(window, SDL_TRUE);
    //texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 480, 432);
    memset(g_SCREENDATA, 0, sizeof(g_SCREENDATA));
}

void PPU::updateGraphics(int cycles){
    setMode();
    if(lcdEnabled()){
        cycles_scanLine -= cycles;
        if (cycles_scanLine <= 0){
            // Cpu cycles for 1 scanline have passed, proceed to next scanline
            r_LY++;
            cycles_scanLine = 456;

            if(r_LY > 153){
                // Reset current scanline back to 0
                r_LY = 0;
            }
            else if(r_LY >= 144){
                // call for vblank interrupt
                ref_cpu->interrupts->requestInterrupt(0);
            }
            else if(r_LY < 144){
                drawScanLine();
            }
        }
    }
}

void PPU::drawScanLine(){
    if((r_LCDC & 0b00000001) != 0){
        renderTiles();
    }
    if((r_LCDC & 0b00000010) != 0){
        renderSprites();
    }
}

void PPU::renderTiles(){
    bool notSigned = true;
    uint16_t tileDataMethod = 0;    //  8000 method or 8800 method
    uint16_t backgroundMemoryAddress = 0;   //  $9800 or $9C00
    bool windowOn = false;  // Window covers bg, therefore we either draw window, or bg
    uint8_t yPos = 0;   // To determine which row of the 32x32 tile map is currently being drawn
    uint16_t tileRow = 0;

    // Check if window is enabled
    if((r_STAT & 0b00100000) != 0){
        if(r_WY <= r_LY){
            // If window is enabled in LCDC and the scanline is at or below the window's starting y position, enable window
            windowOn = true;
        }
    }
    
    // Get Tile Data Method
    if((r_STAT & 0b00010000) != 0){
        tileDataMethod = 0x8000;
        notSigned = true;
    }
    else{
        tileDataMethod = 0x8800;
        notSigned = false;
    }

    // Get Background/Window Map Address
    if(windowOn){
        // Get information from window bit
        if((r_STAT & 0b01000000) != 0) backgroundMemoryAddress = 0x9C00;
        else backgroundMemoryAddress = 0x9800;
    }
    else{
        // Get information from background bit
        if((r_STAT & 0b00001000) != 0) backgroundMemoryAddress = 0x9C00;
        else backgroundMemoryAddress = 0x9800; 
    }
    // Adjust current y position
    if(windowOn) yPos = r_LY - r_WY;
    else yPos = r_SCY + r_LY; 

    tileRow = (uint8_t(yPos / 8) * 32);

    for(int i = 0; i < 160; i++){
        uint8_t xPos = i + r_SCX;
        if(windowOn){
            if(i >= r_WX){
                // Adjust current pixel being drawn to window-x offset
                xPos = i - r_WX;
            }
        }
        // Get the tile column from the current pixel number
        uint16_t tileColumn = xPos/8;
        int16_t tileNumber;

        // Get appropriate tile id
        uint16_t tileAddress = tileColumn + tileRow + backgroundMemoryAddress;
        if(notSigned) tileNumber = ref_cpu->h_MEMORY.readMemory(tileAddress);
        else tileNumber = int8_t(ref_cpu->h_MEMORY.readMemory(tileAddress));

        // Get tile location depending on method (8000 vs 8800)
        uint16_t tileLoc = tileDataMethod;
        if(notSigned) tileLoc += tileNumber * 0x10;
        else tileLoc += (tileNumber + 0x80) * 0x10;

        // Fetch Tile Data with offset
        uint8_t line = 2 * (yPos % 8);
        uint8_t dataLow = ref_cpu->h_MEMORY.readMemory(tileLoc + line);
        uint8_t dataHigh = ref_cpu->h_MEMORY.readMemory(tileLoc + line + 1);

        // Get color and push to screen
        int colorBit = xPos % 8;    // Get x co-ord of pixel in tile being drawn
        colorBit -= 7;  // Pixels are stored in reverse, (1st pixel is in bit 7 of dataX)
        colorBit *= -1; // colorBit was negative, therefore make it positive

        uint8_t mask = 1 << colorBit;   // Create mask to get wanted bit in data

        uint8_t upper = dataHigh & mask ? 1 : 0;    // Get individual bits
        uint8_t lower = dataLow & mask ? 1 : 0;

        uint8_t colorID = (upper << 1) | lower; // Combine bits
        uint8_t color = getColor(colorID, 0xFF47);
        int red, green, blue;
        switch (color)
        {
            case 0: red = 255; green = 255; blue = 255; break;      // White
            case 1: red = 0xCC; green = 0xCC; blue = 0xCC; break;   // L-grey
            case 2: red = 0x77; green = 0x77; blue = 0x77; break;   // D-grey
            case 3: red = 0; green = 0; blue = 0; break;            // Black
        }
        g_SCREENDATA[i][r_LY][0] = red;
        g_SCREENDATA[i][r_LY][1] = green;
        g_SCREENDATA[i][r_LY][2] = blue;
    }
}

void PPU::renderSprites(){
    bool elongatedSprites = false;
    if((r_STAT & 0b00000100) != 0) elongatedSprites = true;
    for(int i = 0; i < 40; i++){    // Up to 40 sprites on screen
        // Get sprite info from spriate attribute table (OAM Memory)
        uint8_t location = i * 4;   // Each sprite data is 4 bytes of info
        uint8_t yPos = ref_cpu->h_MEMORY.readMemory(0xFE00 + location) - 16;
        uint8_t xPos = ref_cpu->h_MEMORY.readMemory(0xFE00 + location + 1) - 8;
        uint8_t tileLoc = ref_cpu->h_MEMORY.readMemory(0xFE00 + location + 2);
        uint8_t attributes = ref_cpu->h_MEMORY.readMemory(0xFE00 + location + 3);

        // Get whether the sprite is flipped or not
        bool xFlip = false;
        bool yFlip = false;
        if(attributes & 0b01000000) yFlip = true;
        if(attributes & 0b00100000) xFlip = true;

        int dimX = 8;
        int dimY = elongatedSprites ? 16 : 8;

        if((r_SCY >= yPos) && (r_SCY < (yPos + dimY))){
            // The scanline is somewhere within the sprite, proceed to draw
            int line = r_SCY - yPos;    // Line is y-offset to start drawing sprite from
            if(yFlip){
                // Read backwards if y-flip
                line -= dimY;
                line *= -1;
            }
            line *= 2;   // 2bpp
            uint16_t spriteAddr = (0x8000 + (tileLoc * 16)) + line;
            uint8_t dataLow = ref_cpu->h_MEMORY.readMemory(spriteAddr);
            uint8_t dataHigh = ref_cpu->h_MEMORY.readMemory(spriteAddr + 1);
            for(int pixel = 7; pixel >= 0; pixel--){
                int colorBit = pixel;
                if(xFlip){
                    // Read backwards if x-flip
                    colorBit -= 7;
                    colorBit *=1 ;
                }
                uint8_t mask = 1 << colorBit;
                
                uint8_t upper = dataHigh & mask ? 1 : 0;    // Get individual bits
                uint8_t lower = dataLow & mask ? 1 : 0;

                uint8_t colorID = (upper << 1) | lower; // Combine bits
                uint8_t color = getColor(colorID, attributes & 0b0001000 ? 0xFF49 : 0xFF48);
                int red, green, blue;
                switch (color)
                {
                    case 0: red = 255; green = 255; blue = 255; break;      // White
                    case 1: red = 0xCC; green = 0xCC; blue = 0xCC; break;   // L-grey
                    case 2: red = 0x77; green = 0x77; blue = 0x77; break;   // D-grey
                    case 3: red = 0; green = 0; blue = 0; break;            // Black
                }

                int pixelLoc = 0 - pixel;
                pixelLoc += 7;
                pixelLoc += xPos;

                // To avoid illegal drawing
                if((r_SCY < 0) || (r_SCY < 143) || (pixelLoc < 0) || (pixelLoc > 159)) continue;   

                g_SCREENDATA[xPos][r_LY][0] = red;
                g_SCREENDATA[xPos][r_LY][1] = green;
                g_SCREENDATA[xPos][r_LY][2] = blue;

            }
        }
    }
}

uint8_t PPU::getColor(uint8_t colorID, uint16_t address){
    // Get palette
    uint8_t palette = ref_cpu->h_MEMORY.readMemory(address);
    
    // Get appropriate palette bits from colorID
    uint8_t low = 2 * colorID;
    uint8_t high = (2 * colorID) + 1;
    uint8_t lowMask = 1 << low;
    uint8_t highMask = 1 << high;

    // Get color from palette bits
    uint8_t lower = palette & lowMask ? 1 : 0;
    uint8_t upper = palette & highMask ? 1 : 0;
    uint8_t color = (upper << 1) | lower;

    //  00 - White
    //  01 - Light Grey
    //  10 - Dark Gret
    //  11 - Black

    return color;
}

void PPU::setMode(){
    if(!lcdEnabled()){
        // Reset for when lcd is enabled
        cycles_scanLine = 456;
        r_LY = 0;
        r_STAT &= 0b11111100;
        r_STAT |= 0b00000001;
        // LCD either enters mode 0 or 1, set to enter mode 1 for now
        return;
    }
    uint8_t currentMode = r_STAT & 0b00000011;
    uint8_t mode = 0;
    bool interruptNeeded;

    if(r_LY >= 144){
        mode = 0b00000001;
        interruptNeeded = r_STAT & 0b00010000;
    }
    else{
        int bounds_mode2 = 456-80;
        int bounds_mode3 = bounds_mode2 - 172;
        if(cycles_scanLine >= bounds_mode2){
            // mode 2
            mode = 0b00000010;
            interruptNeeded = r_STAT & 0b00100000;
        }
        else if(cycles_scanLine>= bounds_mode3){
            // mode 3
            mode = 0b00000011;
        }
        else{
            // mode 0
            mode = 0b00000000;
            interruptNeeded = r_STAT & 0b00001000;
        }
    }
    // Set new mode and check if interrupt is needed
    r_STAT = (r_STAT & 0b11111100) | mode;
    if((currentMode != mode) && interruptNeeded){
        ref_cpu->interrupts->requestInterrupt(1);
    }
    if(r_LY == r_LYC){
        // Check for coincidence flag and interrupt, handle accordingly
        r_STAT |= 0b00000100;
        if((r_STAT & 0b01000000) != 0){
            ref_cpu->interrupts->requestInterrupt(1);
        }
    }
    else{
        // Coincidence flag is false
        r_STAT &= 0b11111011;
    }
}

bool PPU::lcdEnabled(){
    return (r_LCDC & 0b10000000) ? true : false;
}