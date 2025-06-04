#include "TMC4210.h"
#include "Spi2.h"
#include "TMC5130A.h"

static void TMC4210_WriteRegister(uint8_t stepper, uint8_t reg, uint8_t *data)
{
    uint8_t dummy[3];
    TMC4210_TransactionReg(stepper, TMC4210_WRITE, reg, data, dummy);
}

static uint32_t TMC4210_ReadRegister(uint8_t stepper, uint8_t reg)
{
    uint8_t tx[3] = {0}, rx[3] = {0};
    TMC4210_TransactionReg(stepper, TMC4210_READ, reg, tx, rx);
    return (rx[0] << 16) | (rx[1] << 8) | rx[2];
}

void TMC4210_Init(uint8_t stepper, TMC4210 *t)
{
    t->MUL = 2;
    t->PDIV = 2;
    TMC4210_MUL_DIV(stepper, t->MUL, t->PDIV);

    t->IF_CONFIG = 0x20;
    TMC4210_IFCONFIG(stepper, t->IF_CONFIG);

    t->V_MIN = 10000;
    TMC4210_VMIN(stepper, t->V_MIN);

    t->V_MAX = 15000;
    TMC4210_VMAX(stepper, t->V_MAX);

    t->PULSE_DIV = 0;
    t->RAMP_DIV = 8;
    TMC4210_PULSE_RAMP_DIV(stepper, t->PULSE_DIV, t->RAMP_DIV);

    t->A_MAX = 1;
    TMC4210_AMAX(stepper, t->A_MAX);

    t->REF_CONF = 0;
    t->R_M = TMC_4120_VELOCITY_MODE;
    TMC4210_REF_RAMP(stepper, t->REF_CONF, t->R_M);

    t->V_TARGET = 15000;
    TMC4210_VTARGET(stepper, t->V_TARGET);

    t->xtarget = 0;
    TMC4210_XTARGET(stepper, t->xtarget);
}

#define WRITE_REG_FUNC(name, reg_addr, mask_high) \
void name(uint8_t stepper, uint16_t val) { \
    uint8_t data[3] = {0, (val >> 8) & mask_high, val & 0xFF}; \
    TMC4210_WriteRegister(stepper, reg_addr, data); \
}

#define WRITE_REG32_FUNC(name, reg_addr) \
void name(uint8_t stepper, uint32_t val) { \
    uint8_t data[3] = {(val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF}; \
    TMC4210_WriteRegister(stepper, reg_addr, data); \
}

#define READ_REG32_FUNC(name, reg_addr) \
uint32_t name(uint8_t stepper, uint16_t val) { \
    (void)val; \
    return TMC4210_ReadRegister(stepper, reg_addr); \
}

WRITE_REG_FUNC(TMC4210_VMIN, TMC4210_V_MIN, 0x07)
WRITE_REG_FUNC(TMC4210_VMAX, TMC4210_V_MAX, 0x07)
WRITE_REG_FUNC(TMC4210_VTARGET, TMC4210_V_TARGET, 0x0F)
WRITE_REG_FUNC(TMC4210_AMAX, TMC4210_A_MAX, 0x07)

WRITE_REG32_FUNC(TMC4210_XTARGET, TMC4210_X_TARGET)
WRITE_REG32_FUNC(TMC4210_IFCONFIG, TMC4210_IF_CONF)

READ_REG32_FUNC(TMC4210_AACTUAL, TMC4210_A_ACTUAL)
READ_REG32_FUNC(TMC4210_VACTUAL, TMC4210_V_ACTUAL)

uint32_t TMC4210_XACTUAL(uint8_t stepper, uint32_t val)
{
    uint8_t tx[3] = {(val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF}, rx[3];
    TMC4210_TransactionReg(stepper, TMC4210_WRITE, TMC4210_X_ACTUAL, tx, rx);
    return (rx[0] << 16) | (rx[1] << 8) | rx[2];
}

void TMC4210_MUL_DIV(uint8_t stepper, uint8_t mul, uint8_t pdiv)
{
    uint8_t data[3] = {0, mul | 0x80, pdiv & 0x0F};
    TMC4210_WriteRegister(stepper, TMC4210_PMUL_PDIV, data);
}

void TMC4210_PULSE_RAMP_DIV(uint8_t stepper, uint8_t pulse, uint8_t ramp)
{
    uint8_t data[3] = {0, (pulse << 4) | (ramp & 0x0F), 0};
    TMC4210_WriteRegister(stepper, TMC4210_DIV, data);
}

void TMC4210_REF_RAMP(uint8_t stepper, uint8_t ref, uint8_t ramp)
{
    uint8_t data[3] = {0, ref & 0x0F, ramp & 0x03};
    TMC4210_WriteRegister(stepper, TMC4210_REF_CONF, data);
}

void TMC4210_INT(uint8_t stepper, uint8_t *intmask, uint8_t *intflags)
{
    uint8_t tx[3] = {0}, rx[3];
    TMC4210_TransactionReg(stepper, TMC4210_READ, TMC4210_INTERRUPT, tx, rx);
    *intmask = rx[1];
    *intflags = rx[0];
}

void TMC4210_IFCONF(uint8_t stepper, uint8_t *intmask, uint8_t *intflags)
{
    uint8_t tx[3] = {0}, rx[3];
    TMC4210_TransactionReg(stepper, TMC4210_WRITE, TMC4210_IF_CONF, tx, rx);
}

void TMC4210_Stepper_Init(uint8_t stepper, uint8_t mode)
{
    uint8_t data[3] = {0, 0, mode & 0x03};
    TMC4210_WriteRegister(stepper, TMC4210_REF_CONF, data);
}

uint8_t TMC4210_TransactionReg(uint8_t stepper, uint8_t RW, uint8_t reg_adr, uint8_t *tx, uint8_t *rx)
{
    uint8_t first_byte = (reg_adr << 1) | RW;
    uint8_t status = 0;

    CS_Y = 0;
    SPI2_Exchange(&first_byte, &status);
    SPI2_Exchange(tx, rx);
    SPI2_Exchange(tx + 1, rx + 1);
    SPI2_Exchange(tx + 2, rx + 2);
    CS_Y = 1;

    return status;
}
