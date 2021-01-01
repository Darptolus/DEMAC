#include <stdio.h>
#include <stdlib.h>

int main(){

int a = 10;
int b = 20;

int *x;
int *y;

x = &a;
y = &b;

printf("This is a: %d\n", *x);
printf("This is b: %d\n", *y);

}