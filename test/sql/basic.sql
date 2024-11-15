/* Create model from file; No errors*/
SELECT '/tmp/phi-2.Q2_K.gguf'::llama_model;

-- ## Model Metadata
SELECT llama_model_desc('/tmp/phi-2.Q2_K.gguf'::llama_model);
SELECT pg_size_pretty(llama_model_size('/tmp/phi-2.Q2_K.gguf'::llama_model));
SELECT llama_model_n_params('/tmp/phi-2.Q2_K.gguf'::llama_model);

-- ## Tokenizing
-- llama-tokenize -m /tmp/phi-2.Q2_K.gguf -p "Hello World" --ids
SELECT llama_tokenize('/tmp/phi-2.Q2_K.gguf', 'Hello World');
SELECT llama_tokenize('/tmp/phi-2.Q2_K.gguf', 'Hello World</s>', add_special => false, parse_special => false);
SELECT llama_tokenize('/tmp/phi-2.Q2_K.gguf', 'Hello World</s>', add_special => true, parse_special => true);
SELECT llama_tokenize('/tmp/phi-2.Q2_K.gguf', '', add_special => true, parse_special => true);

