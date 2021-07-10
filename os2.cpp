#include <iostream>
#include <iomanip>
using namespace std;

//Number of Processes
int n;

// Structure for Process
struct process
{
    int pid;
    float at, bt, btin, io, ioin, bt2, btin2, wt, tt;
    float ct[2];
    int completed[3];
} p[10];

// Sorting Processes by Arrival Time using Selection Sort
void sortByArrival()
{
    struct process temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (p[i].at > p[j].at)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void round_robin(int n, float rr[], int quant = 3)
{
    int comp_time[10] = {0}, cpu_end[10] = {0}, t = p[0].at, f = 1, completed = 0, orig_quant = quant;
    int first = 0;
    int q;
    if (quant > p[0].bt)
    {
        q = p[0].bt;
        p[0].bt = 0;
        quant -= q;
    }
    else
    {
        q = quant;
        p[0].bt = p[0].bt - q;
        quant = 0;
    }
    t += q;
    int j = 0;
    int nval = 0;
    cpu_end[0] = t;
    int l = 0;
    while (f)
    {
        first = 0;
        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= t)
            {
                if (comp_time[i] == 0)
                {
                    if (cpu_end[i] != 0)
                    {
                        p[i].io = p[i].io - (t - cpu_end[i]);
                        cpu_end[i] = t;
                    }

                    if (i >= j)
                    {

                        if (p[i].bt <= 0 && p[i].bt2 <= 0 && p[i].io <= 0)
                        {
                            comp_time[i] = t;
                            completed++;
                            continue;
                        }

                        else if (p[i].bt)
                        {
                            if (quant > p[i].bt)
                            {
                                q = p[i].bt;
                                p[i].bt = 0;
                                quant -= q;
                            }
                            else
                            {
                                q = quant;
                                p[i].bt = p[i].bt - q;
                                quant = 0;
                            }
                            cpu_end[i] = t + q;
                            l = i;
                            break;
                        }
                        else if (p[i].io > 0)
                        {

                            // q = 1;
                            if (p[i].io > 0)
                            {
                                if (first == 0)
                                {
                                    first = 1;
                                    q = p[i].io;
                                }
                                else if (q > p[i].io)
                                    q = p[i].io;
                            }
                            else
                                q = 1;
                            l = i;
                            // p[i].io = p[i].io - q;
                        }
                        else if (p[i].bt2)
                        {
                            if (quant > p[i].bt2)
                            {
                                q = p[i].bt2;
                                p[i].bt2 = 0;
                                quant -= q;
                            }
                            else
                            {
                                q = quant;
                                p[i].bt2 = p[i].bt2 - q;
                                quant = 0;
                            }
                            cpu_end[i] = t + q;
                            l = i;
                            break;
                        }
                    }
                }
            }
            else
            {
                q = 1;
                nval = 1;
            }
        }
        if (quant <= 0)
            quant = orig_quant;
        if (!nval)
            j += l;
        if (completed >= n)
            f = 0;
        if (j > n - 1)
            j = 0;
        t += q;
        nval = 0;
    }
    float wait_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        wait_time += comp_time[i] - p[i].at - p[i].btin - p[i].btin2;
        turnaround_time += comp_time[i] - p[i].at;
        // cout << comp_time[i];
    }
    rr[0] = wait_time / n;
    rr[1] = turnaround_time / n;
}

void first_come_first_serve(int n, float fcfs[])
{
    int comp_time[10] = {0}, cpu_end[10] = {0}, t = p[0].at + p[0].bt, f = 1, completed = 0;
    int first = 0;
    int q = p[0].bt;
    int minq = 0;
    cpu_end[0] = t;
    p[0].bt = 0;
    while (f)
    {
        first = 0;
        for (int i = 0; i < n; i++)
        {
            if (p[i].at <= t)
            {
                if (comp_time[i] == 0)
                {
                    if (cpu_end[i] != 0)
                    {
                        p[i].io = p[i].io - (t - cpu_end[i]);
                        cpu_end[i] = t;
                    }

                    if (p[i].bt <= 0 && p[i].bt2 <= 0 && p[i].io <= 0)
                    {
                        comp_time[i] = t;
                        completed++;
                        continue;
                    }
                    else if (p[i].bt)
                    {
                        q = p[i].bt;
                        p[i].bt = 0;
                        cpu_end[i] = t + q;
                        break;
                    }
                    else if (p[i].io > 0)
                    {

                        // q = 1;
                        if (p[i].io > 0)
                        {
                            if (first == 0)
                            {
                                first = 1;
                                q = p[i].io;
                            }
                            else if (q > p[i].io)
                                q = p[i].io;
                        }
                        else
                            q = 1;
                        // p[i].io = p[i].io - q;
                    }
                    else if (p[i].bt2)
                    {
                        q = p[i].bt2;
                        p[i].bt2 = 0;
                        cpu_end[i] = t + q;
                        break;
                    }
                }
            }
            else
                break;
        }
        if (completed >= n)
            f = 0;
        t += q;
    }
    float wait_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        wait_time += comp_time[i] - p[i].at - p[i].btin - p[i].btin2;
        turnaround_time += comp_time[i] - p[i].at;
        // cout << comp_time[i];
    }
    fcfs[0] = wait_time / n;
    fcfs[1] = turnaround_time / n;
}

void value_initializer()
{
    for (int i = 0; i < n; i++)
    {
        p[i].bt = p[i].btin;
        p[i].bt2 = p[i].btin2;
        p[i].io = p[i].ioin;
    }
}

