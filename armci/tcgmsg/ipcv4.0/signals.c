#if HAVE_CONFIG_H
#   include "config.h"
#endif

/* $Header: /tmp/hpctools/ga/tcgmsg/ipcv4.0/signals.c,v 1.11 2004-04-01 02:04:57 manoj Exp $ */

#include <signal.h>
#include "sndrcvP.h"
#if defined(AIX) || defined(LINUX)
#include <sys/wait.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif

extern void Error();
int SR_caught_sigint = 0;

#define SigType  void

#ifndef SIG_ERR
#   define SIG_ERR         (SigType (*)())-1
#endif


SigType SigintHandler(int sig)
{
  SR_caught_sigint = 1;
  Error("SigintHandler: signal was caught",(long) sig);
}


void TrapSigint()
/*
  Trap the signal SIGINT so that we can propagate error
  conditions and also tidy up shared system resources in a
  manner not possible just by killing everyone
*/
{
  if ( signal(SIGINT, SigintHandler) == SIG_ERR)
       Error("TrapSigint: error from signal setting SIGINT",(long) SIGINT);
}


void ZapChildren()
/*
  kill -SIGINT all of my beloved children
*/
{
  while (SR_numchild--)
    (void) kill((int) SR_pids[SR_numchild], SIGINT);
}

void SigchldHandler(int sig)
{
  int status;
  (void) wait(&status);
  SR_caught_sigint = 1;
  Error("Child process terminated prematurely, status=",(long) status);
}

/*
  Trap SIGCHLD so that can tell if children die unexpectedly.
*/
void TrapSigchld()
{
  if ( signal(SIGCHLD, SigchldHandler) == SIG_ERR)
    Error("TrapSigchld: error from signal setting SIGCHLD", (long) SIGCHLD);
}

SigType SigsegvHandler(int sig)
{
  SR_caught_sigint = 1;
  Error("SigsegvHandler: signal was caught",(long) sig);
}


/*
  parallel needs to trap the signal SIGSEGV under Solaris 
  that is generated when interrupted in NxtVal  
*/
void TrapSigsegv()
{
  if ( signal(SIGSEGV, SigsegvHandler) == SIG_ERR)
       Error("TrapSigsegv: error from signal setting SIGSEGV", (long) SIGSEGV);
}

SigType SigtermHandler(int sig)
{
  SR_caught_sigint = 1;
  Error("SigtermHandler: signal was caught",(long) sig);
}

/*
  parallel needs to trap the SIGTERM for batch jobs
*/
void TrapSigterm()
{
  if ( signal(SIGTERM, SigtermHandler) == SIG_ERR)
       Error("TrapSigterm: error from signal setting SIGTERM", (long) SIGTERM);
}


