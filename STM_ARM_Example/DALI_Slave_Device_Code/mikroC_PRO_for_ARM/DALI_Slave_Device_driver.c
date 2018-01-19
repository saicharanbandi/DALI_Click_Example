#include "DALI_Slave_Device_objects.h"
#include "DALI_Slave_Device_resources.h"
#include "built_in.h"


// TFT module connections
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;
// End TFT module connections

// Touch Panel module connections
sbit DRIVEX_LEFT at GPIOB_ODR.B1;
sbit DRIVEX_RIGHT at GPIOB_ODR.B8;
sbit DRIVEY_UP at GPIOB_ODR.B9;
sbit DRIVEY_DOWN at GPIOB_ODR.B0;
// End Touch Panel module connections

// Global variables
unsigned int Xcoord, Ycoord;
const ADC_THRESHOLD = 1500;
char PenDown;
void *PressedObject;
int PressedObjectType;
unsigned int caption_length, caption_height;
unsigned int display_width, display_height;

int _object_count;
unsigned short object_pressed;
TLabel *local_label;
TLabel *exec_label;
int label_order;
TBox *local_box;
TBox *exec_box;
int box_order;

static void InitializeTouchPanel() {
  TFT_Init(320, 240);

}


/////////////////////////
  TScreen*  CurrentScreen;

  TScreen                Screen1;
  TBox                   lblAnsBoxClear;
  TBox                   lblCmdBoxClear;
  TLabel                 lblReceivedCommand;
char lblReceivedCommand_Caption[17] = "Received Command";

  TLabel                 lblCommandAddr;
char lblCommandAddr_Caption[8] = "Address";

  TLabel                 lblCommandCMD;
char lblCommandCMD_Caption[4] = "Cmd";

  TLabel                 * const code Screen1_Labels[3]=
         {
         &lblReceivedCommand,  
         &lblCommandAddr,      
         &lblCommandCMD        
         };
  TBox                   * const code Screen1_Boxes[2]=
         {
         &lblAnsBoxClear,      
         &lblCmdBoxClear       
         };




static void InitializeObjects() {
  Screen1.Color                     = 0x0003;
  Screen1.Width                     = 320;
  Screen1.Height                    = 240;
  Screen1.LabelsCount               = 3;
  Screen1.Labels                    = Screen1_Labels;
  Screen1.BoxesCount                = 2;
  Screen1.Boxes                     = Screen1_Boxes;
  Screen1.ObjectsCount              = 5;


  lblAnsBoxClear.OwnerScreen     = &Screen1;
  lblAnsBoxClear.Order           = 0;
  lblAnsBoxClear.Left            = 6;
  lblAnsBoxClear.Top             = 185;
  lblAnsBoxClear.Width           = 174;
  lblAnsBoxClear.Height          = 28;
  lblAnsBoxClear.Pen_Width       = 1;
  lblAnsBoxClear.Pen_Color       = 0x0000;
  lblAnsBoxClear.Visible         = 1;
  lblAnsBoxClear.Active          = 1;
  lblAnsBoxClear.Transparent     = 1;
  lblAnsBoxClear.Gradient        = 0;
  lblAnsBoxClear.Gradient_Orientation    = 0;
  lblAnsBoxClear.Gradient_Start_Color    = 0xFFFF;
  lblAnsBoxClear.Gradient_End_Color      = 0xC618;
  lblAnsBoxClear.Color           = 0x0003;
  lblAnsBoxClear.PressColEnabled     = 0;
  lblAnsBoxClear.Press_Color     = 0x8410;
  lblAnsBoxClear.OnUpPtr         = 0;
  lblAnsBoxClear.OnDownPtr       = 0;
  lblAnsBoxClear.OnClickPtr      = 0;
  lblAnsBoxClear.OnPressPtr      = 0;

  lblCmdBoxClear.OwnerScreen     = &Screen1;
  lblCmdBoxClear.Order           = 1;
  lblCmdBoxClear.Left            = 3;
  lblCmdBoxClear.Top             = 29;
  lblCmdBoxClear.Width           = 174;
  lblCmdBoxClear.Height          = 28;
  lblCmdBoxClear.Pen_Width       = 1;
  lblCmdBoxClear.Pen_Color       = 0x0000;
  lblCmdBoxClear.Visible         = 1;
  lblCmdBoxClear.Active          = 1;
  lblCmdBoxClear.Transparent     = 1;
  lblCmdBoxClear.Gradient        = 0;
  lblCmdBoxClear.Gradient_Orientation    = 0;
  lblCmdBoxClear.Gradient_Start_Color    = 0xFFFF;
  lblCmdBoxClear.Gradient_End_Color      = 0xC618;
  lblCmdBoxClear.Color           = 0x0003;
  lblCmdBoxClear.PressColEnabled     = 0;
  lblCmdBoxClear.Press_Color     = 0x8410;
  lblCmdBoxClear.OnUpPtr         = 0;
  lblCmdBoxClear.OnDownPtr       = 0;
  lblCmdBoxClear.OnClickPtr      = 0;
  lblCmdBoxClear.OnPressPtr      = 0;

  lblReceivedCommand.OwnerScreen     = &Screen1;
  lblReceivedCommand.Order          = 2;
  lblReceivedCommand.Left           = 10;
  lblReceivedCommand.Top            = 9;
  lblReceivedCommand.Width          = 117;
  lblReceivedCommand.Height         = 17;
  lblReceivedCommand.Visible        = 1;
  lblReceivedCommand.Active         = 1;
  lblReceivedCommand.Caption        = lblReceivedCommand_Caption;
  lblReceivedCommand.FontName       = Tahoma13x16_Regular;
  lblReceivedCommand.Font_Color     = 0x07FF;
  lblReceivedCommand.OnUpPtr         = 0;
  lblReceivedCommand.OnDownPtr       = 0;
  lblReceivedCommand.OnClickPtr      = 0;
  lblReceivedCommand.OnPressPtr      = 0;

  lblCommandAddr.OwnerScreen     = &Screen1;
  lblCommandAddr.Order          = 3;
  lblCommandAddr.Left           = 10;
  lblCommandAddr.Top            = 33;
  lblCommandAddr.Width          = 47;
  lblCommandAddr.Height         = 17;
  lblCommandAddr.Visible        = 1;
  lblCommandAddr.Active         = 1;
  lblCommandAddr.Caption        = lblCommandAddr_Caption;
  lblCommandAddr.FontName       = Tahoma13x16_Regular;
  lblCommandAddr.Font_Color     = 0x07FF;
  lblCommandAddr.OnUpPtr         = 0;
  lblCommandAddr.OnDownPtr       = 0;
  lblCommandAddr.OnClickPtr      = 0;
  lblCommandAddr.OnPressPtr      = 0;

  lblCommandCMD.OwnerScreen     = &Screen1;
  lblCommandCMD.Order          = 4;
  lblCommandCMD.Left           = 64;
  lblCommandCMD.Top            = 33;
  lblCommandCMD.Width          = 27;
  lblCommandCMD.Height         = 17;
  lblCommandCMD.Visible        = 1;
  lblCommandCMD.Active         = 1;
  lblCommandCMD.Caption        = lblCommandCMD_Caption;
  lblCommandCMD.FontName       = Tahoma13x16_Regular;
  lblCommandCMD.Font_Color     = 0x07FF;
  lblCommandCMD.OnUpPtr         = 0;
  lblCommandCMD.OnDownPtr       = 0;
  lblCommandCMD.OnClickPtr      = 0;
  lblCommandCMD.OnPressPtr      = 0;
}

