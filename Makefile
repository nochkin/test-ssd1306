CC = clang
CFLAGS = -O0 -g -Wall -I.
MKDIR = mkdir
RM = rm
FIND = find

OBJDIR = .obj

SRC = bcm2835.c main.c glcdfont.c
SRCXX = Adafruit_GFX.cpp Adafruit_SSD1306.cpp
INC = bcm2835.h
INCXX = SSD1306.h Adafruit_GFX.h Adafruit_SSD1306.h
OBJ = $(patsubst %,$(OBJDIR)/%,$(SRC:.c=.c.o))
OBJXX = $(patsubst %,$(OBJDIR)/%,$(SRCXX:.cpp=.cpp.o))
BINARY = ssd1306-mpc

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

all: $(OBJDIR) $(BINARY)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(OBJDIR)/%.c.o: %.c $(INC)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.cpp.o: %.cpp $(INCXX)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BINARY): $(OBJ) $(OBJXX)
	$(CC) $(CFLAGS) -o $(BINARY) $^

clean:
	$(FIND) $(OBJDIR) -type f -rm {} \;
	$(RM) -rf $(BINARY)

