objlist := vpet utility game random video sprites
program_title = vpet
 
CC := gcc
LD := gcc
 
objdir := obj
srcdir := src
objlisto := $(foreach o,$(objlist),$(objdir)/$(o).o)
 
ifeq ($(OS),Windows_NT)
  CFLAGS := -Wall -O2 -std=gnu99 -ggdb
  LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
  LDFLAGS := -Wl,-subsystem,windows
else
  CFLAGS := -Wall -O2 -std=gnu99 `sdl2-config -ggdb
  LDLIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
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
