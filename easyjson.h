#ifndef EASYJSON_H_INCLUDED
#define EASYJSON_H_INCLUDED

#define BUFFER	100

#include <stdlib.h>
#include <string.h>

typedef struct {
	struct var* variables;
	int total_varables;
	int var_per_array;
	int arrays;
}StructJson;


struct var{
	char name[BUFFER];
	char value[BUFFER];
};

extern void DestroyJsonVars(StructJson* j);
extern int TranslateJson(char* json,size_t size_json,StructJson* out);
extern int GetJsonValue(void* ObjInstance,size_t size_instance,const StructJson* code,unsigned int var_number);

#endif // EASYJSON_H_INCLUDED
