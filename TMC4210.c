#include "TMC4210.h"
#include "Spi2.h"

#include "TMC5130A.h"


void TMC4210_Init(uint8_t WichStepper, TMC4210 *T)
{
	T->MUL = 2;
	T->PDIV = 2;
	TMC4210_MUL_DIV(WichStepper, T->MUL, T->PDIV);
	
	
	
	
	
	// 1. Set en_sd to 1 to enable the Step/Dir interface to the driver IC. 
	T->IF_CONFIG = 0x20;
	TMC4210_IFCONFIG(WichStepper, T->IF_CONFIG);
	
	
	//2. Set the velocity parameters V_MIN and V_MAX.

	// This register holds the absolute velocity value at or below which
	// the stepper motor can be stopped abruptly.
	T->V_MIN = 10000;
	TMC4210_VMIN(WichStepper, T->V_MIN);
	
	// This parameter sets the maximum motor velocity. The absolute value of the velocity will not exceed
	// this limit, except if the limit V_MAX is changed during motion to a value below the current velocity. 
	T->V_MAX = 15000;
	TMC4210_VMAX(WichStepper, T->V_MAX);
	
	
	// 3. Set the clock pre-dividers PULSE_DIV and RAMP_DIV.

		
	// The parameter RAMP_DIV scales the acceleration parameter A_MAX
	// The pulse generator clock ? defining the maximum step pulse rate ? is determined by the parameter PULSE_DIV. The parameter
	// PULSE_DIV scales the velocity parameters
	T->PULSE_DIV = 0;
	T->RAMP_DIV = 8;
	TMC4210_PULSE_RAMP_DIV(WichStepper, T->PULSE_DIV, T->RAMP_DIV);
	
	// 4. Set A_MAX with a valid pair of PMUL and PDIV. 
	//This register defines the absolute value of the desired acceleration for velocity_mode and ramp_mode
	//(resp. soft_mode) with a value range from 0 to 2047. 
	T->A_MAX = 1;
	TMC4210_AMAX(WichStepper, T->A_MAX);
	
	//5. Choose the ramp mode with RAMP_MODE register. 

		
	// REFCONF The configuration bits REF_CONF select the behavior of the
	//reference switches.
	// R_M RAMP_MODE Register
	T->REF_CONF = 0;
	T->R_M = TMC_4120_VELOCITY_MODE;
	TMC4210_REF_RAMP(WichStepper, T->REF_CONF, T->R_M);
	
	
	
	
	
	
	//7. Now, the TMC4210 runs a motor if you write either X_TARGET or V_TARGET, depending on the
	// choice of the ramp mode.
	
	//ramp_mode The V_TARGET register holds the current target velocity calculated internally
	// by the ramp generator.
	//velocity_mode A target velocity can be written into the V_TARGET register. The stepper
	//motor accelerates until it reaches the specified target velocity. The velocity is
	//changed according to the motion parameter limits if the register V_TARGET is
	//changed.
	T->V_TARGET = 15000;
	TMC4210_VTARGET(WichStepper, T->V_TARGET);
	
	T->xtarget = 0;
	TMC4210_XTARGET(WichStepper, T->xtarget);	
	
}




void TMC4210_IFCONF(uint8_t WichStepper, uint8_t *intmask, uint8_t *intflags)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;			// 23..16
    DataToWrite[1] = 0;  		// 15.. 8
    DataToWrite[2] = 0;			//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_IF_CONF, &DataToWrite[0], &DataReaded[0]);
}
	
	
void TMC4210_PULSE_RAMP_DIV(uint8_t WichStepper, uint8_t pulse, uint8_t ramp)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
		uint8_t DataReg = 0;
		
		DataReg = pulse & 0x0F;
		DataReg = DataReg << 4;
		DataReg |= (ramp & 0x0F);
						
    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = DataReg;								// 15.. 8
    DataToWrite[2] = 0;											//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_DIV, &DataToWrite[0], &DataReaded[0]);
}

	void TMC4210_INT(uint8_t WichStepper, uint8_t *intmask, uint8_t *intflags)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;			// 23..16
    DataToWrite[1] = 0;  		// 15.. 8
    DataToWrite[2] = 0;			//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_READ, TMC4210_INTERRUPT, &DataToWrite[0], &DataReaded[0]);
		
		*intmask = DataReaded[1];
		*intflags = DataReaded[0];
}


void TMC4210_REF_RAMP(uint8_t WichStepper, uint8_t ref, uint8_t ramp)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = ref & 0x0F;						// 15.. 8
    DataToWrite[2] = ramp & 0x03;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_REF_CONF, &DataToWrite[0], &DataReaded[0]);
}

