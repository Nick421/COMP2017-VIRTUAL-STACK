#include <stdio.h>
#include <stdlib.h>

#define BYTE unsigned char
#define array_length(x) (sizeof(x)/ sizeof(x[0]))

BYTE registers[7];

BYTE stack_addresses[128];

BYTE return_val;

typedef struct {
    BYTE address1[8];
    BYTE address1_type[2];
    BYTE address2[8];
    BYTE address2_type[2];
    BYTE opcode;
}instruct ;

struct function{
    BYTE header;
    BYTE num_args;
    int num_of_int;
    instruct instructions[255];
};

int get_bit(int n, int index){
    return (n >> index) & 1;
}

BYTE binary_to_decimal(BYTE op1, BYTE op2, BYTE op3){
    BYTE arrbin[3] = {op3,op2,op1};

    BYTE bin = 0;
    for (int i = 0; i < 3; ++i )
    {
        bin *= 2;
        bin = bin + arrbin[i];
    }
    return bin;
}


BYTE binary_to_decimal_type(BYTE op1, BYTE op2){
    BYTE arrbin[2] = {op1,op2};

    BYTE bin = 0;
    for (int i = 0; i < 2; ++i )
    {
        bin *= 2;
        bin = bin + arrbin[i];
    }
    return bin;
}

BYTE binary_to_decimal_value(BYTE* addr){


    BYTE bin = 0;
    for (int i = 0; i < 8; ++i )
    {
        bin *= 2;
        bin = bin + addr[i];
    }
    return bin;
}

BYTE binary_to_decimal_addr(BYTE* addr){


    BYTE bin = 0;
    for (int i = 0; i < 7; ++i )
    {
        bin *= 2;
        bin = bin + addr[i];
    }
    return bin;
}

BYTE binary_to_decimal_4bit(BYTE bit1, BYTE bit2, BYTE bit3, BYTE bit4){

    BYTE arrbin[4] = {bit4, bit3, bit2, bit1};
    BYTE bin = 0;
    for (int i = 0; i < 4; ++i )
    {
        bin *= 2;
        bin = bin + arrbin[i];
    }
    return bin;
}

BYTE binary_to_decimal_8bit(BYTE bit1, BYTE bit2, BYTE bit3, BYTE bit4,BYTE bit5, BYTE bit6, BYTE bit7, BYTE bit8){

    BYTE arrbin[8] = {bit8, bit7, bit6, bit5, bit4, bit3, bit2, bit1};
    BYTE bin = 0;
    for (int i = 0; i < 8; ++i )
    {
        bin *= 2;
        bin = bin + arrbin[i];
    }
    return bin;
}


void MOV(instruct* instructions, int instruction_number){

    BYTE type1 = binary_to_decimal_type(instructions[instruction_number].address1_type[0], instructions[instruction_number].address1_type[1]);
    BYTE type2 = binary_to_decimal_type(instructions[instruction_number].address2_type[0], instructions[instruction_number].address2_type[1]);
    BYTE register_number = 0;
    BYTE value = 0;
    BYTE stack_addr = 0;
    BYTE stack_addr2 =0;

    switch (type1) {
        case 0:
            register_number = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
            value = binary_to_decimal_value(instructions[instruction_number].address1);
            if (type2 == 2){
                stack_addr = binary_to_decimal_addr(instructions[instruction_number].address2);
                stack_addresses[stack_addr] = value;

                break;
            }
            if (type2 == 3){
                stack_addr = binary_to_decimal_addr(instructions[instruction_number].address2);

                stack_addresses[stack_addresses[stack_addr]] = value;
                stack_addresses[1]+=1;
                break;
            }

            switch (register_number){
                case 0:
                    // put the value to registers[0]
                    registers[0] = value;
                    break;
                case 1:
                    // put the value to registers[1]
                    registers[1] = value;
                    break;
                case 2:
                    // put the value to registers[2]
                    registers[2] = value;
                    break;
                case 3:
                    // put the value to registers[3]
                    registers[3] = value;
                    break;
                case 4:
                    // put the value to registers[4]
                    registers[4] = value;
                    break;
                case 5:
                    // put the value to registers[5]
                    registers[5] = value;
                    break;
                case 6:
                    // put the value to registers[6]
                    registers[6] = value;
                    break;

            }
            break;
        case 1:
            register_number = binary_to_decimal(instructions[instruction_number].address1[2],instructions[instruction_number].address1[1],instructions[instruction_number].address1[0]);
            switch (type2){
                case 2:
                    // move to address on the stack
                    stack_addr = binary_to_decimal_addr(instructions[instruction_number].address2);
                    stack_addresses[stack_addr] = registers[register_number];
                    break;
                case 3:
                    // move to the next address specified by the stack pointer.
                    stack_addresses[stack_addresses[1]] = registers[register_number];

                    break;
            }
            break;

        case 2:
            switch(type2){
                case 1:
                    register_number = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
                    stack_addr = binary_to_decimal_addr(instructions[instruction_number].address1);

                    registers[register_number] = stack_addresses[stack_addr];

                    break;
                case 2:
                    stack_addr = binary_to_decimal_addr(instructions[instruction_number].address1);
                    stack_addr2 = binary_to_decimal_addr(instructions[instruction_number].address2);

                    stack_addresses[stack_addr2] = stack_addresses[stack_addr];
                    break;
                case 3:
                    stack_addr = binary_to_decimal_addr(instructions[instruction_number].address1);

                    stack_addr2 = binary_to_decimal_addr(instructions[instruction_number].address2);

                    value = stack_addresses[stack_addr2];
                    stack_addresses[value] = stack_addresses[stack_addr];
                    stack_addresses[1]+=1;
                    break;

            }



    }

}

