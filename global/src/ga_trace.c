 /***********************************************************************\
 * Tracing and Timing functions for the GA routines:                     *
 *   trace_init       - initialization                                   *
 *   trace_stime      - starts timing                                    *
 *   trace_etime      - ends timing                                      *
 *   trace_genrec     - generates a trace record for the calling routine *
 *   trace_end        - ends tracing & writes report to a file 'proc'    *
 * Note: the usc timer from the ALOG package is used                     *
 * Jarek Nieplocha, 10.14.93                                             *
 \***********************************************************************/

#include <macdecls.h>
#include <stdio.h>
#include <stdlib.h>
#include "ga.h"

#ifndef MPI
#  include "sndrcv.h"
#endif

static double tt0, tt1;
static unsigned long *tlog, thandle;
static unsigned long *indlog, ihandle;
static int *galog, gahandle;
static unsigned long current, MAX_EVENTS=0; 
static int ganum = 0;

#define MAX_GAS 100

#define min(a,b) ((a)<(b) ? (a) : (b))

double FATR ga_timer_()
{
#ifdef MPI
       return MPI_Wtime();
#else
       return TCGTIME_();
#endif
}

/* n is the max number of events to be traced */
void trace_init_(long *n)
{
    long index,err;
    
    if(*n<=0){
        printf("trace_init>>  invalid max number of events: %d\n",*n);
        return;
    }
    
    current = 0;
    err = 0;
    
    /*  MA_initialize(MT_INT,10000,10000); */ 
    
    MAX_EVENTS = *n;

    if(!MA_push_get(MT_LONGINT, *n*2, "timeLog", &thandle, &index)){
        printf("trace_init>> failed to allocate memory 1\n");
        err ++;
    }
    MA_get_pointer(thandle, &tlog);
    if(!tlog){
        printf("trace_init>> null pointer: 1\n");
        err ++;
    }

    if(!MA_push_get(MT_LONGINT, *n*6, "indexLog", &ihandle, &index)){
        printf("trace_init>> failed to allocate memory 2\n");
        err ++;
    }
    MA_get_pointer(ihandle, &indlog);
    if(!indlog) { 
        printf("trace_init>> null pointer: 2\n");
        err ++;
    }

    if(!MA_push_get(MT_INT, MAX_GAS, "gaLog", &gahandle, &index)){
        printf("trace_init>> failed to allocate memory 2\n");
        err ++;
    }
    MA_get_pointer(gahandle, &galog);
    if(!galog) { 
        printf("trace_init>> null pointer: 2\n");
        err ++;
    }
    
    ganum = 0;
    
    if(err) MAX_EVENTS = 0;
}

void  trace_stime_()
{
    tt0 =  ga_timer_();
}

void  trace_etime_()
{
    tt1 = ga_timer_();
}

void trace_genrec_(long *ga, long *ilo, long *ihi, long *jlo, long *jhi,
                   long *op)
{
    int i, d, has_record, counter;
    FILE *fout;
    char fname[15];
    typedef struct {
       int lo[2];
       int hi[2];
    } patch_t;
    patch_t *regions;
    int *proclist;
    int ndim, dims[2], tmp_dims[2], lo[2], hi[2], type, block[2];
    int me=GA_Nodeid(), nproc=GA_Nnodes(), proc;
    
    if(current >=  MAX_EVENTS) return;

    /* only node 0 does the bookkeeping */
    if(me == 0) {
        /* test if this ga has been recorded */
        has_record = 0;
        for(i=0; i<ganum; i++)
            if(*ga == (long) galog[i]) has_record = 1;

        if(!has_record) {
            galog[ganum++] = *ga;
            
            sprintf(fname, "distrib.%d", *ga);
            fout = fopen(fname,"w");
            
            NGA_Inquire(*ga, &type, &ndim, dims);
            
            /* get memory for arrays describing distribution */
            proclist = (int*)malloc(nproc*sizeof(int));
            if(!proclist)GA_Error("malloc failed for proclist",0);
            regions = (patch_t*)malloc(nproc*sizeof(patch_t));
            if(!regions)GA_Error("malloc failed for regions",0);

            for(d=0; d<ndim; d++) { lo[d] = 0; hi[d] = dims[d] -1;}

            proc = NGA_Locate_region(*ga, lo, hi, (int*)regions, proclist);
            if(proc<1) GA_Error("error in NGA_Locate_region",proc);
            
            for(d=0; d<ndim; d++) {
                tmp_dims[d] = 0; block[d] = 0;
                for(i=0; i<nproc; i++)
                    if(regions[i].hi[d] > tmp_dims[d]) {
                        block[d]++;
                        tmp_dims[d] = regions[i].hi[d];
                    }
            }

            /* print the number of processed */
            fprintf(fout, "%d\n", nproc);
            /* print dimensions */
            for(d=0; d<ndim; d++) fprintf(fout, "%d ", dims[d]);
            fprintf(fout, "\n");

            /* print number of lines to draw for each dimension */
            for(d=0; d<ndim; d++) 
                if(block[d] == 1) fprintf(fout, "%d ", block[d]);
                else fprintf(fout, "%d ", block[d]-1);
            fprintf(fout, "\n");

            /* print the offset for each line */
            for(d=0; d<ndim; d++) {
                if(block[d] == 1) fprintf(fout, "%d\n", dims[d]);
                else {
                    tmp_dims[d] = 0;
                    counter = 0;
                    for(i=0; i<nproc; i++)
                        if(regions[i].hi[d] > tmp_dims[d]) {
                            counter++;
                            if(counter == block[d]) break;
                            fprintf(fout, "%d\n",
                                    regions[i].hi[d]+1);
                            tmp_dims[d] = regions[i].hi[d];
                        }
                }
            }
            fclose(fout);
            free(regions); free(proclist);
        }
    }

    tlog[current*2]     = (unsigned long)(tt0 * 1000000);
    tlog[current*2+1]   = (unsigned long)(tt1 * 1000000);
    indlog[current*6]   = *ga;
    indlog[current*6+1] = *ilo;
    indlog[current*6+2] = *ihi;
    indlog[current*6+3] = *jlo;
    indlog[current*6+4] = *jhi;
    indlog[current*6+5] = *op;
    current++;
}

void trace_end_(long *proc)
{
    FILE *fout;
    char fname[10];
    int i,k;
    
    sprintf(fname,"%03d",*proc);
    fout=fopen(fname,"w");
    
    for(i=0;i<min(current,MAX_EVENTS);i++){
        fprintf(fout,"%d ",*proc);
        for(k=i*6;k<6*(i+1);k++)fprintf(fout,"%d ",indlog[k]);
        fprintf(fout,"%ld %ld\n",(unsigned long)tlog[i*2],
                (unsigned long)tlog[i*2+1]);
    }

    MA_pop_stack(gahandle);
    MA_pop_stack(ihandle);
    MA_pop_stack(thandle);
    
    fclose(fout);
}
