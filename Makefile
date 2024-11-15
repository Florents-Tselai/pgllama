PG_CONFIG = pg_config
PKG_CONFIG = pkg-config

EXTENSION = pgllama

MODULE_big = $(EXTENSION)

OBJS = src/pgllama.o
HEADERS = src/pgllama.h

DATA = $(wildcard sql/*--*.sql)

LLAMA_CFLAGS = $(shell $(PKG_CONFIG) --cflags llama)
LLAMA_LDFLAGS = $(shell $(PKG_CONFIG) --libs llama)

PG_CPPFLAGS = $(LLAMA_CFLAGS)
PG_LDFLAGS = $(LLAMA_LDFLAGS)

TESTS = $(wildcard test/sql/*.sql)
REGRESS = $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test --load-extension=$(EXTENSION)

TEST_MODELS := phi-2.Q2_K #qwen2-0_5b-instruct-q8_0 tinyllama-1.1b-1t-openorca.Q2_K
MODELS_PATHS := $(patsubst %,models/%.gguf,$(TEST_MODELS))
MODELS_TMP_PATHS := $(patsubst %,/tmp/%.gguf,$(TEST_MODELS))

models/phi-%.gguf:
	wget "https://huggingface.co/TheBloke/phi-2-GGUF/resolve/main/phi-$*.gguf?download=true" -O $@

models/tinyllama-%.gguf:
	wget "https://huggingface.co/TheBloke/TinyLlama-1.1B-1T-OpenOrca-GGUF/resolve/main/tinyllama-$*.gguf?download=true" -O $@

models/qwen2-%.gguf:
	wget "https://huggingface.co/Qwen/Qwen2-0.5B-Instruct-GGUF/resolve/main/qwen2-$*.gguf?download=true" -O $@

/tmp/%.gguf: models/%.gguf
	cp $^ $@

installcheck: $(MODELS_TMP_PATHS)
	@echo "All models have been copied to /tmp"

EXAMPLES_SRCS = $(wildcard examples/*.c)
EXAMPLES_EXECS = $(patsubst examples/%.c, llama-%, $(EXAMPLES_SRCS))
llama-%: examples/%.c
	$(CC) $(LLAMA_CFLAGS) $< -o $@ $(LLAMA_LDFLAGS)
llama-examples: $(EXAMPLES_EXECS)

EXTRA_CLEAN = $(EXAMPLES_EXECS)

all: llama-examples

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

dev: clean all install installcheck


