// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */







#ifndef AW_ALPHACORE_H
#define AW_ALPHACORE_H

#include <stdint.h>
#include <stdbool.h>

#include "aw_driver_sim.h"
#include "aw_alphacore_csr_defines.h"
#include "aw_alphacore_vfield_defines.h"
#include "aw_alphacore_ip_defines.h"
#include "aw_alphacore_atest_defines.h"

#define CHECK(x) do { int err = (x); if (err) return err; } while (0)
#define AW_MAX(a,b) (((a)>(b))?(a):(b))
#define AW_MIN(a,b) (((a)<(b))?(a):(b))
#define PI 3.14159265
#define AW_ADV_ABILITIES   20
#define AW_FEC_ABILITIES   5









#ifndef __aw_err_code_enum
#define __aw_err_code_enum


typedef enum aw_err_code_e {
    AW_ERR_CODE_NONE = 0,              
    AW_ERR_CODE_POLL_TIMEOUT = 1,      
    AW_ERR_CODE_INVALID_ARG_VALUE = 2, 
    AW_ERR_CODE_WRITE_FAILURE = 3,     
    AW_ERR_CODE_READ_FAILURE = 4,      
    AW_ERR_CODE_BAD_STATE = 5,         
    AW_ERR_CODE_FUNC_FAILURE = 6,      
    AW_ERR_CODE_CHECK_FAILURE = 7      
} aw_err_code_t;
#endif

typedef enum vref_vals_e {
    vref = 700,
    vref2 = 350
}  vref_vals_t;

#define NULL_CHECK(x) do { if(x == NULL) return AW_ERR_CODE_INVALID_ARG_VALUE; } while (0)



typedef struct aw_dwell_params_s { 
    uint32_t aw_measure_time;  
    uint64_t rx_data_rate;     
} aw_dwell_params_t;


typedef struct digref_struct {
    

    double lsref_freq_mhz;
    

    int digref_data_sel;
    int digref_clock_sel;
    

    double tx_rate;
    double rx_rate;
    

    uint32_t tx_width;
    uint32_t rx_width;
    

    int num_adc;
   

    int pam_en;
   

    double bwidth_hz;

    

    double rx_vco_freq_ghz;
    double tx_vco_freq_ghz;

    

    uint32_t scaler_en;


    

    uint32_t oversampling_mode;

} aw_digref_t;



typedef struct aw_an_newdef_s {
    uint32_t lane_num_u;
    uint32_t lt_sel_u;
    uint32_t pam_u;
    uint32_t timer_sel_u;
} aw_an_newdef_t;


typedef struct tx_hbridge_s {
    uint32_t msb; 
    uint32_t lsb; 
    uint32_t bias_adj; 
    uint32_t rlm_ovr; 
    uint32_t bias_adj_en; 
    uint32_t rlm_ovr_en; 
} tx_hbridge_t;



typedef struct aw_an_spec_s {
    uint32_t an_def_spec_width[28];    
    uint32_t an_def_spec_rate[28];     

    uint32_t an_def_an_rate;        

    aw_an_newdef_t newdef1;         
    aw_an_newdef_t newdef2;         
    aw_an_newdef_t newdef3;         
    aw_an_newdef_t newdef4;         

} aw_an_spec_t;

typedef struct aw_lt_status_s {
    uint32_t lt_running; 
    uint32_t lt_done; 
    uint32_t lt_failure; 
    uint32_t lt_rx_ready; 
} aw_lt_status_t;

typedef enum aw_refclk_term_mode_e {
    AW_RC_HI_Z = 0, 
    AW_RC_R50_SE = 1, 
    AW_RC_R100_DF = 2, 
    
} aw_refclk_term_mode_t;

typedef enum aw_acc_term_mode_e {
    AW_ACC_HI_Z = 0,
    AW_ACC_TERM_VSS_AC = 1, 
    AW_ACC_TERM_FL_AC = 2, 
    
    
    AW_ACC_TERM_VSS_DC = 5, 
    AW_ACC_TERM_FL_DC = 6, 
    
} aw_acc_term_mode_t;

typedef enum aw_force_sigdet_mode_e {
    

    AW_SIGDET_FORCE0 = 0,

    

    AW_SIGDET_FORCE1 = 1,

    

    AW_SIGDET_NORM = 2,
} aw_force_sigdet_mode_t;


