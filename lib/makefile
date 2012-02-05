INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 
SRCDIR	= ./src
OBJDIR	= ./obj
BINDIR	= ./bin
TESTDIR = ./test

COMPILERFLAGS = -Wall -g
CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm 


# $< denotes the link.
# $@ denotes the target.
test: $(OBJDIR)/scene_voxel.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(LIBDIR) $< $(LIBRARIES)

$(OBJDIR)/scene_voxel.o: $(TESTDIR)/scene_voxel.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BINDIR)/voxel : $(OBJDIR)/voxel.o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)  

$(OBJDIR)/voxel.o : $(SRCDIR)/voxel.c
	$(CC) $(CFLAGS) -o $@ -c $<
