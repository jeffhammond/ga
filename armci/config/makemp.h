#
# settings dependent on selection of the message-passing library
# MP_LIBS     - library path 
# MP_INCLUDES - location of header files
# MP_DEFINES  - cpp symbols

#reference to the MPI library name, overwritten by LIBMPI environment variable
# you should also add to it names of any libs on which mpi depends
# e.g., on Compaq with Quadrics network LIBMPI should also add -lelan3
# LIBMPI = -lmpi -lelan3
# 
SKIP_MPILIB = LAPI HITACHI
MPI_LIB_NAME = -lmpi
ifeq ($(TARGET),$(findstring $(TARGET),$(SKIP_MPILIB)))
MPI_LIB_NAME = 
endif
ifeq ($(ARMCI_NETWORK),QUADRICS)
   ifeq ($(TARGET),DECOSF)
     MPI_LOC = /usr/opt/mpi
   else
     MPI_LOC = /usr/lib/mpi
   endif
   MPI_LIB_NAME = -lmpi -lelan
endif
#
#reference to the PVM library name, overwritten by LIBPVM environment variable
ifeq ($(TARGET),CRAY-T3E)
     PVM_LIB_NAME = -lpvm3
else
     PVM_LIB_NAME = -lgpvm3 -lpvm3
endif

ifeq ($(MSG_COMMS),PVM)
  ifdef PVM_INCLUDE
    MP_TMP_INCLUDES = $(PVM_INCLUDE)
  endif
  ifdef PVM_LIB
    MP_LIBS += -L$(PVM_LIB)
  endif
  ifdef LIBPVM
    PVM_LIB_NAME = $(LIBPVM)
  endif
  MP_LIBS += $(PVM_LIB_NAME)
  MP_DEFINES += -DPVM
endif
#
#
ifeq ($(MSG_COMMS),MPI)
  ifdef MPI_INCLUDE
    MP_TMP_INCLUDES = $(MPI_INCLUDE) 
  endif
  ifdef MPI_LIB
    MP_LIBS += -L$(MPI_LIB)
  endif
  ifdef LIBMPI
      MPI_LIB_NAME = $(LIBMPI)
  endif
  MP_LIBS += $(MPI_LIB_NAME)
  MP_DEFINES += -DMPI
endif
#
#
ifeq ($(MSG_COMMS),TCGMSG)
  ifdef TCG_INCLUDE
     MP_TMP_INCLUDES = $(TCG_INCLUDE)
  endif
  ifdef TCG_LIB
     MP_LIBS += -L$(TCG_LIB)
  endif
  MP_LIBS += -ltcgmsg
  MP_DEFINES += -DTCGMSG
endif
#
#
ifdef MP_TMP_INCLUDES
  Comma:= ,
  Empty:=
  Space:= $(Empty) $(Empty)
  MP_INCLUDES += -I$(subst $(Comma), $(Space)-I,$(MP_TMP_INCLUDES))
endif
