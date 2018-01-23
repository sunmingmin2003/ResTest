/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OnPANEL */
#define  PANEL_TABLE_MSG                  2       /* control type: table, callback function: OnTABLE_MSG */
#define  PANEL_TREE_Channel               3       /* control type: tree, callback function: OnTREE_Channel */
#define  PANEL_cButtonReset               4       /* control type: command, callback function: PANEL_Reset */
#define  PANEL_TreeArrow                  5       /* control type: picture, callback function: (none) */
#define  PANEL_cButtonMute                6       /* control type: command, callback function: PANEL_Mute */
#define  PANEL_cButtomQuit                7       /* control type: command, callback function: PANEL_Quit */
#define  PANEL_GRAPH16                    8       /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH15                    9       /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH14                    10      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH13                    11      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH12                    12      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH11                    13      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH10                    14      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH9                     15      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH8                     16      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH7                     17      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH6                     18      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH5                     19      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH4                     20      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH3                     21      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH1                     22      /* control type: graph, callback function: OnGraph */
#define  PANEL_GRAPH2                     23      /* control type: graph, callback function: OnGraph */
#define  PANEL_DECORATION_2               24      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION                 25      /* control type: deco, callback function: (none) */
#define  PANEL_PIC_Unconnect              26      /* control type: picture, callback function: (none) */
#define  PANEL_PIC_Connect                27      /* control type: picture, callback function: (none) */
#define  PANEL_TXT_NetStatus_3            28      /* control type: textMsg, callback function: (none) */
#define  PANEL_TXT_NetStatus              29      /* control type: textMsg, callback function: (none) */
#define  PANEL_TXT_NetStatus_2            30      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_NetStatus            31      /* control type: timer, callback function: PANEL_NetStatus */
#define  PANEL_PIC_ConnectGrey            32      /* control type: picture, callback function: (none) */
#define  PANEL_TIMER_TREE_Channel         33      /* control type: timer, callback function: PANEL_Timer_Tree_Channel */
#define  PANEL_TIMER_AutoConnect          34      /* control type: timer, callback function: PANEL_AutoConnect */
#define  PANEL_cButtonRectScale           35      /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_cButtonYScale              36      /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_cButtonXScale              37      /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_cButtonSingleDisplay       38      /* control type: pictButton, callback function: PANEL_BTNSingleDisplay */
#define  PANEL_TIMER_EnableRXD            39      /* control type: timer, callback function: PANEL_EnableRXD */
#define  PANEL_TIMER_CMD_Process          40      /* control type: timer, callback function: PANEL_Timer_CMD_Process */
#define  PANEL_cBTNShortKey               41      /* control type: command, callback function: OnBTNShortKey */

#define  PANEL_10                         2
#define  PANEL_10_BTNQuit                 2       /* control type: command, callback function: PANEL_10_OnBTNQuit */
#define  PANEL_10_BTNSend_2               3       /* control type: command, callback function: PANEL_10_OnDeviceCMD */
#define  PANEL_10_BTNSend                 4       /* control type: command, callback function: PANEL_10_OnDebugCMD */
#define  PANEL_10_TEXTMSG_2               5       /* control type: textMsg, callback function: (none) */
#define  PANEL_10_RADIOGROUP_2            6       /* control type: tree, callback function: (none) */
#define  PANEL_10_RADIOGROUP              7       /* control type: tree, callback function: (none) */
#define  PANEL_10_RING1                   8       /* control type: ring, callback function: (none) */
#define  PANEL_10_RING2                   9       /* control type: ring, callback function: (none) */
#define  PANEL_10_RING6                   10      /* control type: ring, callback function: (none) */
#define  PANEL_10_RING3                   11      /* control type: ring, callback function: (none) */
#define  PANEL_10_RING5                   12      /* control type: ring, callback function: (none) */
#define  PANEL_10_RING4                   13      /* control type: ring, callback function: (none) */
#define  PANEL_10_DECORATION_3            14      /* control type: deco, callback function: (none) */
#define  PANEL_10_DECORATION_4            15      /* control type: deco, callback function: (none) */
#define  PANEL_10_DECORATION_2            16      /* control type: deco, callback function: (none) */
#define  PANEL_10_NUMERIC5                17      /* control type: numeric, callback function: (none) */
#define  PANEL_10_COMMANDBUTTON           18      /* control type: command, callback function: PANEL_10_Save */
#define  PANEL_10_NUMERIC4                19      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC3                20      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC10               21      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC11               22      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC7                23      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC9                24      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC8                25      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC6                26      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC22               27      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC21               28      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC20               29      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC19               30      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC18               31      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC15               32      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC17               33      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC16               34      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC14               35      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC1                36      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC2                37      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC12               38      /* control type: numeric, callback function: (none) */
#define  PANEL_10_NUMERIC13               39      /* control type: numeric, callback function: (none) */
#define  PANEL_10_STRING_2                40      /* control type: string, callback function: (none) */
#define  PANEL_10_STRING                  41      /* control type: string, callback function: (none) */

