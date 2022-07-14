#include "memlab.h"


// int size =  400000;
 int MAXSIZE = 50000;       
int stack[50000];     
int top = -1; 

pthread_mutex_t    *mutex_ptr; // pointer to mutex
pthread_mutexattr_t mutex_attr; // mutex attribute

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}
int pop() {
   int data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

int push(int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

typedef struct pg
{
	
	int localaddress;
	int physicaladdress;
	pthread_mutex_t mutex; 
	int sizemem;
	int is_available;
    void *location;
  char varname[50];
  char vartype[15];

}page;

page pg_table[50000];


int counter = 0;
void *ptr; 
void createMem(int size)
{
	ptr = (void*)malloc(size);  
	//printf("Address = %u\n", ptr);
	for(int i=0;i<50000;i++)
	{
		pg_table[i].localaddress = -1;
		pg_table[i].sizemem = 0;
		pg_table[i].is_available = 0;
	    pg_table[i].location = NULL;
	  	strcpy(pg_table[i].varname," ");
	  	strcpy(pg_table[i].vartype," ");
	  //char vartype[15];
	}
	return;
}

void createVar(char* type,char* var_name)
{

	if(strcmp(type,"int")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (int*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (int*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = 1;
		pg_table[counter].is_available = 1;
		//ptr = ptr + 4;
		printf("Marking\n")
		push(counter);
		printf("Variable made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"char")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (char*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (char*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = 1;
		pg_table[counter].is_available = 1;
		//ptr = ptr + 4;
		printf("Marking\n")
		push(counter);
		printf("Variable made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"medium int")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (int*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (int*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = 1;
		pg_table[counter].is_available = 1;
		//ptr = ptr + 4;
		printf("Marking\n")
		push(counter);
		printf("Medium int Variable made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"bool")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		 pg_table[counter].location = (bool*)pg_table[counter].location;
		 void* k = ptr;
		pg_table[counter].location =(bool*)ptr;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = 1;
		pg_table[counter].is_available = 1;
		//ptr = ptr + 4;
		printf("Marking\n")
		push(counter);
		printf("boolean Variable made %s \n",pg_table[counter].varname);

	}

	ptr = ptr + 4;
	//printf("Address variable after = %u\n", ptr);
	counter = counter + 1;
	//printf("\n");

}

void AssignVar(char* var_name,int value)
{

	int i = 0;
	//int s = pg_table.size();
	char type1[15]; 
	
	//printf("%d\n",*p);
	for(i=0;i<counter;i++)
	{
		if(strcmp(pg_table[i].varname,var_name) == 0)
		{
			// pg_table[i].location = (int*)pg_table[i].location;
			//printf(typeof(pg_table[i].location));
			strcpy(type1,pg_table[i].vartype);
			
			if(strcmp(type1,"int")==0)
			{
				int *p;
				p = &value;
				memcpy(pg_table[i].location,p,4);
				//printf("val is %d\n",*((int*)pg_table[i].location));
			}

			if(strcmp(type1,"char")==0)
			{
				char *p2;
				char c1 = (char*)value;
				p2 = &c1;
				memcpy(pg_table[i].location,p2,1);
				//printf("val is %c\n",*((char*)pg_table[i].location));
			}
			if(strcmp(type1,"medium int")==0)
			{
				int *p3;
				p3 = &value;
				printf("value got is %d\n",value);
				memcpy(pg_table[i].location,p3,3);
				//printf("medium int val is %d\n",*((int*)pg_table[i].location));
			}

			if(strcmp(type1,"bool")==0)
			{
				bool *p4;
				bool b1 = (bool*)value;
				p4 = &b1;
				memcpy(pg_table[i].location,p4,1);
				//printf("val is %d\n",*((int*)pg_table[i].location));
			}
			
		}
	}
	//printf("\n");
}

void createArr(char* type,char* var_name,int size)
{

	if(strcmp(type,"int")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (int*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (int*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = size;
		//for(int i=0;i<size;i++)
		ptr = ptr + 4*size;
		
		//ptr = ptr + 4;
		printf("Marking\n")
		push(counter);
		printf("Array made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"char")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (char*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (char*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = size;
		pg_table[counter].is_available = 1;
		//for(int i=0;i<size;i++)
		ptr = ptr + size
		printf("Marking\n");
		push(counter);
		//ptr = ptr + 4;
		printf("char Array made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"medium int")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (int*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (int*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = size;
		pg_table[counter
			printf("Marking\n")].is_available = 1;
		push(counter);
		//for(int i=0;i<size;i++)
		ptr = ptr + 3*size;
		
		//ptr = ptr + 4;
		printf("Medium int Array made %s \n",pg_table[counter].varname);

	}

	if(strcmp(type,"bool")==0)
	{
		pg_table[counter].localaddress = counter;
		//?pg_table[counter].virtualaddress = ;
		pg_table[counter].location = (bool*)pg_table[counter].location;
		void* k = ptr;
		pg_table[counter].location = (bool*)k;

		//printf("Address variable = %u\n", pg_table[counter].location);
		strcpy(pg_table[counter].varname,var_name);
		strcpy(pg_table[counter].vartype,type);
		pg_table[counter].sizemem = size;
		pg_table[counter].is_available = 1;
		//for(int i=0;i<
		printf("Marking\n")//size;i++)
		push(counter);
		ptr = ptr + size;
		
		//ptr = ptr + 4;
		printf("Bool Array made %s \n",pg_table[counter].varname);

	}
	
	//printf("Address variable after = %u\n", ptr);
	counter = counter + 1;
	//printf("\n");

}

void assignArr(char* var_name,int value,int index)
{
	// printf("value given is %d\n",value);
	// printf("index is %d\n",index);
	int i = 0;
	//int s = pg_table.size();
	int *p;
	p = &value;
	//printf("%d\n",*p);
	char type2[15]; 

	for(i=0;i<counter;i++)
	{
		
		if(strcmp(pg_table[i].varname,var_name) == 0)
		{
			// pg_table[i].location = (int*)pg_table[i].location;
			//printf(typeof(pg_table[i].location));
			strcpy(type2,pg_table[i].vartype);
			if(strcmp(type2,"int")==0)
			{
				int *k1 = pg_table[i].location;
			 	k1 = k1 + 4*index;
				memcpy(k1,p,4);
				//printf("val is %d\n",*((int*)k1));
			}

			if(strcmp(type2,"char")==0)
			{
				char *k2 = pg_table[i].location;
			 	k2 = k2 + index;
				memcpy(k2,p,1);
				//printf("char val is %c\n",*((char*)k2));
			}
			if(strcmp(type2,"medium int")==0)
			{
				int *k3 = pg_table[i].location;
			 	k3 = k3 + 3*index;
				memcpy(k3,p,3);
				//printf("mint val is %d\n",*((int*)k3));
			}
			if(strcmp(type2,"bool")==0)
			{
				bool *k4 = pg_table[i].location;
			 	k4 = k4 + index;
				memcpy(k4,p,1);
				//printf("bool val is %d\n",*((int*)k4));
			}
			
		}
	}
	//printf("\n");
}

int returnVar(char* var_name)
{

	int i = 0;
	//int s = pg_table.size();
	char type1[15]; 
	
	//printf("%d\n",*p);
	for(i=0;i<counter;i++)
	{
		if(strcmp(pg_table[i].varname,var_name) == 0)
		{
			// pg_table[i].location = (int*)pg_table[i].location;
			//printf(typeof(pg_table[i].location));
			//strcpy(type1,pg_table[i].vartype);
			return(*((int*)pg_table[i].location));
			
			
		}
	}
	//printf("\n");
}


int returnArr(char* var_name,int index)
{
	// printf("array name %s\n",var_name);
	// printf("index in arr %d\n",index);
	int i = 0;
	//int s = pg_table.size();
	int *p;
	//p = &value;
	//printf("%d\n",*p);
	char type2[15]; 

	for(i=0;i<counter;i++)
	{
		
		if(strcmp(pg_table[i].varname,var_name) == 0)
		{
			pg_table[i].location = (int*)pg_table[i].location;
			//printf("\nloc is %u\n",(pg_table[i].location));
			strcpy(type2,pg_table[i].vartype);
			if(strcmp(type2,"int")==0)
			{
				int *k1 = pg_table[i].location;
			 	k1 = k1 + 4*index;
				//memcpy(k1,p,4);
				return(*((int*)k1));
			}

			if(strcmp(type2,"char")==0)
			{
				char *k2 = pg_table[i].location;
			 	k2 = k2 + index;
				//memcpy(k2,p,1);
				return(*((int*)k2));
			}
			if(strcmp(type2,"medium int")==0)
			{
				int *k3 = pg_table[i].location;
			 	k3 = k3 + 3*index;
				//memcpy(k3,p,3);
				return(*((int*)k3));
			}
			if(strcmp(type2,"bool")==0)
			{
				bool *k4 = pg_table[i].location;
			 	k4 = k4 + index;
				//memcpy(k4,p,1);
				return(*((int*)k4));
			}
			
		}
	}
	//printf("\n");
}


// typedef struct pg
// {
// 	int localaddress;
// 	int physicaladdress;
// 	int sizemem;
//     void *location;
//   char varname[50];
//   char vartype[15];

// }page;
//void* locs[50000]; 
int counter1 = 0;
void gc_run()
{
	while(!isempty())
	{
		int count = pop();
		int i;
		printf("Sweeping\n");
		for(i=0;i<counter;i++)
		{
			if(pg_table[i].localaddress == count)
			{
				mutex_ptr = &(pg_table[i].mutex);
				pthread_mutex_lock(mutex_ptr);
				//locs[counter1] = (void*)pg_table[i].location;
				printf("variable %s ",pg_table[i].varname);
				pg_table[i].is_available = 0;
				pg_table[i].localaddress = -1;
				pg_table[i].sizemem = 0;
				pg_table[i].location = NULL;
				strcpy(pg_table[i].varname," ");
				strcpy(pg_table[i].vartype," ");
				pthread_mutex_unlock(mutex_ptr);
			}
			printf("is freed\n");

			// pg_table[i].is_available = 0;
			// 	pg_table[i].localaddress = -1;
			// 	pg_table[i].sizemem = 0;
			// 	pg_table[i].location = NULL;
			// 	strcpy(pg_table[i].varname," ");
			// 	strcpy(pg_table[i].vartype," ");

			// printf("1.%d\n",pg_table[i].is_available);
			// printf("2.%d\n",pg_table[i].localaddress);
			// printf("3.%d\n",pg_table[i].sizemem);
			// printf("4.%u\n",pg_table[i].location);
			// printf("5.%s\n",pg_table[i].varname);
			// printf("6.%s\n",pg_table[i].vartype);
		}

	}
	counter = 0;

}
void freeElem()
{
	 pthread_t garbage_collect;

	  pthread_attr_t attr;
    pthread_attr_init (&attr);

     pthread_create(&garbage_collect, &attr,gc_run, &garbage_collect);

     pthread_join(garbage_collect,NULL);
}



// int main()
// {
// 	createMem(40000);
// 	// createVar("int","x");
// 	// AssignVar("x",2);
// 	// printf("\n");

// 	// createVar("int","y");
// 	// AssignVar("y",4);
// 	// printf("\n");

// 	// int v = returnVar("x");
// 	// printf("elem is %d\n",v);

// 	// createArr("int","ar",2);
// 	// assignArr("ar",6,0);
// 	// assignArr("ar",5,1);

// 	// int kk = returnArr("ar",0);
// 	// printf("Value at index 0 is %d\n",kk);

// 	createArr("int","arr",6);
// 	// int product = 1;
// 	for(int i=1;i<=6;i++)
//     {
       
//         assignArr("arr",i,i-1);
//         //printf("\nFibonaci no %d,%d\n",i,fib(i));
//     }
//     printf("arr values are:");
//     for(int i=0;i<6;i++)
//     {
//     	printf("%d ",(returnArr("arr",i)));
//        // product = product*(returnArr("arr",i));
//     }

// 	//freeElem();
// 	// createVar("char","cc");
// 	// AssignVar("cc",'a');
// 	// printf("\n");
// 	// createVar("medium int","mm");
// 	// AssignVar("mm",6);
// 	// printf("\n");
// 	// createVar("bool","bb");
// 	// AssignVar("bb",'0');
// 	// printf("\n");
// 	// createArr("char","y",2);
// 	// assignArr("y",'a',0);
// 	// assignArr("y",'b',0);
// 	// printf("\n");
// 	// createArr("medium int","m",2);
// 	// assignArr("m",6,0);
// 	// assignArr("m",5,1);
// 	// printf("\n");
// 	// createArr("bool","z",2);
// 	// assignArr("z",0,0);
// 	// assignArr("z",1,0);
// 	// printf("\n");
	
	

//  }