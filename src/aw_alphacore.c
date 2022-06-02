// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */
 

 

#include <stdint.h>
#include <math.h>
#include <string.h>

#include "aw_alphacore.h"
#include "aw_pmd_rx_dsp_get.h"
#include "aw_pmd_vfields.h"
#include "stdlib.h"

#if defined(C_API_COVERAGE_ENABLE)
#include "stdio.h"
#include "execinfo.h"

void __cyg_profile_func_enter(void *func, void *caller)
                              __attribute__((no_instrument_function));

void __cyg_profile_func_enter (void *func, void *caller) {
     
    (void)caller;
    (void)func;
    FILE *fptr;
    fptr = fopen("backtrace_dump.log", "a");
    fprintf(fptr, "\n===================API BTRACE START===================\n");
    int nptrs;
    void *buffer[1024];
    char **bt_syms;
    nptrs = backtrace(buffer, 1024);
    bt_syms = backtrace_symbols(buffer, nptrs);
    fprintf(fptr, "%s\n", bt_syms[1]);
    free(bt_syms);
    fprintf(fptr, "\n===================API BTRACE END====================\n");
    fclose(fptr);
}
#endif

const char aw_library_version[] = "1.0.7";

uint32_t aw_width_decoder (uint32_t width_encoded) {
    uint32_t width = 128;
    if (width_encoded == 7){
        width = 128 ;
    } else if (width_encoded == 6){
        width = 64;
    } else if (width_encoded == 5){
        width = 40;
    } else if (width_encoded == 4){
        width = 32;
    } else if (width_encoded == 3){
        width = 20;
    } else if (width_encoded == 2){
        width = 16;
    } else if (width_encoded == 1){
        width = 10;
    } else if (width_encoded == 0){
        width = 64;
    } else {
        USR_PRINTF("ERROR: Invalid width encoding\n");
        return 0;
    }
    return width;
}

int aw_pmd_anlt_logical_lane_num_set (mss_access_t *mss, uint32_t logical_lane, uint32_t an_no_attached) {
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_ANLT_LANE_NUM_MASK, ETH_ANLT_CTRL_ANLT_LANE_NUM_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_ATTACHED_MASK, ETH_ANLT_CTRL_AN_NO_ATTACHED_OFFSET, an_no_attached));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_link_status_ovr_enable(mss_access_t *mss, uint32_t en) {

    CHECK(pmd_write_field(mss, AN_LINK_CTRL_ADDR, AN_LINK_CTRL_STAT_OVR_EN_NT_MASK, AN_LINK_CTRL_STAT_OVR_EN_NT_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_ICTL_ETH_AN_LINK_STATUS_A_MASK, DIG_SOC_LANE_OVRD_REG4_ICTL_ETH_AN_LINK_STATUS_A_OFFSET, 8388607));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_TXCHNG_PEDGE_A_MASK, DIG_SOC_LANE_OVRD_REG2_TXCHNG_PEDGE_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_adv_ability_set (mss_access_t *mss, uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce){
    uint32_t ability_1_temp = 0;
    uint32_t ability_2_temp = 0;

     
    for (int i = 0; i<=18; i++) {
        if (i <= 10 && adv_ability[i] == 1) {
             
            ability_1_temp = ability_1_temp | (1 << (i + 5));
             
        } else if (i > 10 && i <= 18 && adv_ability[i] == 1) {
             
            ability_2_temp = ability_2_temp | (1 << (i - 11));
             
        }
    }
     
    for (int i = 0; i<=4; i++) {
        if (fec_ability[i] == 1) {
             
            ability_2_temp = ability_2_temp | (1 << (i + 11));
             
        }
    }

    ability_1_temp |= (nonce & 0x1F);


    CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG2_ADDR, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_MASK, ETH_AN_ADV_ABILITY_REG2_AN_MR_ADV_ABILITY_1_OFFSET, ability_1_temp));
    CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG3_ADDR, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_MASK, ETH_AN_ADV_ABILITY_REG3_AN_MR_ADV_ABILITY_2_OFFSET, ability_2_temp));


    return AW_ERR_CODE_NONE;

}

int aw_pmd_anlt_ms_per_ck_set(mss_access_t *mss, uint32_t ms_per_ck) {

    CHECK(pmd_write_field(mss,ETH_ANLT_CTRL_ADDR ,ETH_ANLT_CTRL_ANLT_MS_PER_CK_MASK ,ETH_ANLT_CTRL_ANLT_MS_PER_CK_OFFSET, ms_per_ck));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_config_set (mss_access_t *mss, uint32_t status_check_disable, uint32_t next_page_en, uint32_t an_no_nonce_check){

    CHECK(pmd_write_field(mss,ETH_ANLT_CTRL_ADDR , ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_MASK, ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_OFFSET , status_check_disable));

     
    if (next_page_en == 0) {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 1));
    } else {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 32769  ));
    }

     
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_MASK, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_OFFSET, an_no_nonce_check));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_start_set (mss_access_t *mss, uint32_t start){
    CHECK(pmd_write_field(mss, ETH_AN_CTRL_REG2_ADDR, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_MASK, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_OFFSET, start));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_status_complete_get (mss_access_t *mss, uint32_t * an_complete){

    CHECK(pmd_read_field(mss, ETH_AN_STAT_ADDR, ETH_AN_STAT_AN_MR_AUTONEG_COMPLETE_MASK, ETH_AN_STAT_AN_MR_AUTONEG_COMPLETE_OFFSET, an_complete));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_en_set (mss_access_t *mss, uint32_t en){
    CHECK(pmd_write_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_MASK, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_link_training_without_an_config_set (mss_access_t *mss, uint32_t width, uint32_t clause) {

    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_preset_check_set (mss_access_t *mss, uint32_t clause, uint32_t preset_check) {
    
    
     
    if (clause == 1 || clause == 2){
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C72_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C72_OFFSET, preset_check));
    } else if (clause == 3 || clause == 4){
        if (preset_check == 1){
            CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_OFFSET, 3  ));
        } else {
            CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_OFFSET, 0));
        }
    }
    return AW_ERR_CODE_NONE;   
}

