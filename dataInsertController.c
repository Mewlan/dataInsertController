//
//  dataInsertController.c
//  iWillookDictionary
//
//  Created by Mewlan Musajan on 10/25/17.
//  Copyright (c) 2017 Mewlan Musajan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DICT_LL_LEN sizeof(struct DictionaryLL)
#define DICT_LEN sizeof(struct Dictionary)

unsigned int L;
unsigned int N;
char DictName[32];

typedef struct DictionaryLL
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	unsigned int yearofOrigin;
	struct DictionaryLL *next;
} DICT_LL;

typedef struct Dictionary
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	unsigned int yearofOrigin;
} DICT;

typedef enum Language
{
	english,
	chinese,
	uyghur
} LANG;

LANG CurrentLanguage = 0;

/******Model******/

void dictModelUpdateController(DICT_LL *head)
{
	FILE *dictionaryfp;
	if ((dictionaryfp = fopen(DictName, "wb")) == NULL)
	{
		printf("can not open %s\n", DictName);
		exit(0);
	}
	switch (CurrentLanguage) {
		case english:rewind(dictionaryfp);break;
		case chinese:fseek(dictionaryfp, N * DICT_LEN, 0);break;
		case uyghur:fseek(dictionaryfp, 2 * N * DICT_LEN, 0);break;
		default:break;
	}
	DICT *dictionary;
	dictionary = (DICT*) calloc(N, DICT_LEN);
	DICT_LL *dictPtr = head;
	for (int i = 0; dictPtr->yearofOrigin != 2017; ++i, dictPtr = dictPtr->next) 
	{
		*(dictionary + i)->vocabulary = dictPtr->vocabulary;
		switch (CurrentLanguage) {
			case uyghur:
			case english:
				*(dictionary + i)->definition.latinDefinition = dictPtr->definition.latinDefinition;
			break;
			case chinese:
				*(dictionary + i)->definition.cnDefinition = dictPtr->definition.cnDefinition;
			break;
			default:break;
		}
	}
	for (int i = 0; i < N; ++i)
	{
		if (fwrite(&dictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
		{
			printf("dictionary write error\n");
		}
	}
	free(dictionary);
	fclose(dictionaryfp);
}

/******Model******/

DICT_LL * dataInsertController(DICT_LL *head, DICT_LL *ptr1, DICT_LL *ptr2)
{
	printf("Enter new dictionary name you want to creat:\n");
	scanf("%s", DictName);
	strcat(DictName, ".dat");
	printf("char vocabulary[20], char latinDefinition[80], unsigned int yearofOrigin:\n");
	switch (CurrentLanguage) {
		case uyghur:
		case english:
		scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.latinDefinition, &ptr1->yearofOrigin);
		fflush(stdin);
		N = 0;
		head = NULL;
		while (ptr1->yearofOrigin != 2017) {
			++N;
			if (N == 1)
			{
				head = ptr1;
			} else {
				ptr2->next = ptr1;
			}
			ptr2 = ptr1;
			ptr1 = (DICT_LL*) malloc(DICT_LL_LEN);
			scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.latinDefinition, &ptr1->yearofOrigin);
			fflush(stdin);
		}
		ptr2->next = NULL;
		return(head);			
		break;
		case chinese:
		scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.cnDefinition, &ptr1->yearofOrigin);
		fflush(stdin);
		N = 0;
		head = NULL;
		while (ptr1->yearofOrigin != 2017) {
			++N;
			if (N == 1)
			{
				head = ptr1;
			} else {
				ptr2->next = ptr1;
			}
			ptr2 = ptr1;
			ptr1 = (DICT_LL*) malloc(DICT_LL_LEN);
			scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.cnDefinition, &ptr1->yearofOrigin);
			fflush(stdin);
		}
		ptr2->next = NULL;
		return(head);		
		break;
		default:break;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	DICT_LL *head, *ptr1, *ptr2;
	ptr1 = ptr2 = (DICT_LL*) malloc(DICT_LL_LEN);
	dictModelUpdateController(dataInsertController(head, ptr1, ptr2));

	return 0;
}