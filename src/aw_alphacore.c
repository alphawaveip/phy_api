// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */







#include <stdint.h>
#include <math.h>
#include <string.h>
#define __STDC_FORMAT_MACROS 1 
#include <inttypes.h>

#include "aw_alphacore.h"
#include "aw_alphacore_vfield_defines.h"
#include "aw_pmd_rx_dsp_get.h"
#include "stdlib.h"


const char aw_library_version[] = "1.0.11";

int aw_tc_sm_conv(uint32_t v, uint32_t i) {
    TRACE_ENTER("v = %d, i = %d", v,  i);
    int final_val;
    if(v >= pow(2,i-1)) {
        final_val = v - pow(2,i);
    } else {
        final_val = v;
    }
    TRACE_EXIT("v = %d, i = %d", v,  i);
    return final_val;
}

uint32_t aw_width_decoder (uint32_t width_encoded) {
    TRACE_ENTER("width_encoded = %d", width_encoded);
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
        width = 8;
    } else {
        USR_PRINTF("ERROR: Invalid width encoding\n");
    TRACE_EXIT("width_encoded = %d", width_encoded);
        return 0;
    }
    TRACE_EXIT("width_encoded = %d", width_encoded);
    return width;
}














int aw_pmd_anlt_logical_lane_num_set (mss_access_t *mss, uint32_t logical_lane, uint32_t an_no_attached) {
    TRACE_ENTER("mss=0x%x, 0x%x, logical_lane = %d, an_no_attached = %d", mss->phy_offset, mss->lane_offset,  logical_lane,  an_no_attached);
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_ANLT_LANE_NUM_MASK, ETH_ANLT_CTRL_ANLT_LANE_NUM_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C92_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_LT_PRBS_ADDR, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_MASK, ETH_LT_PRBS_LT_POLYNOMIAL_SEL_C136_OFFSET, logical_lane));
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_ATTACHED_MASK, ETH_ANLT_CTRL_AN_NO_ATTACHED_OFFSET, an_no_attached));

    TRACE_EXIT("mss=0x%x, 0x%x, logical_lane = %d, an_no_attached = %d", mss->phy_offset, mss->lane_offset,  logical_lane,  an_no_attached);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_auto_neg_link_status_ovr_enable(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);

    CHECK(pmd_write_field(mss, AN_LINK_CTRL_ADDR, AN_LINK_CTRL_STAT_OVR_EN_NT_MASK, AN_LINK_CTRL_STAT_OVR_EN_NT_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_ICTL_ETH_AN_LINK_STATUS_A_MASK, DIG_SOC_LANE_OVRD_REG4_ICTL_ETH_AN_LINK_STATUS_A_OFFSET, 8388607));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_TXCHNG_PEDGE_A_MASK, DIG_SOC_LANE_OVRD_REG2_TXCHNG_PEDGE_A_OFFSET, en));
    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_adv_ability_set (mss_access_t *mss, uint32_t *adv_ability, uint32_t *fec_ability, uint32_t nonce){
    TRACE_ENTER("mss=0x%x, 0x%x, *adv_ability = %d, *fec_ability = %d, nonce = %d", mss->phy_offset, mss->lane_offset,  *adv_ability,  *fec_ability,  nonce);
    uint32_t ability_1_temp = 0;
    uint32_t ability_2_temp = 0;

    for (int i = 0; i<=19; i++) {
        if (i <= 10 && adv_ability[i] == 1) {
            ability_1_temp = ability_1_temp | (1 << (i + 5));
        } else if (i > 10 && i <= 19 && adv_ability[i] == 1) {
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


    TRACE_EXIT("mss=0x%x, 0x%x, *adv_ability = %d, *fec_ability = %d, nonce = %d", mss->phy_offset, mss->lane_offset,  *adv_ability,  *fec_ability,  nonce);
    return AW_ERR_CODE_NONE;

}


int aw_pmd_anlt_ms_per_ck_set(mss_access_t *mss, uint32_t ms_per_ck) {
    TRACE_ENTER("mss=0x%x, 0x%x, ms_per_ck = %d", mss->phy_offset, mss->lane_offset,  ms_per_ck);

    CHECK(pmd_write_field(mss,ETH_ANLT_CTRL_ADDR ,ETH_ANLT_CTRL_ANLT_MS_PER_CK_MASK ,ETH_ANLT_CTRL_ANLT_MS_PER_CK_OFFSET, ms_per_ck));
    TRACE_EXIT("mss=0x%x, 0x%x, ms_per_ck = %d", mss->phy_offset, mss->lane_offset,  ms_per_ck);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_auto_neg_config_set (mss_access_t *mss, uint32_t status_check_disable, uint32_t next_page_en, uint32_t an_no_nonce_check){
    TRACE_ENTER("mss=0x%x, 0x%x, status_check_disable = %d, next_page_en = %d, an_no_nonce_check = %d", mss->phy_offset, mss->lane_offset,  status_check_disable,  next_page_en,  an_no_nonce_check);
    CHECK(pmd_write_field(mss,ETH_ANLT_CTRL_ADDR , ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_MASK, ETH_ANLT_CTRL_AN_STATUS_CHECK_DISABLE_OFFSET , status_check_disable));

    
    if (next_page_en == 0) {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 1));
    } else {
        CHECK(pmd_write_field(mss, ETH_AN_ADV_ABILITY_REG1_ADDR, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_MASK, ETH_AN_ADV_ABILITY_REG1_AN_MR_ADV_ABILITY_0_OFFSET, 32769 ));
    }

    
    CHECK(pmd_write_field(mss, ETH_ANLT_CTRL_ADDR, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_MASK, ETH_ANLT_CTRL_AN_NO_NONCE_CHECK_OFFSET, an_no_nonce_check));

    TRACE_EXIT("mss=0x%x, 0x%x, status_check_disable = %d, next_page_en = %d, an_no_nonce_check = %d", mss->phy_offset, mss->lane_offset,  status_check_disable,  next_page_en,  an_no_nonce_check);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_auto_neg_start_set (mss_access_t *mss, uint32_t start){
    TRACE_ENTER("mss=0x%x, 0x%x, start = %d", mss->phy_offset, mss->lane_offset,  start);
    CHECK(pmd_write_field(mss, ETH_AN_CTRL_REG2_ADDR, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_MASK, ETH_AN_CTRL_REG2_AN_MR_AUTONEG_ENABLE_OFFSET, start));

    TRACE_EXIT("mss=0x%x, 0x%x, start = %d", mss->phy_offset, mss->lane_offset,  start);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_anlt_auto_neg_status_complete_get (mss_access_t *mss, uint32_t * an_complete){
    TRACE_ENTER("mss=0x%x, 0x%x, *an_complete = %d", mss->phy_offset, mss->lane_offset,  *an_complete);

    CHECK(pmd_read_field(mss, ETH_AN_STAT_ADDR, ETH_AN_STAT_AN_MR_AUTONEG_COMPLETE_MASK, ETH_AN_STAT_AN_MR_AUTONEG_COMPLETE_OFFSET, an_complete));

    TRACE_EXIT("mss=0x%x, 0x%x, *an_complete = %d", mss->phy_offset, mss->lane_offset,  *an_complete);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_anlt_auto_neg_next_page_set(mss_access_t *mss, uint64_t an_tx_np) {
    TRACE_ENTER("mss=0x%x, 0x%x, an_tx_np = %ld", mss->phy_offset, mss->lane_offset,  an_tx_np);
    uint32_t an_mr_np_tx_1 = (an_tx_np & 0xFFFF);
    uint32_t an_mr_np_tx_2 = (an_tx_np >> 16) & 0xFFFF;
    uint32_t an_mr_np_tx_3 = (an_tx_np >> 32) & 0xFFFF;

    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG1_ADDR, ETH_AN_NP_REG1_AN_MR_NP_TX_1_MASK, ETH_AN_NP_REG1_AN_MR_NP_TX_1_OFFSET, an_mr_np_tx_1));
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG2_ADDR, ETH_AN_NP_REG2_AN_MR_NP_TX_2_MASK, ETH_AN_NP_REG2_AN_MR_NP_TX_2_OFFSET, an_mr_np_tx_2));
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG3_ADDR, ETH_AN_NP_REG3_AN_MR_NP_TX_3_MASK, ETH_AN_NP_REG3_AN_MR_NP_TX_3_OFFSET, an_mr_np_tx_3));

    
    CHECK(pmd_write_field(mss,  ETH_AN_NP_REG3_ADDR, ETH_AN_NP_REG3_AN_MR_NEXT_PAGE_LOADED_MASK, ETH_AN_NP_REG3_AN_MR_NEXT_PAGE_LOADED_OFFSET, 1));

    TRACE_EXIT("mss=0x%x, 0x%x, an_tx_np = %ld", mss->phy_offset, mss->lane_offset,  an_tx_np);
    return AW_ERR_CODE_NONE;

}

int aw_pmd_anlt_auto_neg_next_page_oui_compare_set(mss_access_t *mss, uint32_t np_expected_oui){
    TRACE_ENTER("mss=0x%x, 0x%x, np_expected_oui = %d", mss->phy_offset, mss->lane_offset,  np_expected_oui);
    
    
    

    CHECK(pmd_write_field(mss, ETH_AN_CTRL_REG3_ADDR, ETH_AN_CTRL_REG3_ECON_CID_U_MASK, ETH_AN_CTRL_REG3_ECON_CID_U_OFFSET, np_expected_oui));

    TRACE_EXIT("mss=0x%x, 0x%x, np_expected_oui = %d", mss->phy_offset, mss->lane_offset,  np_expected_oui);
    return AW_ERR_CODE_NONE;

}

