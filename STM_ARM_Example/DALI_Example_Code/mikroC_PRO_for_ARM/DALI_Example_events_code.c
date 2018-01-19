#include "DALI_Example_objects.h"
#include "DALI_Example_resources.h"

//--------------------- User code ---------------------//
//extern unsigned char dali_status;

#include "DALI-pub.h"
#include "DALI_defs.h"

volatile LightCommandType cmdBroadcastAllOff;
volatile LightCommandType cmdBroadcastAllOn;

LightCommandType Command1[1];

LightObjectType lightOut01;
LightObjectType lightOut02;
LightObjectType lightOut03;
LightObjectType lightOut04;
LightObjectType lightOut05;
LightObjectType lightOut06;

LightObjectType lightIn01;
LightObjectType lightIn02;
LightObjectType lightIn03;
LightObjectType lightIn04;

LightObjectType lightArray[10];
TCircle         circleArray[6];
TBox            boxArray[4];

TCircle         circleArcArray[6];
TBox            boxArcArray[4];

//screen specific variables
unsigned char setSceneIndexBallastSelected;
unsigned char setSceneIndexSceneSelected;
unsigned char setSceneSceneLevel;

unsigned char setArcLvlIndexBallastSelected;

unsigned char setQueryBallastSelected;
///////////////////////////

void InitialiseLights(void);

void GraphicsSelectLight(unsigned char SelectlightId);



void InitialiseCommands(void)
{
  cmdBroadcastAllOff._ballastAddress = ADDRESS01;
  cmdBroadcastAllOff._command        = OFF;
  cmdBroadcastAllOff._typeOfCommand  = BROADCAST_CMD;
  cmdBroadcastAllOff._followingType  = FOLLOWING_COMMAND;
  //recall max value for arc level for all ballast. If the ballast is in OFF state,
  //it will be turned ON
  cmdBroadcastAllOn._ballastAddress = ADDRESS01;
  cmdBroadcastAllOn._command        = RECALL_MAX_LEVEL;
  cmdBroadcastAllOn._typeOfCommand  = BROADCAST_CMD;
  cmdBroadcastAllOn._followingType  = FOLLOWING_COMMAND;
}

void ToggleLight(unsigned char lightObjId, unsigned char ObjId, unsigned char type )
{
  if(dali_state == NO_ACTION)
  {
    if(lightArray[lightObjId]._on)
    {
      lightArray[lightObjId]._on = FALSE;
      if(type == 0)
      {
        circleArray[ObjId].Color = 0xF800;
        DrawCircle(&circleArray[ObjId]);
      }
      else
      {
        boxArray[ObjId].Color = 0xF800;
        DrawBox(&boxArray[ObjId]);
      }
      DALI_Send_Cmd(lightArray[lightObjId]._address, OFF, SHORT_ADDRESS, FOLLOWING_COMMAND);
    }
    else
    {
      lightArray[lightObjId]._on = TRUE;
      if(type == 0)
      {
        circleArray[ObjId].Color = 0xFFE0;
        DrawCircle(&circleArray[ObjId]);
      }
      else
      {
        boxArray[ObjId].Color = 0xFFE0;
        DrawBox(&boxArray[ObjId]);
      }
      DALI_Send_Cmd(lightArray[lightObjId]._address, RECALL_MAX_LEVEL, SHORT_ADDRESS, FOLLOWING_COMMAND);
    }
  }
}
//----------------- End of User code ------------------//

// Event Handlers

