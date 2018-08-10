#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include "srftoc.h"
#include <unistd.h>

/* Wrapper around nice for FORTRAN users */
int NICEFTN_(int * ival)
{
  return nice(*ival);
}