int aw_pmd_anlt_link_training_en_set (mss_access_t *mss, uint32_t en){
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    CHECK(pmd_write_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_MASK, ETH_LT_CTRL_LT_MR_TRAINING_ENABLE_OFFSET, en));

    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_link_training_without_an_config_set (mss_access_t *mss, uint32_t width, uint32_t clause) {
    TRACE_ENTER("mss=0x%x, 0x%x, width = %d, clause = %d", mss->phy_offset, mss->lane_offset,  width,  clause);

    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    TRACE_EXIT("mss=0x%x, 0x%x, width = %d, clause = %d", mss->phy_offset, mss->lane_offset,  width,  clause);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_preset_check_set (mss_access_t *mss, uint32_t clause, uint32_t preset_check) {
    TRACE_ENTER("mss=0x%x, 0x%x, clause = %d, preset_check = %d", mss->phy_offset, mss->lane_offset,  clause,  preset_check);
    
    if (clause == 1 || clause == 2){
        CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C72_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C72_OFFSET, preset_check));
    } else if (clause == 3 || clause == 4){
        if (preset_check == 1){
            CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_OFFSET, 3 ));
        } else {
            CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_MASK, ETH_LT_SETTINGS_LT_TRAIN_PRESET_CHECK_C136_OFFSET, 0));
        }
    }
    TRACE_EXIT("mss=0x%x, 0x%x, clause = %d, preset_check = %d", mss->phy_offset, mss->lane_offset,  clause,  preset_check);
    return AW_ERR_CODE_NONE;   
}

int aw_pmd_anlt_link_training_reset (mss_access_t *mss){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    aw_pmd_anlt_link_training_en_set(mss, 0);
    aw_pmd_anlt_link_training_start_set(mss, 0);

    CHECK(pmd_write_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_RESTART_TRAINING_MASK, ETH_LT_CTRL_LT_MR_RESTART_TRAINING_OFFSET, 1));
    CHECK(pmd_write_field(mss, ETH_LT_CTRL_ADDR, ETH_LT_CTRL_LT_MR_RESTART_TRAINING_MASK, ETH_LT_CTRL_LT_MR_RESTART_TRAINING_OFFSET, 0));
    
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_config_set (mss_access_t *mss, uint32_t width, uint32_t clause, uint32_t mod){
    TRACE_ENTER("mss=0x%x, 0x%x, width = %d, clause = %d, mod = %d", mss->phy_offset, mss->lane_offset,  width,  clause,  mod);
    
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WIDTH_MASK, ETH_LT_NO_AN_CTRL_LT_WIDTH_OFFSET, width));
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_CTRL_MASK, ETH_LT_NO_AN_CTRL_LT_CTRL_OFFSET, clause));

    
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

    TRACE_EXIT("mss=0x%x, 0x%x, width = %d, clause = %d, mod = %d", mss->phy_offset, mss->lane_offset,  width,  clause,  mod);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_anlt_link_training_prbs_seed_set (mss_access_t *mss, uint32_t clause, uint32_t logical_lane){
    TRACE_ENTER("mss=0x%x, 0x%x, clause = %d, logical_lane = %d", mss->phy_offset, mss->lane_offset,  clause,  logical_lane);

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

    TRACE_EXIT("mss=0x%x, 0x%x, clause = %d, logical_lane = %d", mss->phy_offset, mss->lane_offset,  clause,  logical_lane);
    return AW_ERR_CODE_NONE; 
}

int aw_pmd_anlt_link_training_init_preset_set (mss_access_t *mss, uint32_t clause, uint32_t init[], uint32_t preset[], uint32_t preset2[]){
    TRACE_ENTER("mss=0x%x, 0x%x, clause = %d", mss->phy_offset, mss->lane_offset,  clause);
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

    TRACE_EXIT("mss=0x%x, 0x%x, clause = %d", mss->phy_offset, mss->lane_offset,  clause);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_link_training_min_max_set (mss_access_t *mss, uint32_t pre1_max, uint32_t main_max, uint32_t post1_max, uint32_t pre1_min, uint32_t main_min, uint32_t post1_min){
    TRACE_ENTER("mss=0x%x, 0x%x, pre1_max = %d, main_max = %d, post1_max = %d, pre1_min = %d, main_min = %d, post1_min = %d", mss->phy_offset, mss->lane_offset,  pre1_max,  main_max,  post1_max,  pre1_min,  main_min,  post1_min);
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CM1_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CM1_MAX_OFFSET, pre1_max));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CP0_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CP0_MAX_OFFSET, main_max + 1));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MAX_ADDR, ETH_LT_TXCOEFF_MAX_LT_CP1_MAX_MASK, ETH_LT_TXCOEFF_MAX_LT_CP1_MAX_OFFSET, post1_max));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CM1_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CM1_MIN_OFFSET, pre1_min));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CP0_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CP0_MIN_OFFSET, main_min + 1));
    CHECK(pmd_write_field(mss, ETH_LT_TXCOEFF_MIN_ADDR, ETH_LT_TXCOEFF_MIN_LT_CP1_MIN_MASK, ETH_LT_TXCOEFF_MIN_LT_CP1_MIN_OFFSET, post1_min));

    TRACE_EXIT("mss=0x%x, 0x%x, pre1_max = %d, main_max = %d, post1_max = %d, pre1_min = %d, main_min = %d, post1_min = %d", mss->phy_offset, mss->lane_offset,  pre1_max,  main_max,  post1_max,  pre1_min,  main_min,  post1_min);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_link_training_start_set (mss_access_t *mss, uint32_t start){
    TRACE_ENTER("mss=0x%x, 0x%x, start = %d", mss->phy_offset, mss->lane_offset,  start);
    CHECK(pmd_write_field(mss, ETH_LT_NO_AN_CTRL_ADDR, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_MASK, ETH_LT_NO_AN_CTRL_LT_WITHOUT_AN_ENA_OFFSET, start));

    TRACE_EXIT("mss=0x%x, 0x%x, start = %d", mss->phy_offset, mss->lane_offset,  start);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_anlt_link_training_status_get (mss_access_t *mss, uint32_t * lt_running, uint32_t * lt_done, uint32_t * lt_training_failure, uint32_t * lt_rx_ready){
    TRACE_ENTER("mss=0x%x, 0x%x, *lt_running = %d, *lt_done = %d, *lt_training_failure = %d, *lt_rx_ready = %d", mss->phy_offset, mss->lane_offset,  *lt_running,  *lt_done,  *lt_training_failure,  *lt_rx_ready);
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RUNNING_MASK, ETH_LT_STAT_LT_RUNNING_OFFSET, lt_running));
    CHECK(pmd_read_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_LT_DONE_MASK, ETH_ANLT_STATUS_LT_DONE_OFFSET, lt_done));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_TRAINING_FAILURE_MASK, ETH_LT_STAT_LT_TRAINING_FAILURE_OFFSET, lt_training_failure));
    CHECK(pmd_read_field(mss, ETH_LT_STAT_ADDR, ETH_LT_STAT_LT_RX_READY_MASK, ETH_LT_STAT_LT_RX_READY_OFFSET, lt_rx_ready));

    TRACE_EXIT("mss=0x%x, 0x%x, *lt_running = %d, *lt_done = %d, *lt_training_failure = %d, *lt_rx_ready = %d", mss->phy_offset, mss->lane_offset,  *lt_running,  *lt_done,  *lt_training_failure,  *lt_rx_ready);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_link_training_timeout_enable_set (mss_access_t *mss, uint32_t enable){
    TRACE_ENTER("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    enable = !enable; 
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136B_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136B_OFFSET, enable));
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C136_OFFSET, enable));
    CHECK(pmd_write_field(mss, ETH_LT_SETTINGS_ADDR, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C72_MASK, ETH_LT_SETTINGS_LT_MAX_WAIT_DISABLE_C72_OFFSET, enable));

    TRACE_EXIT("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_refclk_termination_set(mss_access_t *mss, aw_refclk_term_mode_t lsrefbuf_term_mode){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    CHECK(pmd_write_field(mss, CMN_REFCLK_ADDR, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_MASK, CMN_REFCLK_LSREFBUF_TERM_MODE_NT_OFFSET, lsrefbuf_term_mode ));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_termination_set(mss_access_t *mss, aw_acc_term_mode_t acc_term_mode){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    CHECK(pmd_write_field(mss, AFE_OCTERM_RX_ADDR, AFE_OCTERM_RX_ACC_TERM_MODE_NT_MASK, AFE_OCTERM_RX_ACC_TERM_MODE_NT_OFFSET, acc_term_mode ));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_force_signal_detect_config_set(mss_access_t *mss, aw_force_sigdet_mode_t sigdet_mode){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    switch (sigdet_mode) {
        case AW_SIGDET_FORCE0:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_FORCE1:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 1));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
            return AW_ERR_CODE_NONE;
        case AW_SIGDET_NORM:
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_VALID_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_MASK, RX_SIGNAL_DETECT_REG3_FORCE_INVALID_A_OFFSET, 0));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
            return AW_ERR_CODE_NONE;
        default:
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
            return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
}