//all btn On
void Button1OnClick() {

  unsigned char i;

  if(dali_state == NO_ACTION)
  {
    for(i = 0; i < 10; i++)
    {
      lightArray[i]._on = TRUE;
      lightArray[i]._arcLevel = 254;
      if(i < 6)
      {
        circleArray[i].Color = 0xFFE0;
        DrawCircle(&circleArray[i]);
      }
      if(i < 4)
      {
        boxArray[i].Color = 0xFFE0;
        DrawBox(&boxArray[i]);
      }
    }
    DALI_Send_Cmd(cmdBroadcastAllOn._ballastAddress, cmdBroadcastAllOn._command, cmdBroadcastAllOn._typeOfCommand,
                cmdBroadcastAllOn._followingType);
  }
}
//all btn OFF
void Button2OnClick() {

  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(cmdBroadcastAllOff._ballastAddress, cmdBroadcastAllOff._command, cmdBroadcastAllOff._typeOfCommand, 
                 cmdBroadcastAllOff._followingType);
  
  for(i = 0; i < 10; i++)
    {
      lightArray[i]._on = FALSE;
      lightArray[i]._arcLevel = 0;
      if(i < 6)
      {
        circleArray[i].Color = 0xF800;
        DrawCircle(&circleArray[i]);
      }
      if(i < 4)
      {
        boxArray[i].Color = 0xF800;
        DrawBox(&boxArray[i]);
      }
    }
    
}
//scene 01 ALL OUT
void Button3OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x00, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i < 6)
     {
       lightArray[i]._on = TRUE;
       circleArray[i].Color = 0xFFE0;
       DrawCircle(&circleArray[i]);
     }
     else
     {
       lightArray[i]._on = FALSE;
       boxArray[i - 6].Color = 0xF800;
       DrawBox(&boxArray[i - 6]);
     }
  }
}
//scene 02 ALL IN
void Button4OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x01, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i < 6)
     {
       lightArray[i]._on = FALSE;
       circleArray[i].Color = 0xF800;
       DrawCircle(&circleArray[i]);
     }
     else
     {
       lightArray[i]._on = TRUE;
       boxArray[i - 6].Color = 0xFFE0;
       DrawBox(&boxArray[i - 6]);
     }
  }
}
//scene 03 CORNER OUT
void Button5OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x02, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i == 0 || i == 1 || i == 3 || i == 4)
     {
       lightArray[i]._on = TRUE;
       circleArray[i].Color = 0xFFE0;
       DrawCircle(&circleArray[i]);
     }
     else if(i == 2 || i == 5)
     {
       lightArray[i]._on = FALSE;
       circleArray[i].Color = 0xF800;
       DrawCircle(&circleArray[i]);
     }
     else
     {
       lightArray[i]._on = FALSE;
       boxArray[i - 6].Color = 0xF800;
       DrawBox(&boxArray[i - 6]);
     }
  }
}
//scene 04 FRONT OUT/IN
void Button6OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x03, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i == 0 || i == 4 || i == 5)
     {
       lightArray[i]._on = TRUE;
       circleArray[i].Color = 0xFFE0;
       DrawCircle(&circleArray[i]);
     }
     else
     if (i < 6)
     {
       lightArray[i]._on = FALSE;
       circleArray[i].Color = 0xF800;
       DrawCircle(&circleArray[i]);
     }
     if((i == 6 || i == 7 || i == 8))
     {
       lightArray[i]._on = TRUE;
       boxArray[i - 6].Color = 0xFFE0;
       DrawBox(&boxArray[i - 6]);
     }
     else
     if(i > 5)
     {
       lightArray[i]._on = FALSE;
       boxArray[i - 6].Color = 0xFFE0;
       DrawBox(&boxArray[i - 6]);
     }
  }
}
//scene 05 BACK OUT/IN
void Button7OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x04, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i == 1 || i == 2 || i == 3)
     {
       lightArray[i]._on = TRUE;
       circleArray[i].Color = 0xFFE0;
       DrawCircle(&circleArray[i]);
     }
     else
     if (i < 6)
     {
       lightArray[i]._on = FALSE;
       circleArray[i].Color = 0xF800;
       DrawCircle(&circleArray[i]);
     }
     if(i == 9)
     {
       lightArray[i]._on = TRUE;
       boxArray[i - 6].Color = 0xFFE0;
       DrawBox(&boxArray[i - 6]);
     }
     else
     if (i > 5)
     {
       lightArray[i]._on = FALSE;
       boxArray[i - 6].Color = 0xF800;
       DrawBox(&boxArray[i - 6]);
     }
  }
}
//scene 06 CORNER OUT/IN
void Button8OnClick() {
  unsigned char i;

  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(0x05, RECALL_MAX_LEVEL, GROUP_ADDRESS, FOLLOWING_COMMAND);

  for(i = 0; i < 10; i++)
  {
     if(i == 0 || i == 1 || i == 3 || i == 4)
     {
       lightArray[i]._on = TRUE;
       circleArray[i].Color = 0xFFE0;
       DrawCircle(&circleArray[i]);
     }
     else
     if (i < 6)
     {
       lightArray[i]._on = FALSE;
       circleArray[i].Color = 0xF800;
       DrawCircle(&circleArray[i]);
     }
     if(i > 5)
     {
       lightArray[i]._on = TRUE;
       boxArray[i - 6].Color = 0xFFE0;
       DrawBox(&boxArray[i - 6]);
     }
  }
}

