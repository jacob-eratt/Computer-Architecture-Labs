/*
    Remove all unnecessary lines (including this one)
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Full name of the first partner
    Name 2: Full name of the second partner
    UTEID 1: UT EID of the first partner
    UTEID 2: UT EID of the second partner
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE 1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A
*/

#define WORDS_IN_MEM 0x08000
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT; /* run bit */

typedef struct System_Latches_Struct
{

  int PC,               /* program counter */
      N,                /* n condition bit */
      Z,                /* z condition bit */
      P;                /* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help()
{
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle()
{

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles)
{
  int i;

  if (RUN_BIT == FALSE)
  {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++)
  {
    if (CURRENT_LATCHES.PC == 0x0000)
    {
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
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go()
{
  if (RUN_BIT == FALSE)
  {
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
void mdump(FILE *dumpsim_file, int start, int stop)
{
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
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
void rdump(FILE *dumpsim_file)
{
  int k;

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
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
void get_command(FILE *dumpsim_file)
{
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch (buffer[0])
  {
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
    else
    {
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
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory()
{
  int i;

  for (i = 0; i < WORDS_IN_MEM; i++)
  {
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
void load_program(char *program_filename)
{
  FILE *prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL)
  {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else
  {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF)
  {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM)
    {
      printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
      exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0)
    CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files)
{
  int i;

  init_memory();
  for (i = 0; i < num_prog_files; i++)
  {
    load_program(program_filename);
    while (*program_filename++ != '\0')
      ;
  }
  CURRENT_LATCHES.Z = 1;
  NEXT_LATCHES = CURRENT_LATCHES;

  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[])
{
  FILE *dumpsim_file;

  /* Error Checking */
  argc = 2;
  argv[1] = "isaprogram.asm";

  if (argc < 2)
  {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ((dumpsim_file = fopen("dumpsim", "w")) == NULL)
  {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

#define NUM_OPCODES 29
#define MAX_OPCODE_LENGTH 15

#define IS_BIT_SET(x, n) ((x) & (1 << (n)))

uint16_t alu_op(int opcode, int instruction);
uint16_t branch_op(int opcode, int instruction);
uint16_t jmp_op(int opcode, int instruction);
uint16_t jsr_op(int opcode, int instruction);
uint16_t load_op(int opcode, int instruction);
uint16_t save_op(int opcode, int instruction);
uint16_t lea_op(int opcode, int instruction);
uint16_t rti_op(int opcode, int instruction);
uint16_t shift_op(int opcode, int instruction);
uint16_t trap_op(int opcode, int instruction);
uint16_t halt_op(int opcode, int instruction);
uint16_t nop_op(int opcode, int instruction);
uint16_t fill_op(int opcode, int instruction);

typedef struct OpCode_t
{
  char opCodeName[MAX_OPCODE_LENGTH];
  uint16_t binCode;
  // char binCode[7 + 1];
  uint32_t args;
  uint16_t (*operation)(int, int);
} OpCode_t;
/* For all operations i will do the operatin in 32 bits then convert back to 16 bits and store. Basically everything will be sign extended before an operation is done*/
OpCode_t OpCodes[NUM_OPCODES] = {

    {"add", 0b0001, 3, alu_op}, // 1
    {"and", 0b0101, 3, alu_op}, // 5

    {"br", 0b0000, 1, branch_op},

    {"jmp", 0b1100, 1, jmp_op},
    {"jsr", 0b0100, 1, jsr_op},
    {"jsrr", 0b0100, 1, jsr_op},

    {"ldb", 0b0010, 3, load_op},
    {"ldw", 0b0110, 3, load_op},
    {"lea", 0b1110, 2, lea_op},


    {"not", 0b1001, 2, alu_op},
    {"ret", 0b1100, 0, jmp_op},
    {"rti", 0b1000, 0, rti_op},

    {"lshf", 0b1101, 3, shift_op},
    {"rshfl", 0b1101, 3, shift_op},
    {"rshfa", 0b1101, 3, shift_op},

    {"stb", 0b0011, 3, save_op},
    {"stw", 0b0111, 3, save_op},
    {"trap", 0b1111, 1, trap_op},
    {"halt", 0b1111, 1, halt_op},
    {"xor", 0b1001, 3, alu_op},

    {"nop", 0b00000, 0, nop_op},
    {".fill", 0b0, 1, fill_op}

};

int get_opcode_index(int opcode)
{
  for (int i = 0; i < NUM_OPCODES; i++)
  {
    if (opcode == OpCodes[i].binCode)
    {
      return i;
    }
  }
  return -1;
}

void process_instruction(){
  int pre_pc = CURRENT_LATCHES.PC >> 1;
  int instruction = 0;
  instruction = (MEMORY[pre_pc][1] << 8) + MEMORY[pre_pc][0];
  int opcode = instruction >> 12;
  int opcode_index = get_opcode_index(opcode);
  int next_pc = pre_pc +=1;
  NEXT_LATCHES.PC = next_pc <<1;
  uint16_t ret = OpCodes[opcode_index].operation(opcode, instruction);
}

uint16_t fill_op(int opcode, int instruction){
  return 0;
}

uint16_t set_cc(int dr){
  NEXT_LATCHES.N = ((NEXT_LATCHES.REGS[dr]<<16)>>16 < 0) ? 1 : 0;
  NEXT_LATCHES.Z = ((NEXT_LATCHES.REGS[dr]<<16)>>16 == 0) ? 1 : 0;
  NEXT_LATCHES.P = ((NEXT_LATCHES.REGS[dr]<<16)>>16 > 0) ? 1 : 0;
  return 0;
}

uint16_t alu_op(int opcode, int instruction)
{
  instruction = instruction & 0x0FFF; //removes opCode;
  int dr = instruction >>9 & 0b0111;
  int sr1 = instruction >>6 &0b0111;
  int sr1_val = (CURRENT_LATCHES.REGS[sr1]<<16)>>16;
  int op2 = 0;
  if(IS_BIT_SET(instruction,5) != 0){ //check steering bit
    op2 = ((instruction & 0x1F)<<27)>>27; //SEXT(imm5)

  }
  else{
    op2 = instruction & 0b111; //sr2
    op2 = (CURRENT_LATCHES.REGS[op2] <<16)>>16; //sign exten and do operation 
  }
  switch(opcode){ //performs operation based on opcode
    case 0b0001 : NEXT_LATCHES.REGS[dr] = Low16bits(sr1_val + op2); break; //ADD
    case 0b0101 : NEXT_LATCHES.REGS[dr] = Low16bits(sr1_val & op2); break;//AND
    case 0b1001 : NEXT_LATCHES.REGS[dr] = Low16bits(sr1_val ^ op2); break;//XOR

    default: return -1;
  }

  //sets cond bits based on result
  set_cc(dr);
}

uint16_t branch_op(int opcode, int instruction){
  int compare_cond = instruction >>9;
  int curr_cond = ((CURRENT_LATCHES.N <<2) + (CURRENT_LATCHES.Z <<1) + CURRENT_LATCHES.P) & 0b0111;
  if(compare_cond & curr_cond){
    int offset = ((instruction & 0x1FF)<<23)>>22; //LSHF(SEXT(PCoffset),1)
    NEXT_LATCHES.PC += offset;
  }
  return 0;

}

uint16_t jmp_op(int opcode, int instruction){
  int baseR = (instruction >> 6) & 0b0111;
  NEXT_LATCHES.PC = Low16bits(CURRENT_LATCHES.REGS[baseR]);
  return 0;
}

uint16_t jsr_op(int opcode, int instruction){
  int temp = NEXT_LATCHES.PC;
  if(IS_BIT_SET(instruction, 11)){//checks ir11
    int pc_offset11 = ((instruction & 0x07FF)<<21)>>21; //SEXT(PCoffset11)
    NEXT_LATCHES.PC += (pc_offset11 << 1); //LSHF(pc+offset11, 1)
  }
  else{
    int baseR = (instruction >>6) & 0x0007;
    NEXT_LATCHES.PC = Low16bits(NEXT_LATCHES.REGS[baseR]); // PC = BaseR
  }
  NEXT_LATCHES.REGS[7] = Low16bits(temp);
  
  return 0;
}

uint16_t load_op(int opcode, int instruction){
  int dr = (instruction >>9) & 0b0111;
  int baseR = (instruction >> 6) & 0b0111;
  int offset6 = instruction & 0x3F;
  offset6 = (offset6<<26)>>26; //(SEXT(offset6))
  int mem_location;
  if(opcode == 0b0110){ //LDW
    mem_location = ((offset6<<1) + (CURRENT_LATCHES.REGS[baseR]))>>1; //LSHF(offset) + baseR
    int sign_extended_value = ((MEMORY[mem_location][1]& 0x0FF)<<8) + (MEMORY[mem_location][0] & 0x0FF);
   // sign_extended_value = (sign_extended_value<<16)>>16 //signed_extended load value
    NEXT_LATCHES.REGS[dr] = Low16bits(sign_extended_value);    
  }
  else{ //LDB

    mem_location = (offset6 + Low16bits(CURRENT_LATCHES.REGS[baseR]))>>1;
    if(offset6 <0){
      offset6 *= -1;
    }
    NEXT_LATCHES.REGS[dr] = Low16bits((((MEMORY[mem_location][offset6%2] & 0xFF)<<24)>>24)); //sext 8 bit value and then ensure that only 16 bits are store in a reg
  }

  set_cc(dr);
  return 0;
}

uint16_t lea_op(int opcode, int instruction){ //does not set cc codes
  int dr = (instruction >>9) & 0b0111;
  int pc_offset9 = ((instruction & 0x01FF)<<22)>>21; ///LSHF(SEXT(pc_offset9),1)
  NEXT_LATCHES.REGS[dr] = NEXT_LATCHES.PC + pc_offset9;
}

uint16_t rti_op(int opcode, int instruction){
  return 0;
}

uint16_t shift_op(int opcode, int instruction){
  int dr = (instruction >>9) & 0b0111;
  int sr = (instruction >>6) & 0b0111;
  int amount4 = instruction & 0x0F;
  if(!IS_BIT_SET(instruction, 4)){ //left shift
    NEXT_LATCHES.REGS[dr] =  Low16bits(CURRENT_LATCHES.REGS[sr] << amount4);
  }
  else{
    if(IS_BIT_SET(instruction, 5)/*&& IS_BIT_SET(instruction, 16)*/){ //arithmetic //means there is sign extension needed
      int dr_value = CURRENT_LATCHES.REGS[sr] << 16; //places bit 15 into bit 31 this means that the sign extending bit is in a spot such that it can be sign extended when shifted back 
      dr_value = (dr_value)>>(16+amount4); //shift back 16. This is create sign extension if needed and then shift back the amount needed 
      NEXT_LATCHES.REGS[dr] = Low16bits(dr_value);
    }
    else{
      NEXT_LATCHES.REGS[dr] = Low16bits(CURRENT_LATCHES.REGS[sr]) >> amount4; //set bits 16 to 31 as low. Therefore no sign extension happens 
    }
  }
  set_cc(dr);
  return 0;
}


uint16_t save_op(int opcdoe, int instruction){
  int sr = (instruction>>9) & 0b0111;
  int baseR = (instruction >> 6) & 0b0111;
  int baseR_val = CURRENT_LATCHES.REGS[baseR];
  int boffset6 = instruction & 0x3F;
  boffset6 = (boffset6 << 26) >>26; //sign extend
  int mem_location;
  if(opcdoe == 0b0111){ //STW
    mem_location = ((boffset6<<1) + CURRENT_LATCHES.REGS[baseR])>>1;
    MEMORY[mem_location][0] = CURRENT_LATCHES.REGS[sr] & 0x0FF;
    MEMORY[mem_location][1] = (CURRENT_LATCHES.REGS[sr]>>8) &0x0FF;
  }
  else{
    mem_location = (boffset6 + Low16bits(CURRENT_LATCHES.REGS[baseR]));
    if(boffset6 < 0){
      boffset6 *= -1;
    }
    MEMORY[mem_location>>1][boffset6%2] = CURRENT_LATCHES.REGS[sr] & 0x0FF; //low 8 bits of sr 
  }
  return 0;

}

uint16_t trap_op(int opcode, int instruction){
  NEXT_LATCHES.REGS[7] = NEXT_LATCHES.PC;
  int trapvect8 = instruction & 0x0FF; //left shift that was supposed to be done is undone by the right shift that is done to access the correct mem location
  NEXT_LATCHES.PC = Low16bits((MEMORY[trapvect8][1]<<8) + MEMORY[trapvect8][0]);

  return 0;
}

uint16_t halt_op(int opcode, int instruction){
  NEXT_LATCHES.REGS[7] = NEXT_LATCHES.PC;
  int trapvect8 = 0x25;
  NEXT_LATCHES.PC = MEMORY[trapvect8][1]<<8 + MEMORY[trapvect8][0];
  return 0;
}

uint16_t nop_op(int opcode, int instruction){
  return 0;
}
