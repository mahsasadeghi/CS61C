#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
* the definition of array_size is after its first use,
* so we declare the function here
* Do not change the line below.
*/
int array_size(char s[]);

/*
 * rotates string s (given size of char array, INCLUDING the null terminator) by n places
 * edits the s character array formed by pushing leftmost characters to the right; only alphanumeric characters rotate.
 * the result is saved in the local variable new_array, then printed.
 *
 * EX: rotate("hello", 1) --> "elloh"
 * EX: rotate("hello", 2) --> "llohe"
 * EX: rotate("hello", 5) --> "hello"
 *
 * hint: n can be larger than the size of the string.  Which operator might help you here?
 *       also, don't forget about the null terminator!
 */
void rotate(char s[], int n) {
    int size = array_size(s);
    char new_array[size];

    char t_array[2*size];
    int i;
    /* YOUR CODE HERE */

    if(n >= size - 1)
        n = n % (size - 1);

    //memset(new_array, '\0', size); 
    for(i = 0; i < size; i++){
        new_array[i] = '\0';
        t_array[i] = '\0';
    }

    //strcpy(t_array, s);
    for(i = 0; i < size; i++)
        t_array[i] = s[i];

    //strcat(t_array,s);
    for(i = 0; i< size; i++)
        t_array[i + size - 1] = s[i];

    //strncpy(new_array, t_array+n, size-1);
    for(i = 0; i < size - 1; i++)
        new_array[i] = t_array[n+i];
    new_array[size] = '\0'; 

    /* DO NOT CHANGE THIS PRINT LINE */
    printf("%s\n", new_array);
}

/*
 * returns the size of the char array s, including the null terminator
 * you should NOT need to modify this
 */
int array_size(char s[]) {
    int total = 0;
    int index = 0;
    while (s[index] != '\0') {
        total++;
        index++;
    }
    total++;
    return total;
}

/*
 * Use this function to report an error if incorrect command line args are supplied
 * you should NOT need to modify this
 */
void print_error() {
    printf("Error: Incorrect arguments");
    return;    
}

/*
 * main function
 * You will ONLY need to edit this function for adding the command line args.
 */
int main(int argc, char* argv[]) {
    /* YOUR CODE HERE */

    if(argc == 1){
        printf("Should be: bookcoloring\tResults: ");
        rotate("coloringbook", 8);

        printf("Should be: sauceapple\tResults: ");
        rotate("applesauce", 5);

        printf("Should be: beanscool\tResults: ");
        rotate("coolbeans", 4);

        printf("Should be: 61cCS\tResults: ");
        rotate("CS61c", 2);

        printf("Should be: CS61c\tResults: ");
        rotate("CS61c", 5);   
    }else if(argc == 3)
        rotate(argv[1], atoi(argv[2]));
    else
        print_error();   

    return 0;
}


