##############################################################################
# include makefile for nmake under Windows NT
#
##############################################################################

#path and name of MPI library
#MPI_LIB_NAME=Cvwmpi.lib
MPI_LIB_NAME=mpich.lib
MPI = $(MPI_LIB)\$(MPI_LIB_NAME)

GLOB_DEFINES =-DWIN32
DEFINES = $(GLOB_DEFINES) $(LOC_DEFINES)
GLOB_INCLUDES=
INCLUDES = $(GLOB_INCLUDES) $(LOC_INCLUDES)

AR = link.exe -lib -nologo
ARFLAGS = /out:$(LIBRARY_PATH)

CC = cl -nologo
#COPT =   -Zi
COPT =  -G5 -O2
CFLAGS = $(COPT) $(DEFINES) $(INCLUDES) -Fo"$(OBJDIR)/" -c

FC = fl32 -nologo
#FOPT = -Zi
FOPT = -G5 -Ox
FFLAGS = $(FOPT) -Fo"$(OBJDIR)/" -c

CPP   = $(CC) -EP
CPPFLAGS = $(INCLUDES) $(DEFINES)

.SUFFIXES:
.SUFFIXES:      .obj .s .f .F .c .pg .exe

.c{$(OBJDIR)}.obj:
	$(CC) $(CFLAGS) $<


.F{$(OBJDIR)}.obj: 
        $(CPP) $(CPPFLAGS) $< > $*.for
	$(FC) $(FFLAGS) $*.for 
	del $*.for


.F.for:
        $(CPP) $(CPPFLAGS) $< > $*.for

.exe.pg:
        @echo Creating WMPI file (4 MPI tasks): $*.pg
        @echo local 3 > $*.pg

.c.exe:
	@$(MAKE) -nologo $(OBJDIR)
!IFDEF TESTUTIL
	@$(MAKE) -nologo $(TESTUTIL)
!ENDIF
	@$(MAKE) -nologo $(OBJDIR)\$*.obj
	$(CC) /Fe$@ $(OBJDIR)\$*.obj $(TESTUTIL) $(LIBS) $(CLINK)
	@if not exist $*.pg $(MAKE) -nologo $*.pg

.F.exe:
	@$(MAKE) -nologo $(OBJDIR)
!IFDEF TESTUTIL
	@$(MAKE) -nologo $(TESTUTIL)
!ENDIF
	@$(MAKE) -nologo $(OBJDIR)\$*.obj
	$(FC) /Fe$@ $(OBJDIR)\$*.obj $(TESTUTIL) $(LIBS)
	@if not exist $*.pg $(MAKE) -nologo $*.pg
