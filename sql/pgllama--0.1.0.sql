/* TYPE: llama_model */
CREATE TYPE llama_model;

CREATE FUNCTION llama_model_in(cstring) RETURNS llama_model IMMUTABLE
    STRICT
    LANGUAGE C
AS
'MODULE_PATHNAME';

CREATE FUNCTION llama_model_out(llama_model) RETURNS cstring IMMUTABLE
    STRICT
    LANGUAGE C
AS
'MODULE_PATHNAME';

CREATE TYPE llama_model (
    INTERNALLENGTH = -1,
    INPUT = llama_model_in,
    OUTPUT = llama_model_out,
    STORAGE = extended
);

CREATE CAST (llama_model AS text) WITH INOUT AS ASSIGNMENT;
CREATE CAST (text AS llama_model) WITH INOUT AS ASSIGNMENT;

/* API: Metadata for llama_model */

CREATE FUNCTION llama_model_desc(llama_model) RETURNS text IMMUTABLE STRICT LANGUAGE C AS 'MODULE_PATHNAME', 'pg_llama_model_desc';
CREATE FUNCTION llama_model_size(llama_model) RETURNS bigint IMMUTABLE STRICT LANGUAGE C AS 'MODULE_PATHNAME', 'pg_llama_model_size';
CREATE FUNCTION llama_model_n_params(llama_model) RETURNS bigint IMMUTABLE STRICT LANGUAGE C AS 'MODULE_PATHNAME', 'pg_llama_model_n_params';
