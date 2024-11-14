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

## Usage

### Model Metadata

```tsql
SELECT llama_model_desc('/tmp/tinyllama-1.1b-1t-openorca.Q2_K.gguf'::llama_model);
```
```tsql
    llama_model_desc
------------------------
    llama 1B Q2_K - Medium
(1 row)
```

```tsql
SELECT pg_size_pretty(llama_model_size('/tmp/tinyllama-1.1b-1t-openorca.Q2_K.gguf'::llama_model));
```
```tsql
 pg_size_pretty 
----------------
 459 MB
(1 row)
```

```tsql
SELECT llama_model_n_params('/tmp/tinyllama-1.1b-1t-openorca.Q2_K.gguf'::llama_model);
```
```tsql
 llama_model_n_params 
----------------------
           1100048384
(1 row)
```

### Tokenizing

```tsql

```

```tsql

```
```tsql

```

```tsql

```

```tsql

```

```tsql

```
```tsql

```

```tsql

```

```tsql

```

```tsql

```
```tsql

```

```tsql

```

```tsql

```

```tsql

```
```tsql

```

```tsql

```

```tsql

```

```tsql

```


### Content

```tsql
SELECT '/tmp/pgintro.pdf'::pdf;
```

```tsql
                                       pdf                                        
----------------------------------------------------------------------------------
 PostgreSQL Introduction                                                         +
 Digoal.Zhou                                                                     +
 7/20/2011Catalog                                                                +
  PostgreSQL Origin 
```

```tsql
SELECT pdf_title('/tmp/pgintro.pdf');
```

```tsql
        pdf_title        
-------------------------
 PostgreSQL Introduction
(1 row)
```

Getting a subset of pages

```tsql
SELECT pdf_num_pages('/tmp/pgintro.pdf');
```

```tsql
 pdf_num_pages 
---------------
            24
(1 row)
```

```tsql
SELECT pdf_page('/tmp/pgintro.pdf', 1);
```

```tsql
           pdf_page           
------------------------------
 Catalog                     +
  PostgreSQL Origin         +
  Layout                    +
  Features                  +
  Enterprise Class Attribute+
  Case
(1 row)
```

```tsql
SELECT pdf_subject('/tmp/pgintro.pdf');
```

```tsql
 pdf_subject 
-------------
 
(1 row)
```

### Metadata

```tsql
SELECT pdf_author('/tmp/pgintro.pdf');
```

```tsql
 pdf_author 
------------
 周正中
(1 row)
```

```tsql
SELECT pdf_creation('/tmp/pgintro.pdf');
```

```tsql
       pdf_creation       
--------------------------
 Wed Jul 20 11:13:37 2011
(1 row)
```

```tsql
SELECT pdf_modification('/tmp/pgintro.pdf');
```

```tsql
     pdf_modification     
--------------------------
 Wed Jul 20 11:13:37 2011
(1 row)
```

```tsql
SELECT pdf_creator('/tmp/pgintro.pdf');
```

```tsql
            pdf_creator             
------------------------------------
 Microsoft® Office PowerPoint® 2007
(1 row)
```

```tsql
SELECT pdf_metadata('/tmp/pgintro.pdf');
```

```tsql
 pdf_metadata 
--------------
 
(1 row)
```

```tsql
SELECT pdf_version('/tmp/pgintro.pdf');
```

```tsql
 pdf_version 
-------------
 PDF-1.5
(1 row)
```

### FTS
e
You can also perform full-text search (FTS), since you can work on a `pdf` file like normal text.

```tsql
SELECT '/tmp/pgintro.pdf'::pdf::text @@ to_tsquery('postgres');
```

```tsql
 ?column? 
----------
 t
(1 row)
```

```tsql
SELECT '/tmp/pgintro.pdf'::pdf::text @@ to_tsquery('oracle');
```

```tsql
 ?column? 
----------
 f
(1 row)
```

### `bytea`

If you don't have the PDF file in your filesystem but have already stored its content in a `bytea` column,
you can cast a `bytea` to `pdf`, like so:

```tsql
SELECT pg_read_binary_file('/tmp/pgintro.pdf')::pdf
```

## Installation

1. [Install pre-built version of llama.cpp](https://github.com/ggerganov/llama.cpp/blob/master/docs/install.md)

2. Make sure `libllama` dev files are available too:
    ```shell
    pkg-config llama --cflags
    pkg-config llama --ldflags
    ```
3. Clone the repo, build and install the extension
    ```
    cd /tmp
    git clone https://github.com/Florents-Tselai/pgllama.git
    cd pgllama
    make # You may have to set PKG_CONFIG=/path/to/pkg-config and/or PG_CONFIG=/path/to/pg_config
    make install # May need sudo
    ```
4. Create the extension
    ```tsql
    CREATE EXTENSION pgllama;
    ```
