/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
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
#define  PANEL_STRING                     2       /* control type: string, callback function: (none) */
#define  PANEL_TEXTBOX                    3       /* control type: textBox, callback function: (none) */
#define  PANEL_CMD_DISCONNECT             4       /* control type: command, callback function: disconnect */
#define  PANEL_CMD_CONNECT                5       /* control type: command, callback function: connect */
#define  PANEL_STRIPCHART                 6       /* control type: strip, callback function: (none) */
#define  PANEL_QUITBUTTON                 7       /* control type: command, callback function: QuitCallback */
#define  PANEL_CMD_CLEAR                  8       /* control type: command, callback function: clear */
#define  PANEL_CMD_SEND                   9       /* control type: command, callback function: send */
#define  PANEL_TIMER                      10      /* control type: timer, callback function: timer */
#define  PANEL_BINARYSWITCH               11      /* control type: binary, callback function: (none) */
#define  PANEL_COMBO                      12      /* control type: string, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK clear(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK disconnect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK send(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
