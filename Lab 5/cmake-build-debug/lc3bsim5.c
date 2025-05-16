/*
    Name 1: Your Name
    UTEID 1: Your UTEID
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Simulator                                           */
/*                                                             */
/*   EE 460N - Lab 5                                           */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files:  ucode        Microprogram file                      */
/*         pagetable    page table in LC-3b machine language   */
/*         isaprogram   LC-3b machine language program file    */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void eval_micro_sequencer();
void cycle_memory();
void eval_bus_drivers();
void drive_bus();
void latch_datapath_values();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Definition of the control store layout.                     */
/***************************************************************/
#define CONTROL_STORE_ROWS 64
#define INITIAL_STATE_NUMBER 18

/***************************************************************/
/* Definition of bit order in control store word.              */
/***************************************************************/
enum CS_BITS {
    IRD,
    COND2, COND1, COND0,
    J5, J4, J3, J2, J1, J0,
    LD_MAR,
    LD_MDR,
    LD_IR,
    LD_BEN,
    LD_REG,
    LD_CC,
    LD_PC,
    GATE_PC,
    GATE_MDR,
    GATE_ALU,
    GATE_MARMUX,
    GATE_SHF,
    PCMUX1, PCMUX0,
    DRMUX1, DRMUX0,
    SR1MUX1, SR1MUX0,
    ADDR1MUX,
    ADDR2MUX1, ADDR2MUX0,
    MARMUX,
    ALUK1, ALUK0,
    MIO_EN,
    R_W,
    DATA_SIZE,
    LSHF1,

   /*added for lab 4*/
    REGSelMUX,
    REGDecMUX,
    SPMUX,
    MARDecMUX,
    VectMUX,
    CCMUX,
    MARSel_MUX,
    LD_PSR,
    LD_SSP,
    LD_USP,
    LD_Vector,
    LD_Excep,
    ClrExcep,
    GatePC_2,
    GateSP,
    GatePSR,
    GateTable,
    CLR_PSR15,
    CLR_INTV,

    /*added for lab 5*/
    SetUA,
    SetACV,
    SetPageFault,
    LD_VA,
    Gate_VPN,
    Gate_Offset,
    Gate_PBTR,
    Load_PFN,
    Update_PTE,
    JBit_Out1,
    JBit_Out0,
    LD_MDR_Temp,
    Restore_MDR,
    LD_JBitReg,



    /* MODIFY: you have to add all your new control signals */
    CONTROL_STORE_BITS
} CS_BITS;

/***************************************************************/
/* Functions to get at the control bits.                       */
/***************************************************************/
int GetIRD(int *x)           { return(x[IRD]); }
int GetCOND(int *x)          { return((x[COND2]<<2) + (x[COND1] << 1) + x[COND0]); }
int GetJ(int *x)             { return((x[J5] << 5) + (x[J4] << 4) +
                                      (x[J3] << 3) + (x[J2] << 2) +
                                      (x[J1] << 1) + x[J0]); }
int GetLD_MAR(int *x)        { return(x[LD_MAR]); }
int GetLD_MDR(int *x)        { return(x[LD_MDR]); }
int GetLD_IR(int *x)         { return(x[LD_IR]); }
int GetLD_BEN(int *x)        { return(x[LD_BEN]); }
int GetLD_REG(int *x)        { return(x[LD_REG]); }
int GetLD_CC(int *x)         { return(x[LD_CC]); }
int GetLD_PC(int *x)         { return(x[LD_PC]); }
int GetGATE_PC(int *x)       { return(x[GATE_PC]); }
int GetGATE_MDR(int *x)      { return(x[GATE_MDR]); }
int GetGATE_ALU(int *x)      { return(x[GATE_ALU]); }
int GetGATE_MARMUX(int *x)   { return(x[GATE_MARMUX]); }
int GetGATE_SHF(int *x)      { return(x[GATE_SHF]); }
int GetPCMUX(int *x)         { return((x[PCMUX1] << 1) + x[PCMUX0]); }
int GetDRMUX(int *x)         { return((x[DRMUX1]<<1) + (x[DRMUX0])); }
int GetSR1MUX(int *x)        { return((x[SR1MUX1]<<1) + (x[SR1MUX0])); }
int GetADDR1MUX(int *x)      { return(x[ADDR1MUX]); }
int GetADDR2MUX(int *x)      { return((x[ADDR2MUX1] << 1) + x[ADDR2MUX0]); }
int GetMARMUX(int *x)        { return(x[MARMUX]); }
int GetALUK(int *x)          { return((x[ALUK1] << 1) + x[ALUK0]); }
int GetMIO_EN(int *x)        { return(x[MIO_EN]); }
int GetR_W(int *x)           { return(x[R_W]); }
int GetDATA_SIZE(int *x)     { return(x[DATA_SIZE]); }
int GetLSHF1(int *x)         { return(x[LSHF1]); }
/* MODIFY: you can add more Get functions for your new control signals */
int GetREGSelMUX(int *x)     { return(x[REGSelMUX]); }
int GetREGDecMUX(int *x)     { return(x[REGDecMUX]); }
int GetSPMUX(int *x)         { return(x[SPMUX]); }
int GetMARDecMUX(int *x)     { return(x[MARDecMUX]); }
int GetVectMUX(int *x)       { return(x[VectMUX]); }
//int GetCCMUX(int *x)         { return(x[CCMUX]); }
int GetMARSel_MUX(int *x)    { return(x[MARSel_MUX]); }
int GetLD_PSR(int *x)        { return(x[LD_PSR]); }
int GetLD_SSP(int *x)        { return(x[LD_SSP]); }
int GetLD_USP(int *x)        { return(x[LD_USP]); }
int GetLD_Vector(int *x)     { return(x[LD_Vector]); }
int GetLD_Excep(int *x)      { return(x[LD_Excep]); }
int GetClrExcep(int *x)      { return(x[ClrExcep]); }
int GetGatePC_2(int *x)      { return(x[GatePC_2]); }
int GetGateSP(int *x)        { return(x[GateSP]); }
int GetGatePSR(int *x)       { return(x[GatePSR]); }
int GetGateTable(int *x)     { return(x[GateTable]); }
int GetCLR_PSR15(int *x)     { return(x[CLR_PSR15]);}
int GetCLR_INTV(int *x)      { return(x[CLR_INTV]);}

