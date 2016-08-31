#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "SimDAG simple");


int main(int argc, char **argv) {
  unsigned int count;
  int total_workstations = 0;
  const SD_workstation_t *workstations = NULL;
  SD_task_t start_task, task, end_task;
  SD_task_t starttransfer, endtransfer;
  xbt_dynar_t changed_tasks;
  double task_size = atof(argv[2]);

  SD_init(&argc, argv);
  SD_create_environment(argv[1]);

  total_workstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();

  double a, b, c;
  double maximum_throughput = 0;
  char *compute;
  int n = 0;
  int which = -1;
  
  for(int i=1; i < 25; i++) {
    double a, b, c, throughput;
    a = 186*i;
    b = 40.35*(1-exp(-0.3764*i)) - 12.31;
    c = -0.007838*i + 0.7111;
    
    throughput = 1000*a*exp(-b*exp(-c*log(task_size)));
    if(throughput > maximum_throughput) {
      maximum_throughput = throughput;
      n = i;
      if(which != 0)
	which = 0;
    }
  }
  for(int i=1; i < 5; i++) {
    double a, b, c, throughput;
    a = 6904*i + 8580;
    b = 9320*exp(-1.035*i) - 12.31;
    c = -0.08079*i + 0.7581;
    
    throughput = 1000*a*exp(-b*exp(-c*log(task_size)));
    if(throughput > maximum_throughput) {
      maximum_throughput = throughput;
      n = i;
      if(which != 1)
	which = 1;
    }
  }  

  if(which == 0) {
    compute = "CPU";
  }
  else if(which == 1) {
    compute = "DFE";
  }
  
  start_task = SD_task_create_comp_seq("starttask", NULL, 0);
  starttransfer = SD_task_create_comm_e2e("starttransfer", NULL, 1e10);
  task = SD_task_create_comp_seq("task", NULL, task_size/maximum_throughput);
  endtransfer = SD_task_create_comm_e2e("endtransfer", NULL, 1e10);
  end_task = SD_task_create_comp_seq("endtask", NULL, 0);
  
  SD_task_dependency_add(NULL, NULL, start_task, starttransfer);
  SD_task_dependency_add(NULL, NULL, starttransfer, task);
  SD_task_dependency_add(NULL, NULL, task, endtransfer);
  SD_task_dependency_add(NULL, NULL, endtransfer, end_task);
  
  SD_task_schedulel(start_task, 1, SD_workstation_get_by_name("mainCPU"));
  SD_task_schedulel(task, 1, SD_workstation_get_by_name(compute));
  SD_task_schedulel(end_task, 1, SD_workstation_get_by_name("mainCPU"));

  printf("task was done on %s with %d number of cores\n\n", compute, n);
  
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