int aw_pmd_txfir_config_set(mss_access_t *mss, aw_txfir_config_t *txfir_cfg, uint32_t fir_ovr_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    uint32_t cm3_mask = 0x7;
    uint32_t cm2_mask = 0x7;
    uint32_t cm1_mask = 0x1F;
    uint32_t max_mask = 0x3F;
    uint32_t c1_mask = 0x1F;

    uint32_t max_ele;
    if (txfir_cfg->main_or_max == 0){
        max_ele = txfir_cfg->C1 + txfir_cfg->C0 + txfir_cfg->CM1 + txfir_cfg->CM2 + txfir_cfg->CM3 - 1;
    } else {
        max_ele = txfir_cfg->C0;
    }

    if (txfir_cfg->CM3 > 7){
    USR_PRINTF("ERROR: Exceeds max, CM3 (%d) must be less than or equal to 7 \n", txfir_cfg->CM3);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    } else if (txfir_cfg->CM2 > 7){
    USR_PRINTF("ERROR: Exceeds max, CM2 (%d) must be less than or equal to 7 \n", txfir_cfg->CM2);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    } else if (txfir_cfg->CM1 > 24){
    USR_PRINTF("ERROR: Exceeds max, CM1 (%d) must be less than or equal to  24 \n", txfir_cfg->CM1);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    } else if (txfir_cfg->C0 > 64){
    USR_PRINTF("ERROR: Exceeds max, C0 (%d) must be less than or equal to 64 \n", txfir_cfg->C0);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    } else if (txfir_cfg->C1 > 24){
    USR_PRINTF("ERROR: Exceeds max, C1 (%d) must be less than or equal to 24 \n", txfir_cfg->C1);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    } else if (max_ele > 64){
    USR_PRINTF("ERROR: Exceeds max, parameters must be less than or equal to 64, current value is %d. CM3:%d CM2:%d CM1:%d C0:%d C1:%d \n", max_ele, txfir_cfg->CM3,txfir_cfg->CM2,txfir_cfg->CM1,txfir_cfg->C0,txfir_cfg->C1);
    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_INVALID_ARG_VALUE;
    }   

    uint32_t fir = ((txfir_cfg->C1 & c1_mask) << 17);
    fir = fir + ((max_ele & max_mask) << 11);
    fir = fir + ((txfir_cfg->CM1 & cm1_mask) << 6);
    fir = fir + ((txfir_cfg->CM2 & cm2_mask) << 3);
    fir = fir + (txfir_cfg->CM3 & cm3_mask);

    aw_pmd_txfir_ovr_set(mss, fir_ovr_enable);
    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_VAL_A_MASK, FIR_VAL_A_OFFSET, fir));

    TRACE_EXIT("mss=0x%x, 0x%x, fir_ovr_enable = %d", mss->phy_offset, mss->lane_offset,  fir_ovr_enable);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_txfir_ovr_set(mss_access_t *mss, uint32_t txfir_ovr){
    TRACE_ENTER("mss=0x%x, 0x%x, txfir_ovr = %d", mss->phy_offset, mss->lane_offset,  txfir_ovr);
    CHECK(pmd_write_field(mss, FIR_ADDR, FIR_OVR_EN_A_MASK, FIR_OVR_EN_A_OFFSET, txfir_ovr));
    TRACE_EXIT("mss=0x%x, 0x%x, txfir_ovr = %d", mss->phy_offset, mss->lane_offset,  txfir_ovr);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_txfir_config_get(mss_access_t *mss, aw_txfir_config_t *txfir_cfg){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    int cm3_mask = 0x00000007;
    int cm2_mask = 0x00000038;
    int cm1_mask = 0x000007c0;
    int max_mask = 0x0001f800;
    int c1_mask =  0x003e0000;

    uint32_t rdval;
    CHECK(pmd_read_field(mss, TX_FIR_RDREG_ADDR, TX_FIR_RDREG_AFE_VAL_NT_MASK, TX_FIR_RDREG_AFE_VAL_NT_OFFSET, &rdval));
    txfir_cfg->CM3 = (rdval & cm3_mask);
    txfir_cfg->CM2 = (rdval & cm2_mask) >> 3;
    txfir_cfg->CM1 = (rdval & cm1_mask) >> 6;
    int32_t max_ele = (rdval & max_mask) >> 11; 
    txfir_cfg->C1  = (rdval & c1_mask)  >> 17;
    if (txfir_cfg->main_or_max == 0){
        txfir_cfg->C0 = max_ele + 1 - txfir_cfg->C1 - txfir_cfg->CM1 - txfir_cfg->CM2 - txfir_cfg->CM3;
    } else {
        txfir_cfg->C0 = max_ele;
    }
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_MASK, TX_DATAPATH_REG2_PAMCODE_OVR_EN_A_OFFSET, en));
    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    TRACE_ENTER("mss=0x%x, 0x%x, gray_en = %d, plusd_en = %d", mss->phy_offset, mss->lane_offset,  gray_en,  plusd_en);
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_MASK, TX_DATAPATH_REG1_GRAY_CODE_ENABLE_A_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_PLUSD_ENABLE_A_MASK, TX_DATAPATH_REG1_PLUSD_ENABLE_A_OFFSET, plusd_en));
    TRACE_EXIT("mss=0x%x, 0x%x, gray_en = %d, plusd_en = %d", mss->phy_offset, mss->lane_offset,  gray_en,  plusd_en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_pam4_precoder_override_set(mss_access_t *mss, uint32_t en){
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    if (en == 1) {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 0));
    } else {
        CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_OVR_NT_OFFSET, 1));
    }
    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_rx_pam4_precoder_enable_set(mss_access_t *mss, uint32_t gray_en, uint32_t plusd_en){
    TRACE_ENTER("mss=0x%x, 0x%x, gray_en = %d, plusd_en = %d", mss->phy_offset, mss->lane_offset,  gray_en,  plusd_en);
    CHECK(pmd_write_field(mss, RX_CNTRL_REG2_ADDR, RX_CNTRL_REG2_RX_GRAY_ENA_NT_MASK, RX_CNTRL_REG2_RX_GRAY_ENA_NT_OFFSET, gray_en));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_PLUSD_ENABLE_A_MASK, RX_DEMAPPER_PLUSD_ENABLE_A_OFFSET, plusd_en));
    TRACE_EXIT("mss=0x%x, 0x%x, gray_en = %d, plusd_en = %d", mss->phy_offset, mss->lane_offset,  gray_en,  plusd_en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_remote_loopback_set(mss_access_t *mss, uint32_t remote_loopback_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, remote_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  remote_loopback_enable);
    

    (void)*mss;
    (void)remote_loopback_enable;
    USR_PRINTF("ERROR: aw_pmd_remote_loopback_set - function implementation has been deprecated, use aw_pmd_fep_clock_set & aw_pmd_fep_data_set\n");
    TRACE_EXIT("mss=0x%x, 0x%x, remote_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  remote_loopback_enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_fep_data_set(mss_access_t *mss, uint32_t datapath_en) {
    TRACE_ENTER("mss=0x%x, 0x%x, datapath_en = %d", mss->phy_offset, mss->lane_offset,  datapath_en);
    
    
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, datapath_en));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, datapath_en));

    TRACE_EXIT("mss=0x%x, 0x%x, datapath_en = %d", mss->phy_offset, mss->lane_offset,  datapath_en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_dcd_iq_cal(mss_access_t *mss, uint32_t enable_d)
{
    TRACE_ENTER("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
    int32_t poll_result;
    
    if (enable_d > 1){
        
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
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
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 0, 1000);
    if (poll_result == -1) {
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_TYPE_NT_MASK, TX_PHASE_ADAPT_REG2_TYPE_NT_OFFSET, 2));
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    CHECK(pmd_write_field(mss, TX_PHASE_ADAPT_REG2_ADDR, TX_PHASE_ADAPT_REG2_REQ_A_MASK, TX_PHASE_ADAPT_REG2_REQ_A_OFFSET, 0));
    poll_result = pmd_poll_field(mss, TX_PHASE_ADAPT_RDREG_ADDR, TX_PHASE_ADAPT_RDREG_ACK_MASK, TX_PHASE_ADAPT_RDREG_ACK_OFFSET, 1, 1000);
    if (poll_result == -1) {
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    
    CHECK(pmd_write_field(mss, PD_AFE_TX_ADDR, PD_AFE_TX_DCDIQ_BA_MASK, PD_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, RST_AFE_TX_ADDR, RST_AFE_TX_DCDIQ_BA_MASK, RST_AFE_TX_DCDIQ_BA_OFFSET, 1));
    CHECK(pmd_write_field(mss, TX_FEP_LOOPBACK_FIFO_TOP_ADDR, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_MASK, TX_FEP_LOOPBACK_FIFO_TOP_FIFO_ENABLE_A_OFFSET, enable_d));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_MASK, TX_DATAPATH_REG1_FEP_LOOPBACK_ENABLE_A_OFFSET, enable_d));
    TRACE_EXIT("mss=0x%x, 0x%x, enable_d = %d", mss->phy_offset, mss->lane_offset,  enable_d);
    return AW_ERR_CODE_NONE;        
}

int aw_pmd_fep_clock_set(mss_access_t *mss, uint8_t clock_en) {
    TRACE_ENTER("mss=0x%x, 0x%x, clock_en = %d", mss->phy_offset, mss->lane_offset,  clock_en);
    CHECK(pmd_write_field(mss, TX_LOOPBACK_CNTRL_ADDR, TX_LOOPBACK_CNTRL_ENA_NT_MASK, TX_LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, clock_en));
    
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_TX_ADDR, SEQ_CNTRL_TX_POSTDIV_READY_A_MASK, SEQ_CNTRL_TX_POSTDIV_READY_A_OFFSET, 1));
    
    if (clock_en != 0xff){
        aw_pmd_tx_dcd_iq_cal(mss, clock_en);    
    }
    TRACE_EXIT("mss=0x%x, 0x%x, clock_en = %d", mss->phy_offset, mss->lane_offset,  clock_en);
    return AW_ERR_CODE_NONE;
}





