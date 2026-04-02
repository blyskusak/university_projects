#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "common/io.h"
#include "common/sumset.h"

//------the same stack as in nonrecursive------//

typedef struct stack_element {
	int A;
	int B;
	int w;
	int if_to_free;
} stack_element;

typedef struct sumset_pool_element {
        int size;
        int capacity;
        Sumset* elements;
} spe;

typedef struct sumset_pool {
	int which;
	int length;
	spe** pools;
} sp;

typedef struct stack {
	int size;
	int capacity;
	sp* pool;
	stack_element* elements;
} stack;

//------stucts to keep tasks created before creating threads------//

typedef struct task {
	Sumset* A;
	Sumset* B;
} task;

typedef struct task_pool {
	int size;
	int capacity;
	task* tasks;
} tp;

//------static------//

static InputData input_data;
static Solution best_solution;
static tp pula_zadan;
static tp zadania;
static Solution solutions[64];

//------functions used on tasks and task_pools to create tasks------//

task push_globalny(int ktory, int a, int b, task t) {
	task ret;
	ret.A = NULL;
	ret.B = NULL;
	ret.A = (Sumset*) malloc(sizeof(Sumset));
	ret.B = (Sumset*) malloc(sizeof(Sumset));
	if (ret.A == NULL || ret.B == NULL) exit(1);
	*(ret.A) = *(t.A);
	*(ret.B) = *(t.B);
	if (a > 0) sumset_add(ret.A, t.A, a);
	else sumset_add(ret.B, t.B, b);
	if (ktory == 2) {
		if (pula_zadan.size == pula_zadan.capacity) {
			pula_zadan.capacity *= 2;
			pula_zadan.tasks = (task*) realloc(pula_zadan.tasks, sizeof(task) * pula_zadan.capacity);
			if (pula_zadan.tasks == NULL) exit(1);
		}
		pula_zadan.tasks[pula_zadan.size] = ret;
		pula_zadan.size++;
	}
	else {
		if (zadania.size == zadania.capacity) {
                       zadania.capacity *= 2;
                       zadania.tasks = (task*) realloc(zadania.tasks, sizeof(task) * zadania.capacity);
                       if (zadania.tasks == NULL) exit(1);
                }
                zadania.tasks[zadania.size] = ret;
                zadania.size++;

	}
	return ret;
}


void stworz_zadania(task t, int depth) {
	if (!is_sumset_intersection_trivial(t.A, t.B)) {
		if (get_sumset_intersection_size(t.A, t.B) == 2 && best_solution.sum < t.A->sum && t.A->sum == t.B->sum) {
			solution_build(&best_solution, &input_data, t.A, t.B);
		}
		return;
	}
	if (depth == 0) {
		if (t.A->sum > t.B->sum) {
                	for (int i = input_data.d; i >= t.B->last; i--) {
                        	if(!does_sumset_contain(t.A, i)) (void) push_globalny(2, 0, i, t);
	                }
		}
       		else {
	                for (int i = input_data.d; i >= t.A->last; i--) {
        	                if(!does_sumset_contain(t.B, i)) (void) push_globalny(2, i, 0, t);
        	        }
		}
		return;
	}
	if (t.A->sum > t.B->sum) {
		for (int i = input_data.d; i >= t.B->last; i--) {
			if(!does_sumset_contain(t.A, i)) {
				task t_nowy = push_globalny(1, 0, i, t);
				stworz_zadania(t_nowy, depth - 1);
			}
		}
	}
	else {
		for (int i = input_data.d; i >= t.A->last; i--) {
			if(!does_sumset_contain(t.B, i)) {
				task t_nowy = push_globalny(1, i, 0, t);
				stworz_zadania(t_nowy, depth - 1);
			}
		}
	}
}

//------the same functions as in nonrecursive for stack------//

sp* new_pool() {
	spe* first = (spe*) malloc(sizeof(spe));
	if (first == NULL) exit(1);
	first->size = 2;
	first->capacity = 16;
	first->elements = (Sumset*) malloc(sizeof(Sumset) * 16);
	sp* ret = (sp*) malloc(sizeof(sp));
	if (first->elements == NULL || ret == NULL) exit(1);
	ret->length = 1;
	ret->pools = (spe**) malloc(sizeof(spe*));
	if (ret -> pools == NULL) exit(1);
	ret->pools[0] = first;
	ret->which = 0;
	return ret;
}

