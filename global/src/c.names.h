#ifndef  C_NAMES_H
#define  C_NAMES_H

#define  GA_acc                    ga_acc_
#define  GA_access                 ga_access_
#define  GA_brdcst                 ga_brdcst_
#define  GA_check_handle           ga_check_handle_
#define  GA_compare_distr          ga_compare_distr_
#define  GA_copy                   ga_copy_
#define  GA_copy_patch             ga_copy_patch
#define  GA_copy_patch_dp          ga_copy_patch_dp
#define  GA_create                 ga_create
#define  GA_create_irreg           ga_create_irreg
#define  GA_add                    ga_add_
#define  GA_add_patch              ga_add_patch_
#define  GA_ddot                   ga_ddot_
#define  GA_ddot_patch             ga_ddot_patch
#define  GA_zdot                   ga_zdot_
#define  GA_idot                   ga_idot_
#define  GA_fdot                   ga_sdot_
#define  GA_fdot_patch             ga_sdot_patch
#define  GA_zdot_patch             ga_zdot_patch
#define  GA_ddot_patch_dp          ga_ddot_patch_dp
#define  GA_destroy                ga_destroy_
#define  GA_dgop                   ga_dgop_
#define  GA_fgop                   ga_fgop
#define  GA_dgemm                  ga_dgemm_
#define  GA_diag                   ga_diag_
#define  GA_diag_reuse             ga_diag_reuse_
#define  GA_diag_std               ga_diag_std_
#define  GA_llt_solve              ga_llt_solve_
#define  GA_lu_solve               ga_lu_solve_alt_
#define  GA_solve                  ga_solve_
#define  GA_spd_invert             ga_spd_invert_
#define  GA_distribution           ga_distribution_
#define  GA_scale                  ga_scale_
#define  GA_scale_patch            ga_scale_patch_
#define  GA_duplicate              ga_duplicate
#define  GA_error                  ga_error
#define  GA_fence                  ga_fence_
#define  GA_gather                 ga_gather_
#define  GA_get                    ga_get_
#define  GA_fill_patch             ga_fill_patch_
#define  GA_igop                   ga_igop_
#define  GA_initialize             ga_initialize_
#define  GA_init_fence             ga_init_fence_
#define  GA_initialize_ltd         ga_initialize_ltd_
#define  GA_inquire                ga_inquire_
#define  GA_inquire_memory         ga_inquire_memory_
#define  GA_inquire_name           ga_inquire_name
#define  GA_list_nodeid            ga_list_nodeid_
#define  GA_locate                 ga_locate_
#define  GA_locate_region          ga_locate_region_
#define  GA_matmul_patch           ga_matmul_patch
#define  GA_mpi_communicator       ga_mpi_communicator
#define  GA_nnodes                 ga_nnodes_
#define  GA_nodeid                 ga_nodeid_
#define  GA_print                  ga_print_
#define  GA_print_patch            ga_print_patch_
#define  GA_print_stats            ga_print_stats_
#define  GA_proc_topology          ga_proc_topology_
#define  GA_put                    ga_put_
#define  GA_read_inc               ga_read_inc_
#define  GA_release                ga_release_
#define  GA_release_update         ga_release_update_
#define  GA_scatter                ga_scatter_
#define  GA_scatter_acc            ga_scatter_acc_
#define  GA_sort_scat_dbl          ga_sort_scat_dbl_
#define  GA_sort_scat_int          ga_sort_scat_int_
#define  GA_sync                   ga_sync_
#define  GA_summarize              ga_summarize_
#define  GA_symmetrize             ga_symmetrize_
#define  GA_terminate              ga_terminate_
#define  GA_transpose              ga_transpose_
#define  GA_uses_ma                ga_uses_ma_
#define  GA_verify_handle          ga_verify_handle_
#define  GA_zero                   ga_zero_
#define  GA_ndim                   ga_ndim_
#define  GA_print_distribution     ga_print_distribution_
#define  GA_create_mutexes         ga_create_mutexes_
#define  GA_destroy_mutexes        ga_destroy_mutexes_
#define  GA_lock                   ga_lock_
#define  GA_unlock                 ga_unlock_
#define  GA_valid_handle           ga_valid_handle_
#define  GA_set_memory_limit       ga_set_memory_limit_

