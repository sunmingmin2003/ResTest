/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2018. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_cButtonStart               2       /* control type: command, callback function: PANEL_DAQ_START */
#define  PANEL_TABLE_MSG                  3       /* control type: table, callback function: OnTABLE_MSG */
#define  PANEL_cButtonPrint_3             4       /* control type: command, callback function: PANEL_Single_DAQ */
#define  PANEL_cButtonPrint_2             5       /* control type: command, callback function: PANEL_Print */
#define  PANEL_cButtonPrint               6       /* control type: command, callback function: PANEL_SaveScreen */
#define  PANEL_cButtonStop                7       /* control type: command, callback function: PANEL_DAQ_STOP */
#define  PANEL_TABLE_9                    8       /* control type: table, callback function: (none) */
#define  PANEL_DECORATION_11              9       /* control type: deco, callback function: (none) */
#define  PANEL_TABLE_8                    10      /* control type: table, callback function: (none) */
#define  PANEL_cButtomQuit                11      /* control type: command, callback function: PANEL_Quit */
#define  PANEL_DECORATION_8               12      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_6               13      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_9               14      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_7               15      /* control type: deco, callback function: (none) */
#define  PANEL_PIC_Unconnect              16      /* control type: picture, callback function: (none) */
#define  PANEL_PIC_Connect                17      /* control type: picture, callback function: (none) */
#define  PANEL_TXT_NetStatus              18      /* control type: textMsg, callback function: (none) */
#define  PANEL_TXT_NetStatus_2            19      /* control type: textMsg, callback function: (none) */
#define  PANEL_PIC_ConnectGrey            20      /* control type: picture, callback function: (none) */
#define  PANEL_TIMER_CMD_Process          21      /* control type: timer, callback function: PANEL_Timer_CMD_Process */
#define  PANEL_LED_1                      22      /* control type: LED, callback function: (none) */
#define  PANEL_LED                        23      /* control type: LED, callback function: (none) */
#define  PANEL_TABLE_5                    24      /* control type: table, callback function: (none) */
#define  PANEL_TABLE_4                    25      /* control type: table, callback function: (none) */
#define  PANEL_TABLE_3                    26      /* control type: table, callback function: (none) */
#define  PANEL_TABLE_2                    27      /* control type: table, callback function: (none) */
#define  PANEL_TABLE                      28      /* control type: table, callback function: (none) */
#define  PANEL_DECORATION_3               29      /* control type: deco, callback function: (none) */
#define  PANEL_STRING                     30      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG_4                  31      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  32      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  33      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    34      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_5               35      /* control type: deco, callback function: (none) */
#define  PANEL_COMMANDBUTTON              36      /* control type: command, callback function: PANEL_DEL_ALARM */
#define  PANEL_PICTURE                    37      /* control type: picture, callback function: (none) */
#define  PANEL_TIMER_AutoConnect          38      /* control type: timer, callback function: PANEL_AutoConnect */
#define  PANEL_TIMER_EnableRXD            39      /* control type: timer, callback function: PANEL_EnableRXD */
#define  PANEL_TIMER_NetStatus            40      /* control type: timer, callback function: PANEL_NetStatus */
#define  PANEL_DECORATION_10              41      /* control type: deco, callback function: (none) */
#define  PANEL_PICTUREBUTTON              42      /* control type: pictButton, callback function: PANEL_HV_Test */

#define  PANEL_10                         2
#define  PANEL_10_BTNQuit                 2       /* control type: command, callback function: PANEL_10_OnBTNQuit */
#define  PANEL_10_GRAPH1                  3       /* control type: graph, callback function: OnGraph */
#define  PANEL_10_cButtonRectScale        4       /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_10_cButtonYScale           5       /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_10_cButtonXScale           6       /* control type: pictButton, callback function: PANEL_BTNScale */
#define  PANEL_10_cBTNShortKey            7       /* control type: command, callback function: OnBTNShortKey */

