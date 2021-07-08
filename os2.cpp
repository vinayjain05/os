#include <iostream>
#include <iomanip>
using namespace std; 

//Number of Processes     
int n; 


// Structure for Process
struct process { 
    int pid;
    float at, bt, btin,io,ioin,bt2,btin2, wt, tt;
    float ct[2]; 
    int completed[3];    
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
        p[i].completed[0]=0;
        p[i].completed[1]=0;
        p[i].completed[2]=0;
        
    }
    sortByArrival();
    t=p[0].at;
    int comp=0;
    
    while(comp<n){
        int loc=-1;
        
        float  hrr=-9999; 
        float temp;
        int flag=-1;
        for (int i = 0; i < n; i++) { 
            // Checking if process has arrived and is Incomplete 
            if (p[i].at <= t && p[i].completed[0] != 1) { 
                
                // Calculating Response Ratio 
                temp = (p[i].bt + (p[i].wt)) / p[i].bt; 
                // Checking for Highest Response Ratio 
                if (hrr==temp){
                    if(loc>i){
                        loc=i;
                        flag =0;
                    }
                }
                else if (hrr < temp) { 
    
                    // Storing Response Ratio 
                    hrr = temp; 
    
                    // Storing Location 
                    loc = i; 
                    flag =0;
                }
                 
            }
            else if ((p[i].ct[0]+p[i].ioin)<=t && p[i].completed[2] != 1) { 
                
                // Calculating Response Ratio 
                temp = (p[i].bt2 + (p[i].wt)) / p[i].bt2; 
                // Checking for Highest Response Ratio 
                if (hrr==temp){
                    if(loc>i){
                        loc=i;
                        flag=2;
                    }
                }
                else if (hrr < temp) { 
    
                    // Storing Response Ratio 
                    hrr = temp; 
    
                    // Storing Location 
                    loc = i; 
                    flag=2;
                }
                 
            }
            
        }
        // Checking if Remaining Burst Time of the Process(about to be executed) is less than/equal to quantum time
        if(flag==0){
            if(p[loc].bt<=q&&p[loc].bt>0){
                p[loc].completed[0]=1;
                t+=p[loc].bt;
                p[loc].ct[0]=t;
                
            }
        
            else{
            t+=q;
            }
        }
        else if(flag==2){
            if(p[loc].bt2<=q&&p[loc].bt2>0){
                p[loc].completed[2]=1;
                t+=p[loc].bt2;
                p[loc].ct[1]=t;
                comp++;
            }
        
            else{
            t+=q;
            }
        }
        else{
            t+=q;
        }
        for(int j=0;j<n;j++){
                if(p[j].at<=t&&p[j].completed[0]!=1&&j!=loc){
                    p[j].wt=t+p[j].bt-p[j].btin-p[j].at;
                }
                else if(p[j].ct[0]<=t&&p[j].completed[1]!=1&&j!=loc){
                    p[j].wt=t-p[j].at-p[j].btin;
                    p[j].io=p[j].io-(t-p[j].ct[0]);
                    if(p[j].io<=0){
                        p[j].completed[1]=1;
                    }
                }
                else if((p[j].ct[0]+p[j].ioin)<t&&p[j].completed[2]!=1&&j!=loc){
                    p[j].wt=t-p[j].at-p[j].btin-p[j].btin2+p[j].bt2;
                }
        }
        if(flag==0){
            p[loc].bt-=q;
            if(p[loc].bt<0){
                p[loc].bt=0;
            }
        }
        else if(flag==2){
            p[loc].bt2-=q;
            if(p[loc].bt2<0){
                p[loc].bt2=0;
            }
        }   
    }
    
    float att=0.0,awt=0.0;
    cout << setw(18) << "Process ID" << setw(18) << "Arrival Time"<< setw(18) << "Burst Time"<<setw(18) << "i/o Burst Time"<<setw(18) << "Burst Time"<< setw(18) << "Waiting Time"<< setw(18) << "Turn-around Time"<<endl;
    for(int i=0;i<n;i++){        
        p[i].tt=p[i].ct[1]-p[i].at;
        p[i].wt=p[i].ct[1]-p[i].btin-p[i].btin2-p[i].at;
        cout << setw(18) << p[i].pid << setw(18) << p[i].at<< setw(18) << p[i].btin<<setw(18) << p[i].ioin<<setw(18) << p[i].btin2<< setw(18) << p[i].wt<< setw(18) << p[i].tt<<endl; 
        att+=p[i].tt;
        awt+=p[i].wt;
    }
    att=att/n;
    awt/=n;

    cout<<"Average Turn-around Time: "<<att<<"\n"<<"Average Waiting Time: "<<awt<<"\n"<<"Throughput: "<<(t-p[0].at)/n;
    return 0;
}