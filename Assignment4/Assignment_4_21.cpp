#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <time.h>
//#include <conio.h>
 
// ranges of process variables
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#define MIN_JOB_ID 1
#define MAX_JOB_ID 100000
#define MIN_COMPUTE_TIME 100
#define MAX_COMPUTE_TIME 20
#define MIN_SLEEP_TIME 250 // in microseconds
#define MAX_SLEEP_TIME 500 // in microseconds

using namespace std;

// check if mutex is defined/suported in the system
#ifndef _POSIX_THREAD_PROCESS_SHARED
    #error No definition found for mutex
    exit(EXIT_FAILURE);
#endif

const int JOB_SIZE = 500;
pthread_mutex_t    *mutex_ptr; // pointer to mutex
pthread_mutexattr_t mutex_attr; // mutex attribute

struct Node
{
    int job_id;
    int time_completion;
    int dependent_jobs[100];
    int childs;
    //pthread_mutex_t mutex; // mutex
    //mutex lock
    int parent;
    int status;
};

struct job_buffer{
    int size = 0; // current buffer size (number of elements stored in buffer currently)
    int job_created; // number of jobs created
    int job_completed; // number of jobs completed
    pthread_mutex_t mutex; // mutex
    //priority_queue<process_t, vector<process_t>, ComparePriority> process_queue; // priority queue of processes
    Node Arr[500];
   
};

job_buffer* buffer = new job_buffer;
int NJOBS, producer_count = 0, consumer_count = 0;

// populate process structure with given pid and producer number
Node create_job()
{
    cout<<"Creating job";
    Node job;
    // populate data members of the process
    job.job_id = rand()%100000000;
    //job.producer_number = producer_number;
    //job.priority = rand() % MAX_PRIORITY + MIN_PRIORITY; // value ranges from 1 to 10
    job.time_completion = rand()%MIN_SLEEP_TIME + 1; // value ranges from 1 to 4
    //job.jid = rand() % MAX_JOB_ID + MIN_JOB_ID; // value ranges from 1 to 100000
    job.status = -1;
    return job;
}

// inserting job in the priority queue such that job with highest priority is at the root
// of the max heap
void insert_in_buffer(Node job)
{
    int s = buffer->size;
    if(s>=499)
        return;
    int index = rand()%s;
    buffer->Arr[s] = job;  // pushing job in priority queue
     buffer->Arr[index].dependent_jobs[buffer->Arr[index].childs] = s;
     (buffer->Arr[index].childs)++;
    (buffer->size)++;  // increment buffer sizes
}

int counter = 0; int flag = 0;

void *producer_task(void *param)
{

    int producer_num = producer_count;
    producer_count++;
    // producer code
    int timenow, time = 10+rand()%11;
    time = time*100;
    timenow = 0;
    while(1)
    {  
        Node job;
        
        //sleep(1);
        pthread_mutex_lock(mutex_ptr);
        // check if more jobs are to be crated and buffer has remaining space
      //  cout<<"buffer size is "<<buffer->size<<"\n";
        if((buffer->job_created) < NJOBS  && buffer->size < JOB_SIZE && timenow < time)
        {
            job.job_id = rand()%100000000;
            job.time_completion = rand()%4 + 1; // value ranges from 1 to 4
            //job.jid = rand() % MAX_JOB_ID + MIN_JOB_ID; // value ranges from 1 to 100000
            job.childs = 0;
            job.status = -1;
    //cout<<"Finished creation\n";

           int s = buffer->size;
            buffer->Arr[s] = job;
           if(s != 0 )
           {
           
                while(1)
                {
                    int index = rand()%s;
                    if(buffer->Arr[index].status == -1)
                    {
                        buffer->Arr[index].dependent_jobs[buffer->Arr[index].childs] = s;
                        (buffer->Arr[index].childs)++;
                        job.parent = index;
                        cout<<"\x1b[33mProducer created new job\x1b[0m"<<endl;
                        cout << "ID of new job "<< job.job_id<<"\n";
                        cout << "New job added to "<< index<<" node in tree"<<"\n";
                        int t = rand()%MAX_SLEEP_TIME+250;
                        usleep(t);
                        timenow = timenow + t;
                        counter++;
                        cout <<"\n";
                        break;
                    }
               
                }
           
       
              }

           (buffer->job_created)++;
           (buffer->size)++;  
                pthread_mutex_unlock(mutex_ptr); // unlock mutex
        }
        else
        {
            cout << "\x1b[31;1mKilling Producer: " << producer_num << "\x1b[0m" << endl;
            pthread_mutex_unlock(mutex_ptr);
            pthread_exit(0);
        }
    }
}
int total = 0;
void *consumer_task(void *param)
{
    int consumer_num = consumer_count;
    consumer_count++;
    // consumer code
    while (1)
    {  
       // pthread_mutex_lock(mutex_ptr); // lock mutex
       
        if(buffer->job_completed < NJOBS && buffer->size > 0)
   {
       
      for(int i=0;i<buffer->size;i++)
       {
        //pthread_mutex_lock(mutex_ptr);
        if(buffer->Arr[i].childs == 0 && buffer->Arr[i].status==-1)
        {
        pthread_mutex_lock(mutex_ptr);
        buffer->Arr[i].status  = 0;
        cout<<"\x1b[34;1mConsumer working: " <<consumer_num<<"\x1b[0m"<<"\n";
        cout<<"Job Started "<<"\n";
        cout<<"Job id "<< buffer->Arr[i].job_id <<"\n";
        usleep(buffer->Arr[i].time_completion * MIN_COMPUTE_TIME);
       
        buffer->Arr[i].status  = 1;
        int ind = buffer->Arr[i].parent;
        (buffer->Arr[ind].childs)--;
        //process_t job = remove_from_buffer(); // get new job from buffer
            (buffer->job_completed)++;
            cout<<"Jobs completed are "<<buffer->job_completed<<"\n\n";
            pthread_mutex_unlock(mutex_ptr);
            usleep(250);
           break;

        }
        // check if all jobs have been consumed
        else if (buffer->job_completed >= NJOBS)
        {
            pthread_mutex_lock(mutex_ptr);
            //cout << "\x1b[31;1mKilling Consumer: " << consumer_num << "\x1b[0m" <<endl;
            pthread_mutex_unlock(mutex_ptr); // unlock mutex
            pthread_exit(0);
        }
        // no process in buffer, but more processes will be created (number of jobs created is less than NJOBS)
        else 
        {
        //cout<<"\x1b[34;1mConsumer: "<< consumer_num <<"\x1b[0m\x1b[33m Waiting, (queue empty)\x1b[0m"<<endl;
          //i++;
         //cout<<"child running\n";
            //usleep(25); 
            pthread_mutex_lock(mutex_ptr);
            //usleep(25); 
            pthread_mutex_unlock(mutex_ptr); // unlock mutex
           
                  
        }
       }
    }
}
}

