#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "macdecls.h"
#include "typesf2c.h"
#include "testutil.h"
#include "ga.h"

#define MAXDIM 10
#define BASE 100

#ifdef WIN32
#  include <windows.h>
#  define sleep(x) Sleep(1000*(x))
#endif

/***************************** macros ************************/
#define COPY(src, dst, bytes) memcpy((dst),(src),(bytes))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) <0) ? -(a) : (a))



/*\ generate random range for a section of multidimensional array 
\*/
void get_range( int ndim, int dims[], int lo[], int hi[]) 
{
	int dim;
	for(dim=0; dim <ndim;dim++){
		int toss1, toss2;
		toss1 = rand()%dims[dim];
		toss2 = rand()%dims[dim];
		if(toss1<toss2){
			lo[dim]=toss1;
			hi[dim]=toss2;
		}else {
  			hi[dim]=toss1;
			lo[dim]=toss2;
		}
    }
}




/*\ generates a new random range similar (same size, different indices)
 *  to the input range for an array with specified dimensions
\*/
void new_range(int ndim, int dims[], int lo[], int hi[],
                             int new_lo[], int new_hi[])
{
	int dim;
	for(dim=0; dim <ndim;dim++){
		int toss, range;
		int diff = hi[dim] -lo[dim]+1;
		assert(diff <= dims[dim]);
                range = dims[dim]-diff;
                toss = (range > 0)? rand()%range : lo[dim];
		new_lo[dim] = toss;
		new_hi[dim] = toss + diff -1;
		assert(new_hi[dim] < dims[dim]);
		assert(diff == (new_hi[dim] -new_lo[dim]+1));
	}
}





/*\ print range of n-dimensional array with two strings before and after
\*/
void print_range(char *pre,int ndim, int lo[], int hi[], char* post)
{
	int i;

	printf("%s[",pre);
	for(i=0;i<ndim;i++){
		printf("%d:%d",lo[i],hi[i]);
		if(i==ndim-1)printf("] %s",post);
		else printf(",");
	}
}

/*\ print subscript of ndim dimensional array with two strings before and after
\*/
void print_subscript(char *pre,int ndim, int subscript[], char* post)
{
	int i;

	printf("%s [",pre);
	for(i=0;i<ndim;i++){
		printf("%d",subscript[i]);
		if(i==ndim-1)printf("] %s",post);
		else printf(",");
	}
}


/*\ print a section of a 2-D array of doubles
\*/
void print_2D_double(double *a, int ld, int *lo, int *hi)
{
int i,j;
     for(i=lo[0];i<=hi[0];i++){
       for(j=lo[1];j<=hi[1];j++) printf("%13f ",a[ld*j+i]);
       printf("\n");
     }
}
          

/*\ initialize array: a[i,j,k,..]=i+100*j+10000*k+ ... 
\*/
void init_array(double *a, int ndim, int dims[])
{
	int idx[MAXDIM];
	int i,dim, elems;

        elems = 1;
        for(i=0;i<ndim;i++)elems *= dims[i];

 	for(i=0; i<elems; i++){
		int Index = i;
		double field, val;
        
		for(dim = 0; dim < ndim; dim++){
			idx[dim] = Index%dims[dim];
			Index /= dims[dim];
		}
		
                field=1.; val=0.;
		for(dim=0; dim< ndim;dim++){
			val += field*idx[dim];
			field *= BASE;
		}
		a[i] = val;
		/* printf("(%d,%d,%d)=%6.0f",idx[0],idx[1],idx[2],val); */
	}
}


/*\ compute Index from subscript
 *  assume that first subscript component changes first
\*/
int Index(int ndim, int subscript[], int dims[])
{
	int idx = 0, i, factor=1;
	for(i=0;i<ndim;i++){
		idx += subscript[i]*factor;
		factor *= dims[i];
	}
	return idx;
}


void update_subscript(int ndim, int subscript[], int lo[], int hi[], int dims[])
{
	int i;
	for(i=0;i<ndim;i++){
		if(subscript[i] < hi[i]) { subscript[i]++; return; }
		subscript[i] = lo[i];
	}
}


void print_distribution(int g_a)
{
int ndim, i, proc, type, nproc=GA_Nnodes();
int dims[MAXDIM], lo[MAXDIM], hi[MAXDIM];
char msg[100];

    NGA_Inquire(g_a, &type, &ndim, dims);
    printf("Array handle=%d name:'%s' ",g_a,GA_Inquire_name(g_a));
    printf("data type:");
    switch(type){
      case MT_F_DBL: printf("double"); break;
      case MT_F_INT: printf("integer"); break;
      case MT_F_DCPL: printf("double complex"); break;
      default: printf("ERROR");
    }
    printf(" dimensions:");
    for(i=0; i<ndim-1; i++)printf("%dx",dims[i]);
    printf("%d\n",dims[ndim-1]);
/*    printf("Array Distribution:\n");*/

    /* now everybody prints array range it owns */
    for(proc = 0; proc < nproc; proc++){
        NGA_Distribution(g_a,proc,lo,hi);
        sprintf(msg,"proc=%d\t owns array section: ",proc);
        print_range(msg,ndim,lo,hi,"\n");
    }
    fflush(stdout);
}