#define  PANEL_11                         3
#define  PANEL_11_cButtonQuit             2       /* control type: command, callback function: PANEL_11_Quit */
#define  PANEL_11_GRAPH2                  3       /* control type: graph, callback function: OnGraph */
#define  PANEL_11_GRAPH1                  4       /* control type: graph, callback function: OnGraph */
#define  PANEL_11_cButtomPrint_2          5       /* control type: command, callback function: PANEL_11_Save_Result */
#define  PANEL_11_cButtomPrint            6       /* control type: command, callback function: PANEL_11_Print */
#define  PANEL_11_TABLE_9                 7       /* control type: table, callback function: (none) */
#define  PANEL_11_TABLE_8                 8       /* control type: table, callback function: (none) */
#define  PANEL_11_DECORATION_10           9       /* control type: deco, callback function: (none) */
#define  PANEL_11_DECORATION_9            10      /* control type: deco, callback function: (none) */
#define  PANEL_11_DECORATION_8            11      /* control type: deco, callback function: (none) */
#define  PANEL_11_DECORATION_7            12      /* control type: deco, callback function: (none) */
#define  PANEL_11_DECORATION_6            13      /* control type: deco, callback function: (none) */
#define  PANEL_11_TABLE_5                 14      /* control type: table, callback function: (none) */
#define  PANEL_11_STRING_4                15      /* control type: string, callback function: (none) */
#define  PANEL_11_STRING_3                16      /* control type: string, callback function: (none) */
#define  PANEL_11_STRING                  17      /* control type: string, callback function: (none) */
#define  PANEL_11_STRING_2                18      /* control type: string, callback function: (none) */
#define  PANEL_11_TABLE_2                 19      /* control type: table, callback function: (none) */
#define  PANEL_11_TEXTMSG_4               20      /* control type: textMsg, callback function: (none) */
#define  PANEL_11_TABLE                   21      /* control type: table, callback function: (none) */
#define  PANEL_11_TEXTMSG_3               22      /* control type: textMsg, callback function: (none) */
#define  PANEL_11_TEXTMSG                 23      /* control type: textMsg, callback function: (none) */
#define  PANEL_11_TEXTMSG_2               24      /* control type: textMsg, callback function: (none) */

#define  PANEL_12                         4
#define  PANEL_12_TEXTMSG                 2       /* control type: textMsg, callback function: (none) */
#define  PANEL_12_COMMANDBUTTON           3       /* control type: command, callback function: PANEL_12_Quit */

#define  PANEL_13                         5
#define  PANEL_13_TABLE_6                 2       /* control type: table, callback function: PANEL_7_SetResult */
#define  PANEL_13_TABLE_5                 3       /* control type: table, callback function: (none) */
#define  PANEL_13_TABLE_4                 4       /* control type: table, callback function: (none) */
#define  PANEL_13_PICTUREBUTTON_2         5       /* control type: pictButton, callback function: PANEL_13_PreTest */
#define  PANEL_13_PICTUREBUTTON           6       /* control type: pictButton, callback function: PANEL_13_Res_Test */
#define  PANEL_13_cButtonPrint_2          7       /* control type: command, callback function: PANEL_Print */
#define  PANEL_13_cButtonPrint            8       /* control type: command, callback function: PANEL_SaveScreen */
#define  PANEL_13_cButtonPrint_4          9       /* control type: command, callback function: PANEL_13_Stop_Test */
#define  PANEL_13_cButtonPrint_5          10      /* control type: command, callback function: PANEL_13_StartTestSec */
#define  PANEL_13_cButtonPrint_3          11      /* control type: command, callback function: PANEL_13_Start_Test */
#define  PANEL_13_DECORATION_9            12      /* control type: deco, callback function: (none) */
#define  PANEL_13_DECORATION_6            13      /* control type: deco, callback function: (none) */
#define  PANEL_13_COMMANDBUTTON           14      /* control type: command, callback function: PANEL_13_Exit */
#define  PANEL_13_TABLE_3                 15      /* control type: table, callback function: (none) */
#define  PANEL_13_TEXTMSG_4               16      /* control type: textMsg, callback function: (none) */
#define  PANEL_13_TEXTMSG_3               17      /* control type: textMsg, callback function: (none) */
#define  PANEL_13_TEXTMSG_2               18      /* control type: textMsg, callback function: (none) */
#define  PANEL_13_DECORATION_5            19      /* control type: deco, callback function: (none) */
#define  PANEL_13_TEXTMSG                 20      /* control type: textMsg, callback function: (none) */
#define  PANEL_13_DECORATION_3            21      /* control type: deco, callback function: (none) */
#define  PANEL_13_NUMERICSLIDE            22      /* control type: scale, callback function: (none) */
#define  PANEL_13_DECORATION_8            23      /* control type: deco, callback function: (none) */
#define  PANEL_13_PICTURE                 24      /* control type: picture, callback function: (none) */
#define  PANEL_13_TIMER                   25      /* control type: timer, callback function: PANEL_13_Insulation_Test1 */
#define  PANEL_13_TIMER_3                 26      /* control type: timer, callback function: PANEL_13_Progrocess */
#define  PANEL_13_NUMERIC_LRES            27      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_HRES            28      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_HV              29      /* control type: numeric, callback function: (none) */
#define  PANEL_13_NUMERIC_TIME            30      /* control type: numeric, callback function: (none) */