void CircleLightOut01OnClick() {
   ToggleLight(0, 0, 0);
}

void CircleLightOut02OnClick() {
   ToggleLight(1, 1, 0);
}

void CircleLightOut03OnClick() {
   ToggleLight(2, 2, 0);
}

void CircleLightOut04OnClick() {
   ToggleLight(3, 3, 0);
}

void CircleLightOut05OnClick() {
   ToggleLight(4, 4, 0);
}

void CircleLightOut06OnClick() {
   ToggleLight(5, 5, 0);
}

void BoxLightIn01OnClick() {
  ToggleLight(6, 0, 1);
}

void BoxLightIn02OnClick() {
  ToggleLight(7, 1, 1);
}

void BoxLightIn03OnClick() {
  ToggleLight(8, 2, 1);
}

void BoxLightIn04OnClick() {
  ToggleLight(9, 3, 1);
}

void Button10OnClick() {

  InitialiseLights();
  InitialiseCommands();
  DrawScreen(&Screen1);
  if(dali_state == NO_ACTION)
    DALI_Send_Cmd(ADDRESS01, RECALL_MAX_LEVEL, BROADCAST_CMD, FOLLOWING_COMMAND);
}

void BtnSetArcOnClick() {

  InitialiseLights();
  drawScreen(&Screen3);
  
  setArcLvlIndexBallastSelected = 0;
  
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(ADDRESS01, RECALL_MAX_LEVEL, BROADCAST_CMD, FOLLOWING_COMMAND);
  }
}

void ButtonRound3OnClick() {
   unsigned char i;
   
   for(i = 0; i < 10; i++)
    {
      lightArray[i]._on = TRUE;
      lightArray[i]._arcLevel = 254;
      if(i < 6)
      {
        circleArray[i].Color = 0xFFE0;
        DrawCircle(&circleArray[i]);
      }
      if(i < 4)
      {
        boxArray[i].Color = 0xFFE0;
        DrawBox(&boxArray[i]);
      }
    }

  drawScreen(&Screen2);
}

void BtnBackSceneOnClick() {
  drawScreen(&Screen2);
}

void BtnSetSceneLevelOnClick() {
  InitialiseLights();
  
  setSceneIndexBallastSelected = 0;
  setSceneIndexSceneSelected   = 0;
  setSceneSceneLevel           = 0;
  
  ShortToStr(lightArray[0]._address, btnBallastAddressText.Caption);
  ShortToStr(setSceneIndexSceneSelected, btnSceneNumberText.Caption);

  drawScreen(&ScreenSetScene);
}

void BtnBackSetLightsOnClick() {
  drawScreen(&Screen2);
}


void BtnBalAddrUpOnClick() {
   if(setSceneIndexBallastSelected < 9)
   {
     setSceneIndexBallastSelected++;
     ShortToStr(lightArray[setSceneIndexBallastSelected]._address, btnBallastAddressText.Caption);
     DrawRoundButton(&btnBallastAddressText);
   }
}

void BtnBalAddrDownOnClick() {
   if(setSceneIndexBallastSelected > 0)
   {
     setSceneIndexBallastSelected--;
     ShortToStr(lightArray[setSceneIndexBallastSelected]._address, btnBallastAddressText.Caption);
     DrawRoundButton(&btnBallastAddressText);
   }
}