int compare_patches(int me, double eps, int ndim, double *array1, 
                     int lo1[], int hi1[], int dims1[],
		     double *array2, int lo2[], int hi2[], 
                     int dims2[])
{
	int i,j, elems=1;	
	int subscr1[MAXDIM], subscr2[MAXDIM];
        double diff,max;
        double *patch1, *patch2;

        /* compute pointer to first element in patch */ 
	patch1 = array1 +  Index(ndim, lo1, dims1);	
	patch2 = array2 +  Index(ndim, lo2, dims2);	

        /* count # of elements & verify consistency of both patches */
	for(i=0;i<ndim;i++){  
		Integer diff = hi1[i]-lo1[i];
		assert(diff == (hi2[i]-lo2[i]));
		assert(diff < dims1[i]);
		assert(diff < dims2[i]);
		elems *= diff+1;
		subscr1[i]= lo1[i];
		subscr2[i]=lo2[i];
	}

	/* compare element values in both patches */ 
	for(j=0; j< elems; j++){ 
		Integer idx1, idx2, offset1, offset2;
                /* calculate element Index from a subscript */
		idx1 = Index(ndim, subscr1, dims1);	
		idx2 = Index(ndim, subscr2, dims2);

		if(j==0){
			offset1 =idx1;
			offset2 =idx2;
		}
		idx1 -= offset1;
		idx2 -= offset2;

                diff = patch1[idx1] - patch2[idx2];
                max  = MAX(ABS(patch1[idx1]),ABS(patch2[idx2]));
                if(max == 0. || max <eps) max = 1.; 

		if(eps < ABS(diff)/max){
			char msg[48], val[48];
			sprintf(msg,"ERROR (proc=%d): a",me);
			sprintf(val,"=%lf, ",patch1[idx1]);
			print_subscript(msg,ndim,subscr1,val);
			sprintf(msg,"=%lf\n",patch2[idx2]);
			print_subscript(" b",ndim,subscr2,msg);
                        sleep(1);
                        return(1);
		}

		{ /* update subscript for the patches */
		   update_subscript(ndim, subscr1, lo1,hi1, dims1);
		   update_subscript(ndim, subscr2, lo2,hi2, dims2);
		}
	}

        return(0);
}


void f2c_adj_indices(Integer fsubscript[], int csubscript[], int n)
{
int i;
    for(i=0;i<n; i++)csubscript[i]=(int)fsubscript[i] -1;
}

void f2c_copy_indices(Integer fsubscript[], int csubscript[], int n)
{
int i;
    for(i=0;i<n; i++)csubscript[i]=(int)fsubscript[i];
}


Integer FATR compare_patches_(Integer *me,
                     double* eps, Integer *ndim, double *array1,    
                     Integer LO1[], Integer HI1[], Integer DIMS1[],
                     double *array2, Integer LO2[], Integer HI2[],
                     Integer DIMS2[])
{
int hi1[MAXDIM], lo1[MAXDIM], dims1[MAXDIM]; 
int hi2[MAXDIM], lo2[MAXDIM], dims2[MAXDIM]; 

    assert((int)*ndim < MAXDIM);

    f2c_adj_indices(HI1, hi1, (int)*ndim);
    f2c_adj_indices(HI2, hi2, (int)*ndim);
    f2c_adj_indices(LO1, lo1, (int)*ndim);
    f2c_adj_indices(LO2, lo2, (int)*ndim);
    f2c_copy_indices(DIMS1, dims1, (int)*ndim);
    f2c_copy_indices(DIMS2, dims2, (int)*ndim);

    return (Integer) compare_patches((int)*me, *eps, (int)*ndim, 
                     array1, lo1, hi1, dims1, array2, lo2, hi2, dims2);
}
    