/*added for lab 5*/
int GetSetUA(int *x)         { return(x[SetUA]);}
int GetSetACV(int *x)        { return(x[SetACV]);}
int GetSetPageFault(int *x)  { return(x[SetPageFault]);}
int GetLD_VA(int *x)         { return(x[LD_VA]);}
int GetGate_VPN(int *x)      { return(x[Gate_VPN]);}
int GetGate_Offset(int *x)   { return(x[Gate_Offset]);}
int GetGate_PTBR(int *x)     { return(x[Gate_PBTR]);}
int GetLoad_PFN(int *x)      { return(x[Load_PFN]);}
int GetUpdate_PTE(int *x)    { return(x[Update_PTE]);}
int GetJbitOut(int *x)       { return((x[JBit_Out1]<<1) +
                                         x[JBit_Out0]);}
int GetLDMDRTemp(int *x)     {return((x[LD_MDR_Temp]));}
int GetRestoreMDR(int *x)    {return(x[Restore_MDR]);}
int GetLD_JBITREG(int *x)    { return (x[LD_JBitReg]);}

/* MODIFY: you can add more Get functions for your new control signals */

/***************************************************************/
/* The control store rom.                                      */
/***************************************************************/
int CONTROL_STORE[CONTROL_STORE_ROWS][CONTROL_STORE_BITS];

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A
   There are two write enable signals, one for each byte. WE0 is used for
   the least significant byte of a word. WE1 is used for the most significant
   byte of a word. */

#define WORDS_IN_MEM    0x2000 /* 32 frames */
#define MEM_CYCLES      5
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */
int BUS;	/* value of the bus */

