/*
 * module: global.common.c
 * author: Jarek Nieplocha
 * last modification: Thu Sep 29 09:41:55 PDT 1994
 *
 * DISCLAIMER
 * 
 * This material was prepared as an account of work sponsored by an
 * agency of the United States Government.  Neither the United States
 * Government nor the United States Department of Energy, nor Battelle,
 * nor any of their employees, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY,
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT,
 * SOFTWARE, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT
 * INFRINGE PRIVATELY OWNED RIGHTS.
 * 
 * 
 * ACKNOWLEDGMENT
 * 
 * This software and its documentation were produced with United States
 * Government support under Contract Number DE-AC06-76RLO-1830 awarded by
 * the United States Department of Energy.  The United States Government
 * retains a paid-up non-exclusive, irrevocable worldwide license to
 * reproduce, prepare derivative works, perform publicly and display
 * publicly by or for the US Government, including the right to
 * distribute to other US Government contractors.
 */

#include "global.c.h"
#include "macommon.h"
#include <stdio.h>


#ifdef CRAY_T3D
#include <fortran.h>
#endif

void f2cstring();
void c2fstring();
#if !(defined(SGI)||defined(AIX))
#ifndef CRAY_T3D
int  fprintf();
#endif
#endif
#if defined(SUN)
void fflush();
#endif


extern DoublePrecision *DBL_MB;
extern Integer         *INT_MB;


#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) >= 0) ? (a) : (-(a)))




logical patch_intersect(ilo, ihi, jlo, jhi, ilop, ihip, jlop, jhip)
     Integer *ilo, *ihi, *jlo, *jhi;
     Integer *ilop, *ihip, *jlop, *jhip;
{
     /* check consistency of patch coordinates */ 
     if( *ihi < *ilo || *jhi < *jlo)     return FALSE; /* inconsistent */ 
     if( *ihip < *ilop || *jhip < *jlop) return FALSE; /* inconsistent */ 
     
     /* find the intersection and update (ilop: ihip, jlop: jhip) */ 
     if( *ihi < *ilop || *ihip < *ilo) return FALSE; /* don't intersect */ 
     if( *jhi < *jlop || *jhip < *jlo) return FALSE; /* don't intersect */ 
     *ilop = MAX(*ilo,*ilop);
     *ihip = MIN(*ihi,*ihip);
     *jlop = MAX(*jlo,*jlop);
     *jhip = MIN(*jhi,*jhip);

     return TRUE;
}



/*\ COPY ONE GLOBAL ARRAY INTO ANOTHER
\*/
void ga_copy_(g_a, g_b)
     Integer *g_a, *g_b;
{
Integer atype, btype, adim1, adim2, bdim1, bdim2;
Integer ilo, ihi, jlo, jhi;
Integer me= ga_nodeid_(), index, ld;

   ga_sync_();

#ifdef GA_TRACE
       trace_stime_();
#endif

   ga_check_handle(g_a, "ga_copy");
   ga_check_handle(g_b, "ga_copy");

   if(*g_a == *g_b) ga_error("ga_copy: arrays have to different ", 0L);

   ga_inquire_(g_a, &atype, &adim1, &adim2);
   ga_inquire_(g_b, &btype, &bdim1, &bdim2);

   if(atype != btype || (atype != MT_F_DBL && atype != MT_F_INT))
               ga_error("ga_copy: wrong types ", 0L);

   if(adim1 != bdim1 || adim2!=bdim2 )
               ga_error("ga_copy: arrays not conformant", 0L);

   ga_distribution_(g_a, &me, &ilo, &ihi, &jlo, &jhi);

   if (  ihi>0 && jhi>0 ){
      ga_access_(g_a, &ilo, &ihi, &jlo, &jhi,  &index, &ld);
      if(atype == MT_F_DBL)
           ga_put_(g_b, &ilo, &ihi, &jlo, &jhi, DBL_MB+index-1, &ld);
      else
           ga_put_(g_b, &ilo, &ihi, &jlo, &jhi, INT_MB+index-1, &ld);
      ga_release_(g_a, &ilo, &ihi, &jlo, &jhi);
   }

#ifdef GA_TRACE
   trace_etime_();
   op_code = GA_OP_COP;
   trace_genrec_(g_a, ilo, ihi, jlo, jhi, &op_code);
#endif

   ga_sync_();
}



