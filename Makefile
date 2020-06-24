
EXE = fun
SRCDIR = src
OBJDIR = obj
HEADIR = include

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.opp,$(wildcard $(SRCDIR)/*.cpp))
C_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(HEADIR)/*.hpp) $(wildcard $(HEADIR)/*.h)

CXX = g++
.PHONY: clean all mtest

CXXFLAGS = $(subst HEADIR,$(HEADIR),-O2 -Wall -std=c++17 -IHEADIR -g)
LDFLAGS = 
FSANFLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined

all: $(EXE)

$(EXE): $(OBJECTS) $(C_OBJECTS)
	$(CXX) $(OBJECTS) $(C_OBJECTS) -o $(EXE) $(LDFLAGS) $(FSANFLAGS)

$(OBJDIR)/%.opp: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR) 
	$(CXX) $(CXXFLAGS) $(FSANFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR) 
	$(CXX) $(CXXFLAGS) $(FSANFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXE)

# mtest: all clean

remake:	clean all

# --- Hint ---:

# Source: https://makefiletutorial.com + official Makefile guide \
# 									( https://www.gnu.org/software/make/manual/make.html )

# Use backslash as shown above to split long lines (applies to code, not comments)

# Not to confuse, here's the syntax naming:

#		target(s) : prerequisites [dependencies, if they are files]
#			recipe

# When multiple targets specified, command(s) will run for each of them

# .PHONY [TARGETS]
#		- unties target name from files
# 		- using .PHONY several times stacks targets together as phony-marked

# targets : normal-prerequisites | order-only-prerequisites
# 		- normally, target is rebuilt is its prereq. is updated
# 		- order-only helps avoid that (ex.: create folder)

# addprefix PREFIX,[list]
#		- adds PREFIX to every element in list

# *, ?, [...] (wildcard characters)
# 		- similar to regex, but needs caution
# 		- expands in commands and in prereq.
# 		- DOES NOT expand when assigning a variable (however, it may expand later when using the variable)

# wildcard PATTERN
#		- forces to expand wildcard characters

#	 %
# 		- magic symbol that means 'anything'
# 		- use in regex matching, target names and wherever else you wish to
# 		- matches one time per line

# patsubst PATTERN,REPLACEMENT,TEXT
#		- replace REPLACEMENT with PATTERN in TEXT
# 		- note: use % as wildcard, i.e. "patsubst %.c,%.o,[text]" will turn *.c to *.o
# 		- use subst FROM,TO,TEXT to simply substitute FROM with TO

# $@, $<, $^, ... (automatic variables)
#		- $@ means 'current target name'
# 		- $< means 'first dependency name'
#		- $^ means 'all dependencies with spaces'
#		- $| means 'all order-only dependencies'
#		- see whole list on https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html#Automatic-Variables

# --- ---- ---ean:
	rm -rf obj
	rm -rf hw_01