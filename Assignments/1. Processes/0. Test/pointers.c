#include <stdio.h> 

//int main() {
/*
    int *p;

    int var = 10;

    p = &var;

    printf("Value of var is %d\n", var);
    printf("Value of var is %d\n", *p);
    printf("Adress of var is %0x\n", &var);
    printf("Adress of var is %0x\n", p);
    printf("Adress of pointer p is %0x\n", &p);

    */
  /*  
    int num = 123; 

    int *ptr1;

    int **ptr2; 

    ptr1 = &num;

    ptr2 = &ptr1;

    printf("Value of num = %d\n", num);
    printf("Value of num using ptr1 is %d\n", *ptr1);
    printf("Value of num using ptr2 is %d\n", **ptr2);
    
    printf("Adresses\n");
    
    printf("The adress of num is %p\n", &num);    
    printf("The adress of num using ptr1 is %p\n", ptr1);
    printf("The adress of num using ptr2 is %p\n", *ptr2);

*/

void salaryhike(int  *var, int b) {
    *var = *var+b;
}

int main()
{
    int salary=0, bonus=0;
    printf("Enter the employee current salary:"); 
    scanf("%d", &salary);
    printf("Enter bonus:");
    scanf("%d", &bonus);
    salaryhike(&salary, bonus);
    printf("Final salary: %d\n", salary);
    return 0;
}