int CAL(instruct* instructions, int instruction_number){
    BYTE value = binary_to_decimal_value(instructions[instruction_number].address1);
    //BYTE stack_addr = binary_to_decimal_addr(instructions[instruction_number].address2);
    return value;

}

void POP(instruct* instructions, int instruction_number){
    BYTE stack_addr = binary_to_decimal_addr(instructions[instruction_number].address2);
    return_val = stack_addresses[stack_addr];

}

void RET(instruct* instructions, int instruction_number){

    printf("%d\n", return_val);

}

void ADD(instruct* instructions, int instruction_number){
    int register_number_1, register_number_2;


    register_number_1 = binary_to_decimal(instructions[instruction_number].address1[2],instructions[instruction_number].address1[1],instructions[instruction_number].address1[0]);
    register_number_2 = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
    switch (register_number_1){
        case 0:
            // add the second register to registers[0]
            registers[0] += registers[register_number_2];
            break;
        case 1:
            // add the second register to registers[1]
            registers[1] += registers[register_number_2];
            break;
        case 2:
            // add the second register to registers[2]
            registers[2] += registers[register_number_2];
            break;
        case 3:
            // add the second register to registers[3]
            registers[3] += registers[register_number_2];
            break;
        case 4:
            // add the second register to registers[4]
            registers[4] += registers[register_number_2];
            break;
        case 5:
            // add the second register to registers[5]
            registers[5] += registers[register_number_2];
            break;
        case 6:
            // add the second register to registers[6]
            registers[6] += registers[register_number_2];
            break;

    }

}

void AND(instruct* instructions, int instruction_number){
    BYTE register_number_1, register_number_2;


    register_number_1 = binary_to_decimal(instructions[instruction_number].address1[2],instructions[instruction_number].address1[1],instructions[instruction_number].address1[0]);
    register_number_2 = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
    switch (register_number_1){
        case 0:
            // and the second register to registers[0]
            registers[0] = registers[0] & registers[register_number_2];
            break;
        case 1:
            // and the second register to registers[1]
            registers[1] = registers[1] & registers[register_number_2];
            break;
        case 2:
            // and the second register to registers[2]
            registers[2] = registers[2] & registers[register_number_2];
            break;
        case 3:
            // and the second register to registers[3]
            registers[3] = registers[3] & registers[register_number_2];
            break;
        case 4:
            // and the second register to registers[4]
            registers[4] = registers[4] & registers[register_number_2];
            break;
        case 5:
            // and the second register to registers[5]
            registers[5] = registers[5] & registers[register_number_2];
            break;
        case 6:
            // and the second register to registers[6]
            registers[6] = registers[6] & registers[register_number_2];
            break;

    }
}