#define  PANEL_11                         3
#define  PANEL_11_IPCurrent               2       /* control type: string, callback function: (none) */
#define  PANEL_11_IPNew                   3       /* control type: string, callback function: (none) */
#define  PANEL_11_cButtonQuit             4       /* control type: command, callback function: PANEL_11_Quit */
#define  PANEL_11_BTNModifyIP             5       /* control type: command, callback function: PANEL_11_ModifyIP */
#define  PANEL_11_TEXTMSG_3               6       /* control type: textMsg, callback function: (none) */
#define  PANEL_11_NUMERIC_2               7       /* control type: numeric, callback function: (none) */
#define  PANEL_11_NUMERIC                 8       /* control type: numeric, callback function: (none) */

#define  PANEL_12                         4
#define  PANEL_12_TEXTMSG                 2       /* control type: textMsg, callback function: (none) */
#define  PANEL_12_COMMANDBUTTON           3       /* control type: command, callback function: PANEL_12_Quit */

#define  PANEL_13                         5
#define  PANEL_13_TREE                    2       /* control type: tree, callback function: PANEL_13_UpdateCalRatio */
#define  PANEL_13_NUMERIC_1               3       /* control type: numeric, callback function: (none) */
#define  PANEL_13_STR_NetStatus           4       /* control type: string, callback function: (none) */
#define  PANEL_13_BTN1                    5       /* control type: command, callback function: PANEL_13_AddFiberCalRatio */
#define  PANEL_13_BTN2                    6       /* control type: command, callback function: PANEL_13_DelFiberCalRatio */
#define  PANEL_13_TABLE                   7       /* control type: table, callback function: PANEL_13_OnTable */
#define  PANEL_13_cButtomQuit             8       /* control type: command, callback function: PANEL_13_ButtomQuit */
#define  PANEL_13_PICTURE                 9       /* control type: picture, callback function: (none) */
#define  PANEL_13_NUMERIC_5               10      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_4               11      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_2               12      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_3               13      /* control type: numeric, callback function: (none) */
#define  PANEL_13_DECORATION              14      /* control type: deco, callback function: (none) */
#define  PANEL_13_COMMANDBUTTON           15      /* control type: command, callback function: PANEL_13_Save */
#define  PANEL_13_TEXTMSG_1               16      /* control type: textMsg, callback function: (none) */
#define  PANEL_13_TEXTMSG_2               17      /* control type: textMsg, callback function: (none) */

#define  PANEL_14                         6
#define  PANEL_14_TABLE                   2       /* control type: table, callback function: (none) */
#define  PANEL_14_BTN2                    3       /* control type: command, callback function: PANEL_14_Cancel */
#define  PANEL_14_BTN1                    4       /* control type: command, callback function: PANEL_14_OK */

#define  PANEL_15                         7
#define  PANEL_15_BTN1                    2       /* control type: command, callback function: PANEL_15_OK */
#define  PANEL_15_BTN2                    3       /* control type: command, callback function: PANEL_15_Cancel */
#define  PANEL_15_TABLE                   4       /* control type: table, callback function: (none) */

#define  PANEL_16                         8
#define  PANEL_16_TREE                    2       /* control type: tree, callback function: PANEL_16_UpdateDelRegion */
#define  PANEL_16_STR_NetStatus           3       /* control type: string, callback function: (none) */
#define  PANEL_16_BTN1                    4       /* control type: command, callback function: PANEL_16_AddDelRegion */
#define  PANEL_16_BTN2                    5       /* control type: command, callback function: PANEL_16_DelDelFiberRegion */
#define  PANEL_16_TABLE                   6       /* control type: table, callback function: PANEL_16_OnTable */
#define  PANEL_16_cButtomQuit             7       /* control type: command, callback function: PANEL_16_Cancel */
#define  PANEL_16_PICTURE                 8       /* control type: picture, callback function: (none) */
#define  PANEL_16_COMMANDBUTTON           9       /* control type: command, callback function: PANEL_16_Save */
#define  PANEL_16_NUMERIC1                10      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC                 11      /* control type: numeric, callback function: (none) */
#define  PANEL_16_DECORATION              12      /* control type: deco, callback function: (none) */
#define  PANEL_16_TEXTMSG                 13      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_2               14      /* control type: textMsg, callback function: (none) */