#define  PANEL_16                         6
#define  PANEL_16_cButtomQuit             2       /* control type: command, callback function: PANEL_16_QUIT */
#define  PANEL_16_COMMANDBUTTON_2         3       /* control type: command, callback function: PANEL_16_LoadCFG */
#define  PANEL_16_COMMANDBUTTON_6         4       /* control type: command, callback function: PANEL_16_Clear2 */
#define  PANEL_16_COMMANDBUTTON_3         5       /* control type: command, callback function: PANEL_16_Clear1 */
#define  PANEL_16_COMMANDBUTTON_7         6       /* control type: command, callback function: PANEL_16_SaveCFG */
#define  PANEL_16_COMMANDBUTTON           7       /* control type: command, callback function: PANEL_16_Set */
#define  PANEL_16_TABLE                   8       /* control type: table, callback function: OnTable */
#define  PANEL_16_TABLE_2                 9       /* control type: table, callback function: OnTable */
#define  PANEL_16_NUMERIC_4               10      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_3               11      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_2               12      /* control type: numeric, callback function: (none) */
#define  PANEL_16_DECORATION_8            13      /* control type: deco, callback function: (none) */
#define  PANEL_16_DECORATION_7            14      /* control type: deco, callback function: (none) */
#define  PANEL_16_NUMERIC                 15      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_12              16      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_11              17      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_10              18      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_9               19      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_8               20      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_7               21      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_6               22      /* control type: numeric, callback function: (none) */
#define  PANEL_16_NUMERIC_5               23      /* control type: numeric, callback function: (none) */
#define  PANEL_16_TEXTMSG_5               24      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_6               25      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_7               26      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_8               27      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG                 28      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_3               29      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_4               30      /* control type: textMsg, callback function: (none) */
#define  PANEL_16_TEXTMSG_2               31      /* control type: textMsg, callback function: (none) */

#define  PANEL_2                          7
#define  PANEL_2_IP                       2       /* control type: string, callback function: (none) */
#define  PANEL_2_cButtonQuit              3       /* control type: command, callback function: PANEL_2_ButtonQuit */
#define  PANEL_2_cButtonConnect           4       /* control type: command, callback function: PANEL_2_Connect */
#define  PANEL_2_TEXTMSG                  5       /* control type: textMsg, callback function: (none) */
#define  PANEL_2_PORT                     6       /* control type: numeric, callback function: (none) */
#define  PANEL_2_STR_NetStatus            7       /* control type: string, callback function: (none) */

#define  PANEL_3                          8
#define  PANEL_3_CHK2                     2       /* control type: radioButton, callback function: (none) */
#define  PANEL_3_CHK1                     3       /* control type: radioButton, callback function: (none) */
#define  PANEL_3_cButtonQuit              4       /* control type: command, callback function: PANEL_3_ButtonQuit */
#define  PANEL_3_NUMERIC                  5       /* control type: numeric, callback function: (none) */
#define  PANEL_3_NUMERIC_2                6       /* control type: numeric, callback function: (none) */
#define  PANEL_3_TEXTMSG                  7       /* control type: textMsg, callback function: (none) */
#define  PANEL_3_NUMERIC_3                8       /* control type: numeric, callback function: (none) */
#define  PANEL_3_TEXTMSG_2                9       /* control type: textMsg, callback function: (none) */
#define  PANEL_3_TEXTMSG_3                10      /* control type: textMsg, callback function: (none) */
#define  PANEL_3_DECORATION_7             11      /* control type: deco, callback function: (none) */
#define  PANEL_3_DECORATION_8             12      /* control type: deco, callback function: (none) */

#define  PANEL_4                          9
#define  PANEL_4_cButtomQuit              2       /* control type: command, callback function: PANEL_4_ButtomQuit */
#define  PANEL_4_COMMANDBUTTON_2          3       /* control type: command, callback function: OnModifyPassword */
#define  PANEL_4_COMMANDBUTTON            4       /* control type: command, callback function: Login */
#define  PANEL_4_PASSWORD_3               5       /* control type: string, callback function: (none) */
#define  PANEL_4_PASSWORD_2               6       /* control type: string, callback function: (none) */
#define  PANEL_4_PASSWORD                 7       /* control type: string, callback function: (none) */
#define  PANEL_4_DECORATION               8       /* control type: deco, callback function: (none) */
#define  PANEL_4_TEXTMSG                  9       /* control type: textMsg, callback function: (none) */

