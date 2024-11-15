#ifndef PGLLAMA_H
#define PGLLAMA_H
#include "postgres.h"
#include <utils/palloc.h>

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/jsonb.h"
#include "utils/datetime.h"
#include "utils/date.h"
#include "utils/array.h"
#include "utils/fmgroids.h"
#include "catalog/pg_type.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if PG_VERSION_NUM >= 160000
#include "varatt.h"

#endif

#if PG_VERSION_NUM < 130000
#define TYPALIGN_DOUBLE 'd'
#define TYPALIGN_INT 'i'
#endif

#include "llama.h"



#endif