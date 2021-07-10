#include <iostream>
#include <iomanip>
using namespace std;

//Number of Processes
int n;

// Structure for Process
struct process
{
    int pid;
    float arrival_time, burst_time, burst_time_init, io, io_init, burst_time2, burst_time_init_2, waiting_time, turnaround_time;
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
            if (p[i].arrival_time > p[j].arrival_time)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void shortest_job_first(int n, float sjf[])
{
    int completed_time[10] = {0}, cpu_end[10] = {0}, t = p[0].arrival_time + p[0].burst_time, f = 1, completed = 0;
    int first = 0;
    int q = p[0].burst_time;
    int qio = -1;
    int ioloc = 0, chgloc1 = -1, chgloc2 = -1;
    cpu_end[0] = t;
    p[0].burst_time = 0;

    while (f)
    {
        first = 0;
        ioloc = -1;
        chgloc1 = -1;
        chgloc2 = -1;
        int firstchg1 = 0, firstchg2 = 0, firstchg = 0;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {

                    if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                    {
                        completed_time[i] = cpu_end[i];
                        completed++;
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {
                    if (cpu_end[i] != 0)
                    {
                        p[i].io = p[i].io - (t - cpu_end[i]);
                        cpu_end[i] = t;
                    }

                    if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                    {
                        completed_time[i] = t;
                        completed++;
                        continue;
                    }
                    else if (p[i].burst_time)
                    {
                        if (firstchg == 0)
                        {
                            q = p[i].burst_time;
                            chgloc1 = i;
                            firstchg = 1;
                        }
                        else if (p[i].burst_time < q)
                        {
                            q = p[i].burst_time;
                            chgloc1 = i;
                        }
                        ioloc = -1;
                    }
                    else if (p[i].io > 0)
                    {

                        if (p[i].io > 0)
                        {
                            if (first == 0)
                            {
                                first = 1;
                                qio = p[i].io;
                                ioloc = i;
                            }
                            else if (qio > p[i].io)
                            {
                                qio = p[i].io;
                                ioloc = i;
                            }
                        }
                        else
                            q = 1;
                    }
                    else if (p[i].burst_time2)
                    {
                        if (firstchg == 0)
                        {
                            q = p[i].burst_time2;
                            chgloc2 = i;
                            firstchg = 1;
                        }
                        else if (p[i].burst_time2 < q)
                        {
                            q = p[i].burst_time2;
                            chgloc2 = i;
                        }
                        ioloc = -1;
                    }
                }
            }
            else
                break;
        }
        if (chgloc1 != -1 || chgloc2 != -1)
        {
            if (chgloc1 != -1)
            {
                p[chgloc1].burst_time = 0;
                cpu_end[chgloc1] = t + q;
            }
            else
            {
                p[chgloc2].burst_time2 = 0;
                cpu_end[chgloc2] = t + q;
            }
        }
        else
        {
            p[ioloc].io = p[ioloc].io - qio;
            q = qio;
        }
        if (completed >= n)
            f = 0;
        t += q;
    }
    float wait_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        wait_time += completed_time[i] - p[i].arrival_time - p[i].burst_time_init - p[i].burst_time_init_2;
        turnaround_time += completed_time[i] - p[i].arrival_time;
    }
    sjf[0] = wait_time / n;
    sjf[1] = turnaround_time / n;
}

