/*$Id: adjust0.c,v 1.2 1995-02-02 23:14:35 d3g681 Exp $*/
/***********************************************************************\
* Purpose: adjusts timers in tracefiles generated by ga_trace,          *
*          sorts events, combines tracefiles, and reformats the data    *
* Jarek Nieplocha, 10.15.1993                                           *
\***********************************************************************/

#include <stdio.h>
#define MAX_EVENTS 202500

#define abs(x) ((x)>0 ? (x) :(-x))

main(argc,argv)
int argc;
char **argv;
{
long int p,i,k,MR,events;
unsigned long int  *clock_base, *times, base=0, *tbase;
int *record;
int ga=3;

static int tcomp();
int flag;
          
FILE *fin,*fout;
char *foutname = "adjust.ed", finname[8];

   if(argc<2){
      printf("Usage: adjust <number of processors> [<max number of events>]\n");
      exit(1);
   } 
   sscanf(argv[1],"%d",&p);
   if(argc>2) sscanf(argv[2],"%ld",&MR);
   if(argc>3) sscanf(argv[3],"%d",&ga);
   else MR =  MAX_EVENTS;
   
   printf("Processing tracefiles for %d processors\n",p);
   
   if(!(clock_base = (unsigned long int *)malloc(p*sizeof(unsigned long int)))){
                     printf("couldn't allocate memory 1\n");
                     exit(2);
   }
   if(!(record = (int*)malloc(7*MR*sizeof(int)))){
                     printf("couldn't allocate memory 2\n");
                     exit(2);
   }
   if(!(times = (unsigned long int *)malloc(4*MR*sizeof(unsigned long int)))){
                     printf("couldn't allocate memory 3\n");
                     exit(2);
   }
   
   base = 0; tbase = times; events=0;
   for(i=0;i<p;i++){
      printf("file %d\n",i);
      sprintf(finname,"%03d",i);
      fin = fopen(finname,"r");
      if(!fin){
          printf("%s: File Not Found, Exiting ...\n",finname);
          exit(3);
      }

      for(k=0;k<7;k++) fscanf(fin,"%d",&flag);
      fscanf(fin,"%lu", clock_base+i);
      fscanf(fin,"%lu", clock_base+i);

      for(;;){
          for(k=base;k<7+base;k++)fscanf(fin,"%d",(record+k)); 
          fscanf(fin,"%lu",times);
          if(feof(fin))break;
          times[0] -= clock_base[i]; 
          times[1] = base+1;  
          fscanf(fin,"%lu",times+2);
          if(record[base+1]!=ga)continue;
          times+=2; 
          times[0] -= clock_base[i];
          times[1] = -base-1; 
          times+=2; 
          base += 7;
          events++;
      }
      fclose(fin);
   }
   
   /* sorting events */
   
   times = tbase; 
   printf("sorting %d\n",events);
   qsort(tbase, 2*events, 2*sizeof(unsigned long int), tcomp);
   printf("sorted %d\n",events);
   
   /* output */

      
   fout = fopen(foutname,"w");
   for(i=0;i<events*2;i++){
      base = abs((long int)times[1]); base -=1;
      flag = ((long int)times[1]) < 0 ? -1 : 1; 
      for(k=base;k<7+base;k++)fprintf(fout,"%d ",*(record+k)); 
      fprintf(fout,"%d ",flag);
      fprintf(fout,"%lu\n",times[0]);
      times += 2;
   }     
   
   fclose(fout);
}
   
      
static int tcomp(t1, t2)
unsigned long int *t1, *t2;
{
int flag;
    flag = (*t1 == *t2) ? 0 :(*t1 > *t2 ? 1 : -1);
    return (flag);
}