/*\ GLOBAL OPERATIONS 
 *  (C)
 *  We cannot use TCGMSG in data-server model 
 *  where only half of processes participate
\*/
void ga_dgop(type, x, n, op)
     Integer type, n;
     DoublePrecision *x;
     char *op;
{
#ifdef DATA_SERVER
#define BUF_SIZE 10000
     Integer factor = 1;  /*  log of level of binary tree */
     Integer rem, me, nproc, lenmes, sync=1,to,lenbuf ;
     DoublePrecision work[BUF_SIZE];
     static void ddoop();
     long ndo;

     me = ga_nodeid_(); nproc = ga_nnodes_(); 

     while ((ndo = (n<=BUF_SIZE) ? n : BUF_SIZE)) {

       /* up-tree phase */
       
       do {
	 factor *= 2; rem = me%factor;
	 lenbuf = lenmes = ndo*sizeof(DoublePrecision);
	 if(rem){
	   to = me - rem;
	   snd_(&type, x, &lenmes, &to, &sync); 
	   rcv_(&type, x, &lenmes, &lenbuf, &to, &to, &sync);
	   break;
	 }else{
	   to = me + factor/2;
	   if(to < nproc){
	     rcv_(&type, work, &lenmes, &lenbuf, &to, &to, &sync);
	     ddoop(ndo, op, x, work); 
	   }
	 }
       }while (factor < nproc);
       
       /* Now, root broadcasts the result down the binary tree */
       
       for(factor = 1; factor <= 2*nproc; factor*=2);
       do {
	 factor /= 2;         
	 rem = me % factor;
	 lenbuf = lenmes = ndo*sizeof(DoublePrecision);
	 if(!rem && me != factor){
	   to = me + factor;
	   if(to < nproc) snd_(&type, x, &lenmes, &to, &sync); 
	 }
       } while(factor > 1);
       n -=ndo;
       x +=ndo;
     }

#else

     /* use TCGMSG as a wrapper to a more efficient native implementation
      * of  global ops 
      */
     void dgop_(); 
#ifdef SP1
     ga_sync_();
#endif
     dgop_(&type, x, &n, op, (Integer)strlen(op)); 
#ifdef SP1
     ga_sync_();
#endif
#endif
}



/*\ GLOBAL OPERATIONS 
 *  Fortran
\*/
#ifdef CRAY_T3D
void ga_dgop_(type, x, n, op)
     _fcd op;
#else
void ga_dgop_(type, x, n, op, len)
     char *op;
     long len;
#endif
     Integer *type, *n;
     DoublePrecision *x;
{
long gtype,gn,glen;
     gtype = (long)*type; gn = (long)*n; 

#ifdef CRAY_T3D
     ga_dgop(gtype, x, gn, _fcdtocp(op));
#else
     ga_dgop(gtype, x, gn, op);
#endif
}