void scale_patch(double alpha, int ndim, double *patch1, 
                 int lo1[], int hi1[], int dims1[])
{
	int i,j, elems=1;	
	int subscr1[MAXDIM];

	for(i=0;i<ndim;i++){   /* count # of elements in patch */
		int diff = hi1[i]-lo1[i];
		assert(diff < dims1[i]);
		elems *= diff+1;
		subscr1[i]= lo1[i];
	}

	/* scale element values in both patches */ 
	for(j=0; j< elems; j++){ 
		Integer idx1, offset1;
		
                /* calculate element Index from a subscript */
		idx1 = Index(ndim, subscr1, dims1);	

		if(j==0){
			offset1 =idx1;
		}
		idx1 -= offset1;

		patch1[idx1] *= alpha;
		update_subscript(ndim, subscr1, lo1,hi1, dims1);
	}	
}


void FATR scale_patch_(double *alpha, Integer *ndim, double *patch1, 
                 Integer LO1[], Integer HI1[], Integer DIMS1[])
{
int hi1[MAXDIM], lo1[MAXDIM], dims1[MAXDIM];

    assert((int)*ndim < MAXDIM);
    f2c_adj_indices(HI1, hi1, (int)*ndim);
    f2c_adj_indices(LO1, lo1, (int)*ndim);
    f2c_copy_indices(DIMS1, dims1, (int)*ndim);
    scale_patch(*alpha, (int)*ndim, patch1, lo1, hi1, dims1);
}


void FATR init_array_(double *a, Integer *ndim, Integer DIMS[])
{
int dims[MAXDIM];
    assert((int)*ndim < MAXDIM);

    f2c_copy_indices(DIMS, dims, (int)*ndim);
    init_array(a, (int)*ndim, dims);
}

void FATR print_range_(Integer *me, Integer LO[], Integer HI[], Integer *ndim)
{
int hi[MAXDIM], lo[MAXDIM];
char msg[100];

    assert((int)*ndim < MAXDIM);
    sprintf(msg,"%d: array section ",(int)*me);
    f2c_copy_indices(HI, hi, (int)*ndim);
    f2c_copy_indices(LO, lo, (int)*ndim);
    print_range(msg,(int)*ndim, lo, hi, "\n");
}

void FATR copy_range_(Integer *me, Integer LO1[], Integer HI1[], Integer *ndim1, Integer LO2[], Integer HI2[], Integer *ndim2)
{
int hi1[MAXDIM], lo1[MAXDIM], hi2[MAXDIM], lo2[MAXDIM];
char msg[100];

    assert((int)*ndim1 < MAXDIM);
    assert((int)*ndim2 < MAXDIM);
    sprintf(msg,"%d: copy ",(int)*me);
    f2c_copy_indices(HI1, hi1, (int)*ndim1);
    f2c_copy_indices(LO1, lo1, (int)*ndim1);
    print_range(msg,(int)*ndim1, lo1, hi1, "");
    sprintf(msg,"to ");
    f2c_copy_indices(HI2, hi2, (int)*ndim2);
    f2c_copy_indices(LO2, lo2, (int)*ndim2);
    print_range(msg,(int)*ndim2, lo2, hi2, "\n");
}

void FATR add_range_(Integer *me, Integer LO1[], Integer HI1[], Integer *ndim1, Integer LO2[], Integer HI2[], Integer *ndim2)
{
int hi1[MAXDIM], lo1[MAXDIM], hi2[MAXDIM], lo2[MAXDIM];
char msg[100];

    assert((int)*ndim1 < MAXDIM);
    assert((int)*ndim2 < MAXDIM);
    sprintf(msg,"%d: ",(int)*me);
    f2c_copy_indices(HI1, hi1, (int)*ndim1);
    f2c_copy_indices(LO1, lo1, (int)*ndim1);
    print_range(msg,(int)*ndim1, lo1, hi1, "");
    sprintf(msg,"+= ");
    f2c_copy_indices(HI2, hi2, (int)*ndim2);
    f2c_copy_indices(LO2, lo2, (int)*ndim2);
    print_range(msg,(int)*ndim2, lo2, hi2, "\n");
}

void FATR dot_range_(Integer *me, Integer LO1[], Integer HI1[], Integer *ndim1, Integer LO2[], Integer HI2[], Integer *ndim2)
{
int hi1[MAXDIM], lo1[MAXDIM], hi2[MAXDIM], lo2[MAXDIM];
char msg[100];

    assert((int)*ndim1 < MAXDIM);
    assert((int)*ndim2 < MAXDIM);
    sprintf(msg,"%d: dot ",(int)*me);
    f2c_copy_indices(HI1, hi1, (int)*ndim1);
    f2c_copy_indices(LO1, lo1, (int)*ndim1);
    print_range(msg,(int)*ndim1, lo1, hi1, "");
    sprintf(msg,", ");
    f2c_copy_indices(HI2, hi2, (int)*ndim2);
    f2c_copy_indices(LO2, lo2, (int)*ndim2);
    print_range(msg,(int)*ndim2, lo2, hi2, "\n");
}
