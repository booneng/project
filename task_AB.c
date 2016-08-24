#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "Task_AB");

int main(int argc, char **argv) {

  unsigned int count;
  const SD_workstation_t *workstations = NULL;
  int nworkstations = 0;
  xbt_dynar_t dag, changed_tasks;
  SD_task_t task_A, task_B, transfer, task;
  SD_task_t task_C, task_D, task_E;

  SD_init(&argc, argv);

  SD_create_environment(argv[1]);

  nworkstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();

  task_A = SD_task_create_comp_seq("task_A", NULL, 1e9);
  //transfer = SD_task_create_comm_e2e("transfer", NULL, 1e7);
  task_B = SD_task_create_comp_seq("task_B", NULL, 1e9);
  task_C = SD_task_create_comp_seq("task_C", NULL, 2e9);
  //task_E = SD_task_create_comp_seq("task_E", NULL, 3e9);
  //SD_task_dependency_add(NULL, NULL, task_A, transfer);
  //SD_task_dependency_add(NULL, NULL, transfer, task_B);

  SD_task_schedulev(task_A, 1, workstations);
  SD_task_schedulev(task_B, 1, workstations);
  // SD_task_schedulev(task_C, 1, workstations);
  //SD_task_schedulev(task_E, 1, workstations);
  changed_tasks = SD_simulate(-1.0);

  xbt_dynar_foreach(changed_tasks, count, task) {
    printf("%s started at %f and finished at %f\n", SD_task_get_name(task),
	   SD_task_get_start_time(task), SD_task_get_finish_time(task));
    SD_task_destroy(task);
  }

  xbt_dynar_free_container(&changed_tasks);
  SD_exit();
  return 0;
}