typedef enum aw_txfir_cfg_taps_e {
    AW_CM3 = 0, 
    AW_CM2 = 1, 
    AW_CM1 = 2, 
    AW_C0  = 3, 
    AW_C1  = 4, 
    AW_TXFIR_MAX_TAPS = 5
} aw_txfir_cfg_taps_t;



typedef struct aw_txfir_config_s {
    uint32_t CM3; 
    uint32_t CM2; 
    uint32_t CM1; 
    uint32_t C0;  
    uint32_t C1;  
    uint32_t C2;
    uint32_t C3;
    uint32_t main_or_max; 
} aw_txfir_config_t;

typedef struct aw_analog_loopback_txfir_config_s {
    uint32_t nes_post1; 
    uint32_t nes_c0;    
} aw_analog_loopback_txfir_config_t;



typedef enum aw_state_rate_e {
  AW_NRZ_1p25_2p5 = 0, 
  AW_NRZ_10p3125 = 1, 
  AW_NRZ_25p78125 = 2, 
  AW_NRZ_26p5625 = 3, 
  AW_PAM4_53p125 = 4, 
  AW_NRZ_53p125 = 5, 
  AW_PAM4_106p25 = 6, 
  AW_MAX_RATES = 7,
} aw_state_rate_t;



typedef enum aw_training_mode_e {
  AW_DISABLED = 0,
  AW_CL72 = 1,
  AW_CL92 = 2,
  AW_CL136 = 3,
  AW_CL162 = 4,
  AW_MAX_CFG = 5,
} aw_training_mode_t;









typedef enum aw_pll_pstatus_e {
    AW_PWR_DOWN = 0, 
    AW_PWR_UP = 1, 
    AW_ST_CHNG = 2 
} aw_pll_pstatus_t;



typedef uint32_t aw_ffe_t[AW_FFE_NUM_TAPS];



typedef int aw_ffe_signed_t[AW_FFE_NUM_TAPS];

typedef struct aw_thresholds_s {
    

    uint32_t eh;
    

    uint32_t ez;
    

    uint32_t el;
    

    uint32_t thres_low;
    

    uint32_t thres_hi;
} aw_thresholds_t;



typedef struct aw_slicers_s {
    

    uint32_t eh3;
    

    uint32_t eh1;
    

    uint32_t el1;
    

    uint32_t el3;
} aw_slicers_t;


typedef struct aw_dsp_param_s {
    
    uint32_t dc_offset;
    
    uint32_t dfe;
    aw_ffe_t ffe;
    aw_ffe_signed_t s_ffe;
    aw_ffe_t pre_ffe;
    aw_ffe_signed_t s_pre_ffe;
    aw_ffe_t pre_dfe_ffe;
    aw_ffe_signed_t s_pre_dfe_ffe;
    aw_ffe_t pre_ffe_ctle0;
    aw_ffe_t pre_ffe_ctle1;
    aw_ffe_t pre_ffe_ctle2;
    aw_ffe_t pre_ffe_ctle3;
    aw_ffe_t pre_ffe_ctle4;
    aw_ffe_t pre_ffe_ctle5;
    aw_thresholds_t thresholds;
    aw_slicers_t slicers;
} aw_dsp_param_t;


typedef struct aw_dcdiq_data_s {
    

    uint32_t d0;
    

    uint32_t d90;
    

    uint32_t iq;
} aw_dcdiq_data_t;

typedef struct vga_opt_s {
    

    uint32_t en;
    

    uint32_t vga_cap;
    

    uint32_t use_custom_takeover_ratio;
    

    uint32_t custom_takeover_ratio;
    

    uint32_t custom_nyq_mask;

} vga_opt_t;


typedef struct aw_afe_data_s {
    

    uint32_t ctle_rate;
    

    uint32_t ctle_boost;
    

    uint32_t vga_coarse;
    

    uint32_t vga_fine;
    

    uint32_t vga_offset;
} aw_afe_data_t;



