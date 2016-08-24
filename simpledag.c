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
  SD_init(&argc, argv);
  /*dag = SD_daxload(argv[1]);

  xbt_dynar_foreach(dag, cursor, task) {
    SD_task_watch(task, SD_DONE);
  }
  */
  //SD_create_environment(argv[2]);

  SD_create_environment(argv[1]);

  nworkstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();
  
  task_A = SD_task_create_comp_seq("task_A", NULL, 1e9);
  transfer = SD_task_create_comm_e2e("transfer", NULL, 1e7);
  task_B = SD_task_create_comp_seq("task_B", NULL, 2e9);

  SD_task_dependency_add(NULL, NULL, task_A, transfer);
  SD_task_dependency_add(NULL, NULL, transfer, task_B);

  SD_task_schedulel(task_A, 1, workstations[0]);
  SD_task_schedulel(task_B, 1, workstations[0]);

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
