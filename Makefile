PROJECTNAME = face-off

SRCDIR          = src
KOS_ROMDISK_DIR = romdisk

CSOURCES = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c)))
OBJS     = $(CSOURCES:%.c=$(OBJDIR)/%.o)

# Dreamcast settings
KOS_CFLAGS  += -I${KOS_PORTS}/include/raylib
KOS_LDFLAGS += -lraylib -lGL -lkosutils #-O3
KOS_OBJDIR  =  obj/dc

# Linux settings
LINUX_CFLAGS  = -Wall -Wextra -pedantic
LINUX_LDFLAGS = -I/usr/local/include -L/usr/local/lib -lraylib -lGL -lm -g -O3
LINUX_OBJDIR  = obj/linux

# Rules for building for Dreamcast
dc: CC      = kos-cc
dc: TARGET  = $(PROJECTNAME).elf
dc: CFLAGS  = $(KOS_CFLAGS)
dc: LDFLAGS = $(KOS_LDFLAGS)
dc: OBJDIR  = $(KOS_OBJDIR)
dc: all

# Rules for building for Linux
linux: CC      = gcc
linux: TARGET  = $(PROJECTNAME)
linux: CFLAGS  = $(LINUX_CFLAGS)
linux: LDFLAGS = $(LINUX_LDFLAGS)
linux: OBJDIR  = $(LINUX_OBJDIR)
linux: all

all: prepare $(TARGET)
	
prepare:
	mkdir -p $(OBJDIR)
	
include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) 

run_dc: $(TARGET)
	$(KOS_LOADER) $(TARGET)

#dist_dc: $(TARGET)
#	-rm -f $(OBJS) romdisk.img
#	$(KOS_STRIP) $(TARGET)
dc_cdi:
	echo "Building DreamCast disc...\n"
    mkdcdisc -e $(TARGET) -o $(PROJECTNAME).cdi