/*\ GLOBAL OPERATIONS 
 *  (C)
 *  We cannot use TCGMSG in data-server model 
 *  where only half of processes participate
\*/
void ga_igop(type, x, n, op)
     Integer type, n;
     Integer *x;
     char *op;
{
#ifdef DATA_SERVER
#define BUF_SIZE 10000
     Integer factor = 1;  /*  log of level of binary tree */
     Integer rem, me, nproc, lenmes, sync=1,to,lenbuf ;
     Integer work[BUF_SIZE];
     static void idoop();
     long ndo;

     me = ga_nodeid_(); nproc = ga_nnodes_(); 

     while ((ndo = (n<=BUF_SIZE) ? n : BUF_SIZE)) {

       /* up-tree phase */
       
       do {
	 factor *= 2; rem = me%factor;
	 lenbuf = lenmes = ndo*sizeof(Integer);
	 if(rem){
	   to = me - rem;
	   snd_(&type, x, &lenmes, &to, &sync); 
	   rcv_(&type, x, &lenmes, &lenbuf, &to, &to, &sync);
	   break;
	 }else{
	   to = me + factor/2;
	   if(to < nproc){
	     rcv_(&type, work, &lenmes, &lenbuf, &to, &to, &sync);
	     idoop(ndo, op, x, work); 
	   }
	 }
       }while (factor < nproc);
       
       /* Now, root broadcasts the result down the binary tree */
       
       for(factor = 1; factor <= 2*nproc; factor*=2);
       do {
	 factor /= 2;         
	 rem = me % factor;
	 lenbuf = lenmes = ndo*sizeof(Integer);
	 if(!rem && me != factor){
	   to = me + factor;
	   if(to < nproc) snd_(&type, x, &lenmes, &to, &sync); 
	 }
       } while(factor > 1);
       n -=ndo;
       x +=ndo;
     }

#else

     /* use TCGMSG as a wrapper to a more efficient native implementation
      * of  global ops 
      */
     void igop_(); 
#ifdef SP1
     ga_sync_();
#endif
     igop_(&type, x, &n, op, (Integer)strlen(op)); 
#ifdef SP1
     ga_sync_();
#endif
#endif
}



/*\ GLOBAL OPERATIONS 
 *  Fortran
\*/
#ifdef CRAY_T3D
void ga_igop_(type, x, n, op)
     _fcd op;
#else
void ga_igop_(type, x, n, op, len)
     char *op;
     long len;
#endif
     Integer *type, *n;
     Integer *x;
{
long gtype,gn,glen;
     gtype = (long)*type; gn = (long)*n;

#ifdef CRAY_T3D
     ga_igop(gtype, x, gn, _fcdtocp(op));
#else
     ga_igop(gtype, x, gn, op);
#endif
}



/*\ BROADCAST 
\*/
void ga_brdcst_(type, buf, len, originator)
     Integer *type, *len, *originator;
     char *buf;
{
#ifdef DATA_SERVER
     Integer factor=1, rem, me, nproc, lenmes, sync=1,to,root=0 ;

     me = ga_nodeid_(); nproc = ga_nnodes_();

     /* send the data to root */

     if (*originator != root ){
       if(me == *originator) snd_(type, buf, len, &root, &sync); 
       if(me == root) rcv_(type, buf, len, &lenmes, originator, &sync); 
     }

     /* post receives */
     do {
       factor *= 2; rem = me%factor;
       if(rem){
              to = me - rem;
              /*fprintf(stderr,"%d rcv %d to %d\n",me,*type,to);*/
              rcv_(type, buf, len, &lenmes,  &to, &to, &sync);
              break;
       }
     }while ( factor < nproc );

     /* Now, root broadcasts the data down the binary tree */

     for(factor = 1; factor <= 2*nproc; factor*=2);
     do {
       factor /= 2;        
       rem = me % factor;
       if(!rem && me != factor){
              to = me + factor;
              if( to < nproc ) {
                 /*fprintf(stderr,"%d snd down %d to %d\n",me,*type,to);*/
                 snd_(type, buf, len, &to, &sync);
              }
       }
     } while( factor > 1 );
     /*fprintf(stderr,"%d broadcast done \n",me);*/
#else
     /* use TCGMSG as a wrapper to a more efficient native implementation
      * of  broadcast 
      */
     void brdcst_();
     long gtype,gfrom,glen;
     gtype =(long) *type; gfrom =(long) *originator; glen =(long) *len;
#ifdef SP1
     ga_sync_();
#endif
     brdcst_(&gtype,buf,&glen,&gfrom);
#ifdef SP1
     ga_sync_();
#endif
#endif
}



