objlist := vpet utility random pet_video pet_sprites pet_logic pet_animation pet_main pet_minigame pet_foods
program_title = vpet
 
CC := gcc
LD := gcc
 
objdir := obj
srcdir := src
objlisto := $(foreach o,$(objlist),$(objdir)/$(o).o)
 
ifeq ($(OS),Windows_NT)
  CFLAGS := -Wall -O2 -std=gnu99 -ggdb
  LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
  LDFLAGS := -Wl,-subsystem,windows
else
  CFLAGS := -Wall -O2 -std=gnu99 `sdl2-config -ggdb
  LDLIBS := -lSDL2main -lSDL2 -lSDL2_image
  #LDFLAGS := -Wl
endif
 
vpet: $(objlisto)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
 
$(objdir)/%.o: $(srcdir)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
 
.PHONY: clean
 
clean:
	-rm $(objdir)/*.o
