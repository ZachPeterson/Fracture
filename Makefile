PRODUCT_NAME       = Fractured
SOURCES            = main.c font.c level.c myLib.c dmaLib.c interruptLib.c soundLib.c spriteLib.c text.c buttonLib.c videoLib.c ./assets/music/gameSong.c ./assets/music/menuSong.c ./assets/music/entangledSong.c ./assets/music/loseSong.c ./assets/music/winSong.c ./assets/art/logo.c ./assets/art/spriteSheet.c ./assets/art/introMovie.c ./assets/art/menu.c ./assets/art/instructions.c ./assets/art/pauseScreen.c ./assets/art/winScreen.c ./assets/art/loseScreen.c ./assets/art/entangledScreen.c ./assets/art/level1Dimension0.c ./assets/art/level1Dimension0CollisionMap.c ./assets/art/level1Dimension1.c ./assets/art/level1Dimension1CollisionMap.c ./assets/art/level2Dimension0.c ./assets/art/level2Dimension0CollisionMap.c ./assets/art/level2Dimension1.c ./assets/art/level2Dimension1CollisionMap.c ./assets/sound/jump.c ./assets/sound/shift.c ./assets/art/smokeBackground.c

DKPATH             = C:/GBA/devkitARM/bin
CCPATH             = C:/cygwin/bin
VBASIM             = C:/GBA/VBA/vba.exe
FIND               = find
COPY               = cp -r

# --- File Names
ELF_NAME           = $(PRODUCT_NAME).elf
ROM_NAME           = $(PRODUCT_NAME).gba
BIN_NAME           = $(PRODUCT_NAME)

#MODEL              = -mthumb-interwork -mthumb
MODEL              = -mthumb-interwork -marm -mlong-calls #This makes interrupts work
SPECS              = -specs=gba.specs

# --- Archiver
AS                 = $(DKPATH)/arm-eabi-as
ASFLAGS            = -mthumb-interwork

# --- Compiler
CC                 = $(DKPATH)/arm-eabi-gcc
CFLAGS             = $(MODEL) -O2 -Wall -pedantic -Wextra -std=c99 -D_ROM=$(ROM_NAME) -D_VBA=$(VBASIM)
CC_WRAP            = $(CCPATH)/gcc
CFLAGS_WRAP        = -O3 -Wall -pedantic -D_ROM='"$(ROM_NAME)"' -D_VBA='"$(VBASIM)"'

# --- Linker
LD                 = $(DKPATH)/arm-eabi-gcc
LDFLAGS            = $(SPECS) $(MODEL) -lm

# --- Object/Executable Packager
OBJCOPY            = $(DKPATH)/arm-eabi-objcopy
OBJCOPYFLAGS       = -O binary

# --- ROM Fixer
GBAFIX             = $(DKPATH)/gbafix

# --- Delete
RM                 = rm -f

OBJECTS = $(filter-out gba_wrapper%,$(SOURCES:.c=.o))

# --- Main build target
all : build $(BIN_NAME)

run : build
	$(VBASIM) $(ROM_NAME)

build : UNZIP $(ROM_NAME)

$(BIN_NAME) : gba_wrapper.c
	$(CC_WRAP) $(CFLAGS_WRAP) -o $@ $^

# --- Build .elf file into .gba ROM file
$(ROM_NAME) : $(ELF_NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(ELF_NAME) $(ROM_NAME)
	$(GBAFIX) $(ROM_NAME)

# --- Build .o files into .elf file
$(ELF_NAME) : $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# -- Build .c files into .o files
$(OBJECTS) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


# ============ Common
UNZIP :
	-@$(FIND) . -iname "*.zip" -exec unzip -n {} \; -exec echo "This project must be rebuilt" \; -exec rm {} \;

clean:
	$(RM) $(ROM_NAME)
	$(RM) $(ELF_NAME)
	$(RM) $(BIN_NAME)
	$(RM) *.o
# ============ Common
