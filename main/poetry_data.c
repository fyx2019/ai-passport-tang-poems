// main/poetry_data.c
#include "poetry_data.h"

const poetry_t POETRY_LIST[] = {
    {
        .title = "登鹳雀楼",
        .author = "王之涣",
        .line_count = 4,
        .text = {
            "白日依山尽，",
            "黄河入海流。",
            "欲穷千里目，",
            "更上一层楼。",
        }
    },
    // ... 其余9首保持不变 ...
};

const size_t POETRY_COUNT = sizeof(POETRY_LIST) / sizeof(POETRY_LIST[0]);