int aw_pmd_analog_loopback_set(mss_access_t *mss, uint32_t analog_loopback_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, analog_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  analog_loopback_enable);
    
    
    
    
    

    
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

    TRACE_EXIT("mss=0x%x, 0x%x, analog_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  analog_loopback_enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_fes_loopback_set(mss_access_t *mss, uint32_t fes_loopback_enable) {
    TRACE_ENTER("mss=0x%x, 0x%x, fes_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  fes_loopback_enable);
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_ENA_NT_MASK, LOOPBACK_CNTRL_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_BITCK_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_RX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    CHECK(pmd_write_field(mss, LOOPBACK_CNTRL_ADDR, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_MASK, LOOPBACK_CNTRL_TX_FES_LOOPBACK_ENA_NT_OFFSET, fes_loopback_enable));
    TRACE_EXIT("mss=0x%x, 0x%x, fes_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  fes_loopback_enable);
    return AW_ERR_CODE_NONE;
}




int aw_pmd_nep_loopback_set(mss_access_t *mss, uint32_t nep_loopback_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, nep_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  nep_loopback_enable);
    
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG5_ADDR, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_MASK, RX_ITR_DPLL_DLPF_REG5_INT_BYPASS_NT_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_ITR_DPLL_DLPF_REG3_ADDR, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_MASK, RX_ITR_DPLL_DLPF_REG3_BYPASS_ENA_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVR_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, RX_SIGNAL_DETECT_REG3_ADDR, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_MASK, RX_SIGNAL_DETECT_REG3_DPLL_ADAPT_START_OVREN_A_OFFSET, nep_loopback_enable));
    CHECK(pmd_write_field(mss, SEQ_CNTRL_RX_ADDR, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_MASK, SEQ_CNTRL_RX_NEP_LOOPBACK_ENA_A_OFFSET, nep_loopback_enable));
    TRACE_EXIT("mss=0x%x, 0x%x, nep_loopback_enable = %d", mss->phy_offset, mss->lane_offset,  nep_loopback_enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_polarity_set(mss_access_t *mss, uint32_t tx_pol_flip){
    TRACE_ENTER("mss=0x%x, 0x%x, tx_pol_flip = %d", mss->phy_offset, mss->lane_offset,  tx_pol_flip);
    
    
    
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_INVERT_ENABLE_A_MASK, TX_DATAPATH_REG2_INVERT_ENABLE_A_OFFSET, tx_pol_flip));
    TRACE_EXIT("mss=0x%x, 0x%x, tx_pol_flip = %d", mss->phy_offset, mss->lane_offset,  tx_pol_flip);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_polarity_set(mss_access_t *mss, uint32_t rx_pol_flip){
    TRACE_ENTER("mss=0x%x, 0x%x, rx_pol_flip = %d", mss->phy_offset, mss->lane_offset,  rx_pol_flip);
    
    
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_INVERT_ENABLE_A_MASK, RX_DEMAPPER_INVERT_ENABLE_A_OFFSET, rx_pol_flip));
    TRACE_EXIT("mss=0x%x, 0x%x, rx_pol_flip = %d", mss->phy_offset, mss->lane_offset,  rx_pol_flip);
    return AW_ERR_CODE_NONE;
}











int aw_pmd_rx_dfe_adapt_set(mss_access_t *mss, uint32_t dfe_adapt_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, dfe_adapt_enable = %d", mss->phy_offset, mss->lane_offset,  dfe_adapt_enable);
    CHECK(pmd_write_field(mss, RX_EQDFE_ADDR, RX_EQDFE_DFE_ENABLE_NT_MASK, RX_EQDFE_DFE_ENABLE_NT_OFFSET, dfe_adapt_enable));
    TRACE_EXIT("mss=0x%x, 0x%x, dfe_adapt_enable = %d", mss->phy_offset, mss->lane_offset,  dfe_adapt_enable);
    return AW_ERR_CODE_NONE;
}














int aw_pmd_rx_background_adapt_enable_set(mss_access_t *mss, uint32_t rx_background_adapt)
{
    TRACE_ENTER("mss=0x%x, 0x%x, rx_background_adapt = %d", mss->phy_offset, mss->lane_offset,  rx_background_adapt);
    if (rx_background_adapt == 1){
        CHECK(pmd_write_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_MASK, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_OFFSET, 0));
    }
    else {
        CHECK(pmd_write_field(mss, RXMFSM_CTRL_ADDR, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_MASK, RXMFSM_CTRL_RXMFSM_EQBK_POWER_STATE_OFFSET, 7));
    }
    TRACE_EXIT("mss=0x%x, 0x%x, rx_background_adapt = %d", mss->phy_offset, mss->lane_offset,  rx_background_adapt);
    return AW_ERR_CODE_NONE;
}















int aw_pmd_rxeq_prbs_set(mss_access_t *mss, uint32_t prbs_en) {
    TRACE_ENTER("mss=0x%x, 0x%x, prbs_en = %d", mss->phy_offset, mss->lane_offset,  prbs_en);
    CHECK(pmd_write_field(mss, RXEQ_PRBS_ADDR, RXEQ_PRBS_MASK, RXEQ_PRBS_OFFSET, prbs_en));
    TRACE_EXIT("mss=0x%x, 0x%x, prbs_en = %d", mss->phy_offset, mss->lane_offset,  prbs_en);
    return AW_ERR_CODE_NONE;
}

