typedef struct aw_uc_diag_regs_s {
    uint32_t rxmfsm_rate_cur;
    uint32_t rxmfsm_rate_new;
    uint32_t rxmfsm_width_cur;
    uint32_t rxmfsm_width_new;
    uint32_t rxmfsm_rxdisable;
    uint32_t rxmfsm_req;
    uint32_t rxmfsm_power_cur;
    uint32_t rxmfsm_power_new;
    uint32_t rxmfsm_pam_cur;
    uint32_t rxmfsm_pam_new;
    uint32_t rxmfsm_pam_ctrl_cur;
    uint32_t rxmfsm_pam_ctrl_new;
    uint32_t rxmfsm_instr_num;
    uint32_t rxmfsm_state;
    uint32_t rxiffsm_state;
    uint32_t rx_log0;
    uint32_t rx_log1;
    uint32_t rx_log2;
    uint32_t rx_log3;
    uint32_t rx_log4;
    uint32_t rx_log5;
    uint32_t rx_log6;
    uint32_t rx_log7;
    uint32_t rx_log8;
    uint32_t txmfsm_rate_cur;
    uint32_t txmfsm_rate_new;
    uint32_t txmfsm_width_cur;
    uint32_t txmfsm_width_new;
    uint32_t txmfsm_power_cur;
    uint32_t txmfsm_power_new;
    uint32_t txmfsm_pam_cur;
    uint32_t txmfsm_pam_new;
    uint32_t txmfsm_instr_num;
    uint32_t txmfsm_req;
    uint32_t txmfsm_state;
    uint32_t txiffsm_state;
    uint32_t tx_log0;
    uint32_t tx_log1;
    uint32_t tx_log2;
    uint32_t tx_log3;
    uint32_t tx_log4;
    uint32_t tx_log5;
    uint32_t tx_log6;
    uint32_t tx_log7;
    uint32_t tx_log8;
    uint32_t tx_log9;
    uint32_t cmnmfsm_rate_cur;
    uint32_t cmnmfsm_instr_num;
    uint32_t cmnmfsm_power_cur;
    uint32_t cmnmfsm_power_new;
    uint32_t cmnmfsm_rate_new;
    uint32_t cmnmfsm_req;
    uint32_t cmnmfsm_state;
    uint32_t cmniffsm_state;
    uint32_t cmn_log0;
    uint32_t cmn_log1;
    uint32_t cmn_log2;
    uint32_t cmn_log3;
    uint32_t cmn_log4;
    uint32_t cmn_log5;
    uint32_t cmn_log6;
    uint32_t cmn_log7;
    uint32_t cmn_log8;
    uint32_t cmn_log9;
} aw_uc_diag_regs_t;

typedef struct aw_ucode_s {
    uint32_t address;
    uint32_t value;
} aw_ucode_t;

typedef enum aw_bist_pattern_e {
    AW_PRBS7 = 0,
    AW_PRBS9 = 1,
    AW_PRBS11 = 2,
    AW_PRBS13 = 3,
    AW_PRBS15 = 4,
    AW_PRBS23 = 5,
    AW_PRBS31 = 6,
    AW_QPRBS13 = 7,
    AW_JP03A = 8,
    AW_JP03B = 9,
    AW_LINEARITY_PATTERN = 10,
    AW_USER_DEFINED_PATTERN = 11,
    AW_FULL_RATE_CLOCK = 12,
    AW_HALF_RATE_CLOCK = 13,
    AW_QUARTER_RATE_CLOCK = 14,
    AW_PATT_32_1S_32_0S = 15, 
    AW_BIST_PATTERN_MAX = 16 
} aw_bist_pattern_t;

typedef enum aw_bist_mode_e {
    AW_TIMER = 0, 
    AW_DWELL = 1, 
} aw_bist_mode_t;

typedef enum aw_eq_type_e {
    AW_EQ_FULL_DIR = 0, 
    AW_EQ_EVAL_DIR = 1, 
    AW_EQ_INIT_EVAL = 2, 
    AW_EQ_CLEAR_EVAL = 3, 
    AW_EQ_FULL_FOM = 4, 
    AW_EQ_EVAL_FOM = 5 
} aw_eq_type_t;


typedef enum aw_pstate_e {
    AW_P0   = 0,
    AW_P0S  = 1,
    AW_P1   = 2,
    AW_P2   = 3,
    AW_PD   = 4,
    AW_L1_0 = 5,
    AW_L1_1 = 6,
    AW_L1_2 = 7
} aw_pstate_t;

