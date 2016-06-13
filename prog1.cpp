	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/file.h>
	#include <sys/mman.h>
	#include <sys/wait.h>
	#include <iostream>
	#include <string>
	#include <time.h>
	#include <sstream>
	#include <set>
	#include <algorithm>


	using namespace std;
	void check(string ans, int *result, string guess);
	void generate(int num, string &answer);
	void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
	}
	int main(int argc, char *argv[]){
		
		int r;
		srand(time(NULL));
		cout<<"PLEASE INPUT THE DIGITS YOU WANT TO GUESS: ";
		int num = 0;
		cin>>num;
		string answer="";
        pid_t child_pid;
        /*
        for(int i = 0;i<num;i++){
            int temp = rand() % 10;
            stringstream ss;
            ss<<temp;
            answer += ss.str();
        }
        */
        
       
		generate(num, answer);
		
		
		cout<<answer<<endl;
		int count = 0;
		//cout<<answer<<endl;
		//const size_t SIZE = sysconf(_SC_PAGE_SIZE);
		const int SIZE = 4096;
		const char *name = "smans";
	
		int fd = shm_open(name, O_CREAT | O_TRUNC | O_RDWR, 0666);
		if (fd == -1)
		error_and_die("shm_open");
		r = ftruncate(fd, SIZE);
		if (r != 0)
		error_and_die("ftruncate");
		void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (ptr == MAP_FAILED)
		error_and_die("mmap");
		close(fd);
	    
            
         while(true){ 
            count++;
            cout<<"["<<getpid()<<" User]";
            string guess = "";
            
            
            
            cin>>guess;
            while(guess.length() != num){
                cout<<"Format wrong! Please input the number again!";
                cin>>guess;
            }
            int *a = (int *)ptr;
            stringstream ss;
            ss.str("");
            ss.str(guess);
            int guess_int = 0;
            ss>>guess_int;
            a[0] = guess_int;
             pid_t pid;
             pid = fork();
            
           
                
            int status = 0;
            wait(&status);
            if(pid == 0){ //child process
                
                
                int *a = (int *)ptr;
                stringstream ss;
                ss<<a[0];
                string guess = ss.str();
                check(answer, a, guess);
                
                if(count>1)
                cout<<"["<<child_pid<<" Answer "<<count<<"]"<<a[0]<<"A"<<a[1]<<"B\n";
                else
                cout<<"["<<getpid()<<" Answer "<<count<<"]"<<a[0]<<"A"<<a[1]<<"B\n";
                _exit(0);
        
            
            }
            else if(pid > 0){
                if(count == 1)
                child_pid = pid;
                
                //cout<<"This is parent process, and pid = "<<getpid()<<"\n";
                
                if(a[0] == num){
                    cout<<"["<<getpid()<<" Main "<<count<<"]"<<a[0]<<"A"<<a[1]<<"B Done.\n";
                    break;
                    
                }
                cout<<"["<<getpid()<<" Main "<<count<<"]"<<a[0]<<"A"<<a[1]<<"B\n";
                
               
                
            
            }
            else{
                cout<<"fork() failed!"<<endl;
            }
		}
		
		r = munmap(ptr, SIZE);
		if (r != 0)
		error_and_die("munmap");

		r = shm_unlink(name);
		if (r != 0)
		error_and_die("shm_unlink");



		return 0;
	}

	void check(string ans, int *result, string guess){
		
		int a_count = 0;
		int b_count = 0;
        int *markA = new int[ans.length()];
        int *markB = new int[ans.length()];
        for(int i = 0;i < ans.length() ;i++){
            markA[i] = 0;
        }
        for(int i = 0;i < ans.length() ;i++){
            markB[i] = 0;
        }
        
		for(int i = 0;i < ans.length();i++)
		{
			if(ans[i] == guess[i]){
                a_count++;
                markA[i]++;
                markB[i]++;
            }
		}
        for(int i = 0;i < ans.length();i++){
            
            if(markA[i] == 0){
                for(int j = 0;j < ans.length();j++)
                {
                    if(markB[j] == 0){
                        if(ans[j] == guess[i] && i != j ){
                            b_count++;
                            markB[j]++;
                            break;
                        }
                    }
                    
                }
            }
        }
	

		result[0] = a_count;
		result[1] = b_count;
	}

void generate(int num, string &answer){
        for(int i=0;i<num;i++){
                answer += 'a';
            }
        int a = 0;
        int find;
	    while(a < num){
            int digit = rand() % 10;
            find = 0;
            stringstream ss;
            ss<<digit;
            string str = ss.str();
            answer.replace(a, 1, str);
            //cout<<answer<<endl;
            for(int i=0;i<a;i++){
                if(answer[a] == answer[i]){
                    find = 1;
                    break;
                }
                
            }
            if(find == 0)
            a++;
        }
        
}