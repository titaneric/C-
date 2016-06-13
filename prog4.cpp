#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
//#include <algorithm>
#include <ctime>
#include <unistd.h>
#define NUM_THREAD 30
#define NUM_SEAT 3
#define NUM_TA 2
using namespace std;
pthread_mutex_t mutextime=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexta=PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[NUM_THREAD];
pthread_t ta;
int chair[3]={};
int numStudent = 0;
int count = 0;
int next_seat = 0;
int next_teach = 0;
void *ask(void * stu_id){
    int id = *(int *)stu_id;
    
    while(true){
        if(numStudent < 60){
            if(numStudent == 0) //the first student coming
                sleep(rand() % 31);
            else    //the other students coming
                sleep(rand() % 21 + 10);
                
            pthread_mutex_lock(&mutextime);
            if(count < NUM_SEAT){ //if # of seating student is less than # of chairs, then student sits and waits 
                time_t t = time(0);
                struct tm * now = localtime( & t );
                
                cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
                chair[next_seat] = id;
                count++;
                
                cout<<"Student "<<chair[next_seat]<<": Sitting #"<<(next_seat + 1)<<endl;
                cout<<"Waiting students are #1:"<<chair[0]<<", #2:"<<chair[1]<<", #3:"<<chair[2]<<"(0 stands for empty seat)"<<endl;
                next_seat = (next_seat+1) % NUM_SEAT;//FCFS, circular wait
                pthread_mutex_unlock(&mutextime);
            }
            else{ //else, the student comes after 5 sec 
                pthread_mutex_unlock(&mutextime);
                sleep(5);
            }
        }
        if(numStudent == 60){ //if # of serving student is equal to 60, terminate the thread
            pthread_exit(NULL);
        }
    
    }
 
}
void *answer(void *){
    bool state = false;//denote the TA's current state is busy or not, false denote busy
     while(numStudent < 60){
        
        if(chair[next_teach] != 0){ //if the chairs aren't empty, let student asks question
            pthread_mutex_lock(&mutextime);
            state = false;
            time_t t = time(0);
            struct tm * now = localtime( & t );
            int stu = chair[next_teach];
            chair[next_teach] = 0;
            cout<<"Waiting students are #1:"<<chair[0]<<", #2:"<<chair[1]<<", #3:"<<chair[2]<<"(0 stands for empty seat)"<<endl;
            cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
            cout<<"TA : Teaching student "<<stu<<endl;
            sleep(3);
            numStudent++;
            //cout<<numStudent<<endl;
            cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
            cout<<"Student "<<stu<<": Exit to programming"<<endl;
            cout<<"The number of serving student is "<<numStudent<<endl;
            
            count--;
            next_teach = (next_teach + 1) % NUM_SEAT; //FCFS, circular waiting
            
            pthread_mutex_unlock(&mutextime);
            pthread_mutex_unlock(&mutexta);
        }
        else{//else, the TA will nap or pee
            time_t t = time(0);
            struct tm * now = localtime( & t );
            if(state == 0){
                int taState = rand()%2;
                if(taState == 0){
                    cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
                    cout<<"TA : Napping"<<endl;
                 }
                else{
                    
                    cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
                    cout<<"TA : Pee-Pee"<<endl;
                    sleep(5);
                    cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
                    cout<<"TA : Back to office"<<endl;
                }
                state = true;
            }
            
            //next_teach = (next_teach + 1) % NUM_SEAT;
            pthread_mutex_unlock(&mutexta);
        }
        
    }
    if(numStudent == 60){//if the # of serving student is equal to 60, terminate other threads and itself
        time_t t = time(0);
        struct tm * now = localtime( & t );
        cout<<now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec<<"-";
        cout<<"TA : Exit"<<endl;
        pthread_mutex_unlock(&mutexta);
        for(int i = 0;i<NUM_THREAD;i++){
            pthread_cancel(threads[i]);
        
        }
       
        pthread_exit(NULL);
    }
}
int main(){
    srand(0);
    
    
    
    pthread_attr_t attr;  
    pthread_mutex_init(&mutextime, NULL);
    pthread_mutex_init(&mutexta, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    void *status;
    int student_id[NUM_THREAD];
    int ta_id[NUM_TA];
    pthread_create(&ta, NULL, &answer,NULL);
    for(int i = 0;i<NUM_THREAD;i++){
   
        student_id[i] = i + 1;
        
        pthread_create(&threads[i], NULL, &ask, (void *)&student_id[i]);
        
        
        
    }
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutextime);
    pthread_mutex_destroy(&mutexta);
    pthread_join(ta, &status);
    for(int i=0;i<NUM_THREAD;i++){
        pthread_join(threads[i], &status);
    }
    //cout<<"Program exit"<<endl;
    pthread_exit(NULL);
    
    
    
    return 0;
    
}