void BtnSceneNumDownOnClick() {
   if(setSceneIndexSceneSelected > 0)
   {
     setSceneIndexSceneSelected--;
     ShortToStr(setSceneIndexSceneSelected, btnSceneNumberText.Caption);
     DrawRoundButton(&btnSceneNumberText);
   }
}

void BtnSceneNumUpOnClick() {
   if(setSceneIndexSceneSelected < 15)
   {
     setSceneIndexSceneSelected++;
     ShortToStr(setSceneIndexSceneSelected, btnSceneNumberText.Caption);
     DrawRoundButton(&btnSceneNumberText);
   }
}

void BtnSetSceneBallastOnClick() {
   if (dali_state == NO_ACTION)
   {
     //posalji komandu da snimi za selektovani balast i scenu odgovarajucu vrednost
     DALI_Send_Cmd(setSceneIndexBallastSelected, (GO_TO_SCENE01 + setSceneIndexSceneSelected), SHORT_ADDRESS, FOLLOWING_COMMAND);
   }
}

void BtnArcLevelDownOnClick() {

   if(dali_state == NO_ACTION)
   {
     //DALI_Send_Cmd(ADDRESS08, STEP_DOWN, SHORT_ADDRESS, FOLLOWING_COMMAND);
     DALI_Send_Cmd(lightArray[setArcLvlIndexBallastSelected]._address, STEP_DOWN, SHORT_ADDRESS, FOLLOWING_COMMAND);

     if ((lightArray[setArcLvlIndexBallastSelected]._arcLevel - 7) > 1){

       lightArray[setArcLvlIndexBallastSelected]._arcLevel -= 7; //default value for MIN_LEVEL
     }
     else
     {
       lightArray[setArcLvlIndexBallastSelected]._arcLevel = 1;
     }
     IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);

     DrawRoundButton(&BtnBallastArcLevel);
   }
}

void BtnArcLevelUpOnClick() {
   if(dali_state == NO_ACTION)
   {
     //DALI_Send_Cmd(ADDRESS08, STEP_UP, SHORT_ADDRESS, FOLLOWING_COMMAND);
     DALI_Send_Cmd(lightArray[setArcLvlIndexBallastSelected]._address, STEP_UP, SHORT_ADDRESS, FOLLOWING_COMMAND);

     if ((lightArray[setArcLvlIndexBallastSelected]._arcLevel + 7) < 254){

       lightArray[setArcLvlIndexBallastSelected]._arcLevel += 7; //value for MIN_LEVEL
     }
     else
     {
       lightArray[setArcLvlIndexBallastSelected]._arcLevel = 254;
     }
     IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);

     DrawRoundButton(&BtnBallastArcLevel);
   }
}

void Circle6OnClick() {
   setArcLvlIndexBallastSelected = 5;
   
   IntToStr(lightArray[5]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);
   
   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle6.Color = 0xFFFF;
   DrawCircle(&Circle6);
}

void GraphicsSelectLight(unsigned char SelectlightId)
{
  unsigned char i;
  
  for(i = 0; i < 10; i++)
  {
    if(i != SelectlightId)
    {
      if(i < 6)
      {
        circleArcArray[i].Color = 0xFFE0;
        DrawCircle(&circleArcArray[i]);
      }
      else
      {
        boxArcArray[i - 6].Color = 0xFFE0;
        DrawBox(&boxArcArray[i - 6]);
      }
    }
  }
}

void Box1OnClick() {
   setArcLvlIndexBallastSelected = 6;

   GraphicsSelectLight(setArcLvlIndexBallastSelected);

   Box1.Color = 0xFFFF;
   DrawBox(&Box1);
   
   IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);
   DrawButton(&BtnBallastArcLevel);
}

void BtnSendQueryCmdOnClick() {

  InitialiseLights();

  setQueryBallastSelected = 0;

  ShortToStr(setQueryBallastSelected, BtnSelectBallastQuery.caption);

  DrawScreen(&ScreenQueries);
}

