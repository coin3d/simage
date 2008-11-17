#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <simage.h>
#include <string.h>
#include <stdarg.h>

struct simage_param_data {
  int type;
  char * name;
  union {
    int integerdata;
    float floatdata;
    double doubledata;
    char * stringdata;
    void * pointerdata;
    void (*functiondata)();
  } data;
  struct simage_param_data * next;
};

typedef void s_generic_func();

struct simage_parameters_s {
  struct simage_param_data * list;
};

s_params * 
s_params_create(void)
{
  s_params * par = (s_params*) malloc(sizeof(s_params));
  par->list = NULL;
  return par;
}

void 
s_params_destroy(s_params * params)
{
  struct simage_param_data * next, * ptr = params->list;
  while (ptr) {
    next = ptr->next;
    if (ptr->type == S_STRING_PARAM_TYPE &&
        ptr->data.stringdata) free((void*)ptr->data.stringdata);
    free((void*)ptr);
    ptr = next;
  }
  free((void*)params);
}

static struct simage_param_data * 
find_param(s_params * params, const char * name, int type, int allocnew)
{
  struct simage_param_data * last, * ptr;
  last = NULL;
  ptr = params->list;
  while (ptr) {
    last = ptr;
    if (strcmp(ptr->name, name) == 0 && ptr->type == type) return ptr;
    ptr = ptr->next;
  }
  if (allocnew) {
    ptr = (struct simage_param_data*) malloc(sizeof(struct simage_param_data));
    ptr->name = (char *) malloc(strlen(name)+1);
    strcpy(ptr->name, name);
    ptr->next = NULL;
    ptr->type = type;
    if (last) last->next = ptr;
    else params->list = ptr;
  }
  return ptr;
}

s_params * 
s_params_copy(s_params * params)
{
  struct simage_param_data * data;
  struct simage_param_data * src;
  s_params * par;
  
  if (params == NULL) return NULL;
  
  par = (s_params*) malloc(sizeof(s_params));
  par->list = NULL;
  
  src = params->list;
  while (src) {
    data = find_param(par, src->name, src->type, 1);
    switch (src->type) {
    case S_INTEGER_PARAM_TYPE:
      data->data.integerdata = src->data.integerdata;
      break;
    case S_FLOAT_PARAM_TYPE:
      data->data.floatdata = src->data.floatdata;
      break;
    case S_DOUBLE_PARAM_TYPE:
      data->data.doubledata = src->data.doubledata;
      break;
    case S_STRING_PARAM_TYPE:
      data->data.stringdata = src->data.stringdata;
      if (data->data.stringdata != NULL) {
        data->data.stringdata = (char*) malloc(strlen(src->data.stringdata)+1);
        strcpy(data->data.stringdata, src->data.stringdata);
      }
      break;
    case S_POINTER_PARAM_TYPE:
      data->data.pointerdata = src->data.pointerdata;
      break;
    case S_FUNCTION_PARAM_TYPE:
      data->data.functiondata = src->data.functiondata;
      break;
    default:
      assert(0);
      break;
    }
    src = src->next;
  }
  return par;
}

static void 
add_integer_param(s_params * params, const char * name, int val)
{
  struct simage_param_data * data = find_param(params, name, S_INTEGER_PARAM_TYPE, 1);
  data->data.integerdata = val;
} 

static void 
add_float_param(s_params * params, const char * name, float val)
{
  struct simage_param_data * data = find_param(params, name, S_FLOAT_PARAM_TYPE, 1);
  data->data.floatdata = val;
} 

static void 
add_double_param(s_params * params, const char * name, double val)
{
  struct simage_param_data * data = find_param(params, name, S_DOUBLE_PARAM_TYPE, 1);
  data->data.doubledata = val;
} 

static void 
add_string_param(s_params * params, const char * name,  const char * val)
{
  struct simage_param_data * data = find_param(params, name, S_STRING_PARAM_TYPE, 1);
  data->data.stringdata = NULL;
  if (val) {
    data->data.stringdata = (char*) malloc(strlen(val)+1);
    strcpy(data->data.stringdata, val);
  }
} 

