APP=app
SRCDIR=src
OBJDIR=obj
SUFFIXES=.c .cpp .s
LIBS=m GL glfw GLEW
RELEASEOPTS=-O2
DEBUGOPTS=-ggdb -DDEBUG

SUBDIRS=$(shell find $(SRCDIR) -type d | grep -v include)
INCLUDEDIRS=$(shell find $(SRCDIR) -type d -name include)
OBJSUBDIRS=$(subst $(SRCDIR),$(OBJDIR),$(SUBDIRS))

CC=gcc
CFLAGS=-std=c11 $(if $(DEBUG),$(DEBUGOPTS),$(RELEASEOPTS)) -Wall -Wextra -Wshadow $(foreach inc,$(INCLUDEDIRS), -I$(realpath $(inc)))
CXX=g++
CXXFLAGS=-std=c++11 $(if $(DEBUG),$(DEBUGOPTS),$(RELEASEOPTS)) -Wall -Wextra -Wshadow $(foreach inc,$(INCLUDEDIRS), -I$(realpath $(inc)))
LD=g++
LDFLAGS=$(if $(DEBUG),$(DEBUGOPTS),$(RELEASEOPTS)) $(addprefix -l,$(LIBS))

SRCS=$(foreach subd,$(SUBDIRS),$(foreach suf,$(SUFFIXES),$(wildcard $(subd)/*$(suf))))
HEADERS=$(foreach subd,$(INCLUDEDIRS),$(wildcard $(subd)/*.h))
OBJS=$(foreach subd,$(SUBDIRS),$(foreach suf,$(SUFFIXES),$(subst $(SRCDIR),$(OBJDIR),$(subst $(suf),.o,$(wildcard $(subd)/*$(suf))))))


all: $(APP)

$(APP): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) Makefile | $(OBJSUBDIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) Makefile | $(OBJSUBDIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s Makefile | $(OBJSUBDIRS)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJSUBDIRS):
	mkdir -p $@

tar: clean
	tar -zcvf ../$(APP).tar.gz .
	@mv ../$(APP).tar.gz $(APP).tar.gz

clean:
	@rm -rf $(OBJDIR) $(APP)

.PHONY: all tar clean
