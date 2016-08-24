#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "SimDAG simple");

int main(int argc, char **argv) {

  unsigned int cursor;
  const SD_workstation_t *workstations = NULL;
  int nworkstations = 0;
  xbt_dynar_t dag, ready_tasks, changed_tasks;
  SD_task_t task;
  SD_init(&argc, argv);
  dag = SD_daxload(argv[1]);

  xbt_dynar_foreach(dag, cursor, task) {
    SD_task_watch(task, SD_DONE);
  }
  
  SD_create_environment(argv[2]);

  nworkstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();

  xbt_dynar_get_cpy(dag, 0, &task);
  SD_task_schedulel(task, 1, workstations[0]);

  while(!xbt_dynar_is_empty((changed_tasks = SD_simulate(-1.0)))) {
    xbt_dynar_foreach(changed_tasks, cursor, task) {
      XBT_INFO("%s started at %f and finished at %f", SD_task_get_name(task),
	       SD_task_get_start_time(task), SD_task_get_finish_time(task));
      SD_task_destroy(task);
    }

    xbt_dynar_foreach(dag, cursor, task) {
      if (SD_task_get_kind(task)==SD_TASK_COMP_SEQ && SD_task_get_state(task)==SD_SCHEDULABLE){
	printf("scheduled");
	SD_task_dump(task);
	SD_task_schedulel(task, 1, workstations[1]);
      }
    }
   
  }
  SD_exit();
  return 0;
}

					 