/*\ GROUP SYNCHRONIZATION routine 
 *  synchronizes a group of processors
\*/
void group_sync_(type, nproc, mynode, nextnode)
Integer *type, *nproc;
Integer (*mynode)(), (*nextnode)();
/*
 * assumes linear array topology in the group; 
 * the processor id is returned by mynode();
 * nextnode(n) returns the actual processor id of n-th processor in group; 
 * type - type of messages used in synchronization;
 * 
 * Used in DATA_SERVER implementation of ga_sync
 */ 
{
     Integer factor = 1;  /*  log of binary tree level */
     Integer rem, me, lenmes, sync=1, to, lenbuf ;
     char x[1];

     me = mynode(); 

     /* up-tree phase */

     do {
       factor *= 2; rem = me%factor;
       lenbuf = lenmes = sizeof(char);
       if(rem){
              to = me - rem; to = nextnode(&to);
              /*fprintf(stderr,"%d snd up %d to %d\n",me,*type,to);*/
              snd_(type, x, &lenmes, &to, &sync);
              /*fprintf(stderr,"%d rcv %d to %d\n",me,*type,to);*/
              rcv_(type, x, &lenmes, &lenbuf, &to, &to, &sync);
              break;
       }else{
              to = me + factor/2; to = nextnode(&to);
              if(to < *nproc){
                 /*fprintf(stderr,"%d rcv %d to %d\n",me,*type,to);*/
                 rcv_(type, x, &lenmes, &lenbuf, &to, &to, &sync);
              }
       }
     }while (factor < *nproc);

     /* Now, root broadcasts the result down the binary tree */

     for(factor = 1; factor <= 2* *nproc; factor*=2);
     do {
       factor /= 2;
       rem = me % factor;
       lenbuf = lenmes = sizeof(char);
       if(!rem && me != factor){
              to = me + factor; to = nextnode(&to);
              if(to <  *nproc){
                   /*fprintf(stderr,"%d snd down %d to %d\n",me,*type,to);*/
                   snd_(type, x, &lenmes, &to, &sync);
              }
       }
     } while(factor > 1);
}


/*\ PRINT g_a[ilo:ihi, jlo:jhi]
\*/
void ga_print_patch_(g_a, ilo, ihi, jlo, jhi, pretty)
        Integer *g_a, *ilo, *ihi, *jlo, *jhi, *pretty;