typedef struct System_Latches_Struct{

    int PC,		/* program counter */
    MDR,	/* memory data register */
    MAR,	/* memory address register */
    IR,		/* instruction register */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P,		/* p condition bit */
    BEN;        /* ben register */

    int READY;	/* ready bit */
    /* The ready bit is also latched as you dont want the memory system to assert it
       at a bad point in the cycle*/

    int REGS[LC_3b_REGS]; /* register file. */

    int MICROINSTRUCTION[CONTROL_STORE_BITS]; /* The microintruction */

    int STATE_NUMBER; /* Current State Number - Provided for debugging */

/* For lab 4 */
    int INTV; /* Interrupt vector register */
    int EXCV; /* Exception vector register */
    int SSP; /* Initial value of system stack pointer */
    int USP; /*Inital value of the user stack*/
    int Vector; /*Vector register*/
    int Table; /*register holding 0x200*/
    int PSR;

/* MODIFY: you should add here any other registers you need to implement interrupts and exceptions */

/* For lab 5 */
    int PTBR; /* This is initialized when we load the page table */
    int VA;   /* Temporary VA register */
    int JBITREG; /*stores next j bits before address translation*/
    int MDR_Temp;
/* MODIFY: you should add here any other registers you need to implement virtual memory */

} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/* For lab 5 */
#define PAGE_NUM_BITS 9
#define PTE_PFN_MASK 0x3E00
#define PTE_VALID_MASK 0x0004
#define PAGE_OFFSET_MASK 0x1FF

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int CYCLE_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands.                   */
/*                                                             */
/***************************************************************/
void help() {
    printf("----------------LC-3bSIM Help-------------------------\n");
    printf("go               -  run program to completion       \n");
    printf("run n            -  execute program for n cycles    \n");
    printf("mdump low high   -  dump memory from low to high    \n");
    printf("rdump            -  dump the register & bus values  \n");
    printf("?                -  display this help menu          \n");
    printf("quit             -  exit the program                \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {

    eval_micro_sequencer();
    cycle_memory();
    eval_bus_drivers();
    drive_bus();
    latch_datapath_values();

    CURRENT_LATCHES = NEXT_LATCHES;

    CYCLE_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles.                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {
    int i;

    if (RUN_BIT == FALSE) {
        printf("Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("Simulating for %d cycles...\n\n", num_cycles);
    for (i = 0; i < num_cycles; i++) {
        if (CURRENT_LATCHES.PC == 0x0000) {
            RUN_BIT = FALSE;
            printf("Simulator halted\n\n");
            break;
        }
        cycle();
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed.                 */
/*                                                             */
/***************************************************************/
void go() {
    if (RUN_BIT == FALSE) {
        printf("Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("Simulating...\n\n");
    while (CURRENT_LATCHES.PC != 0x0000)
        cycle();
    RUN_BIT = FALSE;
    printf("Simulator halted\n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {
    int address; /* this is a byte address */

    printf("\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    printf("-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
        printf("  0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    printf("\n");

    /* dump the memory contents into the dumpsim file */
    fprintf(dumpsim_file, "\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    fprintf(dumpsim_file, "-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
        fprintf(dumpsim_file, " 0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {
    int k;

    printf("\nCurrent register/bus values :\n");
    printf("-------------------------------------\n");
    printf("Cycle Count  : %d\n", CYCLE_COUNT);
    printf("PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    printf("IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    printf("STATE_NUMBER : %0.2d\n", CURRENT_LATCHES.STATE_NUMBER);
    printf("BUS          : 0x%0.4x\n", BUS);
    printf("MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    printf("MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    printf("Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
        printf("%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    printf("\n");

    /* dump the state information into the dumpsim file */
    fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
    fprintf(dumpsim_file, "-------------------------------------\n");
    fprintf(dumpsim_file, "Cycle Count  : %d\n", CYCLE_COUNT);
    fprintf(dumpsim_file, "PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    fprintf(dumpsim_file, "IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    fprintf(dumpsim_file, "STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    fprintf(dumpsim_file, "BUS          : 0x%0.4x\n", BUS);
    fprintf(dumpsim_file, "MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    fprintf(dumpsim_file, "MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    fprintf(dumpsim_file, "Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
        fprintf(dumpsim_file, "%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {
    char buffer[20];
    int start, stop, cycles;

    printf("LC-3b-SIM> ");

    scanf("%s", buffer);
    printf("\n");

    switch(buffer[0]) {
        case 'G':
        case 'g':
            go();
            break;

        case 'M':
        case 'm':
            scanf("%i %i", &start, &stop);
            mdump(dumpsim_file, start, stop);
            break;

        case '?':
            help();
            break;
        case 'Q':
        case 'q':
            printf("Bye.\n");
            exit(0);

        case 'R':
        case 'r':
            if (buffer[1] == 'd' || buffer[1] == 'D')
                rdump(dumpsim_file);
            else {
                scanf("%d", &cycles);
                run(cycles);
            }
            break;

        default:
            printf("Invalid Command\n");
            break;
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_control_store                              */
/*                                                             */
/* Purpose   : Load microprogram into control store ROM        */
/*                                                             */
/***************************************************************/
void init_control_store(char *ucode_filename) {
    FILE *ucode;
    int i, j, index;
    char line[200];

    printf("Loading Control Store from file: %s\n", ucode_filename);

    /* Open the micro-code file. */
    if ((ucode = fopen(ucode_filename, "r")) == NULL) {
        printf("Error: Can't open micro-code file %s\n", ucode_filename);
        exit(-1);
    }

    /* Read a line for each row in the control store. */
    for(i = 0; i < CONTROL_STORE_ROWS; i++) {
        if (fscanf(ucode, "%[^\n]\n", line) == EOF) {
            printf("Error: Too few lines (%d) in micro-code file: %s\n",
                   i, ucode_filename);
            exit(-1);
        }

        /* Put in bits one at a time. */
        index = 0;

        for (j = 0; j < CONTROL_STORE_BITS; j++) {
            /* Needs to find enough bits in line. */
            if (line[index] == '\0') {
                printf("Error: Too few control bits in micro-code file: %s\nLine: %d\n",
                       ucode_filename, i);
                exit(-1);
            }
            if (line[index] != '0' && line[index] != '1') {
                printf("Error: Unknown value in micro-code file: %s\nLine: %d, Bit: %d\n",
                       ucode_filename, i, j);
                exit(-1);
            }

            /* Set the bit in the Control Store. */
            CONTROL_STORE[i][j] = (line[index] == '0') ? 0:1;
            index++;
        }

        /* Warn about extra bits in line. */
        if (line[index] != '\0')
            printf("Warning: Extra bit(s) in control store file %s. Line: %d\n",
                   ucode_filename, i);
    }
    printf("\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {
    int i;

    for (i=0; i < WORDS_IN_MEM; i++) {
        MEMORY[i][0] = 0;
        MEMORY[i][1] = 0;
    }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename, int is_virtual_base) {
    FILE * prog;
    int ii, word, program_base, pte, virtual_pc;

    /* Open program file. */
    prog = fopen(program_filename, "r");
    if (prog == NULL) {
        printf("Error: Can't open program file %s\n", program_filename);
        exit(-1);
    }

    /* Read in the program. */
    if (fscanf(prog, "%x\n", &word) != EOF)
        program_base = word >> 1;
    else {
        printf("Error: Program file is empty\n");
        exit(-1);
    }

    if (is_virtual_base) {
        if (CURRENT_LATCHES.PTBR == 0) {
            printf("Error: Page table base not loaded %s\n", program_filename);
            exit(-1);
        }

        /* convert virtual_base to physical_base */
        virtual_pc = program_base << 1;
        pte = (MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][1] << 8) |
              MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][0];

        printf("virtual base of program: %04x\npte: %04x\n", program_base << 1, pte);
        if ((pte & PTE_VALID_MASK) == PTE_VALID_MASK) {
            program_base = (pte & PTE_PFN_MASK) | ((program_base << 1) & PAGE_OFFSET_MASK);
            printf("physical base of program: %x\n\n", program_base);
            program_base = program_base >> 1;
        } else {
            printf("attempting to load a program into an invalid (non-resident) page\n\n");
            exit(-1);
        }
    }
    else {
        /* is page table */
        CURRENT_LATCHES.PTBR = program_base << 1;
    }

    ii = 0;
    while (fscanf(prog, "%x\n", &word) != EOF) {
        /* Make sure it fits. */
        if (program_base + ii >= WORDS_IN_MEM) {
            printf("Error: Program file %s is too long to fit in memory. %x\n",
                   program_filename, ii);
            exit(-1);
        }

        /* Write the word to memory array. */
        MEMORY[program_base + ii][0] = word & 0x00FF;
        MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;;
        ii++;
    }

    if (CURRENT_LATCHES.PC == 0 && is_virtual_base)
        CURRENT_LATCHES.PC = virtual_pc;

    printf("Read %d words from program into memory.\n\n", ii);
}

/***************************************************************/
/*                                                             */
/* Procedure : initialize                                      */
/*                                                             */
/* Purpose   : Load microprogram and machine language program  */
/*             and set up initial state of the machine         */
/*                                                             */
/***************************************************************/
void initialize(char *argv[], int num_prog_files) {
    int i;
    init_control_store(argv[1]);

    init_memory();
    CURRENT_LATCHES.PTBR = 0x1000;

    load_program(argv[2],0);
    for ( i = 0; i < num_prog_files; i++ ) {
        load_program(argv[i + 3],1);
    }
    CURRENT_LATCHES.Z = 1;
    CURRENT_LATCHES.STATE_NUMBER = INITIAL_STATE_NUMBER;
    memcpy(CURRENT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[INITIAL_STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);
    CURRENT_LATCHES.SSP = 0x3000; /* Initial value of system stack pointer */

/* MODIFY: you can add more initialization code HERE */
    CURRENT_LATCHES.PTBR = 0x1000;
    CURRENT_LATCHES.PSR = 0x8000;

    NEXT_LATCHES = CURRENT_LATCHES;

    RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {
    FILE * dumpsim_file;

    /* Error Checking */
    if (argc < 4) {
        printf("Error: usage: %s <micro_code_file> <page table file> <program_file_1> <program_file_2> ...\n",
               argv[0]);
        exit(1);
    }

    printf("LC-3b Simulator\n\n");

    initialize(argv, argc - 3);

    if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
        printf("Error: Can't open dumpsim file\n");
        exit(-1);
    }

    while (1)
        get_command(dumpsim_file);

}

/***************************************************************/
/* Do not modify the above code, except for the places indicated
   with a "MODIFY:" comment.
   You are allowed to use the following global variables in your
   code. These are defined above.

   CONTROL_STORE
   MEMORY
   BUS

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */
/***************************************************************/

/***************************************************************/

#define OPCODE(ir) (ir>>12)

#define IR11(ir) ((ir>>11) & 0x01)
#define IR5(ir) ((ir>>5) & 0x01)
#define IR11_9(ir) ((ir>>9) & 0b0111)
#define IR8_6(ir) ((ir>>6) & 0b0111)
#define NZP(ir) ((ir>>9) & 0b0111)
#define IR5_4(ir) ((ir>>4) & 0b011)
#define IR10(ir) ((ir>>10) & 0b01)
#define IR9(ir) ((ir>>9) & 0b01)

#define GetBit(bit, reg) ((reg>>bit) & 0x01)
#define SetBit15(reg) ((reg &0x7FFF) | (1<<15))
#define SetBit(val, bit ,reg) ((reg & (~(1<<bit)))| (val<<bit))



#define BASE_R(ir) ((ir>>6) & 0b0111)
#define DR(ir) ((ir>>9) & 0b0111)
#define SR2(ir) ((ir & 0b0111))

#define IMM5(ir) ((ir & 0x1F))
#define PC_OFFSET9(ir) (ir & 0x1FF)
#define OFFSET6(ir) (ir & 0x3F)
#define AMOUNT4(ir) (ir & 0x0F)
#define TRAP_VECT8(ir) (ir & 0xFF)
#define PC_OFFSET11(ir) (ir & 0x7FF)


#define ZEXT_and_LSHF1(x) (((x & 0x0FF) << 1))

#define SEXT(reg, len) ((reg<<len)>>len)
#define ZEXT(reg, len) (reg & 0x00FF)


#define UPPER(reg) ((reg>>8) & 0x0FF)
#define LOWER(reg) (reg & 0xFF)

enum{
    NO = 0,
    YES = 1,
    LOAD = 1,

    pcmux_PCPLUS2 = 0,
    pcmux_BUS = 1,
    pcmux_ADDER = 2,

    IR119 = 0,
    dr_R7 = 1,
    IR86 = 1,

    addr1_PC = 0,
    addr1_BASER = 1,

    ZERO = 0,
    OFFSET6 = 1,
    PCOFFSET9 = 2,
    PCOFFSET11 = 3,

    trapvect = 0,
    mar_ADDER = 1,

    ADD = 0,
    AND = 1,
    XOR = 2,
    PASSA =3,

    BYTE = 0,
    WORD = 1,

    RD = 0,
    WR = 1,
};

int GATE_MDR_VAL = 0;
int GATE_PC_VAL = 0;
int GATE_MAR_VAL = 0;
int GATE_ALU_VAL = 0;
int GATE_SHF_VAL = 0;

int Gate_PC_2_VAL = 0;
int GATE_TABLE_VAL = 0;
int GATE_PSR_VAL = 0;
int GATE_SP_VAL = 0;

int GATE_PTBR = 0;
int GATE_VA_OFFSET = 0;
int GATE_VA_VPN = 0;

int nzp_logic_arr[3] = {0,1,0}; //p, z, n


/*logic and muxes used in data path (whatever is used for ldb is current wrong*/
/*************************************************LOGIC BLOCKS***************************************/
void nzp_logic(int bus_val){
    bus_val = SEXT(bus_val,16);
    if(bus_val == 0){
        nzp_logic_arr[0] = 0;
        nzp_logic_arr[1] = 1;
        nzp_logic_arr[2] = 0;
    }
    if(bus_val > 0){
        nzp_logic_arr[0] = 1;
        nzp_logic_arr[1] = 0;
        nzp_logic_arr[2] = 0;
    }
    if(bus_val < 0)
    {
        nzp_logic_arr[0] = 0;
        nzp_logic_arr[1] = 0;
        nzp_logic_arr[2] = 1;
    }
}

/*Logic block before the GateMDR. adjusted for lab 5*/
int GateMDR_logic(){
    int data_size = GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION);
    int mar0 = CURRENT_LATCHES.MAR & 0x01;
    int mdr = CURRENT_LATCHES.MDR;

    mdr = (mar0 == 1) ? UPPER(mdr) : LOWER(mdr);
    mdr = SEXT(mdr, 24);

    int new_mdr = (data_size == WORD) ? CURRENT_LATCHES.MDR : mdr;
    if(GetLoad_PFN((CURRENT_LATCHES.MICROINSTRUCTION))) {
        new_mdr = PTE_PFN_MASK & new_mdr;
    }
    return Low16bits(new_mdr);
}

int isWriteInstruction(){
    int next_state = CURRENT_LATCHES.JBITREG;
    if(next_state == 16 /*stw*/ || next_state == 17 /*stb*/ || next_state == 41 /*Int*/ ||next_state == 52 /*int*/){
        return 1;
    }
    return 0;
}

//logic block before MDR Reg. Adjusted for lab 5
int LdMDR_logic(){
    int new_mdr;
    int mar0 = CURRENT_LATCHES.MAR & 0x01;

    int input  = BUS;


    if(GetRestoreMDR(CURRENT_LATCHES.MICROINSTRUCTION) == 1){
        input = CURRENT_LATCHES.MDR_Temp;
    }

    int BUS7_0 = LOWER(input);

    if(GetUpdate_PTE(CURRENT_LATCHES.MICROINSTRUCTION)){
        new_mdr = Low16bits(input);
        new_mdr = SetBit(1,0,new_mdr);
        if(isWriteInstruction()) {
            new_mdr = SetBit(1, 1, new_mdr);
        }
        return new_mdr;
    }

    if(mar0 == 1 && (GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == BYTE)){
        return Low16bits((BUS7_0<<8) + BUS7_0);
    }
    else{
        return Low16bits(input);
    }
}

int psr_logic(){
    int psr = CURRENT_LATCHES.PSR;
    int n = nzp_logic_arr[2];
    int z = nzp_logic_arr[1];
    int p = nzp_logic_arr[0];

    psr = SetBit(n,2,psr);
    psr = SetBit(z,1,psr);
    psr = SetBit(p,0,psr);
    if(GetLD_PSR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        return BUS;
    }
    else if(GetLD_CC(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        return psr;
    }
    return CURRENT_LATCHES.PSR;
}

/*changed and added for lab 5*/
int ACV_logic(){
    int psr = CURRENT_LATCHES.PSR;
    int psr15 = GetBit(15,psr);
    if(psr15 == 1 &&  (GetBit(3, Low16bits(BUS)) == 0) && GetSetACV(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        return 1;
    }
    return 0;
  /*  if(BUS <= 0x2FFF && BUS >= 0x0000 && GetBit(15,CURRENT_LATCHES.PSR)){
        return 1;
    }
    return 0; */
}

int UA_logic(){
    if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION) == 1 && GetSetUA(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        if(GetBit(0,BUS) == 1){
            return 1;
        }
    }
    return 0;
}

int PageFault_logic(){
    if(GetBit(2,BUS) == 0 && GetSetPageFault(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        return 1;
    }
    return 0;
}


/************************************************MUXS********************************************/
/* DRMUX. Contains changes for lab4 (incorporates r6 as a selection option)*/
int dr_sel(int s){
    int ir = CURRENT_LATCHES.IR;
    if(s == 2){ return 6;}
    int reg_num = (s == IR119) ? DR(ir) : 7;
    return Low16bits(reg_num);
}

/*SR1_Sel. Contains changes for lab for (r6 integration)*/
int sr1_sel(int s){
    int ir = CURRENT_LATCHES.IR;
    if(s == 2){
        return CURRENT_LATCHES.REGS[6];
    }
    int reg_num = (s == IR86) ?  IR8_6(ir): IR11_9(ir);
    return Low16bits(CURRENT_LATCHES.REGS[reg_num]);
}

/*SR2 MUX*/
int sr2_sel(){
    int ir = CURRENT_LATCHES.IR;
    return Low16bits(CURRENT_LATCHES.REGS[SR2(ir)]);
}

int sr2_mux(int s, int sr2, int imm5){
    return ((s == 1) ? Low16bits(SEXT(imm5,27)) : Low16bits(sr2));
}

/*ADDR1_Mux. Selects between PC and SR1*/
int addr1_mux(int s, int pc, int base_r){
    return ((s == addr1_PC ) ? Low16bits(pc) : Low16bits(base_r));
}

/*ADDR2MUX -> selects between different offsets and 0*/
int addr2_mux(int s, int offset6, int pc_offset9, int pc_offset11){
    switch(s){
        case ZERO : return 0;
        case OFFSET6 : return Low16bits(SEXT(offset6, 26));
        case PCOFFSET9 : return Low16bits(SEXT(pc_offset9, 23));
        case PCOFFSET11 : return Low16bits(SEXT(pc_offset11, 21));
        default: return 0;
    }
}

/*MARMUX. Selects between trap vector and Adder*/
int mar_mux(int s, int trapvect8, int adder){
    return s == mar_ADDER ? Low16bits(adder) : Low16bits(trapvect8);
}

/*PCMUX. Selects between BUS, ADDER and PC+2*/
int pc_mux(int s, int inc2, int bus, int adder){
    switch(s){
        case pcmux_PCPLUS2: return Low16bits(inc2);
        case pcmux_BUS: return Low16bits(bus);
        case pcmux_ADDER: return Low16bits(adder);
        default: return -1;
    }
}

/*MDRMUX. Selects between BUS and Mem Output*/
int mdr_mux(int mem_output, int bus_val){
    int mio_en = GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION);
    return mio_en == YES ? Low16bits(mem_output) : Low16bits(bus_val);
}

/*MARSEL_MUX. New for lab 4*/
int mar_sel_mux(int sel, int bus, int mar_inc){
    return (sel == 0) ? bus : mar_inc;
}

/*MARDEC MUX. New for lab 4. Should be able to remove now*/
int mar_dec_mux(int sel, int plus2, int r6pm2){
    return (sel == 0) ? plus2 : r6pm2;
}

/*CCMUX. New for lab 4. Alters global NZP array*/
void cc_mux(int sel, int bus, int psr){
    if(sel == 0){
        nzp_logic(bus);
    }
    else{
        nzp_logic_arr[0] = GetBit(0,psr);
        nzp_logic_arr[1] = GetBit(1,psr);
        nzp_logic_arr[2] = GetBit(2,psr);
    }
}

int exc_mux(int sel){
    if(sel == 1){ return 0;}
    int unaligned_access = UA_logic();
    int protection_exception = ACV_logic();
    int page_fault = PageFault_logic();
    int exception_bit = (unaligned_access<<2) + (protection_exception<<1) + page_fault;
    return exception_bit;
}

int vector_exception_mux(){
    int sel = CURRENT_LATCHES.EXCV;
    switch (sel)
    {
        case 0: return 0x05;
        case 1: return 0x02;
        case 2: return 0x04;
        case 3: return 0x04;
        case 4: return 0x03;
        case 5: return 0x03;
        case 6: return 0x03;
        case 7: return 0x03;
        default:
            printf("somebody gna get a hurt real bad");
            break;
    }

}
int vector_mux(){
    int sel = GetVectMUX(CURRENT_LATCHES.MICROINSTRUCTION);
    int exceptionCode = vector_exception_mux();
    return (sel == 1) ? 0x01 : exceptionCode;
}

int sp_mux(){
    int sel = GetSPMUX(CURRENT_LATCHES.MICROINSTRUCTION);
    return sel == 1 ? CURRENT_LATCHES.USP : CURRENT_LATCHES.SSP;
}

int reg_dec_mux(int sr1_val){
    int sel = GetREGDecMUX(CURRENT_LATCHES.MICROINSTRUCTION);
    return sel == 0 ? (sr1_val + 2): (sr1_val-2);
}

int reg_sel_mux(int sr1_inc_val){
    int sel = GetREGSelMUX(CURRENT_LATCHES.MICROINSTRUCTION);
    return sel == 0 ? BUS : sr1_inc_val;
}

int microsequencer_jbit_mux(int next_j){
    int jsel = GetJbitOut(CURRENT_LATCHES.MICROINSTRUCTION);
    switch (jsel) {
        case 0: return next_j;
        case 1: return CURRENT_LATCHES.JBITREG;
        case 2: return 42;
        default: return -1;
    }
}
/********************************************* Operational blocks ***********************************************/

int memory_access(){
    int we0;
    int we1;
    int rw = GetR_W(CURRENT_LATCHES.MICROINSTRUCTION);
    int mio_en = GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION);
    int data_size = GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION);
    int mar0 = CURRENT_LATCHES.MAR & 0x01;
    if(mio_en == NO){
        return 0;
    }

    we0 = 0; we1 = 0;
    if(rw == WR){
        if(data_size == WORD){
            we0 = 1;
            we1 = 1;
        }
        else if (mar0 == 1)
        {
            we1 = 1;
        }
        else if(mar0 == 0){
            we0 = 1;
        }
    }

    if(rw == RD){
        if(CURRENT_LATCHES.READY == 0){
            return 0;
        }
        return Low16bits((MEMORY[CURRENT_LATCHES.MAR/2][1]<<8) + MEMORY[CURRENT_LATCHES.MAR/2][0]);
    }

    if(rw == WR){
        if(CURRENT_LATCHES.READY == 0){
            return 0;
        }
        if(we0 == 1){
            MEMORY[CURRENT_LATCHES.MAR/2][0] = LOWER(CURRENT_LATCHES.MDR);
        }
        if(we1 == 1){
            MEMORY[CURRENT_LATCHES.MAR/2][1] = UPPER(CURRENT_LATCHES.MDR);
        }
    }
    return 0;

}

int ALU(int a, int b){
    int aluk = GetALUK(CURRENT_LATCHES.MICROINSTRUCTION);
    switch(aluk){
        case ADD: return Low16bits(a + b);
        case AND: return Low16bits(a & b);
        case XOR: return Low16bits(a ^ b);
        case PASSA: return Low16bits(a);
    }
}

int SHF(int sr1){
    int ir = CURRENT_LATCHES.IR;
    int sel_bits = IR5_4(ir);
    int amt4 = AMOUNT4(ir);
    int x = 0;
    switch(sel_bits){
        case 0: return Low16bits(sr1 << amt4);
        case 1:  x = Low16bits(sr1); return Low16bits(x >>amt4);
        case 3: return Low16bits((SEXT(sr1,16)) >> amt4);
        default: return Low16bits(sr1);
    }
}


int LSHF1_block(int x){
    return Low16bits((GetLSHF1(CURRENT_LATCHES.MICROINSTRUCTION) == 1 ? x<<1 : x));
}

int ADDER(int a1, int a2){
    return Low16bits(a2+a1);
}

/************************************************************* State evaluation****************************************/

void eval_micro_sequencer() {
    if(CYCLE_COUNT == 300){
        NEXT_LATCHES.INTV = 1;
    }
    /*
     * Evaluate the address of the next state according to the
     * micro sequencer logic. Latch the next microinstruction.
     */
    int ird_bit = GetIRD(CURRENT_LATCHES.MICROINSTRUCTION);
    int curr_j_field = GetJ(CURRENT_LATCHES.MICROINSTRUCTION);
    int cond_bits = GetCOND(CURRENT_LATCHES.MICROINSTRUCTION);
    int cond0 = cond_bits & 0b01;
    int cond1 = (cond_bits>>1) & 0b01;

    /*additional cond bits added for lab 4*/
    int cond2 = (cond_bits>>2) & 0b01;

    int ir = CURRENT_LATCHES.IR;


    int addressing_mode_set = ((~cond2) & cond1 & cond0 & IR11(ir))<<0;
    int read_set = ( (~cond2)& (~cond1) & cond0 & CURRENT_LATCHES.READY)<<1;
    int ben_set = ((~cond2) & cond1 & (~cond0) & CURRENT_LATCHES.BEN)<<2;

    /*added for lab 4. PSR15 check and INT check*/
    int psr15_set = (cond2 & (~cond1) & (~cond0) & GetBit(15,CURRENT_LATCHES.PSR))<<3;
    int int_set = (cond2 & (~cond1) & cond0 & CURRENT_LATCHES.INTV)<<4;
    /*adjustments for lab 5*/
    int natural_jbits = curr_j_field + addressing_mode_set + read_set + ben_set + psr15_set + int_set;

    if(GetLD_JBITREG(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        NEXT_LATCHES.JBITREG = natural_jbits;
    }

    int next_jbits = microsequencer_jbit_mux(natural_jbits);

    if(ird_bit == 1){
        memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[OPCODE(ir)], sizeof(NEXT_LATCHES.MICROINSTRUCTION)); //opcode branch
        NEXT_LATCHES.STATE_NUMBER = OPCODE(ir);
    }
    else {
        if (CURRENT_LATCHES.EXCV && (curr_j_field != 37)) {
            memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[63], sizeof(NEXT_LATCHES.MICROINSTRUCTION));
            NEXT_LATCHES.STATE_NUMBER = 63;
        } else {
            memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[next_jbits],
                   sizeof(NEXT_LATCHES.MICROINSTRUCTION)); //next instruction based on j-bits
            NEXT_LATCHES.STATE_NUMBER = next_jbits;
        }
    }

}



/*
 * This function emulates memory and the WE logic.
 * Keep track of which cycle of MEMEN we are dealing with.
 * If fourth, we need to latch Ready bit at the end of
 * cycle to prepare microsequencer for the fifth cycle.
 */
void cycle_memory() {
    static int mio_count = 0;
    NEXT_LATCHES.READY = 0;
    if(GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION) == 1){
        mio_count++;
    }
    if(mio_count == 4){
        mio_count = -1;
        NEXT_LATCHES.READY = 1;
    }
    return;

}



void eval_bus_drivers() {

    int ir = CURRENT_LATCHES.IR;
    int sr2 = sr2_sel();
    int sr1 = sr1_sel(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION));
    int pc = CURRENT_LATCHES.PC;


    GATE_MDR_VAL = GateMDR_logic();

//alu gate value
    int b = sr2_mux(IR5(ir), sr2, IMM5(ir));
    int a = sr1;
    GATE_ALU_VAL = ALU(a, b);

//shift gate
    GATE_SHF_VAL = SHF(sr1);

//MAR Gate
    int addr1_result = addr1_mux(GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION), pc, sr1);
    int addr2_result = addr2_mux(GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION), OFFSET6(ir), PC_OFFSET9(ir), PC_OFFSET11(ir));
    int lshf1_result = LSHF1_block(addr2_result);
    int adder = ADDER(addr1_result, lshf1_result);
    int trap = TRAP_VECT8(ir)<<1;
    GATE_MAR_VAL = mar_mux(GetMARMUX(CURRENT_LATCHES.MICROINSTRUCTION), trap, adder);

//Gate PC
    GATE_PC_VAL = pc;

/*Added for Lab 4*/

//Gate PC-2
    Gate_PC_2_VAL = pc-2;

//Gate Table
    int table = 0x200;
    int vector = CURRENT_LATCHES.Vector;
    GATE_TABLE_VAL = table + (vector<<1);

//Gate PSR Val
    GATE_PSR_VAL = CURRENT_LATCHES.PSR;

//Gate SP val
    GATE_SP_VAL = sp_mux();

/*added for lab 5*/
//Gate VPN
    GATE_VA_VPN = LOWER((CURRENT_LATCHES.VA>>9) << 1);

//Gate Virtual Address Offset
    GATE_VA_OFFSET = CURRENT_LATCHES.VA & 0x1FF;

//Gate PTBR
    GATE_PTBR = Low16bits(CURRENT_LATCHES.PTBR & 0xFF00);

}


void drive_bus() {

    /*
     * Datapath routine for driving the bus from one of the 5 possible
     * tristate drivers.
     */
    BUS = 0;
    //load bus. There should only be 1 gate set to 1 so bus should not be corrupted
    if(GetGATE_ALU(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_ALU_VAL;}
    if(GetGATE_MARMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_MAR_VAL;}
    if(GetGATE_MDR(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_MDR_VAL;}
    if(GetGATE_PC(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_PC_VAL;}
    if(GetGATE_SHF(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_SHF_VAL;}

    /*Added for lab 4*/
    if(GetGatePC_2(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = Gate_PC_2_VAL;}
    if(GetGatePSR(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_PSR_VAL;}
    if(GetGateSP(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_SP_VAL;}
    if(GetGateTable(CURRENT_LATCHES.MICROINSTRUCTION) == 1){BUS = GATE_TABLE_VAL;}

    /*Added for lab 5* This might cause some issues*/
    if(GetGate_PTBR(CURRENT_LATCHES.MICROINSTRUCTION) == 1) {
        BUS = BUS & 0x00FF;
        BUS = BUS | GATE_PTBR;
    }
    if(GetGate_VPN(CURRENT_LATCHES.MICROINSTRUCTION) == 1){
        BUS = BUS & 0xFF00;
        BUS = BUS | GATE_VA_VPN;
    }
    if(GetGate_Offset(CURRENT_LATCHES.MICROINSTRUCTION) == 1){
        BUS = BUS & ~0x01FF;
        BUS = BUS | GATE_VA_OFFSET;
    }



}


void latch_datapath_values() {

    /*
     * Datapath routine for computing all functions that need to latch
     * values in the data path at the end of this cycle.  Some values
     * require sourcing the bus; therefore, this routine has to come
     * after drive_bus.
     */
    int mar_inc = mar_dec_mux(GetMARDecMUX(CURRENT_LATCHES.MICROINSTRUCTION),
                              CURRENT_LATCHES.REGS[6] + 2/*curr_mar+2*/,
                              CURRENT_LATCHES.REGS[6] - 2);

    int new_mar = mar_sel_mux(GetMARSel_MUX(CURRENT_LATCHES.MICROINSTRUCTION), BUS, mar_inc);

    /*Altered for lab 4*. Technically could chage by why bother*/
    if (GetLD_MAR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        NEXT_LATCHES.MAR = new_mar;
    }

    if(GetLD_VA(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
        NEXT_LATCHES.VA = new_mar;
    }

//LOAD IR
    if (GetLD_IR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        NEXT_LATCHES.IR = BUS;
    }

//load CC. Altered for Lab 4
    nzp_logic(BUS);

    if (GetLD_CC(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {

        if (GetLD_PSR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
            NEXT_LATCHES.P = GetBit(0, BUS);
            NEXT_LATCHES.Z = GetBit(1, BUS);
            NEXT_LATCHES.N = GetBit(2, BUS);
        } else {
            //if updating cond bits from the bus also update psr to new cc
            NEXT_LATCHES.P = nzp_logic_arr[0];
            NEXT_LATCHES.Z = nzp_logic_arr[1];
            NEXT_LATCHES.N = nzp_logic_arr[2];
        }

    }

//load reg. Altered for lab 4
    if (GetLD_REG(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int dr = dr_sel(GetDRMUX(CURRENT_LATCHES.MICROINSTRUCTION));
        int sr1_val = sr1_sel(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION));
        int sr1_inc = reg_dec_mux(sr1_val);

        NEXT_LATCHES.REGS[dr] = Low16bits(SEXT(reg_sel_mux(sr1_inc), 16));
    }

//load MDR_temp
    if(GetLDMDRTemp(CURRENT_LATCHES.MICROINSTRUCTION)){
        NEXT_LATCHES.MDR_Temp = CURRENT_LATCHES.MDR;
    }

//loadMDR
    int mem_output = memory_access();
    if (GetLD_MDR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int bus_val = LdMDR_logic();
        NEXT_LATCHES.MDR = Low16bits(mdr_mux(mem_output, bus_val));
    }

//load PC
    if (GetLD_PC(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int pc = CURRENT_LATCHES.PC;
        int ir = CURRENT_LATCHES.IR;
        int sr1 = sr1_sel(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION));
        int addr1_result = addr1_mux(GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION), pc, sr1);
        int addr2_result = addr2_mux(GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION), SEXT(OFFSET6(ir), 10),
                                     SEXT(PC_OFFSET9(ir), 7), SEXT(PC_OFFSET11(ir), 4));
        int lshf1_result = LSHF1_block(addr2_result);
        int adder = ADDER(addr1_result, lshf1_result);
        NEXT_LATCHES.PC = Low16bits(pc_mux(GetPCMUX(CURRENT_LATCHES.MICROINSTRUCTION), pc + 2, BUS, adder));
    }

//load Ben
    if (GetLD_BEN(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int ir = CURRENT_LATCHES.IR;
        NEXT_LATCHES.BEN =
                (IR11(ir) & CURRENT_LATCHES.N) || (IR10(ir) & CURRENT_LATCHES.Z) || (IR9(ir) & CURRENT_LATCHES.P);
    }

    if (GetLD_Excep(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int new_excep = exc_mux(GetClrExcep(CURRENT_LATCHES.MICROINSTRUCTION));
        NEXT_LATCHES.EXCV = Low16bits(new_excep);
    }

    if (GetLD_SSP(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int sr1_val = sr1_sel(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION));
    }

    if (GetLD_USP(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int sr1_val = sr1_sel(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION));
        NEXT_LATCHES.USP = sr1_val;
    }


//load PSR shouldn't happen when psr15 is also being set
/*if(GetLD_PSR(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD){
    NEXT_LATCHES.PSR = psr_logic();
}*/

/*PSR logic*/
//contains the load psr check and als updates the cc if load cc was active
    int new_psr = psr_logic();

//clr psr 15 should never happen while ccs are being updated psr is being loaded by bus. However, in the case it does happen it will be possible
    if (GetCLR_PSR15(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        new_psr = SetBit(0, 15, CURRENT_LATCHES.PSR);
    }
    NEXT_LATCHES.PSR = Low16bits(new_psr);


    if (GetLD_Vector(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        int new_vector = vector_mux();
        NEXT_LATCHES.Vector = Low16bits(new_vector);
    }

    if (GetCLR_INTV(CURRENT_LATCHES.MICROINSTRUCTION) == LOAD) {
        NEXT_LATCHES.INTV = 0;
    }

}