void NOT(instruct* instructions, int instruction_number){
    BYTE register_number_2;

    register_number_2 = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
    switch (register_number_2){
        case 0:
            // not the registers[0]
            registers[0] = ~registers[0];
            break;
        case 1:
            // not the registers[1]
            registers[1] = ~registers[1];
            break;
        case 2:
            // not the registers[2]
            registers[2] = ~registers[2];
            break;
        case 3:
            // not the registers[3]
            registers[3] = ~registers[3];
            break;
        case 4:
            // not the registers[4]
            registers[4] = ~registers[4];
            break;
        case 5:
            // not the registers[5]
            registers[5] = ~registers[5];
            break;
        case 6:
            // not the registers[6]
            registers[6] = ~registers[6];
            break;

    }

}

void EQU(instruct* instructions, int instruction_number){
    int register_number_2;

    register_number_2 = binary_to_decimal(instructions[instruction_number].address2[2],instructions[instruction_number].address2[1],instructions[instruction_number].address2[0]);
    if (registers[register_number_2] == 0){
        registers[register_number_2] = 1;
    }else {
        registers[register_number_2] = 0;
    }
}

/*
 * main
 * Scaffold code for the COMP2017/9017 assignment 1
 * This code will take a filename and print the numeric value associated
 * with each byte in the file
 * :: int n_args :: The number of arguments passed to the file
 * :: char** vargs :: Stores the arguments passed to this program as cstrings
 * Returns 0
 */