void round_robin(int n, float rr[], int quant = 1)
{
    int completed_time[10] = {0}, cpu_end[10] = {0}, t = p[0].arrival_time, f = 1, completed = 0, orig_quant = quant;
    int first = 0;
    int q;
    if (quant > p[0].burst_time)
    {
        q = p[0].burst_time;
        p[0].burst_time = 0;
    }
    else
    {
        q = quant;
        p[0].burst_time = p[0].burst_time - q;
    }
    t += q;
    int j = 0;
    int nval = 0;
    cpu_end[0] = t;
    int l = 0;
    int entry = 0;
    while (f)
    {
        first = 0;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {

                    if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                    {
                        completed_time[i] = cpu_end[i];
                        completed++;
                    }
                }
            }
        }

        if (completed >= n)
        {
            f = 0;
            break;
        }
        entry = 0;
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {
                    if (cpu_end[i] != 0 && p[i].burst_time <= 0)
                    {
                        p[i].io = p[i].io - (t - cpu_end[i]);
                        cpu_end[i] = t;
                    }

                    if (i > j)
                    {

                        entry = 1;
                        if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                        {
                            completed_time[i] = t;
                            completed++;
                            continue;
                        }

                        else if (p[i].burst_time)
                        {
                            if (quant > p[i].burst_time)
                            {
                                q = p[i].burst_time;
                                p[i].burst_time = 0;
                                quant = 0;
                            }
                            else
                            {
                                q = quant;
                                p[i].burst_time = p[i].burst_time - q;
                                quant = 0;
                            }
                            cpu_end[i] = t + q;
                            l = i;
                            break;
                        }
                        else if (p[i].io > 0)
                        {

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
                        }
                        else if (p[i].burst_time2)
                        {
                            if (quant > p[i].burst_time2)
                            {
                                q = p[i].burst_time2;
                                p[i].burst_time2 = 0;

                                quant = 0;
                            }
                            else
                            {
                                q = quant;
                                p[i].burst_time2 = p[i].burst_time2 - q;
                                quant = 0;
                            }
                            cpu_end[i] = t + q;
                            l = i;
                            break;
                        }
                    }
                    if (j > n - 1)
                    {
                        j++;
                        break;
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
            j = (l == 0 ? j + 1 : l);
        if (!entry)
        {
            j = -1;
            q = 0;
        }
        if (completed >= n)
        {
            f = 0;
            break;
        }
        if (j >= n - 1)
            j = -1;
        t += q;
        nval = 0;
    }
    float wait_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        wait_time += completed_time[i] - p[i].arrival_time - p[i].burst_time_init - p[i].burst_time_init_2;
        turnaround_time += completed_time[i] - p[i].arrival_time;
    }
    rr[0] = wait_time / n;
    rr[1] = turnaround_time / n;
}

void first_come_first_serve(int n, float fcfs[])
{
    int completed_time[10] = {0}, cpu_end[10] = {0}, t = p[0].arrival_time + p[0].burst_time, f = 1, completed = 0;
    int first = 0;
    int q = p[0].burst_time;
    int ioloc = 0;
    cpu_end[0] = t;
    p[0].burst_time = 0;

    while (f)
    {
        first = 0;
        ioloc = -1;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {

                    if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                    {
                        completed_time[i] = cpu_end[i];
                        completed++;
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= t)
            {
                if (completed_time[i] == 0)
                {
                    if (cpu_end[i] != 0)
                    {
                        p[i].io = p[i].io - (t - cpu_end[i]);
                        cpu_end[i] = t;
                    }

                    if (p[i].burst_time <= 0 && p[i].burst_time2 <= 0 && p[i].io <= 0)
                    {
                        completed_time[i] = t;
                        completed++;
                        continue;
                    }
                    else if (p[i].burst_time)
                    {
                        q = p[i].burst_time;
                        p[i].burst_time = 0;
                        cpu_end[i] = t + q;
                        ioloc = -1;
                        break;
                    }
                    else if (p[i].io > 0)
                    {

                        if (p[i].io > 0)
                        {
                            if (first == 0)
                            {
                                first = 1;
                                q = p[i].io;
                                ioloc = i;
                            }
                            else if (q > p[i].io)
                            {
                                q = p[i].io;
                                ioloc = i;
                            }
                        }
                        else
                            q = 1;
                    }
                    else if (p[i].burst_time2)
                    {
                        q = p[i].burst_time2;
                        p[i].burst_time2 = 0;
                        cpu_end[i] = t + q;
                        ioloc = -1;
                        break;
                    }
                }
            }
            else
                break;
        }
        if (ioloc != -1)
        {
            p[ioloc].io = p[ioloc].io - q;
        }
        if (completed >= n)
            f = 0;
        t += q;
    }
    float wait_time = 0, turnaround_time = 0;
    for (int i = 0; i < n; i++)
    {
        wait_time += completed_time[i] - p[i].arrival_time - p[i].burst_time_init - p[i].burst_time_init_2;
        turnaround_time += completed_time[i] - p[i].arrival_time;
    }
    fcfs[0] = (wait_time) / n;
    fcfs[1] = (turnaround_time) / n;
}

