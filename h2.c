//Programme Name - PSP0.1
//Name - K.K.S.T.Adikaram
//Date - 2016/04/20
//Description - Calculate Regression Parameteres, Coefficient , corelation using linked list

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
double xsq;
double ysq;
double B1;
double B0;
double R;
double rsq;
double rpart;
double rdevide;
double estimate;
double total_x=0;
double total_y=0;
double total_xsq=0;
double total_ysq=0;
double total_xy=0;
double avg_x=0;
double avg_y=0;

//Open the data file
pointer = fopen("Homework2_Test1.txt", "r");

//Store the data to arrays from the pointer
for(j=0;j<10;j++)
{
	fscanf(pointer, "%f\t%f", &array[j],&array1[j]);
}

//Print the Input Data
for(j=0;j<10;j++)
{
	printf("%.2f\n", array[j]);
	printf("%.2f\n", array1[j]);
}

//Assigning two values which are taken from two arrays to Each node 
for(j=0;j<10;j++)
{
	curr = (item *)malloc(sizeof(item));
	curr->val = array[j];
	curr->val1 = array1[j];
	curr->next  = head;
	head = curr;
}

//Get the total of the values of nodes and
//Get the total of squareroot of  values
for(j=0;j<10;j++)
{

		total_y=total_y+curr->val1;
		ysq = curr->val1;
		total_ysq = total_ysq + (ysq*ysq);

		total_x=total_x+curr->val;
		xsq = curr->val;
		total_xsq = total_xsq + (xsq*xsq);

		total_xy = total_xy + (curr->val1 * curr->val);

		curr = curr->next ;
	
}

//calculate the average of data
avg_x=total_x/10.0;
avg_y=total_y/10.0;

//Calculate the B1 Regression parameter
B1 = ((total_xy)-(10*avg_x*avg_y))/((total_xsq)-(10*avg_x*avg_x));

//Calculate the correlation
rpart=sqrt(((10*total_xsq)-(total_x*total_x))*((10*total_ysq)-(total_y*total_y)));
R = (10*(total_xy))-((total_x)*(total_y));
rdevide = R/rpart;

//Calculate the coefficient
rsq = rdevide * rdevide;

//Calculate the B0 Regression parameter
B0 = (avg_y - (B1*avg_x));

//Calculate the Estimate Value
estimate = B0 + (B1*386);

//Print the Answers
printf("Linear Regression Parameters \n");
printf("B0 : %.3f\n",B0);
printf("B1 : %.3f\n\n",B1);

printf("Correlation \n");
printf("R : %.3f\n\n",rdevide);

printf("Coefficients \n");
printf("R square : %.3f\n\n",rsq);

printf("Estimate given an estimated proxy size of 386 \n");
printf("Estimation : %.3f\n\n",estimate);

//close the file pointer
fclose(pointer);

return 0;

}//End of Main