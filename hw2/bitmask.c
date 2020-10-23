#include <stdio.h>

int get_bit(unsigned int, int); // Although get_bit is defined prior to main(), since 
                                // set_bit() is calling get_bit() I put the prototye 
                                // to avoid warning.

unsigned int set_bit(unsigned int number, int value, int position) {
    /* YOUR CODE HERE */
    int bitmask = 0b00000001;

    if(get_bit(number, position) == value)
        return number;
    else if(get_bit(number, position) == 0)
        return (bitmask << position) | number;
    else
        return (bitmask << position) ^ number;
}

int get_bit(unsigned int number, int position) {
    /* YOUR CODE HERE */
    int bitmask = 1;
    return (number >> position) & bitmask;
}

int main(void) {
    unsigned int num = 0;
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 3rd bit to 1. num should read 0x8.\n");
    num = set_bit(num, 1, 3);
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 7th bit to 1. num should read 0x88.\n");
    num = set_bit(num, 1, 7);
    printf("num is currently 0x%X.\n", num);
    printf("Setting the 3rd bit to 0. num should read 0x80.\n");
    num = set_bit(num, 0, 3);
    printf("num is currently 0x%X.\n", num);
    printf("Getting the value of the 4th bit, which should be 0.\n");
    printf("value of the 4th bit is %d.\n", get_bit(num, 4));
    printf("Getting the value of the 7th bit, which should be 1.\n");
    printf("value of the 7th bit is %d.\n", get_bit(num, 7));
    return 0;
}