int aw_pmd_rx_chk_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, aw_bist_mode_t mode, uint64_t udp_63_0, uint64_t udp_127_64, uint32_t lock_thresh, uint32_t timer_thresh){
    TRACE_ENTER("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld, lock_thresh = %d, timer_thresh = %d", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64,  lock_thresh,  timer_thresh);
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));

    uint32_t udp_31_0 = 0;
    uint32_t udp_63_32 = 0;
    uint32_t udp_95_64 = 0;
    uint32_t udp_127_96 = 0;
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
        udp_31_0  = (uint32_t)(udp_63_0 & 0xFFFFFFFF);
        udp_63_32 = (uint32_t)((udp_63_0 >> 32) & 0xFFFFFFFF);
        udp_95_64  = (uint32_t)(udp_127_64 & 0xFFFFFFFF);
        udp_127_96 = (uint32_t)((udp_127_64 >> 32) & 0xFFFFFFFF);
        udp_en = 1;
        break;
    case AW_FULL_RATE_CLOCK:
        udp_31_0  = 0xAAAAAAAA;
        udp_63_32 = 0xAAAAAAAA;
        udp_95_64 = 0xAAAAAAAA;
        udp_127_96 = 0xAAAAAAAA;
        udp_en = 1;
        break;
    case AW_HALF_RATE_CLOCK:
        udp_31_0  = 0xCCCCCCCC;
        udp_63_32 = 0xCCCCCCCC;
        udp_95_64 = 0xCCCCCCCC;
        udp_127_96 = 0xCCCCCCCC;
        udp_en = 1;
        break;
    case AW_QUARTER_RATE_CLOCK:
        udp_31_0  = 0xF0F0F0F0;
        udp_63_32 = 0xF0F0F0F0;
        udp_95_64 = 0xF0F0F0F0;
        udp_127_96 = 0xF0F0F0F0;
        udp_en = 1;
        break;
    case AW_PATT_32_1S_32_0S:
        udp_31_0  = 0x00000000;
        udp_63_32 = 0xFFFFFFFF;
        udp_95_64 = 0x00000000;
        udp_127_96 = 0xFFFFFFFF;
        udp_en = 1;
        break;
    default:
    TRACE_EXIT("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld, lock_thresh = %d, timer_thresh = %d", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64,  lock_thresh,  timer_thresh);
        return AW_ERR_CODE_INVALID_ARG_VALUE;
    }
    if (udp_en) {
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG3_ADDR, RX_DATABIST_TOP_REG3_PATTERN_LENGTH_NT_MASK, RX_DATABIST_TOP_REG3_PATTERN_LENGTH_NT_OFFSET, 0x80));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG2_ADDR, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_MASK, RX_DATABIST_TOP_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG4_ADDR, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_MASK, RX_DATABIST_TOP_REG4_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG5_ADDR, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_MASK, RX_DATABIST_TOP_REG5_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG6_ADDR, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_MASK, RX_DATABIST_TOP_REG6_UDP_PATTERN_95_64_NT_OFFSET, udp_95_64));
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG7_ADDR, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_MASK, RX_DATABIST_TOP_REG7_UDP_PATTERN_127_96_NT_OFFSET, udp_127_96));
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
    TRACE_EXIT("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld, lock_thresh = %d, timer_thresh = %d", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64,  lock_thresh,  timer_thresh);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_chk_en_set(mss_access_t *mss, uint32_t enable){
    TRACE_ENTER("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    
    
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, enable));
    TRACE_EXIT("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_rx_chk_lock_state_get(mss_access_t *mss, uint32_t *rx_bist_lock){
    TRACE_ENTER("mss=0x%x, 0x%x, *rx_bist_lock = %d", mss->phy_offset, mss->lane_offset,  *rx_bist_lock);
    
    
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_LOCKED_NT_MASK, RX_DATABIST_TOP_RDREG1_LOCKED_NT_OFFSET, rx_bist_lock));
    TRACE_EXIT("mss=0x%x, 0x%x, *rx_bist_lock = %d", mss->phy_offset, mss->lane_offset,  *rx_bist_lock);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_chk_err_count_state_get(mss_access_t *mss, uint64_t *err_count, uint32_t *err_count_done, uint32_t *err_count_overflown){
    TRACE_ENTER("mss=0x%x, 0x%x, *err_count = %ld, *err_count_done = %d, *err_count_overflown = %d", mss->phy_offset, mss->lane_offset,  *err_count,  *err_count_done,  *err_count_overflown);
    uint32_t bist_mode;
    uint32_t err_cnt_55_32;
    uint32_t err_cnt_31_0;
    uint32_t err_code;
    uint32_t bist_enable;
    CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_MODE_NT_MASK, RX_DATABIST_TOP_REG1_BIST_MODE_NT_OFFSET, &bist_mode));
    err_code = 0;
    if (bist_mode == 1){ 
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, &bist_enable));
        if (bist_enable == 1){
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0 ));
            if (*err_count_overflown){
                err_code = 1; 
                USR_PRINTF("ERROR: BIST error counter overflown.");
            } else {
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
                *err_count = (uint64_t)err_cnt_55_32 << 32 | (uint64_t)err_cnt_31_0;
            }
            *err_count_done = 1;
        } else {
            err_code = 2; 
            USR_PRINTF("ERROR: BIST is not enabled.");
        }
    } else { 
        CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, RD_EQ, err_count_done, 1, 0 ));
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
        CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
        *err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
        CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, err_count_overflown, 0, 0 ));
        if (*err_count_done == 0) { 
            *err_count_overflown = 0;
            USR_PRINTF("RX BIST timer has not completed yet.");
        } else { 
            *err_count_done = 1;
        }
    }
    if (err_code != 0){
    TRACE_EXIT("mss=0x%x, 0x%x, *err_count = %ld, *err_count_done = %d, *err_count_overflown = %d", mss->phy_offset, mss->lane_offset,  *err_count,  *err_count_done,  *err_count_overflown);
        return AW_ERR_CODE_FUNC_FAILURE;
    } else {
    TRACE_EXIT("mss=0x%x, 0x%x, *err_count = %ld, *err_count_done = %d, *err_count_overflown = %d", mss->phy_offset, mss->lane_offset,  *err_count,  *err_count_done,  *err_count_overflown);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_chk_err_count_state_clear(mss_access_t *mss){
    TRACE_ENTER("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
    TRACE_EXIT("mss=0x%x, 0x%x", mss->phy_offset, mss->lane_offset);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_enable_pam4_mode(mss_access_t *mss, int enable){
    TRACE_ENTER("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_PAM4_MODE_A_MASK, TX_DATAPATH_REG1_PAM4_MODE_A_OFFSET, enable));
    TRACE_EXIT("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_tx_gen_config_set(mss_access_t *mss, aw_bist_pattern_t pattern, uint64_t udp_63_0, uint64_t udp_127_64){
    TRACE_ENTER("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64);
    uint32_t udp_31_0;
    uint32_t udp_63_32;
    uint32_t udp_95_64;
    uint32_t udp_127_96;
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
            udp_31_0  = (uint32_t)(udp_63_0 & 0xFFFFFFFF);
            udp_63_32 = (uint32_t)((udp_63_0 >> 32) & 0xFFFFFFFF);
            udp_95_64 = (uint32_t)(udp_127_64 & 0xFFFFFFFF);
            udp_127_96 = (uint32_t)((udp_127_64 >> 32) & 0xFFFFFFFF);
            udp_en = 1;
            break;
        case AW_FULL_RATE_CLOCK:
            udp_31_0 = 0xAAAAAAAA;
            udp_63_32 = 0xAAAAAAAA;
            udp_95_64 = 0xAAAAAAAA;
            udp_127_96 = 0xAAAAAAAA;
            udp_en = 1;
            break;
        case AW_HALF_RATE_CLOCK:
            udp_31_0 = 0xCCCCCCCC;
            udp_63_32 = 0xCCCCCCCC;
            udp_95_64 = 0xCCCCCCCC;
            udp_127_96 = 0xCCCCCCCC;
            udp_en = 1;
            break;
        case AW_QUARTER_RATE_CLOCK:
            udp_31_0 = 0xF0F0F0F0;
            udp_63_32 = 0xF0F0F0F0;
            udp_95_64 = 0xF0F0F0F0;
            udp_127_96 = 0xF0F0F0F0;
            udp_en = 1;
            break;
        case AW_PATT_32_1S_32_0S:
            udp_31_0 = 0x00000000;
            udp_63_32 = 0xFFFFFFFF;
            udp_95_64 = 0x00000000;
            udp_127_96 = 0xFFFFFFFF;
            udp_en = 1;
            break;
        default:
    TRACE_EXIT("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64);
            return AW_ERR_CODE_INVALID_ARG_VALUE; 
    }

    if (udp_en) {
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG7_ADDR, TX_DATAPATH_REG7_PATTERN_LENGTH_NT_MASK, TX_DATAPATH_REG7_PATTERN_LENGTH_NT_OFFSET, 0x80));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG2_ADDR, TX_DATAPATH_REG2_PATTERN_SEL_NT_MASK, TX_DATAPATH_REG2_PATTERN_SEL_NT_OFFSET, AW_USER_DEFINED_PATTERN));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG11_ADDR, TX_DATAPATH_REG11_UDP_PATTERN_127_96_NT_MASK, TX_DATAPATH_REG11_UDP_PATTERN_127_96_NT_OFFSET, udp_127_96));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG10_ADDR, TX_DATAPATH_REG10_UDP_PATTERN_95_64_NT_MASK, TX_DATAPATH_REG10_UDP_PATTERN_95_64_NT_OFFSET, udp_95_64));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG9_ADDR, TX_DATAPATH_REG9_UDP_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG9_UDP_PATTERN_63_32_NT_OFFSET, udp_63_32));
        CHECK(pmd_write_field(mss, TX_DATAPATH_REG8_ADDR, TX_DATAPATH_REG8_UDP_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG8_UDP_PATTERN_31_0_NT_OFFSET, udp_31_0));
    }
    TRACE_EXIT("mss=0x%x, 0x%x, udp_63_0 = %ld, udp_127_64 = %ld", mss->phy_offset, mss->lane_offset,  udp_63_0,  udp_127_64);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_gen_en_set(mss_access_t *mss, uint32_t enable){
    TRACE_ENTER("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    
    
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, enable));
    TRACE_EXIT("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_gen_err_inject_config_set(mss_access_t *mss, uint64_t err_pattern, uint32_t err_rate){
    TRACE_ENTER("mss=0x%x, 0x%x, err_pattern = %ld, err_rate = %d", mss->phy_offset, mss->lane_offset,  err_pattern,  err_rate);
    
    
    
    
    uint32_t err_pattern_31_0 = err_pattern & 0xFFFFFFFF;
    uint32_t err_pattern_63_32 = err_pattern >> 32 & 0xFFFFFFFF;
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG15_ADDR, TX_DATAPATH_REG15_ERROR_PATTERN_63_32_NT_MASK, TX_DATAPATH_REG15_ERROR_PATTERN_63_32_NT_OFFSET, err_pattern_63_32));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG14_ADDR, TX_DATAPATH_REG14_ERROR_PATTERN_31_0_NT_MASK, TX_DATAPATH_REG14_ERROR_PATTERN_31_0_NT_OFFSET, err_pattern_31_0));
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_RATE_NT_OFFSET, err_rate));
    TRACE_EXIT("mss=0x%x, 0x%x, err_pattern = %ld, err_rate = %d", mss->phy_offset, mss->lane_offset,  err_pattern,  err_rate);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_gen_err_inject_en_set(mss_access_t *mss, uint32_t enable){
    TRACE_ENTER("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_MASK, TX_DATAPATH_REG1_TXDATA_ERROR_ENABLE_A_OFFSET, enable));
    TRACE_EXIT("mss=0x%x, 0x%x, enable = %d", mss->phy_offset, mss->lane_offset,  enable);
    return AW_ERR_CODE_NONE;
}













int aw_pmd_uc_ucode_load(mss_access_t *mss, aw_ucode_t *ucode, uint32_t ucode_len){
    TRACE_ENTER("mss=0x%x, 0x%x, ucode_len = %d", mss->phy_offset, mss->lane_offset,  ucode_len);
    for (uint32_t i = 0; i<ucode_len; i++){
        CHECK(pmd_write_addr(mss, ucode[i].address, ucode[i].value)); 
    }
    TRACE_EXIT("mss=0x%x, 0x%x, ucode_len = %d", mss->phy_offset, mss->lane_offset,  ucode_len);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_pll_lock_min_set(mss_access_t *mss, uint32_t val){
    TRACE_ENTER("mss=0x%x, 0x%x, val = %d", mss->phy_offset, mss->lane_offset,  val);
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_MIN_NT_MASK, LCPLL_CHECK_MIN_NT_OFFSET, val));
    TRACE_EXIT("mss=0x%x, 0x%x, val = %d", mss->phy_offset, mss->lane_offset,  val);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_pll_lock_get(mss_access_t *mss, uint32_t *pll_lock ,uint32_t check_en, uint32_t expected_val){
    TRACE_ENTER("mss=0x%x, 0x%x, *pll_lock = %d, check_en = %d, expected_val = %d", mss->phy_offset, mss->lane_offset,  *pll_lock,  check_en,  expected_val);
    int poll_result;
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 1));
    poll_result = pmd_poll_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_DONE_A_MASK, LCPLL_CHECK_RDREG_DONE_A_OFFSET, 1, 400);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Polling for LC PLL Lock check done\n");
    TRACE_EXIT("mss=0x%x, 0x%x, *pll_lock = %d, check_en = %d, expected_val = %d", mss->phy_offset, mss->lane_offset,  *pll_lock,  check_en,  expected_val);
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("LC PLL Lock check done\n");
        if (check_en) {
            CHECK(pmd_read_check_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, RD_EQ, pll_lock, expected_val, 0 ));
        } else {
            CHECK(pmd_read_field(mss, LCPLL_CHECK_RDREG_ADDR, LCPLL_CHECK_RDREG_STAT_NT_MASK, LCPLL_CHECK_RDREG_STAT_NT_OFFSET, pll_lock));
        }
    }
    CHECK(pmd_write_field(mss, LCPLL_CHECK_ADDR, LCPLL_CHECK_START_A_MASK, LCPLL_CHECK_START_A_OFFSET, 0));
    TRACE_EXIT("mss=0x%x, 0x%x, *pll_lock = %d, check_en = %d, expected_val = %d", mss->phy_offset, mss->lane_offset,  *pll_lock,  check_en,  expected_val);
    return AW_ERR_CODE_NONE;
}