int main()
{
    cout << "Enter number of processes: ";
    cin >> n;
    float t = 0.0;
    float q = 1.0;
    for (int i = 0; i < n; i++)
    {
        cout << "Enter PID: ";
        cin >> p[i].pid;
        cout << "Enter Arrival time for P" << p[i].pid << ": ";
        cin >> p[i].at;
        cout << "Enter cpu Burst time 1 for P" << i << ": ";
        cin >> p[i].bt;
        cout << "Enter i/o Burst time for P" << i << ": ";
        cin >> p[i].io;
        cout << "Enter cpu Burst time 2 for P" << i << ": ";

        cin >> p[i].bt2;
        p[i].btin = p[i].bt;
        p[i].btin2 = p[i].bt2;
        p[i].ioin = p[i].io;
        p[i].wt = 0;
        p[i].tt = 0;
        p[i].completed[0] = 0;
        p[i].completed[1] = 0;
        p[i].completed[2] = 0;
    }
    sortByArrival();
    float fcfs[2];
    float rr[2];

    first_come_first_serve(n, fcfs);
    cout << "FCFS Result: " << fcfs[0] << " " << fcfs[1] << "\n";
    value_initializer();

    round_robin(n, rr);
    cout << "Round Robin Result: " << rr[0] << " " << rr[1] << "\n";

    value_initializer();

    t = p[0].at;
    int comp = 0;

    while (comp < n)
    {
        int loc = -1;

        float hrr = -9999;
        float temp;
        int flag = -1;
        for (int i = 0; i < n; i++)
        {
            // Checking if process has arrived and is Incomplete
            if (p[i].at <= t && p[i].completed[0] != 1)
            {

                // Calculating Response Ratio
                temp = (p[i].bt + (p[i].wt)) / p[i].bt;
                // Checking for Highest Response Ratio
                if (hrr == temp)
                {
                    if (loc > i)
                    {
                        loc = i;
                        flag = 0;
                    }
                }
                else if (hrr < temp)
                {

                    // Storing Response Ratio
                    hrr = temp;

                    // Storing Location
                    loc = i;
                    flag = 0;
                }
            }
            else if ((p[i].ct[0] + p[i].ioin) <= t && p[i].completed[2] != 1)
            {

                // Calculating Response Ratio
                temp = (p[i].bt2 + (p[i].wt)) / p[i].bt2;
                // Checking for Highest Response Ratio
                if (hrr == temp)
                {
                    if (loc > i)
                    {
                        loc = i;
                        flag = 2;
                    }
                }
                else if (hrr < temp)
                {

                    // Storing Response Ratio
                    hrr = temp;

                    // Storing Location
                    loc = i;
                    flag = 2;
                }
            }
        }
        // Checking if Remaining Burst Time of the Process(about to be executed) is less than/equal to quantum time
        if (flag == 0)
        {
            if (p[loc].bt <= q && p[loc].bt > 0)
            {
                p[loc].completed[0] = 1;
                t += p[loc].bt;
                p[loc].ct[0] = t;
                if (p[loc].ioin == 0)
                {
                    p[loc].ct[1] = t;
                    p[loc].completed[2] = 1;
                    comp++;
                }
            }

            else
            {
                t += q;
            }
        }
        else if (flag == 2)
        {
            if (p[loc].bt2 <= q && p[loc].bt2 > 0)
            {
                p[loc].completed[2] = 1;
                t += p[loc].bt2;
                p[loc].ct[1] = t;
                //
            }

            else
            {
                t += q;
            }
        }
        else
        {
            t += q;
        }
        for (int j = 0; j < n; j++)
        {
            if (p[j].at <= t && p[j].completed[0] != 1 && j != loc)
            {
                p[j].wt = t + p[j].bt - p[j].btin - p[j].at;
            }
            else if (p[j].ct[0] <= t && p[j].completed[1] != 1 && j != loc)
            {
                p[j].wt = t - p[j].at - p[j].btin;
                p[j].io = p[j].io - (t - p[j].ct[0]);
                if (p[j].io <= 0)
                {
                    p[j].completed[1] = 1;
                }
            }
            else if ((p[j].ct[0] + p[j].ioin) < t && p[j].completed[2] != 1 && j != loc)
            {
                p[j].wt = t - p[j].at - p[j].btin - p[j].btin2 + p[j].bt2;
            }
        }
        if (flag == 0)
        {
            p[loc].bt -= q;
            if (p[loc].bt < 0)
            {
                p[loc].bt = 0;
            }
        }
        else if (flag == 2)
        {
            p[loc].bt2 -= q;
            if (p[loc].bt2 <= 0)
            {
                p[loc].bt2 = 0;

                comp++;
            }
        }
    }

    float att = 0.0, awt = 0.0;
    cout << setw(18) << "Process ID" << setw(18) << "Arrival Time" << setw(18) << "Burst Time" << setw(18) << "i/o Burst Time" << setw(18) << "Burst Time" << setw(18) << "Waiting Time" << setw(18) << "Turn-around Time" << endl;
    for (int i = 0; i < n; i++)
    {
        p[i].tt = p[i].ct[1] - p[i].at;
        p[i].wt = p[i].ct[1] - p[i].btin - p[i].btin2 - p[i].at;
        cout << setw(18) << p[i].pid << setw(18) << p[i].at << setw(18) << p[i].btin << setw(18) << p[i].ioin << setw(18) << p[i].btin2 << setw(18) << p[i].wt << setw(18) << p[i].tt << endl;
        att += p[i].tt;
        awt += p[i].wt;
    }
    att = att / n;
    awt /= n;

    cout << "Average Turn-around Time: " << att << "\n"
         << "Average Waiting Time: " << awt << "\n"
         << "Throughput: " << (t - p[0].at) / n;
    return 0;
}