int aw_pmd_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause, uint32_t mod){
    
    width = width;
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_RXDURINGEQ_MASK, ETH_LT_SETTINGS2_LT_RXDURINGEQ_OFFSET, 1));

     
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_MASK, ETH_LT_SETTINGS_LT_REG_FINAL_MOD_OFFSET, mod));

    if (clause >= 3){
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_MASK, ETH_LT_SETTINGS_LT_REG_TRAINING_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_SWEEP_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS2_ADDR, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_MASK, ETH_LT_SETTINGS2_LT_REG_TRAINING_INIT_MOD_OFFSET, mod+2));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, 0));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_prbs_seed_set (mss_access_t *mss, uint32_t clause, uint32_t logical_lane){

    if (clause == 1){
         
        CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, rand()&0x7FF));
    }
    else if (clause == 2){
         
        if ( logical_lane == 0 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x57e));
        } else if ( logical_lane == 1 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x645));
        } else if ( logical_lane == 2 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x72d));
        } else if ( logical_lane == 3 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_11B_MASK, ETH_LT_PRBS_LT_SEED_11B_OFFSET, 0x7b6));
        }
    } else if (clause == 3 || clause == 4){
         
         
        if ( logical_lane%4 == 0 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x00ab));

        } else if ( logical_lane%4 == 1 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x0741));
        } else if ( logical_lane%4 == 2 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x122c));
        } else if ( logical_lane%4 == 3 ){
             
            CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_SEED_13B_MASK, ETH_LT_PRBS_LT_SEED_13B_OFFSET, 0x0882));
        }
    }

    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_link_training_init_preset_set (mss_access_t *mss, uint32_t clause, uint32_t init[], uint32_t preset[], uint32_t preset2[]){
    if (clause == 1 || clause == 2) {
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_INIT_ADDR, ETH_LT_TXCOEFF_INIT_LT_CM1_INIT_MASK, ETH_LT_TXCOEFF_INIT_LT_CM1_INIT_OFFSET, init[0]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_INIT_ADDR, ETH_LT_TXCOEFF_INIT_LT_CP0_INIT_MASK, ETH_LT_TXCOEFF_INIT_LT_CP0_INIT_OFFSET, init[1] + 1));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_INIT_ADDR, ETH_LT_TXCOEFF_INIT_LT_CP1_INIT_MASK, ETH_LT_TXCOEFF_INIT_LT_CP1_INIT_OFFSET, init[2]));
        CHECK(pmd_write_field(mss, ETH_LT_COEFF_PRESET_ADDR, ETH_LT_COEFF_PRESET_LT_CM1_PRESET_MASK, ETH_LT_COEFF_PRESET_LT_CM1_PRESET_OFFSET, preset[0]));
        CHECK(pmd_write_field(mss, ETH_LT_COEFF_PRESET_ADDR, ETH_LT_COEFF_PRESET_LT_CP0_PRESET_MASK, ETH_LT_COEFF_PRESET_LT_CP0_PRESET_OFFSET, preset[1] + 1));
        CHECK(pmd_write_field(mss, ETH_LT_COEFF_PRESET_ADDR, ETH_LT_COEFF_PRESET_LT_CP1_PRESET_MASK, ETH_LT_COEFF_PRESET_LT_CP1_PRESET_OFFSET, preset[2]));
    } else if (clause == 3 || clause == 4) {
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET1_ADDR, ETH_LT_TXCOEFF_PRESET1_LT_CM1_PRESET1_MASK, ETH_LT_TXCOEFF_PRESET1_LT_CM1_PRESET1_OFFSET, init[0]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET1_ADDR, ETH_LT_TXCOEFF_PRESET1_LT_CP0_PRESET1_MASK, ETH_LT_TXCOEFF_PRESET1_LT_CP0_PRESET1_OFFSET, init[1] + 1));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET1_ADDR, ETH_LT_TXCOEFF_PRESET1_LT_CP1_PRESET1_MASK, ETH_LT_TXCOEFF_PRESET1_LT_CP1_PRESET1_OFFSET, init[2]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET2_ADDR, ETH_LT_TXCOEFF_PRESET2_LT_CM1_PRESET2_MASK, ETH_LT_TXCOEFF_PRESET2_LT_CM1_PRESET2_OFFSET, preset[0]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET2_ADDR, ETH_LT_TXCOEFF_PRESET2_LT_CP0_PRESET2_MASK, ETH_LT_TXCOEFF_PRESET2_LT_CP0_PRESET2_OFFSET, preset[1] + 1));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET2_ADDR, ETH_LT_TXCOEFF_PRESET2_LT_CP1_PRESET2_MASK, ETH_LT_TXCOEFF_PRESET2_LT_CP1_PRESET2_OFFSET, preset[2]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET3_ADDR, ETH_LT_TXCOEFF_PRESET3_LT_CM1_PRESET3_MASK, ETH_LT_TXCOEFF_PRESET3_LT_CM1_PRESET3_OFFSET, preset2[0]));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET3_ADDR, ETH_LT_TXCOEFF_PRESET3_LT_CP0_PRESET3_MASK, ETH_LT_TXCOEFF_PRESET3_LT_CP0_PRESET3_OFFSET, preset2[1] + 1));
        CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_PRESET3_ADDR, ETH_LT_TXCOEFF_PRESET3_LT_CP1_PRESET3_MASK, ETH_LT_TXCOEFF_PRESET3_LT_CP1_PRESET3_OFFSET, preset2[2]));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_min_max_set (mss_access_t *mss, uint32_t pre1_max, uint32_t main_max, uint32_t post1_max, uint32_t pre1_min, uint32_t main_min, uint32_t post1_min){
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CM1_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CM1_MAX_OFFSET, pre1_max));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CP0_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CP0_MAX_OFFSET, main_max + 1));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CP1_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CP1_MAX_OFFSET, post1_max));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CM1_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CM1_MIN_OFFSET, pre1_min));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CP0_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CP0_MIN_OFFSET, main_min + 1));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CP1_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CP1_MIN_OFFSET, post1_min));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_status_get (mss_access_t *mss, uint32_t * lt_running, uint32_t * lt_done, uint32_t * lt_training_failure, uint32_t * lt_rx_ready){
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RUNNING_MASK, ETH_LT_STAT_LT_RUNNING_OFFSET, lt_running));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_LT_DONE_MASK, ETH_ANLT_STATUS_LT_DONE_OFFSET, lt_done));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_TRAINING_FAILURE_MASK, ETH_LT_STAT_LT_TRAINING_FAILURE_OFFSET, lt_training_failure));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RX_READY_MASK, ETH_LT_STAT_LT_RX_READY_OFFSET, lt_rx_ready));

    return AW_ERR_CODE_NONE;
}


