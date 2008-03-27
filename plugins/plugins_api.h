/*
 * $Id$
 *
 * Copyright (c) 2008 Vyacheslav Frolov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * $Log$
 * Revision 1.1  2008/03/26 08:38:37  vfrolov
 * Initial revision
 *
 *
 */

#ifndef _PLUGINS_API_H
#define _PLUGINS_API_H

#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************/
#define HUB_MSG_UNION_TYPE_MASK    0xF000
#define HUB_MSG_UNION_TYPE_NONE    0x0000
#define HUB_MSG_UNION_TYPE_BUF     0x1000
#define HUB_MSG_UNION_TYPE_VAL     0x2000
/*******************************************************************/
#define HUB_MSG_TYPE_EMPTY         (HUB_MSG_UNION_TYPE_NONE  |   0)
#define HUB_MSG_TYPE_LINE_DATA     (HUB_MSG_UNION_TYPE_BUF   |   1)
#define HUB_MSG_TYPE_CONNECT       (HUB_MSG_UNION_TYPE_VAL   |   2)
#define HUB_MSG_TYPE_MODEM_STATUS  (HUB_MSG_UNION_TYPE_VAL   |   3)
#define HUB_MSG_TYPE_COM_ERRORS    (HUB_MSG_UNION_TYPE_VAL   |   4)
#define HUB_MSG_TYPE_COM_FUNCTION  (HUB_MSG_UNION_TYPE_VAL   |   5)
/*******************************************************************/
typedef struct _HUB_MSG {
  WORD type;
  union {
    struct {
      BYTE *pBuf;
      DWORD size;
    } buf;
    DWORD val;
  } u;
} HUB_MSG;
/*******************************************************************/
DECLARE_HANDLE(HHUB);
DECLARE_HANDLE(HMASTERPORT);
/*******************************************************************/
typedef BYTE *(CALLBACK ROUTINE_BUF_ALLOC)(
        DWORD size);
typedef VOID (CALLBACK ROUTINE_BUF_FREE)(
        BYTE *pBuf);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_REPLACE_BUF)(
        HUB_MSG *pMsg,
        WORD type,
        const BYTE *pSrc,
        DWORD sizeSrc);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_INSERT_BUF)(
        HUB_MSG *pPrevMsg,
        WORD type,
        const BYTE *pSrc,
        DWORD sizeSrc);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_REPLACE_VAL)(
        HUB_MSG *pMsg,
        WORD type,
        DWORD val);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_INSERT_VAL)(
        HUB_MSG *pMsg,
        WORD type,
        DWORD val);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_REPLACE_NONE)(
        HUB_MSG *pMsg,
        WORD type);
typedef HUB_MSG *(CALLBACK ROUTINE_MSG_INSERT_NONE)(
        HUB_MSG *pMsg,
        WORD type);
typedef int (CALLBACK ROUTINE_NUM_PORTS)(
        HHUB hHub);
typedef const char *(CALLBACK ROUTINE_PORT_NAME_A)(
        HHUB hHub,
        int n);
typedef void (CALLBACK ROUTINE_ON_XOFF)(
        HHUB hHub,
        HMASTERPORT hMasterPort);
typedef void (CALLBACK ROUTINE_ON_XON)(
        HHUB hHub,
        HMASTERPORT hMasterPort);
typedef void (CALLBACK ROUTINE_ON_READ)(
        HHUB hHub,
        HMASTERPORT hMasterPort,
        HUB_MSG *pMsg);
/*******************************************************************/
typedef struct _HUB_ROUTINES_A {
  size_t size;
  ROUTINE_BUF_ALLOC *pBufAlloc;
  ROUTINE_BUF_FREE *pBufFree;
  ROUTINE_MSG_REPLACE_BUF *pMsgReplaceBuf;
  ROUTINE_MSG_INSERT_BUF *pMsgInsertBuf;
  ROUTINE_MSG_REPLACE_VAL *pMsgReplaceVal;
  ROUTINE_MSG_INSERT_VAL *pMsgInsertVal;
  ROUTINE_MSG_REPLACE_NONE *pMsgReplaceNone;
  ROUTINE_MSG_INSERT_NONE *pMsgInsertNone;
  ROUTINE_NUM_PORTS *pNumPorts;
  ROUTINE_PORT_NAME_A *pPortName;
  ROUTINE_ON_XOFF *pOnXoff;
  ROUTINE_ON_XON *pOnXon;
  ROUTINE_ON_READ *pOnRead;
} HUB_ROUTINES_A;
/*******************************************************************/
typedef enum _PLUGIN_TYPE {
  PLUGIN_TYPE_INVALID,
  PLUGIN_TYPE_FILTER,
  PLUGIN_TYPE_PORT,
} PLUGIN_TYPE;
/*******************************************************************/
DECLARE_HANDLE(HCONFIG);
/*******************************************************************/
typedef struct _PLUGIN_ABOUT_A {
  size_t size;
  const char *pName;
  const char *pCopyright;
  const char *pLicense;
  const char *pDescription;
} PLUGIN_ABOUT_A;
/*******************************************************************/
typedef PLUGIN_TYPE (CALLBACK PLUGIN_GET_TYPE)();
typedef const PLUGIN_ABOUT_A *(CALLBACK PLUGIN_GET_ABOUT_A)();
typedef void (CALLBACK PLUGIN_HELP_A)(
        const char *pProgPath);
