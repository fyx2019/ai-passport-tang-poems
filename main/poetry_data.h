// main/poetry_data.h —— 唐诗数据库：10首经典唐诗
#pragma once

#include <stddef.h>

typedef struct {
    const char *title;           // 诗名
    const char *author;          // 作者
    const char *text[8];         // 诗文（最多8行，每行可换行显示）
    int line_count;              // 实际行数
} poetry_t;

static const poetry_t POETRY_LIST[] = {
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
    {
        .title = "早发白帝城",
        .author = "李白",
        .line_count = 4,
        .text = {
            "朝辞白帝彩云间，",
            "千里江陵一日还。",
            "两岸猿声啼不住，",
            "轻舟已过万重山。",
        }
    },
    {
        .title = "静夜思",
        .author = "李白",
        .line_count = 4,
        .text = {
            "床前明月光，",
            "疑是地上霜。",
            "举头望明月，",
            "低头思故乡。",
        }
    },
    {
        .title = "春晓",
        .author = "孟浩然",
        .line_count = 4,
        .text = {
            "春眠不觉晓，",
            "处处闻啼鸟。",
            "夜来风雨声，",
            "花落知多少。",
        }
    },
    {
        .title = "绝句",
        .author = "杜甫",
        .line_count = 4,
        .text = {
            "两个黄鹂鸣翠柳，",
            "一行白鹭上青天。",
            "窗含西岭千秋雪，",
            "门泊东吴万里船。",
        }
    },
    {
        .title = "望庐山瀑布",
        .author = "李白",
        .line_count = 4,
        .text = {
            "日照香炉生紫烟，",
            "遥看瀑布挂前川。",
            "飞流直下三千尺，",
            "疑是银河落九天。",
        }
    },
    {
        .title = "蜀道难",
        .author = "李白",
        .line_count = 4,
        .text = {
            "噫吁嘻，危乎高哉！",
            "蜀道之难，难于上青天。",
            "蚕丛及鱼凫，",
            "开国何茫然。",
        }
    },
    {
        .title = "江南春",
        .author = "杜牧",
        .line_count = 4,
        .text = {
            "千里莺啼绿映红，",
            "水村山郭酒旗风。",
            "南朝四百八十寺，",
            "多少楼台烟雨中。",
        }
    },
    {
        .title = "将进酒",
        .author = "李白",
        .line_count = 4,
        .text = {
            "君不见，黄河之水天上来，",
            "奔流到海不复回。",
            "君不见，高堂明镜悲白发，",
            "朝如青丝暮成雪。",
        }
    },
    {
        .title = "独坐敬亭山",
        .author = "李白",
        .line_count = 4,
        .text = {
            "众鸟高飞尽，",
            "孤云独去闲。",
            "相看两不厌，",
            "只有敬亭山。",
        }
    }
};

#define POETRY_COUNT (sizeof(POETRY_LIST) / sizeof(POETRY_LIST[0]))
