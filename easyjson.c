#include "easyjson.h"
#include <stdio.h>

void DestroyJsonVars(StructJson* j)
{
	free(j->variables);
	j->arrays = 0;
	j->variables = NULL;
	j->total_varables = 0;
	j->var_per_array = 0;
}

int GetJsonValue(void* ObjInstance,size_t size_instance,const StructJson* code,unsigned int var_number)
{
	char* p =(char *) &code->variables[var_number].value;
	char* dest = (char *)ObjInstance;
	for(register int i=0;i<size_instance;i++)
	{
		*dest = *p;
		dest++;
		p++;
	}
	return 0;
}

int TranslateJson(char* json,size_t size,StructJson* out)
{
	out->variables = NULL;
	out->var_per_array = 0;
	out->total_varables = 0;
	out->arrays = 0;

	char temp[BUFFER];
	char* data = (char *) malloc(size);
	if(!data)
		return -1;

	register int delim = 0,temp_counter=0,value = 0,aspas = 0, operation = 0, index = 0;
	int loop=1;
	while(index < strlen(json) - 1){

		memset(temp,'\0',BUFFER);
		memset(data,'\0',size+1);

		for(register int i=index; i< strlen(json);i++)
		{
			if(json[i] == '['){
				continue;
			}
			if(json[i] == '{'){
				value = 1;
				temp_counter = 0;
				continue;
			}
			if(json[i] == '\"'){
				aspas = aspas ^ 1;
			}
			if(json[i] == '}' && !aspas){
				index = i + 1;
				data[temp_counter] = '}';
				data[temp_counter + 1] = '\0';
				size = temp_counter + 2;
				temp_counter = 0;
				out->arrays++;
				out->var_per_array = 0;
				break;
			}
			if(value)
			{
				data[temp_counter] = json[i];
				temp_counter++;
			}
		}

		aspas =0;
		value = 0;
		delim = 0;
		for(register int i=0;i<size;i++)
		{

			if(data[i] == '\"' || data[i] == ':' || data[i] == ',' || data[i] == '}'){
				if(data[i] == '\"' && value>0){
					aspas = aspas ^ 1;
				}
				else if(data[i] == ':' && value>0){
				}
				else
				{
					if(data[i] == ':' || data[i] == ',')
					{
						if(!aspas){
							value = value ^ 1;
							if(value)
								operation = 1;
						}
						else
							loop = 0;
					}
					else if(data[i] == '\"' && value == 0)
					{
						aspas = 0;
						delim = delim ^ 1;
						if(delim)
							operation = 0;
					}
					else if(data[i] == '}'){
						delim = 0;
						value = 0;
					}
					if(!delim  && !value){
						aspas = 0;
						temp[temp_counter] = '\0';


						if(!operation){
							out->total_varables++;
							out->var_per_array++;

							out->variables = (struct var *) realloc(out->variables,sizeof(struct var) * out->total_varables);
							if(!out->variables)
								return -10;
							strncpy(out->variables[out->total_varables -1].name,temp,BUFFER);
							printf("%s = ",out->variables[out->total_varables - 1].name);
						}
						else{
							strncpy(out->variables[out->total_varables -1].value,temp,BUFFER);
							printf("%s\n",out->variables[out->total_varables - 1].value);
						}
					}
					if(loop)
					{
						temp_counter = 0;
						continue;
					}
					else
						loop = 1;
				}
			}

			if(delim || value)
			{
				if(temp_counter<size)
				{
					if(data[i] != '{' || data[i] != '}' || data[i] != '[' || data[i] != ']')
					{
						temp[temp_counter] = data[i];
						temp_counter++;
					}
				}
			}
		}
	}
	memset(data,'\0',size);
	free(data);
	return 0;
}
