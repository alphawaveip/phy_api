// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */







#include "aw_alphacore.h"
#include "aw_pmd_rx_dsp_get.h"



int aw_pmd_rx_dsp_get(mss_access_t *mss, uint32_t branch, aw_dsp_param_t *dsp_info, uint32_t print_en){
    uint32_t target_rx_ccg;
    uint32_t slicer_rx_ccg;
    uint32_t ffe_rx_ccg;
    uint32_t eqdfe_rx_ccg;
    uint32_t offset_rx_ccg;
    uint32_t ffe_adapt_ena_a;
    uint32_t target_adapt_ena_a;
    uint32_t dfe_adapt_ena_a;
    uint32_t offset_adapt_ena_a;

    
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_TARGET_ADAPT_ENA_A_MASK, RX_CCG_TARGET_ADAPT_ENA_A_OFFSET, &target_rx_ccg));
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_SLICER_ADAPT_ENA_A_MASK, RX_CCG_SLICER_ADAPT_ENA_A_OFFSET, &slicer_rx_ccg));
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_FFE_ADAPT_ENA_A_MASK, RX_CCG_FFE_ADAPT_ENA_A_OFFSET, &ffe_rx_ccg));
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_DFE_ADAPT_ENA_A_MASK, RX_CCG_DFE_ADAPT_ENA_A_OFFSET, &eqdfe_rx_ccg));
    CHECK(pmd_read_field(mss, RX_CCG_ADDR, RX_CCG_DC_OFFSET_ADAPT_ENA_A_MASK, RX_CCG_DC_OFFSET_ADAPT_ENA_A_OFFSET, &offset_rx_ccg));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_REG19_ADDR, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_MASK, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_OFFSET, &ffe_adapt_ena_a));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_REG10_ADDR, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_MASK, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_OFFSET, &target_adapt_ena_a));
    CHECK(pmd_read_field(mss, RX_DFE_ADAPT_REG6_ADDR, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_MASK, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_OFFSET, &dfe_adapt_ena_a));
    CHECK(pmd_read_field(mss, RX_DATABLOCK_OFFSET_REG6_ADDR, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_MASK, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_OFFSET, &offset_adapt_ena_a));

    
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_TARGET_ADAPT_ENA_A_MASK, RX_CCG_TARGET_ADAPT_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_SLICER_ADAPT_ENA_A_MASK, RX_CCG_SLICER_ADAPT_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_FFE_ADAPT_ENA_A_MASK, RX_CCG_FFE_ADAPT_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_DFE_ADAPT_ENA_A_MASK, RX_CCG_DFE_ADAPT_ENA_A_OFFSET, 1));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_DC_OFFSET_ADAPT_ENA_A_MASK, RX_CCG_DC_OFFSET_ADAPT_ENA_A_OFFSET,1));
    CHECK(pmd_write_field(mss, RX_FFE_ADAPT_REG19_ADDR, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_MASK, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_SLICER_TARGET_REG10_ADDR, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_MASK, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DFE_ADAPT_REG6_ADDR, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_MASK, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_OFFSET, 0));
    CHECK(pmd_write_field(mss, RX_DATABLOCK_OFFSET_REG6_ADDR, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_MASK, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_OFFSET, 0));


    CHECK(pmd_write_field(mss, RX_DATABLOCK_OFFSET_REG8_ADDR, RX_DATABLOCK_OFFSET_REG8_DBG_BRANCH_SEL_NT_MASK, RX_DATABLOCK_OFFSET_REG8_DBG_BRANCH_SEL_NT_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_DATABLOCK_OFFSET_RDREG9_ADDR, RX_DATABLOCK_OFFSET_RDREG9_OFFSET_NT_MASK, RX_DATABLOCK_OFFSET_RDREG9_OFFSET_NT_OFFSET,   &(dsp_info->dc_offset) ));


    CHECK(pmd_write_field(mss, RX_FFE_ADAPT_REG20_ADDR, RX_FFE_ADAPT_REG20_DBG_BRANCH_SEL_NT_MASK, RX_FFE_ADAPT_REG20_DBG_BRANCH_SEL_NT_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP0_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP0_NT_OFFSET,   &(dsp_info->ffe[0]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP1_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP1_NT_OFFSET,   &(dsp_info->ffe[1]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP2_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP2_NT_OFFSET,   &(dsp_info->ffe[2]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP3_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP3_NT_OFFSET,   &(dsp_info->ffe[3]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP4_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP4_NT_OFFSET,   &(dsp_info->ffe[4]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG0_ADDR, RX_FFE_ADAPT_COEF_RDREG0_TAP5_NT_MASK, RX_FFE_ADAPT_COEF_RDREG0_TAP5_NT_OFFSET,   &(dsp_info->ffe[5]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG1_ADDR, RX_FFE_ADAPT_COEF_RDREG1_TAP6_NT_MASK, RX_FFE_ADAPT_COEF_RDREG1_TAP6_NT_OFFSET,   &(dsp_info->ffe[6]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG1_ADDR, RX_FFE_ADAPT_COEF_RDREG1_TAP7_NT_MASK, RX_FFE_ADAPT_COEF_RDREG1_TAP7_NT_OFFSET,   &(dsp_info->ffe[7]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG1_ADDR, RX_FFE_ADAPT_COEF_RDREG1_TAP8_NT_MASK, RX_FFE_ADAPT_COEF_RDREG1_TAP8_NT_OFFSET,   &(dsp_info->ffe[8]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG1_ADDR, RX_FFE_ADAPT_COEF_RDREG1_TAP9_NT_MASK, RX_FFE_ADAPT_COEF_RDREG1_TAP9_NT_OFFSET,   &(dsp_info->ffe[9]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG2_ADDR, RX_FFE_ADAPT_COEF_RDREG2_TAP10_NT_MASK, RX_FFE_ADAPT_COEF_RDREG2_TAP10_NT_OFFSET,   &(dsp_info->ffe[10]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG2_ADDR, RX_FFE_ADAPT_COEF_RDREG2_TAP11_NT_MASK, RX_FFE_ADAPT_COEF_RDREG2_TAP11_NT_OFFSET,   &(dsp_info->ffe[11]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG2_ADDR, RX_FFE_ADAPT_COEF_RDREG2_TAP12_NT_MASK, RX_FFE_ADAPT_COEF_RDREG2_TAP12_NT_OFFSET,   &(dsp_info->ffe[12]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG3_ADDR, RX_FFE_ADAPT_COEF_RDREG3_TAP13_NT_MASK, RX_FFE_ADAPT_COEF_RDREG3_TAP13_NT_OFFSET,   &(dsp_info->ffe[13]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG3_ADDR, RX_FFE_ADAPT_COEF_RDREG3_TAP14_NT_MASK, RX_FFE_ADAPT_COEF_RDREG3_TAP14_NT_OFFSET,   &(dsp_info->ffe[14]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG3_ADDR, RX_FFE_ADAPT_COEF_RDREG3_TAP15_NT_MASK, RX_FFE_ADAPT_COEF_RDREG3_TAP15_NT_OFFSET,   &(dsp_info->ffe[15]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG4_ADDR, RX_FFE_ADAPT_COEF_RDREG4_TAP16_NT_MASK, RX_FFE_ADAPT_COEF_RDREG4_TAP16_NT_OFFSET,   &(dsp_info->ffe[16]) ));
    CHECK(pmd_read_field(mss, RX_FFE_ADAPT_COEF_RDREG4_ADDR, RX_FFE_ADAPT_COEF_RDREG4_TAP17_NT_MASK, RX_FFE_ADAPT_COEF_RDREG4_TAP17_NT_OFFSET,   &(dsp_info->ffe[17]) ));


    dsp_info->s_ffe[0] = aw_tc_sm_conv(dsp_info->ffe[0], RX_FFE_ADAPT_COEF_RDREG0_TAP0_NT_BITWIDTH);
    dsp_info->s_ffe[1] = aw_tc_sm_conv(dsp_info->ffe[1], RX_FFE_ADAPT_COEF_RDREG0_TAP1_NT_BITWIDTH);
    dsp_info->s_ffe[2] = aw_tc_sm_conv(dsp_info->ffe[2], RX_FFE_ADAPT_COEF_RDREG0_TAP2_NT_BITWIDTH);
    dsp_info->s_ffe[3] = aw_tc_sm_conv(dsp_info->ffe[3], RX_FFE_ADAPT_COEF_RDREG0_TAP3_NT_BITWIDTH);
    dsp_info->s_ffe[4] = aw_tc_sm_conv(dsp_info->ffe[4], RX_FFE_ADAPT_COEF_RDREG0_TAP4_NT_BITWIDTH);
    dsp_info->s_ffe[5] = aw_tc_sm_conv(dsp_info->ffe[5], RX_FFE_ADAPT_COEF_RDREG0_TAP5_NT_BITWIDTH);
    dsp_info->s_ffe[6] = aw_tc_sm_conv(dsp_info->ffe[6], RX_FFE_ADAPT_COEF_RDREG1_TAP6_NT_BITWIDTH);
    dsp_info->s_ffe[7] = aw_tc_sm_conv(dsp_info->ffe[7], RX_FFE_ADAPT_COEF_RDREG1_TAP7_NT_BITWIDTH);
    dsp_info->s_ffe[8] = aw_tc_sm_conv(dsp_info->ffe[8], RX_FFE_ADAPT_COEF_RDREG1_TAP8_NT_BITWIDTH);
    dsp_info->s_ffe[9] = aw_tc_sm_conv(dsp_info->ffe[9], RX_FFE_ADAPT_COEF_RDREG1_TAP9_NT_BITWIDTH);
    dsp_info->s_ffe[10] = aw_tc_sm_conv(dsp_info->ffe[10], RX_FFE_ADAPT_COEF_RDREG2_TAP10_NT_BITWIDTH);
    dsp_info->s_ffe[11] = aw_tc_sm_conv(dsp_info->ffe[11], RX_FFE_ADAPT_COEF_RDREG2_TAP11_NT_BITWIDTH);
    dsp_info->s_ffe[12] = aw_tc_sm_conv(dsp_info->ffe[12], RX_FFE_ADAPT_COEF_RDREG2_TAP12_NT_BITWIDTH);
    dsp_info->s_ffe[13] = aw_tc_sm_conv(dsp_info->ffe[13], RX_FFE_ADAPT_COEF_RDREG3_TAP13_NT_BITWIDTH);
    dsp_info->s_ffe[14] = aw_tc_sm_conv(dsp_info->ffe[14], RX_FFE_ADAPT_COEF_RDREG3_TAP14_NT_BITWIDTH);
    dsp_info->s_ffe[15] = aw_tc_sm_conv(dsp_info->ffe[15], RX_FFE_ADAPT_COEF_RDREG3_TAP15_NT_BITWIDTH);
    dsp_info->s_ffe[16] = aw_tc_sm_conv(dsp_info->ffe[16], RX_FFE_ADAPT_COEF_RDREG4_TAP16_NT_BITWIDTH);
    dsp_info->s_ffe[17] = aw_tc_sm_conv(dsp_info->ffe[17], RX_FFE_ADAPT_COEF_RDREG4_TAP17_NT_BITWIDTH);


    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP0_NT_MASK, RXEQSTORE0_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP1_NT_MASK, RXEQSTORE0_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP2_NT_MASK, RXEQSTORE0_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP3_NT_MASK, RXEQSTORE0_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP4_NT_MASK, RXEQSTORE0_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG1_ADDR, RXEQSTORE0_TAP_REG1_TAP5_NT_MASK, RXEQSTORE0_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR, RXEQSTORE0_TAP_REG2_TAP6_NT_MASK, RXEQSTORE0_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR, RXEQSTORE0_TAP_REG2_TAP7_NT_MASK, RXEQSTORE0_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR, RXEQSTORE0_TAP_REG2_TAP8_NT_MASK, RXEQSTORE0_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG2_ADDR, RXEQSTORE0_TAP_REG2_TAP9_NT_MASK, RXEQSTORE0_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR, RXEQSTORE0_TAP_REG3_TAP10_NT_MASK, RXEQSTORE0_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR, RXEQSTORE0_TAP_REG3_TAP11_NT_MASK, RXEQSTORE0_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG3_ADDR, RXEQSTORE0_TAP_REG3_TAP12_NT_MASK, RXEQSTORE0_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR, RXEQSTORE0_TAP_REG4_TAP13_NT_MASK, RXEQSTORE0_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR, RXEQSTORE0_TAP_REG4_TAP14_NT_MASK, RXEQSTORE0_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG4_ADDR, RXEQSTORE0_TAP_REG4_TAP15_NT_MASK, RXEQSTORE0_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG5_ADDR, RXEQSTORE0_TAP_REG5_TAP16_NT_MASK, RXEQSTORE0_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE0_TAP_REG5_ADDR, RXEQSTORE0_TAP_REG5_TAP17_NT_MASK, RXEQSTORE0_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe[17]) ));


    dsp_info->s_pre_ffe[0] = aw_tc_sm_conv(dsp_info->pre_ffe[0], RXEQSTORE0_TAP_REG1_TAP0_NT_BITWIDTH);
    dsp_info->s_pre_ffe[1] = aw_tc_sm_conv(dsp_info->pre_ffe[1], RXEQSTORE0_TAP_REG1_TAP1_NT_BITWIDTH);
    dsp_info->s_pre_ffe[2] = aw_tc_sm_conv(dsp_info->pre_ffe[2], RXEQSTORE0_TAP_REG1_TAP2_NT_BITWIDTH);
    dsp_info->s_pre_ffe[3] = aw_tc_sm_conv(dsp_info->pre_ffe[3], RXEQSTORE0_TAP_REG1_TAP3_NT_BITWIDTH);
    dsp_info->s_pre_ffe[4] = aw_tc_sm_conv(dsp_info->pre_ffe[4], RXEQSTORE0_TAP_REG1_TAP4_NT_BITWIDTH);
    dsp_info->s_pre_ffe[5] = aw_tc_sm_conv(dsp_info->pre_ffe[5], RXEQSTORE0_TAP_REG1_TAP5_NT_BITWIDTH);
    dsp_info->s_pre_ffe[6] = aw_tc_sm_conv(dsp_info->pre_ffe[6], RXEQSTORE0_TAP_REG2_TAP6_NT_BITWIDTH);
    dsp_info->s_pre_ffe[7] = aw_tc_sm_conv(dsp_info->pre_ffe[7], RXEQSTORE0_TAP_REG2_TAP7_NT_BITWIDTH);
    dsp_info->s_pre_ffe[8] = aw_tc_sm_conv(dsp_info->pre_ffe[8], RXEQSTORE0_TAP_REG2_TAP8_NT_BITWIDTH);
    dsp_info->s_pre_ffe[9] = aw_tc_sm_conv(dsp_info->pre_ffe[9], RXEQSTORE0_TAP_REG2_TAP9_NT_BITWIDTH);
    dsp_info->s_pre_ffe[10] = aw_tc_sm_conv(dsp_info->pre_ffe[10], RXEQSTORE0_TAP_REG3_TAP10_NT_BITWIDTH);
    dsp_info->s_pre_ffe[11] = aw_tc_sm_conv(dsp_info->pre_ffe[11], RXEQSTORE0_TAP_REG3_TAP11_NT_BITWIDTH);
    dsp_info->s_pre_ffe[12] = aw_tc_sm_conv(dsp_info->pre_ffe[12], RXEQSTORE0_TAP_REG3_TAP12_NT_BITWIDTH);
    dsp_info->s_pre_ffe[13] = aw_tc_sm_conv(dsp_info->pre_ffe[13], RXEQSTORE0_TAP_REG4_TAP13_NT_BITWIDTH);
    dsp_info->s_pre_ffe[14] = aw_tc_sm_conv(dsp_info->pre_ffe[14], RXEQSTORE0_TAP_REG4_TAP14_NT_BITWIDTH);
    dsp_info->s_pre_ffe[15] = aw_tc_sm_conv(dsp_info->pre_ffe[15], RXEQSTORE0_TAP_REG4_TAP15_NT_BITWIDTH);
    dsp_info->s_pre_ffe[16] = aw_tc_sm_conv(dsp_info->pre_ffe[16], RXEQSTORE0_TAP_REG5_TAP16_NT_BITWIDTH);
    dsp_info->s_pre_ffe[17] = aw_tc_sm_conv(dsp_info->pre_ffe[17], RXEQSTORE0_TAP_REG5_TAP17_NT_BITWIDTH);


    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP0_NT_MASK, RXEQSTORE1_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP1_NT_MASK, RXEQSTORE1_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP2_NT_MASK, RXEQSTORE1_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP3_NT_MASK, RXEQSTORE1_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP4_NT_MASK, RXEQSTORE1_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG1_ADDR, RXEQSTORE1_TAP_REG1_TAP5_NT_MASK, RXEQSTORE1_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR, RXEQSTORE1_TAP_REG2_TAP6_NT_MASK, RXEQSTORE1_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR, RXEQSTORE1_TAP_REG2_TAP7_NT_MASK, RXEQSTORE1_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR, RXEQSTORE1_TAP_REG2_TAP8_NT_MASK, RXEQSTORE1_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG2_ADDR, RXEQSTORE1_TAP_REG2_TAP9_NT_MASK, RXEQSTORE1_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR, RXEQSTORE1_TAP_REG3_TAP10_NT_MASK, RXEQSTORE1_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR, RXEQSTORE1_TAP_REG3_TAP11_NT_MASK, RXEQSTORE1_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG3_ADDR, RXEQSTORE1_TAP_REG3_TAP12_NT_MASK, RXEQSTORE1_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR, RXEQSTORE1_TAP_REG4_TAP13_NT_MASK, RXEQSTORE1_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR, RXEQSTORE1_TAP_REG4_TAP14_NT_MASK, RXEQSTORE1_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG4_ADDR, RXEQSTORE1_TAP_REG4_TAP15_NT_MASK, RXEQSTORE1_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG5_ADDR, RXEQSTORE1_TAP_REG5_TAP16_NT_MASK, RXEQSTORE1_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE1_TAP_REG5_ADDR, RXEQSTORE1_TAP_REG5_TAP17_NT_MASK, RXEQSTORE1_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_dfe_ffe[17]) ));


    dsp_info->s_pre_dfe_ffe[0] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[0], RXEQSTORE1_TAP_REG1_TAP0_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[1] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[1], RXEQSTORE1_TAP_REG1_TAP1_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[2] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[2], RXEQSTORE1_TAP_REG1_TAP2_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[3] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[3], RXEQSTORE1_TAP_REG1_TAP3_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[4] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[4], RXEQSTORE1_TAP_REG1_TAP4_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[5] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[5], RXEQSTORE1_TAP_REG1_TAP5_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[6] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[6], RXEQSTORE1_TAP_REG2_TAP6_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[7] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[7], RXEQSTORE1_TAP_REG2_TAP7_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[8] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[8], RXEQSTORE1_TAP_REG2_TAP8_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[9] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[9], RXEQSTORE1_TAP_REG2_TAP9_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[10] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[10], RXEQSTORE1_TAP_REG3_TAP10_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[11] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[11], RXEQSTORE1_TAP_REG3_TAP11_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[12] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[12], RXEQSTORE1_TAP_REG3_TAP12_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[13] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[13], RXEQSTORE1_TAP_REG4_TAP13_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[14] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[14], RXEQSTORE1_TAP_REG4_TAP14_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[15] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[15], RXEQSTORE1_TAP_REG4_TAP15_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[16] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[16], RXEQSTORE1_TAP_REG5_TAP16_NT_BITWIDTH);
    dsp_info->s_pre_dfe_ffe[17] = aw_tc_sm_conv(dsp_info->pre_dfe_ffe[17], RXEQSTORE1_TAP_REG5_TAP17_NT_BITWIDTH);


    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP0_NT_MASK, RXEQSTORE2_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP1_NT_MASK, RXEQSTORE2_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP2_NT_MASK, RXEQSTORE2_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP3_NT_MASK, RXEQSTORE2_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP4_NT_MASK, RXEQSTORE2_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG1_ADDR, RXEQSTORE2_TAP_REG1_TAP5_NT_MASK, RXEQSTORE2_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR, RXEQSTORE2_TAP_REG2_TAP6_NT_MASK, RXEQSTORE2_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR, RXEQSTORE2_TAP_REG2_TAP7_NT_MASK, RXEQSTORE2_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR, RXEQSTORE2_TAP_REG2_TAP8_NT_MASK, RXEQSTORE2_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG2_ADDR, RXEQSTORE2_TAP_REG2_TAP9_NT_MASK, RXEQSTORE2_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR, RXEQSTORE2_TAP_REG3_TAP10_NT_MASK, RXEQSTORE2_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR, RXEQSTORE2_TAP_REG3_TAP11_NT_MASK, RXEQSTORE2_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG3_ADDR, RXEQSTORE2_TAP_REG3_TAP12_NT_MASK, RXEQSTORE2_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR, RXEQSTORE2_TAP_REG4_TAP13_NT_MASK, RXEQSTORE2_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR, RXEQSTORE2_TAP_REG4_TAP14_NT_MASK, RXEQSTORE2_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG4_ADDR, RXEQSTORE2_TAP_REG4_TAP15_NT_MASK, RXEQSTORE2_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG5_ADDR, RXEQSTORE2_TAP_REG5_TAP16_NT_MASK, RXEQSTORE2_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE2_TAP_REG5_ADDR, RXEQSTORE2_TAP_REG5_TAP17_NT_MASK, RXEQSTORE2_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle0[17]) ));


    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP0_NT_MASK, RXEQSTORE3_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP1_NT_MASK, RXEQSTORE3_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP2_NT_MASK, RXEQSTORE3_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP3_NT_MASK, RXEQSTORE3_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP4_NT_MASK, RXEQSTORE3_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG1_ADDR, RXEQSTORE3_TAP_REG1_TAP5_NT_MASK, RXEQSTORE3_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR, RXEQSTORE3_TAP_REG2_TAP6_NT_MASK, RXEQSTORE3_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR, RXEQSTORE3_TAP_REG2_TAP7_NT_MASK, RXEQSTORE3_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR, RXEQSTORE3_TAP_REG2_TAP8_NT_MASK, RXEQSTORE3_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG2_ADDR, RXEQSTORE3_TAP_REG2_TAP9_NT_MASK, RXEQSTORE3_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR, RXEQSTORE3_TAP_REG3_TAP10_NT_MASK, RXEQSTORE3_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR, RXEQSTORE3_TAP_REG3_TAP11_NT_MASK, RXEQSTORE3_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG3_ADDR, RXEQSTORE3_TAP_REG3_TAP12_NT_MASK, RXEQSTORE3_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR, RXEQSTORE3_TAP_REG4_TAP13_NT_MASK, RXEQSTORE3_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR, RXEQSTORE3_TAP_REG4_TAP14_NT_MASK, RXEQSTORE3_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG4_ADDR, RXEQSTORE3_TAP_REG4_TAP15_NT_MASK, RXEQSTORE3_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG5_ADDR, RXEQSTORE3_TAP_REG5_TAP16_NT_MASK, RXEQSTORE3_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE3_TAP_REG5_ADDR, RXEQSTORE3_TAP_REG5_TAP17_NT_MASK, RXEQSTORE3_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle1[17]) ));


    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP0_NT_MASK, RXEQSTORE4_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP1_NT_MASK, RXEQSTORE4_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP2_NT_MASK, RXEQSTORE4_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP3_NT_MASK, RXEQSTORE4_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP4_NT_MASK, RXEQSTORE4_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG1_ADDR, RXEQSTORE4_TAP_REG1_TAP5_NT_MASK, RXEQSTORE4_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR, RXEQSTORE4_TAP_REG2_TAP6_NT_MASK, RXEQSTORE4_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR, RXEQSTORE4_TAP_REG2_TAP7_NT_MASK, RXEQSTORE4_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR, RXEQSTORE4_TAP_REG2_TAP8_NT_MASK, RXEQSTORE4_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG2_ADDR, RXEQSTORE4_TAP_REG2_TAP9_NT_MASK, RXEQSTORE4_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR, RXEQSTORE4_TAP_REG3_TAP10_NT_MASK, RXEQSTORE4_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR, RXEQSTORE4_TAP_REG3_TAP11_NT_MASK, RXEQSTORE4_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG3_ADDR, RXEQSTORE4_TAP_REG3_TAP12_NT_MASK, RXEQSTORE4_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR, RXEQSTORE4_TAP_REG4_TAP13_NT_MASK, RXEQSTORE4_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR, RXEQSTORE4_TAP_REG4_TAP14_NT_MASK, RXEQSTORE4_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG4_ADDR, RXEQSTORE4_TAP_REG4_TAP15_NT_MASK, RXEQSTORE4_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG5_ADDR, RXEQSTORE4_TAP_REG5_TAP16_NT_MASK, RXEQSTORE4_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE4_TAP_REG5_ADDR, RXEQSTORE4_TAP_REG5_TAP17_NT_MASK, RXEQSTORE4_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle2[17]) ));


    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP0_NT_MASK, RXEQSTORE5_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP1_NT_MASK, RXEQSTORE5_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP2_NT_MASK, RXEQSTORE5_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP3_NT_MASK, RXEQSTORE5_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP4_NT_MASK, RXEQSTORE5_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG1_ADDR, RXEQSTORE5_TAP_REG1_TAP5_NT_MASK, RXEQSTORE5_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR, RXEQSTORE5_TAP_REG2_TAP6_NT_MASK, RXEQSTORE5_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR, RXEQSTORE5_TAP_REG2_TAP7_NT_MASK, RXEQSTORE5_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR, RXEQSTORE5_TAP_REG2_TAP8_NT_MASK, RXEQSTORE5_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG2_ADDR, RXEQSTORE5_TAP_REG2_TAP9_NT_MASK, RXEQSTORE5_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR, RXEQSTORE5_TAP_REG3_TAP10_NT_MASK, RXEQSTORE5_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR, RXEQSTORE5_TAP_REG3_TAP11_NT_MASK, RXEQSTORE5_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG3_ADDR, RXEQSTORE5_TAP_REG3_TAP12_NT_MASK, RXEQSTORE5_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR, RXEQSTORE5_TAP_REG4_TAP13_NT_MASK, RXEQSTORE5_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR, RXEQSTORE5_TAP_REG4_TAP14_NT_MASK, RXEQSTORE5_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG4_ADDR, RXEQSTORE5_TAP_REG4_TAP15_NT_MASK, RXEQSTORE5_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG5_ADDR, RXEQSTORE5_TAP_REG5_TAP16_NT_MASK, RXEQSTORE5_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE5_TAP_REG5_ADDR, RXEQSTORE5_TAP_REG5_TAP17_NT_MASK, RXEQSTORE5_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle3[17]) ));


    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP0_NT_MASK, RXEQSTORE6_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP1_NT_MASK, RXEQSTORE6_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP2_NT_MASK, RXEQSTORE6_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP3_NT_MASK, RXEQSTORE6_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP4_NT_MASK, RXEQSTORE6_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG1_ADDR, RXEQSTORE6_TAP_REG1_TAP5_NT_MASK, RXEQSTORE6_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR, RXEQSTORE6_TAP_REG2_TAP6_NT_MASK, RXEQSTORE6_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR, RXEQSTORE6_TAP_REG2_TAP7_NT_MASK, RXEQSTORE6_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR, RXEQSTORE6_TAP_REG2_TAP8_NT_MASK, RXEQSTORE6_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG2_ADDR, RXEQSTORE6_TAP_REG2_TAP9_NT_MASK, RXEQSTORE6_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR, RXEQSTORE6_TAP_REG3_TAP10_NT_MASK, RXEQSTORE6_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR, RXEQSTORE6_TAP_REG3_TAP11_NT_MASK, RXEQSTORE6_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG3_ADDR, RXEQSTORE6_TAP_REG3_TAP12_NT_MASK, RXEQSTORE6_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR, RXEQSTORE6_TAP_REG4_TAP13_NT_MASK, RXEQSTORE6_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR, RXEQSTORE6_TAP_REG4_TAP14_NT_MASK, RXEQSTORE6_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG4_ADDR, RXEQSTORE6_TAP_REG4_TAP15_NT_MASK, RXEQSTORE6_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG5_ADDR, RXEQSTORE6_TAP_REG5_TAP16_NT_MASK, RXEQSTORE6_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE6_TAP_REG5_ADDR, RXEQSTORE6_TAP_REG5_TAP17_NT_MASK, RXEQSTORE6_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle4[17]) ));


    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP0_NT_MASK, RXEQSTORE7_TAP_REG1_TAP0_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[0]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP1_NT_MASK, RXEQSTORE7_TAP_REG1_TAP1_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[1]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP2_NT_MASK, RXEQSTORE7_TAP_REG1_TAP2_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[2]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP3_NT_MASK, RXEQSTORE7_TAP_REG1_TAP3_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[3]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP4_NT_MASK, RXEQSTORE7_TAP_REG1_TAP4_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[4]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG1_ADDR, RXEQSTORE7_TAP_REG1_TAP5_NT_MASK, RXEQSTORE7_TAP_REG1_TAP5_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[5]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR, RXEQSTORE7_TAP_REG2_TAP6_NT_MASK, RXEQSTORE7_TAP_REG2_TAP6_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[6]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR, RXEQSTORE7_TAP_REG2_TAP7_NT_MASK, RXEQSTORE7_TAP_REG2_TAP7_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[7]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR, RXEQSTORE7_TAP_REG2_TAP8_NT_MASK, RXEQSTORE7_TAP_REG2_TAP8_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[8]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG2_ADDR, RXEQSTORE7_TAP_REG2_TAP9_NT_MASK, RXEQSTORE7_TAP_REG2_TAP9_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[9]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR, RXEQSTORE7_TAP_REG3_TAP10_NT_MASK, RXEQSTORE7_TAP_REG3_TAP10_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[10]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR, RXEQSTORE7_TAP_REG3_TAP11_NT_MASK, RXEQSTORE7_TAP_REG3_TAP11_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[11]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG3_ADDR, RXEQSTORE7_TAP_REG3_TAP12_NT_MASK, RXEQSTORE7_TAP_REG3_TAP12_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[12]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR, RXEQSTORE7_TAP_REG4_TAP13_NT_MASK, RXEQSTORE7_TAP_REG4_TAP13_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[13]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR, RXEQSTORE7_TAP_REG4_TAP14_NT_MASK, RXEQSTORE7_TAP_REG4_TAP14_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[14]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG4_ADDR, RXEQSTORE7_TAP_REG4_TAP15_NT_MASK, RXEQSTORE7_TAP_REG4_TAP15_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[15]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG5_ADDR, RXEQSTORE7_TAP_REG5_TAP16_NT_MASK, RXEQSTORE7_TAP_REG5_TAP16_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[16]) ));
    CHECK(pmd_read_field(mss, RXEQSTORE7_TAP_REG5_ADDR, RXEQSTORE7_TAP_REG5_TAP17_NT_MASK, RXEQSTORE7_TAP_REG5_TAP17_NT_OFFSET,   &(dsp_info->pre_ffe_ctle5[17]) ));


    CHECK(pmd_write_field(mss, RX_DFE_ADAPT_REG8_ADDR, RX_DFE_ADAPT_REG8_DBG_BRANCH_SEL_NT_MASK, RX_DFE_ADAPT_REG8_DBG_BRANCH_SEL_NT_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_DFE_ADAPT_RDREG9_ADDR, RX_DFE_ADAPT_RDREG9_COEF_NT_MASK, RX_DFE_ADAPT_RDREG9_COEF_NT_OFFSET, &(dsp_info->dfe) ));

    CHECK(pmd_write_field(mss, RX_SLICER_THRESHOLD_REG12_ADDR, RX_SLICER_THRESHOLD_REG12_DBG_BRANCH_SEL_NT_MASK, RX_SLICER_THRESHOLD_REG12_DBG_BRANCH_SEL_NT_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EH_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EH_NT_OFFSET, &(dsp_info->thresholds.eh) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EZ_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EZ_NT_OFFSET, &(dsp_info->thresholds.ez) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EL_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EL_NT_OFFSET, &(dsp_info->thresholds.el) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG14_ADDR, RX_SLICER_THRESHOLD_RDREG14_LOW_NT_MASK, RX_SLICER_THRESHOLD_RDREG14_LOW_NT_OFFSET, &(dsp_info->thresholds.thres_low) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG14_ADDR, RX_SLICER_THRESHOLD_RDREG14_UPP_NT_MASK, RX_SLICER_THRESHOLD_RDREG14_UPP_NT_OFFSET, &(dsp_info->thresholds.thres_hi) ));

    CHECK(pmd_write_field(mss, RX_SLICER_TARGET_REG12_ADDR, RX_SLICER_TARGET_REG12_DBG_BRANCH_SEL_NT_MASK, RX_SLICER_TARGET_REG12_DBG_BRANCH_SEL_NT_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EL3_NT_MASK, RX_SLICER_TARGET_RDREG13_EL3_NT_OFFSET, &(dsp_info->slicers.el3) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EL1_NT_MASK, RX_SLICER_TARGET_RDREG13_EL1_NT_OFFSET, &(dsp_info->slicers.el1) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EH1_NT_MASK, RX_SLICER_TARGET_RDREG13_EH1_NT_OFFSET, &(dsp_info->slicers.eh1) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG14_ADDR, RX_SLICER_TARGET_RDREG14_EH3_NT_MASK, RX_SLICER_TARGET_RDREG14_EH3_NT_OFFSET, &(dsp_info->slicers.eh3) ));

    
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_TARGET_ADAPT_ENA_A_MASK, RX_CCG_TARGET_ADAPT_ENA_A_OFFSET, target_rx_ccg));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_SLICER_ADAPT_ENA_A_MASK, RX_CCG_SLICER_ADAPT_ENA_A_OFFSET, slicer_rx_ccg));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_FFE_ADAPT_ENA_A_MASK, RX_CCG_FFE_ADAPT_ENA_A_OFFSET, ffe_rx_ccg));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_DFE_ADAPT_ENA_A_MASK, RX_CCG_DFE_ADAPT_ENA_A_OFFSET, eqdfe_rx_ccg));
    CHECK(pmd_write_field(mss, RX_CCG_ADDR, RX_CCG_DC_OFFSET_ADAPT_ENA_A_MASK, RX_CCG_DC_OFFSET_ADAPT_ENA_A_OFFSET, offset_rx_ccg));
    CHECK(pmd_write_field(mss, RX_FFE_ADAPT_REG19_ADDR, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_MASK, RX_FFE_ADAPT_REG19_ADAPT_ENA_A_OFFSET, ffe_adapt_ena_a));
    CHECK(pmd_write_field(mss, RX_SLICER_TARGET_REG10_ADDR, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_MASK, RX_SLICER_TARGET_REG10_ADAPT_ENA_A_OFFSET, target_adapt_ena_a));
    CHECK(pmd_write_field(mss, RX_DFE_ADAPT_REG6_ADDR, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_MASK, RX_DFE_ADAPT_REG6_ADAPT_ENA_A_OFFSET, dfe_adapt_ena_a));
    CHECK(pmd_write_field(mss, RX_DATABLOCK_OFFSET_REG6_ADDR, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_MASK, RX_DATABLOCK_OFFSET_REG6_ADAPT_ENA_A_OFFSET, offset_adapt_ena_a));


    
    if (print_en == 1) {
        USR_PRINTF("Branch |   EL3   |   EL1   |   EH1   |   EH3   \n");
        USR_PRINTF("  %d  |   %d   |   %d   |   %d   |   %d   \n", branch, dsp_info->slicers.el3, dsp_info->slicers.el1, dsp_info->slicers.eh1, dsp_info->slicers.eh3);
    }

    return AW_ERR_CODE_NONE;
}
