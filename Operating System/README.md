- Dining_philosophers.cpp는 Midterm 문제 중 하나로,
  skeleton code가 주어졌을 때 코드 작동 원리를 이해하고 To do이하 란에 원하는 결과가 출력되도록 코드를 작성하는 방식
  1. The program using pthread_mutex_t and pthread_cond_t.

- DropMultBalls.cpp는 과제 중 하나로,
  skeleton code가 주어졌을 때 코드 작동 원리를 이해하고 To do이하 란에 원하는 결과가 출력되도록 코드를 작성하는 방식
  1. Pass the top and bottom coordinates of the critical section to the threads through the ThreadParam structure
  2. Declare a mutex or semaphore as a global variable
  3. Initialize the mutex or semaphore
  4. When a ball enters the critical region, wait the mutex or semaphore (entry section)
  4. When a ball leaves the critical region, signal the mutex or semaphore (exit section)
  5. If the thread breaks the loop while the ball is in the critical region, signal the mutex or semaphore to release other threads.
  6. After the threads finishes, destroy the mutex or semaphore. 
