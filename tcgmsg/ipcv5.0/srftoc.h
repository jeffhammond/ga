/* $Header: /tmp/hpctools/ga/tcgmsg/ipcv5.0/srftoc.h,v 1.4 2001-05-08 17:30:39 edo Exp $ */

#ifndef SRFTOC_H_
#define SRFTOC_H_
/* 
  This header file provides definitions for c for the names of the
  c message passing routines accessible from FORTRAN. It need not
  be included directly in user c code, assuming that sndrcv.h has already.

  It is needed as the FORTRAN naming convention varies between machines
  and it is the FORTRAN interface that is portable, not the c interface.
  However by coding with the macro defnition names c portability is
  ensured.

  On most system V machines (at least Cray and Ardent) FORTRAN uppercases
  the names of all FORTRAN externals. On most BSD machines (at least
  Sun, Alliant, Encore, Balance) FORTRAN appends an underbar (_).
  Here the uppercase c routine name with an underbar is used as a
  macro name appropriately defined for each machine. Also the BSD naming
  is assumed by default.
  Note that pbegin and pfilecopy are only called from c.
*/

#if (defined(CRAY) || defined(ARDENT))

#define NICEFTN_     NICEFTN
#define NODEID_      NODEID
#define PROBE_       PROBE
#define NNODES_      NNODES
#define MTIME_       MTIME
#define TCGTIME_     TCGTIME
#define SND_         SND
#define RCV_         RCV
#define BRDCST_      BRDCST
#define SYNCH_       SYNCH
#define PBEGINF_     PBEGINF
#define PBGINF_      PBGINF
#define PEND_        PEND
#define SETDBG_      SETDBG
#define NXTVAL_      NXTVAL
#define PBFTOC_      PBFTOC
#define PARERR_      PARERR
#define LLOG_        LLOG
#define STATS_       STATS
#define WAITCOM_     WAITCOM
#define MITOD_       MITOD
#define MDTOI_       MDTOI
#define MDTOB_       MDTOB
#define MITOB_       MITOB
#define DRAND48_     DRAND48
#define SRAND48_     SRAND48
#define PFCOPY_      PFCOPY
#define DGOP_        DGOP
#define IGOP_        IGOP

#else

#define NICEFTN_     niceftn_ 
#define wrap_nodeid  nodeid_ 
#define wrap_probe   probe_ 
#define wrap_nnodes  nnodes_
#define wrap_mtime   mtime_ 
#define TCGTIME_     tcgtime_ 
#define wrap_snd     snd_ 
#define wrap_rcv     rcv_  
#define wrap_brdcst  brdcst_ 
#define wrap_synch   synch_ 
#define PBEGINF_     pbeginf_  
#define PBGINF_      pbginf_ 
#define PEND_        pend_ 
#define wrap_setdbg  setdbg_ 
#define wrap_nxtval  nxtval_
#define PBFTOC_      pbftoc_
#define wrap_parerr  parerr_ 
#define LLOG_        llog_ 
#define STATS_       stats_ 
#define wrap_igop    igop_
#define wrap_waitcom waitcom_
#define wrap_mitod   mitod_ 
#define wrap_mdtoi   mdtoi_ 
#define wrap_mdtob   mdtob_ 
#define wrap_mitob   mitob_ 
#define DRAND48_     drand48_ 
#define SRAND48_     srand48_ 
#define wrap_pfcopy  pfcopy_ 
#define TCGREADY_    tcgready_ 
#endif
#endif

