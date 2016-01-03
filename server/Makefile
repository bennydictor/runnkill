$(shell mkdir -p .make)
$(shell touch .make/options)
$(shell touch .make/config.mk)

include .make/config.mk

SUFFIXES 			:= .c .cpp .s
DEPENDS_SUFFIXES	:= .c .cpp

SOURCES_SUBDIRS	:= $(shell find src -type d ! -path 'src/include*')
ALL_SOURCES 	:= $(foreach subd,$(SOURCES_SUBDIRS),$(foreach suf,$(SUFFIXES),$(wildcard $(subd)/*$(suf))))
C_SOURCES 		:= $(foreach subd,$(SOURCES_SUBDIRS),$(foreach suf,$(SUFFIXES),$(wildcard $(subd)/*.c)))
CXX_SOURCES 	:= $(foreach subd,$(SOURCES_SUBDIRS),$(foreach suf,$(SUFFIXES),$(wildcard $(subd)/*.cpp)))
LD_SOURCES 		:= $(NAME)
AS_SOURCES 		:= $(foreach subd,$(SOURCES_SUBDIRS),$(foreach suf,$(SUFFIXES),$(wildcard $(subd)/*.s)))
HEADERS_SUBDIRS	:= $(shell find src -type d -path 'src/include*')
HEADERS 		:= $(foreach subd,$(HEADERS_SUBDIRS),$(wildcard $(subd)/*.h))
OBJECTS_SUBDIRS	:= $(foreach subd,$(SOURCES_SUBDIRS),$(subst src,obj,$(subd)))
OBJECTS 		:= $(foreach src,$(ALL_SOURCES),$(subst src,obj,$(src).o))
DEPENDS_SUBDIRS	:= $(foreach subd,$(SOURCES_SUBDIRS),$(subst src,.make/depends,$(subd)))
DEPENDS_SOURCES	:= $(foreach subd,$(SOURCES_SUBDIRS),$(foreach suf,$(DEPENDS_SUFFIXES),$(wildcard $(subd)/*$(suf))))
DEPENDS 		:= $(foreach src,$(DEPENDS_SOURCES),$(subst src,.make/depends,$(src).d))
OPTIONS_SUBDIRS	:= $(foreach subd,$(SOURCES_SUBDIRS),$(subst src,.make/buildopts,$(subd)))
OPTIONS 		:= $(foreach src,$(ALL_SOURCES),$(subst src,.make/buildopts,$(src).o))

OPTS 		:=	$(shell cat .make/options | grep -E '^[A-Z]+=[01]$$'	| sed 's/=[01]//')
OPTS_ON 	:=	$(shell cat .make/options | grep -E '^[A-Z]+=1$$'		| sed 's/=[01]//')
OPTS_OFF	:=	$(shell cat .make/options | grep -E '^[A-Z]+=0$$'		| sed 's/=[01]//')

ALLOPTSVARS 	:=	$(foreach opt,$(OPTS),$(opt)_ON_CFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_OFF_CFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_ON_CXXFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_OFF_CXXFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_ON_LDFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_OFF_LDFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_ON_ASFLAGS) \
					$(foreach opt,$(OPTS),$(opt)_OFF_ASFLAGS)
OPTSVARS 		:=	$(foreach opt,$(OPTS_ON),$(opt)_ON_CFLAGS) \
					$(foreach opt,$(OPTS_OFF),$(opt)_OFF_CFLAGS) \
					$(foreach opt,$(OPTS_ON),$(opt)_ON_CXXFLAGS) \
					$(foreach opt,$(OPTS_OFF),$(opt)_OFF_CXXFLAGS) \
					$(foreach opt,$(OPTS_ON),$(opt)_ON_LDFLAGS) \
					$(foreach opt,$(OPTS_OFF),$(opt)_OFF_LDFLAGS) \
					$(foreach opt,$(OPTS_ON),$(opt)_ON_ASFLAGS) \
					$(foreach opt,$(OPTS_OFF),$(opt)_OFF_ASFLAGS)
SHOULD_DEFINE	:=	$(ALLOPTSVARS) NAME LIBS CC CFLAGS CXX CXXFLAGS LD LDFLAGS AS ASFLAGS
UNDEF_OPTS		:=	$(foreach opt,$(SHOULD_DEFINE),$(if $(shell echo $(origin $(opt)) | grep undefined),$(opt),))

ifneq "$(strip $(UNDEF_OPTS))" ""
$(error Define this variables in .make/flags.mk: $(UNDEF_OPTS))
endif


CFLAGS 		+= $(foreach opt,$(OPTS_ON),$($(opt)_ON_CFLAGS))
CXXFLAGS 	+= $(foreach opt,$(OPTS_ON),$($(opt)_ON_CXXFLAGS))
LDFLAGS 	+= $(foreach opt,$(OPTS_ON),$($(opt)_ON_LDFLAGS))
ASFLAGS 	+= $(foreach opt,$(OPTS_ON),$($(opt)_ON_ASFLAGS))

CFLAGS 		+= $(foreach opt,$(OPTS_OFF),$($(opt)_OFF_CFLAGS))
CXXFLAGS 	+= $(foreach opt,$(OPTS_OFF),$($(opt)_OFF_CXXFLAGS))
LDFLAGS 	+= $(foreach opt,$(OPTS_OFF),$($(opt)_OFF_LDFLAGS))
ASFLAGS 	+= $(foreach opt,$(OPTS_OFF),$($(opt)_OFF_ASFLAGS))

CFLAGS 		+= -Isrc/include
CXXFLAGS 	+= -Isrc/include
LDFLAGS		+= $(addprefix -l,$(LIBS))

CFLAGS		:= $(shell echo $(CFLAGS) 	| sed -E 's/\s{2,}/ /')
CXXFLAGS	:= $(shell echo $(CXXFLAGS)	| sed -E 's/\s{2,}/ /')
LDFLAGS		:= $(shell echo $(LDFLAGS)	| sed -E 's/\s{2,}/ /')
ASFLAGS		:= $(shell echo $(ASFLAGS)	| sed -E 's/\s{2,}/ /')

C_COMPILE	:= $(CC) $(CFLAGS)
CXX_COMPILE	:= $(CXX) $(CXXFLAGS)
LD_COMPILE	:= $(LD) $(LDFLAGS)
AS_COMPILE	:= $(AS) $(ASFLAGS)

CHANGED_SOURCES	:=	$(foreach src,$(C_SOURCES),$(if $(shell echo $(C_COMPILE) | diff - $(subst src,.make/buildopts,$(src) 2>&1)),$(src),)) \
					$(foreach src,$(CXX_SOURCES),$(if $(shell echo $(CXX_COMPILE) | diff - $(subst src,.make/buildopts,$(src) 2>&1)),$(src),)) \
					$(foreach src,$(AS_SOURCES),$(if $(shell echo $(AS_COMPILE) | diff - $(subst src,.make/buildopts,$(src)) 2>&1),$(src),))
CHANGED_NAME 	:= $(if $(shell echo $(LD_COMPILE) | diff - .make/buildopts/$(NAME) 2>&1),$(NAME),)
CHANGED_OBJECTS	:= $(foreach src,$(CHANGED_SOURCES),$(subst src,obj,$(src).o))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	@echo $(LD) $(LDFLAGS) >.make/buildopts/$(NAME)

obj/%.c.o: src/%.c | $(OBJECTS_SUBDIRS) $(OPTIONS_SUBDIRS)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo $(CC) $(CFLAGS) >$(subst src,.make/buildopts,$<)

obj/%.cpp.o: src/%.cpp | $(OBJECTS_SUBDIRS) $(OPTIONS_SUBDIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo $(CXX) $(CXXFLAGS) >$(subst src,.make/buildopts,$<)

obj/%.s.o: src/%.s | $(OBJECTS_SUBDIRS) $(OPTIONS_SUBDIRS)
	$(AS) $(ASFLAGS) $< -o $@
	@echo $(AS) $(ASFLAGS) >$(subst src,.make/buildopts,$<)

.make/depends/%.c.d: src/%.c | $(DEPENDS_SUBDIRS)
	@set -e; \
	$(CC) $(CFLAGS) -M $< > $@.tmp; \
	sed -E 's,(.*).o\s*:,$(subst src,obj,$<.o) $@:,g' < $@.tmp >$@; \
	rm $@.tmp

.make/depends/%.cpp.d: src/%.cpp | $(DEPENDS_SUBDIRS)
	@set -e; \
	$(CXX) $(CXXFLAGS) -M $< > $@.tmp; \
	sed -E 's,(.*).o\s*:,$(subst src,obj,$<.o) $@:,g' < $@.tmp >$@; \
	rm $@.tmp

$(OBJECTS_SUBDIRS):
	mkdir -p $@
	
$(DEPENDS_SUBDIRS) $(OPTIONS_SUBDIRS):
	@mkdir -p $@

print_vars:
	@echo SUFFIXES = $(SUFFIXES)
	@echo DEPENDS_SUFFIXES = $(DEPENDS_SUFFIXES)
	@echo SOURCES_SUBDIRS = $(SOURCES_SUBDIRS)
	@echo ALL_SOURCES  = $(ALL_SOURCES )
	@echo C_SOURCES = $(C_SOURCES)
	@echo CXX_SOURCES = $(CXX_SOURCES)
	@echo LD_SOURCES = $(LD_SOURCES)
	@echo AS_SOURCES = $(AS_SOURCES)
	@echo HEADERS_SUBDIRS = $(HEADERS_SUBDIRS)
	@echo HEADERS = $(HEADERS)
	@echo OBJECTS_SUBDIRS = $(OBJECTS_SUBDIRS)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDS_SUBDIRS = $(DEPENDS_SUBDIRS)
	@echo DEPENDS_SOURCES = $(DEPENDS_SOURCES)
	@echo DEPENDS = $(DEPENDS)
	@echo OPTIONS_SUBDIRS = $(OPTIONS_SUBDIRS)
	@echo OPTIONS = $(OPTIONS)
	@echo OPTS = $(OPTS)
	@echo OPTS_ON = $(OPTS_ON)
	@echo OPTS_OFF = $(OPTS_OFF)
	@echo ALLOPTSVARS = $(ALLOPTSVARS)
	@echo OPTSVARS = $(OPTSVARS)
	@echo SHOULD_DEFINE = $(SHOULD_DEFINE)
	@echo UNDEF_OPTS = $(UNDEF_OPTS)
	@echo C_COMPILE = $(C_COMPILE)
	@echo CXX_COMPILE = $(CXX_COMPILE)
	@echo LD_COMPILE = $(LD_COMPILE)
	@echo AS_COMPILE = $(AS_COMPILE)
	@echo CHANGED_SOURCES = $(CHANGED_SOURCES)
	@echo CHANGED_NAME = $(CHANGED_NAME)
	@echo CHANGED_OBJECTS = $(CHANGED_OBJECTS)

clean:
	rm -rf $(NAME) obj gmon.out analysis
	@rm -rf .make/depends .make/buildopts

tar: clean
	tar -zcvf /tmp/$(NAME).tar.gz .
	cp /tmp/$(NAME).tar.gz .

FORCE:

$(CHANGED_NAME) $(CHANGED_OBJECTS): FORCE

.PHONY: all clean tar FORCE

-include $(DEPENDS)