#define  PANEL_5                          10
#define  PANEL_5_PASSWORD                 2       /* control type: string, callback function: (none) */
#define  PANEL_5_COMMANDBUTTON_2          3       /* control type: command, callback function: OnCancel */
#define  PANEL_5_COMMANDBUTTON            4       /* control type: command, callback function: OnSuperAdminLogin */

#define  PANEL_6                          11
#define  PANEL_6_NUMERIC3                 2       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC1                 3       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC4                 4       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC2                 5       /* control type: numeric, callback function: (none) */
#define  PANEL_6_COMMANDBUTTON_2          6       /* control type: command, callback function: PANEL_6_Exit */
#define  PANEL_6_COMMANDBUTTON            7       /* control type: command, callback function: PABEL_6_SetCom */
#define  PANEL_6_NUMERIC_4                8       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_3                9       /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC_2                10      /* control type: numeric, callback function: (none) */
#define  PANEL_6_NUMERIC                  11      /* control type: numeric, callback function: (none) */

#define  PANEL_7                          12
#define  PANEL_7_TABLE_4                  2       /* control type: table, callback function: PANEL_7_SetResult */
#define  PANEL_7_TABLE_6                  3       /* control type: table, callback function: (none) */
#define  PANEL_7_PICTUREBUTTON_2          4       /* control type: pictButton, callback function: PANEL_7_PreTest */
#define  PANEL_7_TABLE_5                  5       /* control type: table, callback function: (none) */
#define  PANEL_7_PICTUREBUTTON            6       /* control type: pictButton, callback function: PANEL_7_HI_Test */
#define  PANEL_7_cButtonPrint_2           7       /* control type: command, callback function: PANEL_Print */
#define  PANEL_7_cButtonPrint_4           8       /* control type: command, callback function: PANEL_7_Stop_Test */
#define  PANEL_7_cButtonPrint_5           9       /* control type: command, callback function: PANEL_7_Auto_GND_Start */
#define  PANEL_7_cButtonPrint_3           10      /* control type: command, callback function: PANEL_7_Auto_Start */
#define  PANEL_7_cButtonPrint             11      /* control type: command, callback function: PANEL_SaveScreen */
#define  PANEL_7_DECORATION_10            12      /* control type: deco, callback function: (none) */
#define  PANEL_7_DECORATION_9             13      /* control type: deco, callback function: (none) */
#define  PANEL_7_DECORATION_6             14      /* control type: deco, callback function: (none) */
#define  PANEL_7_COMMANDBUTTON            15      /* control type: command, callback function: PANEL_7_Exit */
#define  PANEL_7_TABLE_3                  16      /* control type: table, callback function: (none) */
#define  PANEL_7_TEXTMSG_4                17      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG_3                18      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_TEXTMSG_2                19      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_DECORATION_5             20      /* control type: deco, callback function: (none) */
#define  PANEL_7_TEXTMSG                  21      /* control type: textMsg, callback function: (none) */
#define  PANEL_7_DECORATION_3             22      /* control type: deco, callback function: (none) */
#define  PANEL_7_NUMERICSLIDE             23      /* control type: scale, callback function: (none) */
#define  PANEL_7_COMMANDBUTTON_4          24      /* control type: command, callback function: PANEL_7_Close_Device */
#define  PANEL_7_DECORATION_8             25      /* control type: deco, callback function: (none) */
#define  PANEL_7_NUMERIC_TIME1_2          26      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_LC1_2            27      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_HC1_2            28      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_HV1_2            29      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_TIME1_1          30      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_LC1_1            31      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_HC1_1            32      /* control type: numeric, callback function: (none) */
#define  PANEL_7_NUMERIC_HV1_1            33      /* control type: numeric, callback function: (none) */
#define  PANEL_7_PICTURE                  34      /* control type: picture, callback function: (none) */
#define  PANEL_7_TIMER                    35      /* control type: timer, callback function: PANEL_7_Auto_Test */
#define  PANEL_7_TIMER_2                  36      /* control type: timer, callback function: PANEL_7_Progrocess */

