// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Alphawave IP Inc. All rights reserved.
 */







#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <svdpi.h>
#include "aw_alphacore.h"



#if defined(DO_NOT_PRINT)
#define printf(...) do {} while (0)
#endif




int serdes_init(mss_access_t *mss, uint32_t lane_offset, uint32_t phy_offset) {
    mss->lane_offset = lane_offset;
    mss->phy_offset = phy_offset;
    return 0;
}




void write_csr(uint32_t addr, uint32_t wdata) {
    sv_write_csr(addr, wdata);
}

void read_csr(uint32_t addr, uint32_t *rdata) {
    sv_read_csr(addr, rdata);
}

int delay_us(int x) {
    sv_delay_us(x);

    return 0;
}

void simple_print(const char *fmt, ...) {
   char str[200];
   va_list args;

   va_start(args,fmt);
   vsnprintf(str,200,fmt,args);
   sv_print(str);
   va_end(args);
}

int c_test_api_write(void) {
    TRACE_ENTER("%s", "No args");
    uint32_t wval = 0x7FF;
    write_csr(0x00000000, wval);
    printf("[aw_driver_sim.c] Wrote value: %d\n", wval);
    TRACE_EXIT("%s", "No args");
    return 0;
}

int c_test_api_read(void) {
    TRACE_ENTER("%s", "No args");
    uint32_t rddata;
    read_csr(0x00000000, &rddata);
    printf("[aw_driver_sim.c] Read Value:  %d\n", rddata);
    TRACE_EXIT("%s", "No args");
    return 0;
}




int pmd_set_lane(mss_access_t *mss, uint32_t lane){
    
    if (lane == 99) {
        mss->lane_offset = LANE_BROADCAST;
        return 0;
    } else if (lane > LANE_MAX) {
        printf("[pmd_set_lane]: Lane number %d does not exist in the design\n", lane);
        return 1;
    } else {
        mss->lane_offset = (lane) * LANE0_OFFSET;
        return 0;
    }
}

int pmd_get_lane(mss_access_t *mss, uint32_t *lane){
    uint32_t lane_temp;
    
    if (mss->lane_offset == LANE_BROADCAST) {
        *lane = 99;
    } else {
        lane_temp = mss->lane_offset / LANE0_OFFSET ;
        if (lane_temp > LANE_MAX) {
            printf("[pmd_get_lane]: Lane number %d does not exist in the design\n", lane_temp);
            return 1;
        } else {
            *lane = lane_temp;
        }
    }
    return 0;
}


int pmd_write_addr(mss_access_t *mss, uint32_t addr, uint32_t value){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET ||  addr >= LANE_BROADCAST) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    printf("[pmd_write_addr]: Writing addr: 0x%08X, value: %d\n", final_addr, value); 
    write_csr(final_addr, value);


    return 0;
}

int pmd_read_addr(mss_access_t *mss, uint32_t addr, uint32_t *rdval){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST && final_addr < SRAM_OFFSET) {
        printf("[pmd_read_addr]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }
    printf("[pmd_read_addr]: Reading addr: 0x%08X\n", final_addr);
    read_csr(final_addr, rdval);
    return 0;
}

int pmd_write_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t wval){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else if (mss->lane_offset == LANE_BROADCAST) {
        final_addr = addr + mss->lane_offset - LANE0_OFFSET + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }

    printf("[pmd_write_field]: Writing addr: 0x%08X, value: %d\n", final_addr, wval);
    uint32_t reg_read;
    wval = wval << fld_offset;
    read_csr(final_addr, &reg_read);
    wval = (wval & fld_mask) | (reg_read & ~fld_mask);
    write_csr(final_addr, wval);


    return 0;
}

int pmd_read_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t *rdval){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    }
    else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    printf("[pmd_read_field]: Reading addr: 0x%08X\n", final_addr);
    uint32_t rddata;
    read_csr(final_addr, &rddata);
    printf("[pmd_read_field]: Read value: 0x%08X\n", rddata);
    uint32_t fld_val = ((rddata & fld_mask) >> fld_offset);
    *rdval = fld_val;


    return 0;
}

