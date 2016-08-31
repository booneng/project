#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "SimDAG simple");

int same(char *a, char *b) {
  if(*a == '\0' && *b == '\0')
    return 1;
  if(*a == *b) {
    a++;
    b++;
    return same(a, b);
  }
  else {
    return 0;
  }
}
int main(int argc, char **argv) {
  unsigned int count;
  int total_workstations = 0;
  const SD_workstation_t *workstations = NULL;
  SD_task_t start_task, task, end_task;
  SD_task_t starttransfer, endtransfer;
  xbt_dynar_t changed_tasks;
  char *compute = argv[2];
  double number_of_cores = atoi(argv[3]);
  double task_size = atof(argv[4]);

    
  SD_init(&argc, argv);
  SD_create_environment(argv[1]);

  total_workstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();

  double a, b, c;
  double throughput;
  
  if(same(compute, "CPU")) {
      a = 186*number_of_cores;
      b = 40.35*(1-exp(-0.3764*number_of_cores)) - 12.31;
      c = -0.007838*number_of_cores + 0.7111;
    
      throughput = 1000*a*exp(-b*exp(-c*log(task_size)));
  }
  else if(same(compute, "DFE")) {
      a = 6904*number_of_cores + 8580;
      b = 9320*exp(-1.035*number_of_cores) - 12.31;
      c = -0.08079*number_of_cores + 0.7581;
      throughput = 1000*a*exp(-b*exp(-c*log(task_size)));
  }
  
  start_task = SD_task_create_comp_seq("starttask", NULL, 0);
  starttransfer = SD_task_create_comm_e2e("starttransfer", NULL, 1e10);
  task = SD_task_create_comp_seq("task", NULL, task_size/throughput);
  endtransfer = SD_task_create_comm_e2e("endtransfer", NULL, 1e10);
  end_task = SD_task_create_comp_seq("endtask", NULL, 0);
  
  SD_task_dependency_add(NULL, NULL, start_task, starttransfer);
  SD_task_dependency_add(NULL, NULL, starttransfer, task);
  SD_task_dependency_add(NULL, NULL, task, endtransfer);
  SD_task_dependency_add(NULL, NULL, endtransfer, end_task);
  
  SD_task_schedulel(start_task, 1, SD_workstation_get_by_name("mainCPU"));
  SD_task_schedulel(task, 1, SD_workstation_get_by_name(compute));
  SD_task_schedulel(end_task, 1, SD_workstation_get_by_name("mainCPU"));

  changed_tasks = SD_simulate(-1.0);
  
  xbt_dynar_foreach(changed_tasks, count, task) {
    XBT_INFO("%s started at %f and finished at %f", SD_task_get_name(task),
	     SD_task_get_start_time(task), SD_task_get_finish_time(task));
    SD_task_destroy(task);
  }
  
  xbt_dynar_free_container(&changed_tasks);
  SD_exit();
  return 0;
}
