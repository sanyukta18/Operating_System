#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<time.h>
#include<signal.h>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include<chrono>
#include<bits/stdc++.h>
#include <locale>
#include <semaphore.h>
#include <string>

#include <vector>
using namespace std;
using namespace std::chrono;
struct JOB
{
	int producer_number;
	int status;
	int M[1000*1000];
	int matrix_id;
};

struct standard_qu
{
	JOB a[11];
	int sz;
	void init()
	{
		sz = 0;
	}
	JOB get_top()
	{
		int indexTop = 0;
		JOB ret = a[indexTop];
		sz--;
		
		for(int i = indexTop; i < sz; i++)
		{
			a[i] = a[i + 1];
		}
		
		return ret;
	}
	bool empty()
	{
		return sz == 0;
	}
	bool is_full()
	{
		return sz == 10;
	}
	void insert(JOB x)
	{
		a[sz] = x;
		sz++;
	}
};

struct shared_data
{
	int job_created;
	int job_completed;
	int NJ;
	standard_qu q;
	int consumer_update;
	int producer_update;
	sem_t semaphore_consumer;
	sem_t semaphore_producer;
	void init()
	{
		job_created = 0;
		job_completed = 0;
		NJ = -1;
		q.init();
		consumer_update = 0;
		producer_update = 0;
		sem_init(&semaphore_producer, 1, 0);
		sem_init(&semaphore_consumer, 1, 0);
	}
};

