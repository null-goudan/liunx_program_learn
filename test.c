#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct right{
	int day;
	int mouth;
	int year;
};
void sr(struct right);
struct right *fanh(struct right*);
int main(){
	struct right today;
	today=(struct right){1,5,2022};
	struct right *p=&today;
	printf("%d\n",today.mouth);
	p->mouth=15;
	fanh(&today);
	*(fanh(&today)) = (struct right){8,9,2022};
	printf("%d\n",today.mouth);
	sr(today);
	int b;
	scanf("%d",&b);
} ;

void sr(struct right today){
	printf("%d",today.year);
	
}
struct right *fanh(struct right*p) {
	
	printf("%d\n",p->mouth);
    return p;
}