/*
  Pretty = 0 ... spew output out with no formatting
  Pretty = 1 ... format output so that it is readable
*/  
{
#define DEV stdout
#define BUFSIZE 6
#define FLEN 80 
Integer i, j,jj, dim1, dim2, type, ibuf[BUFSIZE], jmax, ld=1 ;
DoublePrecision  dbuf[BUFSIZE];
char name[80];

  ga_sync_();
  ga_check_handle(g_a, "ga_print");
  if(ga_nodeid_() == 0){

     ga_inquire_(g_a,  &type, &dim1, &dim2);
     name[FLEN-1]='\0';
     ga_inquire_name(g_a,  name);
     if (*ilo <= 0 || *ihi > dim1 || *jlo <= 0 || *jhi > dim2){
                      fprintf(stderr,"%d %d %d %d dims: [%d,%d]\n", 
                             *ilo,*ihi, *jlo,*jhi, dim1, dim2);
                      ga_error(" ga_print: indices out of range ", *g_a);
     }

     fprintf(DEV,"\n global array: %s[%d:%d,%d:%d],  handle: %d \n",
             name, *ilo, *ihi, *jlo, *jhi, (int)*g_a);

     if (!pretty) {
       for (i=*ilo; i <*ihi+1; i++){
         for (j=*jlo; j <*jhi+1; j+=BUFSIZE){
	   jmax = MIN(j+BUFSIZE-1,*jhi);
	   if(type == MT_F_INT){
	     ga_get_(g_a, &i, &i, &j, &jmax, ibuf, &ld);
	     for(jj=0; jj<(jmax-j+1); jj++)
	       fprintf(DEV," %8d",ibuf[jj]);
	     
	   }else if(type == MT_F_DBL){
	     ga_get_(g_a, &i, &i, &j, &jmax, dbuf, &ld);
	     for(jj=0; jj<(jmax-j+1); jj++)
	       fprintf(DEV," %12.6f",dbuf[jj]);
	     
	   }else ga_error("ga_print: wrong type",0);
	   
	 }
	 fprintf(DEV,"\n");
       }
       fflush(DEV);
     }
     else {
       for (j=*jlo; j<*jhi+1; j+=BUFSIZE){
	 jmax = MIN(j+BUFSIZE-1,*jhi);
	 fprintf(DEV, "\n"); fprintf(DEV, "\n");

	 /* Print out column headers */

	 fprintf(DEV, "      ");
	 if (type == MT_F_INT) {
	   for (jj=j; jj<=jmax; jj++)
	     fprintf(DEV, "%6d  ", jj);
	   fprintf(DEV,"\n      ");
	   for (jj=j; jj<=jmax; jj++)
	     fprintf(DEV," -------");
	 }
	 else {
	   for (jj=j; jj<=jmax; jj++)
	     fprintf(DEV,"%8d    ", jj);
	   fprintf(DEV,"\n      ");
	   for (jj=j; jj<=jmax; jj++)
	     fprintf(DEV," -----------");
	 }
	 fprintf(DEV,"\n");
	   
	 for (i=*ilo; i <*ihi+1; i++){
	   fprintf(DEV,"%4i  ",i);
	   if(type == MT_F_INT){
	     ga_get_(g_a, &i, &i, &j, &jmax, ibuf, &ld);
	     for(jj=0; jj<(jmax-j+1); jj++)
	       fprintf(DEV,"%8d",ibuf[jj]);
	     
	   }else if(type == MT_F_DBL){
	     ga_get_(g_a, &i, &i, &j, &jmax, dbuf, &ld);
	     for(jj=0; jj<(jmax-j+1); jj++)
	       fprintf(DEV,"%12.6f",dbuf[jj]);
	     
	   }else ga_error("ga_print: wrong type",0);
	   fprintf(DEV,"\n");
	 }
       }
       fflush(DEV);
     }
   }
       
  ga_sync_();
}


void ga_print_(g_a)
     Integer *g_a;
{
  Integer type, dim1, dim2;
  Integer ilo=1, jlo=1;
  Integer pretty = 1;

  ga_inquire_(g_a, &type, &dim1, &dim2);

  ga_print_patch_(g_a, &ilo, &dim1, &jlo, &dim2, &pretty);
}
  


/*\  ERROR TERMINATION
 *   C-version
\*/
void ga_error(string, icode)
     char     *string;
     Integer  icode;
{
Integer i;
#ifdef TCGMSG
extern void Error();
#ifdef DATA_SERVER
extern int SR_caught_sigint;
#endif
#endif

#ifdef SHMEM
    void clean_all();
    clean_all(); 
#endif /* SHMEM */

#ifdef TCGMSG
#ifdef DATA_SERVER
     if (SR_caught_sigint)fprintf(stderr,"%s %ld",string,icode);
#endif
     Error(string,  icode);
#else
     fprintf(stderr,"%s %ld",string,icode);
     fflush(stderr);
#if defined(SUN) || defined(SGI)
     abort();
#else
     exit(1);
#endif
#endif /*TCGMSG */
}




/*\  ERROR TERMINATION
 *   Fortran version
\*/
#ifdef CRAY_T3D
void ga_error_(string, icode)
     _fcd        string;
#else
void ga_error_(string, icode, slen)
     char        *string;
     Integer     slen;
#endif
     Integer     *icode;
{
#define FMSG 256
char buf[FMSG];
#ifdef CRAY_T3D
      f2cstring(_fcdtocp(string), _fcdlen(string), buf, FMSG);
#else
      f2cstring(string,slen, buf, FMSG);
#endif
      ga_error(buf,*icode);
}




#ifndef SHMEM
/*\ CHECK GA HANDLE AND IF IT'S WRONG TERMINATE
 *  C version
\*/
void ga_check_handle(g_a, string)
     Integer *g_a;
     char *string;
{
  ga_check_handle_(g_a,string, (Integer)strlen(string));
}

