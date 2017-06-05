#ifndef SYSTEMCONSTANTS_H
#define SYSTEMCONSTANTS_H

#define OPCODEFIELD_LENGTH  6   // machine instruction field lengths
#define FUNCTFIELD_LENGTH   6
#define REGFIELD_LENGTH     5
#define SHFTFIELD_LENGTH    5
#define IMMFIELD_LENGTH    16
#define TARGET_LENGTH	   26		

#define OPCODE_OFFSET       0   //  0 -  5
#define REG1_OFFSET         6   //  6 - 10
#define REG2_OFFSET        11   // 11 - 15
#define REG3_OFFSET        16   // 16 - 20
#define SHMT_OFFSET        21   // 21 - 25
#define FUNCT_OFFSET       26   // 26 - 31
#define IMM_OFFSET         16   // 16 - 31
#define TARGET_OFFSET       6   //  6 - 31

#define INSTRNAME_LENGTH    8   // used in IData.c, Instruction.h
#define REGNAME_LENGTH      5   // includes '$'

#define MILength           32   // used in Instruction.h

#endif

