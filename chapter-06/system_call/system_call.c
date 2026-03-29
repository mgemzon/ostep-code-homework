#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

int main() {

  struct timespec start_time;
  struct timespec end_time;
  uint64_t total_duration = 0;
  const int kIterations = 100000;

  // Get measurement overhead.
  for (int i = 0; i < kIterations; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    if (start_time.tv_sec > end_time.tv_sec) {
      fprintf(stderr, "Holy cow. We should never be here. Is timetravel "
                      "already existing when you ran this code?\n");
      return -1;
    }

    total_duration += ((end_time.tv_sec - start_time.tv_sec) * 1000000000) -
                      start_time.tv_nsec + end_time.tv_nsec;
  }

  double average_measurement_overhead_duration =
      (double)(total_duration) / (double)(kIterations);

  total_duration = 0;

  // getpid() average execution time.
  for (int i = 0; i < kIterations; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    volatile pid_t dummy =
        getpid(); // 'volatile' is used, so that compiler won't optimize this.
                  // Optimizing it might store the getpid in cache and
                  // proceeding calls won't be system calls anymore.
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    if (start_time.tv_sec > end_time.tv_sec) {
      fprintf(stderr, "Holy cow. We should never be here. Is timetravel "
                      "already existing when you ran this code?\n");
      return -1;
    }

    // Increment the total duration by the current duration. Current duration is
    // computed by first calculating the difference of the seconds between the
    // end time and start time, then converting it to nanoseconds. Then, the
    // offsets by the nanosecond portion of the end time and start time is
    // applied.
    //
    // Note: tv_nsec goes from (0 - 999,999,999). After it reaches the
    // 999,999,999 it goes back to zero and incrememnts tv_sec by one.
    total_duration += ((end_time.tv_sec - start_time.tv_sec) * 1000000000) -
                      start_time.tv_nsec + end_time.tv_nsec;
  }

  double average_duration = (double)(total_duration) / (double)(kIterations);

  printf("[average duration (including measurement overhead)] %lf ns\n",
         average_duration);

  double avg_system_exec_time =
      average_duration - average_measurement_overhead_duration;

  printf("[average measurement overhead] %lf ns\n",
         average_measurement_overhead_duration);

  printf("[average system call execution time] %lf ns\n", avg_system_exec_time);

  return 0;
}

// --Notes--
// Using gettimeofday() for measuring durations is unreliable because the system
// clock is continuously being adjusted (slewed or stepped) to match real time,
// and those adjustment distort duration of events.
//
// Example:
// Clock slowed down. Operation looks faster than reality.
// Clock sped up. Operation looks slower than reality.
// Clock jumped forward. Operation looks much slower.
// Clock jumped backward. Operation now might be negative which is absurd.
//
// Getting duration requirements like what we are doing here, measuring the cost
// of a system call, requires a clock that never changes its rate or jumps. This
// is why clock_gettime API is used and using the CLOCK_MONOTONIC clock source.