/*\ GET ARRAY NAME
\*/
void  ga_inquire_name(g_a,  name)
      Integer *g_a;
      char *name;
{
#define FLEN 80
Integer len=0,slen=FLEN-1;
void ga_inquire_name_();

if(name[slen]!='\0')ga_error("ga_inquire_name: C string shorter than 80",0L);

   for(len=0;len<slen;len++)name[len]='\0';
   ga_inquire_name_(g_a, name, slen-2);
   for(len=0;len<slen;len++)if(name[len]==' '){name[len]='\0';break;}
   name[slen]='\0';
}
#endif



/************** Fortran - C conversion routines for strings ************/

/*\ converts C strings to  Fortran strings
\*/
void c2fstring( cstring, fstring, flen)
     char *cstring, *fstring;
     Integer flen;
{
char *strncpy();
int clen = strlen(cstring);
    strncpy(fstring, cstring, flen);
    /* remove \n character if any */
    if(flen>clen)fstring[clen]=' ';
}


/*\
 * Strip trailing blanks from fstring and copy it to cstring,
 * truncating if necessary to fit in cstring, and ensuring that
 * cstring is NUL-terminated.
\*/
void f2cstring(fstring, flength, cstring, clength)
    char        *fstring;       /* FORTRAN string */
    Integer      flength;        /* length of fstring */
    char        *cstring;       /* C buffer */
    Integer      clength;        /* max length (including NUL) of cstring */
{
    /* remove trailing blanks from fstring */
    while (flength-- && fstring[flength] == ' ') ;

    /* the postdecrement above went one too far */
    flength++;

    /* truncate fstring to cstring size */
    if (flength >= clength)
        flength = clength - 1;

    /* ensure that cstring is NUL-terminated */
    cstring[flength] = '\0';

    /* copy fstring to cstring */
    while (flength--)
        cstring[flength] = fstring[flength];
}


/*************************** other auxiliary routines **********************/
 
static void ddoop(n, op, x, work)
     long n;
     char *op;
     double *x, *work;
{
  if (strncmp(op,"+",1) == 0)
    while(n--)
      *x++ += *work++;
  else if (strncmp(op,"*",1) == 0)
    while(n--)
      *x++ *= *work++;
  else if (strncmp(op,"max",3) == 0)
    while(n--) {
      *x = MAX(*x, *work);
      x++; work++;
    }
  else if (strncmp(op,"min",3) == 0)
    while(n--) {
      *x = MIN(*x, *work);
      x++; work++;
    }
  else if (strncmp(op,"absmax",6) == 0)
    while(n--) {
      register double x1 = ABS(*x), x2 = ABS(*work);
      *x = MAX(x1, x2);
      x++; work++;
    }
  else if (strncmp(op,"absmin",6) == 0)
    while(n--) {
      register double x1 = ABS(*x), x2 = ABS(*work);
      *x = MIN(x1, x2);
      x++; work++;
    }
  else
    ga_error("ddoop: unknown operation requested", (long) n);
}

static void idoop(n, op, x, work)
     long n;
     char *op;
     Integer *x, *work;
{
  if (strncmp(op,"+",1) == 0)
    while(n--)
      *x++ += *work++;
  else if (strncmp(op,"*",1) == 0)
    while(n--)
      *x++ *= *work++;
  else if (strncmp(op,"max",3) == 0)
    while(n--) {
      *x = MAX(*x, *work);
      x++; work++;
    }
  else if (strncmp(op,"min",3) == 0)
    while(n--) {
      *x = MIN(*x, *work);
      x++; work++;
    }
  else if (strncmp(op,"absmax",6) == 0)
    while(n--) {
      register double x1 = ABS(*x), x2 = ABS(*work);
      *x = MAX(x1, x2);
      x++; work++;
    }
  else if (strncmp(op,"absmin",6) == 0)
    while(n--) {
      register double x1 = ABS(*x), x2 = ABS(*work);
      *x = MIN(x1, x2);
      x++; work++;
    }
  else
    ga_error("idoop: unknown operation requested", (long) n);
}