int aw_pmd_iso_cmn_pstate_set(mss_access_t *mss, uint32_t value){
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_pstate_get(mss_access_t *mss, uint32_t *value){
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_state_req_set(mss_access_t *mss, uint32_t value){
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_MASK, DIG_SOC_CMN_OVRD_ICTL_PCLK_STATE_REQ_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_cmn_state_ack_get(mss_access_t *mss, uint32_t *cmn_state_ack){
    TRACE_ENTER("mss=0x%x, 0x%x, *cmn_state_ack = %d", mss->phy_offset, mss->lane_offset,  *cmn_state_ack);
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_STAT_ADDR, DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_MASK,  DIG_SOC_CMN_STAT_OCTL_PCLK_STATE_ACK_OFFSET, cmn_state_ack));
    TRACE_EXIT("mss=0x%x, 0x%x, *cmn_state_ack = %d", mss->phy_offset, mss->lane_offset,  *cmn_state_ack);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_reset_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_reset_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_TX_BA_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_reset_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_reset_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG4_ADDR, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_MASK, DIG_SOC_LANE_OVRD_REG4_IRST_RX_BA_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_rate_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_rate_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_TX_STATE_RATE_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_rate_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_rate_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_RATE_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_pstate_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_pstate_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_pstate_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_pstate_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_POWER_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_width_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_width_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_TX_STATE_WIDTH_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_width_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_width_get(mss_access_t *mss, uint32_t *value) {
    TRACE_ENTER("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_STATE_WIDTH_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, *value = %d", mss->phy_offset, mss->lane_offset,  *value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_state_req_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_TX_STATE_REQ_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_state_req_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG3_ADDR, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG3_ICTL_RX_STATE_REQ_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_tx_state_ack_get(mss_access_t *mss, uint32_t *tx_state_ack) {
    TRACE_ENTER("mss=0x%x, 0x%x, *tx_state_ack = %d", mss->phy_offset, mss->lane_offset,  *tx_state_ack);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_TX_STATE_ACK_OFFSET, tx_state_ack));
    TRACE_EXIT("mss=0x%x, 0x%x, *tx_state_ack = %d", mss->phy_offset, mss->lane_offset,  *tx_state_ack);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_iso_rx_state_ack_get(mss_access_t *mss, uint32_t *rx_state_ack) {
    TRACE_ENTER("mss=0x%x, 0x%x, *rx_state_ack = %d", mss->phy_offset, mss->lane_offset,  *rx_state_ack);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_STATE_ACK_OFFSET, rx_state_ack));
    TRACE_EXIT("mss=0x%x, 0x%x, *rx_state_ack = %d", mss->phy_offset, mss->lane_offset,  *rx_state_ack);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_cmn_set(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CTRL_CLK_A_MASK, DIG_SOC_CMN_OVRD_CTRL_CLK_A_OFFSET, en));
    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_cmn_get(mss_access_t *mss, uint32_t *en) {
    TRACE_ENTER("mss=0x%x, 0x%x, *en = %d", mss->phy_offset, mss->lane_offset,  *en);
    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_CMN_OVRD_EN_A_OFFSET, en));
    TRACE_EXIT("mss=0x%x, 0x%x, *en = %d", mss->phy_offset, mss->lane_offset,  *en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_set(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);

    
    aw_pmd_isolate_lane_tx_set(mss, en);
    aw_pmd_isolate_lane_rx_set(mss, en);
    aw_pmd_isolate_lane_txrx_set(mss, en);

    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_tx_set(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG1_ADDR, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_MASK, DIG_SOC_LANE_OVRD_REG1_ICTL_CK_TX_BLOCK_DATA_ENA_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_TXSOC_CLK_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXBEACON_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXLEVEL_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXFIR_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXRXDET_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TXDATA_OVRD_EN_A_OFFSET, en));

    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_rx_set(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXTERM_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXDISABLE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXSTATE_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXLINKEVAL_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RXMARGIN_OVRD_EN_A_OFFSET, en));

    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_isolate_lane_txrx_set(mss_access_t *mss, uint32_t en) {
    TRACE_ENTER("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_MASK, DIG_SOC_LANE_OVRD_REG5_CTRL_CLK_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_DME_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_PCIEL1_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_BYPASS_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_RESET_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_LOOPBACK_OVRD_EN_A_OFFSET, en));
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_ETH_AN_CTRL_OVRD_EN_A_OFFSET, en));

    TRACE_EXIT("mss=0x%x, 0x%x, en = %d", mss->phy_offset, mss->lane_offset,  en);
    return AW_ERR_CODE_NONE;
}






















int aw_pmd_isolate_lane_get(mss_access_t *mss, uint32_t *en) {
    TRACE_ENTER("mss=0x%x, 0x%x, *en = %d", mss->phy_offset, mss->lane_offset,  *en);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_OVRD_REG5_ADDR, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_MASK, DIG_SOC_LANE_OVRD_REG5_LANE_TX_BLOCK_DATA_ENA_OVRD_EN_A_OFFSET, en));
    TRACE_EXIT("mss=0x%x, 0x%x, *en = %d", mss->phy_offset, mss->lane_offset,  *en);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_cmn_r2l0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);

    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);

    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_cmn_l2r0_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);

    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r1_lsref_sel_set(mss_access_t *mss, uint32_t sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);

    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_OFFSET , sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x, sel = %d", mss->phy_offset, mss->lane_offset,  sel);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_cmn_r2l0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);

    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L0_LSREF_SELECT_NT_OFFSET, sel));
    TRACE_EXIT("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_r2l1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);

    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_R2L1_LSREF_SELECT_NT_OFFSET, sel));
    TRACE_EXIT("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_cmn_l2r0_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);

    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R0_LSREF_SELECT_NT_OFFSET , sel));
    TRACE_EXIT("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_l2r1_lsref_sel_get(mss_access_t *mss, uint32_t *sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);

    CHECK(pmd_read_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_L2R1_LSREF_SELECT_NT_OFFSET, sel));
    TRACE_EXIT("mss=0x%x, 0x%x, *sel = %d", mss->phy_offset, mss->lane_offset,  *sel);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_cmn_lsref_sel_set(mss_access_t *mss, uint32_t ref_sel) {
    TRACE_ENTER("mss=0x%x, 0x%x, ref_sel = %d", mss->phy_offset, mss->lane_offset,  ref_sel);

    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_ICTL_LSREF_SELECT_NT_MASK,DIG_SOC_CMN_OVRD_ICTL_LSREF_SELECT_NT_OFFSET, ref_sel));
    CHECK(pmd_write_field(mss, DIG_SOC_CMN_OVRD_ADDR, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_MASK, DIG_SOC_CMN_OVRD_REF_SEL_OVRD_EN_A_OFFSET, 1));
    TRACE_EXIT("mss=0x%x, 0x%x, ref_sel = %d", mss->phy_offset, mss->lane_offset,  ref_sel);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_pcie_cmn_lsref_25m_get(mss_access_t *mss, uint32_t *lsref_25m) {
    TRACE_ENTER("mss=0x%x, 0x%x, *lsref_25m = %d", mss->phy_offset, mss->lane_offset,  *lsref_25m);
    CHECK(pmd_read_field(mss, SWITCHCLK_DBE_CMN_ADDR, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_MASK, SWITCHCLK_DBE_CMN_REFCLK_SEL_NT_OFFSET, lsref_25m));
    TRACE_EXIT("mss=0x%x, 0x%x, *lsref_25m = %d", mss->phy_offset, mss->lane_offset,  *lsref_25m);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_gen_tx_en_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_BIST_ENABLE_A_MASK, TX_DATAPATH_REG1_BIST_ENABLE_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}