static void 
add_pointer_param(s_params * params, const char * name, void * val)
{
  struct simage_param_data * data = find_param(params, name, S_POINTER_PARAM_TYPE, 1);
  data->data.pointerdata = val;
} 

static void 
add_function_param(s_params * params, const char * name, void (*val)())
{
  struct simage_param_data * data = find_param(params, name, S_FUNCTION_PARAM_TYPE, 1);
  data->data.functiondata = val;
} 

void 
s_params_set(s_params * params, ...)
{
  int type;
  int argerr;
  const char * name;
  
  va_list ap;
  va_start(ap, params);
   
  name = va_arg(ap, const char*);
  while (name) {
    type = va_arg(ap, int);
    argerr = 0;
    switch (type) {

    case S_INTEGER_PARAM_TYPE:
      add_integer_param(params, name, va_arg(ap, int));
      break;

      /* FIXME: one should never use 'float' as a type for va_arg, so
         this type is bogus. see for instance

         http://c-faq.com/varargs/float.html

         -mortene
      */
    case S_FLOAT_PARAM_TYPE:
#if (__GNUC__ == 2 && __GNUC_MINOR__ == 96) || __GNUC__ >= 3
      /* fix for silly bug in gcc 2.96 */
      add_float_param(params, name, va_arg(ap, double));      
#else /* ! gcc version 2.96 */
      add_float_param(params, name, va_arg(ap, float));
#endif /* gcc version 2.96 */
      break;

    case S_DOUBLE_PARAM_TYPE:
      add_double_param(params, name, va_arg(ap, double));
      break;

    case S_STRING_PARAM_TYPE:
      add_string_param(params, name, va_arg(ap, const char*));
      break;

    case S_POINTER_PARAM_TYPE:
      add_pointer_param(params, name, va_arg(ap, void *));
      break;

    case S_FUNCTION_PARAM_TYPE:
      add_function_param(params, name, va_arg(ap, s_generic_func*));
      break;

    default:
      argerr = 1;
      break;
    }
    if (argerr) break; /* whoa, get out of here */

    /* process next token */
    name = va_arg(ap, const char*);
  }
  va_end(ap);
}

int 
s_params_get(s_params * params, ...)
{
  int numok;
  int type;
  int argerr;
  const char * name;
  struct simage_param_data * data;
  
  va_list ap;
  va_start(ap, params);
  
  numok = 0;

  name = va_arg(ap, const char*);
  while (name) {
    type = va_arg(ap, int);
    argerr = 0;
    switch (type) {
    case S_INTEGER_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        int * ptr = va_arg(ap, int*);
        *ptr = data->data.integerdata;
        numok++;
      }
      else argerr = 1;
      break;
    case S_FLOAT_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        float * ptr = va_arg(ap, float*);
        *ptr = data->data.floatdata;
        numok++;
      }
      else argerr = 1;
      break;
    case S_DOUBLE_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        double * ptr = va_arg(ap, double*);
        *ptr = data->data.doubledata;
        numok++;
      }
      else argerr = 1;
      break;
    case S_STRING_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        char ** ptr = va_arg(ap, char**);
        *ptr = data->data.stringdata;
        numok++;
      }
      else argerr = 1;
      break;
    case S_POINTER_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        void ** ptr = va_arg(ap, void**);
        *ptr = data->data.pointerdata;
        numok++;
      }
      else argerr = 1;
      break;
    case S_FUNCTION_PARAM_TYPE:
      data = find_param(params, name, type, 0);
      if (data) {
        s_generic_func ** ptr = va_arg(ap, s_generic_func**);
        *ptr = data->data.functiondata;
        numok++;
      }
      else argerr = 1;
      break;
    default:
      argerr = 1;
      break;
    }
    if (argerr) break;
    /* process next token */
    name = va_arg(ap, const char*);
  }
  va_end(ap);
  return numok;
}