int aw_pmd_refclk_termination_set(mss_access_t *mss, aw_refclk_term_mode_t lsrefbuf_term_mode){
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_MASK, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_OFFSET, lsrefbuf_term_mode ));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_termination_set(mss_access_t *mss, aw_acc_term_mode_t acc_term_mode){
    CHECK(pmd_write_field(mss, AFE_OCTERM_RX_ADDR, AFE_OCTERM_RX_ACC_TERM_MODE_NT_MASK, AFE_OCTERM_RX_ACC_TERM_MODE_NT_OFFSET, acc_term_mode ));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_force_signal_detect_config_set(mss_access_t *mss, aw_force_sigdet_mode_t sigdet_mode){
    switch (sigdet_mode) {
        case AW_SIGDET_FORCE0:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 1));
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_FORCE1:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 1));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_NORM:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
            return AW_ERR_CODE_NONE;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
}

int aw_pmd_txfir_config_set(mss_access_t *mss, aw_txfir_config_t txfir_cfg){
    uint32_t cm3_mask = 0x7;
    uint32_t cm2_mask = 0x7;
    uint32_t cm1_mask = 0x1F;
    uint32_t max_mask = 0x3F;
    uint32_t c1_mask = 0x1F;

    uint32_t max_ele;
    if (txfir_cfg.main_or_max == 0){
        max_ele = txfir_cfg.C1 + txfir_cfg.C0 + txfir_cfg.CM1 + txfir_cfg.CM2 + txfir_cfg.CM3 - 1;
    } else {
        max_ele = txfir_cfg.C0;
    }

    uint32_t fir = ((txfir_cfg.C1 & c1_mask) << 17);
    fir = fir + ((max_ele & max_mask) << 11);
    fir = fir + ((txfir_cfg.CM1 & cm1_mask) << 6);
    fir = fir + ((txfir_cfg.CM2 & cm2_mask) << 3);
    fir = fir + (txfir_cfg.CM3 & cm3_mask);

    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_OVR_EN_A_MASK, FIR_OVR_EN_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_VAL_A_MASK, FIR_VAL_A_OFFSET, fir));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
     
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_PLUSD_ENABLE_A_MASK, TX_DATAPATH_REG1_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
    if (en == 1) {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
    } else {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 1));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_PLUSD_ENABLE_A_MASK, RX_DEMAPPER_PLUSD_ENABLE_A_OFFSET, plusd_en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_remote_loopback_set(mss_access_t *mss, uint32_t remote_loopback_enable){
     
    *mss = *mss;
    remote_loopback_enable = remote_loopback_enable;
    USR_PRINTF("ERROR: aw_pmd_remote_loopback_set - function implementation has been deprecated, use aw_pmd_fep_clock_set & aw_pmd_fep_dat_set\n");
    return AW_ERR_CODE_NONE;
}

