#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//function prototypes
void heapSort(int arr[], int n);
void mergeSort(int pData[], int l, int r);
int parseData(char *inputFileName, int **ppData);
void printArray(int pData[], int dataSz);
void heapify(int arr[], int size, int parentIndex);

int extraMemoryAllocated;

//organizes the heap so that the larger value is the parent
void heapify(int arr[], int size, int parentIndex)
{
	int largestIndex = parentIndex; //set the largest value's index to the parent index by default

	//assign the values of the indexes of the children
	int leftChild = parentIndex * 2 + 1;
	int rightChild = parentIndex * 2 + 2;

	//if the left child is larger, the largest value's index is the left child's index
	if(arr[leftChild] > arr[largestIndex] && leftChild < size)
		largestIndex = leftChild;

	//if the right child is larger, the largest value's index is the right child's index
	if(arr[rightChild] > arr[largestIndex] && rightChild < size)
		largestIndex = rightChild;

	//if the parent was not the largest value
	if(largestIndex != parentIndex)
	{
		//swap the largest value and the parent
		int temp = arr[parentIndex];
		arr[parentIndex] = arr[largestIndex];
		arr[largestIndex] = temp;

		//heapify the rest of that child's branch after the swap
		heapify(arr, size, largestIndex); 
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	//create a heap, in a max heap structure
	for(int i = (n / 2) - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}

	//swap the root and the last node in the array, then heapify the remaining values in the array
	for(int i = n - 1; i >= 0; i--)
	{
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		heapify(arr, i, 0);
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		//find the midpoint of the array
        int mid = (l + r) / 2;

		//recursive calls for each half of the array
        mergeSort(pData, l, mid);
        mergeSort(pData, mid + 1, r);
  
        int i, j, k;
		int n1 = mid - l + 1;
		int n2 = r - mid;
	
		//malloc space for the temporary arrays
		int * left = (int *) malloc(n1 * sizeof(int));
		extraMemoryAllocated += sizeof(left);
		int * right = (int *) malloc(n2 * sizeof(int));
		extraMemoryAllocated += sizeof(right);
	
		//copy data into both temporary arrays
		for (i = 0; i < n1; i++)
			left[i] = pData[l + i];

		for (j = 0; j < n2; j++)
			right[j] = pData[mid + 1 + j];
	
		//starting index of each array
		i = 0;
		j = 0;
		k = l;

		//merge the arrays back together
		while (i < n1 && j < n2)
		{
			if (left[i] <= right[j])
			{
				pData[k] = left[i];
				i++;
			}
			else
			{
				pData[k] = right[j];
				j++;
			}

			k++;
		}
	
		//if there are remaining values, copy them over
		while (i < n1)
		{
			pData[k] = left[i];
			i++;
			k++;
		}
	
		//if there are remaining values, copy them over
		while (j < n2) 
		{
			pData[k] = right[j];
			j++;
			k++;
		}

		free(left);
		free(right);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}