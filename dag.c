#include "simdag/simdag.h"
#include "xbt.h"

typedef struct _WorkstationAttribute {
  double available_at;
  SD_task_t last_scheduled_task;
} *WorkstationAttribute;

static double SD_workstation_get_available_at(SD_workstation_t ws) {
  WorkstationAttribute attr = (WorkstationAttribute) SD_workstation_get_data(ws);
  return attr->available_at;
}

static void SD_workstation_set_available_at(SD_workstation_t ws, double time) {
  WorkstationAttribute attr = (WorkstationAttribute) SD_workstation_get_data(ws);
  attr->available_at = time;
  SD_workstation_set_data(ws, attr);
}

static void SD_workstation_allocate_attribute(SD_workstation_t ws) {
  void *data = calloc(1, sizeof(struct _WorkstationAttribute));
  SD_workstation_set_data(ws, data);
}

static void SD_workstation_free_attribute(SD_workstation_t ws) {
  free(SD_workstation_get_data(ws));
  SD_workstation_set_data(ws, NULL);
}

static SD_task_t SD_workstation_get_last_scheduled_task(SD_workstation_t ws) {
  WorkstationAttribute attr = (WorkstationAttribute) SD_workstation_get_data(ws);
  return attr->last_scheduled_task;
}

static void SD_workstation_set_last_scheduled_task(SD_workstation_t ws, SD_task_t task) {
  WorkstationAttribute attr = (WorkstationAttribute) SD_workstation_get_data(ws);
  return attr->last_scheduled_task;
}

static void SD_workstation_set_last_scheduled_task(SD_workstation_t ws, SD_task_t task) {
  WorkstationAttribute attr = (WorkstationAttribute) SD_workstation_get_data(ws);
  attr->last_scheduled_task = task;
  SD_workstation_set_data(ws, attr);
}

double finish_on_at(SD_task_t task, SD_workstation_t workstation) {
  double result, data_available = 0., last_data_available, redist_time = 0;
  unsigned int i;
  SD_task_t parent, grand_parent;
  xbt_dynar_t parents, grand_parents;
  SD_workstation_t *grand_parent_workstation_list;
  parents = SD_task_get_parents(task);
  if(!xbt_dynar_is_empty(parents)) {
    last_data_available = -1.0;
    xbt_dynar_foreach(parents, i, parent) {
      if(SD_task_get_kind(parent) == SD_TASK_COMM_E2E) {
	grand_parents = SD_task_get_parents(parent);

	xbt_dynar_get_cpy(grand_parents, 0, &grand_parent);
	grand_parent_workstation_list = SD_task_get_workstation_list(grand_parent);

	redist_time = SD_route_get_communication_time(grand_parent_workstation_list[0], workstation, SD_task_get_amount(parent));
	data_available = SD_task_get_finish_time(grand_parent) + redist_time;
	xbt_dynar_free_container(&grand_parents);
      }

      if(SD_task_get_kind(parent) == SD_TASK_COMP_SEQ) {
	data_available = SD_task_get_finish_time(parent);
      }

      if(last_data_available < data_available)
	last_data_available = data_available;
    }
    xbt_dynar_free_container(&parents);
    result = MAX(SD_workstation_get_available_at(workstation), last_data_available) +
      SD_workstation_get_computation_time(workstation, SD_task_get_amount(task));
  } else {
    xbt_dynar_free_container(&parents);
    result = SD_workstation_get_available_at(workstation) +
      SD_workstation_get_computation_time(workstation, SD_task_get_amount(task));
  }
  return result;
}

SD_workstation_t SD_task_get_best_workstation(SD_task_t task) {
  int i, nworkstations = SD_workstation_get_number();
  double EFT, min_EFT = -1.0;
    const SD_workstation_t *workstations = SD_workstation_get_list();
  SD_workstation_t best_workstation;
  best_workstation = workstations[0];
  min_EFT = finish_on_at(task, workstations[0]);
  printf("%s would complete at %f on %s\n", SD_task_get_name(task),
	 min_EFT, SD_workstation_get_name(workstations[0]));
  for (i = 1; i < nworkstations; i++) {
    EFT = finish_on_at(task, workstations[i]);
    printf("%s would complete at %f on %s\n", SD_task_get_name(task),
	   EFT, SD_workstation_get_name(workstations[i]));
    if (EFT < min_EFT){
      min_EFT = EFT; 
      best_workstation = workstations[i];
    }
  }
  return best_workstation;
}

xbt_dynar_t get_ready_tasks(xbt_dynar_t dag) {
  unsigned int i;
  xbt_dynar_t ready_tasks = xbt_dynar_new(sizeof(SD_task_t), NULL);
  SD_task_t task;
  xbt_dynar_foreach(dag, i, task)
    if (SD_task_get_kind(task)==SD_TASK_COMP_SEQ && SD_task_get_state(task)==SD_SCHEDULABLE)
      xbt_dynar_push(ready_tasks, &task);
  return ready_tasks;
}
						      
int main(int argc, char **argv) {
  unsigned int cursor;
  double finish_time, min_finish_time = -1.0;
  SD_task_t task, selected_task = NULL, last_scheduled_task;
  xbt_dynar_t ready_tasks;
  SD_workstation_t workstation, selected_workstation = NULL;
  int total_nworkstations = 0;
  const SD_workstation_t *workstations = NULL;
  xbt_dynar_t dag, changed;
  SD_init(&argc, argv);
  dag = SD_daxload(argv[1]);

  xbt_dynar_foreach(dag, cursor, task) {
    SD_task_watch(task, SD_DONE);
  }

  SD_create_environment(argv[2]);

  total_nworkstations = SD_workstation_get_number();
  workstations = SD_workstation_get_list();
  for(cursor = 0; cursor < total_nworkstations; cursor++)
    SD_workstation_allocate_attribute(workstations[cursor]);
