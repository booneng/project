#include "simdag/simdag.h"
#include "xbt.h"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(sd_typed_tasks_test, "SimDAG simple");


int main(int argc, char **argv) {
  unsigned int count;
  int total_workstations = 0;
  const SD_workstation_t *workstations = NULL;
  xbt_dynar_t changed_tasks;
  SD_init(&argc, argv);
  SD_create_environment(argv[1]);
  total_workstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();

  double minimum_time = -1;
  int nDFE;
  int nCPU;
  double DFE_task_size = 0;
  double CPU_task_size = 0;
  double CPU_throughput;
  double DFE_throughput;
  
  printf("Enter number of CPU cores used for computation: ");
  scanf("%d", &nCPU);
  
  printf("Enter task size for CPU cores: ");
  scanf("%lf", &CPU_task_size);

  printf("Enter number of DFEs used for computation: ");
  scanf("%d", &nDFE);
    
  printf("Enter task size for DFEs: ");
  scanf("%lf", &DFE_task_size);

  printf("%.0f running on %d CPU core(s)\n", CPU_task_size, nCPU);
  printf("%.0f running on %d DFE(s)\n", DFE_task_size, nDFE);

  double a, b, c;
  a = 186*nCPU;
  b = 40.35*(1-exp(-0.3764*nCPU)) - 12.31;
  c = -0.007838*nCPU + 0.7111;
  
  CPU_throughput = 1000*a*exp(-b*exp(-c*log(CPU_task_size)));

  a = 6904*nDFE + 8580;
  b = 9320*exp(-1.035*nDFE) - 12.31;
  c = -0.08079*nDFE + 0.7581;
	
  DFE_throughput = 1000*a*exp(-b*exp(-c*log(DFE_task_size)));
  
  if(nCPU > 0  && nDFE > 0) {
    SD_task_t start_task, end_task;
    start_task = SD_task_create_comp_seq("start task", NULL, 0);
    end_task = SD_task_create_comp_seq("endtask", NULL, 0);
  
    SD_task_t cpu_task, dfe_task;
    cpu_task = SD_task_create_comp_seq("cpu task", NULL, CPU_task_size/CPU_throughput);
    dfe_task = SD_task_create_comp_seq("dfe task", NULL, DFE_task_size/DFE_throughput);

    SD_task_t transferCPU, transferDFE;
    transferCPU = SD_task_create_comm_e2e("transfer CPU", NULL, 100);
    transferDFE = SD_task_create_comm_e2e("transfer DFE", NULL, 100);
    
    SD_task_t CPUtransfer, DFEtransfer;
    CPUtransfer = SD_task_create_comm_e2e("CPU transfer", NULL, 100);
    DFEtransfer = SD_task_create_comm_e2e("DFE transfer", NULL, 100);

    SD_task_dependency_add(NULL, NULL, start_task, transferCPU);
    SD_task_dependency_add(NULL, NULL, start_task, transferDFE);
    SD_task_dependency_add(NULL, NULL, transferCPU, cpu_task);
    SD_task_dependency_add(NULL, NULL, transferDFE, dfe_task);
    SD_task_dependency_add(NULL, NULL, cpu_task, CPUtransfer);
    SD_task_dependency_add(NULL, NULL, dfe_task, DFEtransfer);
    SD_task_dependency_add(NULL, NULL, CPUtransfer, end_task);
    SD_task_dependency_add(NULL, NULL, DFEtransfer, end_task);
    
    SD_task_schedulel(start_task, 1, SD_workstation_get_by_name("mainCPU"));
    SD_task_schedulel(cpu_task, 1, SD_workstation_get_by_name("CPU"));
    SD_task_schedulel(dfe_task, 1, SD_workstation_get_by_name("DFE"));
    SD_task_schedulel(end_task, 1, SD_workstation_get_by_name("endCPU"));
    
    changed_tasks = SD_simulate(-1.0);
  }
  else {
    double tasks, throughput;
    char *compute;
    if(nDFE > 0) {
      tasks = DFE_task_size;
      throughput = DFE_throughput;
      compute = "DFE";
    }
    else if(nCPU > 0) {
      tasks = CPU_task_size;
      throughput = CPU_throughput;
      compute = "CPU";
    }
    
    SD_task_t start_task, start_transfer, task, end_transfer, end_task;
    start_task = SD_task_create_comp_seq("start task", NULL, 0);
    start_transfer = SD_task_create_comm_e2e("start transfer", NULL, 0);
    task = SD_task_create_comp_seq("task", NULL, tasks/throughput);
    end_transfer = SD_task_create_comm_e2e("end transfer", NULL, 0);
    end_task = SD_task_create_comp_seq("end task", NULL, 0);
    
    SD_task_dependency_add(NULL, NULL, start_task, start_transfer);
    SD_task_dependency_add(NULL, NULL, start_transfer, task);
    SD_task_dependency_add(NULL, NULL, task, end_transfer);
    SD_task_dependency_add(NULL, NULL, end_transfer, end_task);
    
    SD_task_schedulel(start_task, 1, SD_workstation_get_by_name("mainCPU"));
    SD_task_schedulel(task, 1, SD_workstation_get_by_name(compute));
    SD_task_schedulel(end_task, 1, SD_workstation_get_by_name("endCPU"));
  
    changed_tasks = SD_simulate(-1.0);
  }
  
  SD_task_t task;
  xbt_dynar_foreach(changed_tasks, count, task) {
    printf("%s started at %f and finished at %f\n", SD_task_get_name(task),
	     SD_task_get_start_time(task), SD_task_get_finish_time(task));
    SD_task_destroy(task);
  }
  
  xbt_dynar_free_container(&changed_tasks);
  SD_exit();
  return 0;
}
