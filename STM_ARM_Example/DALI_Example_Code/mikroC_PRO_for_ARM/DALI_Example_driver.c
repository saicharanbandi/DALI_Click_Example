#include "DALI_Example_objects.h"
#include "DALI_Example_resources.h"
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
typedef unsigned long TPointer;
TPointer PressedObject;
int PressedObjectType;
unsigned int caption_length, caption_height;
unsigned int display_width, display_height;

int _object_count;
unsigned short object_pressed;
TButton *local_button;
TButton *exec_button;
int button_order;
TButton_Round *local_round_button;
TButton_Round *exec_round_button;
int round_button_order;
TCButton_Round *local_round_cbutton;
TCButton_Round *exec_round_cbutton;
int round_cbutton_order;
TLabel *local_label;
TLabel *exec_label;
int label_order;
TCLabel *local_clabel;
TCLabel *exec_clabel;
int clabel_order;
TImage *local_image;
TImage *exec_image;
int image_order;
TCircle *local_circle;
TCircle *exec_circle;
int circle_order;
TBox *local_box;
TBox *exec_box;
int box_order;


void Init_ADC() {
  ADC_Set_Input_Channel(_ADC_CHANNEL_8 | _ADC_CHANNEL_9);
  ADC1_Init();
  Delay_ms(100);
}
static void InitializeTouchPanel() {
  Init_ADC();
  TFT_Init(320, 240);

  TP_TFT_Init(320, 240, 8, 9);                                  // Initialize touch panel
  TP_TFT_Set_ADC_Threshold(ADC_THRESHOLD);                              // Set touch panel ADC threshold

  PenDown = 0;
  PressedObject = 0;
  PressedObjectType = -1;
}

