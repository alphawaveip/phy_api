 

 

#ifndef __aw_driver_sim
#define __aw_driver_sim

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "svdpi.h"
#include "aw_alphacore_csr_defines.h"


 
extern void sv_write_csr(uint32_t addr, uint32_t wdata);
extern void sv_read_csr(uint32_t addr, uint32_t *rdata);
extern void sv_delay_us(int delay_us);
extern void sv_print(char *str);

#define LANE0_OFFSET 0x02000000  

#define USR_SLEEP(x) \
    delay_us(x);

#ifndef USR_PRINTF
#define USR_PRINTF(...) simple_print(__VA_ARGS__)
#endif

 
typedef struct mss_access_s {
     
    uint32_t phy_offset;  
    uint32_t lane_offset;  

     
     
     
     
     
     
     
     
     
     
} mss_access_t;

typedef enum aw_rd_opcode_e {
    RD_EQ    = 0,  
    RD_GT    = 1,  
    RD_GTE   = 2,  
    RD_LT    = 3,  
    RD_LTE   = 4,  
    RD_RANGE = 5   
} aw_rd_opcode_t;

int serdes_init(mss_access_t *mss, uint32_t lane_offset, uint32_t phy_offset);
void write_csr(uint32_t addr, uint32_t wdata);
void read_csr(uint32_t addr, uint32_t *rdata);
int delay_us(int x);
void simple_print(const char *fmt, ...);
int c_test_api_write(void);
int c_test_api_read(void);
int pmd_set_lane(mss_access_t *mss, uint32_t lane);
int pmd_write_addr(mss_access_t *mss, uint32_t addr, uint32_t value);
int pmd_read_addr(mss_access_t *mss, uint32_t addr, uint32_t *rdval);
int pmd_write_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t wval);
int pmd_read_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t *rdval);
int pmd_read_check_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, aw_rd_opcode_t rd_opcode, uint32_t *rdval, uint32_t rdcheck_val1, uint32_t rdcheck_val2);
int pmd_poll_field(mss_access_t *mss, uint32_t addr, uint32_t fld_mask, uint32_t fld_offset, uint32_t poll_val, uint32_t timeout_us);
int pmd_ate_vec_comment(char comment[]);

#endif  