spe* new_el(int a) {
	spe* ret = (spe*) malloc(sizeof(spe));
	if (ret == NULL) exit(1);
	ret->size = 0;
	ret->capacity = a;
	ret->elements = (Sumset*) malloc(sizeof(Sumset) * a);
	if(ret->elements == NULL) exit(1);
	return ret;
}

stack* new_stack(Sumset* A, Sumset* B) {
	stack* stos = (stack*) malloc(sizeof(stack));
	if (stos == NULL) exit(1);
	stos->capacity = 8;
	stos->size = 1;
	stos->pool = new_pool();
	stos->elements = (stack_element*) malloc(sizeof(stack_element) * stos->capacity);
	if(stos->elements == NULL) exit(1);

	stos->elements[0].A = 0;
	stos->elements[0].B = 1;
	stos->elements[0].w = 0;

	stos->pool->pools[0]->elements[0] = *(A);
	stos->pool->pools[0]->elements[1] = *(B);
	stos->elements[0].if_to_free = 0;

	return stos;
}

void free_mem(stack* stos) {
	int x = stos->pool->length;
	for (int i = 0; i < x; i++) {
		free(stos->pool->pools[i]->elements);
		free(stos->pool->pools[i]);
	}
	free(stos->pool->pools);
	free(stos->pool);
	free(stos->elements);
	free(stos);
}

stack_element* push(stack* stos, int a_next, int b_next, int which) {
	if (stos->size == stos->capacity) {
		stos->capacity *= 2;

		stack_element* temp = (stack_element*) realloc(stos->elements, sizeof(stack_element) * stos->capacity);
		if (temp == NULL) exit(1);
		stos->elements = temp;

		spe* new_element = new_el(stos->capacity);
		stos->pool->length++;
		stos->pool->pools = (spe**) realloc(stos->pool->pools, sizeof(spe*) * stos->pool->length);
		if (stos->pool->pools == NULL) exit(1);
		stos->pool->which++;
		stos->pool->pools[stos->pool->which] = new_element;
	}
	if (stos->pool->pools[stos->pool->which]->size == stos->pool->pools[stos->pool->which]->capacity) stos->pool->which++;
	int akt = stos->pool->which;
	stos->elements[stos->size].A = stos->pool->pools[akt]->size;
	stos->elements[stos->size].B = stos->pool->pools[akt]->size + 1;
	stos->elements[stos->size].w = akt;
	stos->pool->pools[akt]->elements[stos->elements[stos->size].A] = stos->pool->pools[stos->elements[which].w]->elements[stos->elements[which].A];
	stos->pool->pools[akt]->elements[stos->elements[stos->size].B] = stos->pool->pools[stos->elements[which].w]->elements[stos->elements[which].B];

	stos->elements[stos->size].if_to_free = 0;

	if (a_next > 0) sumset_add(&(stos->pool->pools[akt]->elements[stos->elements[stos->size].A]), &(stos->pool->pools[stos->elements[which].w]->elements[stos->elements[which].A]), a_next);
	else sumset_add(&(stos->pool->pools[akt]->elements[stos->elements[stos->size].B]), &(stos->pool->pools[stos->elements[which].w]->elements[stos->elements[which].B]), b_next);

	stos->size++;
	stos->pool->pools[akt]->size += 2;
	return &stos->elements[which];
}

void pop(stack* stos) {
	if (stos->size == 0) return;
	stos->size--;
	if (stos->pool->pools[stos->pool->which]->size == 0) stos->pool->which--;
	stos->pool->pools[stos->pool->which]->size -= 2;
}

//------function for thread to solve it's tasks------//