int aw_pmd_fep_data_set(mss_access_t *mss, uint32_t datapath_en) {
     
     
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, datapath_en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_dcd_iq_cal(mss_access_t *mss, uint32_t enable_d)
{
    int32_t poll_result;
    
    if (enable_d > 1){
         
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }

     
    CHECK(pmd_write_field(mss, PD_AFE_TX_ADDR, PD_AFE_TX_DCDIQ_BA_MASK, PD_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_TX_ADDR, RST_AFE_TX_DCDIQ_BA_MASK, RST_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG1_ADDR, TX_PHASE_ADAPT_REG1_BYPASS_ENA_A_MASK, TX_PHASE_ADAPT_REG1_BYPASS_ENA_A_OFFSET, 0));

     
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG4_ADDR, TX_PHASE_ADAPT_REG4_MAX_ITER_NT_MASK, TX_PHASE_ADAPT_REG4_MAX_ITER_NT_OFFSET, 170));
     
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
     
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
     
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 2));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
     
    CHECK(pmd_write_field(mss, PD_AFE_TX_ADDR, PD_AFE_TX_DCDIQ_BA_MASK, PD_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_TX_ADDR, RST_AFE_TX_DCDIQ_BA_MASK, RST_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, enable_d));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, enable_d));
    return AW_ERR_CODE_NONE;        
}