void Calibrate() {
  TFT_Set_Pen(CL_WHITE, 3);
  TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
  TFT_Write_Text("Touch selected corners for calibration", 50, 80);
  TFT_Line(315, 239, 319, 239);
  TFT_Line(309, 229, 319, 239);
  TFT_Line(319, 234, 319, 239);
  TFT_Write_Text("first here",235,220);

  TP_TFT_Calibrate_Min();                      // Calibration of bottom left corner
  Delay_ms(500);

  TFT_Set_Pen(CL_BLACK, 3);
  TFT_Set_Font(TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
  TFT_Line(315, 239, 319, 239);
  TFT_Line(309, 229, 319, 239);
  TFT_Line(319, 234, 319, 239);
  TFT_Write_Text("first here",235,220);

  TFT_Set_Pen(CL_WHITE, 3);
  TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
  TFT_Write_Text("now here ", 20, 5);
  TFT_Line(0, 0, 5, 0);
  TFT_Line(0, 0, 0, 5);
  TFT_Line(0, 0, 10, 10);

  TP_TFT_Calibrate_Max();                      // Calibration of bottom left corner
  Delay_ms(500);
}


/////////////////////////
  TScreen*  CurrentScreen;

  TScreen                Screen1;
  TImage               Image1;
  TBox                   Box7;
  TBox                   BoxLightIn04;
  TBox                   BoxLightIn03;
  TBox                   BoxLightIn01;
  TButton               Button1;
char Button1_Caption[7] = "ALL ON";

  TButton               Button2;
char Button2_Caption[8] = "ALL OFF";

  TLabel                 Label4;
char Label4_Caption[13] = "Select Group";

  TButton               Button3;
char Button3_Caption[2] = "1";

  TButton               Button4;
char Button4_Caption[2] = "2";

  TButton               Button5;
char Button5_Caption[2] = "3";

  TBox                   BoxLightIn02;
  TCircle                CircleLightOut02;
  TCircle                CircleLightOut03;
  TCircle                CircleLightOut04;
  TCircle                CircleLightOut05;
  TCircle                CircleLightOut06;
  TCircle                CircleLightOut01;
  TButton               Button6;
char Button6_Caption[2] = "4";

  TButton               Button7;
char Button7_Caption[2] = "5";

  TButton               Button8;
char Button8_Caption[2] = "6";

  TButton_Round          BtnBackSetLights;
char BtnBackSetLights_Caption[5] = "BACK";

  TButton                * const code Screen1_Buttons[8]=
         {
         &Button1,             
         &Button2,             
         &Button3,             
         &Button4,             
         &Button5,             
         &Button6,             
         &Button7,             
         &Button8              
         };
  TButton_Round          * const code Screen1_Buttons_Round[1]=
         {
         &BtnBackSetLights     
         };
  TLabel                 * const code Screen1_Labels[1]=
         {
         &Label4               
         };
  TImage                 * const code Screen1_Images[1]=
         {
         &Image1               
         };
  TCircle                * const code Screen1_Circles[6]=
         {
         &CircleLightOut02,    
         &CircleLightOut03,    
         &CircleLightOut04,    
         &CircleLightOut05,    
         &CircleLightOut06,    
         &CircleLightOut01     
         };
  TBox                   * const code Screen1_Boxes[5]=
         {
         &Box7,                
         &BoxLightIn04,        
         &BoxLightIn03,        
         &BoxLightIn01,        
         &BoxLightIn02         
         };


  TScreen                Screen2;
  TButton               Button10;
char Button10_Caption[11] = "SET LIGHTS";

  TLabel                 Label1;
char Label1_Caption[24] = "DALI Light Control Demo";

  TButton               BtnSetArc;
char BtnSetArc_Caption[27] = "SET INDIVIDUAL LIGHT LEVEL";

  TButton               BtnSendQueryCmd;
char BtnSendQueryCmd_Caption[20] = "SEND QUERY COMMANDS";

  TButton               BtnSetSceneLevel;
char BtnSetSceneLevel_Caption[18] = "GO TO SCENE LEVEL";

  TLabel                 Label14;
char Label14_Caption[17] = "mikroElektronika";

  TButton                * const code Screen2_Buttons[4]=
         {
         &Button10,            
         &BtnSetArc,           
         &BtnSendQueryCmd,     
         &BtnSetSceneLevel     
         };
  TLabel                 * const code Screen2_Labels[2]=
         {
         &Label1,              
         &Label14              
         };


  TScreen                Screen3;
  TImage               Image2;
  TBox                   Box4;
  TBox                   Box3;
  TBox                   Box1;
  TBox                   Box2;
  TCircle                Circle2;
  TCircle                Circle3;
  TCircle                Circle4;
  TCircle                Circle5;
  TCircle                Circle6;
  TCircle                Circle1;
  TBox                   Box5;
  TLabel                 Label2;
char Label2_Caption[3] = "01";

  TLabel                 Label3;
char Label3_Caption[3] = "02";

  TLabel                 Label5;
char Label5_Caption[3] = "03";

  TLabel                 Label6;
char Label6_Caption[3] = "04";

  TLabel                 Label7;
char Label7_Caption[3] = "05";

  TLabel                 Label8;
char Label8_Caption[3] = "06";

  TLabel                 Label9;
char Label9_Caption[3] = "01";

  TLabel                 Label10;
char Label10_Caption[3] = "02";

  TLabel                 Label11;
char Label11_Caption[3] = "03";

  TLabel                 Label12;
char Label12_Caption[3] = "04";

  TButton_Round          BtnArcLevelDown;
char BtnArcLevelDown_Caption[5] = "DOWN";

  TButton_Round          BtnArcLevelUp;
char BtnArcLevelUp_Caption[4] = "UP ";

  TButton_Round          ButtonRound3;
char ButtonRound3_Caption[5] = "BACK";

  TButton_Round          BtnBallastArcLevel;
char BtnBallastArcLevel_Caption[4] = "254";

  TButton_Round          * const code Screen3_Buttons_Round[4]=
         {
         &BtnArcLevelDown,     
         &BtnArcLevelUp,       
         &ButtonRound3,        
         &BtnBallastArcLevel   
         };
  TLabel                 * const code Screen3_Labels[10]=
         {
         &Label2,              
         &Label3,              
         &Label5,              
         &Label6,              
         &Label7,              
         &Label8,              
         &Label9,              
         &Label10,             
         &Label11,             
         &Label12              
         };
  TImage                 * const code Screen3_Images[1]=
         {
         &Image2               
         };
  TCircle                * const code Screen3_Circles[6]=
         {
         &Circle2,             
         &Circle3,             
         &Circle4,             
         &Circle5,             
         &Circle6,             
         &Circle1              
         };
  TBox                   * const code Screen3_Boxes[5]=
         {
         &Box4,                
         &Box3,                
         &Box1,                
         &Box2,                
         &Box5                 
         };


  TScreen                ScreenSetScene;
  TCButton_Round          BtnBalAddrUp = 
         {
         &ScreenSetScene       , //   BtnBalAddrUp.OwnerScreen
         0                     , //   BtnBalAddrUp.Order           
         8                     , //   BtnBalAddrUp.Left            
         22                    , //   BtnBalAddrUp.Top              
         95                    , //   BtnBalAddrUp.Width            
         30                    , //   BtnBalAddrUp.Height        
         1                     , //   BtnBalAddrUp.Pen_Width          
         0x07FF                , //   BtnBalAddrUp.Pen_Color       
         1                     , //   BtnBalAddrUp.Visible         
         1                     , //   BtnBalAddrUp.Active           
         1                     , //   BtnBalAddrUp.Transparent     
         &BtnBalAddrUp_Caption  , //   BtnBalAddrUp.Caption        
         _taCenter             , //       BtnBalAddrUp.TextAlign        
         &Tahoma25x25_Bold      , //   BtnBalAddrUp.FontName        
         0x07FF                , //
         0                     , //   BtnBalAddrUp.Gradient        
         0                     , //   BtnBalAddrUp.Gradient_Orientation    
         0xFFFF                , //   BtnBalAddrUp.Gradient_Start_Color    
         0xC618                , //  BtnBalAddrUp.Gradient_Start_Color    
         0x0010                , //   BtnBalAddrUp.Color           
         3                     , //   BtnBalAddrUp.CornerRadius           
         1                     , //   BtnBalAddrUp.PressColEnabled 
         0x07FF                , //   BtnBalAddrUp.Press_Color     
         0                     ,//  BtnBalAddrUp.OnUpPtr
         0                     ,//  BtnBalAddrUp.OnDownPtr
         BtnBalAddrUpOnClick   ,//  BtnBalAddrUp.OnClickPtr
         0                      //  BtnBalAddrUp.OnPressPtr
         };
const char BtnBalAddrUp_Caption[3] = "Up";

  TCButton_Round          BtnBalAddrDown = 
         {
         &ScreenSetScene       , //   BtnBalAddrDown.OwnerScreen
         1                     , //   BtnBalAddrDown.Order           
         8                     , //   BtnBalAddrDown.Left            
         121                   , //   BtnBalAddrDown.Top              
         95                    , //   BtnBalAddrDown.Width            
         30                    , //   BtnBalAddrDown.Height        
         1                     , //   BtnBalAddrDown.Pen_Width          
         0x07FF                , //   BtnBalAddrDown.Pen_Color       
         1                     , //   BtnBalAddrDown.Visible         
         1                     , //   BtnBalAddrDown.Active           
         1                     , //   BtnBalAddrDown.Transparent     
         &BtnBalAddrDown_Caption, //   BtnBalAddrDown.Caption        
         _taCenter             , //       BtnBalAddrDown.TextAlign        
         &Tahoma25x25_Bold      , //   BtnBalAddrDown.FontName        
         0x07FF                , //
         0                     , //   BtnBalAddrDown.Gradient        
         0                     , //   BtnBalAddrDown.Gradient_Orientation    
         0xFFFF                , //   BtnBalAddrDown.Gradient_Start_Color    
         0xC618                , //  BtnBalAddrDown.Gradient_Start_Color    
         0x0010                , //   BtnBalAddrDown.Color           
         3                     , //   BtnBalAddrDown.CornerRadius           
         1                     , //   BtnBalAddrDown.PressColEnabled 
         0x07FF                , //   BtnBalAddrDown.Press_Color     
         0                     ,//  BtnBalAddrDown.OnUpPtr
         0                     ,//  BtnBalAddrDown.OnDownPtr
         BtnBalAddrDownOnClick ,//  BtnBalAddrDown.OnClickPtr
         0                      //  BtnBalAddrDown.OnPressPtr
         };
const char BtnBalAddrDown_Caption[5] = "Down";

  TCLabel          Label15 = 
         {
         &ScreenSetScene       , //   Label15.OwnerScreen
         2                     , //   Label15.Order          
         11                    , //   Label15.Left           
         3                     , //   Label15.Top             
         89                    , //   Label15.Width         
         17                    , //   Label15.Height       
         &Label15_Caption      , //   Label15.Caption        
         &Tahoma13x16_Regular  , //   Label15.FontName       
         0x07FF                , //   Label15.Font_Color     
         1                     , //   Label15.Visible        
         1                     , //   Label15.Active        
         0                     ,//  Label15.OnUpPtr
         0                     ,//  Label15.OnDownPtr
         0                     ,//  Label15.OnClickPtr
         0                      //  Label15.OnPressPtr
         };
const char Label15_Caption[16] = "Ballast Address";

  TCButton_Round          BtnBackScene = 
         {
         &ScreenSetScene       , //   BtnBackScene.OwnerScreen
         3                     , //   BtnBackScene.Order           
         5                     , //   BtnBackScene.Left            
         195                   , //   BtnBackScene.Top              
         95                    , //   BtnBackScene.Width            
         40                    , //   BtnBackScene.Height        
         1                     , //   BtnBackScene.Pen_Width          
         0x07FF                , //   BtnBackScene.Pen_Color       
         1                     , //   BtnBackScene.Visible         
         1                     , //   BtnBackScene.Active           
         1                     , //   BtnBackScene.Transparent     
         &BtnBackScene_Caption  , //   BtnBackScene.Caption        
         _taCenter             , //       BtnBackScene.TextAlign        
         &Tahoma25x25_Bold      , //   BtnBackScene.FontName        
         0x07FF                , //
         0                     , //   BtnBackScene.Gradient        
         0                     , //   BtnBackScene.Gradient_Orientation    
         0xFFFF                , //   BtnBackScene.Gradient_Start_Color    
         0xC618                , //  BtnBackScene.Gradient_Start_Color    
         0x0010                , //   BtnBackScene.Color           
         3                     , //   BtnBackScene.CornerRadius           
         1                     , //   BtnBackScene.PressColEnabled 
         0x07FF                , //   BtnBackScene.Press_Color     
         0                     ,//  BtnBackScene.OnUpPtr
         0                     ,//  BtnBackScene.OnDownPtr
         BtnBackSceneOnClick   ,//  BtnBackScene.OnClickPtr
         0                      //  BtnBackScene.OnPressPtr
         };
const char BtnBackScene_Caption[5] = "BACK";

  TCButton_Round          BtnSetSceneBallast = 
         {
         &ScreenSetScene       , //   BtnSetSceneBallast.OwnerScreen
         4                     , //   BtnSetSceneBallast.Order           
         219                   , //   BtnSetSceneBallast.Left            
         67                    , //   BtnSetSceneBallast.Top              
         95                    , //   BtnSetSceneBallast.Width            
         40                    , //   BtnSetSceneBallast.Height        
         1                     , //   BtnSetSceneBallast.Pen_Width          
         0x07FF                , //   BtnSetSceneBallast.Pen_Color       
         1                     , //   BtnSetSceneBallast.Visible         
         1                     , //   BtnSetSceneBallast.Active           
         1                     , //   BtnSetSceneBallast.Transparent     
         &BtnSetSceneBallast_Caption, //   BtnSetSceneBallast.Caption        
         _taCenter             , //       BtnSetSceneBallast.TextAlign        
         &Tahoma25x25_Bold      , //   BtnSetSceneBallast.FontName        
         0x07FF                , //
         0                     , //   BtnSetSceneBallast.Gradient        
         0                     , //   BtnSetSceneBallast.Gradient_Orientation    
         0xFFFF                , //   BtnSetSceneBallast.Gradient_Start_Color    
         0xC618                , //  BtnSetSceneBallast.Gradient_Start_Color    
         0x0010                , //   BtnSetSceneBallast.Color           
         3                     , //   BtnSetSceneBallast.CornerRadius           
         1                     , //   BtnSetSceneBallast.PressColEnabled 
         0x07FF                , //   BtnSetSceneBallast.Press_Color     
         0                     ,//  BtnSetSceneBallast.OnUpPtr
         0                     ,//  BtnSetSceneBallast.OnDownPtr
         BtnSetSceneBallastOnClick,//  BtnSetSceneBallast.OnClickPtr
         0                      //  BtnSetSceneBallast.OnPressPtr
         };
const char BtnSetSceneBallast_Caption[4] = "SET";

  TCButton_Round          BtnSceneNumUp = 
         {
         &ScreenSetScene       , //   BtnSceneNumUp.OwnerScreen
         5                     , //   BtnSceneNumUp.Order           
         114                   , //   BtnSceneNumUp.Left            
         22                    , //   BtnSceneNumUp.Top              
         95                    , //   BtnSceneNumUp.Width            
         30                    , //   BtnSceneNumUp.Height        
         1                     , //   BtnSceneNumUp.Pen_Width          
         0x07FF                , //   BtnSceneNumUp.Pen_Color       
         1                     , //   BtnSceneNumUp.Visible         
         1                     , //   BtnSceneNumUp.Active           
         1                     , //   BtnSceneNumUp.Transparent     
         &BtnSceneNumUp_Caption , //   BtnSceneNumUp.Caption        
         _taCenter             , //       BtnSceneNumUp.TextAlign        
         &Tahoma25x25_Bold      , //   BtnSceneNumUp.FontName        
         0x07FF                , //
         0                     , //   BtnSceneNumUp.Gradient        
         0                     , //   BtnSceneNumUp.Gradient_Orientation    
         0xFFFF                , //   BtnSceneNumUp.Gradient_Start_Color    
         0xC618                , //  BtnSceneNumUp.Gradient_Start_Color    
         0x0010                , //   BtnSceneNumUp.Color           
         3                     , //   BtnSceneNumUp.CornerRadius           
         1                     , //   BtnSceneNumUp.PressColEnabled 
         0x07FF                , //   BtnSceneNumUp.Press_Color     
         0                     ,//  BtnSceneNumUp.OnUpPtr
         0                     ,//  BtnSceneNumUp.OnDownPtr
         BtnSceneNumUpOnClick  ,//  BtnSceneNumUp.OnClickPtr
         0                      //  BtnSceneNumUp.OnPressPtr
         };
const char BtnSceneNumUp_Caption[3] = "Up";

  TCButton_Round          BtnSceneNumDown = 
         {
         &ScreenSetScene       , //   BtnSceneNumDown.OwnerScreen
         6                     , //   BtnSceneNumDown.Order           
         114                   , //   BtnSceneNumDown.Left            
         121                   , //   BtnSceneNumDown.Top              
         95                    , //   BtnSceneNumDown.Width            
         30                    , //   BtnSceneNumDown.Height        
         1                     , //   BtnSceneNumDown.Pen_Width          
         0x07FF                , //   BtnSceneNumDown.Pen_Color       
         1                     , //   BtnSceneNumDown.Visible         
         1                     , //   BtnSceneNumDown.Active           
         1                     , //   BtnSceneNumDown.Transparent     
         &BtnSceneNumDown_Caption, //   BtnSceneNumDown.Caption        
         _taCenter             , //       BtnSceneNumDown.TextAlign        
         &Tahoma25x25_Bold      , //   BtnSceneNumDown.FontName        
         0x07FF                , //
         0                     , //   BtnSceneNumDown.Gradient        
         0                     , //   BtnSceneNumDown.Gradient_Orientation    
         0xFFFF                , //   BtnSceneNumDown.Gradient_Start_Color    
         0xC618                , //  BtnSceneNumDown.Gradient_Start_Color    
         0x0010                , //   BtnSceneNumDown.Color           
         3                     , //   BtnSceneNumDown.CornerRadius           
         1                     , //   BtnSceneNumDown.PressColEnabled 
         0x07FF                , //   BtnSceneNumDown.Press_Color     
         0                     ,//  BtnSceneNumDown.OnUpPtr
         0                     ,//  BtnSceneNumDown.OnDownPtr
         BtnSceneNumDownOnClick,//  BtnSceneNumDown.OnClickPtr
         0                      //  BtnSceneNumDown.OnPressPtr
         };
const char BtnSceneNumDown_Caption[5] = "Down";

  TCLabel          Label19 = 
         {
         &ScreenSetScene       , //   Label19.OwnerScreen
         7                     , //   Label19.Order          
         119                   , //   Label19.Left           
         3                     , //   Label19.Top             
         86                    , //   Label19.Width         
         17                    , //   Label19.Height       
         &Label19_Caption      , //   Label19.Caption        
         &Tahoma13x16_Regular  , //   Label19.FontName       
         0x07FF                , //   Label19.Font_Color     
         1                     , //   Label19.Visible        
         1                     , //   Label19.Active        
         0                     ,//  Label19.OnUpPtr
         0                     ,//  Label19.OnDownPtr
         0                     ,//  Label19.OnClickPtr
         0                      //  Label19.OnPressPtr
         };
const char Label19_Caption[13] = "Scene Number";

  TButton_Round          btnBallastAddressText;
char btnBallastAddressText_Caption[3] = "63";

  TButton_Round          btnSceneNumberText;
char btnSceneNumberText_Caption[3] = "15";

  TButton_Round          * const code Screen4_Buttons_Round[2]=
         {
         &btnBallastAddressText,
         &btnSceneNumberText   
         };
  TCButton_Round          * const code Screen4_CButtons_Round[6]=
         {
         &BtnBalAddrUp,        
         &BtnBalAddrDown,      
         &BtnBackScene,        
         &BtnSetSceneBallast,  
         &BtnSceneNumUp,       
         &BtnSceneNumDown      
         };
  TCLabel                 * const code Screen4_CLabels[2]=
         {
         &Label15,             
         &Label19              
         };


  TScreen                ScreenQueries;
  TCButton_Round          BtnBackQueries = 
         {
         &ScreenQueries        , //   BtnBackQueries.OwnerScreen
         0                     , //   BtnBackQueries.Order           
         5                     , //   BtnBackQueries.Left            
         195                   , //   BtnBackQueries.Top              
         95                    , //   BtnBackQueries.Width            
         40                    , //   BtnBackQueries.Height        
         1                     , //   BtnBackQueries.Pen_Width          
         0x07FF                , //   BtnBackQueries.Pen_Color       
         1                     , //   BtnBackQueries.Visible         
         1                     , //   BtnBackQueries.Active           
         1                     , //   BtnBackQueries.Transparent     
         &BtnBackQueries_Caption, //   BtnBackQueries.Caption        
         _taCenter             , //       BtnBackQueries.TextAlign        
         &Tahoma25x25_Bold      , //   BtnBackQueries.FontName        
         0x07FF                , //
         0                     , //   BtnBackQueries.Gradient        
         0                     , //   BtnBackQueries.Gradient_Orientation    
         0xFFFF                , //   BtnBackQueries.Gradient_Start_Color    
         0xC618                , //  BtnBackQueries.Gradient_Start_Color    
         0x0010                , //   BtnBackQueries.Color           
         3                     , //   BtnBackQueries.CornerRadius           
         1                     , //   BtnBackQueries.PressColEnabled 
         0x07FF                , //   BtnBackQueries.Press_Color     
         0                     ,//  BtnBackQueries.OnUpPtr
         0                     ,//  BtnBackQueries.OnDownPtr
         BtnBackQueriesOnClick ,//  BtnBackQueries.OnClickPtr
         0                      //  BtnBackQueries.OnPressPtr
         };
const char BtnBackQueries_Caption[5] = "BACK";

  TCButton_Round          BtnQueryActualLevel = 
         {
         &ScreenQueries        , //   BtnQueryActualLevel.OwnerScreen
         1                     , //   BtnQueryActualLevel.Order           
         178                   , //   BtnQueryActualLevel.Left            
         10                    , //   BtnQueryActualLevel.Top              
         134                   , //   BtnQueryActualLevel.Width            
         25                    , //   BtnQueryActualLevel.Height        
         1                     , //   BtnQueryActualLevel.Pen_Width          
         0x07FF                , //   BtnQueryActualLevel.Pen_Color       
         1                     , //   BtnQueryActualLevel.Visible         
         1                     , //   BtnQueryActualLevel.Active           
         1                     , //   BtnQueryActualLevel.Transparent     
         &BtnQueryActualLevel_Caption, //   BtnQueryActualLevel.Caption        
         _taCenter             , //       BtnQueryActualLevel.TextAlign        
         &Tahoma11x13_Regular   , //   BtnQueryActualLevel.FontName        
         0x07FF                , //
         0                     , //   BtnQueryActualLevel.Gradient        
         0                     , //   BtnQueryActualLevel.Gradient_Orientation    
         0xFFFF                , //   BtnQueryActualLevel.Gradient_Start_Color    
         0xC618                , //  BtnQueryActualLevel.Gradient_Start_Color    
         0x0010                , //   BtnQueryActualLevel.Color           
         3                     , //   BtnQueryActualLevel.CornerRadius           
         1                     , //   BtnQueryActualLevel.PressColEnabled 
         0x07FF                , //   BtnQueryActualLevel.Press_Color     
         0                     ,//  BtnQueryActualLevel.OnUpPtr
         0                     ,//  BtnQueryActualLevel.OnDownPtr
         BtnQueryActualLevelOnClick,//  BtnQueryActualLevel.OnClickPtr
         0                      //  BtnQueryActualLevel.OnPressPtr
         };
const char BtnQueryActualLevel_Caption[19] = "Query Actual Level";

  TButton_Round          BtnSelectBallastQuery;
char BtnSelectBallastQuery_Caption[3] = "63";

  TCButton_Round          BtnScreenQueriesBalUp = 
         {
         &ScreenQueries        , //   BtnScreenQueriesBalUp.OwnerScreen
         3                     , //   BtnScreenQueriesBalUp.Order           
         5                     , //   BtnScreenQueriesBalUp.Left            
         10                    , //   BtnScreenQueriesBalUp.Top              
         95                    , //   BtnScreenQueriesBalUp.Width            
         35                    , //   BtnScreenQueriesBalUp.Height        
         1                     , //   BtnScreenQueriesBalUp.Pen_Width          
         0x07FF                , //   BtnScreenQueriesBalUp.Pen_Color       
         1                     , //   BtnScreenQueriesBalUp.Visible         
         1                     , //   BtnScreenQueriesBalUp.Active           
         1                     , //   BtnScreenQueriesBalUp.Transparent     
         &BtnScreenQueriesBalUp_Caption, //   BtnScreenQueriesBalUp.Caption        
         _taCenter             , //       BtnScreenQueriesBalUp.TextAlign        
         &Tahoma11x13_Regular   , //   BtnScreenQueriesBalUp.FontName        
         0x07FF                , //
         0                     , //   BtnScreenQueriesBalUp.Gradient        
         0                     , //   BtnScreenQueriesBalUp.Gradient_Orientation    
         0xFFFF                , //   BtnScreenQueriesBalUp.Gradient_Start_Color    
         0xC618                , //  BtnScreenQueriesBalUp.Gradient_Start_Color    
         0x0010                , //   BtnScreenQueriesBalUp.Color           
         3                     , //   BtnScreenQueriesBalUp.CornerRadius           
         1                     , //   BtnScreenQueriesBalUp.PressColEnabled 
         0x07FF                , //   BtnScreenQueriesBalUp.Press_Color     
         0                     ,//  BtnScreenQueriesBalUp.OnUpPtr
         0                     ,//  BtnScreenQueriesBalUp.OnDownPtr
         BtnScreenQueriesBalUpOnClick,//  BtnScreenQueriesBalUp.OnClickPtr
         0                      //  BtnScreenQueriesBalUp.OnPressPtr
         };
const char BtnScreenQueriesBalUp_Caption[3] = "UP";

  TCButton_Round          BtnScreenQueriesBalDown = 
         {
         &ScreenQueries        , //   BtnScreenQueriesBalDown.OwnerScreen
         4                     , //   BtnScreenQueriesBalDown.Order           
         6                     , //   BtnScreenQueriesBalDown.Left            
         137                   , //   BtnScreenQueriesBalDown.Top              
         95                    , //   BtnScreenQueriesBalDown.Width            
         35                    , //   BtnScreenQueriesBalDown.Height        
         1                     , //   BtnScreenQueriesBalDown.Pen_Width          
         0x07FF                , //   BtnScreenQueriesBalDown.Pen_Color       
         1                     , //   BtnScreenQueriesBalDown.Visible         
         1                     , //   BtnScreenQueriesBalDown.Active           
         1                     , //   BtnScreenQueriesBalDown.Transparent     
         &BtnScreenQueriesBalDown_Caption, //   BtnScreenQueriesBalDown.Caption        
         _taCenter             , //       BtnScreenQueriesBalDown.TextAlign        
         &Tahoma11x13_Regular   , //   BtnScreenQueriesBalDown.FontName        
         0x07FF                , //
         0                     , //   BtnScreenQueriesBalDown.Gradient        
         0                     , //   BtnScreenQueriesBalDown.Gradient_Orientation    
         0xFFFF                , //   BtnScreenQueriesBalDown.Gradient_Start_Color    
         0xC618                , //  BtnScreenQueriesBalDown.Gradient_Start_Color    
         0x0010                , //   BtnScreenQueriesBalDown.Color           
         3                     , //   BtnScreenQueriesBalDown.CornerRadius           
         1                     , //   BtnScreenQueriesBalDown.PressColEnabled 
         0x07FF                , //   BtnScreenQueriesBalDown.Press_Color     
         0                     ,//  BtnScreenQueriesBalDown.OnUpPtr
         0                     ,//  BtnScreenQueriesBalDown.OnDownPtr
         BtnScreenQueriesBalDownOnClick,//  BtnScreenQueriesBalDown.OnClickPtr
         0                      //  BtnScreenQueriesBalDown.OnPressPtr
         };
const char BtnScreenQueriesBalDown_Caption[5] = "DOWN";

  TButton_Round          BtnScreenQueriesAnswer;
char BtnScreenQueriesAnswer_Caption[10] = "1111 1111";

  TLabel                 Label13;
char Label13_Caption[7] = "Answer";

  TCButton_Round          BtnQueryBallast = 
         {
         &ScreenQueries        , //   BtnQueryBallast.OwnerScreen
         7                     , //   BtnQueryBallast.Order           
         178                   , //   BtnQueryBallast.Left            
         42                    , //   BtnQueryBallast.Top              
         134                   , //   BtnQueryBallast.Width            
         25                    , //   BtnQueryBallast.Height        
         1                     , //   BtnQueryBallast.Pen_Width          
         0x07FF                , //   BtnQueryBallast.Pen_Color       
         1                     , //   BtnQueryBallast.Visible         
         1                     , //   BtnQueryBallast.Active           
         1                     , //   BtnQueryBallast.Transparent     
         &BtnQueryBallast_Caption, //   BtnQueryBallast.Caption        
         _taCenter             , //       BtnQueryBallast.TextAlign        
         &Tahoma11x13_Regular   , //   BtnQueryBallast.FontName        
         0x07FF                , //
         0                     , //   BtnQueryBallast.Gradient        
         0                     , //   BtnQueryBallast.Gradient_Orientation    
         0xFFFF                , //   BtnQueryBallast.Gradient_Start_Color    
         0xC618                , //  BtnQueryBallast.Gradient_Start_Color    
         0x0010                , //   BtnQueryBallast.Color           
         3                     , //   BtnQueryBallast.CornerRadius           
         1                     , //   BtnQueryBallast.PressColEnabled 
         0x07FF                , //   BtnQueryBallast.Press_Color     
         0                     ,//  BtnQueryBallast.OnUpPtr
         0                     ,//  BtnQueryBallast.OnDownPtr
         BtnQueryBallastOnClick,//  BtnQueryBallast.OnClickPtr
         0                      //  BtnQueryBallast.OnPressPtr
         };
const char BtnQueryBallast_Caption[14] = "Query Ballast";

  TCButton_Round          BtnQueryDeviceType = 
         {
         &ScreenQueries        , //   BtnQueryDeviceType.OwnerScreen
         8                     , //   BtnQueryDeviceType.Order           
         178                   , //   BtnQueryDeviceType.Left            
         74                    , //   BtnQueryDeviceType.Top              
         134                   , //   BtnQueryDeviceType.Width            
         25                    , //   BtnQueryDeviceType.Height        
         1                     , //   BtnQueryDeviceType.Pen_Width          
         0x07FF                , //   BtnQueryDeviceType.Pen_Color       
         1                     , //   BtnQueryDeviceType.Visible         
         1                     , //   BtnQueryDeviceType.Active           
         1                     , //   BtnQueryDeviceType.Transparent     
         &BtnQueryDeviceType_Caption, //   BtnQueryDeviceType.Caption        
         _taCenter             , //       BtnQueryDeviceType.TextAlign        
         &Tahoma11x13_Regular   , //   BtnQueryDeviceType.FontName        
         0x07FF                , //
         0                     , //   BtnQueryDeviceType.Gradient        
         0                     , //   BtnQueryDeviceType.Gradient_Orientation    
         0xFFFF                , //   BtnQueryDeviceType.Gradient_Start_Color    
         0xC618                , //  BtnQueryDeviceType.Gradient_Start_Color    
         0x0010                , //   BtnQueryDeviceType.Color           
         3                     , //   BtnQueryDeviceType.CornerRadius           
         1                     , //   BtnQueryDeviceType.PressColEnabled 
         0x07FF                , //   BtnQueryDeviceType.Press_Color     
         0                     ,//  BtnQueryDeviceType.OnUpPtr
         0                     ,//  BtnQueryDeviceType.OnDownPtr
         BtnQueryDeviceTypeOnClick,//  BtnQueryDeviceType.OnClickPtr
         0                      //  BtnQueryDeviceType.OnPressPtr
         };
const char BtnQueryDeviceType_Caption[18] = "Query Device Type";

  TCButton_Round          BtnQueryGroup0_7 = 
         {
         &ScreenQueries        , //   BtnQueryGroup0_7.OwnerScreen
         9                     , //   BtnQueryGroup0_7.Order           
         178                   , //   BtnQueryGroup0_7.Left            
         107                   , //   BtnQueryGroup0_7.Top              
         134                   , //   BtnQueryGroup0_7.Width            
         25                    , //   BtnQueryGroup0_7.Height        
         1                     , //   BtnQueryGroup0_7.Pen_Width          
         0x07FF                , //   BtnQueryGroup0_7.Pen_Color       
         1                     , //   BtnQueryGroup0_7.Visible         
         1                     , //   BtnQueryGroup0_7.Active           
         1                     , //   BtnQueryGroup0_7.Transparent     
         &BtnQueryGroup0_7_Caption, //   BtnQueryGroup0_7.Caption        
         _taCenter             , //       BtnQueryGroup0_7.TextAlign        
         &Tahoma11x13_Regular   , //   BtnQueryGroup0_7.FontName        
         0x07FF                , //
         0                     , //   BtnQueryGroup0_7.Gradient        
         0                     , //   BtnQueryGroup0_7.Gradient_Orientation    
         0xFFFF                , //   BtnQueryGroup0_7.Gradient_Start_Color    
         0xC618                , //  BtnQueryGroup0_7.Gradient_Start_Color    
         0x0010                , //   BtnQueryGroup0_7.Color           
         3                     , //   BtnQueryGroup0_7.CornerRadius           
         1                     , //   BtnQueryGroup0_7.PressColEnabled 
         0x07FF                , //   BtnQueryGroup0_7.Press_Color     
         0                     ,//  BtnQueryGroup0_7.OnUpPtr
         0                     ,//  BtnQueryGroup0_7.OnDownPtr
         BtnQueryGroup0_7OnClick,//  BtnQueryGroup0_7.OnClickPtr
         0                      //  BtnQueryGroup0_7.OnPressPtr
         };
const char BtnQueryGroup0_7_Caption[18] = "Query Group 0 - 7";

  TCButton_Round          BtnQueryGroup8_15 = 
         {
         &ScreenQueries        , //   BtnQueryGroup8_15.OwnerScreen
         10                    , //   BtnQueryGroup8_15.Order           
         178                   , //   BtnQueryGroup8_15.Left            
         140                   , //   BtnQueryGroup8_15.Top              
         134                   , //   BtnQueryGroup8_15.Width            
         25                    , //   BtnQueryGroup8_15.Height        
         1                     , //   BtnQueryGroup8_15.Pen_Width          
         0x07FF                , //   BtnQueryGroup8_15.Pen_Color       
         1                     , //   BtnQueryGroup8_15.Visible         
         1                     , //   BtnQueryGroup8_15.Active           
         1                     , //   BtnQueryGroup8_15.Transparent     
         &BtnQueryGroup8_15_Caption, //   BtnQueryGroup8_15.Caption        
         _taCenter             , //       BtnQueryGroup8_15.TextAlign        
         &Tahoma11x13_Regular   , //   BtnQueryGroup8_15.FontName        
         0x07FF                , //
         0                     , //   BtnQueryGroup8_15.Gradient        
         0                     , //   BtnQueryGroup8_15.Gradient_Orientation    
         0xFFFF                , //   BtnQueryGroup8_15.Gradient_Start_Color    
         0xC618                , //  BtnQueryGroup8_15.Gradient_Start_Color    
         0x0010                , //   BtnQueryGroup8_15.Color           
         3                     , //   BtnQueryGroup8_15.CornerRadius           
         1                     , //   BtnQueryGroup8_15.PressColEnabled 
         0x07FF                , //   BtnQueryGroup8_15.Press_Color     
         0                     ,//  BtnQueryGroup8_15.OnUpPtr
         0                     ,//  BtnQueryGroup8_15.OnDownPtr
         BtnQueryGroup8_15OnClick,//  BtnQueryGroup8_15.OnClickPtr
         0                      //  BtnQueryGroup8_15.OnPressPtr
         };
const char BtnQueryGroup8_15_Caption[19] = "Query Group 8 - 15";

  TButton_Round          * const code Screen5_Buttons_Round[2]=
         {
         &BtnSelectBallastQuery,
         &BtnScreenQueriesAnswer
         };
  TCButton_Round          * const code Screen5_CButtons_Round[8]=
         {
         &BtnBackQueries,      
         &BtnQueryActualLevel, 
         &BtnScreenQueriesBalUp,
         &BtnScreenQueriesBalDown,
         &BtnQueryBallast,     
         &BtnQueryDeviceType,  
         &BtnQueryGroup0_7,    
         &BtnQueryGroup8_15    
         };
  TLabel                 * const code Screen5_Labels[1]=
         {
         &Label13              
         };




static void InitializeObjects() {
  Screen1.Color                     = 0x0003;
  Screen1.Width                     = 320;
  Screen1.Height                    = 240;
  Screen1.ButtonsCount              = 8;
  Screen1.Buttons                   = Screen1_Buttons;
  Screen1.Buttons_RoundCount        = 1;
  Screen1.Buttons_Round             = Screen1_Buttons_Round;
  Screen1.CButtons_RoundCount        = 0;
  Screen1.LabelsCount               = 1;
  Screen1.Labels                    = Screen1_Labels;
  Screen1.CLabelsCount               = 0;
  Screen1.ImagesCount               = 1;
  Screen1.Images                    = Screen1_Images;
  Screen1.CirclesCount              = 6;
  Screen1.Circles                   = Screen1_Circles;
  Screen1.BoxesCount                = 5;
  Screen1.Boxes                     = Screen1_Boxes;
  Screen1.ObjectsCount              = 22;

  Screen2.Color                     = 0x0003;
  Screen2.Width                     = 320;
  Screen2.Height                    = 240;
  Screen2.ButtonsCount              = 4;
  Screen2.Buttons                   = Screen2_Buttons;
  Screen2.Buttons_RoundCount        = 0;
  Screen2.CButtons_RoundCount        = 0;
  Screen2.LabelsCount               = 2;
  Screen2.Labels                    = Screen2_Labels;
  Screen2.CLabelsCount               = 0;
  Screen2.ImagesCount               = 0;
  Screen2.CirclesCount              = 0;
  Screen2.BoxesCount                = 0;
  Screen2.ObjectsCount              = 6;

  Screen3.Color                     = 0x0003;
  Screen3.Width                     = 320;
  Screen3.Height                    = 240;
  Screen3.ButtonsCount              = 0;
  Screen3.Buttons_RoundCount        = 4;
  Screen3.Buttons_Round             = Screen3_Buttons_Round;
  Screen3.CButtons_RoundCount        = 0;
  Screen3.LabelsCount               = 10;
  Screen3.Labels                    = Screen3_Labels;
  Screen3.CLabelsCount               = 0;
  Screen3.ImagesCount               = 1;
  Screen3.Images                    = Screen3_Images;
  Screen3.CirclesCount              = 6;
  Screen3.Circles                   = Screen3_Circles;
  Screen3.BoxesCount                = 5;
  Screen3.Boxes                     = Screen3_Boxes;
  Screen3.ObjectsCount              = 26;

  ScreenSetScene.Color                     = 0x0003;
  ScreenSetScene.Width                     = 320;
  ScreenSetScene.Height                    = 240;
  ScreenSetScene.ButtonsCount              = 0;
  ScreenSetScene.Buttons_RoundCount        = 2;
  ScreenSetScene.Buttons_Round             = Screen4_Buttons_Round;
  ScreenSetScene.CButtons_RoundCount        = 6;
  ScreenSetScene.CButtons_Round             = Screen4_CButtons_Round;
  ScreenSetScene.LabelsCount               = 0;
  ScreenSetScene.CLabelsCount               = 2;
  ScreenSetScene.CLabels                    = Screen4_CLabels;
  ScreenSetScene.ImagesCount               = 0;
  ScreenSetScene.CirclesCount              = 0;
  ScreenSetScene.BoxesCount                = 0;
  ScreenSetScene.ObjectsCount              = 10;

  ScreenQueries.Color                     = 0x0003;
  ScreenQueries.Width                     = 320;
  ScreenQueries.Height                    = 240;
  ScreenQueries.ButtonsCount              = 0;
  ScreenQueries.Buttons_RoundCount        = 2;
  ScreenQueries.Buttons_Round             = Screen5_Buttons_Round;
  ScreenQueries.CButtons_RoundCount        = 8;
  ScreenQueries.CButtons_Round             = Screen5_CButtons_Round;
  ScreenQueries.LabelsCount               = 1;
  ScreenQueries.Labels                    = Screen5_Labels;
  ScreenQueries.CLabelsCount               = 0;
  ScreenQueries.ImagesCount               = 0;
  ScreenQueries.CirclesCount              = 0;
  ScreenQueries.BoxesCount                = 0;
  ScreenQueries.ObjectsCount              = 11;


  Image1.OwnerScreen     = &Screen1;
  Image1.Order          = 0;
  Image1.Left           = 30;
  Image1.Top            = 16;
  Image1.Width          = 148;
  Image1.Height         = 199;
  Image1.Picture_Type   = 0;
  Image1.Picture_Ratio  = 1;
  Image1.Picture_Name   = first_floor_bmp;
  Image1.Visible        = 1;
  Image1.Active         = 1;
  Image1.OnUpPtr         = 0;
  Image1.OnDownPtr       = 0;
  Image1.OnClickPtr      = 0;
  Image1.OnPressPtr      = 0;

  Box7.OwnerScreen     = &Screen1;
  Box7.Order           = 1;
  Box7.Left            = 211;
  Box7.Top             = 1;
  Box7.Width           = 111;
  Box7.Height          = 250;
  Box7.Pen_Width       = 1;
  Box7.Pen_Color       = 0x27FF;
  Box7.Visible         = 1;
  Box7.Active          = 1;
  Box7.Transparent     = 1;
  Box7.Gradient        = 0;
  Box7.Gradient_Orientation    = 0;
  Box7.Gradient_Start_Color    = 0xFFFF;
  Box7.Gradient_End_Color      = 0xC618;
  Box7.Color           = 0x000D;
  Box7.PressColEnabled     = 0;
  Box7.Press_Color     = 0x8410;
  Box7.OnUpPtr         = 0;
  Box7.OnDownPtr       = 0;
  Box7.OnClickPtr      = 0;
  Box7.OnPressPtr      = 0;

  BoxLightIn04.OwnerScreen     = &Screen1;
  BoxLightIn04.Order           = 2;
  BoxLightIn04.Left            = 134;
  BoxLightIn04.Top             = 172;
  BoxLightIn04.Width           = 28;
  BoxLightIn04.Height          = 28;
  BoxLightIn04.Pen_Width       = 0;
  BoxLightIn04.Pen_Color       = 0x0000;
  BoxLightIn04.Visible         = 1;
  BoxLightIn04.Active          = 1;
  BoxLightIn04.Transparent     = 1;
  BoxLightIn04.Gradient        = 0;
  BoxLightIn04.Gradient_Orientation    = 0;
  BoxLightIn04.Gradient_Start_Color    = 0xF800;
  BoxLightIn04.Gradient_End_Color      = 0xF800;
  BoxLightIn04.Color           = 0xFFE0;
  BoxLightIn04.PressColEnabled     = 0;
  BoxLightIn04.Press_Color     = 0xF800;
  BoxLightIn04.OnUpPtr         = 0;
  BoxLightIn04.OnDownPtr       = 0;
  BoxLightIn04.OnClickPtr      = BoxLightIn04OnClick;
  BoxLightIn04.OnPressPtr      = 0;

  BoxLightIn03.OwnerScreen     = &Screen1;
  BoxLightIn03.Order           = 3;
  BoxLightIn03.Left            = 56;
  BoxLightIn03.Top             = 172;
  BoxLightIn03.Width           = 28;
  BoxLightIn03.Height          = 28;
  BoxLightIn03.Pen_Width       = 0;
  BoxLightIn03.Pen_Color       = 0x0000;
  BoxLightIn03.Visible         = 1;
  BoxLightIn03.Active          = 1;
  BoxLightIn03.Transparent     = 1;
  BoxLightIn03.Gradient        = 0;
  BoxLightIn03.Gradient_Orientation    = 0;
  BoxLightIn03.Gradient_Start_Color    = 0xF800;
  BoxLightIn03.Gradient_End_Color      = 0xF800;
  BoxLightIn03.Color           = 0xFFE0;
  BoxLightIn03.PressColEnabled     = 0;
  BoxLightIn03.Press_Color     = 0xF800;
  BoxLightIn03.OnUpPtr         = 0;
  BoxLightIn03.OnDownPtr       = 0;
  BoxLightIn03.OnClickPtr      = BoxLightIn03OnClick;
  BoxLightIn03.OnPressPtr      = 0;

  BoxLightIn01.OwnerScreen     = &Screen1;
  BoxLightIn01.Order           = 4;
  BoxLightIn01.Left            = 71;
  BoxLightIn01.Top             = 42;
  BoxLightIn01.Width           = 28;
  BoxLightIn01.Height          = 28;
  BoxLightIn01.Pen_Width       = 1;
  BoxLightIn01.Pen_Color       = 0x0000;
  BoxLightIn01.Visible         = 1;
  BoxLightIn01.Active          = 1;
  BoxLightIn01.Transparent     = 1;
  BoxLightIn01.Gradient        = 0;
  BoxLightIn01.Gradient_Orientation    = 0;
  BoxLightIn01.Gradient_Start_Color    = 0xF800;
  BoxLightIn01.Gradient_End_Color      = 0xF800;
  BoxLightIn01.Color           = 0xFFE0;
  BoxLightIn01.PressColEnabled     = 0;
  BoxLightIn01.Press_Color     = 0xF800;
  BoxLightIn01.OnUpPtr         = 0;
  BoxLightIn01.OnDownPtr       = 0;
  BoxLightIn01.OnClickPtr      = BoxLightIn01OnClick;
  BoxLightIn01.OnPressPtr      = 0;

  Button1.OwnerScreen     = &Screen1;
  Button1.Order           = 5;
  Button1.Left            = 220;
  Button1.Top             = 11;
  Button1.Width           = 95;
  Button1.Height          = 30;
  Button1.Pen_Width       = 1;
  Button1.Pen_Color       = 0x27FF;
  Button1.Visible         = 1;
  Button1.Active          = 1;
  Button1.Transparent     = 1;
  Button1.Caption         = Button1_Caption;
  Button1.TextAlign             = _taCenter;
  Button1.FontName        = Tahoma13x13_Bold;
  Button1.PressColEnabled = 1;
  Button1.Font_Color      = 0x37FF;
  Button1.Gradient        = 0;
  Button1.Gradient_Orientation    = 0;
  Button1.Gradient_Start_Color    = 0x0013;
  Button1.Gradient_End_Color      = 0x0010;
  Button1.Color           = 0x0010;
  Button1.Press_Color     = 0x1FFF;
  Button1.OnUpPtr         = 0;
  Button1.OnDownPtr       = 0;
  Button1.OnClickPtr      = Button1OnClick;
  Button1.OnPressPtr      = 0;

  Button2.OwnerScreen     = &Screen1;
  Button2.Order           = 6;
  Button2.Left            = 220;
  Button2.Top             = 47;
  Button2.Width           = 95;
  Button2.Height          = 30;
  Button2.Pen_Width       = 1;
  Button2.Pen_Color       = 0x27FF;
  Button2.Visible         = 1;
  Button2.Active          = 1;
  Button2.Transparent     = 1;
  Button2.Caption         = Button2_Caption;
  Button2.TextAlign             = _taCenter;
  Button2.FontName        = Tahoma13x13_Bold;
  Button2.PressColEnabled = 1;
  Button2.Font_Color      = 0x37FF;
  Button2.Gradient        = 0;
  Button2.Gradient_Orientation    = 0;
  Button2.Gradient_Start_Color    = 0x0013;
  Button2.Gradient_End_Color      = 0x0010;
  Button2.Color           = 0x0010;
  Button2.Press_Color     = 0x1FFF;
  Button2.OnUpPtr         = 0;
  Button2.OnDownPtr       = 0;
  Button2.OnClickPtr      = Button2OnClick;
  Button2.OnPressPtr      = 0;

  Label4.OwnerScreen     = &Screen1;
  Label4.Order          = 7;
  Label4.Left           = 237;
  Label4.Top            = 87;
  Label4.Width          = 64;
  Label4.Height         = 15;
  Label4.Visible        = 1;
  Label4.Active         = 1;
  Label4.Caption        = Label4_Caption;
  Label4.FontName       = Tahoma11x13_Regular;
  Label4.Font_Color     = 0x37FF;
  Label4.OnUpPtr         = 0;
  Label4.OnDownPtr       = 0;
  Label4.OnClickPtr      = 0;
  Label4.OnPressPtr      = 0;

  Button3.OwnerScreen     = &Screen1;
  Button3.Order           = 8;
  Button3.Left            = 218;
  Button3.Top             = 110;
  Button3.Width           = 28;
  Button3.Height          = 28;
  Button3.Pen_Width       = 1;
  Button3.Pen_Color       = 0x27FF;
  Button3.Visible         = 1;
  Button3.Active          = 1;
  Button3.Transparent     = 1;
  Button3.Caption         = Button3_Caption;
  Button3.TextAlign             = _taCenter;
  Button3.FontName        = Tahoma13x13_Bold;
  Button3.PressColEnabled = 1;
  Button3.Font_Color      = 0x37FF;
  Button3.Gradient        = 0;
  Button3.Gradient_Orientation    = 0;
  Button3.Gradient_Start_Color    = 0x0013;
  Button3.Gradient_End_Color      = 0x0010;
  Button3.Color           = 0x0010;
  Button3.Press_Color     = 0x1FFF;
  Button3.OnUpPtr         = 0;
  Button3.OnDownPtr       = 0;
  Button3.OnClickPtr      = Button3OnClick;
  Button3.OnPressPtr      = 0;

  Button4.OwnerScreen     = &Screen1;
  Button4.Order           = 9;
  Button4.Left            = 253;
  Button4.Top             = 110;
  Button4.Width           = 28;
  Button4.Height          = 28;
  Button4.Pen_Width       = 1;
  Button4.Pen_Color       = 0x27FF;
  Button4.Visible         = 1;
  Button4.Active          = 1;
  Button4.Transparent     = 1;
  Button4.Caption         = Button4_Caption;
  Button4.TextAlign             = _taCenter;
  Button4.FontName        = Tahoma13x13_Bold;
  Button4.PressColEnabled = 1;
  Button4.Font_Color      = 0x37FF;
  Button4.Gradient        = 0;
  Button4.Gradient_Orientation    = 0;
  Button4.Gradient_Start_Color    = 0x0013;
  Button4.Gradient_End_Color      = 0x0010;
  Button4.Color           = 0x0010;
  Button4.Press_Color     = 0x1FFF;
  Button4.OnUpPtr         = 0;
  Button4.OnDownPtr       = 0;
  Button4.OnClickPtr      = Button4OnClick;
  Button4.OnPressPtr      = 0;

  Button5.OwnerScreen     = &Screen1;
  Button5.Order           = 10;
  Button5.Left            = 288;
  Button5.Top             = 110;
  Button5.Width           = 28;
  Button5.Height          = 28;
  Button5.Pen_Width       = 1;
  Button5.Pen_Color       = 0x27FF;
  Button5.Visible         = 1;
  Button5.Active          = 1;
  Button5.Transparent     = 1;
  Button5.Caption         = Button5_Caption;
  Button5.TextAlign             = _taCenter;
  Button5.FontName        = Tahoma13x13_Bold;
  Button5.PressColEnabled = 1;
  Button5.Font_Color      = 0x37FF;
  Button5.Gradient        = 0;
  Button5.Gradient_Orientation    = 0;
  Button5.Gradient_Start_Color    = 0x0013;
  Button5.Gradient_End_Color      = 0x0010;
  Button5.Color           = 0x0010;
  Button5.Press_Color     = 0x1FFF;
  Button5.OnUpPtr         = 0;
  Button5.OnDownPtr       = 0;
  Button5.OnClickPtr      = Button5OnClick;
  Button5.OnPressPtr      = 0;

  BoxLightIn02.OwnerScreen     = &Screen1;
  BoxLightIn02.Order           = 11;
  BoxLightIn02.Left            = 57;
  BoxLightIn02.Top             = 106;
  BoxLightIn02.Width           = 28;
  BoxLightIn02.Height          = 28;
  BoxLightIn02.Pen_Width       = 1;
  BoxLightIn02.Pen_Color       = 0x0000;
  BoxLightIn02.Visible         = 1;
  BoxLightIn02.Active          = 1;
  BoxLightIn02.Transparent     = 1;
  BoxLightIn02.Gradient        = 0;
  BoxLightIn02.Gradient_Orientation    = 0;
  BoxLightIn02.Gradient_Start_Color    = 0xF800;
  BoxLightIn02.Gradient_End_Color      = 0xF800;
  BoxLightIn02.Color           = 0xFFE0;
  BoxLightIn02.PressColEnabled     = 0;
  BoxLightIn02.Press_Color     = 0xF800;
  BoxLightIn02.OnUpPtr         = 0;
  BoxLightIn02.OnDownPtr       = 0;
  BoxLightIn02.OnClickPtr      = BoxLightIn02OnClick;
  BoxLightIn02.OnPressPtr      = 0;

  CircleLightOut02.OwnerScreen     = &Screen1;
  CircleLightOut02.Order           = 12;
  CircleLightOut02.Left            = 166;
  CircleLightOut02.Top             = 8;
  CircleLightOut02.Radius          = 14;
  CircleLightOut02.Pen_Width       = 1;
  CircleLightOut02.Pen_Color       = 0x0000;
  CircleLightOut02.Visible         = 1;
  CircleLightOut02.Active          = 1;
  CircleLightOut02.Transparent     = 1;
  CircleLightOut02.Gradient        = 0;
  CircleLightOut02.Gradient_Orientation    = 0;
  CircleLightOut02.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut02.Gradient_End_Color      = 0xC618;
  CircleLightOut02.Color           = 0xFFE0;
  CircleLightOut02.PressColEnabled = 0;
  CircleLightOut02.Press_Color     = 0xF800;
  CircleLightOut02.OnUpPtr         = 0;
  CircleLightOut02.OnDownPtr       = 0;
  CircleLightOut02.OnClickPtr      = CircleLightOut02OnClick;
  CircleLightOut02.OnPressPtr      = 0;

  CircleLightOut03.OwnerScreen     = &Screen1;
  CircleLightOut03.Order           = 13;
  CircleLightOut03.Left            = 166;
  CircleLightOut03.Top             = 74;
  CircleLightOut03.Radius          = 14;
  CircleLightOut03.Pen_Width       = 1;
  CircleLightOut03.Pen_Color       = 0x0000;
  CircleLightOut03.Visible         = 1;
  CircleLightOut03.Active          = 1;
  CircleLightOut03.Transparent     = 1;
  CircleLightOut03.Gradient        = 0;
  CircleLightOut03.Gradient_Orientation    = 0;
  CircleLightOut03.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut03.Gradient_End_Color      = 0xC618;
  CircleLightOut03.Color           = 0xFFE0;
  CircleLightOut03.PressColEnabled = 0;
  CircleLightOut03.Press_Color     = 0xF800;
  CircleLightOut03.OnUpPtr         = 0;
  CircleLightOut03.OnDownPtr       = 0;
  CircleLightOut03.OnClickPtr      = CircleLightOut03OnClick;
  CircleLightOut03.OnPressPtr      = 0;

  CircleLightOut04.OwnerScreen     = &Screen1;
  CircleLightOut04.Order           = 14;
  CircleLightOut04.Left            = 179;
  CircleLightOut04.Top             = 208;
  CircleLightOut04.Radius          = 14;
  CircleLightOut04.Pen_Width       = 1;
  CircleLightOut04.Pen_Color       = 0x0000;
  CircleLightOut04.Visible         = 1;
  CircleLightOut04.Active          = 1;
  CircleLightOut04.Transparent     = 1;
  CircleLightOut04.Gradient        = 0;
  CircleLightOut04.Gradient_Orientation    = 0;
  CircleLightOut04.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut04.Gradient_End_Color      = 0xC618;
  CircleLightOut04.Color           = 0xFFE0;
  CircleLightOut04.PressColEnabled = 0;
  CircleLightOut04.Press_Color     = 0xF800;
  CircleLightOut04.OnUpPtr         = 0;
  CircleLightOut04.OnDownPtr       = 0;
  CircleLightOut04.OnClickPtr      = CircleLightOut04OnClick;
  CircleLightOut04.OnPressPtr      = 0;

  CircleLightOut05.OwnerScreen     = &Screen1;
  CircleLightOut05.Order           = 15;
  CircleLightOut05.Left            = 8;
  CircleLightOut05.Top             = 208;
  CircleLightOut05.Radius          = 14;
  CircleLightOut05.Pen_Width       = 1;
  CircleLightOut05.Pen_Color       = 0x0000;
  CircleLightOut05.Visible         = 1;
  CircleLightOut05.Active          = 1;
  CircleLightOut05.Transparent     = 1;
  CircleLightOut05.Gradient        = 0;
  CircleLightOut05.Gradient_Orientation    = 0;
  CircleLightOut05.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut05.Gradient_End_Color      = 0xC618;
  CircleLightOut05.Color           = 0xFFE0;
  CircleLightOut05.PressColEnabled = 0;
  CircleLightOut05.Press_Color     = 0xFFE0;
  CircleLightOut05.OnUpPtr         = 0;
  CircleLightOut05.OnDownPtr       = 0;
  CircleLightOut05.OnClickPtr      = CircleLightOut05OnClick;
  CircleLightOut05.OnPressPtr      = 0;

  CircleLightOut06.OwnerScreen     = &Screen1;
  CircleLightOut06.Order           = 16;
  CircleLightOut06.Left            = 8;
  CircleLightOut06.Top             = 88;
  CircleLightOut06.Radius          = 14;
  CircleLightOut06.Pen_Width       = 1;
  CircleLightOut06.Pen_Color       = 0x0000;
  CircleLightOut06.Visible         = 1;
  CircleLightOut06.Active          = 1;
  CircleLightOut06.Transparent     = 1;
  CircleLightOut06.Gradient        = 0;
  CircleLightOut06.Gradient_Orientation    = 0;
  CircleLightOut06.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut06.Gradient_End_Color      = 0xC618;
  CircleLightOut06.Color           = 0xFFE0;
  CircleLightOut06.PressColEnabled = 0;
  CircleLightOut06.Press_Color     = 0xF800;
  CircleLightOut06.OnUpPtr         = 0;
  CircleLightOut06.OnDownPtr       = 0;
  CircleLightOut06.OnClickPtr      = CircleLightOut06OnClick;
  CircleLightOut06.OnPressPtr      = 0;

  CircleLightOut01.OwnerScreen     = &Screen1;
  CircleLightOut01.Order           = 17;
  CircleLightOut01.Left            = 11;
  CircleLightOut01.Top             = 5;
  CircleLightOut01.Radius          = 14;
  CircleLightOut01.Pen_Width       = 1;
  CircleLightOut01.Pen_Color       = 0x0000;
  CircleLightOut01.Visible         = 1;
  CircleLightOut01.Active          = 1;
  CircleLightOut01.Transparent     = 1;
  CircleLightOut01.Gradient        = 0;
  CircleLightOut01.Gradient_Orientation    = 0;
  CircleLightOut01.Gradient_Start_Color    = 0xFFFF;
  CircleLightOut01.Gradient_End_Color      = 0xC618;
  CircleLightOut01.Color           = 0xFFE0;
  CircleLightOut01.PressColEnabled = 0;
  CircleLightOut01.Press_Color     = 0xF800;
  CircleLightOut01.OnUpPtr         = 0;
  CircleLightOut01.OnDownPtr       = 0;
  CircleLightOut01.OnClickPtr      = CircleLightOut01OnClick;
  CircleLightOut01.OnPressPtr      = 0;

  Button6.OwnerScreen     = &Screen1;
  Button6.Order           = 18;
  Button6.Left            = 218;
  Button6.Top             = 147;
  Button6.Width           = 28;
  Button6.Height          = 28;
  Button6.Pen_Width       = 1;
  Button6.Pen_Color       = 0x27FF;
  Button6.Visible         = 1;
  Button6.Active          = 1;
  Button6.Transparent     = 1;
  Button6.Caption         = Button6_Caption;
  Button6.TextAlign             = _taCenter;
  Button6.FontName        = Tahoma13x13_Bold;
  Button6.PressColEnabled = 1;
  Button6.Font_Color      = 0x37FF;
  Button6.Gradient        = 0;
  Button6.Gradient_Orientation    = 0;
  Button6.Gradient_Start_Color    = 0x0013;
  Button6.Gradient_End_Color      = 0x0010;
  Button6.Color           = 0x0010;
  Button6.Press_Color     = 0x1FFF;
  Button6.OnUpPtr         = 0;
  Button6.OnDownPtr       = 0;
  Button6.OnClickPtr      = Button6OnClick;
  Button6.OnPressPtr      = 0;

  Button7.OwnerScreen     = &Screen1;
  Button7.Order           = 19;
  Button7.Left            = 253;
  Button7.Top             = 147;
  Button7.Width           = 28;
  Button7.Height          = 28;
  Button7.Pen_Width       = 1;
  Button7.Pen_Color       = 0x27FF;
  Button7.Visible         = 1;
  Button7.Active          = 1;
  Button7.Transparent     = 1;
  Button7.Caption         = Button7_Caption;
  Button7.TextAlign             = _taCenter;
  Button7.FontName        = Tahoma13x13_Bold;
  Button7.PressColEnabled = 1;
  Button7.Font_Color      = 0x37FF;
  Button7.Gradient        = 0;
  Button7.Gradient_Orientation    = 0;
  Button7.Gradient_Start_Color    = 0x0013;
  Button7.Gradient_End_Color      = 0x0010;
  Button7.Color           = 0x0010;
  Button7.Press_Color     = 0x1FFF;
  Button7.OnUpPtr         = 0;
  Button7.OnDownPtr       = 0;
  Button7.OnClickPtr      = Button7OnClick;
  Button7.OnPressPtr      = 0;

  Button8.OwnerScreen     = &Screen1;
  Button8.Order           = 20;
  Button8.Left            = 288;
  Button8.Top             = 147;
  Button8.Width           = 28;
  Button8.Height          = 28;
  Button8.Pen_Width       = 1;
  Button8.Pen_Color       = 0x27FF;
  Button8.Visible         = 1;
  Button8.Active          = 1;
  Button8.Transparent     = 1;
  Button8.Caption         = Button8_Caption;
  Button8.TextAlign             = _taCenter;
  Button8.FontName        = Tahoma13x13_Bold;
  Button8.PressColEnabled = 1;
  Button8.Font_Color      = 0x37FF;
  Button8.Gradient        = 0;
  Button8.Gradient_Orientation    = 0;
  Button8.Gradient_Start_Color    = 0x0013;
  Button8.Gradient_End_Color      = 0x0010;
  Button8.Color           = 0x0010;
  Button8.Press_Color     = 0x1FFF;
  Button8.OnUpPtr         = 0;
  Button8.OnDownPtr       = 0;
  Button8.OnClickPtr      = Button8OnClick;
  Button8.OnPressPtr      = 0;

  BtnBackSetLights.OwnerScreen     = &Screen1;
  BtnBackSetLights.Order           = 21;
  BtnBackSetLights.Left            = 219;
  BtnBackSetLights.Top             = 193;
  BtnBackSetLights.Width           = 95;
  BtnBackSetLights.Height          = 40;
  BtnBackSetLights.Pen_Width       = 1;
  BtnBackSetLights.Pen_Color       = 0x07FF;
  BtnBackSetLights.Visible         = 1;
  BtnBackSetLights.Active          = 1;
  BtnBackSetLights.Transparent     = 1;
  BtnBackSetLights.Caption         = BtnBackSetLights_Caption;
  BtnBackSetLights.TextAlign             = _taCenter;
  BtnBackSetLights.FontName        = Tahoma25x25_Bold;
  BtnBackSetLights.PressColEnabled = 1;
  BtnBackSetLights.Font_Color      = 0x07FF;
  BtnBackSetLights.Gradient        = 0;
  BtnBackSetLights.Gradient_Orientation    = 0;
  BtnBackSetLights.Gradient_Start_Color    = 0xFFFF;
  BtnBackSetLights.Gradient_End_Color      = 0xC618;
  BtnBackSetLights.Color           = 0x0010;
  BtnBackSetLights.Press_Color     = 0x07FF;
  BtnBackSetLights.Corner_Radius      = 3;
  BtnBackSetLights.OnUpPtr         = 0;
  BtnBackSetLights.OnDownPtr       = 0;
  BtnBackSetLights.OnClickPtr      = BtnBackSetLightsOnClick;
  BtnBackSetLights.OnPressPtr      = 0;

  Button10.OwnerScreen     = &Screen2;
  Button10.Order           = 0;
  Button10.Left            = 65;
  Button10.Top             = 77;
  Button10.Width           = 190;
  Button10.Height          = 25;
  Button10.Pen_Width       = 1;
  Button10.Pen_Color       = 0x27FF;
  Button10.Visible         = 1;
  Button10.Active          = 1;
  Button10.Transparent     = 1;
  Button10.Caption         = Button10_Caption;
  Button10.TextAlign             = _taCenter;
  Button10.FontName        = Tahoma11x13_Regular;
  Button10.PressColEnabled = 1;
  Button10.Font_Color      = 0x37FF;
  Button10.Gradient        = 0;
  Button10.Gradient_Orientation    = 0;
  Button10.Gradient_Start_Color    = 0x0013;
  Button10.Gradient_End_Color      = 0x0010;
  Button10.Color           = 0x0010;
  Button10.Press_Color     = 0x1FFF;
  Button10.OnUpPtr         = 0;
  Button10.OnDownPtr       = 0;
  Button10.OnClickPtr      = Button10OnClick;
  Button10.OnPressPtr      = 0;

  Label1.OwnerScreen     = &Screen2;
  Label1.Order          = 1;
  Label1.Left           = 29;
  Label1.Top            = 18;
  Label1.Width          = 272;
  Label1.Height         = 32;
  Label1.Visible        = 1;
  Label1.Active         = 1;
  Label1.Caption        = Label1_Caption;
  Label1.FontName       = Tahoma24x29_Regular;
  Label1.Font_Color     = 0x07FF;
  Label1.OnUpPtr         = 0;
  Label1.OnDownPtr       = 0;
  Label1.OnClickPtr      = 0;
  Label1.OnPressPtr      = 0;

  BtnSetArc.OwnerScreen     = &Screen2;
  BtnSetArc.Order           = 2;
  BtnSetArc.Left            = 65;
  BtnSetArc.Top             = 113;
  BtnSetArc.Width           = 190;
  BtnSetArc.Height          = 25;
  BtnSetArc.Pen_Width       = 1;
  BtnSetArc.Pen_Color       = 0x27FF;
  BtnSetArc.Visible         = 1;
  BtnSetArc.Active          = 1;
  BtnSetArc.Transparent     = 1;
  BtnSetArc.Caption         = BtnSetArc_Caption;
  BtnSetArc.TextAlign             = _taCenter;
  BtnSetArc.FontName        = Tahoma11x13_Regular;
  BtnSetArc.PressColEnabled = 1;
  BtnSetArc.Font_Color      = 0x37FF;
  BtnSetArc.Gradient        = 0;
  BtnSetArc.Gradient_Orientation    = 0;
  BtnSetArc.Gradient_Start_Color    = 0xA53F;
  BtnSetArc.Gradient_End_Color      = 0x0010;
  BtnSetArc.Color           = 0x0010;
  BtnSetArc.Press_Color     = 0x1FFF;
  BtnSetArc.OnUpPtr         = 0;
  BtnSetArc.OnDownPtr       = 0;
  BtnSetArc.OnClickPtr      = BtnSetArcOnClick;
  BtnSetArc.OnPressPtr      = 0;

  BtnSendQueryCmd.OwnerScreen     = &Screen2;
  BtnSendQueryCmd.Order           = 3;
  BtnSendQueryCmd.Left            = 65;
  BtnSendQueryCmd.Top             = 149;
  BtnSendQueryCmd.Width           = 190;
  BtnSendQueryCmd.Height          = 25;
  BtnSendQueryCmd.Pen_Width       = 1;
  BtnSendQueryCmd.Pen_Color       = 0x27FF;
  BtnSendQueryCmd.Visible         = 1;
  BtnSendQueryCmd.Active          = 1;
  BtnSendQueryCmd.Transparent     = 1;
  BtnSendQueryCmd.Caption         = BtnSendQueryCmd_Caption;
  BtnSendQueryCmd.TextAlign             = _taCenter;
  BtnSendQueryCmd.FontName        = Tahoma11x13_Regular;
  BtnSendQueryCmd.PressColEnabled = 1;
  BtnSendQueryCmd.Font_Color      = 0x37FF;
  BtnSendQueryCmd.Gradient        = 0;
  BtnSendQueryCmd.Gradient_Orientation    = 0;
  BtnSendQueryCmd.Gradient_Start_Color    = 0x0013;
  BtnSendQueryCmd.Gradient_End_Color      = 0x0010;
  BtnSendQueryCmd.Color           = 0x0010;
  BtnSendQueryCmd.Press_Color     = 0x1FFF;
  BtnSendQueryCmd.OnUpPtr         = 0;
  BtnSendQueryCmd.OnDownPtr       = 0;
  BtnSendQueryCmd.OnClickPtr      = BtnSendQueryCmdOnClick;
  BtnSendQueryCmd.OnPressPtr      = 0;

  BtnSetSceneLevel.OwnerScreen     = &Screen2;
  BtnSetSceneLevel.Order           = 4;
  BtnSetSceneLevel.Left            = 65;
  BtnSetSceneLevel.Top             = 185;
  BtnSetSceneLevel.Width           = 190;
  BtnSetSceneLevel.Height          = 25;
  BtnSetSceneLevel.Pen_Width       = 1;
  BtnSetSceneLevel.Pen_Color       = 0x27FF;
  BtnSetSceneLevel.Visible         = 1;
  BtnSetSceneLevel.Active          = 1;
  BtnSetSceneLevel.Transparent     = 1;
  BtnSetSceneLevel.Caption         = BtnSetSceneLevel_Caption;
  BtnSetSceneLevel.TextAlign             = _taCenter;
  BtnSetSceneLevel.FontName        = Tahoma11x13_Regular;
  BtnSetSceneLevel.PressColEnabled = 1;
  BtnSetSceneLevel.Font_Color      = 0x37FF;
  BtnSetSceneLevel.Gradient        = 0;
  BtnSetSceneLevel.Gradient_Orientation    = 0;
  BtnSetSceneLevel.Gradient_Start_Color    = 0x0013;
  BtnSetSceneLevel.Gradient_End_Color      = 0x0010;
  BtnSetSceneLevel.Color           = 0x0010;
  BtnSetSceneLevel.Press_Color     = 0x1FFF;
  BtnSetSceneLevel.OnUpPtr         = 0;
  BtnSetSceneLevel.OnDownPtr       = 0;
  BtnSetSceneLevel.OnClickPtr      = BtnSetSceneLevelOnClick;
  BtnSetSceneLevel.OnPressPtr      = 0;

  Label14.OwnerScreen     = &Screen2;
  Label14.Order          = 5;
  Label14.Left           = 3;
  Label14.Top            = 223;
  Label14.Width          = 83;
  Label14.Height         = 15;
  Label14.Visible        = 1;
  Label14.Active         = 1;
  Label14.Caption        = Label14_Caption;
  Label14.FontName       = Tahoma11x13_Regular;
  Label14.Font_Color     = 0x07FF;
  Label14.OnUpPtr         = 0;
  Label14.OnDownPtr       = 0;
  Label14.OnClickPtr      = 0;
  Label14.OnPressPtr      = 0;

  Image2.OwnerScreen     = &Screen3;
  Image2.Order          = 0;
  Image2.Left           = 30;
  Image2.Top            = 16;
  Image2.Width          = 148;
  Image2.Height         = 199;
  Image2.Picture_Type   = 0;
  Image2.Picture_Ratio  = 1;
  Image2.Picture_Name   = first_floor_bmp;
  Image2.Visible        = 1;
  Image2.Active         = 1;
  Image2.OnUpPtr         = 0;
  Image2.OnDownPtr       = 0;
  Image2.OnClickPtr      = 0;
  Image2.OnPressPtr      = 0;

  Box4.OwnerScreen     = &Screen3;
  Box4.Order           = 1;
  Box4.Left            = 133;
  Box4.Top             = 175;
  Box4.Width           = 28;
  Box4.Height          = 28;
  Box4.Pen_Width       = 0;
  Box4.Pen_Color       = 0x0000;
  Box4.Visible         = 1;
  Box4.Active          = 1;
  Box4.Transparent     = 1;
  Box4.Gradient        = 0;
  Box4.Gradient_Orientation    = 0;
  Box4.Gradient_Start_Color    = 0xF800;
  Box4.Gradient_End_Color      = 0xF800;
  Box4.Color           = 0xFFE0;
  Box4.PressColEnabled     = 0;
  Box4.Press_Color     = 0xF800;
  Box4.OnUpPtr         = 0;
  Box4.OnDownPtr       = 0;
  Box4.OnClickPtr      = Box4OnClick;
  Box4.OnPressPtr      = 0;

  Box3.OwnerScreen     = &Screen3;
  Box3.Order           = 2;
  Box3.Left            = 55;
  Box3.Top             = 175;
  Box3.Width           = 28;
  Box3.Height          = 28;
  Box3.Pen_Width       = 0;
  Box3.Pen_Color       = 0x0000;
  Box3.Visible         = 1;
  Box3.Active          = 1;
  Box3.Transparent     = 1;
  Box3.Gradient        = 0;
  Box3.Gradient_Orientation    = 0;
  Box3.Gradient_Start_Color    = 0xF800;
  Box3.Gradient_End_Color      = 0xF800;
  Box3.Color           = 0xFFE0;
  Box3.PressColEnabled     = 0;
  Box3.Press_Color     = 0xF800;
  Box3.OnUpPtr         = 0;
  Box3.OnDownPtr       = 0;
  Box3.OnClickPtr      = Box3OnClick;
  Box3.OnPressPtr      = 0;

  Box1.OwnerScreen     = &Screen3;
  Box1.Order           = 3;
  Box1.Left            = 63;
  Box1.Top             = 32;
  Box1.Width           = 28;
  Box1.Height          = 28;
  Box1.Pen_Width       = 1;
  Box1.Pen_Color       = 0x0000;
  Box1.Visible         = 1;
  Box1.Active          = 1;
  Box1.Transparent     = 1;
  Box1.Gradient        = 0;
  Box1.Gradient_Orientation    = 0;
  Box1.Gradient_Start_Color    = 0xF800;
  Box1.Gradient_End_Color      = 0xF800;
  Box1.Color           = 0xFFE0;
  Box1.PressColEnabled     = 0;
  Box1.Press_Color     = 0xF800;
  Box1.OnUpPtr         = 0;
  Box1.OnDownPtr       = 0;
  Box1.OnClickPtr      = Box1OnClick;
  Box1.OnPressPtr      = 0;

  Box2.OwnerScreen     = &Screen3;
  Box2.Order           = 4;
  Box2.Left            = 52;
  Box2.Top             = 100;
  Box2.Width           = 28;
  Box2.Height          = 28;
  Box2.Pen_Width       = 1;
  Box2.Pen_Color       = 0x0000;
  Box2.Visible         = 1;
  Box2.Active          = 1;
  Box2.Transparent     = 1;
  Box2.Gradient        = 0;
  Box2.Gradient_Orientation    = 0;
  Box2.Gradient_Start_Color    = 0xF800;
  Box2.Gradient_End_Color      = 0xF800;
  Box2.Color           = 0xFFE0;
  Box2.PressColEnabled     = 0;
  Box2.Press_Color     = 0xF800;
  Box2.OnUpPtr         = 0;
  Box2.OnDownPtr       = 0;
  Box2.OnClickPtr      = Box2OnClick;
  Box2.OnPressPtr      = 0;

  Circle2.OwnerScreen     = &Screen3;
  Circle2.Order           = 5;
  Circle2.Left            = 160;
  Circle2.Top             = 8;
  Circle2.Radius          = 14;
  Circle2.Pen_Width       = 1;
  Circle2.Pen_Color       = 0x0000;
  Circle2.Visible         = 1;
  Circle2.Active          = 1;
  Circle2.Transparent     = 1;
  Circle2.Gradient        = 0;
  Circle2.Gradient_Orientation    = 0;
  Circle2.Gradient_Start_Color    = 0xFFFF;
  Circle2.Gradient_End_Color      = 0xC618;
  Circle2.Color           = 0xFFE0;
  Circle2.PressColEnabled = 0;
  Circle2.Press_Color     = 0xF800;
  Circle2.OnUpPtr         = 0;
  Circle2.OnDownPtr       = 0;
  Circle2.OnClickPtr      = Circle2OnClick;
  Circle2.OnPressPtr      = 0;

  Circle3.OwnerScreen     = &Screen3;
  Circle3.Order           = 6;
  Circle3.Left            = 162;
  Circle3.Top             = 72;
  Circle3.Radius          = 14;
  Circle3.Pen_Width       = 1;
  Circle3.Pen_Color       = 0x0000;
  Circle3.Visible         = 1;
  Circle3.Active          = 1;
  Circle3.Transparent     = 1;
  Circle3.Gradient        = 0;
  Circle3.Gradient_Orientation    = 0;
  Circle3.Gradient_Start_Color    = 0xFFFF;
  Circle3.Gradient_End_Color      = 0xC618;
  Circle3.Color           = 0xFFE0;
  Circle3.PressColEnabled = 0;
  Circle3.Press_Color     = 0xF800;
  Circle3.OnUpPtr         = 0;
  Circle3.OnDownPtr       = 0;
  Circle3.OnClickPtr      = Circle3OnClick;
  Circle3.OnPressPtr      = 0;

  Circle4.OwnerScreen     = &Screen3;
  Circle4.Order           = 7;
  Circle4.Left            = 179;
  Circle4.Top             = 203;
  Circle4.Radius          = 14;
  Circle4.Pen_Width       = 1;
  Circle4.Pen_Color       = 0x0000;
  Circle4.Visible         = 1;
  Circle4.Active          = 1;
  Circle4.Transparent     = 1;
  Circle4.Gradient        = 0;
  Circle4.Gradient_Orientation    = 0;
  Circle4.Gradient_Start_Color    = 0xFFFF;
  Circle4.Gradient_End_Color      = 0xC618;
  Circle4.Color           = 0xFFE0;
  Circle4.PressColEnabled = 0;
  Circle4.Press_Color     = 0xF800;
  Circle4.OnUpPtr         = 0;
  Circle4.OnDownPtr       = 0;
  Circle4.OnClickPtr      = Circle4OnClick;
  Circle4.OnPressPtr      = 0;

  Circle5.OwnerScreen     = &Screen3;
  Circle5.Order           = 8;
  Circle5.Left            = 6;
  Circle5.Top             = 203;
  Circle5.Radius          = 14;
  Circle5.Pen_Width       = 1;
  Circle5.Pen_Color       = 0x0000;
  Circle5.Visible         = 1;
  Circle5.Active          = 1;
  Circle5.Transparent     = 1;
  Circle5.Gradient        = 0;
  Circle5.Gradient_Orientation    = 0;
  Circle5.Gradient_Start_Color    = 0xFFFF;
  Circle5.Gradient_End_Color      = 0xC618;
  Circle5.Color           = 0xFFE0;
  Circle5.PressColEnabled = 0;
  Circle5.Press_Color     = 0xFFE0;
  Circle5.OnUpPtr         = 0;
  Circle5.OnDownPtr       = 0;
  Circle5.OnClickPtr      = Circle5OnClick;
  Circle5.OnPressPtr      = 0;

  Circle6.OwnerScreen     = &Screen3;
  Circle6.Order           = 9;
  Circle6.Left            = 8;
  Circle6.Top             = 88;
  Circle6.Radius          = 14;
  Circle6.Pen_Width       = 1;
  Circle6.Pen_Color       = 0x0000;
  Circle6.Visible         = 1;
  Circle6.Active          = 1;
  Circle6.Transparent     = 1;
  Circle6.Gradient        = 0;
  Circle6.Gradient_Orientation    = 0;
  Circle6.Gradient_Start_Color    = 0xFFFF;
  Circle6.Gradient_End_Color      = 0xC618;
  Circle6.Color           = 0xFFE0;
  Circle6.PressColEnabled = 0;
  Circle6.Press_Color     = 0xF800;
  Circle6.OnUpPtr         = 0;
  Circle6.OnDownPtr       = 0;
  Circle6.OnClickPtr      = Circle6OnClick;
  Circle6.OnPressPtr      = 0;

  Circle1.OwnerScreen     = &Screen3;
  Circle1.Order           = 10;
  Circle1.Left            = 11;
  Circle1.Top             = 5;
  Circle1.Radius          = 14;
  Circle1.Pen_Width       = 1;
  Circle1.Pen_Color       = 0x0000;
  Circle1.Visible         = 1;
  Circle1.Active          = 1;
  Circle1.Transparent     = 1;
  Circle1.Gradient        = 0;
  Circle1.Gradient_Orientation    = 0;
  Circle1.Gradient_Start_Color    = 0xFFFF;
  Circle1.Gradient_End_Color      = 0xC618;
  Circle1.Color           = 0xFFE0;
  Circle1.PressColEnabled = 0;
  Circle1.Press_Color     = 0xF800;
  Circle1.OnUpPtr         = 0;
  Circle1.OnDownPtr       = 0;
  Circle1.OnClickPtr      = Circle1OnClick;
  Circle1.OnPressPtr      = 0;

  Box5.OwnerScreen     = &Screen3;
  Box5.Order           = 11;
  Box5.Left            = 211;
  Box5.Top             = 1;
  Box5.Width           = 111;
  Box5.Height          = 250;
  Box5.Pen_Width       = 1;
  Box5.Pen_Color       = 0x27FF;
  Box5.Visible         = 1;
  Box5.Active          = 1;
  Box5.Transparent     = 1;
  Box5.Gradient        = 0;
  Box5.Gradient_Orientation    = 0;
  Box5.Gradient_Start_Color    = 0xFFFF;
  Box5.Gradient_End_Color      = 0xC618;
  Box5.Color           = 0x000D;
  Box5.PressColEnabled     = 0;
  Box5.Press_Color     = 0x8410;
  Box5.OnUpPtr         = 0;
  Box5.OnDownPtr       = 0;
  Box5.OnClickPtr      = 0;
  Box5.OnPressPtr      = 0;

  Label2.OwnerScreen     = &Screen3;
  Label2.Order          = 12;
  Label2.Left           = 4;
  Label2.Top            = 28;
  Label2.Width          = 12;
  Label2.Height         = 15;
  Label2.Visible        = 1;
  Label2.Active         = 1;
  Label2.Caption        = Label2_Caption;
  Label2.FontName       = Tahoma11x13_Regular;
  Label2.Font_Color     = 0x07FF;
  Label2.OnUpPtr         = 0;
  Label2.OnDownPtr       = 0;
  Label2.OnClickPtr      = 0;
  Label2.OnPressPtr      = 0;

  Label3.OwnerScreen     = &Screen3;
  Label3.Order          = 13;
  Label3.Left           = 186;
  Label3.Top            = 28;
  Label3.Width          = 12;
  Label3.Height         = 15;
  Label3.Visible        = 1;
  Label3.Active         = 1;
  Label3.Caption        = Label3_Caption;
  Label3.FontName       = Tahoma11x13_Regular;
  Label3.Font_Color     = 0x07FF;
  Label3.OnUpPtr         = 0;
  Label3.OnDownPtr       = 0;
  Label3.OnClickPtr      = 0;
  Label3.OnPressPtr      = 0;

  Label5.OwnerScreen     = &Screen3;
  Label5.Order          = 14;
  Label5.Left           = 186;
  Label5.Top            = 96;
  Label5.Width          = 12;
  Label5.Height         = 15;
  Label5.Visible        = 1;
  Label5.Active         = 1;
  Label5.Caption        = Label5_Caption;
  Label5.FontName       = Tahoma11x13_Regular;
  Label5.Font_Color     = 0x07FF;
  Label5.OnUpPtr         = 0;
  Label5.OnDownPtr       = 0;
  Label5.OnClickPtr      = 0;
  Label5.OnPressPtr      = 0;

  Label6.OwnerScreen     = &Screen3;
  Label6.Order          = 15;
  Label6.Left           = 170;
  Label6.Top            = 226;
  Label6.Width          = 12;
  Label6.Height         = 15;
  Label6.Visible        = 1;
  Label6.Active         = 1;
  Label6.Caption        = Label6_Caption;
  Label6.FontName       = Tahoma11x13_Regular;
  Label6.Font_Color     = 0x07FF;
  Label6.OnUpPtr         = 0;
  Label6.OnDownPtr       = 0;
  Label6.OnClickPtr      = 0;
  Label6.OnPressPtr      = 0;

  Label7.OwnerScreen     = &Screen3;
  Label7.Order          = 16;
  Label7.Left           = 31;
  Label7.Top            = 225;
  Label7.Width          = 12;
  Label7.Height         = 15;
  Label7.Visible        = 1;
  Label7.Active         = 1;
  Label7.Caption        = Label7_Caption;
  Label7.FontName       = Tahoma11x13_Regular;
  Label7.Font_Color     = 0x07FF;
  Label7.OnUpPtr         = 0;
  Label7.OnDownPtr       = 0;
  Label7.OnClickPtr      = 0;
  Label7.OnPressPtr      = 0;

  Label8.OwnerScreen     = &Screen3;
  Label8.Order          = 17;
  Label8.Left           = 5;
  Label8.Top            = 110;
  Label8.Width          = 12;
  Label8.Height         = 15;
  Label8.Visible        = 1;
  Label8.Active         = 1;
  Label8.Caption        = Label8_Caption;
  Label8.FontName       = Tahoma11x13_Regular;
  Label8.Font_Color     = 0x07FF;
  Label8.OnUpPtr         = 0;
  Label8.OnDownPtr       = 0;
  Label8.OnClickPtr      = 0;
  Label8.OnPressPtr      = 0;

  Label9.OwnerScreen     = &Screen3;
  Label9.Order          = 18;
  Label9.Left           = 93;
  Label9.Top            = 57;
  Label9.Width          = 12;
  Label9.Height         = 15;
  Label9.Visible        = 1;
  Label9.Active         = 1;
  Label9.Caption        = Label9_Caption;
  Label9.FontName       = Tahoma11x13_Regular;
  Label9.Font_Color     = 0xF980;
  Label9.OnUpPtr         = 0;
  Label9.OnDownPtr       = 0;
  Label9.OnClickPtr      = 0;
  Label9.OnPressPtr      = 0;

  Label10.OwnerScreen     = &Screen3;
  Label10.Order          = 19;
  Label10.Left           = 80;
  Label10.Top            = 121;
  Label10.Width          = 12;
  Label10.Height         = 15;
  Label10.Visible        = 1;
  Label10.Active         = 1;
  Label10.Caption        = Label10_Caption;
  Label10.FontName       = Tahoma11x13_Regular;
  Label10.Font_Color     = 0xF980;
  Label10.OnUpPtr         = 0;
  Label10.OnDownPtr       = 0;
  Label10.OnClickPtr      = 0;
  Label10.OnPressPtr      = 0;

  Label11.OwnerScreen     = &Screen3;
  Label11.Order          = 20;
  Label11.Left           = 47;
  Label11.Top            = 163;
  Label11.Width          = 12;
  Label11.Height         = 15;
  Label11.Visible        = 1;
  Label11.Active         = 1;
  Label11.Caption        = Label11_Caption;
  Label11.FontName       = Tahoma11x13_Regular;
  Label11.Font_Color     = 0xF980;
  Label11.OnUpPtr         = 0;
  Label11.OnDownPtr       = 0;
  Label11.OnClickPtr      = 0;
  Label11.OnPressPtr      = 0;

  Label12.OwnerScreen     = &Screen3;
  Label12.Order          = 21;
  Label12.Left           = 121;
  Label12.Top            = 193;
  Label12.Width          = 12;
  Label12.Height         = 15;
  Label12.Visible        = 1;
  Label12.Active         = 1;
  Label12.Caption        = Label12_Caption;
  Label12.FontName       = Tahoma11x13_Regular;
  Label12.Font_Color     = 0xF980;
  Label12.OnUpPtr         = 0;
  Label12.OnDownPtr       = 0;
  Label12.OnClickPtr      = 0;
  Label12.OnPressPtr      = 0;

  BtnArcLevelDown.OwnerScreen     = &Screen3;
  BtnArcLevelDown.Order           = 22;
  BtnArcLevelDown.Left            = 220;
  BtnArcLevelDown.Top             = 146;
  BtnArcLevelDown.Width           = 95;
  BtnArcLevelDown.Height          = 40;
  BtnArcLevelDown.Pen_Width       = 1;
  BtnArcLevelDown.Pen_Color       = 0x07FF;
  BtnArcLevelDown.Visible         = 1;
  BtnArcLevelDown.Active          = 1;
  BtnArcLevelDown.Transparent     = 1;
  BtnArcLevelDown.Caption         = BtnArcLevelDown_Caption;
  BtnArcLevelDown.TextAlign             = _taCenter;
  BtnArcLevelDown.FontName        = Tahoma25x25_Bold;
  BtnArcLevelDown.PressColEnabled = 1;
  BtnArcLevelDown.Font_Color      = 0x07FF;
  BtnArcLevelDown.Gradient        = 0;
  BtnArcLevelDown.Gradient_Orientation    = 0;
  BtnArcLevelDown.Gradient_Start_Color    = 0xFFFF;
  BtnArcLevelDown.Gradient_End_Color      = 0xC618;
  BtnArcLevelDown.Color           = 0x0010;
  BtnArcLevelDown.Press_Color     = 0x07FF;
  BtnArcLevelDown.Corner_Radius      = 3;
  BtnArcLevelDown.OnUpPtr         = 0;
  BtnArcLevelDown.OnDownPtr       = 0;
  BtnArcLevelDown.OnClickPtr      = BtnArcLevelDownOnClick;
  BtnArcLevelDown.OnPressPtr      = 0;

  BtnArcLevelUp.OwnerScreen     = &Screen3;
  BtnArcLevelUp.Order           = 23;
  BtnArcLevelUp.Left            = 220;
  BtnArcLevelUp.Top             = 15;
  BtnArcLevelUp.Width           = 95;
  BtnArcLevelUp.Height          = 40;
  BtnArcLevelUp.Pen_Width       = 1;
  BtnArcLevelUp.Pen_Color       = 0x07FF;
  BtnArcLevelUp.Visible         = 1;
  BtnArcLevelUp.Active          = 1;
  BtnArcLevelUp.Transparent     = 1;
  BtnArcLevelUp.Caption         = BtnArcLevelUp_Caption;
  BtnArcLevelUp.TextAlign             = _taCenter;
  BtnArcLevelUp.FontName        = Tahoma25x25_Bold;
  BtnArcLevelUp.PressColEnabled = 1;
  BtnArcLevelUp.Font_Color      = 0x07FF;
  BtnArcLevelUp.Gradient        = 0;
  BtnArcLevelUp.Gradient_Orientation    = 0;
  BtnArcLevelUp.Gradient_Start_Color    = 0xFFFF;
  BtnArcLevelUp.Gradient_End_Color      = 0xC618;
  BtnArcLevelUp.Color           = 0x0010;
  BtnArcLevelUp.Press_Color     = 0x07FF;
  BtnArcLevelUp.Corner_Radius      = 3;
  BtnArcLevelUp.OnUpPtr         = 0;
  BtnArcLevelUp.OnDownPtr       = 0;
  BtnArcLevelUp.OnClickPtr      = BtnArcLevelUpOnClick;
  BtnArcLevelUp.OnPressPtr      = 0;

  ButtonRound3.OwnerScreen     = &Screen3;
  ButtonRound3.Order           = 24;
  ButtonRound3.Left            = 220;
  ButtonRound3.Top             = 197;
  ButtonRound3.Width           = 95;
  ButtonRound3.Height          = 40;
  ButtonRound3.Pen_Width       = 1;
  ButtonRound3.Pen_Color       = 0x07FF;
  ButtonRound3.Visible         = 1;
  ButtonRound3.Active          = 1;
  ButtonRound3.Transparent     = 1;
  ButtonRound3.Caption         = ButtonRound3_Caption;
  ButtonRound3.TextAlign             = _taCenter;
  ButtonRound3.FontName        = Tahoma25x25_Bold;
  ButtonRound3.PressColEnabled = 1;
  ButtonRound3.Font_Color      = 0x07FF;
  ButtonRound3.Gradient        = 0;
  ButtonRound3.Gradient_Orientation    = 0;
  ButtonRound3.Gradient_Start_Color    = 0xFFFF;
  ButtonRound3.Gradient_End_Color      = 0xC618;
  ButtonRound3.Color           = 0x0010;
  ButtonRound3.Press_Color     = 0x07FF;
  ButtonRound3.Corner_Radius      = 3;
  ButtonRound3.OnUpPtr         = 0;
  ButtonRound3.OnDownPtr       = 0;
  ButtonRound3.OnClickPtr      = ButtonRound3OnClick;
  ButtonRound3.OnPressPtr      = 0;

  BtnBallastArcLevel.OwnerScreen     = &Screen3;
  BtnBallastArcLevel.Order           = 25;
  BtnBallastArcLevel.Left            = 220;
  BtnBallastArcLevel.Top             = 64;
  BtnBallastArcLevel.Width           = 95;
  BtnBallastArcLevel.Height          = 72;
  BtnBallastArcLevel.Pen_Width       = 1;
  BtnBallastArcLevel.Pen_Color       = 0x07FF;
  BtnBallastArcLevel.Visible         = 1;
  BtnBallastArcLevel.Active          = 0;
  BtnBallastArcLevel.Transparent     = 1;
  BtnBallastArcLevel.Caption         = BtnBallastArcLevel_Caption;
  BtnBallastArcLevel.TextAlign             = _taCenter;
  BtnBallastArcLevel.FontName        = Tahoma38x39_Bold;
  BtnBallastArcLevel.PressColEnabled = 0;
  BtnBallastArcLevel.Font_Color      = 0x07FF;
  BtnBallastArcLevel.Gradient        = 0;
  BtnBallastArcLevel.Gradient_Orientation    = 0;
  BtnBallastArcLevel.Gradient_Start_Color    = 0xFFFF;
  BtnBallastArcLevel.Gradient_End_Color      = 0xC618;
  BtnBallastArcLevel.Color           = 0x0009;
  BtnBallastArcLevel.Press_Color     = 0x8410;
  BtnBallastArcLevel.Corner_Radius      = 3;
  BtnBallastArcLevel.OnUpPtr         = 0;
  BtnBallastArcLevel.OnDownPtr       = 0;
  BtnBallastArcLevel.OnClickPtr      = 0;
  BtnBallastArcLevel.OnPressPtr      = 0;

  btnBallastAddressText.OwnerScreen     = &ScreenSetScene;
  btnBallastAddressText.Order           = 8;
  btnBallastAddressText.Left            = 14;
  btnBallastAddressText.Top             = 59;
  btnBallastAddressText.Width           = 83;
  btnBallastAddressText.Height          = 54;
  btnBallastAddressText.Pen_Width       = 1;
  btnBallastAddressText.Pen_Color       = 0x07FF;
  btnBallastAddressText.Visible         = 1;
  btnBallastAddressText.Active          = 0;
  btnBallastAddressText.Transparent     = 1;
  btnBallastAddressText.Caption         = btnBallastAddressText_Caption;
  btnBallastAddressText.TextAlign             = _taCenter;
  btnBallastAddressText.FontName        = Tahoma38x39_Bold;
  btnBallastAddressText.PressColEnabled = 0;
  btnBallastAddressText.Font_Color      = 0x07FF;
  btnBallastAddressText.Gradient        = 0;
  btnBallastAddressText.Gradient_Orientation    = 0;
  btnBallastAddressText.Gradient_Start_Color    = 0xFFFF;
  btnBallastAddressText.Gradient_End_Color      = 0xC618;
  btnBallastAddressText.Color           = 0x000A;
  btnBallastAddressText.Press_Color     = 0x0010;
  btnBallastAddressText.Corner_Radius      = 3;
  btnBallastAddressText.OnUpPtr         = 0;
  btnBallastAddressText.OnDownPtr       = 0;
  btnBallastAddressText.OnClickPtr      = 0;
  btnBallastAddressText.OnPressPtr      = 0;

  btnSceneNumberText.OwnerScreen     = &ScreenSetScene;
  btnSceneNumberText.Order           = 9;
  btnSceneNumberText.Left            = 121;
  btnSceneNumberText.Top             = 60;
  btnSceneNumberText.Width           = 83;
  btnSceneNumberText.Height          = 54;
  btnSceneNumberText.Pen_Width       = 1;
  btnSceneNumberText.Pen_Color       = 0x07FF;
  btnSceneNumberText.Visible         = 1;
  btnSceneNumberText.Active          = 0;
  btnSceneNumberText.Transparent     = 1;
  btnSceneNumberText.Caption         = btnSceneNumberText_Caption;
  btnSceneNumberText.TextAlign             = _taCenter;
  btnSceneNumberText.FontName        = Tahoma38x39_Bold;
  btnSceneNumberText.PressColEnabled = 0;
  btnSceneNumberText.Font_Color      = 0x07FF;
  btnSceneNumberText.Gradient        = 0;
  btnSceneNumberText.Gradient_Orientation    = 0;
  btnSceneNumberText.Gradient_Start_Color    = 0xFFFF;
  btnSceneNumberText.Gradient_End_Color      = 0xC618;
  btnSceneNumberText.Color           = 0x000A;
  btnSceneNumberText.Press_Color     = 0x0010;
  btnSceneNumberText.Corner_Radius      = 3;
  btnSceneNumberText.OnUpPtr         = 0;
  btnSceneNumberText.OnDownPtr       = 0;
  btnSceneNumberText.OnClickPtr      = 0;
  btnSceneNumberText.OnPressPtr      = 0;

  BtnSelectBallastQuery.OwnerScreen     = &ScreenQueries;
  BtnSelectBallastQuery.Order           = 2;
  BtnSelectBallastQuery.Left            = 5;
  BtnSelectBallastQuery.Top             = 56;
  BtnSelectBallastQuery.Width           = 95;
  BtnSelectBallastQuery.Height          = 72;
  BtnSelectBallastQuery.Pen_Width       = 1;
  BtnSelectBallastQuery.Pen_Color       = 0x07FF;
  BtnSelectBallastQuery.Visible         = 1;
  BtnSelectBallastQuery.Active          = 0;
  BtnSelectBallastQuery.Transparent     = 1;
  BtnSelectBallastQuery.Caption         = BtnSelectBallastQuery_Caption;
  BtnSelectBallastQuery.TextAlign             = _taCenter;
  BtnSelectBallastQuery.FontName        = Tahoma38x39_Bold;
  BtnSelectBallastQuery.PressColEnabled = 1;
  BtnSelectBallastQuery.Font_Color      = 0x07FF;
  BtnSelectBallastQuery.Gradient        = 0;
  BtnSelectBallastQuery.Gradient_Orientation    = 0;
  BtnSelectBallastQuery.Gradient_Start_Color    = 0xFFFF;
  BtnSelectBallastQuery.Gradient_End_Color      = 0xC618;
  BtnSelectBallastQuery.Color           = 0x0009;
  BtnSelectBallastQuery.Press_Color     = 0x8410;
  BtnSelectBallastQuery.Corner_Radius      = 3;
  BtnSelectBallastQuery.OnUpPtr         = 0;
  BtnSelectBallastQuery.OnDownPtr       = 0;
  BtnSelectBallastQuery.OnClickPtr      = 0;
  BtnSelectBallastQuery.OnPressPtr      = 0;

  BtnScreenQueriesAnswer.OwnerScreen     = &ScreenQueries;
  BtnScreenQueriesAnswer.Order           = 5;
  BtnScreenQueriesAnswer.Left            = 147;
  BtnScreenQueriesAnswer.Top             = 193;
  BtnScreenQueriesAnswer.Width           = 165;
  BtnScreenQueriesAnswer.Height          = 43;
  BtnScreenQueriesAnswer.Pen_Width       = 1;
  BtnScreenQueriesAnswer.Pen_Color       = 0x07FF;
  BtnScreenQueriesAnswer.Visible         = 1;
  BtnScreenQueriesAnswer.Active          = 0;
  BtnScreenQueriesAnswer.Transparent     = 1;
  BtnScreenQueriesAnswer.Caption         = BtnScreenQueriesAnswer_Caption;
  BtnScreenQueriesAnswer.TextAlign             = _taCenter;
  BtnScreenQueriesAnswer.FontName        = Tahoma13x16_Regular;
  BtnScreenQueriesAnswer.PressColEnabled = 0;
  BtnScreenQueriesAnswer.Font_Color      = 0x07FF;
  BtnScreenQueriesAnswer.Gradient        = 0;
  BtnScreenQueriesAnswer.Gradient_Orientation    = 0;
  BtnScreenQueriesAnswer.Gradient_Start_Color    = 0xFFFF;
  BtnScreenQueriesAnswer.Gradient_End_Color      = 0xC618;
  BtnScreenQueriesAnswer.Color           = 0x0009;
  BtnScreenQueriesAnswer.Press_Color     = 0x8410;
  BtnScreenQueriesAnswer.Corner_Radius      = 3;
  BtnScreenQueriesAnswer.OnUpPtr         = 0;
  BtnScreenQueriesAnswer.OnDownPtr       = 0;
  BtnScreenQueriesAnswer.OnClickPtr      = 0;
  BtnScreenQueriesAnswer.OnPressPtr      = 0;

  Label13.OwnerScreen     = &ScreenQueries;
  Label13.Order          = 6;
  Label13.Left           = 148;
  Label13.Top            = 178;
  Label13.Width          = 43;
  Label13.Height         = 17;
  Label13.Visible        = 1;
  Label13.Active         = 1;
  Label13.Caption        = Label13_Caption;
  Label13.FontName       = Tahoma13x16_Regular;
  Label13.Font_Color     = 0x07FF;
  Label13.OnUpPtr         = 0;
  Label13.OnDownPtr       = 0;
  Label13.OnClickPtr      = 0;
  Label13.OnPressPtr      = 0;
}

static char IsInsideObject (unsigned int X, unsigned int Y, unsigned int Left, unsigned int Top, unsigned int Width, unsigned int Height) { // static
  if ( (Left<= X) && (Left+ Width - 1 >= X) &&
       (Top <= Y)  && (Top + Height - 1 >= Y) )
    return 1;
  else
    return 0;
}


#define GetButton(index)              CurrentScreen->Buttons[index]
#define GetRoundButton(index)         CurrentScreen->Buttons_Round[index]
#define GetCRoundButton(index)        CurrentScreen->CButtons_Round[index]
#define GetLabel(index)               CurrentScreen->Labels[index]
#define GetCLabel(index)              CurrentScreen->CLabels[index]
#define GetImage(index)               CurrentScreen->Images[index]
#define GetCircle(index)              CurrentScreen->Circles[index]
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

void DrawButton(TButton *Abutton) {
  if (Abutton->Visible == 1) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(Abutton->Transparent, Abutton->Press_Color, Abutton->Gradient, Abutton->Gradient_Orientation, Abutton->Gradient_End_Color, Abutton->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(Abutton->Transparent, Abutton->Color, Abutton->Gradient, Abutton->Gradient_Orientation, Abutton->Gradient_Start_Color, Abutton->Gradient_End_Color);
    }
    TFT_Set_Pen(Abutton->Pen_Color, Abutton->Pen_Width);
    TFT_Rectangle(Abutton->Left, Abutton->Top, Abutton->Left + Abutton->Width - 1, Abutton->Top + Abutton->Height - 1);
    TFT_Set_Font(Abutton->FontName, Abutton->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text_Return_Pos(Abutton->Caption, Abutton->Left, Abutton->Top);
    if (AButton->TextAlign == _taLeft)
      TFT_Write_Text(Abutton->Caption, Abutton->Left + 4, (Abutton->Top + ((Abutton->Height - caption_height) / 2)));
    else if (AButton->TextAlign == _taCenter)
      TFT_Write_Text(Abutton->Caption, (Abutton->Left + (Abutton->Width - caption_length) / 2), (Abutton->Top + ((Abutton->Height - caption_height) / 2)));
    else if (AButton->TextAlign == _taRight)
      TFT_Write_Text(Abutton->Caption, Abutton->Left + (Abutton->Width - caption_length - 4), (Abutton->Top + (Abutton->Height - caption_height) / 2));
  }
}

void DrawRoundButton(TButton_Round *Around_button) {
    if (Around_button->Visible == 1) {
      if (object_pressed == 1) {
        object_pressed = 0;
        TFT_Set_Brush(Around_button->Transparent, Around_button->Press_Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_End_Color, Around_button->Gradient_Start_Color);
      }
      else {
        TFT_Set_Brush(Around_button->Transparent, Around_button->Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_Start_Color, Around_button->Gradient_End_Color);
      }
      TFT_Set_Pen(Around_button->Pen_Color, Around_button->Pen_Width);
      if (Around_button->Height > Around_button->Width) {
        TFT_Rectangle_Round_Edges(Around_button->Left + 1, Around_button->Top + 1,
          Around_button->Left + Around_button->Width - 2,
          Around_button->Top + Around_button->Height - 2, Around_button->Corner_Radius);
      }
      else
        {
          TFT_Rectangle_Round_Edges(Around_button->Left + 1, Around_button->Top + 1,
            Around_button->Left + Around_button->Width - 2,
            Around_button->Top + Around_button->Height - 2, Around_button->Corner_Radius);
        }
      TFT_Set_Font(Around_button->FontName, Around_button->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text_Return_Pos(Around_button->Caption, Around_button->Left, Around_button->Top);
    if (Around_button->TextAlign == _taLeft)
      TFT_Write_Text(Around_button->Caption, Around_button->Left + 4, (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
    else if (Around_button->TextAlign == _taCenter)
      TFT_Write_Text(Around_button->Caption, (Around_button->Left + (Around_button->Width - caption_length) / 2), (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
    else if (Around_button->TextAlign == _taRight)
      TFT_Write_Text(Around_button->Caption, Around_button->Left + (Around_button->Width - caption_length - 4), (Around_button->Top + (Around_button->Height - caption_height) / 2));
    }
}

void DrawCRoundButton(TCButton_Round *Around_button) {
    if (Around_button->Visible == 1) {
      if (object_pressed == 1) {
        object_pressed = 0;
        TFT_Set_Brush(Around_button->Transparent, Around_button->Press_Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_End_Color, Around_button->Gradient_Start_Color);
      }
      else {
        TFT_Set_Brush(Around_button->Transparent, Around_button->Color, Around_button->Gradient, Around_button->Gradient_Orientation,
                      Around_button->Gradient_Start_Color, Around_button->Gradient_End_Color);
      }
      TFT_Set_Pen(Around_button->Pen_Color, Around_button->Pen_Width);
      if (Around_button->Height > Around_button->Width) {
        TFT_Rectangle_Round_Edges(Around_button->Left + 1, Around_button->Top + 1,
          Around_button->Left + Around_button->Width - 2,
          Around_button->Top + Around_button->Height - 2, Around_button->Corner_Radius);
      }
      else
        {
          TFT_Rectangle_Round_Edges(Around_button->Left + 1, Around_button->Top + 1,
            Around_button->Left + Around_button->Width - 2,
            Around_button->Top + Around_button->Height - 2, Around_button->Corner_Radius);
        }
      TFT_Set_Font(Around_button->FontName, Around_button->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text_Return_Pos(Around_button->Caption, Around_button->Left, Around_button->Top);
    if (Around_button->TextAlign == _taLeft)
      TFT_Write_Text(Around_button->Caption, Around_button->Left + 4, (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
    else if (Around_button->TextAlign == _taCenter)
      TFT_Write_Text(Around_button->Caption, (Around_button->Left + (Around_button->Width - caption_length) / 2), (Around_button->Top + ((Around_button->Height - caption_height) / 2)));
    else if (Around_button->TextAlign == _taRight)
      TFT_Write_Text(Around_button->Caption, Around_button->Left + (Around_button->Width - caption_length - 4), (Around_button->Top + (Around_button->Height - caption_height) / 2));
    }
}

void DrawLabel(TLabel *ALabel) {
  if (ALabel->Visible == 1) {
    TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text(ALabel->Caption, ALabel->Left, ALabel->Top);
  }
}

void DrawCLabel(TCLabel *ALabel) {
  if (ALabel->Visible == 1) {
    TFT_Set_Font(ALabel->FontName, ALabel->Font_Color, FO_HORIZONTAL);
    TFT_Write_Text(ALabel->Caption, ALabel->Left, ALabel->Top);
  }
}

void DrawImage(TImage *AImage) {
  if (AImage->Visible) {
    TFT_Image(AImage->Left, AImage->Top, AImage->Picture_Name, AImage->Picture_Ratio);
  }
}

void DrawCircle(TCircle *ACircle) {
  if (ACircle->Visible == 1) {
    if (object_pressed == 1) {
      object_pressed = 0;
      TFT_Set_Brush(ACircle->Transparent, ACircle->Press_Color, ACircle->Gradient, ACircle->Gradient_Orientation,
                    ACircle->Gradient_End_Color, ACircle->Gradient_Start_Color);
    }
    else {
      TFT_Set_Brush(ACircle->Transparent, ACircle->Color, ACircle->Gradient, ACircle->Gradient_Orientation,
                    ACircle->Gradient_Start_Color, ACircle->Gradient_End_Color);
    }
    TFT_Set_Pen(ACircle->Pen_Color, ACircle->Pen_Width);
    TFT_Circle(ACircle->Left + ACircle->Radius,
               ACircle->Top + ACircle->Radius,
               ACircle->Radius);
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
  unsigned short button_idx;
  TButton *local_button;
  unsigned short round_button_idx;
  TButton_Round *local_round_button;
  unsigned short round_cbutton_idx;
  TCButton_Round *local_round_cbutton;
  unsigned short label_idx;
  TLabel *local_label;
  unsigned short clabel_idx;
  TCLabel *local_clabel;
  unsigned short image_idx;
  TImage *local_image;
  unsigned short circle_idx;
  TCircle *local_circle;
  unsigned short box_idx;
  TBox *local_box;
  char save_bled, save_bled_direction;

  object_pressed = 0;
  order = 0;
  button_idx = 0;
  round_button_idx = 0;
  round_cbutton_idx = 0;
  label_idx = 0;
  clabel_idx = 0;
  image_idx = 0;
  circle_idx = 0;
  box_idx = 0;
  CurrentScreen = aScreen;

  if ((display_width != CurrentScreen->Width) || (display_height != CurrentScreen->Height)) {
    save_bled = TFT_BLED;
    TFT_BLED           = 0;
    TFT_Init(CurrentScreen->Width, CurrentScreen->Height);
    TP_TFT_Init(CurrentScreen->Width, CurrentScreen->Height, 8, 9);                                  // Initialize touch panel
    TP_TFT_Set_ADC_Threshold(ADC_THRESHOLD);                              // Set touch panel ADC threshold
    TFT_Fill_Screen(CurrentScreen->Color);
    display_width = CurrentScreen->Width;
    display_height = CurrentScreen->Height;
    TFT_BLED           = save_bled;
  }
  else
    TFT_Fill_Screen(CurrentScreen->Color);


  while (order < CurrentScreen->ObjectsCount) {
    if (button_idx < CurrentScreen->ButtonsCount) {
      local_button = GetButton(button_idx);
      if (order == local_button->Order) {
        button_idx++;
        order++;
        DrawButton(local_button);
      }
    }

    if (round_button_idx < CurrentScreen->Buttons_RoundCount) {
      local_round_button = GetRoundButton(round_button_idx);
      if (order == local_round_button->Order) {
        order++;
        round_button_idx++;
        DrawRoundButton(local_round_button);
      }
    }

    if (round_cbutton_idx < CurrentScreen->CButtons_RoundCount) {
      local_round_cbutton = GetCRoundButton(round_cbutton_idx);
      if (order == local_round_cbutton->Order) {
        order++;
        round_cbutton_idx++;
        DrawCRoundButton(local_round_cbutton);
      }
    }

    if (label_idx < CurrentScreen->LabelsCount) {
      local_label = GetLabel(label_idx);
      if (order == local_label->Order) {
        label_idx++;
        order++;
        DrawLabel(local_label);
      }
    }

    if (clabel_idx < CurrentScreen->CLabelsCount) {
      local_clabel = GetCLabel(clabel_idx);
      if (order == local_clabel->Order) {
        clabel_idx++;
        order++;
        DrawCLabel(local_clabel);
      }
    }

    if (circle_idx < CurrentScreen->CirclesCount) {
      local_circle = GetCircle(circle_idx);
      if (order == local_circle->Order) {
        circle_idx++;
        order++;
        DrawCircle(local_circle);
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

    if (image_idx  < CurrentScreen->ImagesCount) {
      local_image = GetImage(image_idx);
      if (order == local_image->Order) {
        image_idx++;
        order++;
        DrawImage(local_image);
      }
    }

  }
}

void Get_Object(unsigned int X, unsigned int Y) {
  button_order        = -1;
  round_button_order  = -1;
  round_cbutton_order = -1;
  label_order         = -1;
  clabel_order        = -1;
  image_order         = -1;
  circle_order        = -1;
  box_order           = -1;
  //  Buttons
  for ( _object_count = 0 ; _object_count < CurrentScreen->ButtonsCount ; _object_count++ ) {
    local_button = GetButton(_object_count);
    if (local_button->Active == 1) {
      if (IsInsideObject(X, Y, local_button->Left, local_button->Top,
                         local_button->Width, local_button->Height) == 1) {
        button_order = local_button->Order;
        exec_button = local_button;
      }
    }
  }

  //  Buttons with Round Edges
  for ( _object_count = 0 ; _object_count < CurrentScreen->Buttons_RoundCount ; _object_count++ ) {
    local_round_button = GetRoundButton(_object_count);
    if (local_round_button->Active == 1) {
      if (IsInsideObject(X, Y, local_round_button->Left, local_round_button->Top,
                         local_round_button->Width, local_round_button->Height) == 1) {
        round_button_order = local_round_button->Order;
        exec_round_button = local_round_button;
      }
    }
  }

  //  CButtons with Round Edges
  for ( _object_count = 0 ; _object_count < CurrentScreen->CButtons_RoundCount ; _object_count++ ) {
    local_round_cbutton = GetCRoundButton(_object_count);
    if (local_round_cbutton->Active == 1) {
      if (IsInsideObject(X, Y, local_round_cbutton->Left, local_round_cbutton->Top,
                         local_round_cbutton->Width, local_round_cbutton->Height) == 1) {
        round_cbutton_order = local_round_cbutton->Order;
        exec_round_cbutton = local_round_cbutton;
      }
    }
  }

  //  Labels
  for ( _object_count = 0 ; _object_count < CurrentScreen->LabelsCount ; _object_count++ ) {
    local_label = GetLabel(_object_count);
    if (local_label->Active == 1) {
      if (IsInsideObject(X, Y, local_label->Left, local_label->Top,
                         local_label->Width, local_label->Height) == 1) {
        label_order = local_label->Order;
        exec_label = local_label;
      }
    }
  }

  //  CLabels
  for ( _object_count = 0 ; _object_count < CurrentScreen->CLabelsCount ; _object_count++ ) {
    local_clabel = GetCLabel(_object_count);
    if (local_clabel->Active == 1) {
      if (IsInsideObject(X, Y, local_clabel->Left, local_clabel->Top,
                         local_clabel->Width, local_clabel->Height) == 1) {
        clabel_order = local_clabel->Order;
        exec_clabel = local_clabel;
      }
    }
  }

  //  Images
  for ( _object_count = 0 ; _object_count < CurrentScreen->ImagesCount ; _object_count++ ) {
    local_image = GetImage(_object_count);
    if (local_image->Active == 1) {
      if (IsInsideObject(X, Y, local_image->Left, local_image->Top,
                         local_image->Width, local_image->Height) == 1) {
        image_order = local_image->Order;
        exec_image = local_image;
      }
    }
  }

  //  Circles
  for ( _object_count = 0 ; _object_count < CurrentScreen->CirclesCount ; _object_count++ ) {
    local_circle = GetCircle(_object_count);
    if (local_circle->Active == 1) {
      if (IsInsideObject(X, Y, local_circle->Left, local_circle->Top,
                        (local_circle->Radius * 2), (local_circle->Radius * 2)) == 1) {
        circle_order = local_circle->Order;
        exec_circle = local_circle;
      }
    }
  }

  //  Boxes
  for ( _object_count = 0 ; _object_count < CurrentScreen->BoxesCount ; _object_count++ ) {
    local_box = GetBox(_object_count);
    if (local_box->Active == 1) {
      if (IsInsideObject(X, Y, local_box->Left, local_box->Top,
                         local_box->Width, local_box->Height) == 1) {
        box_order = local_box->Order;
        exec_box = local_box;
      }
    }
  }

  _object_count = -1;
  if (button_order > _object_count)
    _object_count = button_order;
  if (round_button_order > _object_count)
    _object_count = round_button_order;
  if (round_cbutton_order >  _object_count )
    _object_count = round_cbutton_order;
  if (label_order >  _object_count )
    _object_count = label_order;
  if (clabel_order >  _object_count )
    _object_count = clabel_order;
  if (image_order >  _object_count )
    _object_count = image_order;
  if (circle_order >  _object_count )
    _object_count = circle_order;
  if (box_order >  _object_count )
    _object_count = box_order;
}


static void Process_TP_Press(unsigned int X, unsigned int Y) {
  exec_button         = 0;
  exec_round_button   = 0;
  exec_round_cbutton  = 0;
  exec_label          = 0;
  exec_clabel         = 0;
  exec_image          = 0;
  exec_circle         = 0;
  exec_box            = 0;

  Get_Object(X, Y);


  if (_object_count != -1) {
    if (_object_count == button_order) {
      if (exec_button->Active == 1) {
        if (exec_button->OnPressPtr != 0) {
          exec_button->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->OnPressPtr != 0) {
          exec_round_button->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == round_cbutton_order) {
      if (exec_round_cbutton->Active == 1) {
        if (exec_round_cbutton->OnPressPtr != 0) {
          exec_round_cbutton->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        if (exec_label->OnPressPtr != 0) {
          exec_label->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == clabel_order) {
      if (exec_clabel->Active == 1) {
        if (exec_clabel->OnPressPtr != 0) {
          exec_clabel->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == image_order) {
      if (exec_image->Active == 1) {
        if (exec_image->OnPressPtr != 0) {
          exec_image->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == circle_order) {
      if (exec_circle->Active == 1) {
        if (exec_circle->OnPressPtr != 0) {
          exec_circle->OnPressPtr();
          return;
        }
      }
    }

    if (_object_count == box_order) {
      if (exec_box->Active == 1) {
        if (exec_box->OnPressPtr != 0) {
          exec_box->OnPressPtr();
          return;
        }
      }
    }

  }
}

static void Process_TP_Up(unsigned int X, unsigned int Y) {

  switch (PressedObjectType) {
    // Button
    case 0: {
      if (PressedObject != 0) {
        exec_button = (TButton*)PressedObject;
        if ((exec_button->PressColEnabled == 1) && (exec_button->OwnerScreen == CurrentScreen)) {
          DrawButton(exec_button);
        }
        break;
      }
      break;
    }
    // Round Button
    case 1: {
      if (PressedObject != 0) {
        exec_round_button = (TButton_Round*)PressedObject;
        if ((exec_round_button->PressColEnabled == 1) && (exec_round_button->OwnerScreen == CurrentScreen)) {
          DrawRoundButton(exec_round_button);
        }
        break;
      }
      break;
    }
    // Round CButton
    case 9: {
      if (PressedObject != 0) {
        exec_round_cbutton = (TCButton_Round*)PressedObject;
        if ((exec_round_cbutton->PressColEnabled == 1) && (exec_round_cbutton->OwnerScreen == CurrentScreen)) {
          DrawCRoundButton(exec_round_cbutton);
        }
        break;
      }
      break;
    }
    // Circle
    case 4: {
      if (PressedObject != 0) {
        exec_circle = (TCircle*)PressedObject;
        if ((exec_circle->PressColEnabled == 1) && (exec_circle->OwnerScreen == CurrentScreen)) {
          DrawCircle(exec_circle);
        }
        break;
      }
      break;
    }
    // Box
    case 6: {
      if (PressedObject != 0) {
        exec_box = (TBox*)PressedObject;
        if ((exec_box->PressColEnabled == 1) && (exec_box->OwnerScreen == CurrentScreen)) {
          DrawBox(exec_box);
        }
        break;
      }
      break;
    }
  }

  exec_label          = 0;
  exec_clabel          = 0;
  exec_image          = 0;

  Get_Object(X, Y);


  if (_object_count != -1) {
  // Buttons
    if (_object_count == button_order) {
      if (exec_button->Active == 1) {
        if (exec_button->OnUpPtr != 0)
          exec_button->OnUpPtr();
        if (PressedObject == (TPointer)exec_button)
          if (exec_button->OnClickPtr != 0)
            exec_button->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Buttons with Round Edges
    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->OnUpPtr != 0)
          exec_round_button->OnUpPtr();
        if (PressedObject == (TPointer)exec_round_button)
          if (exec_round_button->OnClickPtr != 0)
            exec_round_button->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // CButtons with Round Edges
    if (_object_count == round_cbutton_order) {
      if (exec_round_cbutton->Active == 1) {
        if (exec_round_cbutton->OnUpPtr != 0)
          exec_round_cbutton->OnUpPtr();
        if (PressedObject == (TPointer)exec_round_cbutton)
          if (exec_round_cbutton->OnClickPtr != 0)
            exec_round_cbutton->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Labels
    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        if (exec_label->OnUpPtr != 0)
          exec_label->OnUpPtr();
        if (PressedObject == (TPointer)exec_label)
          if (exec_label->OnClickPtr != 0)
            exec_label->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // CLabels
    if (_object_count == clabel_order) {
      if (exec_clabel->Active == 1) {
        if (exec_clabel->OnUpPtr != 0)
          exec_clabel->OnUpPtr();
        if (PressedObject == (TPointer)exec_clabel)
          if (exec_clabel->OnClickPtr != 0)
            exec_clabel->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Images
    if (_object_count == image_order) {
      if (exec_image->Active == 1) {
        if (exec_image->OnUpPtr != 0)
          exec_image->OnUpPtr();
        if (PressedObject == (TPointer)exec_image)
          if (exec_image->OnClickPtr != 0)
            exec_image->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Circles
    if (_object_count == circle_order) {
      if (exec_circle->Active == 1) {
        if (exec_circle->OnUpPtr != 0)
          exec_circle->OnUpPtr();
        if (PressedObject == (TPointer)exec_circle)
          if (exec_circle->OnClickPtr != 0)
            exec_circle->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  // Boxes
    if (_object_count == box_order) {
      if (exec_box->Active == 1) {
        if (exec_box->OnUpPtr != 0)
          exec_box->OnUpPtr();
        if (PressedObject == (TPointer)exec_box)
          if (exec_box->OnClickPtr != 0)
            exec_box->OnClickPtr();
        PressedObject = 0;
        PressedObjectType = -1;
        return;
      }
    }

  }
  PressedObject = 0;
  PressedObjectType = -1;
}

static void Process_TP_Down(unsigned int X, unsigned int Y) {

  object_pressed      = 0;
  exec_button         = 0;
  exec_round_button   = 0;
  exec_round_cbutton = 0;
  exec_label          = 0;
  exec_clabel         = 0;
  exec_image          = 0;
  exec_circle         = 0;
  exec_box            = 0;

  Get_Object(X, Y);

  if (_object_count != -1) {
    if (_object_count == button_order) {
      if (exec_button->Active == 1) {
        if (exec_button->PressColEnabled == 1) {
          object_pressed = 1;
          DrawButton(exec_button);
        }
        PressedObject = (TPointer)exec_button;
        PressedObjectType = 0;
        if (exec_button->OnDownPtr != 0) {
          exec_button->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == round_button_order) {
      if (exec_round_button->Active == 1) {
        if (exec_round_button->PressColEnabled == 1) {
          object_pressed = 1;
          DrawRoundButton(exec_round_button);
        }
        PressedObject = (TPointer)exec_round_button;
        PressedObjectType = 1;
        if (exec_round_button->OnDownPtr != 0) {
          exec_round_button->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == round_cbutton_order) {
      if (exec_round_cbutton->Active == 1) {
        if (exec_round_cbutton->PressColEnabled == 1) {
          object_pressed = 1;
          DrawCRoundButton(exec_round_cbutton);
        }
        PressedObject = (TPointer)exec_round_cbutton;
        PressedObjectType = 9;
        if (exec_round_cbutton->OnDownPtr != 0) {
          exec_round_cbutton->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == label_order) {
      if (exec_label->Active == 1) {
        PressedObject = (TPointer)exec_label;
        PressedObjectType = 2;
        if (exec_label->OnDownPtr != 0) {
          exec_label->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == clabel_order) {
      if (exec_clabel->Active == 1) {
        PressedObject = (TPointer)exec_clabel;
        PressedObjectType = 10;
        if (exec_clabel->OnDownPtr != 0) {
          exec_clabel->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == image_order) {
      if (exec_image->Active == 1) {
        PressedObject = (TPointer)exec_image;
        PressedObjectType = 3;
        if (exec_image->OnDownPtr != 0) {
          exec_image->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == circle_order) {
      if (exec_circle->Active == 1) {
        if (exec_circle->PressColEnabled == 1) {
          object_pressed = 1;
          DrawCircle(exec_circle);
        }
        PressedObject = (TPointer)exec_circle;
        PressedObjectType = 4;
        if (exec_circle->OnDownPtr != 0) {
          exec_circle->OnDownPtr();
          return;
        }
      }
    }

    if (_object_count == box_order) {
      if (exec_box->Active == 1) {
        if (exec_box->PressColEnabled == 1) {
          object_pressed = 1;
          DrawBox(exec_box);
        }
        PressedObject = (TPointer)exec_box;
        PressedObjectType = 6;
        if (exec_box->OnDownPtr != 0) {
          exec_box->OnDownPtr();
          return;
        }
      }
    }

  }
}

void Check_TP() {
  if (TP_TFT_Press_Detect()) {
    // After a PRESS is detected read X-Y and convert it to Display dimensions space
    if (TP_TFT_Get_Coordinates(&Xcoord, &Ycoord) == 0) {
      Process_TP_Press(Xcoord, Ycoord);
      if (PenDown == 0) {
        PenDown = 1;
        Process_TP_Down(Xcoord, Ycoord);
      }
    }
  }
  else if (PenDown == 1) {
    PenDown = 0;
    Process_TP_Up(Xcoord, Ycoord);
  }
}

void Init_MCU() {
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_9);
  TFT_BLED = 1;
  TFT_Set_Default_Mode();
  TP_TFT_Set_Default_Mode();
}

void Start_TP() {
  Init_MCU();

  InitializeTouchPanel();

  Delay_ms(1000);
  TFT_Fill_Screen(0);
  Calibrate();
  TFT_Fill_Screen(0);

  InitializeObjects();
  display_width = Screen2.Width;
  display_height = Screen2.Height;
  DrawScreen(&Screen2);
}