void TMC4210_MUL_DIV(uint8_t WichStepper, uint8_t mul, uint8_t pdiv)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = mul | 0x80;						// 15.. 8
    DataToWrite[2] = pdiv & 0x0F;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_PMUL_PDIV, &DataToWrite[0], &DataReaded[0]);
}

uint32_t TMC4210_AACTUAL(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    uint32_t DataInReturn = 0;

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x0F;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_READ, TMC4210_A_ACTUAL, &DataToWrite[0], &DataReaded[0]);
		
		DataInReturn |= (DataReaded[0] << 16);
		DataInReturn |= (DataReaded[1] << 8);
		DataInReturn |= (DataReaded[2]);
		
		return(DataInReturn);
}

void TMC4210_AMAX(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x07;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_A_MAX, &DataToWrite[0], &DataReaded[0]);
}

uint32_t TMC4210_VACTUAL(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
		uint8_t DataInReturn = 0;

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x0F;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_READ, TMC4210_V_ACTUAL, &DataToWrite[0], &DataReaded[0]);
		
		DataInReturn |= (DataReaded[0] << 16);
		DataInReturn |= (DataReaded[1] << 8);
		DataInReturn |= (DataReaded[2]);
		
		return(DataInReturn);
}

void TMC4210_VTARGET(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x0F;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_V_TARGET, &DataToWrite[0], &DataReaded[0]);
}



void TMC4210_VMAX(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x07;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_V_MAX, &DataToWrite[0], &DataReaded[0]);
}


void TMC4210_VMIN(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x07;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_V_MIN, &DataToWrite[0], &DataReaded[0]);
}


uint32_t TMC4210_XACTUAL(uint8_t WichStepper, uint32_t xact)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    uint8_t DataInReturn = 0;

    DataToWrite[0] = (xact >> 16) & 0xFF;   	// 23..16
    DataToWrite[1] = (xact >> 8) & 0xFF;  		// 15.. 8
    DataToWrite[2] = xact & 0xFF;							//  7.. 0

		
    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_X_ACTUAL, &DataToWrite[0], &DataReaded[0]);
		
		DataInReturn |= (DataReaded[0] << 16);
		DataInReturn |= (DataReaded[1] << 8);
		DataInReturn |= (DataReaded[2]);
		
		return(DataInReturn);
}


void TMC4210_XTARGET(uint8_t WichStepper, uint32_t xtarget)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = (xtarget >> 16) & 0xFF;   	// 23..16
    DataToWrite[1] = (xtarget >> 8) & 0xFF;  		// 15.. 8
    DataToWrite[2] = xtarget & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_X_TARGET, &DataToWrite[0], &DataReaded[0]);
}



void TMC4210_IFCONFIG(uint8_t WichStepper, uint16_t val)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    

    DataToWrite[0] = 0;											// 23..16
    DataToWrite[1] = (val >> 8) & 0x01;  		// 15.. 8
    DataToWrite[2] = val & 0xFF;						//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_IF_CONF, &DataToWrite[0], &DataReaded[0]);
}



void TMC4210_Stepper_Init(uint8_t WichStepper, uint8_t mode)
{
    uint8_t status_reg = 0;
    uint8_t DataToWrite[3] = {0};
    uint8_t DataReaded[3] = {0};
    


    // mode : ramp mode 3 -> Hold 2-> Velocity negative 1-> Velocity positive 0-> positionning mode (A,D,V params)
    DataToWrite[0] = 0b00000000;   		// 23..16
    DataToWrite[1] = 0b00000000;   		// 15.. 8
    DataToWrite[2] = mode & 0x03;   	//  7.. 0


    // 	Proceed the transfert to the good Stepper
		status_reg = TMC4210_TransactionReg(WichStepper, TMC4210_WRITE, TMC4210_REF_CONF, &DataToWrite[0], &DataReaded[0]);
		
}


uint8_t TMC4210_TransactionReg(uint8_t WichStepper, uint8_t RW, uint8_t reg_adr, uint8_t *pTransmitData, uint8_t *pReceiveData)
{
    uint8_t first_byte = 0;
    uint8_t status_reg = 0;
    
		first_byte = reg_adr << 1;
		first_byte |= RW;
							
		
		
		//CS_TMC4210_X = 0;
		CS_Y = 0;
						
		// first transaction containt the addr and RW bit
    SPI2_Exchange(&first_byte, &status_reg);
    
    SPI2_Exchange(pTransmitData,pReceiveData);
    SPI2_Exchange(pTransmitData+1,pReceiveData);
    SPI2_Exchange(pTransmitData+2,pReceiveData);
		
		//CS_TMC4210_X = 1;
		CS_Y = 1;
		
		return(status_reg);
}




