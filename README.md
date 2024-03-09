# How to run

1. Make file is provided.
2. To compile, type 'make' in the command line.
3. To run, './main [CPU-total] [S] [file_name.txt]'.
   - Note: S is in micro seconds
4. To clean, 'make clean'.

# Simulation Result

| S = 200 us          | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 86792   | 87904    | 84788    |
| `Type 1 Turnaround` | 290841  | 294099   | 284496   |
| `Type 2 Turnaround` | 1679096 | 1666340  | 1541372  |
| `Type 3 Turnaround` | 2453146 | 2222560  | 1866243  |
| `Type 0 Response`   | 86677   | 87788    | 84673    |
| `Type 1 Response`   | 290726  | 293986   | 284380   |
| `Type 2 Response`   | 1678980 | 1666222  | 1541257  |
| `Type 3 Response`   | 2453041 | 2222455  | 1866138  |

| S = 800 us          | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 87207   | 84121    | 84362    |
| `Type 1 Turnaround` | 299394  | 290363   | 274742   |
| `Type 2 Turnaround` | 1684486 | 1579861  | 1542472  |
| `Type 3 Turnaround` | 1857583 | 2014410  | 1779940  |
| `Type 0 Response`   | 87091   | 84005    | 84246    |
| `Type 1 Response`   | 299275  | 290249   | 274626   |
| `Type 2 Response`   | 1684366 | 1579746  | 1542356  |
| `Type 3 Response`   | 1857469 | 2014298  | 1779832  |

| S = 1600 us         | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 87923   | 85745    | 85141    |
| `Type 1 Turnaround` | 293527  | 301037   | 283166   |
| `Type 2 Turnaround` | 1606579 | 1623551  | 1577502  |
| `Type 3 Turnaround` | 1899317 | 2241522  | 1956697  |
| `Type 0 Response`   | 87806   | 85629    | 85025    |
| `Type 1 Response`   | 293411  | 300922   | 283052   |
| `Type 2 Response`   | 1606464 | 1623437  | 1577386  |
| `Type 3 Response`   | 1899212 | 2241420  | 1956591  |

| S = 3200 us         | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 83156   | 83527    | 84868    |
| `Type 1 Turnaround` | 275941  | 285158   | 287586   |
| `Type 2 Turnaround` | 1585980 | 1564771  | 1565997  |
| `Type 3 Turnaround` | 2077605 | 2048111  | 1714825  |
| `Type 0 Response`   | 83041   | 83414    | 84752    |
| `Type 1 Response`   | 275826  | 285043   | 287469   |
| `Type 2 Response`   | 1585865 | 1564656  | 1565881  |
| `Type 3 Response`   | 2077507 | 2048001  | 1714718  |

# Analysis Questions

1. Q: How does the value of S affect turnaround time and response time? Is the difference in turnaround time and response time what you expected to see as S and the number of CPUs change? Why or why not?

   Ans: According to the experiment that has been done, we can see that when S is too small (S = time allotment = 200 microsec),this will force the tasks to be pushed to the top even though it hasn't fully reached the bottom queue. Since moving tasks take a lot of time and blocks the CPU worker to get any new work for a moment, this will cause a lot of waiting and context switching, thus, will cause the average turnaround time and response time to increase for most tasks.

   On the other hand, if we make S to be too high (S = 3200 microsec), jobs such as long and I/O tasks (since the length task for I/O is similar to long tasks), will be starved for some amount of time, resulting in a higher turnaround time. The response time will be better compared to S = 200, because when the job initially comes the top most queue won't be over populated with a lot of tasks.

   Finaly, the optimal S for this experiment is (S = 800 or 1600), both of them have a better turnaround and response time compared to S = 200 and S = 3200. Because the value of S is not set too high or too low.

   Yes, the difference in turnaround time and response time is what I expected to see, as S is set to the right amount, it will have a better result. Additionally, as the number of CPUs are higher, it will make turnaround and response time to be faster because there are more workers to do the tasks.

2. Q: How does adjusting the S value in the system affect the turnaround time or response time for long-running and I/O tasks specifically? Does it appear to be highly correlated?

   Ans:

   - As we can see from the result of S = 200, S is set too low. I/O jobs (type 3) doesn't receive the proper share of the CPU.
   - On the other hand, if S = 3200, S is set too high, long running jobs will starve (long running jobs in the assignment is type 2 and 3). As we can see, type 2 and 3 have a higher average of turnaround and response in S = 3200 compared to S = 800 and 1600.
   - Yes, it appears that they are highly correlated.