int aw_pmd_iso_request_cmn_state_change(mss_access_t *mss, aw_cmn_pstate_t cmn_pstate, uint32_t timeout_us) {
    TRACE_ENTER("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
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
    TRACE_EXIT("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
    TRACE_EXIT("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_iso_request_tx_state_change(mss_access_t *mss, aw_pstate_t tx_pstate, uint32_t tx_rate, uint32_t tx_width, uint32_t timeout_us) {
    TRACE_ENTER("mss=0x%x, 0x%x, tx_pstate = %d, tx_rate = %d, tx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  tx_pstate,  tx_rate,  tx_width,  timeout_us);
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
    TRACE_EXIT("mss=0x%x, 0x%x, tx_pstate = %d, tx_rate = %d, tx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  tx_pstate,  tx_rate,  tx_width,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
    TRACE_EXIT("mss=0x%x, 0x%x, tx_pstate = %d, tx_rate = %d, tx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  tx_pstate,  tx_rate,  tx_width,  timeout_us);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_iso_request_rx_state_change(mss_access_t *mss, aw_pstate_t rx_pstate, uint32_t rx_rate, uint32_t rx_width, uint32_t timeout_us) {
    TRACE_ENTER("mss=0x%x, 0x%x, rx_pstate = %d, rx_rate = %d, rx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  rx_pstate,  rx_rate,  rx_width,  timeout_us);
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
    TRACE_EXIT("mss=0x%x, 0x%x, rx_pstate = %d, rx_rate = %d, rx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  rx_pstate,  rx_rate,  rx_width,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
    TRACE_EXIT("mss=0x%x, 0x%x, rx_pstate = %d, rx_rate = %d, rx_width = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  rx_pstate,  rx_rate,  rx_width,  timeout_us);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_check_cdr_lock(mss_access_t *mss, uint32_t timeout_us) {
    TRACE_ENTER("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
    int poll_result;
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, 1, timeout_us);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: RX CDR timed out waiting for lock\n");
    TRACE_EXIT("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;

    } else {
        USR_PRINTF("RX CDR is locked\n");
    TRACE_EXIT("mss=0x%x, 0x%x, timeout_us = %d", mss->phy_offset, mss->lane_offset,  timeout_us);
        return AW_ERR_CODE_NONE;
    }
}

int aw_pmd_rx_check_bist(mss_access_t *mss, aw_bist_mode_t bist_mode, uint32_t timer_threshold, uint32_t rx_width, uint32_t timeout_us, int32_t expected_errors, aw_dwell_params_t *dwell_params) {
    TRACE_ENTER("mss=0x%x, 0x%x, timer_threshold = %d, rx_width = %d, timeout_us = %d, expected_errors = %d", mss->phy_offset, mss->lane_offset,  timer_threshold,  rx_width,  timeout_us,  expected_errors);
    uint32_t err_count_overflow, err_cnt_55_32, err_cnt_31_0;
    uint64_t err_count;
    uint32_t width;
    double ber;
    uint32_t dwell_loop;

    int poll_result;
    if (bist_mode == AW_TIMER) {
        
        poll_result = pmd_poll_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_DONE_NT_OFFSET, 1, timeout_us);

        if (poll_result == -1) {
            USR_PRINTF("ERROR: Timed out waiting for error_cnt_done\n");
    TRACE_EXIT("mss=0x%x, 0x%x, timer_threshold = %d, rx_width = %d, timeout_us = %d, expected_errors = %d", mss->phy_offset, mss->lane_offset,  timer_threshold,  rx_width,  timeout_us,  expected_errors);
            return AW_ERR_CODE_POLL_TIMEOUT;

        } else {
            if (expected_errors == -1) {
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, RD_EQ, &err_cnt_55_32, 0, 0 ));
                CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
            } else {
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, RD_EQ, &err_cnt_55_32, 0, 0 ));
                CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, RD_EQ, &err_cnt_31_0, (uint32_t)expected_errors, 0 ));
            }
            
            
            err_count = (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
            (void)err_count;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, &err_count_overflow, 0, 0 ));
            width = aw_width_decoder(rx_width);
            ber = (double) err_count / (width * timer_threshold);
            (void)ber;
            USR_PRINTF("err_count = %" PRIu64 "\n", err_count);
            USR_PRINTF("err_count_overflow = %d\n", err_count_overflow);
            USR_PRINTF("ber = %e\n", ber);

    TRACE_EXIT("mss=0x%x, 0x%x, timer_threshold = %d, rx_width = %d, timeout_us = %d, expected_errors = %d", mss->phy_offset, mss->lane_offset,  timer_threshold,  rx_width,  timeout_us,  expected_errors);
            return AW_ERR_CODE_NONE;
        }
    }
    if (bist_mode == AW_DWELL) {
        
        USR_PRINTF ("AW_DWELL (wallclock) running...\n");
        err_count = 0;
        ber = 0;    

        
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, 0));
        
        CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_MASK, RX_DATABIST_TOP_REG1_BIST_ENABLE_A_OFFSET, 1));

        for (dwell_loop = 0; dwell_loop < dwell_params->aw_measure_time; dwell_loop++) {
            
            CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
            CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 1));
            CHECK(pmd_write_field(mss, RX_DATABIST_TOP_REG1_ADDR, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_MASK, RX_DATABIST_TOP_REG1_ERROR_CNT_CLR_A_OFFSET, 0));
            
            USR_SLEEP(1000000);
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG3_ADDR, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_MASK, RX_DATABIST_TOP_RDREG3_ERROR_CNT_55T32_NT_OFFSET, &err_cnt_55_32));
            CHECK(pmd_read_field(mss, RX_DATABIST_TOP_RDREG2_ADDR, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_MASK, RX_DATABIST_TOP_RDREG2_ERROR_CNT_NT_OFFSET, &err_cnt_31_0));
            err_count += (uint64_t) err_cnt_55_32 << 32 | (uint64_t) err_cnt_31_0;
            CHECK(pmd_read_check_field(mss, RX_DATABIST_TOP_RDREG1_ADDR, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_MASK, RX_DATABIST_TOP_RDREG1_ERROR_CNT_OVERFLOW_NT_OFFSET, RD_EQ, &err_count_overflow, 0, 0 ));
            ber += (double)err_count / (dwell_params->aw_measure_time * dwell_params->rx_data_rate);
            USR_PRINTF("err_count = %lu\n", err_count);
            USR_PRINTF("err_count_overflow = %d\n", err_count_overflow);
            USR_PRINTF("ber = %e\n", ber);
        }
    TRACE_EXIT("mss=0x%x, 0x%x, timer_threshold = %d, rx_width = %d, timeout_us = %d, expected_errors = %d", mss->phy_offset, mss->lane_offset,  timer_threshold,  rx_width,  timeout_us,  expected_errors);
        return AW_ERR_CODE_NONE;
    }
    
    TRACE_EXIT("mss=0x%x, 0x%x, timer_threshold = %d, rx_width = %d, timeout_us = %d, expected_errors = %d", mss->phy_offset, mss->lane_offset,  timer_threshold,  rx_width,  timeout_us,  expected_errors);
    return AW_ERR_CODE_FUNC_FAILURE;
}


int aw_pmd_eqeval_type_set(mss_access_t *mss, uint32_t eq_type) {
    TRACE_ENTER("mss=0x%x, 0x%x, eq_type = %d", mss->phy_offset, mss->lane_offset,  eq_type);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_TYPE_A_OFFSET, eq_type));
    TRACE_EXIT("mss=0x%x, 0x%x, eq_type = %d", mss->phy_offset, mss->lane_offset,  eq_type);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_req_set(mss_access_t *mss, uint32_t value) {
    TRACE_ENTER("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    CHECK(pmd_write_field(mss, DIG_SOC_LANE_OVRD_REG2_ADDR, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_MASK, DIG_SOC_LANE_OVRD_REG2_ICTL_RX_LINKEVAL_REQ_A_OFFSET, value));
    TRACE_EXIT("mss=0x%x, 0x%x, value = %d", mss->phy_offset, mss->lane_offset,  value);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_ack_get(mss_access_t *mss, uint32_t *eqeval_ack) {
    TRACE_ENTER("mss=0x%x, 0x%x, *eqeval_ack = %d", mss->phy_offset, mss->lane_offset,  *eqeval_ack);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, eqeval_ack));
    TRACE_EXIT("mss=0x%x, 0x%x, *eqeval_ack = %d", mss->phy_offset, mss->lane_offset,  *eqeval_ack);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_eqeval_incdec_get(mss_access_t *mss, uint32_t *incdec) {
    TRACE_ENTER("mss=0x%x, 0x%x, *incdec = %d", mss->phy_offset, mss->lane_offset,  *incdec);

    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG3_ADDR, DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_DIR_MASK, DIG_SOC_LANE_STAT_REG3_OCTL_RX_LINKEVAL_DIR_OFFSET, incdec));

    TRACE_EXIT("mss=0x%x, 0x%x, *incdec = %d", mss->phy_offset, mss->lane_offset,  *incdec);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_rx_equalize(mss_access_t *mss, aw_eq_type_t eq_type, uint32_t timeout_us) {
    TRACE_ENTER("mss=0x%x, 0x%x, eq_type = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  eq_type,  timeout_us);
    int poll_result;
    uint32_t incdec;

    aw_pmd_eqeval_type_set(mss, eq_type);
    aw_pmd_rxeq_prbs_set(mss, 1);
    aw_pmd_eqeval_req_set(mss, 1);
    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 1, timeout_us);
    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for asserting rx linkeval ack\n");
    TRACE_EXIT("mss=0x%x, 0x%x, eq_type = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  eq_type,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;
    }
    aw_pmd_eqeval_req_set(mss, 0);

    poll_result = pmd_poll_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_LINKEVAL_ACK_OFFSET, 0, 10);

    if (poll_result == -1) {
        USR_PRINTF("ERROR: Timed out waiting for rx linkeval ack\n");
    TRACE_EXIT("mss=0x%x, 0x%x, eq_type = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  eq_type,  timeout_us);
        return AW_ERR_CODE_POLL_TIMEOUT;
    } else {
        USR_PRINTF("Received RXEQ EVAL Ack\n");
        aw_pmd_eqeval_incdec_get(mss,&incdec);
        USR_PRINTF("EqEval incdec = 0x%X\n",incdec);
    TRACE_EXIT("mss=0x%x, 0x%x, eq_type = %d, timeout_us = %d", mss->phy_offset, mss->lane_offset,  eq_type,  timeout_us);
        return AW_ERR_CODE_NONE;
    }
}












int aw_pmd_rd_data_pipeline_stages_set(mss_access_t *mss, uint32_t stages) {
    TRACE_ENTER("mss=0x%x, 0x%x, stages = %d", mss->phy_offset, mss->lane_offset,  stages);
    CHECK(pmd_write_field(mss, SRAM0_CFG_ADDR, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_MASK, SRAM0_CFG_RD_DATA_PIPELINE_STAGES_A_OFFSET, stages));
#if AW_NUM_LANES == 16
    CHECK(pmd_write_field(mss, SRAM1_CFG_ADDR, SRAM1_CFG_RD_DATA_PIPELINE_STAGES_A_MASK, SRAM1_CFG_RD_DATA_PIPELINE_STAGES_A_OFFSET, stages));
#endif
    TRACE_EXIT("mss=0x%x, 0x%x, stages = %d", mss->phy_offset, mss->lane_offset,  stages);
    return AW_ERR_CODE_NONE;
}













