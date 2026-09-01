#pragma once
#include <stddef.h>

typedef struct {
    const char *title;             // 诗名
    const char *author;            // 作者
    const char *text[8];           // 诗文（最多8行，每行可换行显示）
    int line_count;                // 实际行数
} poetry_t;

extern const poetry_t POETRY_LIST[];
extern const size_t POETRY_COUNT;