int main(int n_args, char** vargs)
{

    // For readability
    char* filename = vargs[1];
    // A pro-tip to prevent seg-faults for yourself might be to check
    // if this file actually exists, and if an argument is actually passed...

    // Create a 1 byte buffer. You might want more than one byte yourself
    BYTE buffer;

    // Open our file in 'read binary' mode,
    // I sure do hope that this file actually exists
    FILE *file_ptr = fopen(filename,"rb");

    // Skip to the end of the file
    fseek(file_ptr, 0, SEEK_END);

    // Find the length of the file
    size_t file_length = ftell(file_ptr);

    // Rewind to the start of the file so we can actually read it
    rewind(file_ptr);

    int array[file_length];

    // For each byte in the file, first write the byte to our buffer
    // then print the integer representation of the byte
    for (int i = 0; i < file_length; i++)
    {
        fread(&buffer, 1, 1, file_ptr); // read 1 byte
        array[i] = (int)buffer;

    }



    int number_of_instructions = array[array_length(array)-1];
    int no_in_start = number_of_instructions;

    BYTE bincodes[file_length*8];
    int bin_length = array_length(bincodes);
    struct function functions[16];
    instruct *instructions = malloc(sizeof(instruct) * number_of_instructions);
    BYTE frame_pointer = 0;
    BYTE stack_pointer=0;
    BYTE program_counter=0;
    BYTE function_label = 0;
    int bool = 0;
    int number_of_function = 0;



    int count =0;
    for (int x = array_length(array)-1; x >= 0; --x){
        for (int y = 0; y < 8; ++y){
            bincodes[count] = get_bit(array[x],y);
            count++;
        }

    }


    for (int k = 8; k < array_length(bincodes) ; ++k){

        if (bool == 1){
           instructions = realloc(instructions, sizeof(instruct) * number_of_instructions);
           bool = 0;
        }

        if (number_of_instructions > 0){

            int opcode = binary_to_decimal(bincodes[k],bincodes[k+1],bincodes[k+2]);

            switch (opcode) {
                case 0: //MOV
                    --number_of_instructions;
                    for (int i = 0; i<3; ++i){
                        instructions[number_of_instructions].opcode = 0;
                    }
                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];
                    int addrJump = 0;
                    int address_type2 = binary_to_decimal_type(instructions[number_of_instructions].address2_type[0],instructions[number_of_instructions].address2_type[1]);
                    switch (address_type2){

                        case 1: // @type is register address
                            addrJump = 5;
                            for (int y = 2; y >= 0; --y){
                                instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                                addrJump++;
                            }

                            // check the next @type
                            instructions[number_of_instructions].address1_type[1] = bincodes[k+8];
                            instructions[number_of_instructions].address1_type[0] = bincodes[k+9];
                            int address_type1 = binary_to_decimal_type(instructions[number_of_instructions].address1_type[0],instructions[number_of_instructions].address1_type[1]);
                            switch (address_type1) {
                                case 0:
                                    addrJump = 10;
                                    for (int y = 7; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                                case 1:
                                    addrJump = 10;
                                    for (int y = 2; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    break;
                                case 2:
                                    addrJump = 10;
                                    for (int y = 6; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                                    // end of checking first @type and get that address for 01 2nd @type
                            }

                            break;
                            // end of second @type of 00


                        case 2:
                            addrJump = 5;
                            for (int y = 6; y >= 0; --y){
                                instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                                addrJump++;
                            }
                            // check the next @type
                            instructions[number_of_instructions].address1_type[1] = bincodes[k+12];
                            instructions[number_of_instructions].address1_type[0] = bincodes[k+13];
                            address_type1 = binary_to_decimal_type(instructions[number_of_instructions].address1_type[0],instructions[number_of_instructions].address1_type[1]);
                            switch (address_type1) {
                                case 0:
                                    addrJump = 14;
                                    for (int y = 7; y >=0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                                case 1:
                                    addrJump = 14;
                                    for (int y = 2; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;


                                case 2:
                                    addrJump = 14;
                                    for (int y = 6; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                            }

                            break;
                            // end of second @type of 10

                        case 3:
                            addrJump = 5;
                            for (int y = 6; y >= 0; --y){
                                instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                                addrJump++;
                            }
                            // check the next @type
                            instructions[number_of_instructions].address1_type[1] = bincodes[k+12];
                            instructions[number_of_instructions].address1_type[0] = bincodes[k+13];


                            address_type1 = binary_to_decimal_type(instructions[number_of_instructions].address1_type[0],instructions[number_of_instructions].address1_type[1]);
                            switch (address_type1) {
                                case 0:
                                    addrJump = 14;
                                    for (int y = 7; y >=0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                                case 1:
                                    addrJump = 14;
                                    for (int y = 2; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    //addrJump--;
                                    break;
                                    // end of checking first @type and get that address
                                case 2:
                                    addrJump = 14;
                                    for (int y = 6; y >= 0; --y){
                                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                                        addrJump++;
                                    }
                                    addrJump--;
                                    break;
                            }

                            break;
                            // end of second @type of 11

                    }
                    k += addrJump;
                    break;


                case 2: // POP
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 2;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];


                    addrJump = 5;
                    for (int y = 6; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }
                    k += addrJump-1;
                    break;


                case 3: // RET
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 3;

                    k += 2;
                    break;

                case 4: // ADD
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 4;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];


                    addrJump = 5;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }

                    instructions[number_of_instructions].address1_type[1] = bincodes[k+8];
                    instructions[number_of_instructions].address1_type[0] = bincodes[k+9];
                    addrJump = 10;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                        addrJump++;
                    }
                    k += addrJump-1;
                    break;


                case 5: // AND
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 5;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];


                    addrJump = 5;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }

                    instructions[number_of_instructions].address1_type[1] = bincodes[k+8];
                    instructions[number_of_instructions].address1_type[0] = bincodes[k+9];
                    addrJump = 10;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                        addrJump++;
                    }
                    k += addrJump-1;
                    break;



                case 1: //CAL
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 1;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];
                    addrJump = 5;

                    for (int y = 6; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }

                    instructions[number_of_instructions].address1_type[1] = bincodes[k+12];
                    instructions[number_of_instructions].address1_type[0] = bincodes[k+13];

                    addrJump = 14;
                    for (int y = 7; y >= 0; --y){
                        instructions[number_of_instructions].address1[y] = bincodes[k+addrJump];
                        addrJump++;
                    }

                    k += addrJump-1;

                    break;


                case 6: // NOT
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 6;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];


                    addrJump = 5;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }
                    k += addrJump-1;


                    break;



                case 7: //EQU
                    --number_of_instructions;
                    instructions[number_of_instructions].opcode = 7;

                    instructions[number_of_instructions].address2_type[1] = bincodes[k+3];
                    instructions[number_of_instructions].address2_type[0] = bincodes[k+4];


                    addrJump = 5;
                    for (int y = 2; y >= 0; --y){
                        instructions[number_of_instructions].address2[y] = bincodes[k+addrJump];
                        addrJump++;
                    }
                    k += addrJump-1;

                    break;
            }
        }



        if (number_of_instructions == 0){
            k++;
            BYTE arguments = binary_to_decimal_4bit(bincodes[k],bincodes[k+1],bincodes[k+2],bincodes[k+3]);
            function_label = binary_to_decimal_4bit(bincodes[k+4],bincodes[k+5],bincodes[k+6],bincodes[k+7]);
            switch (function_label){
                case 0:
                    functions[0].num_args = arguments;
                    functions[0].header = function_label;
                    functions[0].num_of_int = no_in_start;
                    for (int i =0; i < no_in_start; ++i){
                        functions[0].instructions[i] = instructions[i];
                    }
                    number_of_function++;
                    break;
                case 1:
                    functions[1].num_args = arguments;
                    functions[1].header = function_label;
                    functions[1].num_of_int = no_in_start;
                    for (int i =0; i < no_in_start; ++i){
                        functions[1].instructions[i] = instructions[i];
                    }
                    number_of_function++;
                    break;
                case 2:
                    functions[2].num_args = arguments;
                    functions[2].header = function_label;
                    functions[2].num_of_int = no_in_start;
                    for (int i =0; i < no_in_start; ++i){
                        functions[2].instructions[i] = instructions[i];
                    }
                    number_of_function++;
                    break;
                case 3:
                    functions[3].num_args = arguments;
                    functions[3].header = function_label;
                    functions[3].num_of_int = no_in_start;
                    for (int i =0; i < no_in_start; ++i){
                        functions[3].instructions[i] = instructions[i];
                    }
                    number_of_function++;
                    break;

            }
            if (arguments == 0){
                stack_pointer = 3;
            }else {
                stack_pointer = 4;
            }


            frame_pointer = 0;
            program_counter = 0;


        }
        if ((bin_length-k) < 14){
            break;
        }
        if ((bin_length-k) > 15 & number_of_instructions == 0){
            number_of_instructions = binary_to_decimal_8bit(bincodes[k+8],bincodes[k+9],bincodes[k+10],bincodes[k+11],bincodes[k+12],bincodes[k+13],bincodes[k+14],bincodes[k+15]);
            no_in_start = number_of_instructions;
            k+=15;
        }

    }

    free(instructions);

    /*---------------------------------------------------------------------------
     * Debugs
     ---------------------------------------------------------------------------*/

    /*int gap = 0;
    for (int k = 0; k < array_length(bincodes) ; ++k){
        if (gap == 8){
          printf("\n");
          gap = 0;
        }
        printf("%d", bincodes[k]);
        gap++;
    }


    printf("\n");
    printf("function 1\n");
    for (int x = 0; x < functions[1].num_of_int; ++x){
        printf("Address 1 type: %d", functions[1].instructions[x].address1_type[0]);
        printf(" %d opcode: ", functions[1].instructions[x].address1_type[1]);

        printf("%d ", functions[1].instructions[x].opcode);

        printf("Address 2 type: %d", functions[1].instructions[x].address2_type[0]);
        printf(" %d", functions[1].instructions[x].address2_type[1]);
        printf("\n");
    }
    printf("\n");

    printf("function 0\n");
    printf("\n");
    for (int x = 0; x < functions[0].num_of_int; ++x){
        printf("Address 1 type: %d", functions[0].instructions[x].address1_type[0]);
        printf(" %d opcode: ", functions[0].instructions[x].address1_type[1]);

        printf("%d ", functions[0].instructions[x].opcode);


        printf("Address 2 type: %d", functions[0].instructions[x].address2_type[0]);
        printf(" %d", functions[0].instructions[x].address2_type[1]);
        printf("\n");
    }
    printf("\n");

    //printf("%d\n", bin_length-163);
    for (int i = 0; i< 3; i++){
        //printf("%d", functions[0].instructions[1].opcode[i]);
    }*/


    /*---------------------------------------------------------------------------
    // interpreting the instructions
    ----------------------------------------------------------------------------*/

    stack_addresses[0] = frame_pointer;
    stack_addresses[1] = stack_pointer;
    stack_addresses[2] = program_counter;

    int funct_cal = 0;

        while (stack_addresses[2] < functions[funct_cal].num_of_int) {

            if (stack_addresses[1] == 128) {
                printf("Stack Overflow!\n");
                return 1;
            }
            int opcode = functions[funct_cal].instructions[stack_addresses[2]].opcode;

            switch (opcode) {
                case 0:

                    MOV(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
                case 1:

                    funct_cal = CAL(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] = 0;
                    break;
                case 2:

                    POP(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
                case 3:

                    RET(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
                case 4:

                    ADD(functions[funct_cal].instructions, stack_addresses[2]);

                    stack_addresses[2] += 1;
                    break;
                case 5:

                    AND(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
                case 6:

                    NOT(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
                case 7:

                    EQU(functions[funct_cal].instructions, stack_addresses[2]);
                    stack_addresses[2] += 1;
                    break;
            }

        }
    return 0;
}