JOB generate_job(int k)
{
	JOB ans;
	ans.matrix_id = ((rand()) % 100000) + 1;
	ans.producer_number = (rand() % 100) + 1;
	ans.status = 0;
	//make matrix here 
	if(k == 0)
		{
			for(int i = 0;i<1000;i++)
			{
				for(int j=0 ;j< 1000;j++)
				{
					int val = (rand() % 19)  - 9;
					//ans.M[j] = val;
					ans.M[i*1000+j] = val;
					//cout<<ans.M[i*10+j]<<" ";
				}
				
				//cout<<"\n";
			}
		}

		if(k == 1)
		{
			for(int j = 0;j<1000000;j++)
			{
				ans.M[j] = 0;		
			}
		}

	return ans;
}
unsigned int sizeof_dm(int rows, int cols, size_t sizeElement)
{
    size_t size = rows * (sizeof(void *) + (cols * sizeElement));
    return size;
}
int main()
{

	cout << "debug "<<'\n';
	fflush(stdout);
	//key_t key = ftok("lavda", 65);
	 int shmid = shmget(IPC_PRIVATE,400000000,IPC_CREAT|0600);
	 shared_data *s = (shared_data *)shmat(shmid, (void *)0, 0);
	// (*s).init();

	cout << "Enter number of producer:\n";
	int NP;
	cin >> NP;
	 int sum = 0;
	//cout << "Enter number of jobs : ";
	cout << "Enter number of matrices to be multiplied:\n";
	cin >> (*s).NJ;
	//cout << "JOBS  : " << (*s).NJ << endl;
	//;
	int flag3 = 0;
	cout << "Enter number of workers:\n";
	int NW;
	cin >> NW;
	// cout<< "Enter number of matrices to be multiplied:\n";
	// //cin >> N;
	// cin >> (*s).NJ;
	bool flag = 1; //1 if process is god
	int pro_num;
	auto start=high_resolution_clock::now();
	for(pro_num = 1; pro_num <= NP; pro_num++)
	{
		int pid = fork();
		//cout<<"pid is "<<pid<<'\n';
		if (pid == 0)
		{
			flag = 0;
			break;
		}
		//cout << "producer  " << pro_num << " :  " << pid << endl;
		//cout<<"flag "<<flag<<'\n';
	}

	if (flag == 0)
	{
		// do producer thing
		//s = (shared_data*) shmat(shmid,NULL,0);
		// --debug cout<<"Inside child process\n";
		// --debug cout<<"NJ is "<< (*s).NJ<<"\n";
		// --debug cout<<"job created "<<((*s).job_created)<<"\n";
		// --debug cout<<"check full "<<(*s).q.is_full() <<"\n";
		// while (((*s).NJ) == -1)
		// 	;
		while ((*s).job_created < ((*s).NJ))
		{
			if ((*s).q.is_full())
				continue;
			sem_wait(&((*s).semaphore_producer));
			sleep(rand()%4);
			// int tt=10;
			// while(tt--) cout<<pro_num<<" ";cout<<endl;
			//cout<<"JOBS  : "<<(*s).NJ<<endl;
			//cout<<"DEBUG2"<<"\n";
			if ((*s).job_created < ((*s).NJ) && (*s).q.is_full() == 0)
			{
				//cout << "produced : " << (*s).job_created + 1 << " in  " << pro_num << endl;
				(*s).job_created++;
				JOB J = generate_job(0);

				cout<<"Job Genereated Producer number: "<< J.producer_number<<" Matrix_id generated "<<J.matrix_id<<"\n";
				int *m = J.M;
				//cout<<"Matrix check: \n";
				// for(int i=0;i<10;i++)
				// {
				// 	for(int j=0;j<10;j++)
				// 	{
				// 		cout<<m[i*10+j]<<" ";
				// 	}
				// 	cout<<"\n";
				// }
				// cout<<"\n";
				(*s).q.insert(J);
			}
			// tt=10	;
			// while(tt--) cout<<pro_num<<" ";cout<<endl;
			// (*s).producer_update=0;

			sem_post(&((*s).semaphore_producer));
		}
		//cout << "producer ended num : " << pro_num << endl;
		//shmdt(s);
		exit(0);
	}
	
	int con_num;
	for (con_num = 1; con_num <= NW; con_num++)
	{
		int pid = fork();
		if (pid == 0)
		{
			flag = 0;
			break;
		}
		//cout << "consumer is " << con_num<<" "<<'\n';
	}
	if (flag == 0)
	{
		// do consumer thing
		//s = (shared_data*) shmat(shmid,NULL,0);
		while (((*s).NJ) == -1)
			;

		while ((*s).job_completed < ((*s).NJ))
		{
			if ((*s).q.empty())
				continue;
			bool flag1 = 0;
			int mat1[500*500]; int mat2[500*500]; int res[500*500];
			for(int i=0;i<500;i++)
			{
				for(int j=0;j<500;j++)
				{
					res[i*5+j] = 0;
				}
			}
	 
			sem_wait(&((*s).semaphore_consumer));
			sleep(rand()%4);
			int lr1,hr1,lc1,hc1,lr2,hr2,lc2,hc2,rr1,rr2,rc1,rc2;
			//cout<<"debug\n"<<'\n';
			if ((*s).job_completed < ((*s).NJ) && (*s).q.empty() == 0)
			{
				// temp1 = (*s).q.get_top();
				// temp2 = (*s).q.get_top();
				flag1 = 1; 
				//retrive appx blocks from top two matrices in queue and multiply
				int *a = (*s).q.a[0].M; int *b = (*s).q.a[1].M;  int *r; //(*s).q.a[10].M;
				cout<<"WORKER NUMBER: "<<(rand()%NW)+1<<"\n\n";
				cout<<"PRODUCER NUMBER of MATRIX 1 "<<(*s).q.a[0].producer_number<<" MATRIX ID of MATRIX1 "<<(*s).q.a[0].matrix_id<<"\n";
				cout<<"PRODUCER NUMBER of MATRIX 2 "<< (*s).q.a[1].producer_number<<" MATRIX ID of MATRIX 2 "<<(*s).q.a[1].matrix_id<<"\n";
				if((*s).q.a[0].status == 0 && (*s).q.a[1].status == 0)
				{
						
					// cout<<"Matrix1: \n";
					// for(int i=0;i<10;i++)
					// {
					// 	for(int j=0;j<10;j++)
					// 	{
					// 		cout<<a[i*10+j] <<" ";
					// 	}
					// 	cout<<"\n";
					// }
					// cout<<"Matrix2:" << "\n";
					// for(int i=0;i<10;i++)
					// {
					// 	for(int j=0;j<10;j++)
					// 	{
					// 		cout<<b[i*10+j] <<" ";
					// 	}
					// 	cout<<"\n";
					// }
				}
				//cout<<"Status of matrix1 "<<(*s).q.a[0].status<<" Status of matrix2 "<<(*s).q.a[1].status<<"\n";
				if((*s).q.a[0].status == 8 && (*s).q.a[1].status == 8)
				{
					sleep(rand()%4);
					continue;
				}
				else
				{
					if((*s).q.a[0].status == 0 && (*s).q.a[1].status == 0)
					{
						if((*s).job_completed == 0)
						{	
							JOB J = generate_job(1);
							(*s).q.a[10] = J;
							r = (*s).q.a[10].M;
							// for(int i=0;i<10;i++)
							// {
							// 	for(int j=0;j<10;j++)
							// 	{
							// 		cout<<r[i*10+j] <<" ";
							// 	}
							// 	cout<<"\n";
							// }
						}
								
						lr1 = 0; hr1 = 499;
						lc1 = 0; hc1 = 499;
						lr2 = 0; hr2 = 499;
						lc2 = 0; hc2 = 499;
						rr1 = 0; rr2 = 499;
						rc1 = 0; rc2 = 499;
						cout<<"BLOCK 1 OF MATRIX 1 AND BLOCK 1 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 1 && (*s).q.a[1].status == 1)
					{
						lr1 = 0; hr1 = 499;
						lc1 = 500; hc1 = 999;
						lr2 = 500; hr2 = 999;
						lc2 = 0; hc2 = 499;
						rr1 = 0; rr2 = 499;
						rc1 = 0; rc2 = 499;
						cout<<"BLOCK 2 OF MATRIX 1 AND BLOCK 3 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 2 && (*s).q.a[1].status == 2)
					{
						
						lr1 = 0; hr1 = 499;
						lc1 = 0; hc1 = 499;
						lr2 = 0; hr2 = 499;
						lc2 = 500; hc2 = 999;
						rr1 = 0; rr2 = 499;
						rc1 = 500; rc2 = 999;
						cout<<"BLOCK 1 OF MATRIX 1 AND BLOCK 2 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 3 && (*s).q.a[1].status == 3)
					{
						//(*s).q.insert(generate_job());
						lr1 = 0; hr1 = 499;
						lc1 = 500; hc1 = 999;
						lr2 = 500; hr2 = 999;
						lc2 = 500; hc2 = 999;
						rr1 = 0; rr2 = 499;
						rc1 = 500; rc2 = 999;
						cout<<"BLOCK 2 OF MATRIX 1 AND BLOCK 4 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 4 && (*s).q.a[1].status == 4)
					{
						//(*s).q.insert(generate_job());
						lr1 = 500; hr1 = 999;
						lc1 = 0; hc1 = 499;
						lr2 = 0; hr2 = 499;
						lc2 = 0; hc2 = 499;
						rr1 = 500; rr2 = 999;
						rc1 = 0; rc2 = 499;
						cout<<"BLOCK 3 OF MATRIX 1 AND BLOCK 1 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 5 && (*s).q.a[1].status == 5)
					{
						//(*s).q.insert(generate_job());
						lr1 = 500; hr1 = 999;
						lc1 = 500; hc1 = 999;
						lr2 = 500; hr2 = 999;
						lc2 = 0; hc2 = 499;
						rr1 = 500; rr2 = 999;
						rc1 = 0; rc2 = 499;
						cout<<"BLOCK 4 OF MATRIX 1 AND BLOCK 3 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 6 && (*s).q.a[1].status == 6)
					{
						//(*s).q.insert(generate_job());
						lr1 = 500; hr1 = 999;
						lc1 = 0; hc1 = 499;
						lr2 = 0; hr2 = 499;
						lc2 = 500; hc2 = 999;
						rr1 = 500; rr2 = 999;
						rc1 = 500; rc2 = 999;
						cout<<"BLOCK 3 OF MATRIX 1 AND BLOCK 2 OF MATRIX 2 IS BEING RETRIVED\n" ;
					}
					if((*s).q.a[0].status == 7 && (*s).q.a[1].status == 7)
					{
						//(*s).q.insert(generate_job());
						lr1 = 500; hr1 = 999;
						lc1 = 500; hc1 = 999;
						lr2 = 500; hr2 = 999;
						lc2 = 500; hc2 = 999;
						rr1 = 500; rr2 = 999;
						rc1 = 500; rc2 = 999;
						//(*s).job_completed++;
						cout<<"BLOCK 4 OF MATRIX 1 AND BLOCK 4 OF MATRIX 2 IS BEING RETRIVED\n";
					}
					flag1 = 1;
					for(int i = lr1;i<=hr1;i++)
					{
						for(int j = lc1;j<=hc1;j++)
						{
							mat1[(i-lr1)*500+(j-lc1)] = a[i*1000+j]; 
						}
					}

					for(int i = lr2;i<=hr2;i++)
					{
						for(int j = lc2;j<=hc2;j++)
						{
							
							mat2[(i-lr2)*500+(j-lc2)] = b[i*1000+j]; 
						}
					}
					//cout<<"Mat1: "<<'\n';
					// for(int i = 0;i<5;i++)
					// {
					// 	for(int j = 0;j<5;j++)
					// 	{
					// 		cout<<mat1[i*5+j]<<" "; 
					// 	}
					// 	cout<<"\n";
					// }


					// 	cout<<"\nMat2: "<<'\n';
					// for(int i = 0;i<5;i++)
					// {
					// 	for(int j = 0;j<5;j++)
					// 	{
					// 		cout<<mat2[i*5+j]<<" "; 
					// 	}
					// 	cout<<"\n";
					// }
					cout<<"Retrived blocks are being multiplied \n";
					for(int i = 0; i < 500; ++i)
	        			for(int j = 0; j < 500; ++j)
	           				 for(int k = 0; k < 500; ++k)
				            {
				            	
				                res[i*500+j] += mat1[i*500+k] * mat2[k*500+j];
				            }

				     //cout<<"Res1: "<<'\n';
					// for(int i = 0;i<5;i++)
					// {
					// 	for(int j = 0;j<5;j++)
					// 	{
					// 		cout<<res[i*5+j]<<" "; 
					// 	}
					// 	cout<<"\n";
					// }
					// cout<<"\n";

				    r = (*s).q.a[10].M;
				    cout<<"Retrived blocks product is being added to final sum\n";
				     for(int i = rr1;i<=rr2;i++)
					{
						for(int j = rc1;j<=rc2;j++)
						{
							
							r[i*1000+j] = r[i*1000+j]+res[(i-rr1)*500+(j-rc1)];
						}
					}

					  //cout<<"Result final 1: "<<'\n';
					// for(int i = 0;i<10;i++)
					// {
					// 	for(int j = 0;j<10;j++)
					// 	{
					// 		cout<<r[i*10+j]<<" "; 
					// 	}
					// 	cout<<"\n";
					// }

				    (*s).q.a[0].status++;
				    (*s).q.a[1].status++;
				    cout<<"Status of matrix1 "<<(*s).q.a[0].status<<" Status of matrix2 "<<(*s).q.a[1].status<<"\n\n";
				    if((*s).q.a[0].status == 8 && (*s).q.a[1].status == 8)
					{
						//(*s).q.insert(generate_job());
						if(flag3 == 0)
						{
							(*s).job_completed = (*s).job_completed + 2;
							flag3 = 1;
						}
						else
						{
							(*s).job_completed = (*s).job_completed + 1;
						}
						(*s).q.get_top();
						(*s).q.a[0].status = 0;
						int *mtrx = (*s).q.a[0].M;
						 r = (*s).q.a[10].M;
						for(int i = 0;i<1000;i++)
						{
							for(int j = 0;j<1000;j++)
							{
								mtrx[i*1000+j] = r[i*1000+j]; 
							}
							//cout<<"\n";
						}

						for(int i = 0;i<1000;i++)
						{
							for(int j = 0;j<1000;j++)
							{
								r[i*1000+j] = 0; 
							}
							//cout<<"\n";
						}
						//(*s).q.get_top();

						cout<<"\n\n";
					}
					
				}

				sem_post(&((*s).semaphore_consumer));
				if (flag1)
					sleep(rand()%4);
			}
		}
		int *mtrx = (*s).q.a[0].M;
		int siz = (*s).q.sz;
		//cout<<"size is "<<siz<<"\n";
		if(siz == 1)
		{	
			for(int i=0;i<1000;i++)
			{
				for(int j=0;j<1000;j++)
				{
					if(i==j)
					{
						//cout << mtrx[i*10+j]<<"\n";
						sum = sum + mtrx[i*1000+j];
						//cout<<"curr sum is "<<sum <<'\n';

					}
					
				}
				
			}

				cout<<"sum is "<<sum<<"\n";
		}
	
	

		//cout << "consumer ended num : " << con_num << endl;

		auto stop=high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop-start);
	cout<<"Time Elapsed "<<duration.count()<<" seconds"<<"\n";
		//pthread_exit(NULL);
		cout<<"-------Terminated-----";
		shmdt(s);
		exit(0);
	}

	// 
	// auto stop=high_resolution_clock::now();
	// auto duration = duration_cast<seconds>(stop-start);
	// cout<<"Time Elapsed "<<duration.count()<<" seconds"<<"\n";

	sem_post(&((*s).semaphore_producer));
	sem_post(&((*s).semaphore_consumer));
	shmdt(s);
	exit(0);

	return 0;
}