#define  PANEL_17                         9
#define  PANEL_17_COMMANDBUTTON_2         2       /* control type: command, callback function: PANEL_17_Exit */
#define  PANEL_17_COMMANDBUTTON           3       /* control type: command, callback function: PANEL_17_Set */
#define  PANEL_17_STRING1                 4       /* control type: string, callback function: (none) */
#define  PANEL_17_STRING                  5       /* control type: string, callback function: (none) */
#define  PANEL_17_CHECKBOX                6       /* control type: radioButton, callback function: (none) */
#define  PANEL_17_NUMERIC                 7       /* control type: numeric, callback function: (none) */

#define  PANEL_2                          10
#define  PANEL_2_IP                       2       /* control type: string, callback function: (none) */
#define  PANEL_2_cButtonQuit              3       /* control type: command, callback function: PANEL_2_ButtonQuit */
#define  PANEL_2_cButtonConnect           4       /* control type: command, callback function: PANEL_2_Connect */
#define  PANEL_2_TEXTMSG                  5       /* control type: textMsg, callback function: (none) */
#define  PANEL_2_PORT                     6       /* control type: numeric, callback function: (none) */
#define  PANEL_2_STR_NetStatus            7       /* control type: string, callback function: (none) */

#define  PANEL_3                          11
#define  PANEL_3_CHK2                     2       /* control type: radioButton, callback function: (none) */
#define  PANEL_3_CHK3                     3       /* control type: radioButton, callback function: (none) */
#define  PANEL_3_CHK1                     4       /* control type: radioButton, callback function: (none) */
#define  PANEL_3_cButtonQuit              5       /* control type: command, callback function: PANEL_3_ButtonQuit */
#define  PANEL_3_NUMERIC                  6       /* control type: numeric, callback function: (none) */
#define  PANEL_3_NUMERIC_2                7       /* control type: numeric, callback function: (none) */
#define  PANEL_3_TEXTMSG                  8       /* control type: textMsg, callback function: (none) */

#define  PANEL_4                          12
#define  PANEL_4_cButtomQuit              2       /* control type: command, callback function: PANEL_4_ButtomQuit */
#define  PANEL_4_COMMANDBUTTON_2          3       /* control type: command, callback function: OnModifyPassword */
#define  PANEL_4_COMMANDBUTTON            4       /* control type: command, callback function: Login */
#define  PANEL_4_PASSWORD_3               5       /* control type: string, callback function: (none) */
#define  PANEL_4_PASSWORD_2               6       /* control type: string, callback function: (none) */
#define  PANEL_4_PASSWORD                 7       /* control type: string, callback function: (none) */
#define  PANEL_4_DECORATION               8       /* control type: deco, callback function: (none) */
#define  PANEL_4_TEXTMSG                  9       /* control type: textMsg, callback function: (none) */

#define  PANEL_5                          13
#define  PANEL_5_PASSWORD                 2       /* control type: string, callback function: (none) */
#define  PANEL_5_COMMANDBUTTON_2          3       /* control type: command, callback function: OnCancel */
#define  PANEL_5_COMMANDBUTTON            4       /* control type: command, callback function: OnSuperAdminLogin */

#define  PANEL_6                          14
#define  PANEL_6_TREE                     2       /* control type: tree, callback function: PANEL_6_UpdateAlarm */
#define  PANEL_6_NUMERIC_1                3       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_2                4       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_5                5       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_3                6       /* control type: numeric, callback function: (none) */
#define  PANEL_6_CHECKBOX                 7       /* control type: radioButton, callback function: (none) */
#define  PANEL_6_STR_NetStatus            8       /* control type: string, callback function: (none) */
#define  PANEL_6_NUMERIC_6                9       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_4                10      /* control type: numeric, callback function: (none) */
#define  PANEL_6_CHECKBOX_1               11      /* control type: radioButton, callback function: (none) */
#define  PANEL_6_BTNAddRelayRegion        12      /* control type: command, callback function: PANEL_6_AddAlarmRegion */
#define  PANEL_6_BTNDelRelayRegion        13      /* control type: command, callback function: PANEL_6_DeleteAlarmRegion */
#define  PANEL_6_TABLE                    14      /* control type: table, callback function: PANEL_6_OnTable */
#define  PANEL_6_cButtomQuit              15      /* control type: command, callback function: PANEL_6_ButtomQuit */
#define  PANEL_6_PICTURE                  16      /* control type: picture, callback function: (none) */
#define  PANEL_6_DECORATION               17      /* control type: deco, callback function: (none) */
#define  PANEL_6_TEXTMSG                  18      /* control type: textMsg, callback function: (none) */
#define  PANEL_6_COMMANDBUTTON            19      /* control type: command, callback function: PANEL_6_Save */
#define  PANEL_6_DECORATION_2             20      /* control type: deco, callback function: (none) */
#define  PANEL_6_TEXTMSG_2                21      /* control type: textMsg, callback function: (none) */
#define  PANEL_6_TIMER_TREE               22      /* control type: timer, callback function: PANEL_6_Timer_Tree */
#define  PANEL_6_TEXTMSG_3                23      /* control type: textMsg, callback function: (none) */
#define  PANEL_6_STRING                   24      /* control type: string, callback function: (none) */