typedef enum aw_cmn_pstate_e {
    AW_CMN_PD   = 0,
    AW_CMN_P0   = 1
} aw_cmn_pstate_t;

typedef enum aw_rx_roaming_mode_e {
    AW_NO_ROAMING = 0,  
    AW_BANK1_ROAMING = 1, 
    AW_BANK2_ROAMING = 2, 
    AW_BANK1_BANK2_ROAMING = 3, 
    AW_RESET_DEFAULT_ROAMING_MODE = 4, 
    AW_MANUAL_WINDOW_SELECT = 5 
} aw_rx_roaming_mode_t;

typedef enum aw_rx_ffe_tap_count_e {
    AW_FFE_11_TAPS = 0,
    AW_FFE_15_TAPS = 1,
    AW_FFE_18_TAPS = 2,
    AW_FFE_ALL_TAPS_ENABLED = 3
} aw_rx_ffe_tap_count_t;


















uint32_t aw_width_decoder (uint32_t width_encoded);






int aw_pmd_anlt_logical_lane_num_set (mss_access_t *mss, uint32_t logical_lane, uint32_t an_no_attached);




int aw_pmd_anlt_auto_neg_link_status_ovr_enable(mss_access_t *mss, uint32_t en);



int aw_pmd_anlt_auto_neg_adv_ability_set (mss_access_t *mss, uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce);



int aw_pmd_anlt_lp_auto_neg_adv_ability_get (mss_access_t *mss, uint32_t * adv_ability, uint32_t *fec_ability);




int aw_pmd_anlt_ms_per_ck_set(mss_access_t *mss, uint32_t ms_per_ck);




int aw_pmd_anlt_auto_neg_config_set (mss_access_t *mss, uint32_t status_check_disable, uint32_t next_page_en, uint32_t an_no_nonce_check);




int aw_pmd_anlt_auto_neg_start_set (mss_access_t *mss, uint32_t start);




int aw_pmd_anlt_auto_neg_status_get (mss_access_t *mss, uint32_t * link_good);




int aw_pmd_anlt_auto_neg_status_complete_get (mss_access_t *mss, uint32_t * an_complete);



int aw_pmd_anlt_auto_neg_result_get (mss_access_t *mss, uint32_t no_consortium, uint32_t * an_result);




int aw_pmd_anlt_auto_neg_next_page_set(mss_access_t *mss, uint64_t an_tx_np);






int aw_pmd_anlt_auto_neg_next_page_oui_compare_set(mss_access_t *mss, uint32_t np_expected_oui);



int aw_pmd_anlt_link_training_en_set (mss_access_t *mss, uint32_t en);




int aw_pmd_link_training_without_an_config_set (mss_access_t *mss, uint32_t width, uint32_t clause);



int aw_pmd_anlt_link_training_preset_check_set (mss_access_t *mss, uint32_t clause, uint32_t preset_check);



int aw_pmd_anlt_link_training_reset (mss_access_t *mss);



int aw_pmd_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause, uint32_t mod);





int aw_pmd_anlt_link_training_prbs_seed_set (mss_access_t *mss, uint32_t clause, uint32_t logical_lane);



int aw_pmd_anlt_link_training_init_preset_set (mss_access_t *mss, uint32_t clause, uint32_t init[], uint32_t preset[], uint32_t preset2[]);




int aw_pmd_anlt_link_training_min_max_set (mss_access_t *mss, uint32_t pre1_max, uint32_t main_max, uint32_t post1_max, uint32_t pre1_min, uint32_t main_min, uint32_t post1_min);




int aw_pmd_anlt_link_training_start_set (mss_access_t *mss, uint32_t start);




int aw_pmd_anlt_link_training_status_get (mss_access_t *mss, uint32_t * lt_running, uint32_t * lt_done, uint32_t * lt_training_failure, uint32_t * lt_rx_ready);



int aw_pmd_anlt_link_training_timeout_enable_set (mss_access_t *mss, uint32_t enable);










int aw_pmd_refclk_termination_set(mss_access_t *mss, aw_refclk_term_mode_t lsrefbuf_term_mode);





int aw_pmd_rx_termination_set(mss_access_t *mss, aw_acc_term_mode_t acc_term_mode);