int aw_pmd_fep_clock_set(mss_access_t *mss, uint8_t clock_en) {
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 1));
    
    if (clock_en != 0xff){
        aw_pmd_tx_dcd_iq_cal(mss, clock_en);    
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_analog_loopback_set(mss_access_t *mss, uint32_t analog_loopback_enable){
     
     
     
     
     

     
    CHECK(pmd_write_field(mss, RX_ADAPT_VGA_OFFSET_CTLE_ADDR, RX_ADAPT_VGA_OFFSET_CTLE_ISOLATE_A_MASK, RX_ADAPT_VGA_OFFSET_CTLE_ISOLATE_A_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_NES_LOOPBACK_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_NES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_NES_LOOPBACK_ENA_NT_OFFSET, analog_loopback_enable));
    CHECK(pmd_write_field(mss, NES_MODE_ADDR, NES_MODE_MASK, NES_MODE_OFFSET, analog_loopback_enable));

     
    if (analog_loopback_enable == 0){
        CHECK(aw_pmd_force_signal_detect_config_set(mss, AW_SIGDET_NORM));
    } else if (analog_loopback_enable == 1){
        CHECK(aw_pmd_force_signal_detect_config_set(mss, AW_SIGDET_FORCE1));
    }

    return AW_ERR_CODE_NONE;
}

int aw_pmd_fes_loopback_set(mss_access_t *mss, uint32_t fes_loopback_enable) {
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_analog_loopback_txfir_set(mss_access_t *mss, aw_analog_loopback_txfir_config_t *nes_txfir_cfg){
     
    uint32_t fir_val;
    fir_val = ((nes_txfir_cfg->nes_post1 & 0xF) << 4 ) | ((nes_txfir_cfg->nes_c0) & 0xF);
    CHECK(pmd_write_field(mss, TX_ADDR, TX_NES_LOOPBACK_FIR_A_MASK, TX_NES_LOOPBACK_FIR_A_OFFSET, fir_val));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_nep_loopback_set(mss_access_t *mss, uint32_t nep_loopback_enable){
     
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG5_ADDR, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_MASK, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG3_ADDR, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_MASK, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_MASK, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_OFFSET, nep_loopback_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_polarity_set(mss_access_t *mss, uint32_t tx_pol_flip){
     
     
     
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, tx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_polarity_set(mss_access_t *mss, uint32_t rx_pol_flip){
     
     
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, rx_pol_flip));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_dfe_adapt_set(mss_access_t *mss, uint32_t dfe_adapt_enable){
    CHECK(pmd_write_field(mss, RX_EQDFE_ADDR, RX_EQDFE_DFE_ENABLE_NT_MASK, RX_EQDFE_DFE_ENABLE_NT_OFFSET, dfe_adapt_enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rxeq_prbs_set(mss_access_t *mss, uint32_t prbs_en) {
    CHECK(pmd_write_field(mss, RXEQ_PRBS_ADDR, RXEQ_PRBS_MASK, RXEQ_PRBS_OFFSET, prbs_en));
    return AW_ERR_CODE_NONE;
}

 
int aw_pmd_rx_chk_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, aw_bist_mode_t mode, uint64_t udp, uint32_t lock_thresh, uint32_t timer_thresh){
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));

    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t udp_en = 0;

    switch (pattern){
    case AW_PRBS7:
    case AW_PRBS9:
    case AW_PRBS11:
    case AW_PRBS13:
    case AW_PRBS15:
    case AW_PRBS23:
    case AW_PRBS31:
    case AW_QPRBS13:
    case AW_JP03A:
    case AW_JP03B:
    case AW_LINEARITY_PATTERN:
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, pattern));
        break;
    case AW_USER_DEFINED_PATTERN:
        udp_31_0  = (uint32_t)(udp & 0xFFFFFFFF);
        udp_63_32 = (uint32_t)((udp >> 32) & 0xFFFFFFFF);
        udp_en = 1;
        break;
    case AW_FULL_RATE_CLOCK:
        udp_31_0  = 0xAAAAAAAA;
        udp_63_32 = 0xAAAAAAAA;
        udp_en = 1;
        break;
    case AW_HALF_RATE_CLOCK:
        udp_31_0  = 0xCCCCCCCC;
        udp_63_32 = 0xCCCCCCCC;
        udp_en = 1;
        break;
    case AW_QUARTER_RATE_CLOCK:
        udp_31_0  = 0xF0F0F0F0;
        udp_63_32 = 0xF0F0F0F0;
        udp_en = 1;
        break;
    case AW_PATT_32_1S_32_0S:
        udp_63_32 = 0xFFFFFFFF;
        udp_31_0  = 0x00000000;
        udp_en = 1;
        break;
    default:
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    if (udp_en) {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG4_ADDR, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_MASK, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG5_ADDR, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_MASK, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG6_ADDR, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_MASK, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG7_ADDR, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_MASK, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG8_ADDR, RX_DATABIST_TOP_REG8_UDP_PATTERN_159_128_NT_MASK, RX_DATABIST_TOP_REG8_UDP_PATTERN_159_128_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG9_ADDR, RX_DATABIST_TOP_REG9_UDP_PATTERN_191_160_NT_MASK, RX_DATABIST_TOP_REG9_UDP_PATTERN_191_160_NT_OFFSET, udp_63_32));
    }

    if (mode == AW_TIMER) {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, 0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_LOCK_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG1_LOCK_THRESHOLD_NT_OFFSET, lock_thresh));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG2_TIMER_THRESHOLD_NT_OFFSET, timer_thresh));
    } else {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_WALL_CLOCK_ENABLE_A_OFFSET, 1));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, 1));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_chk_en_set(mss_access_t *mss, uint32_t enable){
     
     
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_chk_lock_state_get(mss_access_t *mss, uint32_t *rx_bist_lock){
     
     
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_LOCKED_NT_MASK, RX_DATABIST_TOP_RDREG1_LOCKED_NT_OFFSET, rx_bist_lock));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_chk_err_count_state_get(mss_access_t *mss, uint64_t *err_count, uint32_t *err_count_done, uint32_t *err_count_overflown){
    uint32_t bist_mode;
    uint32_t err_cnt_55_32;
    uint32_t err_cnt_31_0;
    uint32_t err_code;

    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, &bist_mode));

    if (bist_mode == 1){
        uint32_t bist_enable;
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, &bist_enable));
        if (bist_enable == 1){
            *err_count_done = 0;
            *err_count = 0;
            *err_count_overflown = 0;
            err_code = 1;
        } else {
            *err_count_done = 1;
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
            *err_count = (uint64_t)err_cnt_55_32 << 32 | (uint64_t)err_cnt_31_0;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0  ));
            err_code = 0;
        }
    } else {
        CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, RD_EQ, err_count_done, 1, 0  ));
        if (!err_count_done) {
            *err_count_done = 0;
            *err_count = 0;
            *err_count_overflown = 0;
            err_code = 2;
        } else {
            *err_count_done = 1;
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
            *err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0  ));
            err_code = 0;
        }
    }
    if (err_code == 1 || err_code == 2 ) {
        return AW_ERR_CODE_FUNC_FAILURE;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_chk_err_count_state_clear(mss_access_t *mss){
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gen_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, uint64_t udp){
    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t udp_en = 0;

    switch (pattern){
        case AW_PRBS7:
        case AW_PRBS9:
        case AW_PRBS11:
        case AW_PRBS13:
        case AW_PRBS15:
        case AW_PRBS23:
        case AW_PRBS31:
        case AW_QPRBS13:
        case AW_JP03A:
        case AW_JP03B:
        case AW_LINEARITY_PATTERN:
            CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, pattern));
            break;
        case AW_USER_DEFINED_PATTERN:
            udp_31_0  = (uint32_t)(udp & 0xFFFFFFFF);
            udp_63_32 = (uint32_t)((udp >> 32) & 0xFFFFFFFF);
            udp_en = 1;
            break;
        case AW_FULL_RATE_CLOCK:
            udp_31_0 = 0xAAAAAAAA;
            udp_63_32 = 0xAAAAAAAA;
            udp_en = 1;
            break;
        case AW_HALF_RATE_CLOCK:
            udp_31_0 = 0xCCCCCCCC;
            udp_63_32 = 0xCCCCCCCC;
            udp_en = 1;
            break;
        case AW_QUARTER_RATE_CLOCK:
            udp_31_0 = 0xF0F0F0F0;
            udp_63_32 = 0xF0F0F0F0;
            udp_en = 1;
            break;
        case AW_PATT_32_1S_32_0S:
            udp_63_32 = 0xFFFFFFFF;
            udp_31_0 = 0x00000000;
            udp_en = 1;
            break;
        default:
            return AW_ERR_CODE_INVALID_ARG_VALUE;  
    }
    if (udp_en) {
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
 
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG9_ADDR, TX_DATAPATH_REG9_UDP_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG9_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG8_ADDR, TX_DATAPATH_REG8_UDP_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG8_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
    }
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gen_en_set(mss_access_t *mss, uint32_t enable){
     
     
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gen_err_inject_config_set(mss_access_t *mss, uint64_t err_pattern, uint32_t err_rate){
     
     
     
     
    uint32_t err_pattern_31_0 = err_pattern & 0xFFFFFFFF;
    uint32_t err_pattern_63_32 = err_pattern >> 32 & 0xFFFFFFFF;
 
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG15_ADDR, TX_DATAPATH_REG15_ERROR_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG15_ERROR_PATTERN_63_32_NT_OFFSET, err_pattern_63_32));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG14_ADDR, TX_DATAPATH_REG14_ERROR_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG14_ERROR_PATTERN_31_0_NT_OFFSET, err_pattern_31_0));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_OFFSET, err_rate));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gen_err_inject_en_set(mss_access_t *mss, uint32_t enable){
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_OFFSET, enable));
    return AW_ERR_CODE_NONE;
}

 

 
int aw_pmd_uc_ucode_load(mss_access_t *mss, uint32_t (*ucode_arr)[2], uint32_t ucode_len){
    for (uint32_t i = 0; i<ucode_len; i++){
        CHECK(pmd_write_addr(mss, ucode_arr[i][0], ucode_arr[i][1]));  
    }
    return AW_ERR_CODE_NONE;
}