#define  PANEL_8                          13
#define  PANEL_8_COMMANDBUTTON_2          2       /* control type: command, callback function: PANEL_8_PASS */
#define  PANEL_8_TABLE_MSG                3       /* control type: table, callback function: OnTABLE_MSG */
#define  PANEL_8_PICTUREBUTTON_2          4       /* control type: pictButton, callback function: PANEL_8_PreTest */
#define  PANEL_8_PICTUREBUTTON            5       /* control type: pictButton, callback function: PANEL_8_NestTest */
#define  PANEL_8_cButtonPrint_2           6       /* control type: command, callback function: PANEL_Print */
#define  PANEL_8_cButtonPrint             7       /* control type: command, callback function: PANEL_SaveScreen */
#define  PANEL_8_COMMANDBUTTON            8       /* control type: command, callback function: PANEL_8_Exit */
#define  PANEL_8_TABLE_3                  9       /* control type: table, callback function: PANEL_Table3 */
#define  PANEL_8_DECORATION_10            10      /* control type: deco, callback function: (none) */
#define  PANEL_8_DECORATION_9             11      /* control type: deco, callback function: (none) */
#define  PANEL_8_TEXTMSG_4                12      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_TEXTMSG_3                13      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_TEXTMSG_2                14      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_DECORATION_5             15      /* control type: deco, callback function: (none) */
#define  PANEL_8_TEXTMSG                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_DECORATION_3             17      /* control type: deco, callback function: (none) */
#define  PANEL_8_TABLE_4                  18      /* control type: table, callback function: (none) */
#define  PANEL_8_NUMERICMETER_2           19      /* control type: scale, callback function: (none) */
#define  PANEL_8_NUMERICMETER             20      /* control type: scale, callback function: (none) */
#define  PANEL_8_COMMANDBUTTON_3          21      /* control type: command, callback function: PANEL_8_UNPASS */
#define  PANEL_8_NUMERICSLIDE             22      /* control type: scale, callback function: (none) */
#define  PANEL_8_COMMANDBUTTON_6          23      /* control type: command, callback function: PANEL_8_Set_HI */
#define  PANEL_8_COMMANDBUTTON_5          24      /* control type: command, callback function: PANEL_8_Start_Test */
#define  PANEL_8_COMMANDBUTTON_4          25      /* control type: command, callback function: PANEL_8_Stop_Test */
#define  PANEL_8_DECORATION               26      /* control type: deco, callback function: (none) */
#define  PANEL_8_TEXTMSG_5                27      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_DECORATION_8             28      /* control type: deco, callback function: (none) */
#define  PANEL_8_TEXTMSG_6                29      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_NUMERIC                  30      /* control type: numeric, callback function: (none) */
#define  PANEL_8_PICTURE                  31      /* control type: picture, callback function: (none) */
#define  PANEL_8_TEXTMSG_7                32      /* control type: textMsg, callback function: (none) */
#define  PANEL_8_TIMER                    33      /* control type: timer, callback function: PANEL_8_Progress */

#define  PANEL_9                          14
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
#define  CTRLARRAY_3                      2
#define  CTRLARRAY_Scale                  3

     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_ITEM               3       /* callback function: ITEM_NetConnect */
#define  MENUBAR_MENU1_ITEM2              4       /* callback function: Item_DisconnectNet */
#define  MENUBAR_MENU2                    5
#define  MENUBAR_MENU2_ITEM               6       /* callback function: ITEM_DisplaySet */
#define  MENUBAR_MENU4                    7       /* callback function: Item_ChanSel */
#define  MENUBAR_MENU6                    8       /* callback function: Item_ComSet */
#define  MENUBAR_MENU5                    9
#define  MENUBAR_MENU5_ITEM1              10      /* callback function: ITEM_History */
#define  MENUBAR_MENU5_SEPARATOR          11
#define  MENUBAR_MENU5_ITEM2              12      /* callback function: Item_Alarm */
#define  MENUBAR_MENU5_ITEM               13      /* callback function: ITEM_RecordSet */


     /* Callback Prototypes: */

void CVICALLBACK Item_Alarm(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_ChanSel(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_ComSet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Item_DisconnectNet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_DisplaySet(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_History(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_NetConnect(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_RecordSet(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Login(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnBTNShortKey(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnModifyPassword(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSuperAdminLogin(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTABLE_MSG(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PABEL_6_SetCom(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_10_OnBTNQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_11_Print(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_11_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_11_Save_Result(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_12_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Insulation_Test1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_PreTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Progrocess(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Res_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Start_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_StartTestSec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_13_Stop_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_Clear1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_Clear2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_LoadCFG(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_QUIT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_SaveCFG(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_16_Set(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_2_ButtonQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_2_Connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_3_ButtonQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_4_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_6_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Auto_GND_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Auto_Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Auto_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Close_Device(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_HI_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_PreTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Progrocess(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_SetResult(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_7_Stop_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_NestTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_PASS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_PreTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Progress(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Set_HI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Start_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_Stop_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_8_UNPASS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_9_ButtomQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_9_SetSaveDir(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_AutoConnect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_BTNScale(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_DAQ_START(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_DAQ_STOP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_DEL_ALARM(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_EnableRXD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_HV_Test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_NetStatus(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Print(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_SaveScreen(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Single_DAQ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Table3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PANEL_Timer_CMD_Process(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