int aw_pmd_rx_cdr_lock_get(mss_access_t *mss, uint32_t *rx_cdr_lock) {
    TRACE_ENTER("mss=0x%x, 0x%x, *rx_cdr_lock = %d", mss->phy_offset, mss->lane_offset,  *rx_cdr_lock);
    CHECK(pmd_read_field(mss, DIG_SOC_LANE_STAT_REG1_ADDR, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_MASK, DIG_SOC_LANE_STAT_REG1_OCTL_RX_DATA_VLD_OFFSET, rx_cdr_lock));
    TRACE_EXIT("mss=0x%x, 0x%x, *rx_cdr_lock = %d", mss->phy_offset, mss->lane_offset,  *rx_cdr_lock);
    return AW_ERR_CODE_NONE;
}






















int aw_pmd_rx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map) {
    TRACE_ENTER("mss=0x%x, 0x%x, gray_code_map = %d", mss->phy_offset, mss->lane_offset,  gray_code_map);
    uint8_t el3 = gray_code_map & 0x3;
    uint8_t el1 = (gray_code_map >> 2) & 0x3;
    uint8_t eh1 = (gray_code_map >> 4) & 0x3;
    uint8_t eh3 = (gray_code_map >> 6) & 0x3;

    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL3_VAL_NT_MASK, RX_DEMAPPER_EL3_VAL_NT_OFFSET, el3));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EL1_VAL_NT_MASK, RX_DEMAPPER_EL1_VAL_NT_OFFSET, el1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH1_VAL_NT_MASK, RX_DEMAPPER_EH1_VAL_NT_OFFSET, eh1));
    CHECK(pmd_write_field(mss, RX_DEMAPPER_ADDR, RX_DEMAPPER_EH3_VAL_NT_MASK, RX_DEMAPPER_EH3_VAL_NT_OFFSET, eh3));

    TRACE_EXIT("mss=0x%x, 0x%x, gray_code_map = %d", mss->phy_offset, mss->lane_offset,  gray_code_map);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_tx_gray_code_mapping_set(mss_access_t *mss, uint8_t gray_code_map){
    TRACE_ENTER("mss=0x%x, 0x%x, gray_code_map = %d", mss->phy_offset, mss->lane_offset,  gray_code_map);

    CHECK(pmd_write_field(mss, TX_DATAPATH_REG1_ADDR, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_MASK, TX_DATAPATH_REG1_GRAY_CODE_MAPPING_NT_OFFSET, gray_code_map));

    TRACE_EXIT("mss=0x%x, 0x%x, gray_code_map = %d", mss->phy_offset, mss->lane_offset,  gray_code_map);
    return AW_ERR_CODE_NONE;
}













int aw_pmd_snr_vld_hys_thresh_set_from_target_snr(mss_access_t *mss, uint32_t target_snr_low, uint32_t target_snr_high){
    TRACE_ENTER("mss=0x%x, 0x%x, target_snr_low = %d, target_snr_high = %d", mss->phy_offset, mss->lane_offset,  target_snr_low,  target_snr_high);
    uint32_t thresh_low_reg_value  = round(pow(10, target_snr_low/10.0));
    uint32_t thresh_high_reg_value = round(pow(10, target_snr_high/10.0));

    USR_PRINTF("Target SNR is between %d dB and %d dB.\n", target_snr_low, target_snr_high);
    USR_PRINTF("Setting register values for snr valid thresholds: thresh_low_reg_value=%d and thresh_high_reg_value=%d.\n", thresh_low_reg_value, thresh_high_reg_value);

    CHECK(pmd_write_field(mss, RX_SNR_REG4_ADDR, RX_SNR_REG4_VLD_HYS_THRESH_LOW_NT_MASK,  RX_SNR_REG4_VLD_HYS_THRESH_LOW_NT_OFFSET,  thresh_low_reg_value ));
    CHECK(pmd_write_field(mss, RX_SNR_REG5_ADDR, RX_SNR_REG5_VLD_HYS_THRESH_HIGH_NT_MASK, RX_SNR_REG5_VLD_HYS_THRESH_HIGH_NT_OFFSET, thresh_high_reg_value));

    TRACE_EXIT("mss=0x%x, 0x%x, target_snr_low = %d, target_snr_high = %d", mss->phy_offset, mss->lane_offset,  target_snr_low,  target_snr_high);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_snr_vld_hys_thresh_set_recommended(mss_access_t *mss, uint32_t nrz_mode){
    TRACE_ENTER("mss=0x%x, 0x%x, nrz_mode = %d", mss->phy_offset, mss->lane_offset,  nrz_mode);
    uint32_t target_snr_low  = (nrz_mode==1) ? 20 : 12; 
    uint32_t target_snr_high = (nrz_mode==1) ? 25 : 15; 

    aw_pmd_snr_vld_hys_thresh_set_from_target_snr(mss, target_snr_low, target_snr_high);

    TRACE_EXIT("mss=0x%x, 0x%x, nrz_mode = %d", mss->phy_offset, mss->lane_offset,  nrz_mode);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_snr_mon_enable_set(mss_access_t *mss, uint32_t nrz_mode, uint32_t mon_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, nrz_mode = %d, mon_enable = %d", mss->phy_offset, mss->lane_offset,  nrz_mode,  mon_enable);
    CHECK(pmd_write_field(mss, RST_DBE_RX_ADDR,  RST_DBE_RX_SNR_BA_MASK,           RST_DBE_RX_SNR_BA_OFFSET,           1         ));
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_MON_NRZ_MODE_NT_MASK, RX_SNR_REG1_MON_NRZ_MODE_NT_OFFSET, nrz_mode  )); 
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_MON_ENABLE_A_MASK,    RX_SNR_REG1_MON_ENABLE_A_OFFSET,    mon_enable));
    TRACE_EXIT("mss=0x%x, 0x%x, nrz_mode = %d, mon_enable = %d", mss->phy_offset, mss->lane_offset,  nrz_mode,  mon_enable);
    return AW_ERR_CODE_NONE;
}


int aw_pmd_snr_vld_enable_set(mss_access_t *mss, uint32_t vld_enable){
    TRACE_ENTER("mss=0x%x, 0x%x, vld_enable = %d", mss->phy_offset, mss->lane_offset,  vld_enable);
    CHECK(pmd_write_field(mss, RX_SNR_REG1_ADDR, RX_SNR_REG1_VLD_ENABLE_A_MASK, RX_SNR_REG1_VLD_ENABLE_A_OFFSET, vld_enable));
    TRACE_EXIT("mss=0x%x, 0x%x, vld_enable = %d", mss->phy_offset, mss->lane_offset,  vld_enable);
    return AW_ERR_CODE_NONE;
}

int aw_pmd_anlt_auto_neg_result_get (mss_access_t *mss, uint8_t no_consortium, uint32_t * an_result){

    uint32_t an_link_good;
    CHECK(pmd_read_check_field(mss, ETH_ANLT_STATUS_ADDR, ETH_ANLT_STATUS_AN_LINK_GOOD_MASK, ETH_ANLT_STATUS_AN_LINK_GOOD_OFFSET, RD_EQ, &an_link_good,  1, 0 /*NULL*/));
 
    if(no_consortium == 1) {

        uint32_t an_spec_1;
        uint32_t an_spec_2;
        uint32_t an_spec_3;
        uint32_t an_spec_4;
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_1_MASK, ETH_AN_RESULT_REG1_AN_SPEC_1_OFFSET, &an_spec_1));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_2_MASK, ETH_AN_RESULT_REG1_AN_SPEC_2_OFFSET, &an_spec_2));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG1_ADDR, ETH_AN_RESULT_REG1_AN_SPEC_3_MASK, ETH_AN_RESULT_REG1_AN_SPEC_3_OFFSET, &an_spec_3));
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_SPEC_4_MASK, ETH_AN_RESULT_REG2_AN_SPEC_4_OFFSET, &an_spec_4));


        uint32_t an_spec = an_spec_1;
        an_spec = (an_spec_2 << 3)  | an_spec;
        an_spec = (an_spec_3 << 5)  | an_spec;
        an_spec = (an_spec_4 << 13) | an_spec;

        // there should only be 1 bit that is 1, all other bits should be 0
        int cntr = 0;
        for (int i = 0 ; i < 32 ; i++){
            int bit = (an_spec >> i) & 1;
            if (bit == 1){
                cntr++;
            }
        }
        if (cntr != 1){
            return AW_ERR_CODE_BAD_STATE;
        }


        *an_result = log(an_spec)/log(2);
    } else if (no_consortium == 0) {

        uint32_t an_econ_spec_1;
        CHECK(pmd_read_field(mss, ETH_AN_RESULT_REG2_ADDR, ETH_AN_RESULT_REG2_AN_ECON_SPEC_MASK, ETH_AN_RESULT_REG2_AN_ECON_SPEC_OFFSET, &an_econ_spec_1));

        uint32_t an_spec = an_econ_spec_1;

        int cntr = 0;
        for (int i = 0 ; i < 5 ; i++) {
            int bit = (an_spec >> i) & 1;
            if (bit == 1) {
                cntr++;
            }
        }
        if (cntr != 1) {
            return AW_ERR_CODE_BAD_STATE;
        }

        *an_result = log(an_spec)/log(2);
    } else {
        USR_PRINTF("ERROR: Please set no_consortium field to either 0 or 1. Other values not accepted \n");
        return AW_ERR_CODE_BAD_STATE;
    }

    return AW_ERR_CODE_NONE;
}