int c_load_hexfile(mss_access_t *mss, char * fileName) {
    int pass = 0;

    FILE* file = fopen(fileName, "r");  
    char line[256];
    uint32_t aw_ucode[AW_SRAM_SIZE][2];
    uint32_t i = 0;
    while (fgets(line, sizeof(line), file)) {
        char * token = strtok(line, ",");  
        uint32_t addr;
        sscanf(token, "%x", &addr);  
        token = strtok(NULL, ",");  
        uint32_t value;
        sscanf(token, "%x", &value);  
        aw_ucode[i][0] = addr;
        aw_ucode[i][1] = value;
        i++;
        continue;
    }

    fclose(file);

    USR_PRINTF("Calling aw_pmd_uc_ucode_load\n");
    pass += aw_pmd_uc_ucode_load(mss, aw_ucode, i);
    return pass;

}

int aw_pmd_pll_lock_min_set(mss_access_t *mss, uint32_t val){
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_MIN_NT_MASK, LCPLL_CHECK_MIN_NT_OFFSET, val));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock ,uint32_t check_en, uint32_t expected_val){
    int poll_result;
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_DONE_A_MASK, LCPLL_CHECK_RDREG_DONE_A_OFFSET, 1, 20);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Polling for LC PLL Lock check done\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("LC PLL Lock check done\n");
        if (check_en) {
            CHECK(pmd_read_check_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, RD_EQ, pll_lock, expected_val, 0  ));
        } else {
            CHECK(pmd_read_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, pll_lock));
        }
    }
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 0));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_pstate_set(mss_access_t *mss, uint32_t value){
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_pstate_get(mss_access_t *mss, uint32_t *value){
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_state_req_set(mss_access_t *mss, uint32_t value){
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_state_ack_get(mss_access_t *mss, uint32_t *cmn_state_ack){
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_STAT_ADDR, DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_MASK,  DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET, cmn_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_reset_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_reset_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_reset_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_reset_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_rate_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_rate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_rate_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_rate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_pstate_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_pstate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_pstate_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_pstate_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_width_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_width_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_width_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_width_get(mss_access_t *mss, uint32_t *value) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_state_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_state_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_state_ack_get(mss_access_t *mss, uint32_t *tx_state_ack) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_OFFSET, tx_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_state_ack_get(mss_access_t *mss, uint32_t *rx_state_ack) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET, rx_state_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_cmn_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CTRL_CLK_A_MASK, DIG_SOC_CMN_OVRD_CTRL_CLK_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_cmn_get(mss_access_t *mss, uint32_t *en) {
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_set(mss_access_t *mss, uint32_t en) {

     
    aw_pmd_isolate_lane_tx_set(mss, en);
    aw_pmd_isolate_lane_rx_set(mss, en);
    aw_pmd_isolate_lane_txrx_set(mss, en);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_tx_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_rx_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_txrx_set(mss_access_t *mss, uint32_t en) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET, en));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_get(mss_access_t *mss, uint32_t *en) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
 
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
 
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
 
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
 
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
 
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_OFFSET, sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
 
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_OFFSET, sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
 
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
 
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_OFFSET, sel));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_lsref_sel_set(mss_access_t *mss, uint32_t ref_sel) {
 
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_LSREF_SELECT_NT_OFFSET, ref_sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_pcie_cmn_lsref_25m_get(mss_access_t *mss, uint32_t *lsref_25m) {
    CHECK(pmd_read_field(mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_MASK, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_OFFSET, lsref_25m));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_gen_tx_en_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_request_cmn_state_change(mss_access_t *mss, aw_cmn_pstate_t cmn_pstate, uint32_t timeout_us) {
    int poll_result = 0;
    aw_pmd_iso_cmn_pstate_set(mss, cmn_pstate);
    aw_pmd_iso_cmn_state_req_set(mss, 1);
     
    USR_SLEEP(1000);
    poll_result = pmd_poll_field(mss, DIG_SOC_CMN_STAT_ADDR, DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_MASK,  DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for CMN state ack\n");
    } else {
        USR_PRINTF("CMN state ack received\n");
    }
    aw_pmd_iso_cmn_state_req_set(mss, 0);
    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_iso_request_tx_state_change(mss_access_t *mss, aw_pstate_t tx_pstate, uint32_t tx_rate, uint32_t tx_width, uint32_t timeout_us) {
    int poll_result;
    USR_PRINTF("Setting TX rate/width/pstate\n");
    aw_pmd_iso_tx_reset_set(mss,1);
    aw_pmd_iso_tx_rate_set(mss,tx_rate);
    aw_pmd_iso_tx_width_set(mss,tx_width);
    aw_pmd_iso_tx_pstate_set(mss,tx_pstate);

    aw_pmd_iso_tx_state_req_set(mss,1);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for TX state ack\n");
    } else {
        USR_PRINTF("TX state ack received\n");
    }

    aw_pmd_iso_tx_state_req_set(mss,0);

    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_iso_request_rx_state_change(mss_access_t *mss, aw_pstate_t rx_pstate, uint32_t rx_rate, uint32_t rx_width, uint32_t timeout_us) {
    int poll_result;
    USR_PRINTF("Setting RX rate/width/pstate\n");
    aw_pmd_iso_rx_reset_set(mss,1);
    aw_pmd_iso_rx_rate_set(mss,rx_rate);
    aw_pmd_iso_rx_width_set(mss,rx_width);
    aw_pmd_iso_rx_pstate_set(mss,rx_pstate);

    aw_pmd_iso_rx_state_req_set(mss,1);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: polling for RX state ack\n");
    } else {
        USR_PRINTF("RX state ack received\n");
    }

    aw_pmd_iso_rx_state_req_set(mss,0);

    if (poll_result == -1) {
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_check_cdr_lock(mss_access_t *mss, uint32_t timeout_us) {
    int poll_result;
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: RX CDR timed out waiting for lock\n");
        return AW_ERR_CODE_POLL_TIMEOUT;

    } else {
        USR_PRINTF("RX CDR is locked\n");
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_check_bist(mss_access_t *mss, aw_bist_mode_t bist_mode, uint32_t timer_threshold, uint32_t rx_width, uint32_t timeout_us, int32_t expected_errors) {
    uint32_t err_count_overflow, err_cnt_55_32, err_cnt_31_0;
    uint64_t err_count;
    uint32_t width;
    double ber;
    int poll_result;
    if (bist_mode == AW_TIMER) {
         
        poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, 1, timeout_us);

        if (poll_result == -1) {
            USR_PRINTF("ERROR: Timed out waiting for error_cnt_done\n");
            return AW_ERR_CODE_POLL_TIMEOUT;

        } else {
            if (expected_errors == -1) {
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));

            } else {
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, RD_EQ, &err_cnt_55_32, 0, 0  ));
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, RD_EQ, &err_cnt_31_0, (uint32_t)expected_errors, 0  ));
            }
             
             
            err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
            (void)err_count;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, &err_count_overflow, 0, 0  ));
            width = aw_width_decoder(rx_width);
            ber = (double) err_count / (width * timer_threshold);
            (void)ber;
            USR_PRINTF("err_count = %" PRIu64 "\n", err_count);
            USR_PRINTF("err_count_overflow = %d\n", err_count_overflow);
            USR_PRINTF("ber = %e\n", ber);

            return AW_ERR_CODE_NONE;
        }
    }
     
    return AW_ERR_CODE_FUNC_FAILURE;
}