void* thread_solve(void* id) {
	int id_w = *((int*) id);
	int maks = 0;
	int d = input_data.d;
	int which;

	//after creating task pool we take tasks from this pool, every thread takes different so it is lockfree
	for (int i = id_w; i < pula_zadan.size; i += input_data.t) {
		stack* stos = new_stack(pula_zadan.tasks[i].A, pula_zadan.tasks[i].B);
		while (stos->size > 0) {
			which = stos->size - 1;
			stack_element* top = &stos->elements[which];
			if (top->if_to_free) {
				pop(stos);
				continue;
			}

			if (is_sumset_intersection_trivial(&stos->pool->pools[top->w]->elements[top->A], &stos->pool->pools[top->w]->elements[top->B])) top->if_to_free = 1;
			else {
				if (maks < stos->pool->pools[top->w]->elements[top->B].sum && stos->pool->pools[top->w]->elements[top->A].sum == stos->pool->pools[top->w]->elements[top->B].sum &&
					get_sumset_intersection_size(&stos->pool->pools[top->w]->elements[top->A], &stos->pool->pools[top->w]->elements[top->B]) == 2) {
					maks = stos->pool->pools[top->w]->elements[top->B].sum;
					solution_build(&solutions[id_w], &input_data, &stos->pool->pools[top->w]->elements[top->A], &stos->pool->pools[top->w]->elements[top->B]);
				}
				pop(stos);
				continue;
			}


			if (stos->pool->pools[top->w]->elements[top->A].sum > stos->pool->pools[top->w]->elements[top->B].sum) {
				for (int i = d; i >= stos->pool->pools[top->w]->elements[top->B].last; i--) {
	        	                if (!does_sumset_contain(&stos->pool->pools[top->w]->elements[top->A], i)) {
        	        	                top = push(stos, 0, i, which);
                		        }
				}
			}
			else {
				for (int i = d; i >= stos->pool->pools[top->w]->elements[top->A].last; i--) {
					if (!does_sumset_contain(&stos->pool->pools[top->w]->elements[top->B], i)) {
						top = push(stos, i, 0, which);
					}
				}
			}
		}
		free_mem(stos);
		free(pula_zadan.tasks[i].A);
		free(pula_zadan.tasks[i].B);
	}
	return NULL;
}

int main()
{
	input_data_read(&input_data);
	//input_data_init(&input_data, 8, 32, (int[]){0}, (int[]){1, 0});
	solution_init(&best_solution);

	pthread_t threads[input_data.t - 1];
	int threads_id[input_data.t];

	task zadanie;
        zadanie.A = (Sumset*) malloc(sizeof(Sumset));
        zadanie.B = (Sumset*) malloc(sizeof(Sumset));
	if (zadanie.A == NULL || zadanie.B == NULL) exit(1);
        *(zadanie.A) = input_data.a_start;
        *(zadanie.B) = input_data.b_start;

        zadania.size = 1;
        zadania.capacity = 8;
        zadania.tasks = (task*) malloc(sizeof(task) * 8);
	if (zadania.tasks == NULL) exit(1);
        zadania.tasks[0] = zadanie;

        pula_zadan.size = 0;
        pula_zadan.capacity = 8;
        pula_zadan.tasks = (task*) malloc(sizeof(task) * 8);
	if(pula_zadan.tasks == NULL) exit(1);

	stworz_zadania(zadanie, 2);


	for (int i = 0; i < input_data.t - 1; i++) {
		solution_init(&solutions[i]);
		threads_id[i] = i;
		int czy_stworzony = pthread_create(&threads[i], NULL, thread_solve, (void*)&threads_id[i]);
		if (czy_stworzony != 0) exit(1);
	}

	solution_init(&solutions[input_data.t - 1]);
	threads_id[input_data.t - 1] = input_data.t - 1;
	thread_solve((void*)&threads_id[input_data.t - 1]);

	for (int i = 0; i < input_data.t - 1; i++) {
		pthread_join(threads[i], NULL);
		if (solutions[i].sum > best_solution.sum) best_solution = solutions[i];
	}
	if (solutions[input_data.t - 1].sum > best_solution.sum) best_solution = solutions[input_data.t - 1];


	for (int i = 0; i < zadania.size; i++) {
		free(zadania.tasks[i].A);
		free(zadania.tasks[i].B);
	}
	free(zadania.tasks);
	free(pula_zadan.tasks);

	solution_print(&best_solution);

	return 0;
}

