#include <stdio.h>
#include <locale.h>
#include "easyjson.h"

#define BUFFER	100

struct Product {
	int id;
	char name[BUFFER];
	double price;
	char description[BUFFER];
	char imageUri[BUFFER];
}*products;


void PrintProduct(struct Product* p)
{
	printf("id = %d\nname = %s\nprice = %f\ndescription = %s\nimageUri = %s\n--------------------------------\n",
		p->id,p->name,p->price,p->description,p->imageUri);
}


int main(int argc, char** argv)
{
	StructJson Json_struct = {
		NULL,0,0,0
	};

	char temp[100] = "json.txt";
	char* data = NULL,*json = NULL;
	char file_name[100];
	if(argc == 1){
		strncpy(file_name,temp,100);
	}
	else
		strncpy(file_name,argv[1],100);
	FILE* arq = fopen(file_name,"r");
	if(!arq){
		printf("Erro ao abrir arquivo %s!",file_name);
		return -1;
	}
	fseek(arq,0,SEEK_END);
	long size = ftell(arq);

	data = (char *) malloc(size + 1);
	json = (char *) malloc(size + 1);
	if(!data || !json)
	{
		fclose(arq);
		return -2;
	}
	fseek(arq,0,SEEK_SET);
	fread(json,size,1,arq);
	fclose(arq);
	TranslateJson(json,size,&Json_struct);
	free(json);

//	printf("arrays = %d, var_per_array = %d, total = %d\n",
//		Json_struct.arrays,Json_struct.var_per_array,Json_struct.total_varables);
//	system("pause");

	products = (struct Product *) malloc(sizeof(struct Product) * Json_struct.arrays);
	if(!products)
		return -10;
	int index = 0;

//	for(register int i=0;i<Json_struct.total_varables;i++)
//	{
//		printf("%s = %s\n\n",
//			Json_struct.variables[i].name,Json_struct.variables[i].value);
//	}

	for(register int i=0;i<Json_struct.arrays;i++)
	{
		index = i * Json_struct.var_per_array;
		GetJsonValue(temp,BUFFER,&Json_struct,index);
		products[i].id = (int) strtol(temp,NULL,0);
		GetJsonValue(&products[i].name,BUFFER,&Json_struct,index+1);
		GetJsonValue(temp,BUFFER,&Json_struct,index+2);
		products[i].price = strtod(temp,NULL);
		GetJsonValue(&products[i].description,BUFFER,&Json_struct,index+3);
		GetJsonValue(&products[i].imageUri,BUFFER,&Json_struct,index+4);

		PrintProduct(&products[i]);
	}

	free(products);
	DestroyJsonVars(&Json_struct);

	fclose(arq);
	return 0;
}