void BtnBackQueriesOnClick() {
  DrawScreen(&Screen2);
}

void BtnQueryActualLevelOnClick() {
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(lightArray[setQueryBallastSelected]._address, QUERY_ACTUAL_LEVEL, SHORT_ADDRESS, FOLLOWING_COMMAND);
  }
}

void BtnQueryBallastOnClick() {
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(lightArray[setQueryBallastSelected]._address, QUERY_BALLAST, SHORT_ADDRESS, FOLLOWING_COMMAND);
  }
}

void BtnQueryDeviceTypeOnClick() {
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(lightArray[setQueryBallastSelected]._address, QUERY_DEVICE_TYPE, SHORT_ADDRESS, FOLLOWING_COMMAND);
  }
}

void BtnQueryGroup0_7OnClick() {
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(lightArray[setQueryBallastSelected]._address, QUERY_GROUPS_0_7, SHORT_ADDRESS, FOLLOWING_COMMAND);
  }
}

void BtnQueryGroup8_15OnClick() {
  if(dali_state == NO_ACTION)
  {
    DALI_Send_Cmd(lightArray[setQueryBallastSelected]._address, QUERY_GROUPS_8_15, SHORT_ADDRESS, FOLLOWING_COMMAND);
  }
}

void BtnScreenQueriesBalUpOnClick() {
  if(setQueryBallastSelected < 9)
  {
     setQueryBallastSelected++;
     ShortToStr(lightArray[setQueryBallastSelected]._address, BtnSelectBallastQuery.Caption);
     DrawRoundButton(&BtnSelectBallastQuery);
   }
}

void BtnScreenQueriesBalDownOnClick() {
  if(setQueryBallastSelected > 0)
  {
     setQueryBallastSelected--;
     ShortToStr(lightArray[setQueryBallastSelected]._address, BtnSelectBallastQuery.Caption);
     DrawRoundButton(&BtnSelectBallastQuery);
   }
}

void Circle1OnClick() {
   setArcLvlIndexBallastSelected = 0;

   IntToStr(lightArray[0]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);

   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle1.Color = 0xFFFF;
   DrawCircle(&Circle1);
}

void Circle2OnClick() {
   setArcLvlIndexBallastSelected = 1;

   IntToStr(lightArray[1]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);

   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle2.Color = 0xFFFF;
   DrawCircle(&Circle2);
}

void Circle3OnClick() {
   setArcLvlIndexBallastSelected = 2;

   IntToStr(lightArray[2]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);

   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle3.Color = 0xFFFF;
   DrawCircle(&Circle3);
}

void Circle4OnClick() {
   setArcLvlIndexBallastSelected = 3;

   IntToStr(lightArray[3]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);

   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle4.Color = 0xFFFF;
   DrawCircle(&Circle4);
}

void Circle5OnClick() {
   setArcLvlIndexBallastSelected = 4;

   IntToStr(lightArray[4]._arcLevel, BtnBallastArcLevel.Caption);
   DrawRoundButton(&BtnBallastArcLevel);

   GraphicsSelectLight(setArcLvlIndexBallastSelected);
   Circle5.Color = 0xFFFF;
   DrawCircle(&Circle5);
}

void Box2OnClick() {
   setArcLvlIndexBallastSelected = 7;

   GraphicsSelectLight(setArcLvlIndexBallastSelected);

   Box2.Color = 0xFFFF;
   DrawBox(&Box2);

   IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);
   DrawButton(&BtnBallastArcLevel);
}

void Box3OnClick() {
   setArcLvlIndexBallastSelected = 8;

   GraphicsSelectLight(setArcLvlIndexBallastSelected);

   Box3.Color = 0xFFFF;
   DrawBox(&Box3);

   IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);
   DrawButton(&BtnBallastArcLevel);
}

void Box4OnClick() {
   setArcLvlIndexBallastSelected = 9;

   GraphicsSelectLight(setArcLvlIndexBallastSelected);

   Box4.Color = 0xFFFF;
   DrawBox(&Box4);

   IntToStr(lightArray[setArcLvlIndexBallastSelected]._arcLevel, BtnBallastArcLevel.Caption);
   DrawButton(&BtnBallastArcLevel);
}

