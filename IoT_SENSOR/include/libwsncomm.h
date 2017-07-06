#ifndef _LIBWSNCOMM_H_
#define _LIBWSNCOMM_H_

#ifndef externC
	#ifdef __cplusplus
		#define externC extern"C"
	#else
		#define externC
	#endif
#endif

#ifndef EXPORT_SYMBOL
#ifndef _WIN32
#define EXPORT_SYMBOL   __attribute__((visibility("default")))
#else
#ifdef _EXPORT
#define EXPORT_SYMBOL  __declspec(dllexport)
#else
#define EXPORT_SYMBOL  __declspec(dllimport)
#endif
#endif
#endif

typedef struct wsncomm_node_func_t{
    unsigned char funcCode, funcID;
    unsigned char rCycle;
}WSNCOMM_NODE_FUNC;

typedef struct wsncomm_node_t{
    unsigned short nwkAddr;
    unsigned short parAddr;
    unsigned char hwAddr[8];
	int funcNum;
	WSNCOMM_NODE_FUNC *funcInfo;   //function list
}WSNCOMM_NODE;


externC EXPORT_SYMBOL WSNCOMM_NODE *wsncomm_getNode_byAddr(const char *ip, unsigned short nwkAddr);
externC EXPORT_SYMBOL WSNCOMM_NODE *wsncomm_getNode_byType(const char *ip, int type,int id);
externC EXPORT_SYMBOL int wsncomm_getAllNode(const char *ip, WSNCOMM_NODE **nodes);
externC EXPORT_SYMBOL WSNCOMM_NODE *wsncomm_node_dup(const WSNCOMM_NODE *node);
externC EXPORT_SYMBOL WSNCOMM_NODE *wsncomm_node_list_dup(const WSNCOMM_NODE *node, int count);
externC EXPORT_SYMBOL void wsncomm_delete_node(WSNCOMM_NODE *node);
externC EXPORT_SYMBOL void wsncomm_delete_node_list(WSNCOMM_NODE *node, int count);
externC EXPORT_SYMBOL int wsncomm_sendNode_byPoint(const char *ip, unsigned short nwkAddr, int point, const char *data, int len);
externC EXPORT_SYMBOL int wsncomm_sendNode_byType(const char *ip, int type, int id, const char *data, int len);
externC EXPORT_SYMBOL int wsncomm_getNodeData_byPoint(const char *ip, unsigned short nwkAddr, int point, char **data);
externC EXPORT_SYMBOL int wsncomm_getNodeData_byType(const char *ip, int type, int id, char **data);

typedef void (*CB_NEW_NODE)(void *arg, unsigned short nwkAddr, unsigned short parAddr, unsigned char macAddr[8]);
typedef void (*CB_NEW_FUNC)(void *arg, unsigned short nwkAddr, int funcNum, WSNCOMM_NODE_FUNC *funcList);
typedef void (*CB_NEW_DATA)(void *arg, unsigned short nwkAddr, int endPoint, int funcCode, int funcID, char *data, int len);
typedef void (*CB_NODE_GONE)(void *arg, unsigned short nwkAddr);
typedef void (*CB_SERVER_GONE)(void *arg);
externC EXPORT_SYMBOL void *wsncomm_register(const char *ip,
            CB_NEW_NODE cb_newNode,
            CB_NEW_FUNC cb_newFunc,
            CB_NEW_DATA cb_newData,
            CB_NODE_GONE cb_nodeGone,
            CB_SERVER_GONE cb_serverGone,
            void *arg);
externC EXPORT_SYMBOL int wsncomm_unregister(void *user);

externC EXPORT_SYMBOL const char *wsncomm_strerror(int errCode);

#endif // _LIBWSNCOMM_H_
