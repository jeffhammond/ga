/***
   AUTHOR
     Jialin Ju, PNNL
   NAME
     myrinet.h
   PURPOSE
     
   NOTES
     
   HISTORY
     jju - Mar 1, 2000: Created.
     jn  - Oct, 2000: restructured memory allocation, extra optimization
                      bug fixes
***/

#ifndef MYRINET_H
#define MYRINET_H

#define GM_STRONG_TYPES 0 
#include "gm.h"
/* in GM 1.4 memory registration got so slow we cannot use 0-copy protocols
 * we are disabling it for medium messages by changing thresholds */
#if defined(GM_MAX_DEFAULT_MESSAGE_SIZE) && !defined(GM_ENABLE_PROGRESSION)
#   define GM_1_2      /* most likely we have GM <1.4 */
#endif

/*  CLIENT_BUF_BYPASS should be defined to enable zero-copy protocol
    that uses registration -- sloooooow on GM >1.2 */
extern int _armci_bypass;
#define CLIENT_BUF_BYPASS_ 
#ifdef __i386__
# ifdef GM_1_2
#   define LONG_GET_THRESHOLD 66248
#   define LONG_GET_THRESHOLD_STRIDED 3000
# else
#   define LONG_GET_THRESHOLD 1000000
#   define LONG_GET_THRESHOLD_STRIDED 30000 
# endif
#define INTERLEAVE_GET_THRESHOLD 66248
#else
#define LONG_GET_THRESHOLD 524288
#define LONG_GET_THRESHOLD_STRIDED 30000 
#define INTERLEAVE_GET_THRESHOLD 524288 
#endif

#define PIPE_BUFSIZE  (8*1024 -128)
#define PIPE_MIN_BUFSIZE 1024 
#define PIPE_MEDIUM_BUFSIZE (4*8192)

#define MEM_NONUNIFORM_HIGH

#ifdef MEM_UNIFORM_HIGH
#define NUMRCVBUFS 43 
#elif defined(MEM_NONUNIFORM_HIGH)
#define NUMRCVBUFS  108
#else 
#define NUMRCVBUFS 30
#endif


/* call back */ 
#define ARMCI_GM_SENDING 3

/* msg ack */ 
#define ARMCI_GM_CLEAR   0

/* context for callback routine */
typedef struct {
    int tag;
    volatile int done;
} armci_gm_context_t;
extern struct gm_port *gmpi_gm_port;
#define SND_BUFLEN (MSG_BUFLEN +128) 

#define INIT_SEND_BUF(_cntr,_snd,_rcv) (_cntr).done=ARMCI_GM_CLEAR

#define CLEAR_SEND_BUF_FIELD(_cntr, _s, _r,_t) if((_cntr).done==ARMCI_GM_SENDING){\
MPI_Status status;\
int flag;\
while((_cntr).done==ARMCI_GM_SENDING)\
MPI_Iprobe(armci_me, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);\
if((_cntr).done == ARMCI_GM_FAILED)\
       armci_die("armci_client_send_complete: failed code=",(_cntr).done);}\


#define MULTIPLE_SND_BUFS 
#ifdef MULTIPLE_SND_BUFS
#  define BUF_ALLOCATE(_size) gm_dma_malloc(gmpi_gm_port,_size+256) 
#  define BUF_EXTRA_FIELD_T armci_gm_context_t 
#  define GET_SEND_BUFFER _armci_buf_get
#  define FREE_SEND_BUFFER _armci_buf_release  
#else
#  define GET_SEND_BUFFER(x)(char*)(((armci_gm_context_t*)MessageSndBuffer)+1);\
        armci_client_send_complete((armci_gm_context_t*)MessageSndBuffer)
#  define FREE_SEND_BUFFER(x) 
#endif

#define BALANCE_BUFFERS
#ifdef BALANCE_BUFFERS
#  define BALANCE_FACTOR 1.6
#  define BALANCE_BUFSIZE 190000
#endif

/* two ports used by ARMCI and their boards iff STATIC_PORTS defined */
#define ARMCI_GM_SERVER_RCV_PORT 5
#define ARMCI_GM_SERVER_RCV_DEV 0
#define ARMCI_GM_SERVER_SND_PORT 6
#define ARMCI_GM_SERVER_SND_DEV 0

/* message types */
#define ARMCI_GM_BLOCKING 1
#define ARMCI_GM_NONBLOCKING 2

#define ARMCI_GM_FAILED  -1

typedef struct {
    void *data_ptr;         /* pointer where the data should go */
    long ack;               /* header ack */
} msg_tag_t;

#include <mpi.h>

extern void armci_server_send_ack(int client);
extern int armci_pin_contig(void *ptr, int bytes);
extern void armci_unpin_contig(void *ptr, int bytes);
extern void armci_serv_send_ack(int client);
extern int armci_pin_memory(void *ptr, int stride_arr[], int count[], int lev);
extern void armci_unpin_memory(void *ptr,int stride_arr[],int count[],int lev);
extern int armci_serv_send_complete();
extern void armci_server_direct_send(int p,char *src,char *dst,int len,int typ);
extern void armci_data_server(void *msg);
extern void armci_serv_send_nonblocking_complete(int max_outstanding);
extern void armci_wait_for_data_bypass();
extern int  armci_wait_pin_client(int);
extern void armci_client_send_ack(int p, int success);
extern void armci_gm_freebuf(void *ptr);
extern char* armci_gm_get_send_buf(int bufsize);
extern void armci_gm_free_send_buf(void *ptr);
extern char* armci_gm_getbuf(size_t size);
extern void armci_client_send_complete(armci_gm_context_t*);
extern void  armci_check_context_for_complete(int);

#endif /* MYRINET_H */