//set default values - only for testing
//real app must first read ballasts actual arc valuea, and if the ballast has
//the value, it must get and set that value
void InitialiseLights(void)
{
  unsigned char i;
  unsigned char lightCount = 0;

  lightOut01._address  = ADDRESS01;
  lightOut01._on       = FALSE;
  lightOut01._arcLevel = 254;
  lightOut01._group0_7    = 0x2D;
  lightOut01._group8_15    = 0;

  lightArray[0]       = lightOut01;
  circleArray[0]      = CircleLightOut01;

  circleArcArray[0]   = Circle1;

  lightOut02._address  = ADDRESS02;
  lightOut02._on       = FALSE;
  lightOut02._arcLevel = 254;
  lightOut02._group0_7    = 0x35;
  lightOut02._group8_15   = 0;

  lightArray[1]       = lightOut02;
  circleArray[1]      = CircleLightOut02;

  circleArcArray[1]   = Circle2;

  lightOut03._address  = ADDRESS03;
  lightOut03._on       = FALSE;
  lightOut03._arcLevel = 254;
  lightOut03._group0_7    = 0x11;
  lightOut03._group8_15    = 0;

  lightArray[2]       = lightOut03;
  circleArray[2]      = CircleLightOut03;

  circleArcArray[2]   = Circle3;

  lightOut04._address  = ADDRESS04;
  lightOut04._on       = FALSE;
  lightOut04._arcLevel = 254;
  lightOut04._group0_7    = 0x15;
  lightOut04._group8_15   = 0;

  lightArray[3]       = lightOut04;
  circleArray[3]      = CircleLightOut04;

  circleArcArray[3]   = Circle4;

  lightOut05._address  = ADDRESS05;
  lightOut05._on       = FALSE;
  lightOut05._arcLevel = 254;
  lightOut05._group0_7    = 0x29;
  lightOut05._group8_15   = 0;

  lightArray[4]       = lightOut05;
  circleArray[4]      = CircleLightOut05;

  circleArcArray[4]   = Circle5;

  lightOut06._address  = ADDRESS06;   //hw specific
  lightOut06._on       = FALSE;
  lightOut06._arcLevel = 254;
  lightOut06._group0_7    = 0x2D;
  lightOut06._group8_15   = 0;

  lightArray[5]       = lightOut06;
  circleArray[5]      = CircleLightOut06;

  circleArcArray[5]   = Circle6;

  lightIn01._address  = ADDRESS07;    //hw specific
  lightIn01._on       = FALSE;
  lightIn01._arcLevel = 254;
  lightIn01._group0_7    = 0x2A;
  lightIn01._group8_15   = 0;

  lightArray[6]      = lightIn01;
  boxArray[0]        = BoxLightIn01;

  boxArcArray[0]     = Box1;

  lightIn02._address  = ADDRESS08;
  lightIn02._on       = FALSE;
  lightIn02._arcLevel = 254;
  lightIn02._group0_7    = 0x2A;
  lightIn02._group8_15   = 0;

  lightArray[7]      = lightIn02;
  boxArray[1]        = BoxLightIn02;

  boxArcArray[1]     = Box2;

  lightIn03._address  = ADDRESS09;
  lightIn03._on       = FALSE;
  lightIn03._arcLevel = 254;
  lightIn03._group0_7    = 0x2A;
  lightIn03._group8_15   = 0;

  lightArray[8]      = lightIn03;
  boxArray[2]        = BoxLightIn03;

  boxArcArray[2]     = Box3;

  lightIn04._address  = ADDRESS10;
  lightIn04._on       = FALSE;
  lightIn04._arcLevel = 254;
  lightIn04._group0_7    = 0x3A;
  lightIn04._group8_15   = 0;

  lightArray[9]      = lightIn04;
  boxArray[3]        = BoxLightIn04;

  boxArcArray[3]     = Box4;
}