#define  PANEL_7                          15
#define  PANEL_7_NUMERIC_2                2       /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_3                3       /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_4                4       /* control type: numeric, callback function: PANEL_7_UpdateCalParameter */
#define  PANEL_7_STR_NetStatus            5       /* control type: string, callback function: (none) */
#define  PANEL_7_NUMERIC_5                6       /* control type: numeric, callback function: PANEL_7_UpdateCalParameter */
#define  PANEL_7_NUMERIC_6                7       /* control type: numeric, callback function: PANEL_7_UpdateCalTime */
#define  PANEL_7_NUMERIC_7                8       /* control type: numeric, callback function: PANEL_7_UpdateCalTime */
#define  PANEL_7_RING                     9       /* control type: ring, callback function: (none) */
#define  PANEL_7_NUMERIC_11               10      /* control type: numeric, callback function: (none) */
#define  PANEL_7_COMMANDBUTTON_6          11      /* control type: command, callback function: OnSaveCalPara */
#define  PANEL_7_COMMANDBUTTON            12      /* control type: command, callback function: PANEL_7_Save */
#define  PANEL_7_cButtomQuit              13      /* control type: command, callback function: PANEL_7_ButtomQuit */
#define  PANEL_7_PICTURE                  14      /* control type: picture, callback function: (none) */
#define  PANEL_7_TREE                     15      /* control type: tree, callback function: PANEL_7_UpdateChannel */
#define  PANEL_7_DECORATION               16      /* control type: deco, callback function: (none) */
#define  PANEL_7_TEXTMSG_5                17      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_DECORATION_2             18      /* control type: deco, callback function: (none) */
#define  PANEL_7_TEXTMSG_6                19      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG_4                20      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG                  21      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_DECORATION_5             22      /* control type: deco, callback function: (none) */
#define  PANEL_7_TEXTMSG_9                23      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG_2                24      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG_8                25      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_RADIOGROUP               26      /* control type: tree, callback function: PANEL_7_SelCalPara */
#define  PANEL_7_TIMER_TREE               27      /* control type: timer, callback function: PANEL_7_Timer_Tree_Channel */

#define  PANEL_8                          16
#define  PANEL_8_TREE                     2       /* control type: tree, callback function: PANEL_8_UpdateRelayAssign */
#define  PANEL_8_TABLE                    3       /* control type: table, callback function: PANEL_8_OnTable */
#define  PANEL_8_RING_1                   4       /* control type: ring, callback function: (none) */
#define  PANEL_8_RING_2                   5       /* control type: ring, callback function: (none) */
#define  PANEL_8_cButtomQuit              6       /* control type: command, callback function: PANEL_8_ButtomQuit */
#define  PANEL_8_STR_NetStatus            7       /* control type: string, callback function: (none) */
#define  PANEL_8_PICTURE                  8       /* control type: picture, callback function: (none) */
#define  PANEL_8_BTNDelRelayRegion        9       /* control type: command, callback function: PANEL_8_DelRelayRegion */
#define  PANEL_8_BTNAddRelayRegion        10      /* control type: command, callback function: PANEL_8_AddRelayRegion */
#define  PANEL_8_NUMERIC_2                11      /* control type: numeric, callback function: (none) */
#define  PANEL_8_NUMERIC                  12      /* control type: numeric, callback function: (none) */
#define  PANEL_8_DECORATION               13      /* control type: deco, callback function: (none) */
#define  PANEL_8_TEXTMSG                  14      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_TEXTMSG_2                15      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_TIMER_TREE               16      /* control type: timer, callback function: PANEL_8_Timer_Tree */
#define  PANEL_8_COMMANDBUTTON            17      /* control type: command, callback function: PANEL_8_Save */