#if defined(CRAY) || defined(_WIN32)
#define  ga_acc_                   GA_ACC
#define  ga_access_                GA_ACCESS
#define  ga_brdcst_                GA_BRDCST
#define  ga_check_handle_          GA_CHECK_HANDLE
#define  ga_compare_distr_         GA_COMPARE_DISTR
#define  ga_copy_                  GA_COPY
#define  ga_copy_patch_            GA_COPY_PATCH
#define  ga_create_                GA_CREATE
#define  ga_create_irreg_          GA_CREATE_IRREG
#define  ga_add_                   GA_ADD
#define  ga_add_patch_             GA_ADD_PATCH
#define  ga_ddot_                  GA_DDOT
#define  ga_zdot_                  GA_ZDOT
#define  ga_idot_                  GA_IDOT
#define  ga_sdot_                  GA_SDOT
#define  ga_destroy_               GA_DESTROY
#define  ga_fence_                 GA_FENCE
#define  ga_gather_                GA_GATHER
#define  ga_dgop_                  GA_DGOP
#define  ga_sgop_                  GA_SGOP
#define  ga_distribution_          GA_DISTRIBUTION
#define  ga_duplicate_             GA_DUPLICATE
#define  ga_error_                 GA_ERROR
#define  ga_get_                   GA_GET
#define  ga_fill_patch_            GA_FILL_PATCH
#define  ga_igop_                  GA_IGOP
#define  ga_initialize_            GA_INITIALIZE
#define  ga_init_fence_            GA_INIT_FENCE
#define  ga_initialize_ltd_        GA_INITIALIZE_LTD
#define  ga_inquire_               GA_INQUIRE
#define  ga_inquire_name_          GA_INQUIRE_NAME
#define  ga_inquire_memory_        GA_INQUIRE_MEMORY
#define  ga_list_nodeid_           GA_LIST_NODEID
#define  ga_locate_                GA_LOCATE
#define  ga_locate_region_         GA_LOCATE_REGION
#define  ga_matmul_patch_          GA_MATMUL_PATCH
#define  ga_memory_limited_        GA_MEMORY_LIMITED
#define  ga_memory_avail_          GA_MEMORY_AVAIL
#define  ga_nnodes_                GA_NNODES
#define  ga_nodeid_                GA_NODEID
#define  ga_net_nnodes_            GA_NET_NNODES
#define  ga_net_nodeid_            GA_NET_NODEID
#define  ga_print_                 GA_PRINT
#define  ga_print_patch_           GA_PRINT_PATCH
#define  ga_print_stats_           GA_PRINT_STATS
#define  ga_put_                   GA_PUT
#define  ga_read_inc_              GA_READ_INC
#define  ga_reinit_handler_        GA_REINIT_HANDLER
#define  ga_release_               GA_RELEASE
#define  ga_release_update_        GA_RELEASE_UPDATE
#define  ga_server_                GA_SERVER
#define  ga_sort_scat2_            GA_SORT_SCAT2
#define  ga_sync_                  GA_SYNC
#define  ga_scale_                 GA_SCALE
#define  ga_scale_patch_           GA_SCALE_PATCH
#define  ga_scatter_               GA_SCATTER
#define  ga_scatter_acc_           GA_SCATTER_ACC
#define  ga_terminate_             GA_TERMINATE
#define  ga_uses_ma_               GA_USES_MA
#define  ga_zero_                  GA_ZERO
#define  ga_verify_handle_         GA_VERIFY_HANDLE
#define  ga_copy_patch_dp_         GA_COPY_PATCH_DP
#define  ga_ddot_patch_dp_         GA_DDOT_PATCH_DP
#define  ga_proc_topology_         GA_PROC_TOPOLOGY
#define  ga_symmetrize_            GA_SYMMETRIZE
#define  ga_summarize_             GA_SUMMARIZE
#define  ga_transpose_             GA_TRANSPOSE
#define  ga_create_mutexes_        GA_CREATE_MUTEXES
#define  ga_destroy_mutexes_       GA_DESTROY_MUTEXES
#define  ga_lock_                  GA_LOCK
#define  ga_unlock_                GA_UNLOCK
#define  ga_fill_                  GA_FILL
#define  ga_valid_handle_          GA_VALID_HANDLE
#define  ga_set_memory_limit_      GA_SET_MEMORY_LIMIT
#define  ga_ndim_                  GA_NDIM
#define  ga_print_distribution_    GA_PRINT_DISTRIBUTION

#define  nga_create_               NGA_CREATE
#define  nga_create_irreg_         NGA_CREATE_IRREG
#define  nga_acc_                  NGA_ACC
#define  nga_put_                  NGA_PUT
#define  nga_get_                  NGA_GET
#define  nga_read_inc_             NGA_READ_INC
#define  nga_locate_               NGA_LOCATE
#define  nga_locate_region_        NGA_LOCATE_REGION
#define  nga_distribution_         NGA_DISTRIBUTION
#define  nga_access_               NGA_ACCESS
#define  nga_scatter_              NGA_SCATTER
#define  nga_gather_               NGA_GATHER
#define  nga_scatter_acc_          NGA_SCATTER_ACC
#define  nga_release_              NGA_RELEASE
#define  nga_release_update_       NGA_RELEASE_UPDATE

#define  nga_periodic_acc_         NGA_PERIODIC_ACC
#define  nga_periodic_put_         NGA_PERIODIC_PUT
#define  nga_periodic_get_         NGA_PERIODIC_GET

#define  nga_copy_patch_           NGA_COPY_PATCH
#define  ngai_dot_patch_           NGAI_DOT_PATCH
#define  nga_idot_patch_           NGA_IDOT_PATCH
#define  nga_sdot_patch_           NGA_SDOT_PATCH   
#define  nga_ddot_patch_           NGA_DDOT_PATCH
#define  nga_zdot_patch_           NGA_ZDOT_PATCH
#define  nga_fill_patch_           NGA_FILL_PATCH
#define  nga_scale_patch_          NGA_SCALE_PATCH
#define  nga_add_patch_            NGA_ADD_PATCH
#define  nga_print_patch_          NGA_PRINT_PATCH

#define  nga_select_elem_          NGA_SELECT_ELEM
#define  ga_patch_enum_            GA_PATCH_ENUM

#define  gai_dot_patch_            GAI_DOT_PATCH
#define  gai_dot_                  GAI_DOT
#define  ga_ma_base_address_       GA_MA_BASE_ADDRESS
#define  ga_ma_diff_               GA_MA_DIFF
#define  ga_ma_get_ptr_            GA_MA_GET_PTR

#define  ga_nblock_                GA_NBLOCK
#define  ga_diag_                  GA_DIAG
#define  ga_diag_seq_	      	   GA_DIAG_SEQ
#define  ga_diag_reuse_            GA_DIAG_REUSE
#define  ga_diag_std_	           GA_DIAG_STD
#define  ga_diag_std_seq_	   GA_DIAG_STD_SEQ
#define  ga_llt_solve_             GA_LLT_SOLVE
#define  ga_lu_solve_alt_	   GA_LU_SOLVE_ALT
#define  ga_solve_		   GA_SOLVE
#define  ga_spd_invert_		   GA_SPD_INVERT

#endif

#endif
