// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */

#include "aw_alphacore.h"
#include "aw_pmd_rx_dsp_get.h"

 
int aw_pmd_rx_dsp_get(mss_access_t *mss, uint32_t branch, aw_dsp_param_t *dsp_info){
     
    CHECK(pmd_write_field(mss, RX_DATABLOCK_OFFSET_REG1_ADDR, RX_DATABLOCK_OFFSET_REG1_BRANCH_SEL_A_MASK, RX_DATABLOCK_OFFSET_REG1_BRANCH_SEL_A_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_DATABLOCK_OFFSET_RDREG9_ADDR, RX_DATABLOCK_OFFSET_RDREG9_OFFSET_NT_MASK, RX_DATABLOCK_OFFSET_RDREG9_OFFSET_NT_OFFSET, &(dsp_info->dc_offset) ));
    CHECK(pmd_write_field(mss, RX_FFE_ADAPT_REG1_ADDR, RX_FFE_ADAPT_REG1_BRANCH_SEL_A_MASK, RX_FFE_ADAPT_REG1_BRANCH_SEL_A_OFFSET, branch));

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


    CHECK(pmd_write_field(mss, RX_DFE_ADAPT_REG1_ADDR, RX_DFE_ADAPT_REG1_BRANCH_SEL_A_MASK, RX_DFE_ADAPT_REG1_BRANCH_SEL_A_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_DFE_ADAPT_RDREG9_ADDR, RX_DFE_ADAPT_RDREG9_COEF_NT_MASK, RX_DFE_ADAPT_RDREG9_COEF_NT_OFFSET, &(dsp_info->dfe) ));

    CHECK(pmd_write_field(mss, RX_SLICER_THRESHOLD_REG1_ADDR, RX_SLICER_THRESHOLD_REG1_BRANCH_SEL_A_MASK, RX_SLICER_THRESHOLD_REG1_BRANCH_SEL_A_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EH_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EH_NT_OFFSET, &(dsp_info->thresholds.eh) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EZ_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EZ_NT_OFFSET, &(dsp_info->thresholds.ez) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG13_ADDR, RX_SLICER_THRESHOLD_RDREG13_EL_NT_MASK, RX_SLICER_THRESHOLD_RDREG13_EL_NT_OFFSET, &(dsp_info->thresholds.el) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG14_ADDR, RX_SLICER_THRESHOLD_RDREG14_LOW_NT_MASK, RX_SLICER_THRESHOLD_RDREG14_LOW_NT_OFFSET, &(dsp_info->thresholds.thres_low) ));
    CHECK(pmd_read_field(mss, RX_SLICER_THRESHOLD_RDREG14_ADDR, RX_SLICER_THRESHOLD_RDREG14_UPP_NT_MASK, RX_SLICER_THRESHOLD_RDREG14_UPP_NT_OFFSET, &(dsp_info->thresholds.thres_hi) ));

    CHECK(pmd_write_field(mss, RX_SLICER_TARGET_REG1_ADDR, RX_SLICER_TARGET_REG1_BRANCH_SEL_A_MASK, RX_SLICER_TARGET_REG1_BRANCH_SEL_A_OFFSET, branch));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EL3_NT_MASK, RX_SLICER_TARGET_RDREG13_EL3_NT_OFFSET, &(dsp_info->slicers.el3) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EL1_NT_MASK, RX_SLICER_TARGET_RDREG13_EL1_NT_OFFSET, &(dsp_info->slicers.el1) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG13_ADDR, RX_SLICER_TARGET_RDREG13_EH1_NT_MASK, RX_SLICER_TARGET_RDREG13_EH1_NT_OFFSET, &(dsp_info->slicers.eh1) ));
    CHECK(pmd_read_field(mss, RX_SLICER_TARGET_RDREG14_ADDR, RX_SLICER_TARGET_RDREG14_EH3_NT_MASK, RX_SLICER_TARGET_RDREG14_EH3_NT_OFFSET, &(dsp_info->slicers.eh3) ));

     
    USR_PRINTF("Branch |   EL3   |   EL1   |   EH1   |   EH3   \n");
    USR_PRINTF("  %d  |   %d   |   %d   |   %d   |   %d   \n", branch, dsp_info->slicers.el3, dsp_info->slicers.el1, dsp_info->slicers.eh1, dsp_info->slicers.eh3);
    return AW_ERR_CODE_NONE;
}
