#include <stddef.h>
#include <stdlib.h>

#include "common/io.h"
#include "common/sumset.h"

//------structs used to create stack------//

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

//------functions on stack------//

sp* new_pool() {
	spe* first = (spe*) malloc(sizeof(spe) + sizeof(Sumset) * 16);
	if (first == NULL) exit(1);
	first->size = 2;
	first->capacity = 16;
	first->elements = (Sumset*) malloc(sizeof(Sumset) * 16);
	sp* ret = (sp*) malloc(sizeof(sp));
	if (ret == NULL || first->elements == NULL) exit(1);
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
	if (ret->elements == NULL) exit(1);
	return ret;
}

stack* new_stack(InputData* input_data) {
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

	stos->pool->pools[0]->elements[0] = input_data->a_start;
	stos->pool->pools[0]->elements[1] = input_data->b_start;
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

//------function used to calculate best solutions for problem using stack------//

void solve(Solution* rozwiazanie, InputData* input_data) {
	stack* stos = new_stack(input_data);
	int d = input_data->d;
	int maks = 0;
	int which;
	while (stos->size > 0) {
		//we take top of stack. If we want to remove it, we do pop on stack, otherwise we try to get needed values
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
				solution_build(rozwiazanie, input_data, &stos->pool->pools[top->w]->elements[top->A], &stos->pool->pools[top->w]->elements[top->B]);
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
}

int main() {
	InputData input_data;
	input_data_read(&input_data);
	//input_data_init(&input_data, 8, 32, (int[]){0}, (int[]){1, 0});

	Solution best_solution;
	solution_init(&best_solution);
	solve(&best_solution, &input_data);
	solution_print(&best_solution);

	return 0;
}
