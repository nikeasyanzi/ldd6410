#
#	Makefile for the simpleEgi sample
#
#	Copyright (c) Embedthis Software, 2003-2009. All Rights Reserved.
#

NAME			:= simpleEgi

include 		.makedep

TARGETS			+= $(NAME)$(BLD_EXE)
LIBS			+= $(BLD_APPWEB_LIBS)
ifeq ($(BLD_FEATURE_STATIC),0)
	MODS		+= mod_egi$(BLD_SHOBJ)
endif

compileExtra: 	$(NAME)$(BLD_EXE)

$(NAME)$(BLD_EXE): $(OBJECTS)
	@bld --exe $(NAME)$(BLD_EXE) --search "$(BLD_LIB_DIR)/modules" --modules "$(MODS)" --libs "$(LIBS)" $(OBJECTS)

run:
	$(call setlibpath) ; ./$(NAME)$(BLD_EXE)

projectsExtra:
	@makeVsProj --project $(NAME).vcproj --exe $(NAME).exe --libs "mod_egi $(BLD_APPWEB_LIBS)" *.c
