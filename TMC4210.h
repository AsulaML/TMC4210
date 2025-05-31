#ifndef TMC4210_H
#define	TMC4210_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    //  Section: Included Files
    #include "main.h"
    
    #define CS_TMC4210_X LATGbits.LATG7      
    
    #define TMC4210_WRITE 0
    #define TMC4210_READ 1 

    #define TMC4210_X_TARGET 0
    #define TMC4210_X_ACTUAL 1
    #define TMC4210_V_MIN 2
    #define TMC4210_V_MAX 3
    #define TMC4210_V_TARGET 4
    #define TMC4210_V_ACTUAL 5
    #define TMC4210_A_MAX 6
    #define TMC4210_A_ACTUAL 7
    #define TMC4210_PMUL_PDIV 9
    #define TMC4210_REF_CONF 10
    #define TMC4210_INTERRUPT 11
    #define TMC4210_DIV 12
    #define TMC4210_DX_REF 13
    #define TMC4210_X_LATCHED 14
    #define TMC4210_USTEP_COUNT 15
    #define TMC4210_IF_CONF 20
    #define TMC4210_POS_COMP 21
    #define TMC4210_POS_COMP_INT 22
    #define TMC4210_POWER_DOWN 24
    #define TMC4210_TYPE_VERSION 25
    #define TMC4210_GPIO_READ 26
    #define TMC4210_STPDIV 63
    
    typedef struct TMC4210_REG
    {
        uint32_t xtarget;
        
        uint16_t V_MIN;
        uint16_t V_MAX;
        uint16_t V_TARGET;
        uint16_t A_MAX;
        
        uint8_t MUL;
        uint8_t PDIV;
        uint8_t PULSE_DIV;
        uint8_t RAMP_DIV;
        
        uint8_t REF_CONF;
        uint8_t R_M;
        uint16_t IF_CONFIG;
        
    }TMC4210;
    
    
    
    void TMC4210_Init(uint8_t WichStepper, TMC4210 *T);
    void TMC4210_IFCONF(uint8_t WichStepper, uint8_t *intmask, uint8_t *intflags);
    void TMC4210_PULSE_RAMP_DIV(uint8_t WichStepper, uint8_t pulse, uint8_t ramp);
    void TMC4210_INT(uint8_t WichStepper, uint8_t *intmask, uint8_t *intflags);
    void TMC4210_REF_RAMP(uint8_t WichStepper, uint8_t ref, uint8_t ramp);
    void TMC4210_MUL_DIV(uint8_t WichStepper, uint8_t mul, uint8_t pdiv);
    uint32_t TMC4210_AACTUAL(uint8_t WichStepper, uint16_t val);
    void TMC4210_AMAX(uint8_t WichStepper, uint16_t val);
    uint32_t TMC4210_VACTUAL(uint8_t WichStepper, uint16_t val);
    void TMC4210_VTARGET(uint8_t WichStepper, uint16_t val);
    void TMC4210_VMAX(uint8_t WichStepper, uint16_t val);
    void TMC4210_VMIN(uint8_t WichStepper, uint16_t val);
    uint32_t TMC4210_XACTUAL(uint8_t WichStepper, uint32_t xact);
    void TMC4210_XTARGET(uint8_t WichStepper, uint32_t xtarget);
    void TMC4210_Stepper_Init(uint8_t WichStepper, uint8_t mode);
    uint8_t TMC4210_TransactionReg(uint8_t WichStepper, uint8_t RW, uint8_t reg_adr, uint8_t *pTransmitData, uint8_t *pReceiveData);
    void TMC4210_IFCONFIG(uint8_t WichStepper, uint16_t val);
    
    
    #define TMC_4120_RAMP_MODE 0b00
    #define TMC_4120_VELOCITY_MODE 0b10
    

    
    
        
#ifdef	__cplusplus
}
#endif

#endif