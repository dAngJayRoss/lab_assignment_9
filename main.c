#include <stdio.h>
#include <stdlib.h>

//Number of rows on a table.
int row_table_sz = 22;

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
}RecordType;

typedef struct node
{
  RecordType *data;

  struct node *prev;
  struct node *next;
}node;


// Fill out this structure
typedef struct HashType
{
  node **roots_of_nodes;

}HashType;

// Compute the hash function
int hash(int x)
{
  return x % row_table_sz;
}


node* create_node(RecordType *rt)
{
  node *temp = malloc(sizeof(node));

  temp->data = rt;
  temp->prev = NULL;
  temp->next = NULL;

  return temp;
}

HashType* createHashType()
{
  HashType *temp = malloc(sizeof(HashType));
  temp->roots_of_nodes = calloc(row_table_sz, sizeof(node));

  return temp;
}

void InsertNode(node *head, node *nData)
{
  node *temp = head;
  int count = 0;
  while(temp != NULL)
    {
      if(temp->next == NULL)
      {
        temp->next = nData;
        temp = nData->next;
      }
      else
      {
        temp = temp->next;
      }

    }
}

void LoadDataToHasType(HashType *hType, node *nData, int ndx)
{
  if(hType->roots_of_nodes[ndx] == NULL)
  {
    hType->roots_of_nodes[ndx] = nData;
  }
  else
  {
    InsertNode(hType->roots_of_nodes[ndx], nData);
  }
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

  node *temp;;
  
	for (i=0;i<hashSz;++i)
	{
    temp = &(pHashArray->roots_of_nodes[i][0]);
    
		if(temp == NULL)
    {
      printf("\tIndex %d\n\n", i);
    }
    else
    {
      printf("\tIndex %d ", i);
      while(temp != NULL)
        {
          printf("\t%d\t%c\t%d\t| ", temp->data->id, temp->data->name, temp->data->order);
          temp = temp->next;
        }
      printf("\n\n");
    }
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

  HashType *hType = createHashType();
  node     *tNode;

  int ndx = 0;
  for(int i = 0; i < recordSz; ++i)
    {
      ndx = hash(pRecords[i].id);

      tNode = create_node(&pRecords[i]);

      LoadDataToHasType(hType, tNode, ndx);
    }

  displayRecordsInHash(hType, row_table_sz);


  return 0;
}
