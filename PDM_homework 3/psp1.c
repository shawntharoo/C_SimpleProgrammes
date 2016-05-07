//Programme Name - PSP1
//Name - K.K.S.T.Adikaram
//Date - 2016/04/20
//Description - Calculate The Relative Size Table using linked list

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

//Start Of Main
int main()
{
//Define Node properties
item * curr, * head;
head = NULL;

//Define File Pointer
FILE* pointer=NULL;

//Define Variables
float array[20];
float array1[20];
int j;
double VS;
double S;
double M;
double L;
double VL;
double total_y=0;
double avg_y=0;
double varience;
double std;

//Open the data file
pointer = fopen("Homework3_Test1.txt", "r");

//Store the data to a array from the pointer
for(j=0;j<13;j++)
{
	fscanf(pointer, "%f", &array[j]);
}

//Print the Input Data
	printf("Input Data\n\n");
for(j=0;j<13;j++)
{
	printf("%.4f\n", array[j]);
}

//Assigning the values to Each node 
//Calculate the log Value of each node
//print the Log value
printf("\n");
printf("After Converting To the Log Values\n\n");
for(j=0;j<13;j++)
{
	curr = (item *)malloc(sizeof(item));
	curr->val = array[j];
	curr->val1 = log(curr->val);
	total_y=total_y+curr->val1;
	printf("%.4f\n", curr->val1);
	curr->next  = head;
	head = curr;
}

//calculate the average of data
avg_y=total_y/13.0;

for(j=0;j<13;j++)
{
	//printf("%.4f\n", curr->val1);
		varience=varience+(((curr->val1)-avg_y)*((curr->val1)-avg_y));
		curr = curr ->next;
}


//calculate the squreroot of Varience
std = sqrt(varience/12.0);

//Calculating Logarithamic Changes
VS= avg_y -(2*std);
S = avg_y - std;
M = avg_y;
L = avg_y + std;
VL= avg_y +(2*std);

//Calculating anti-Logaritham
VS= exp(VS);
S= exp(S);
M= exp(M);
L= exp(L);
VL= exp(VL);

//Print the Answers
printf("\n");
printf("Varience And Standard Deviation \n\n");
printf("Varience : %lf\n",varience);
printf("Standard Deviation : %.4f\n\n",std);

printf("Relative Size Table Values \n\n");
printf("Very Small Value : %.4f\n",VS);

printf("Small Value : %.4f\n",S);

printf("Medium Value : %.4f\n",M);

printf("Large Value : %.4f\n",L);

printf("Very Large Value : %.4f\n",VL);

//close the file pointer
fclose(pointer);

return 0;

}//End of Main