# PgLlama: LLM inference in PostgreSQL

[![build](https://github.com/Florents-Tselai/pgllama/actions/workflows/build.yml/badge.svg)](https://github.com/Florents-Tselai/pgpdf/actions/workflows/build.yml)

Most LLMs programs and libraries  a wrapper, around a Python wrapper, around another obscure wrapper of `llama.cpp`.
This exposes `libllama` as SQL primitives (types and functions).

```tsql
SELECT '/tmp/tinyllama-1.1b-1t-openorca.Q2_K.gguf'::llama_model;
```

```tsql
      llama_model       
------------------------
 llama 1B Q2_K - Medium
(1 row)
```

## API

- `llama_tokenize(model llama_model, prompt text, add_special boolean default true, parse_special boolean default false) → bigint[]`

    Convert the provided text into tokens. `add_special` Allow to add BOS and EOS tokens if model is configured to do so. `parse_special` Allow tokenizing special and/or control tokens which otherwise are not exposed and treated as plaintext. Does not insert a leading space.