void value_initializer()
{
    for (int i = 0; i < n; i++)
    {
        p[i].burst_time = p[i].burst_time_init;
        p[i].burst_time2 = p[i].burst_time_init_2;
        p[i].io = p[i].io_init;
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
        cin >> p[i].arrival_time;
        cout << "Enter cpu Burst time 1 for P" << p[i].pid << ": ";
        cin >> p[i].burst_time;
        cout << "Enter i/o Burst time for P" << p[i].pid << ": ";
        cin >> p[i].io;
        cout << "Enter cpu Burst time 2 for P" << p[i].pid << ": ";

        cin >> p[i].burst_time2;
        p[i].burst_time_init = p[i].burst_time;
        p[i].burst_time_init_2 = p[i].burst_time2;
        p[i].io_init = p[i].io;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completed[0] = 0;
        p[i].completed[1] = 0;
        p[i].completed[2] = 0;
    }
    sortByArrival();
    float fcfs[2], rr[2], sjf[2];

    first_come_first_serve(n, fcfs);

    value_initializer(); //initializes values of processes back to original for subsequent steps

    round_robin(n, rr);

    value_initializer();

    shortest_job_first(n, sjf);

    value_initializer();

    t = p[0].arrival_time;
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
            if (p[i].arrival_time <= t && p[i].completed[0] != 1)
            {

                // Calculating Response Ratio
                temp = (p[i].burst_time + (p[i].waiting_time)) / p[i].burst_time;
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
            else if ((p[i].ct[0] + p[i].io_init) <= t && p[i].completed[2] != 1)
            {

                // Calculating Response Ratio
                temp = (p[i].burst_time2 + (p[i].waiting_time)) / p[i].burst_time2;
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
            if (p[loc].burst_time <= q && p[loc].burst_time > 0)
            {
                p[loc].completed[0] = 1;
                t += p[loc].burst_time;
                p[loc].ct[0] = t;
                if (p[loc].io_init == 0)
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
            if (p[loc].burst_time2 <= q && p[loc].burst_time2 > 0)
            {
                p[loc].completed[2] = 1;
                t += p[loc].burst_time2;
                p[loc].ct[1] = t;
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
            if (p[j].arrival_time <= t && p[j].completed[0] != 1 && j != loc)
            {
                p[j].waiting_time = t + p[j].burst_time - p[j].burst_time_init - p[j].arrival_time;
            }
            else if (p[j].ct[0] <= t && p[j].completed[1] != 1 && j != loc)
            {
                p[j].waiting_time = t - p[j].arrival_time - p[j].burst_time_init;
                p[j].io = p[j].io - (t - p[j].ct[0]);
                if (p[j].io <= 0)
                {
                    p[j].completed[1] = 1;
                }
            }
            else if ((p[j].ct[0] + p[j].io_init) < t && p[j].completed[2] != 1 && j != loc)
            {
                p[j].waiting_time = t - p[j].arrival_time - p[j].burst_time_init - p[j].burst_time_init_2 + p[j].burst_time2;
            }
        }
        if (flag == 0)
        {
            p[loc].burst_time -= q;
            if (p[loc].burst_time < 0)
            {
                p[loc].burst_time = 0;
            }
        }
        else if (flag == 2)
        {
            p[loc].burst_time2 -= q;
            if (p[loc].burst_time2 <= 0)
            {
                p[loc].burst_time2 = 0;

                comp++;
            }
        }
    }

    float average_turnaround_time = 0.0, average_wait_time = 0.0;
    cout << setw(25) << "Process ID" << setw(25) << "Arrival Time" << setw(25) << "Burst Time" << setw(25) << "i/o Burst Time" << setw(25) << "Burst Time" << setw(25) << "Waiting Time" << setw(25) << "Turn-around Time" << endl;
    for (int i = 0; i < n; i++)
    {
        p[i].turnaround_time = p[i].ct[1] - p[i].arrival_time;
        p[i].waiting_time = p[i].ct[1] - p[i].burst_time_init - p[i].burst_time_init_2 - p[i].arrival_time;
        cout << setw(25) << p[i].pid << setw(25) << p[i].arrival_time << setw(25) << p[i].burst_time_init << setw(25) << p[i].io_init << setw(25) << p[i].burst_time_init_2 << setw(25) << p[i].waiting_time << setw(25) << p[i].turnaround_time << endl;
        average_turnaround_time += p[i].turnaround_time;
        average_wait_time += p[i].waiting_time;
    }
    average_turnaround_time = average_turnaround_time / n;
    average_wait_time /= n;

    cout << "Average Turn-around Time: " << average_turnaround_time << "\n"
         << "Average Waiting Time: " << average_wait_time << "\n"
         << "Throughput: " << n / (t - p[0].arrival_time) << "\n";
    cout << "Comparision Table" << endl;
    cout << setw(25) << " " << setw(25) << "FCFS" << setw(25) << "SJF" << setw(25) << "Round Robin" << setw(25) << "NewAlgo" << endl;
    cout << setw(25) << "Average Waiting Time " << setw(25) << fcfs[0] << setw(25) << sjf[0] << setw(25) << rr[0] << setw(25) << average_wait_time << endl;
    cout << setw(25) << "Average Turnaround Time " << setw(25) << fcfs[1] << setw(25) << sjf[1] << setw(25) << rr[1] << setw(25) << average_turnaround_time << endl;
    return 0;
}
