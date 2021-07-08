#include <iostream>
#include <iomanip>
using namespace std; 

//Number of Processes     
int n; 


// Structure for Process
struct process { 
    int pid;
    float at, bt, btin,io,ioin,bt2,btin2, wt, tt,ct; 
    int completed;    
} p[10]; 
  
// Sorting Processes by Arrival Time using Selection Sort 
void sortByArrival() 
{ 
    struct process temp; 
    for (int i = 0; i < n - 1; i++) { 
        for (int j = i + 1; j < n; j++) {         
            if (p[i].at > p[j].at) { 
                temp = p[i]; 
                p[i] = p[j]; 
                p[j] = temp; 
            } 
        } 
    } 
}
int main(){
    cout<<"Enter number of processes: ";
    cin>>n;
    float t=0.0;
    float q=1.0;
    for(int i=0;i<n;i++){
        cout<<"Enter PID: ";
        cin>>p[i].pid;
        cout<<"Enter Arrival time for P"<<p[i].pid<<": ";
        cin>>p[i].at;
        cout<<"Enter cpu Burst time 1 for P"<<i<<": ";
        cin>>p[i].bt;
        cout<<"Enter i/o Burst time for P"<<i<<": ";
        cin>>p[i].io;
        cout<<"Enter cpu Burst time 2 for P"<<i<<": ";
        cin>>p[i].bt2;
        p[i].btin=p[i].bt;
        p[i].btin2=p[i].bt2;
        p[i].ioin=p[i].io;
        p[i].wt=0;
        p[i].tt=0;
        p[i].completed=0;
        
    }
    sortByArrival();
    t=p[0].at;
    int comp=0;
    while(comp<n){
        int loc=-1;
        
        float  hrr=-9999; 
        float temp;
        
        for (int i = 0; i < n; i++) { 
            // Checking if process has arrived and is Incomplete 
            if (p[i].at <= t && p[i].completed != 1) { 
                
                // Calculating Response Ratio 
                temp = (p[i].bt + (p[i].wt)) / p[i].bt; 
                // Checking for Highest Response Ratio 
                if (hrr==temp){
                    if(loc>i){
                        loc=i;
                    }
                }
                else if (hrr < temp) { 
    
                    // Storing Response Ratio 
                    hrr = temp; 
    
                    // Storing Location 
                    loc = i; 
                } 
            }
            
        }
        // Checking if Remaining Burst Time of the Process(about to be executed) is less than/equal to quantum time
        if(p[loc].bt<=q&&p[loc].bt!=0){
            p[loc].completed=1;
            t+=p[loc].bt;
            p[loc].ct=t;
            comp++;
            
        }
        
        else{
            t+=q;
            
        }
        
        for(int j=0;j<10;j++){
                if(p[j].at<t&&p[j].completed!=1&&j!=loc){
                    p[j].wt=t+p[j].bt-p[j].btin-p[j].at;
                }
        }
        p[loc].bt-=q;
        if(p[loc].bt<0){
            p[loc].bt=0;
        }
    }
    
    float att=0.0,awt=0.0;
    cout << setw(18) << "Process ID" << setw(18) << "Arrival Time"<< setw(18) << "Burst Time"<<setw(18) << "i/o Burst Time"<<setw(18) << "Burst Time"<< setw(18) << "Waiting Time"<< setw(18) << "Turn-around Time"<<endl;
    for(int i=0;i<n;i++){        
        p[i].tt=p[i].ct-p[i].at;
        cout << setw(18) << p[i].pid << setw(18) << p[i].at<< setw(18) << p[i].btin<<setw(18) << p[i].ioin<<setw(18) << p[i].btin2<< setw(18) << p[i].wt<< setw(18) << p[i].tt<<endl; 
        att+=p[i].tt;
        awt+=p[i].wt;
    }
    att=att/n;
    awt/=n;

    cout<<"Average Turn-around Time: "<<att<<"\n"<<"Average Waiting Time: "<<awt;
    return 0;
}