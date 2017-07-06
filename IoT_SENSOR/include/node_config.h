#ifndef _NODE_CONFIG_H_
#define _NODE_CONFIG_H_
#ifndef externC
# ifdef __cplusplus
#  define externC extern"C"
# else
#  define externC
# endif
#endif

enum {
    DevTemp = 1,
    DevHumm,
    DevILLum,
    DevRain,
    DevIRDist,
    DevGas,
    DevSmoke,
    DevFire,
    DevIRPers,
    DevVoice,
    DevExecuteB,
    DevExecuteA,
    DevRemoter,
    Dev125kReader,
    DevRouter = 240,
    DevCoordinator,
    DevMaxNum
};
typedef struct node_type_t {
    int id;
    const char *type;
} NODE_TYPE_STRING;

#ifdef MAIN_NODE_CONFIG
#ifdef __cplusplus
extern "C" {
#endif
static const NODE_TYPE_STRING wsncomm_nodeTypeList[] = {
    { DevTemp, "温度", },
    { DevHumm, "湿度", },
    { DevILLum, "光照", },
    { DevRain, "雨滴", },
    { DevIRDist, "距离", },
    { DevGas, "燃气", },
    { DevSmoke, "烟雾", },
    { DevFire, "火焰", },
    { DevIRPers, "安防", },
    { DevVoice, "语音", },
    { DevExecuteB, "控制B", },
    { DevExecuteA, "控制A", },
    { DevRemoter, "遥控", },
    { Dev125kReader, "IC\xE5\x8D\xA1", },//"IC卡", },
    { DevRouter, "路由", },
    { DevCoordinator, "\xE5\x8D\x8F\xE8\xB0\x83\xE5\x99\xA8", },//"协调器", },
};
const char *wsncomm_find_nodeTypeString(int type)
{
    int left = 0;
    int right = (sizeof(wsncomm_nodeTypeList) / sizeof(wsncomm_nodeTypeList[0])) - 1;
    int middle;

    while(left <= right)
    {
        middle = (left+right) / 2;
        if(type == wsncomm_nodeTypeList[middle].id)
            return wsncomm_nodeTypeList[middle].type;
        if(type > wsncomm_nodeTypeList[middle].id)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif
#else
externC const char *wsncomm_find_nodeTypeString(int type);
#endif

#ifndef _NEW_TYPES
#define _NEW_TYPES
typedef unsigned char uint8;
typedef unsigned short uint16, WORD;
typedef unsigned long uint32;
typedef uint8 byte;
#endif//_NET_TYPES

#endif //_NODE_CONFIG_H_