int aw_pmd_force_signal_detect_config_set(mss_access_t *mss, aw_force_sigdet_mode_t sigdet_mode);
















int aw_pmd_txfir_config_set(mss_access_t *mss, aw_txfir_config_t *txfir_cfg, uint32_t fir_ovr_enable);






int aw_pmd_txfir_config_get(mss_access_t *mss, aw_txfir_config_t *txfir_cfg);




int aw_pmd_tx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en);




int aw_pmd_tx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en);




int aw_pmd_rx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en);





int aw_pmd_rx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en);




int aw_pmd_remote_loopback_set(mss_access_t *mss, uint32_t remote_loopback_enable);




int aw_pmd_tx_dcd_iq_cal(mss_access_t *mss, uint32_t enable_d);



int aw_pmd_fep_data_set(mss_access_t *mss, uint32_t datapath_en);



int aw_pmd_analog_loopback_set(mss_access_t *mss, uint32_t analog_loopback_enable);




int aw_pmd_fep_clock_set(mss_access_t *mss, uint32_t clock_en);











int aw_pmd_tx_polarity_set(mss_access_t *mss, uint32_t tx_pol_flip);




int aw_pmd_rx_polarity_set(mss_access_t *mss, uint32_t rx_pol_flip);







int aw_pmd_rx_dfe_adapt_set(mss_access_t *mss, uint32_t dfe_adapt_enable);










int aw_pmd_rx_background_adapt_enable_set(mss_access_t *mss, uint32_t rx_background_adapt);



int aw_pmd_rx_background_adapt_enable_get(mss_access_t *mss, uint32_t *rx_bkgrnd_adapt_enable);









int aw_pmd_rxeq_prbs_set(mss_access_t *mss, uint32_t prbs_en);



















int aw_pmd_rx_chk_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, aw_bist_mode_t mode, uint64_t udp_63_0, uint64_t udp_127_64, uint32_t lock_thresh, uint32_t timer_thresh);




int aw_pmd_rx_chk_en_set(mss_access_t *mss, uint32_t enable);




int aw_pmd_rx_chk_lock_state_get(mss_access_t *mss, uint32_t *rx_bist_lock);



int aw_pmd_rx_chk_err_count_state_get(mss_access_t *mss, uint64_t *err_count, uint32_t *err_count_done, uint32_t *err_count_overflown);



int aw_pmd_rx_chk_err_count_state_clear(mss_access_t *mss);



int aw_pmd_tx_gen_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, uint64_t udp_63_0, uint64_t udp_127_64);




int aw_pmd_tx_gen_en_set(mss_access_t *mss, uint32_t enable);




int aw_pmd_tx_gen_err_inject_config_set(mss_access_t *mss, uint64_t err_pattern, uint32_t err_rate);




int aw_pmd_tx_gen_err_inject_en_set(mss_access_t *mss, uint32_t enable);













int aw_pmd_pll_lock_min_set(mss_access_t *mss, uint32_t val);




int aw_pmd_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock ,uint32_t check_en, uint32_t expected_val);







int aw_pmd_iso_cmn_pstate_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_cmn_pstate_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_cmn_state_req_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_cmn_state_ack_get(mss_access_t *mss, uint32_t *cmn_state_ack);



int aw_pmd_iso_tx_reset_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_tx_reset_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_rx_reset_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_rx_reset_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_tx_rate_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_tx_rate_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_rx_rate_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_rx_rate_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_tx_pstate_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_tx_pstate_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_rx_pstate_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_rx_pstate_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_tx_width_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_tx_width_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_rx_width_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_rx_width_get(mss_access_t *mss, uint32_t *value);



int aw_pmd_iso_tx_state_req_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_rx_state_req_set(mss_access_t *mss, uint32_t value);



int aw_pmd_iso_tx_state_ack_get(mss_access_t *mss, uint32_t *tx_state_ack);



int aw_pmd_iso_rx_state_ack_get(mss_access_t *mss, uint32_t *rx_state_ack);



int aw_pmd_isolate_cmn_set(mss_access_t *mss, uint32_t en);



int aw_pmd_isolate_cmn_get(mss_access_t *mss, uint32_t *en);
