typedef HCONFIG (CALLBACK PLUGIN_CONFIG_START)();
typedef BOOL (CALLBACK PLUGIN_CONFIG_A)(
        HCONFIG hConfig,
        const char *pArg);
typedef void (CALLBACK PLUGIN_CONFIG_STOP)(
        HCONFIG hConfig);
/*******************************************************************/
#define COMMON_PLUGIN_ROUTINES_A \
  size_t size; \
  PLUGIN_GET_TYPE *pGetPluginType; \
  PLUGIN_GET_ABOUT_A *pGetPluginAbout; \
  PLUGIN_HELP_A *pHelp; \
  PLUGIN_CONFIG_START *pConfigStart; \
  PLUGIN_CONFIG_A *pConfig; \
  PLUGIN_CONFIG_STOP *pConfigStop; \
/*******************************************************************/
typedef struct _PLUGIN_ROUTINES_A {
  COMMON_PLUGIN_ROUTINES_A
} PLUGIN_ROUTINES_A;
/*******************************************************************/
#define PLUGIN_INIT_PROC_NAME "Init"
#define PLUGIN_INIT_PROC_NAME_A PLUGIN_INIT_PROC_NAME "A"
/*******************************************************************/
typedef const PLUGIN_ROUTINES_A *const *(CALLBACK PLUGIN_INIT_A)(
        const HUB_ROUTINES_A *pHubRoutines);
/*******************************************************************/
DECLARE_HANDLE(HFILTER);
/*******************************************************************/
typedef HFILTER (CALLBACK FILTER_CREATE_A)(
        HCONFIG hConfig,
        int argc,
        const char *const argv[]);
typedef BOOL (CALLBACK FILTER_INIT)(
        HFILTER hFilter,
        HHUB hHub);
typedef BOOL (CALLBACK FILTER_IN_METHOD)(
        HFILTER hFilter,
        int nFromPort,
        HUB_MSG *pInMsg,
        HUB_MSG **ppEchoMsg);
typedef BOOL (CALLBACK FILTER_OUT_METHOD)(
        HFILTER hFilter,
        int nFromPort,
        int nToPort,
        HUB_MSG *pOutMsg);
/*******************************************************************/
typedef struct _FILTER_ROUTINES_A {
  COMMON_PLUGIN_ROUTINES_A
  FILTER_CREATE_A *pCreate;
  FILTER_INIT *pInit;
  FILTER_IN_METHOD *pInMethod;
  FILTER_OUT_METHOD *pOutMethod;
} FILTER_ROUTINES_A;
/*******************************************************************/
DECLARE_HANDLE(HPORT);
/*******************************************************************/
typedef HPORT (CALLBACK PORT_CREATE_A)(
        HCONFIG hConfig,
        const char *pPath);
typedef const char *(CALLBACK PORT_GET_NAME_A)(
        HPORT hPort);
typedef void (CALLBACK PORT_SET_NAME_A)(
        HPORT hPort,
        const char *pName);
typedef BOOL (CALLBACK PORT_INIT)(
        HPORT hPort,
        HMASTERPORT hMasterPort,
        HHUB hHub);
typedef BOOL (CALLBACK PORT_START)(
        HPORT hPort);
typedef BOOL (CALLBACK PORT_WRITE)(
        HPORT hPort,
        HUB_MSG *pMsg);
typedef void (CALLBACK PORT_ADD_XOFF)(
        HPORT hPort);
typedef void (CALLBACK PORT_ADD_XON)(
        HPORT hPort);
typedef void (CALLBACK PORT_LOST_REPORT)(
        HPORT hPort);
/*******************************************************************/
typedef struct _PORT_ROUTINES_A {
  COMMON_PLUGIN_ROUTINES_A
  PORT_CREATE_A *pCreate;
  PORT_GET_NAME_A *pGetPortName;
  PORT_SET_NAME_A *pSetPortName;
  PORT_INIT *pInit;
  PORT_START *pStart;
  PORT_WRITE *pWrite;
  PORT_ADD_XOFF *pAddXoff;
  PORT_ADD_XON *pAddXon;
  PORT_LOST_REPORT *pLostReport;
} PORT_ROUTINES_A;
/*******************************************************************/
#define ROUTINE_GET(pStruct, pRoutine) \
        ((((BYTE *)(&(pStruct)->pRoutine + 1)) <= ((BYTE *)(pStruct) + (pStruct)->size)) \
          ? (pStruct)->pRoutine \
          : NULL)

#define ROUTINE_IS_VALID(pStruct, pRoutine) \
        (ROUTINE_GET(pStruct, pRoutine) != NULL)
/*******************************************************************/

#ifdef  __cplusplus
}
#endif

#endif  /* _PLUGINS_API_H */