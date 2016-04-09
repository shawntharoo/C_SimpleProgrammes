#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//declare node
struct list_el
{
	float val,val1;
	struct list_el * next;
};

//Define Node
typedef struct list_el item;

int main()
{
//Define Node data
item * curr, * head;
head = NULL;

//Define File Pointer
FILE* pointer=NULL;
float array[20];
int j;
double total=0;
double total_se=0;
double avg=0;
double avg_se=0;
double std=0;
double varience=0;
double std_se=0;
double varience_se=0;

//Open the data file
pointer = fopen("Data.txt", "r");

//Store the data to a array
for(j=0;j<20;j++)
{
	fscanf(pointer, "%f", &array[j]);
}

//make nodes and store data inside the node
for(j=0;j<20;j++)
{
	curr = (item *)malloc(sizeof(item));
	curr->val = array[j];
	curr->next  = head;
	head = curr;
}

curr = head;

//Get the total of the values of nodes
for(j=0;j<20;j++)
{
	if(j<10){
		printf("%.2f\n", curr->val);
		total=total+curr->val;
		curr = curr->next ;
	}
	else{
		printf("%.2f\n", curr->val);
		total_se=total_se+curr->val;
		curr = curr->next ;
	}
}

//calculate the average of data
avg=total/10.0;
avg_se=total_se/10.0;

curr = head;

//Calculate the varience of the values of the nodes
for(j=0;j<20;j++)
{
	if(j<10){
		varience=varience+(((curr->val)-avg)*((curr->val)-avg));
		curr = curr ->next;
	}
	else{
		varience_se=varience_se+(((curr->val)-avg_se)*((curr->val)-avg_se));
		curr = curr ->next;
	}
}

//calculate the squreroot
std = sqrt(varience/9.0);
std_se = sqrt(varience_se/9.0);

//Print the answers
printf("Column 1 :\n");
printf("Mean : %.2f		Std.Dev : %.2f\n\n",avg_se,std_se);

printf("Column 2 :\n");
printf("Mean : %.2f		Std.Dev : %.2f\n\n",avg,std);

//close the file pointer
fclose(pointer);
return 0;

}