static char IsInsideObject (unsigned int X, unsigned int Y, unsigned int Left, unsigned int Top, unsigned int Width, unsigned int Height) { // static
  if ( (Left<= X) && (Left+ Width - 1 >= X) &&
       (Top <= Y)  && (Top + Height - 1 >= Y) )
    return 1;
  else
    return 0;
}


#define GetLabel(index)               CurrentScreen->Labels[index]
#define GetBox(index)                 CurrentScreen->Boxes[index]

 void DeleteTrailingSpaces(char* str){
   char i;
   i = 0;
   while(1) {
   if(str[0] == ' ') {
      for(i = 0; i < strlen(str); i++) {
       str[i] = str[i+1];
      }
   }
   else
     break;
  }
 }

void DrawLabel(TLabel *ALabel) {
  if (ALabel->Visible == 1) {
    TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text(ALabel->Caption, ALabel->Left, ALabel->Top);
  }
}

void DrawBox(TBox *ABox) {
  if (ABox->Visible == 1) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(ABox->Transparent, ABox->Press_Color, ABox->Gradient, ABox->Gradient_Orientation, ABox->Gradient_End_Color, ABox->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(ABox->Transparent, ABox->Color, ABox->Gradient, ABox->Gradient_Orientation, ABox->Gradient_Start_Color, ABox->Gradient_End_Color);
    }
    TFT_Set_Pen(ABox->Pen_Color, ABox->Pen_Width);
    TFT_Rectangle(ABox->Left, ABox->Top, ABox->Left + ABox->Width - 1, ABox->Top + ABox->Height - 1);
  }
}

void DrawScreen(TScreen *aScreen) {
 int order;
  unsigned short label_idx;
  TLabel *local_label;
  unsigned short box_idx;
  TBox *local_box;
  char save_bled, save_bled_direction;

  object_pressed = 0;
  order = 0;
  label_idx = 0;
  box_idx = 0;
  CurrentScreen = aScreen;

  if ((display_width != CurrentScreen->Width) || (display_height != CurrentScreen->Height)) {
    save_bled = TFT_BLED;
    TFT_BLED           = 0;
    TFT_Init(CurrentScreen->Width, CurrentScreen->Height);
    TFT_Fill_Screen(CurrentScreen->Color);
    display_width = CurrentScreen->Width;
    display_height = CurrentScreen->Height;
    TFT_BLED           = save_bled;
  }
  else
    TFT_Fill_Screen(CurrentScreen->Color);


  while (order < CurrentScreen->ObjectsCount) {
    if (label_idx < CurrentScreen->LabelsCount) {
      local_label = GetLabel(label_idx);
      if (order == local_label->Order) {
        label_idx++;
        order++;
        DrawLabel(local_label);
      }
    }

    if (box_idx < CurrentScreen->BoxesCount) {
      local_box = GetBox(box_idx);
      if (order == local_box->Order) {
        box_idx++;
        order++;
        DrawBox(local_box);
      }
    }

  }
}

void Init_MCU() {
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_9);
  TFT_BLED = 1;
  TFT_Set_Default_Mode();
  //TP_TFT_Set_Default_Mode();
}

void Start_TP() {
  Init_MCU();

  InitializeTouchPanel();

  InitializeObjects();
  display_width = Screen1.Width;
  display_height = Screen1.Height;
  DrawScreen(&Screen1);
}