int main()
{  
    int i, y,P;
    buffer->size = 0;
    buffer->job_created=0;
    buffer->job_completed = 0;
    mutex_ptr = &(buffer->mutex);
    srand(time(0));
    int num = rand()%201 + 300;
    //cout<<"num is "<<num<<'\n';
    NJOBS = num;
     buffer->job_created= num;
    for(int j=0;j<num;j++)
    {
    //cout<<"Inside \n";
         Node job;
         //cout<<"Created \n";

         job.job_id = rand()%100000000;
    job.time_completion =rand()%4; // value ranges from 1 to 4
    //job.jid = rand() % MAX_JOB_ID + MIN_JOB_ID; // value ranges from 1 to 100000
    job.childs = 0;
    job.status = -1;
    //cout<<"Finished creation\n";

        int s = buffer->size;
        buffer->Arr[s] = job;
        if(s == 0)
        {
        job.parent = -1;
        }
        if(s != 0)
        {
        int index = rand()%s;
        buffer->Arr[index].dependent_jobs[buffer->Arr[index].childs] = s;
    (buffer->Arr[index].childs)++;
     job.parent = index;
   
        }

   (buffer->job_created)++;
   (buffer->size)++;  //
     
    }
    buffer->size = num;
    //cout<<"buffer_size "<<buffer->size<<"\n";
    cout<<"Tree created\n";
   

    cout << "number of producers=";
    cin >> P;
    cout << "\n";
    NJOBS = 500;
    cout << "number of consumers=";
    cin >> y;
    cout << "\n";
   
    //NJOBS = P;

   
    buffer->job_created=num;
    buffer->job_completed = 0;
    mutex_ptr = &(buffer->mutex);
    // buffer -> f = 0;
    int error_code;
    // setup mutex and catch any errors that might occur
    if (error_code = pthread_mutexattr_init(&mutex_attr))
    {
        fprintf(stderr,"pthread_mutexattr_init: %s", strerror(error_code));
        exit(EXIT_FAILURE);
    }
    if (error_code = pthread_mutexattr_setpshared(&mutex_attr,PTHREAD_PROCESS_SHARED))
    {
        fprintf(stderr,"pthread_mutexattr_setpshared %s",strerror(error_code));
        exit(EXIT_FAILURE);
    }
    if (error_code = pthread_mutex_init(mutex_ptr, &mutex_attr))
    {
        fprintf(stderr,"pthread_mutex_init %s",strerror(error_code));
        exit(EXIT_FAILURE);
    }
    time_t start_time = time(NULL);

    pthread_t producer_thread[P];
     pthread_t consumer_thread[y];

    //pthread_t consumer_thread[y];

    pthread_attr_t attr;
    pthread_attr_init (&attr); // get default attributes

    //cout<<"start creating producers...."<<endl;
    // create producer threads
     /* start timer */
    pid_t pid=fork();
    if(pid!=0)
    {
         //cout<<"Creating Producers "<<"\n";
        for (int i = 0; i < P; i++)
        {
            pthread_create(&producer_thread[i], &attr, producer_task, &producer_thread[i]); // create the thread
            //cout<<"\n"<<producer_thread[i]<<endl;
        }
         //cout<<"Done with Producers "<<"\n";  
        sleep(5);
        int returnStatus; 
        if (returnStatus == 0)  // Verify child process terminated without error.  
        {
            //int flag = 0;
            for (i = 0; i < y; i++)
            {
               //cout << "Finished consuming "<<"\n";
                cout << "\x1b[31;1mKilling Consumer: " << y-i-1 << "\x1b[0m" <<endl;
            } //wait for the threads to exit
            
            for (i = 0; i < P; i++)
            {
                // cout << "flag is "<< flag<<"\n";
                 if(flag == 0)
                 {

                    cout << "Jobs created are "<<counter<<"\n";
                     flag = 1;
                 }
                
               pthread_join(producer_thread[i],NULL); //wait for the threads to exit
               //flag = 0;
            }   
        }
        
    }
   else
    {
        //cout<<"Creating Consumers "<<"\n";
     for (i = 0; i < y; i++)
       pthread_create(&consumer_thread[i], &attr, consumer_task, NULL); // create the thread
       
       //cout<<"Done with Consumers "<<"\n";   
      for (i = 0; i < y; i++)
      {
        //cout << "Finished consuming "<<"\n";
        pthread_join(consumer_thread[i], NULL);
      } //wait for the threads to exit

      exit(0);
    }
   
    
    
    return 0;
}

