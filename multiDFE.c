#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "SimDAG simple");

int main(int argc, char **argv) {

  unsigned int cursor;
  const SD_workstation_t *workstations = NULL;
  int nworkstations = 0;
  xbt_dynar_t dag, changed_tasks;
  SD_task_t task, task_A, transfer, task_B;
  double task_size = atof(argv[3]);
  double number_of_cores = atoi(argv[2]);
  double a, b, c;
  a = 186*number_of_cores;
  b = 40.35*(1-exp(-0.3764*number_of_cores)) - 12.31;
  c = -0.007838*number_of_cores + 0.7111;
    
  double throughput = 1000*a*exp(-b*exp(-c*log(task_size)));
  
  SD_init(&argc, argv);

  SD_create_environment(argv[1]);

  nworkstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();
  
  task_A = SD_task_create_comp_seq("task_A", NULL, task_size/throughput);

  SD_task_schedulel(task_A, 1, workstations[0]);
  
  changed_tasks = SD_simulate(-1.0);
  unsigned int count;
  xbt_dynar_foreach(changed_tasks, count, task) {
    XBT_INFO("%s started at %f and finished at %f", SD_task_get_name(task),
	     SD_task_get_start_time(task), SD_task_get_finish_time(task));
    SD_task_destroy(task);
  }

  xbt_dynar_free_container(&changed_tasks);
  SD_exit();
  return 0;
}
