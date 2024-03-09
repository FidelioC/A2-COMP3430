# How to run

1. Make file is provided.
2. To compile, type 'make' in the command line.
3. To run, './main [CPU-total] [S] [file_name.txt]'.
   - Note: S is in micro seconds
4. To clean, 'make clean'.

# Simulation Result

| S = 200 us          | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 309452  | 191115   | 137556   |
| `Type 1 Turnaround` | 1045729 | 708512   | 508313   |
| `Type 2 Turnaround` | 6434872 | 4584026  | 4602278  |
| `Type 3 Turnaround` | 6353370 | 4482688  | 4472316  |
| `Type 0 Response`   | 309342  | 191008   | 137370   |
| `Type 1 Response`   | 1045615 | 708398   | 508206   |
| `Type 2 Response`   | 6434768 | 4583923  | 4602172  |
| `Type 3 Response`   | 6353271 | 4482583  | 4472216  |

| S = 800 us          | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 121142  | 116051   | 102101   |
| `Type 1 Turnaround` | 539474  | 378352   | 314426   |
| `Type 2 Turnaround` | 3182795 | 2560439  | 1838193  |
| `Type 3 Turnaround` | 3088969 | 3695125  | 2112622  |
| `Type 0 Response`   | 121032  | 115941   | 101991   |
| `Type 1 Response`   | 539363  | 378243   | 314315   |
| `Type 2 Response`   | 3182684 | 2560332  | 1838077  |
| `Type 3 Response`   | 3088863 | 3695030  | 2112513  |

| S = 1600 us         | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 111346  | 84832    | 90585    |
| `Type 1 Turnaround` | 381642  | 323755   | 310235   |
| `Type 2 Turnaround` | 2878281 | 2117877  | 1750692  |
| `Type 3 Turnaround` | 3308317 | 2021991  | 2500108  |
| `Type 0 Response`   | 111234  | 84715    | 90470    |
| `Type 1 Response`   | 381528  | 323637   | 310119   |
| `Type 2 Response`   | 2878167 | 2117769  | 1750580  |
| `Type 3 Response`   | 3308208 | 2021888  | 2500005  |

| S = 3200 us         | `1 CPU` | `2 CPUs` | `8 CPUs` |
| ------------------- | ------- | -------- | -------- |
| `Type 0 Turnaround` | 247605  | 212594   | 126064   |
| `Type 1 Turnaround` | 1166546 | 675440   | 355687   |
| `Type 2 Turnaround` | 4949277 | 3261726  | 1955189  |
| `Type 3 Turnaround` | 5158945 | 3674782  | 2163934  |
| `Type 0 Response`   | 247492  | 212478   | 125956   |
| `Type 1 Response`   | 1166441 | 675323   | 355572   |
| `Type 2 Response`   | 4949170 | 3261612  | 1955076  |
| `Type 3 Response`   | 5158844 | 3674672  | 2163823  |

# Analysis Questions

1. Q: How does the value of S affect turnaround time and response time? Is the difference in turnaround time and response time what you expected to see as S and the number of CPUs change? Why or why not?

   Ans: According to the experiment that has been done, we can see that when S is too small (S = time allotment = 200 microsec),this will force the tasks to be pushed to the top even though it hasn't fully reached the bottom queue. Since moving tasks take a lot of time and blocks the CPU worker to do any work, this will cause a lot of waiting and context switching, thus, will cause the average turnaround time and response time to increase for most tasks.

   On the other hand, if we make S to be too high (S = 3200 microsec), jobs such as long and I/O tasks, will be starved for some amount of time, resulting in a higher turnaround time. The response time will be better compared to S = 200, because when the job initially comes the top most queue won't be over populated with a lot of tasks.

   Finaly, the optimal S for this experiment is (S = 800 or 1600), both of them have a better turnaround and response time compared to S = 200 and S = 3200. Because the value of S is not set too high or too low.

   Yes, the difference in turnaround time and response time is what I expected to see, as S is set to the right amount, it will have a better result. Additionally, as the number of CPUs are higher, it will make turnaround and response time to be faster because there are more workers to do the tasks.

2. Q: How does adjusting the S value in the system affect the turnaround time or response time for long-running and I/O tasks specifically? Does it appear to be highly correlated?

   Ans:

   - As we can see from the result of S = 200, S is set too low.
