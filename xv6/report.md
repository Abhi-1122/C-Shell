# Scheduler Performance Report

This report compares different CPU scheduling policies using the `schedulertest` command. All processes were executed on a single CPU to ensure fairness in the comparison. Below is a summary of implementation details, changes made, and performance evaluation across **First Come First Serve (FCFS)**, **Round Robin (RR)** (default), **Completely Fair Scheduler (CFS)**, and **Multi-Level Feedback Queue (MLFQ)**.

---

## Implementation Notes

### First Come First Serve (FCFS)
- Implemented a simple queue where processes are scheduled strictly in their arrival order.
- Each process runs until completion before the next process is executed.
- Change: Modified the scheduler to select the next runnable process based only lowest creation time (based on ticks).

### Round Robin (RR)
- Default scheduler where each process gets an equal, fixed time slice in cyclic order.
- Provides fairness but may lead to higher waiting times if time slices are small.
- Change: Used the default implementation with a time quantum of 1 unit.

### Completely Fair Scheduler (CFS)
- Distributes CPU time proportionally among all runnable processes to approximate fairness.
- Maintains virtual runtime accounting to ensure no process starves.
- Change: Implemented fair sharing of CPU, assigning time proportional to remaining demand and tracking wait time.

### Multi-Level Feedback Queue (MLFQ)
- Uses multiple priority queues with feedback to adjust a process’s priority dynamically.
- Interactive/small jobs move up queues, CPU-bound jobs are lowered gradually.
- Change: Implemented tiered queues with demotion/promotion policy for balancing responsiveness and throughput.

---

## Performance Comparison

All five processes were executed under different schedulers. The results below summarize **average waiting times** and **average running times**.

| Scheduler | Average Waiting Time | Average Running Time |
|-----------|-----------------------|-----------------------|
| FCFS      | 3                     | 1                     |
| Round Robin (default) | 5         | 1                     |
| CFS       | 4                     | 1                     |
| MLFQ      | 5                     | 1                     |

---

## Observations
- **FCFS** achieved the lowest waiting time average (3), as each process is executed without interruption in arrival order.
- **Round Robin** maintained fairness but produced the highest waiting times (5), since each process had to wait its turn in cycles.
- **CFS** provided a balance between fairness and efficiency, with moderate waiting times (4).
- **MLFQ** showed similar waiting times to RR (5) but benefits more in dynamic and diverse workloads where priority adjustment plays a larger role.

Overall, FCFS minimized waiting time for this workload, while CFS represented a good compromise between fairness and responsiveness across processes.

