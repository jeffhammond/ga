/**********************************************************************/
/* store and retrieve parameters for disk resident array              */ 
/*       -- at present time, we use additional file for parameters    */
/**********************************************************************/


#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "global.h"
#include "drap.h"

#define MAX_HD_NAME_LEN 100
#define HD_NAME_EXT_LEN 10
#define HDLEN           80 
#define HD_EXT          ".info"


/*\ check file configuration: shared or independent files are used
 *  we'll verify if every process can access DRA param file
 *  if yes, then we have shared file, otherwise independent files
\*/
int dai_file_config(char* filename)
{
    /* on Cray  always use independent files */

#ifdef CRAY
    if(ga_nnodes_()==1) return 0;
    else return 1;
#else

char param_filename[MAX_HD_NAME_LEN];
Integer len;
char  sum='+';
Integer me=ga_nodeid_();
Integer nproc = ga_nnodes_();
Integer status;
stat_t info;

    if(nproc==1) return 0;

    /* build param file name */
    len = strlen(filename);
    if(len+HD_NAME_EXT_LEN >= MAX_HD_NAME_LEN)
       dai_error("dai_read_param: filename too long:",len);
    strcpy(param_filename,filename);
    strcat(param_filename,HD_EXT);

    /*    printf("checking file: %s\n",param_filename);fflush(stdout);*/

    status = (Integer) elio_stat(param_filename, &info);

    /* processor 0 created the file => it must be able to stat it */
    if(me==0 && status!= ELIO_OK)
                dai_error("dai_file_config: no access from 0",status);

    status = (status==ELIO_OK) ? 1 : 0; /* normalize status */

    /* combine status accross all processors */
    ga_igop(DRA_GOP_TYPE, &status, 1, &sum);

    /* 1     - only 0 can access the file => independent files 
     * nproc - all can access it => shared file
     * otherwise - same processors can access it => something is wrong!!!
     */ 
    if(status == 1) return(1);
    else if(status == nproc) return 0;
         else dai_error("dai_file_config: confusing file configuration",status); 
    return 1;
#endif
}
    


/*\ Retrive parameters of a disk array from the disk
\*/
int dai_read_param(char* filename,Integer d_a)
{
FILE *fd;
char param_filename[MAX_HD_NAME_LEN];
Integer len;
Integer me=ga_nodeid_();
Integer brd_type=DRA_BRD_TYPE, orig, dra_hndl=d_a+DRA_OFFSET;
long input;
int rc=0;
char dummy[HDLEN];

  ga_sync_();
    
  if(!me){ /* only process 0 reads param file */

    /* build param file name */
    len = strlen(filename);
    if(len+HD_NAME_EXT_LEN >= MAX_HD_NAME_LEN)
       dai_error("dai_read_param: filename too long:",len);
    strcpy(param_filename,filename);
    strcat(param_filename,HD_EXT);

    if((fd=fopen(param_filename,"r"))){

      if(!fscanf(fd,"%ld", &input))  dai_error("dai_read_param:dim1",0);
      DRA[dra_hndl].dim1 = (Integer) input;
      if(!fscanf(fd,"%ld",&input))   dai_error("dai_read_param:dim2",0);
      DRA[dra_hndl].dim2 = (Integer) input;

      if(!fscanf(fd,"%ld",&input))   dai_error("dai_read_param:type",0);
      DRA[dra_hndl].type = (Integer) input;
      if(!fscanf(fd,"%ld",&input))   dai_error("dai_read_param:layout",0);
      DRA[dra_hndl].layout = (Integer) input;

      if(!fscanf(fd,"%ld",&input))   dai_error("dai_read_param:chunk1",0);
      DRA[dra_hndl].chunk1 = (Integer) input;
      if(!fscanf(fd,"%ld",&input))   dai_error("dai_read_param:chunk2",0);
      DRA[dra_hndl].chunk2 = (Integer) input;

      fgets(dummy,HDLEN,fd); /*advance to next line*/
      if(!fgets(DRA[dra_hndl].name,DRA_MAX_NAME,fd))dai_error("dai_read_param:name",0);

      if(fclose(fd))dai_error("dai_read_param: fclose failed",0);
   }else rc = -1;
  }


  orig = 0; len=sizeof(int);
  ga_brdcst_(&brd_type, &rc, &len, &orig);
  if(rc) return(rc);

  /* process 0 broadcasts data to everybody else                            */
  /* for 6 Integers there shouldn't be alignement padding in the structure */
  /* the integers are followed by array name */
  len = 6*sizeof(Integer)+DRA_MAX_NAME+1;
  ga_brdcst_(&brd_type, DRA + dra_hndl, &len, &orig);
  
  return(rc);
}
  

  
/*\ Store parameters of a disk array on the disk
\*/
void dai_write_param(char* filename,Integer d_a)
{
Integer len;
FILE *fd;
char param_filename[MAX_HD_NAME_LEN];
Integer me=ga_nodeid_(), dra_hndl=d_a+DRA_OFFSET;

  ga_sync_();
   
  if(!me){ /* only process 0 writes param file */

    /* build param file name */
    len = strlen(filename);
    if(len + HD_NAME_EXT_LEN >= MAX_HD_NAME_LEN)
       dai_error("dai_write_param: filename too long:",len);
    strcpy(param_filename,filename);
    strcat(param_filename,HD_EXT);

    if(! (fd = fopen(param_filename,"w")) )
                                dai_error("dai_write_param:open failed",0);

    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].dim1)) 
                                dai_error("dai_write_param:dim1",0);
    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].dim2)) 
                                dai_error("dai_write_param:dim2",0);
    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].type)) 
                                dai_error("dai_write_param:type",0);
    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].layout))
                                dai_error("dai_write_param:layout",0);
    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].chunk1))
                                dai_error("dai_write_param:chunk1",0);
    if(!fprintf(fd,"%ld ",(long)DRA[dra_hndl].chunk2))
                                dai_error("dai_write_param:chunk2",0);
    if(!fprintf(fd,"\n%s\n",DRA[dra_hndl].name))
                                dai_error("dai_write_param:name",0);

    if(fclose(fd))dai_error("dai_write_param: fclose failed",0);
  }

  ga_sync_();
}



/*\ Delete info file
\*/
void dai_delete_param(char* filename,Integer d_a)
{
char param_filename[MAX_HD_NAME_LEN];
int len;
Integer me=ga_nodeid_();

  ga_sync_();

  if(!me){ /* only process 0 reads param file */

    /* build param file name */
    len = strlen(filename);
    if(len+HD_NAME_EXT_LEN >= MAX_HD_NAME_LEN)
       dai_error("dai_read_param: filename too long:",len);
    strcpy(param_filename,filename);
    strcat(param_filename,HD_EXT);

    if(unlink(param_filename)) dai_error("dai_delete_param failed",d_a);
  }
}