int pmd_read_check_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, aw_rd_opcode_t rd_opcode, uint32_t *rdval, uint32_t rdcheck_val1, uint32_t rdcheck_val2){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET) { 
        final_addr = addr + mss->phy_offset;
    }
    else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_read_check_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    printf("[pmd_read_check_field]: Reading addr: 0x%08X\n", final_addr);
    uint32_t rddata;
    read_csr(final_addr, &rddata);
    printf("[pmd_read_check_field]: Read value: 0x%08X\n", rddata);
    uint32_t fld_val = ((rddata & fld_mask) >> fld_offset);
    *rdval = fld_val;



    if (rd_opcode == RD_EQ) {
        if (fld_val == rdcheck_val1) {
            printf("[pmd_read_check_field]: Register check passed. Expected value = 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected value = 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_GT) {
        if (fld_val > rdcheck_val1) {
            printf("[pmd_read_check_field]: Register check passed. Expected value > 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected > 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_GTE) {
        if (fld_val >= rdcheck_val1) {
            printf("[pmd_read_check_field]: Register check passed. Expected value >= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected value >= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_LT) {
        if (fld_val < rdcheck_val1) {
            printf("[pmd_read_check_field]: Register check passed. Expected value < 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected < 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_LTE) {
        if (fld_val <= rdcheck_val1) {
            printf("[pmd_read_check_field]: Register check passed. Expected value <= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected <= 0x%X. Field value = 0x%X\n", rdcheck_val1, fld_val);
            return -1;
        }
    } else if (rd_opcode == RD_RANGE) {
        if (fld_val >= rdcheck_val1 && fld_val <= rdcheck_val2) {
            printf("[pmd_read_check_field]: Register check passed. Expected range = 0x%X -> 0x%X. Field value = 0x%X\n", rdcheck_val1, rdcheck_val2, fld_val);
            return 0;
        } else {
            printf("[pmd_read_check_field]: ERROR. Register check failed. Expected range = 0x%X -> 0x%X. Field value = 0x%X\n", rdcheck_val1, rdcheck_val2, fld_val);
            return -1;
       	}

    } else {
        printf("[pmd_read_check_field]: ERROR. Invalid read opcode");
        return -1;
    }

}



int pmd_poll_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t poll_val, uint32_t timeout_us){
    uint32_t final_addr;
    if (addr < LANE0_OFFSET) {
        final_addr = addr + mss->phy_offset;
    } else {
        final_addr = addr + mss->lane_offset + mss->phy_offset;
    }
    if (final_addr >= LANE_BROADCAST) {
        printf("[pmd_poll_field]: Cannot read register while mss.lane_offset has lane broadcast set.\n");
        return 1;
    }

    printf("[pmd_poll_field]: Using SIM version\n");
    printf("[pmd_poll_field]: Polling addr: 0x%08X\n", final_addr);
    uint32_t rddata = 0;
    uint32_t fld_val = 0;
    uint32_t i = 0;
    while (i < timeout_us) {
        i++;
        USR_SLEEP(1);
        read_csr(final_addr, &rddata);
        
        fld_val = ((rddata & fld_mask) >> fld_offset);
        printf("[pmd_poll_field]: Field value: 0x%X\n", fld_val);
        if (fld_val == poll_val) {
            break;
        }
    }
    if (fld_val == poll_val) {
        printf("[pmd_poll_field]: Polling successful after %d us\n", i);
        return 0;
    } else {
        printf("[pmd_poll_field]: Polling timed out after %d us\n", timeout_us);
        return -1;
    }
}


int pmd_ate_vec_comment(char comment[]) {
    printf("[pmd_ate_vec_comment]: %s\n", comment);
    return 0;
}