int aw_pmd_isolate_lane_set(mss_access_t *mss, uint32_t en);



int aw_pmd_isolate_lane_tx_set(mss_access_t *mss, uint32_t en);



int aw_pmd_isolate_lane_rx_set(mss_access_t *mss, uint32_t en);



int aw_pmd_isolate_lane_txrx_set(mss_access_t *mss, uint32_t en);



int aw_pmd_isolate_lane_get(mss_access_t *mss, uint32_t *en);




int aw_pmd_cmn_r2l0_lsref_sel_set(mss_access_t *mss, uint32_t sel);



int aw_pmd_cmn_r2l1_lsref_sel_set(mss_access_t *mss, uint32_t sel);




int aw_pmd_cmn_l2r0_lsref_sel_set(mss_access_t *mss, uint32_t sel);



int aw_pmd_cmn_l2r1_lsref_sel_set(mss_access_t *mss, uint32_t sel);




int aw_pmd_cmn_r2l0_lsref_sel_get(mss_access_t *mss, uint32_t *sel);



int aw_pmd_cmn_r2l1_lsref_sel_get(mss_access_t *mss, uint32_t *sel);




int aw_pmd_cmn_l2r0_lsref_sel_get(mss_access_t *mss, uint32_t *sel);



int aw_pmd_cmn_l2r1_lsref_sel_get(mss_access_t *mss, uint32_t *sel);



int aw_pmd_cmn_lsref_sel_set(mss_access_t *mss, uint32_t ref_sel);






int aw_pmd_gen_tx_en_set(mss_access_t *mss, uint32_t value);





int aw_pmd_iso_request_cmn_state_change(mss_access_t *mss, aw_cmn_pstate_t cmn_pstate, uint32_t timeout_us);



int aw_pmd_iso_request_tx_state_change(mss_access_t *mss, aw_pstate_t tx_pstate, uint32_t tx_rate, uint32_t tx_width, uint32_t timeout_us);



int aw_pmd_iso_request_rx_state_change(mss_access_t *mss, aw_pstate_t rx_pstate, uint32_t rx_rate, uint32_t rx_width, uint32_t timeout_us);



int aw_pmd_rx_check_cdr_lock(mss_access_t *mss, uint32_t timeout_us);



int aw_pmd_rx_check_bist(mss_access_t *mss, aw_bist_mode_t bist_mode, uint32_t timer_threshold, uint32_t rx_width, uint32_t timeout_us, int32_t expected_errors, aw_dwell_params_t *dwell_params);











int aw_pmd_eqeval_type_set(mss_access_t *mss, uint32_t eq_type);



int aw_pmd_eqeval_req_set(mss_access_t *mss, uint32_t value);



int aw_pmd_eqeval_ack_get(mss_access_t *mss, uint32_t *eqeval_ack );



int aw_pmd_eqeval_incdec_get(mss_access_t *mss, uint32_t * incdec);



int aw_pmd_rx_equalize(mss_access_t *mss, aw_eq_type_t eq_type, uint32_t timeout_us );






















typedef enum {
    AW_TB_DCOFFSET = 0,
    AW_TB_TBUS     = 1,
    AW_TB_RAWADC   = 2,
} aw_pmd_tracebuffer_mode_t;





















int aw_pmd_nep_loopback_set(mss_access_t *mss, uint32_t nep_loopback_enable);



















int aw_pmd_rd_data_pipeline_stages_set(mss_access_t *mss, uint32_t stages);


























int aw_pmd_rx_cdr_lock_get(mss_access_t *mss, uint32_t *rx_cdr_lock);









int aw_pmd_enable_pam4_mode(mss_access_t *mss, int enable);




















int aw_pmd_snr_vld_hys_thresh_set_from_target_snr(mss_access_t *mss, uint32_t target_snr_low, uint32_t target_snr_high);



int aw_pmd_snr_vld_hys_thresh_set_recommended(mss_access_t *mss, uint32_t nrz_mode);



int aw_pmd_snr_mon_enable_set(mss_access_t *mss, uint32_t nrz_mode, uint32_t mon_enable);



int aw_pmd_snr_vld_enable_set(mss_access_t *mss, uint32_t vld_enable);






int aw_tc_sm_conv(uint32_t v, uint32_t i);





#endif 
