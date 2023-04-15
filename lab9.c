#include <stdio.h>
#include <stdlib.h>
// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType record;
	struct HashType *next;
	int empty;
};

// Compute the hash function
int hash(int x)
{
	int hashVal = x%11;
	return hashVal;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	printf("Hash Table:\n");
	for (i=0;i<hashSz;++i)
	{
		printf("Index %d -> ", i);
		if(pHashArray[i].empty != 1)
			printf("%d, %c, %d", pHashArray[i].record.id, pHashArray[i].record.name, pHashArray[i].record.order);
		struct HashType *nextRecord = pHashArray[i].next;
		while(nextRecord != NULL){
			printf(" -> %d, %c, %d", nextRecord->record.id, nextRecord->record.name, nextRecord->record.order);
			nextRecord = nextRecord->next;
		}
		printf(" -> NULL");
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	struct HashType hashArr[11];
	int hashSize = 11;
	int hashVal;
	for(int i = 0; i < 11; i++){
		hashArr[i].empty = 1;
		hashArr[i].next = NULL;
	}
	for(int i = 0; i < recordSz; i++){
		hashVal = hash(pRecords[i].id);
		if(hashArr[hashVal].empty != 1){
			struct HashType *node = &hashArr[hashVal];
			while(node->next != NULL)
				node = node->next;
			node->next = (struct HashType*)malloc(sizeof(struct HashType));
			node = node->next;
			node->empty = 0;
			node->record.id = pRecords[i].id;
			node->record.name = pRecords[i].name;
			node->record.order = pRecords[i].order;
			node->next = NULL;
			continue;
		}
		hashArr[hashVal].empty = 0;
		hashArr[hashVal].record.id = pRecords[i].id;
		hashArr[hashVal].record.name = pRecords[i].name;
		hashArr[hashVal].record.order = pRecords[i].order;
	}
	displayRecordsInHash(hashArr, hashSize);
}