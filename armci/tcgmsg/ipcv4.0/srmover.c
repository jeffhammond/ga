#if HAVE_CONFIG_H
#   include "config.h"
#endif

/* $Header: /tmp/hpctools/ga/tcgmsg/ipcv4.0/srmover.c,v 1.4 1995-02-24 02:17:53 d3h325 Exp $ */

#include <string.h>

void SRmover(char * a, char * b, long n)
{
  (void) memcpy(a, b, n);
}
