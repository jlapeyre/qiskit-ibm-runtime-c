#include <qiskit.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <qiskit_ibm_runtime/qiskit_ibm_runtime.h>

void my_expectation_values_copy_into(ExpectationValues *evs, double *data) {
  size_t n = qkrt_expectation_values_num_evs(evs);
  for(size_t i = 0; i < n; i++)
    *data++ = qkrt_expectation_values_get_ev(evs, i);
}

/* #define CRASH_IN_RUST */
/* #define CRASH_IN_C */

int main() {
  ExpectationValues *evs;
  qkrt_fake_estimator_results(&evs);
  size_t n = qkrt_expectation_values_num_evs(evs);
  printf("n = %lu\n\n", n);

  double *data = NULL;

#ifndef CRASH_IN_RUST
  data = (double *)malloc(sizeof(double) * n);
#endif
  qkrt_expectation_values_copy_into(evs, data);

  double *data2 = NULL;
#ifdef CRASH_IN_C
  data2 = data + 1;
#else
  data2 = (double *)malloc(sizeof(double) * n);
#endif

  my_expectation_values_copy_into(evs, data2);

  for(size_t i = 0; i < n; i++) {
    printf("%f %f\n", data[i], data2[i]);
  }
  if (data)
    free(data);
  if (data2)
    free(data2);
  if (evs)
    qkrt_expectation_values_free(evs);
}