#define  PANEL_9                          17
#define  PANEL_9_CHK_SaveDeviceInfo       2       /* control type: radioButton, callback function: (none) */
#define  PANEL_9_CHK_SaveAlarm            3       /* control type: radioButton, callback function: (none) */
#define  PANEL_9_CHK_SaveData             4       /* control type: radioButton, callback function: (none) */
#define  PANEL_9_cButtomQuit              5       /* control type: command, callback function: PANEL_9_ButtomQuit */
#define  PANEL_9_NUMERIC                  6       /* control type: numeric, callback function: (none) */
#define  PANEL_9_DECORATION_2             7       /* control type: deco, callback function: (none) */
#define  PANEL_9_cBTNSetDir               8       /* control type: command, callback function: PANEL_9_SetSaveDir */
#define  PANEL_9_STRING                   9       /* control type: string, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1
#define  CTRLARRAY_2                      2
#define  CTRLARRAY_3                      3
#define  CTRLARRAY_Scale                  4

     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_ITEM               3       /* callback function: ITEM_NetConnect */
#define  MENUBAR_MENU1_ITEM2              4       /* callback function: Item_DisconnectNet */
#define  MENUBAR_MENU2                    5
#define  MENUBAR_MENU2_ITEM               6       /* callback function: ITEM_DisplaySet */
#define  MENUBAR_MENU3                    7
#define  MENUBAR_MENU3_ITEM               8       /* callback function: ITEM_Login */
#define  MENUBAR_MENU3_ITEM1              9       /* callback function: ITEM_Logoff */
#define  MENUBAR_MENU4                    10
#define  MENUBAR_MENU4_ITEM2              11      /* callback function: Item_CalParameterSet */
#define  MENUBAR_MENU4_SEPARATOR_4        12
#define  MENUBAR_MENU4_ITEM5              13      /* callback function: Item_CalRatio */
#define  MENUBAR_MENU4_ITEM6              14      /* callback function: Item_DelFiberRegion */
#define  MENUBAR_MENU4_SEPARATOR_2        15
#define  MENUBAR_MENU4_ITEM3              16      /* callback function: ITEM_AlarmAssign */
#define  MENUBAR_MENU4_ITEM4              17      /* callback function: Item_RelayAssign */
#define  MENUBAR_MENU4_SEPARATOR_3        18
#define  MENUBAR_MENU4_ITEM1              19      /* callback function: ITEM_SetDefault */
#define  MENUBAR_MENU5                    20
#define  MENUBAR_MENU5_ITEM               21      /* callback function: ITEM_RecordSet */
#define  MENUBAR_MENU5_SEPARATOR          22
#define  MENUBAR_MENU5_ITEM1              23      /* callback function: ITEM_History */
#define  MENUBAR_MENU5_ITEM2              24      /* callback function: Item_Alarm */
#define  MENUBAR_MENU6                    25
#define  MENUBAR_MENU6_ITEM3              26      /* callback function: ITEM_DeviceIP */
#define  MENUBAR_MENU6_ITEM1              27      /* callback function: ITEM_Support */
#define  MENUBAR_MENU6_ITEM2              28
#define  MENUBAR_MENU6_ITEM4              29      /* callback function: ITEM_Quit */


     /* Callback Prototypes: */

void CVICALLBACK Item_Alarm(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_AlarmAssign(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_CalParameterSet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_CalRatio(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_DelFiberRegion(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_DeviceIP(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_DisconnectNet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_DisplaySet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_History(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Login(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Logoff(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_NetConnect(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Quit(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_RecordSet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_RelayAssign(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_SetDefault(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Support(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Login(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnBTNShortKey(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnModifyPassword(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPANEL(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveCalPara(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSuperAdminLogin(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTABLE_MSG(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTREE_Channel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_10_OnBTNQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_10_OnDebugCMD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_10_OnDeviceCMD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_10_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_11_ModifyIP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_11_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_12_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_AddFiberCalRatio(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_DelFiberCalRatio(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_OnTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_UpdateCalRatio(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_14_Cancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_14_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_15_Cancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_15_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_AddDelRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_Cancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_DelDelFiberRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_OnTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_UpdateDelRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_17_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_17_Set(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_2_ButtonQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_2_Connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_3_ButtonQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_4_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_AddAlarmRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_DeleteAlarmRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_OnTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_Timer_Tree(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_UpdateAlarm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_SelCalPara(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Timer_Tree_Channel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_UpdateCalParameter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_UpdateCalTime(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_UpdateChannel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_AddRelayRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_DelRelayRegion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_OnTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Timer_Tree(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_UpdateRelayAssign(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_9_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_9_SetSaveDir(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_AutoConnect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_BTNScale(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_BTNSingleDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_EnableRXD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Mute(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_NetStatus(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Reset(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Timer_CMD_Process(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Timer_Tree_Channel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