int aw_pmd_rx_burst_err_config_set(mss_access_t *mss, uint32_t burst_err_threshold) {
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, burst_err_threshold));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_burst_err_get(mss_access_t *mss, uint32_t *burst_errs) {
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG12_ADDR, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_MASK, RX_DATABIST_TOP_RDREG12_BURST_ERR_FOUND_CNT_NT_OFFSET, burst_errs));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_type_set(mss_access_t *mss, uint32_t eq_type) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_OFFSET, eq_type));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_req_set(mss_access_t *mss, uint32_t value) {
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_OFFSET, value));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_ack_get(mss_access_t *mss, uint32_t *eqeval_ack) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, eqeval_ack));
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_incdec_get(mss_access_t *mss, uint32_t *incdec) {
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG3_ADDR, DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_DIR_MASK, DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_DIR_OFFSET, incdec));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_equalize(mss_access_t *mss, aw_eq_type_t eq_type, uint32_t timeout_us) {
    int poll_result;
    uint32_t incdec;
    aw_pmd_eqeval_type_set(mss, eq_type);
    aw_pmd_rxeq_prbs_set(mss, 1);
    aw_pmd_eqeval_req_set(mss, 1);
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 1, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for asserting rx linkeval ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    aw_pmd_eqeval_req_set(mss, 0);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 0, 10);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for rx linkeval ack\n");
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("Received RXEQ EVAL Ack\n");
        aw_pmd_eqeval_incdec_get(mss,&incdec);
        USR_PRINTF("EqEval incdec = 0x%X\n",incdec);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_burst_mode_config_set(mss_access_t *mss, uint32_t pam_mode, uint32_t burst_threshold, uint32_t burst_mode){

    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_ERR_SYMBOL_MODE_NT_OFFSET, pam_mode));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BURST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BURST_MODE_NT_OFFSET, burst_mode));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG14_ADDR, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_MASK, RX_DATABIST_TOP_REG14_BURST_ERROR_BITS_THRESHOLD_NT_OFFSET, burst_threshold));


    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map) {
    uint8_t el3 = gray_code_map & 0x3;
    uint8_t el1 = (gray_code_map >> 2) & 0x3;
    uint8_t eh1 = (gray_code_map >> 4) & 0x3;
    uint8_t eh3 = (gray_code_map >> 6) & 0x3;

    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, el3));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, el1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, eh1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, eh3));

    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map){

    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, gray_code_map));

    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_fastsim_timer_set(mss_access_t *mss) {

    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG1_ADDR, ETH_AN_TIMER_REG1_AN_LINK_FAIL_INHIBIT_TIMER_MAX_A_MASK, ETH_AN_TIMER_REG1_AN_LINK_FAIL_INHIBIT_TIMER_MAX_A_OFFSET, 19));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG1_ADDR, ETH_AN_TIMER_REG1_AN_LINK_FAIL_INHIBIT_TIMER_MAX_B_MASK, ETH_AN_TIMER_REG1_AN_LINK_FAIL_INHIBIT_TIMER_MAX_B_OFFSET, 19));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG2_ADDR, ETH_AN_TIMER_REG2_AN_LINK_FAIL_INHIBIT_TIMER_MAX_C_MASK, ETH_AN_TIMER_REG2_AN_LINK_FAIL_INHIBIT_TIMER_MAX_C_OFFSET, 19));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG2_ADDR, ETH_AN_TIMER_REG2_AN_PD_ATTEMPT_TIMER_MAX_MASK, ETH_AN_TIMER_REG2_AN_PD_ATTEMPT_TIMER_MAX_OFFSET, 0));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG3_ADDR, ETH_AN_TIMER_REG3_AN_PD_AUTONEG_TIMER_MAX_MASK, ETH_AN_TIMER_REG3_AN_PD_AUTONEG_TIMER_MAX_OFFSET, 2));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG3_ADDR, ETH_AN_TIMER_REG3_AN_PD_AUTONEG_TIMER_MAX_MASK, ETH_AN_TIMER_REG3_AN_PD_AUTONEG_TIMER_MAX_OFFSET, 2));
    CHECK(pmd_write_field(mss, ETH_AN_TIMER_REG3_ADDR, ETH_AN_TIMER_REG3_AN_BREAK_LINK_TIMER_MAX_MASK, ETH_AN_TIMER_REG3_AN_BREAK_LINK_TIMER_MAX_OFFSET, 1));
    CHECK(pmd_write_field(mss, ETH_LT_TIMER_REG1_ADDR, ETH_LT_TIMER_REG1_LT_MAX_WAIT_C136_MASK, ETH_LT_TIMER_REG1_LT_MAX_WAIT_C136_OFFSET, 19));
    CHECK(pmd_write_field(mss, ETH_LT_TIMER_REG1_ADDR, ETH_LT_TIMER_REG1_LT_MAX_WAIT_C72_MASK, ETH_LT_TIMER_REG1_LT_MAX_WAIT_C72_OFFSET, 19));
    CHECK(pmd_write_field(mss, ETH_LT_TIMER_REG2_ADDR, ETH_LT_TIMER_REG2_LT_READY_TIMER_C136_MASK, ETH_LT_TIMER_REG2_LT_READY_TIMER_C136_OFFSET, 99));
    CHECK(pmd_write_field(mss, ETH_LT_TIMER_REG2_ADDR, ETH_LT_TIMER_REG2_LT_READY_TIMER_C72_MASK, ETH_LT_TIMER_REG2_LT_READY_TIMER_C72_OFFSET, 199));
    CHECK(pmd_write_field(mss, ETH_LT_TIMER_REG6_ADDR, ETH_LT_TIMER_REG6_LT_HOLDOFF_C136_MASK, ETH_LT_TIMER_REG6_LT_HOLDOFF_C136_OFFSET, 1));

    return AW_ERR_CODE_NONE;
}


 
 
 
int aw_pmd_snr_vld_hys_thresh_set_from_target_snr(mss_access_t *mss, uint32_t target_snr_low, uint32_t target_snr_high){
    uint32_t thresh_low_reg_value  = round(pow(10, target_snr_low/10.0));
    uint32_t thresh_high_reg_value = round(pow(10, target_snr_high/10.0));

    USR_PRINTF("Target SNR is between %d dB and %d dB.\n", target_snr_low, target_snr_high);
    USR_PRINTF("Setting register values for snr valid thresholds: thresh_low_reg_value=%d and thresh_high_reg_value=%d.\n", thresh_low_reg_value, thresh_high_reg_value);

    CHECK(pmd_write_field(mss, RX_SNR_REG4_ADDR, RX_SNR_REG4_VLD_HYS_THRESH_LOW_NT_MASK,  RX_SNR_REG4_VLD_HYS_THRESH_LOW_NT_OFFSET,  thresh_low_reg_value ));
    CHECK(pmd_write_field(mss, RX_SNR_REG5_ADDR, RX_SNR_REG5_VLD_HYS_THRESH_HIGH_NT_MASK, RX_SNR_REG5_VLD_HYS_THRESH_HIGH_NT_OFFSET, thresh_high_reg_value));

    return AW_ERR_CODE_NONE;
}
 
 
int aw_pmd_snr_vld_hys_thresh_set_recommended(mss_access_t *mss, uint32_t nrz_mode){
    uint32_t target_snr_low  = (nrz_mode==1) ? 20 : 12;  
    uint32_t target_snr_high = (nrz_mode==1) ? 25 : 15;  

    aw_pmd_snr_vld_hys_thresh_set_from_target_snr(mss, target_snr_low, target_snr_high);

    return AW_ERR_CODE_NONE;
}

int aw_pmd_snr_mon_enable_set(mss_access_t *mss, uint32_t nrz_mode, uint32_t mon_enable){
    CHECK(pmd_write_field(mss, RST_DBE_RX_ADDR,  RST_DBE_RX_SNR_BA_MASK,           RST_DBE_RX_SNR_BA_OFFSET,           1         ));
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_MON_NRZ_MODE_NT_MASK, RX_SNR_REG1_MON_NRZ_MODE_NT_OFFSET, nrz_mode  ));  
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_MON_ENABLE_A_MASK,    RX_SNR_REG1_MON_ENABLE_A_OFFSET,    mon_enable));
    return AW_ERR_CODE_NONE;
}

 
int aw_pmd_snr_vld_enable_set(mss_access_t *mss, uint32_t vld_enable){
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_VLD_ENABLE_A_MASK, RX_SNR_REG1_VLD_ENABLE_A_OFFSET, vld_enable));
    return AW_ERR_CODE_NONE;
}
