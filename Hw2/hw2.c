#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

typedef struct{
	int *arrPtr;
	int arrBase;
}ARR_INFO;

int sum = 0;
pthread_barrier_t barrier;

void *ThreadSort(void *arg);
void *ThreadAdd(void *arg);
int *Merge(int *arr, int arrSize, int arrDivideSize, int totalThread);
int Compare(const void *arg1, const void *arg2);

int main(int agrc, char* argv[]){
	int totalThread = sysconf(_SC_NPROCESSORS_ONLN);
	int randSeed = atoi(argv[1]);
	int arrSize = atoi(argv[2]);
	int arrDivideSize = arrSize / totalThread;
	int *arr = (int *)malloc(arrSize * sizeof(int));
	int *arrSorted = NULL;
	int i;
	bool isDivide = (arrSize % totalThread == 0);
	ARR_INFO *arrInfo = NULL;
	pthread_t tid;

	srand(randSeed);
	for(i = 0; i < arrSize; i++){
		arr[i] = rand();
	}
		
	if(!isDivide){
		totalThread++;
	}

	pthread_barrier_init(&barrier, NULL, totalThread + 1);
	for(i = 0; i < totalThread; i++){
		arrInfo = malloc(sizeof(ARR_INFO));
		arrInfo->arrPtr = &(arr[i * arrDivideSize]);
		if(!isDivide && totalThread -1 == i){
			arrInfo->arrBase = arrSize % (totalThread - 1);
		}
		else{
			arrInfo->arrBase = arrDivideSize;
		}
		pthread_create(&tid, NULL, ThreadSort, (void*)arrInfo);
	}
	pthread_barrier_wait(&barrier);
	pthread_barrier_destroy(&barrier);
	arrSorted = Merge(arr, arrSize, arrDivideSize, totalThread);

	totalThread = sysconf(_SC_NPROCESSORS_ONLN);
	pthread_barrier_init(&barrier, NULL, totalThread + 1);
	sum = arrSorted[arrSize-1] - arrSorted[0];
	for(i = 0; i < totalThread; i++){
		arrInfo = malloc(sizeof(ARR_INFO));
		arrInfo->arrPtr = arrSorted;
		arrInfo->arrBase = arrDivideSize;
		if(i != 0){
			arrInfo->arrPtr = &(arrSorted[(i * arrDivideSize) - i]);
		}
		if(!isDivide && totalThread -1 == i){ 
			arrInfo->arrBase = arrSize - i * (arrDivideSize - 1) - 1;
		}
		pthread_create(&tid, NULL, ThreadAdd, (void*)arrInfo);	
	}
	pthread_barrier_wait(&barrier);
	pthread_barrier_destroy(&barrier);
	printf("%d\n", sum);
}

void *ThreadSort(void *arg){
	ARR_INFO *arrInfo = (ARR_INFO *)arg;
	qsort(arrInfo->arrPtr, arrInfo->arrBase, sizeof(int), Compare);
	pthread_barrier_wait(&barrier);
}

void *ThreadAdd(void *arg){
	ARR_INFO *arrInfo = (ARR_INFO *)arg;
	int num = 0;
	int i;
	int *ptr = arrInfo->arrPtr;
	for(i = 0; i < arrInfo->arrBase - 1; i++){
		num += (ptr[i + 1] - ptr[i]);
	}
	pthread_barrier_wait(&barrier);
}

int* Merge(int *arr, int arrSize, int arrDivideSize, int totalThread){
	int i;
	int arrIndex;
	int *arrDivideIndex = (int *)malloc(totalThread *sizeof(int));
	int *arrSorted = (int *)malloc(arrSize * sizeof(int));

	for(i = 0; i < totalThread; i++){
		arrDivideIndex[i] = i * arrDivideSize;
	}

	for(arrIndex = 0; arrIndex < arrSize; arrIndex++){
		int minValue = INT_MAX;
		int currentMinIndex = -1;
		for(i = 0; i < totalThread; i++){
			if(arrDivideIndex[i] < ((i + 1) * arrDivideSize) && arr[arrDivideIndex[i]] < minValue){
				currentMinIndex = i;
				minValue = arr[arrDivideIndex[i]];
			}
		}
		arrSorted[arrIndex] = arr[arrDivideIndex[currentMinIndex]];
		if(arrDivideIndex[currentMinIndex] == (arrSize - 1) && totalThread == sysconf(_SC_NPROCESSORS_ONLN) + 1){
			totalThread--;
			continue;
		}
		arrDivideIndex[currentMinIndex]++;
	}
	return arrSorted;
}

int Compare(const void *arg1, const void *arg2){
	int item1 = *(int *)arg1;
	int item2 = *(int *)arg2;
	return item1 - item2;
}

