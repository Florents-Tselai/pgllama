#include "pgllama.h"
PG_MODULE_MAGIC;

typedef struct varlena llama_model_type;
#define DatumGetLlamaModelP(X)		((llama_model_type *) PG_DETOAST_DATUM(X))
#define DatumGetLlamaModelPP(X)	    ((llama_model_type *) PG_DETOAST_DATUM_PACKED(X))
#define LlamaModelPGetDatum(X)		PointerGetDatum(X)

#define PG_GETARG_LLAMA_MODEL_P(n)	DatumGetLlamaModelP(PG_GETARG_DATUM(n))
#define PG_GETARG_LLAMA_MODEL_PP(n)	DatumGetLlamaModelPP(PG_GETARG_DATUM(n))
#define PG_RETURN_LLAMA_MODEL_P(x)	PG_RETURN_POINTER(x)

PG_FUNCTION_INFO_V1(llama_model_in);
Datum
llama_model_in(PG_FUNCTION_ARGS)
{
    const char* model_path = PG_GETARG_CSTRING(0);
    llama_model_type* result;
    size_t path_length = strlen(model_path) + 1;

    result = (llama_model_type*)palloc(VARHDRSZ + path_length);
    SET_VARSIZE(result, VARHDRSZ + path_length);
    memcpy(VARDATA(result), model_path, path_length);

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(llama_model_out);
Datum
llama_model_out(PG_FUNCTION_ARGS)
{
    llama_model_type* model_data = (llama_model_type*) PG_GETARG_POINTER(0);
    const char* model_path = VARDATA(model_data);
    struct llama_model_params model_params = llama_model_default_params();
    struct llama_model* model;
    char desc_buf[256];
    char *result;

    model = llama_load_model_from_file(model_path, model_params);
    if (model == NULL)
        elog(ERROR, "llama_load_model_from_file failed");

    if (llama_model_desc(model, desc_buf, sizeof(desc_buf)) < 0)
        elog(ERROR, "Failed to retrieve model description");

    llama_free_model(model);

    result = pstrdup(desc_buf);

    PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(pg_llama_model_desc);
Datum
pg_llama_model_desc(PG_FUNCTION_ARGS)
{
    llama_model_type* model_data = (llama_model_type*) PG_GETARG_POINTER(0);
    const char* model_path = VARDATA(model_data);
    struct llama_model_params model_params = llama_model_default_params();
    struct llama_model* model;
    char desc_buf[256];
    char *result;

    model = llama_load_model_from_file(model_path, model_params);
    if (model == NULL)
        elog(ERROR, "llama_load_model_from_file failed");

    if (llama_model_desc(model, desc_buf, sizeof(desc_buf)) < 0)
        elog(ERROR, "Failed to retrieve model description");

    llama_free_model(model);

    result = pstrdup(desc_buf);

    PG_RETURN_TEXT_P(cstring_to_text(result));
}


PG_FUNCTION_INFO_V1(pg_llama_model_size);
Datum
pg_llama_model_size(PG_FUNCTION_ARGS)
{
    llama_model_type* model_data = (llama_model_type*) PG_GETARG_POINTER(0);
    const char* model_path = VARDATA(model_data);
    struct llama_model_params model_params = llama_model_default_params();
    struct llama_model* model;
    char desc_buf[256];
    uint64_t result;

    model = llama_load_model_from_file(model_path, model_params);
    if (model == NULL)
        elog(ERROR, "llama_load_model_from_file failed");

    result = (llama_model_size(model));

    llama_free_model(model);

    PG_RETURN_INT64(result);
}

PG_FUNCTION_INFO_V1(pg_llama_model_n_params);
Datum
pg_llama_model_n_params(PG_FUNCTION_ARGS)
{
    llama_model_type* model_data = (llama_model_type*) PG_GETARG_POINTER(0);
    const char* model_path = VARDATA(model_data);
    struct llama_model_params model_params = llama_model_default_params();
    struct llama_model* model;
    char desc_buf[256];
    uint64_t result;

    model = llama_load_model_from_file(model_path, model_params);
    if (model == NULL)
        elog(ERROR, "llama_load_model_from_file failed");

    result = (llama_model_n_params(model));

    llama_free_model(model);


    PG_RETURN_INT64(result);
}

