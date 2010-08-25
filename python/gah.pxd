from libc.stdio  cimport FILE
from libc.stdint cimport int64_t

cdef extern from "typesf2c.h":
    ctypedef int Integer
    ctypedef float Real
    ctypedef double DoublePrecision
    ctypedef struct DoubleComplex:
        DoublePrecision real
        DoublePrecision imag
    ctypedef struct SingleComplex:
        Real real
        Real imag

cdef extern from "ga.h":
    ctypedef struct ga_nbhdl_t:
        pass
    ctypedef void* nbhandle

    void          GA_Abs_value(int g_a) 
    void          GA_Abs_value_patch(int g_a, int *lo, int *hi)
    void          GA_Add_constant(int g_a, void* alpha)
    void          GA_Add_constant_patch(int g,int *lo,int *hi,void *alpha)
    void          GA_Add_diagonal(int g_a, int g_v)
    void          GA_Add(void *alpha, int g_a, void* beta, int g_b, int g_c) 
    int           GA_Allocate(int g_a)
    int           GA_Assemble_duplicate(int g_a, char *name, void *ptr)
    void          GA_Brdcst(void *buf, int lenbuf, int root)
    SingleComplex GA_Cdot(int g_a, int g_b) 
    void          GA_Cgop(SingleComplex x[], int n, char *op)
    void          GA_Cgemm(char ta, char tb, int m, int n, int k, SingleComplex alpha, int g_a, int g_b, SingleComplex beta, int g_c )
    void          GA_Check_handle(int g_a, char *string)
    int           GA_Cluster_nnodes()
    int           GA_Cluster_nodeid()
    int           GA_Cluster_nprocs(int x)
    int           GA_Cluster_procid(int x, int y)
    int           GA_Cluster_proc_nodeid(int proc)
    int           GA_Compare_distr(int g_a, int g_b) 
    void          GA_Copy(int g_a, int g_b) 
    int           GA_Create_handle()
    int           GA_Create_mutexes(int number)
    double        GA_Ddot(int g_a, int g_b) 
    void          GA_Destroy(int g_a)
    int           GA_Destroy_mutexes()
    void          GA_Dgemm(char ta, char tb, int m, int n, int k, double alpha, int g_a, int g_b, double beta, int g_c )
    void          GA_Dgop(double x[], int n, char *op)
    void          GA_Diag(int g_a, int g_s, int g_v, void *eval)
    void          GA_Diag_reuse(int reuse, int g_a, int g_s, int g_v, void *eval)
    void          GA_Diag_seq(int g_a, int g_s, int g_v, void *eval)
    void          GA_Diag_std(int g_a, int g_v, void *eval)
    void          GA_Diag_std_seq(int g_a, int g_v, void *eval)
    int           GA_Duplicate(int g_a, char* array_name)
    void          GA_Elem_divide(int g_a, int g_b, int g_c)
    void          GA_Elem_divide_patch(int g_a,int *alo,int *ahi, int g_b,int *blo,int *bhi,int g_c,int *clo,int *chi)
    void          GA_Elem_maximum(int g_a, int g_b, int g_c)
    void          GA_Elem_maximum_patch(int g_a,int *alo,int *ahi, int g_b,int *blo,int *bhi,int g_c,int *clo,int *chi)
    void          GA_Elem_minimum(int g_a, int g_b, int g_c)
    void          GA_Elem_minimum_patch(int g_a,int *alo,int *ahi, int g_b,int *blo,int *bhi,int g_c,int *clo,int *chi)
    void          GA_Elem_multiply(int g_a, int g_b, int g_c)
    void          GA_Elem_multiply_patch(int g_a,int *alo,int *ahi, int g_b,int *blo,int *bhi,int g_c,int *clo,int *chi)
    void          GA_Error(char *str, int code)
    void          GA_Fast_merge_mirrored(int g_a)
    float         GA_Fdot(int g_a, int g_b)
    void          GA_Fence()
    void          GA_Fgop(float x[], int n, char *op)
    void          GA_Fill(int g_a, void *value)
    void          GA_Freemem(void* ptr)
    int           GA_Get_debug()
    void          GA_Get_diag(int g_a, int g_v)
    int           GA_Get_dimension(int g_a)
    void*         GA_Getmem(int type, int nelem, int grp_id)
    void          GA_Get_mirrored_block(int g_a, int nblock, int lo[], int hi[])
    int           GA_Get_pgroup(int g_a)
    void          GA_Gop(int type, void *x, int n, char *op)
    int           GA_Has_ghosts(int g_a)
    int           GA_Idot(int g_a, int g_b)
    void          GA_Igop(int x[], int n, char *op)
    void          GA_Init_fence()
    void          GA_Initialize_args(int *argc, char ***argv)
    void          GA_Initialize_ltd(size_t limit)
    void          GA_Initialize()
    size_t        GA_Inquire_memory()
    char*         GA_Inquire_name(int g_a)
    int           GA_Is_mirrored(int g_a)
    long          GA_Ldot(int g_a, int g_b)
    void          GA_Lgop(long x[], int n, char *op)
    long long     GA_Lldot(int g_a, int g_b)
    void          GA_Llgop(long long x[], int n, char *op)
    int           GA_Llt_solve(int g_a, int g_b)
    void          GA_Lock(int mutex)
    void          GA_Lu_solve(char tran, int g_a, int g_b)
    void          GA_Mask_sync(int first, int last)
    void          GA_Matmul_patch(char transa, char transb, void* alpha, void *beta, int g_a, int ailo, int aihi, int ajlo, int ajhi, int g_b, int bilo, int bihi, int bjlo, int bjhi, int g_c, int cilo, int cihi, int cjlo, int cjhi)
    void          GA_Median(int g_a, int g_b, int g_c, int g_m)
    void          GA_Median_patch(int g_a, int *alo, int *ahi, int g_b, int *blo, int *bhi, int g_c, int *clo, int *chi, int g_m, int *mlo, int *mhi)
    size_t        GA_Memory_avail()
    int           GA_Memory_limited()
    void          GA_Merge_mirrored(int g_a)
    void          GA_Nblock(int g_a, int *nblock)
    int           GA_Ndim(int g_a)
    int           GA_Nnodes()
    int           GA_Nodeid()
    void          GA_Norm1(int g_a, double *nm)
    void          GA_Norm_infinity(int g_a, double *nm)
    int           GA_Num_mirrored_seg(int g_a)
    void          GA_Pgroup_brdcst(int grp, void *buf, int lenbuf, int root)
    int           GA_Pgroup_create(int *list, int count)
    int           GA_Pgroup_destroy(int grp)
    void          GA_Pgroup_dgop(int grp, double x[], int n, char *op)
    void          GA_Pgroup_fgop(int grp_id, float x[], int n, char *op)   
    int           GA_Pgroup_get_default()
    int           GA_Pgroup_get_mirror()
    int           GA_Pgroup_get_world()
    void          GA_Pgroup_igop(int grp, int x[], int n, char *op)
    void          GA_Pgroup_lgop(int grp, long x[], int n, char *op)
    int           GA_Pgroup_nnodes(int grp_id)
    int           GA_Pgroup_nodeid(int grp_id)
    void          GA_Pgroup_set_default(int p_handle)
    int           GA_Pgroup_split(int grp_id, int num_group)
    int           GA_Pgroup_split_irreg(int grp_id, int color, int key)
    void          GA_Pgroup_sync(int grp_id)
    void          GA_Print_distribution(int g_a) 
    void          GA_Print_file(FILE *file, int g_a)
    void          GA_Print(int g_a)
    void          GA_Print_patch(int g_a,int ilo,int ihi,int jlo,int jhi,int pretty)
    void          GA_Print_stats()
    void          GA_Randomize(int g_a, void *value)
    void          GA_Recip(int g_a)
    void          GA_Recip_patch(int g_a,int *lo, int *hi)
    void          GA_Register_stack_memory(void * (*ext_alloc)(size_t, int, char *), void (*ext_free)(void *))
    void          GA_Scale_cols(int g_a, int g_v)
    void          GA_Scale(int g_a, void *value) 
    void          GA_Scale_rows(int g_a, int g_v)
    void          GA_Scan_add(int g_a, int g_b, int g_sbit, int lo, int hi, int excl)
    void          GA_Scan_copy(int g_a, int g_b, int g_sbit, int lo, int hi)
    void          GA_Set_array_name(int g_a, char *name)
    void          GA_Set_block_cyclic(int g_a, int dims[])
    void          GA_Set_block_cyclic_proc_grid(int g_a, int block[], int proc_grid[])
    void          GA_Set_chunk(int g_a, int chunk[])
    void          GA_Set_data(int g_a, int ndim, int dims[], int type)
    void          GA_Set_debug(int flag)
    void          GA_Set_diagonal(int g_a, int g_v)
    void          GA_Set_ghost_corner_flag(int g_a, int flag)
    void          GA_Set_ghosts(int g_a, int width[])
    void          GA_Set_irreg_distr(int g_a, int map[], int block[])
    void          GA_Set_irreg_flag(int g_a, int flag)
    void          GA_Set_memory_limit(size_t limit)
    void          GA_Set_pgroup(int g_a, int p_handle)
    void          GA_Set_restricted(int g_a, int list[], int size)
    void          GA_Set_restricted_range(int g_a, int lo_proc, int hi_proc)
    void          GA_Sgemm(char ta, char tb, int m, int n, int k, float alpha, int g_a, int g_b, float beta, int g_c )
    void          GA_Shift_diagonal(int g_a, void *c)
    int           GA_Solve(int g_a, int g_b)
    int           GA_Spd_invert(int g_a)
    void          GA_Step_bound_info(int g_xx, int g_vv, int g_xxll, int g_xxuu, void *boundmin, void *wolfemin, void *boundmax)
    void          GA_Step_bound_info_patch(int g_xx, int *xxlo, int *xxhi, int g_vv, int *vvlo, int *vvhi, int g_xxll, int *xxlllo, int *xxllhi, int g_xxuu, int *xxuulo, int *xxuuhi, void *boundmin, void *wolfemin, void *boundmax)
    void          GA_Step_max(int g_a, int g_b, void *step)
    void          GA_Step_max_patch(int g_a, int *alo, int *ahi, int g_b, int *blo, int *bhi, void *step)
    void          GA_Summarize(int verbose)
    void          GA_Symmetrize(int g_a)
    void          GA_Sync()
    void          GA_Terminate()
    int           GA_Total_blocks(int g_a)   
    void          GA_Transpose(int g_a, int g_b)
    void          GA_Unlock(int mutex)
    void          GA_Update_ghosts(int g_a)
    int           GA_Uses_fapi()
    int           GA_Uses_ma()
    double        GA_Wtime()
    DoubleComplex GA_Zdot(int g_a, int g_b) 
    void          GA_Zgop(DoubleComplex x[], int n, char *op)
    void          GA_Zero_diagonal(int g_a)
    void          GA_Zero(int g_a) 
    void          GA_Zgemm(char ta, char tb, int m, int n, int k, DoubleComplex alpha, int g_a, int g_b, DoubleComplex beta, int g_c )
    void          NGA_Access_block_grid(int g_a, int index[], void *ptr, int ld[])
    void          NGA_Access_block(int g_a, int idx, void *ptr, int ld[])
    void          NGA_Access_block_segment(int g_a, int proc, void *ptr, int *len)
    void          NGA_Access_ghost_element(int g_a,  void *ptr, int subscript[], int ld[])
    void          NGA_Access_ghosts(int g_a, int dims[], void *ptr, int ld[])
    void          NGA_Access(int g_a, int lo[], int hi[], void *ptr, int ld[])
    void          NGA_Acc(int g_a, int lo[], int hi[],void* buf,int ld[],void* alpha)
    void          NGA_Add_patch(void * alpha, int g_a, int alo[], int ahi[], void * beta,  int g_b, int blo[], int bhi[], int g_c, int clo[], int chi[])
    SingleComplex NGA_Cdot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    void          NGA_Copy_patch(char trans, int g_a, int alo[], int ahi[], int g_b, int blo[], int bhi[])
    int           NGA_Create_config(int type,int ndim,int dims[], char *name, int chunk[], int p_handle)
    int           NGA_Create_ghosts_config(int type,int ndim,int dims[], int width[], char *name, int chunk[], int p_handle)
    int           NGA_Create_ghosts(int type,int ndim,int dims[], int width[], char *name, int chunk[])
    int           NGA_Create_ghosts_irreg_config(int type,int ndim,int dims[], int width[], char *name, int map[], int nblock[], int p_handle)
    int           NGA_Create_ghosts_irreg(int type,int ndim,int dims[], int width[], char *name, int map[], int nblock[])
    int           NGA_Create(int type,int ndim,int dims[], char *name, int chunk[])
    int           NGA_Create(int type,int ndim,int dims[], char *name, int chunk[])
    int           NGA_Create_irreg_config(int type,int ndim,int dims[],char *name, int map[], int block[], int p_handle)
    int           NGA_Create_irreg(int type,int ndim,int dims[],char *name, int map[], int block[])
    double        NGA_Ddot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    void          NGA_Distribution(int g_a, int iproc, int lo[], int hi[]) 
    float         NGA_Fdot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    void          NGA_Fill_patch(int g_a, int lo[], int hi[], void *val)
    void          NGA_Gather(int g_a, void *v, int* subsArray[], int n)
    void          NGA_Gather_flat(int g_a, void *v, int subsArray[], int n)
    void          NGA_Get_ghost_block(int g_a, int lo[], int hi[], void *buf, int ld[])
    void          NGA_Get(int g_a, int lo[], int hi[], void* buf, int ld[]) 
    int           NGA_Idot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    void          NGA_Inquire(int g_a, int *type, int *ndim, int dims[])
    long          NGA_Ldot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    long long     NGA_Lldot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    int           NGA_Locate(int g_a, int subscript[])
    int           NGA_Locate_num_blocks(int g_a, int lo[], int hi[])
    int           NGA_Locate_region(int g_a,int lo[],int hi[],int map[],int procs[])
    void          NGA_Matmul_patch(char transa, char transb, void* alpha, void *beta, int g_a, int alo[], int ahi[], int g_b, int blo[], int bhi[], int g_c, int clo[], int chi[]) 
    void          NGA_Merge_distr_patch(int g_a, int alo[], int ahi[], int g_b, int blo[], int bhi[])
    void          NGA_NbAcc(int g_a,int lo[], int hi[],void* buf,int ld[],void* alpha, ga_nbhdl_t* nbhandle)
    void          NGA_NbGet_ghost_dir(int g_a, int mask[], ga_nbhdl_t* handle)
    void          NGA_NbGet(int g_a, int lo[], int hi[], void* buf, int ld[], ga_nbhdl_t* nbhandle)
    void          NGA_NbPut(int g_a, int lo[], int hi[], void* buf, int ld[], ga_nbhdl_t* nbhandle)
    int           NGA_NbTest(ga_nbhdl_t* nbhandle)
    int           NGA_NbWait(ga_nbhdl_t* nbhandle)
    void          NGA_Periodic_acc(int g_a, int lo[], int hi[],void* buf,int ld[],void* alpha)
    void          NGA_Periodic_get(int g_a, int lo[], int hi[], void* buf, int ld[]) 
    void          NGA_Periodic_put(int g_a, int lo[], int hi[], void* buf, int ld[]) 
    void          NGA_Print_patch(int g_a, int lo[], int hi[], int pretty)
    void          NGA_Proc_topology(int g_a, int proc, int coord[])
    void          NGA_Put(int g_a, int lo[], int hi[], void* buf, int ld[]) 
    long          NGA_Read_inc(int g_a, int subscript[], long inc)
    void          NGA_Release_block_grid(int g_a, int index[])
    void          NGA_Release_block(int g_a, int idx)
    void          NGA_Release_block_segment(int g_a, int idx)
    void          NGA_Release_ghost_element(int g_a, int index[])
    void          NGA_Release_ghosts(int g_a)
    void          NGA_Release(int g_a, int lo[], int hi[])
    void          NGA_Release_update_block_grid(int g_a, int index[])
    void          NGA_Release_update_block(int g_a, int idx)
    void          NGA_Release_update_block_segment(int g_a, int idx)
    void          NGA_Release_update_ghost_element(int g_a, int index[])
    void          NGA_Release_update_ghosts(int g_a)
    void          NGA_Release_update(int g_a, int lo[], int hi[])
    void          NGA_Scale_patch(int g_a, int lo[], int hi[], void *alpha)
    void          NGA_Scatter_acc(int g_a, void *v, int* subsArray[], int n, void *alpha)
    void          NGA_Scatter(int g_a, void *v, int* subsArray[], int n)
    void          NGA_Select_elem(int g_a, char* op, void* val, int *index)
    void          NGA_Strided_acc(int g_a, int lo[], int hi[], int skip[], void* buf, int ld[], void *alpha) 
    void          NGA_Strided_get(int g_a, int lo[], int hi[], int skip[], void* buf, int ld[]) 
    void          NGA_Strided_put(int g_a, int lo[], int hi[], int skip[], void* buf, int ld[]) 
    int           NGA_Update_ghost_dir(int g_a, int dimension, int idir, int flag)
    DoubleComplex NGA_Zdot_patch(int g_a, char t_a, int alo[], int ahi[], int g_b, char t_b, int blo[], int bhi[])
    void          NGA_Zero_patch(int g_a, int lo[], int hi[])
    
    void          GA_Abs_value_patch64(int g_a, int64_t *lo, int64_t *hi)
    void          GA_Add_constant_patch64(int g,int64_t *lo,int64_t *hi,void *alpha)
    void          GA_Cgemm64(char ta, char tb, int64_t m, int64_t n, int64_t k, SingleComplex alpha, int g_a, int g_b, SingleComplex beta, int g_c )   
    void          GA_Dgemm64(char ta, char tb, int64_t m, int64_t n, int64_t k, double alpha, int g_a, int g_b, double beta, int g_c )
    void          GA_Elem_divide_patch64(int g_a, int64_t alo[], int64_t ahi[], int g_b, int64_t blo[], int64_t bhi[], int g_c, int64_t  clo[], int64_t chi[])
    void          GA_Elem_maximum_patch64(int g_a,int64_t *alo,int64_t *ahi, int g_b,int64_t *blo,int64_t *bhi, int g_c,int64_t *clo,int64_t *chi)
    void          GA_Elem_minimum_patch64(int g_a,int64_t *alo,int64_t *ahi, int g_b,int64_t *blo,int64_t *bhi, int g_c,int64_t *clo,int64_t *chi)
    void          GA_Elem_multiply_patch64(int g_a, int64_t alo[], int64_t ahi[], int g_b, int64_t blo[], int64_t bhi[], int g_c, int64_t clo[], int64_t chi[])
    void          GA_Get_block_info(int g_a, int num_blocks[], int block_dims[])
    void          GA_Get_mirrored_block64(int g_a, int nblock, int64_t lo[], int64_t hi[])
    void          GA_Matmul_patch64(char transa, char transb, void* alpha, void *beta, int g_a, int64_t ailo, int64_t aihi, int64_t ajlo, int64_t ajhi, int g_b, int64_t bilo, int64_t bihi, int64_t bjlo, int64_t bjhi, int g_c, int64_t cilo, int64_t cihi, int64_t cjlo, int64_t cjhi)
    void          GA_Median_patch64(int g_a, int64_t *alo, int64_t *ahi, int g_b, int64_t *blo, int64_t *bhi, int g_c, int64_t *clo, int64_t *chi, int g_m, int64_t *mlo, int64_t *mhi)
    void          GA_Pack64(int g_src, int g_dest, int g_mask, int64_t lo, int64_t hi, int64_t *icount)
    void          GA_Pack(int g_src, int g_dest, int g_mask, int lo, int hi, int *icount)
    void          GA_Patch_enum64(int g_a, int64_t lo, int64_t hi, int64_t istart, int64_t inc)
    void          GA_Patch_enum(int g_a, int lo, int hi, int istart, int inc)
    void          GA_Recip_patch64(int g_a,int64_t *lo, int64_t *hi)
    void          GA_Scan_add64(int g_a, int g_b, int g_sbit, int64_t lo, int64_t hi, int excl)
    void          GA_Scan_copy64(int g_a, int g_b, int g_sbit, int64_t lo, int64_t hi)
    void          GA_Set_chunk64(int g_a, int64_t chunk[])
    void          GA_Set_data64(int g_a, int ndim, int64_t dims[], int type)
    void          GA_Set_ghosts64(int g_a, int64_t width[])
    void          GA_Set_irreg_distr64(int g_a, int64_t map[], int64_t block[])
    void          GA_Sgemm64(char ta, char tb, int64_t m, int64_t n, int64_t k, float alpha, int g_a, int g_b, float beta,  int g_c )
    void          GA_Step_bound_info_patch64(int g_xx, int64_t xxlo[], int64_t xxhi[], int g_vv, int64_t vvlo[], int64_t vvhi[], int g_xxll, int64_t xxlllo[], int64_t xxllhi[], int64_t g_xxuu,  int64_t xxuulo[], int64_t xxuuhi[], void *boundmin, void *wolfemin, void *boundmax)
    void          GA_Step_max_patch64(int g_a, int64_t alo[], int64_t  ahi[], int g_b, int64_t blo[], int64_t  bhi[], void *step)
    void          GA_Unpack64(int g_src, int g_dest, int g_mask, int64_t lo, int64_t hi, int64_t *icount)
    void          GA_Unpack(int g_src, int g_dest, int g_mask, int lo, int hi, int *icount)
    void          GA_Zgemm64(char ta, char tb, int64_t m, int64_t n, int64_t k, DoubleComplex alpha, int g_a, int g_b, DoubleComplex beta, int g_c )
    void          NGA_Acc64(int g_a, int64_t lo[], int64_t hi[],void* buf,int64_t ld[],void* alpha)
    void          NGA_Access64(int g_a, int64_t lo[], int64_t hi[], void *ptr, int64_t ld[])
    void          NGA_Access_block64(int g_a, int64_t idx, void *ptr, int64_t ld[])
    void          NGA_Access_block_grid64(int g_a, int64_t index[], void *ptr, int64_t ld[])
    void          NGA_Access_block_segment64(int g_a, int proc, void *ptr, int64_t *len)
    void          NGA_Access_ghost_element64(int g_a,  void *ptr, int64_t subscript[], int64_t ld[])
    void          NGA_Access_ghosts64(int g_a, int64_t dims[], void *ptr, int64_t ld[])
    void          NGA_Add_patch64(void * alpha, int g_a, int64_t alo[], int64_t ahi[], void * beta,  int g_b, int64_t blo[], int64_t bhi[], int g_c, int64_t clo[], int64_t chi[])
    SingleComplex NGA_Cdot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    void          NGA_Copy_patch64(char trans, int g_a, int64_t alo[], int64_t ahi[], int g_b, int64_t blo[], int64_t bhi[])
    int           NGA_Create64(int type,int ndim,int64_t dims[], char *name, int64_t chunk[])
    int           NGA_Create_config64(int type,int ndim,int64_t dims[], char *name, int64_t chunk[], int p_handle)
    int           NGA_Create_ghosts64(int type,int ndim,int64_t dims[], int64_t width[], char *name, int64_t chunk[])
    int           NGA_Create_ghosts_config64(int type,int ndim,int64_t dims[], int64_t width[], char *name, int64_t chunk[], int p_handle)
    int           NGA_Create_ghosts_irreg64(int type,int ndim,int64_t dims[], int64_t width[], char *name, int64_t map[], int64_t nblock[])
    int           NGA_Create_ghosts_irreg_config64(int type,int ndim,int64_t dims[], int64_t width[], char *name, int64_t map[], int64_t nblock[], int p_handle)
    int           NGA_Create_irreg64(int type,int ndim,int64_t dims[],char *name, int64_t map[], int64_t block[])
    int           NGA_Create_irreg_config64(int type,int ndim,int64_t dims[],char *name, int64_t map[], int64_t block[], int p_handle)
    double        NGA_Ddot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    void          NGA_Distribution64(int g_a, int iproc, int64_t lo[], int64_t hi[])
    float         NGA_Fdot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    void          NGA_Fill_patch64(int g_a, int64_t lo[], int64_t hi[], void *val)
    void          NGA_Gather64(int g_a, void *v, int64_t* subsArray[], int64_t n)
    void          NGA_Gather_flat64(int g_a, void *v, int64_t subsArray[], int64_t n)
    void          NGA_Get64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[]) 
    void          NGA_Get_ghost_block64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[]) 
    int           NGA_Idot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    void          NGA_Inquire64(int g_a, int *type, int *ndim, int64_t dims[])
    long          NGA_Ldot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    long long     NGA_Lldot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    int           NGA_Locate64(int g_a, int64_t subscript[])
    int           NGA_Locate_region64(int g_a,int64_t lo[],int64_t hi[],int64_t map[],int procs[])
    void          NGA_Matmul_patch64(char transa, char transb, void* alpha, void *beta, int g_a, int64_t alo[], int64_t ahi[], int g_b, int64_t blo[], int64_t bhi[], int g_c, int64_t clo[], int64_t chi[]) 
    void          NGA_Merge_distr_patch64(int g_a, int64_t alo[], int64_t ahi[], int g_b, int64_t blo[], int64_t bhi[])
    void          NGA_NbAcc64(int g_a,int64_t lo[],int64_t hi[],void* buf,int64_t ld[],void* alpha, ga_nbhdl_t* nbhandle)
    void          NGA_NbGet64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[], ga_nbhdl_t* nbhandle)
    void          NGA_NbGet_ghost_dir64(int g_a, int64_t mask[], ga_nbhdl_t* handle)
    void          NGA_NbPut64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[], ga_nbhdl_t* nbhandle)
    void          NGA_Periodic_acc64(int g_a, int64_t lo[], int64_t hi[],void* buf,int64_t ld[],void* alpha)
    void          NGA_Periodic_get64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[]) 
    void          NGA_Periodic_put64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[]) 
    void          NGA_Print_patch64(int g_a, int64_t lo[], int64_t hi[], int pretty)
    void          NGA_Put64(int g_a, int64_t lo[], int64_t hi[], void* buf, int64_t ld[]) 
    long          NGA_Read_inc64(int g_a, int64_t subscript[], long inc)
    void          NGA_Release64(int g_a, int64_t lo[], int64_t hi[])
    void          NGA_Release_ghost_element64(int g_a, int64_t index[])
    void          NGA_Release_update64(int g_a, int64_t lo[], int64_t hi[])
    void          NGA_Release_update_ghost_element64(int g_a, int64_t index[])
    void          NGA_Scale_patch64(int g_a, int64_t lo[], int64_t hi[], void *alpha)
    void          NGA_Scatter64(int g_a, void *v, int64_t* subsArray[], int64_t n)
    void          NGA_Scatter_acc64(int g_a, void *v, int64_t* subsArray[], int64_t n, void *alpha)
    void          NGA_Select_elem64(int g_a, char* op, void* val, int64_t* index)
    void          NGA_Strided_acc64(int g_a, int64_t lo[], int64_t hi[], int64_t skip[], void* buf, int64_t  ld[], void *alpha) 
    void          NGA_Strided_get64(int g_a, int64_t lo[], int64_t hi[], int64_t skip[], void* buf, int64_t  ld[]) 
    void          NGA_Strided_put64(int g_a, int64_t lo[], int64_t hi[], int64_t skip[], void* buf, int64_t  ld[]) 
    DoubleComplex NGA_Zdot_patch64(int g_a, char t_a, int64_t alo[], int64_t ahi[], int g_b, char t_b, int64_t blo[], int64_t bhi[])
    void          NGA_Zero_patch64(int g_a, int64_t lo[], int64_t hi[])
