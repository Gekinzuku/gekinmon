#include "area.h"
#include "sprite.h"
#include "window.h"
#include "player.h"
#include "npc.h"
#include "xml/tinystr.h"
#include "xml/tinyxml.h"

AreaClass Area;

/** Warning: This function might take some time to complete on a slow PC. */
int AreaClass::Load(const char * FileName, const char * Entrance)
{
    // First, initlize the XML stuff
    TiXmlDocument Doc(FileName);
    if (!Doc.LoadFile()) return -1;
    TiXmlHandle hDoc(&Doc);
    TiXmlElement * Root = hDoc.FirstChildElement().Element();
    if (!Root) return -1;
    TiXmlHandle hRoot(0);
    hRoot = TiXmlHandle(Root);

    AreaName = hRoot.FirstChild("Title").Element()->GetText();

    // Next step: find the entrance to spawn at
    int StartX = -1, StartY = -1, StartZ = -1;
    for (TiXmlElement * hElement = 
            hRoot.FirstChild("Entrances").FirstChild().Element(); hElement; 
            hElement = hElement->NextSiblingElement())
    {
        cout << "Entrance: " << hElement->GetText() << " " << 
                hElement->Attribute("X") << ", " << hElement->Attribute("Y") <<
                endl;

        if (strcmp(hElement->GetText(),Entrance) == 0) {
            if (hElement->Attribute("X"))
                StartX = atoi(hElement->Attribute("X"));
            if (hElement->Attribute("Y"))
                StartY = atoi(hElement->Attribute("Y"));
            if (hElement->Attribute("Z"))
                StartZ = atoi(hElement->Attribute("Z"));
            break;
        }
    }
    if (StartX == -1 || StartY == -1 || StartZ == -1) return -1;    
    Player.SetLoc(StartX,StartY+8);
    Player.SetZ(StartZ);

    // Let's load up some NPCs now
    DeleteAllNPCs();
    for (TiXmlElement * hElement =
            hRoot.FirstChild("Objects").FirstChild().Element(); hElement;
            hElement = hElement->NextSiblingElement()) 
    {
        TiXmlElement * TmpElement = hElement->FirstChildElement("File");
        int NPCNum = -1;
        if (TmpElement) {
            string NPCFileName = "Data/"; 
            NPCFileName.append(TmpElement->GetText());
            NPCNum = AddNPC(Window.LoadSprite(NPCFileName.c_str(), 
                            255, 0, 255));
        } else return -1;
        // Adds the dialogue 
        for (TmpElement = hElement->FirstChildElement("Dialogue"); TmpElement;
                 TmpElement = TmpElement->NextSiblingElement("Dialogue")) {
            DialogueStruct TmpDialogue; bool Add = false;
            for (TiXmlElement * BoxElement = 
                    TmpElement->FirstChildElement("Box"); BoxElement; 
                    BoxElement = BoxElement->NextSiblingElement("Box")) {

                MessageStruct TmpMessage;
                if (BoxElement->Attribute("id"))
                    TmpMessage.ID = BoxElement->Attribute("id");

                TiXmlElement * TxtEle = BoxElement->FirstChildElement("Text");
                if (!TxtEle) break;
                TmpMessage.Message = TxtEle->GetText();

                TiXmlElement * EndEle = BoxElement->FirstChildElement("End");
                if (!EndEle) TmpMessage.End = false;
                else TmpMessage.End = true;

                TiXmlElement * BattleEle = BoxElement->FirstChildElement("Battle");
                if (!BattleEle) TmpMessage.Battle = false;
                else TmpMessage.Battle = true;

                // Selections the player can pick
                for (TiXmlElement * OptEle = 
                        BoxElement->FirstChildElement("Option"); OptEle;
                        OptEle = OptEle->NextSiblingElement("Option"))
                {
                    OptionStruct Option;
                    Option.Title = OptEle->GetText();
                    if (OptEle->Attribute("jump"))
                        Option.JumpID = OptEle->Attribute("jump");
                    TmpMessage.Option.push_back(Option);
                }

                TmpDialogue.Message.push_back(TmpMessage);
                Add = true;
            }
            if (Add) NPC[NPCNum].AddDialogue(TmpDialogue);
        }
        // Determine where to spawn the NPC
        TmpElement = hElement->FirstChildElement("Spawn");
        if (TmpElement) {
            int X = -1, Y = -1, Z = -1;
            if (TmpElement->Attribute("X")) 
                X = atoi(TmpElement->Attribute("X"));
            if (TmpElement->Attribute("Y")) 
                Y = atoi(TmpElement->Attribute("Y"));
            if (TmpElement->Attribute("Z"))
                Z = atoi(TmpElement->Attribute("Z"));
            if (X == -1 || Y == -1 || Z == -1) {
                DeleteAllNPCs();
                return -1;
            }
            NPC[NPCNum].SetLoc(X,Y+8);
            NPC[NPCNum].SetZ(Z);
            NPC[NPCNum].UpdateSprite();
            SetSpriteOrder(NPC[NPCNum].GetSprite(), 0);
            NPC[NPCNum].SplitFrames(16, 24);
            NPC[NPCNum].SetFrame(FACE_DOWN);
        }
        const char * ID = hElement->Attribute("id");
        if (ID == NULL) {
            DeleteAllNPCs();
            return -1;
        }
        NPC[NPCNum].SetID(ID);
    }

    // Get the various file names we need.
    string BGFileName = "Data/";
    BGFileName.append(hRoot.FirstChild("Background").Element()->GetText());
    if (Background != -1) DeleteSprite(Background);
    Background = Window.LoadSprite(BGFileName);
    SetSpriteOrder(Background, -1);

    string FGFileName;
    for (unsigned int i = 0; i < Foreground.size(); ++i) {
        DeleteSprite(Foreground[i]);
    }
    Foreground.clear();
    for (TiXmlElement * hElement = 
            hRoot.FirstChild("Foregrounds").FirstChild().Element(); hElement;
            hElement = hElement->NextSiblingElement()) {
        FGFileName = "Data/"; FGFileName.append(hElement->GetText());
        if (FGFileName != "") {
            Foreground.push_back(Window.LoadSprite(FGFileName,255,0,255));
            SetSpriteOrder(Foreground[Foreground.size()-1], 0);
        }
    }

    // Here we are going to actually load and map out the collision map
    if (!hRoot.FirstChild("Collision").Element()) return -1;
    string ColMapFileName = "Data/"; 
    ColMapFileName.append(hRoot.FirstChild("Collision").Element()->GetText());
    SDL_Surface * tmpImage = NULL;
    SDL_Surface * OptImage = NULL;
    tmpImage = IMG_Load(ColMapFileName.c_str());

    if (tmpImage != NULL) {
        OptImage = SDL_DisplayFormat(tmpImage);
        SDL_FreeSurface(tmpImage);

        Uint32 * Pixels = (Uint32 *)OptImage->pixels;   
        SDL_PixelFormat *fmt = OptImage->format;

        Width = OptImage->w;
        Height = OptImage->h;
        ScrollX = ScrollY = 0;

        // Now for the fun part of mapping out the collision map. 
        // This may take a while depending on the size of the map and your PC
        if (Tiles != NULL) delete[] Tiles;
        Tiles = new int[Width*Height];

        for (int i = 0; i < Width*Height; ++i) {
            Uint8 red = (Uint8) (((Pixels[i] & fmt->Rmask) >> fmt->Rshift) << fmt->Rloss);
            Uint8 green = (Uint8) (((Pixels[i] & fmt->Gmask) >> fmt->Gshift) << fmt->Gloss);
            Uint8 blue = (Uint8) (((Pixels[i] & fmt->Bmask) >> fmt->Bshift) << fmt->Bloss);
            if (red == 255 && green == 0 && blue == 0) {
                Tiles[i] = 1;
            } else if (blue == 0 && red == 0 && green == 255) {
                Tiles[i] = 2;
            } else {
                Tiles[i] = 0;
            }
        }

        SDL_FreeSurface(OptImage);
        return 0;
    }
    return -1;
}

void AreaClass::Update()
{
    if (Background != -1)
        Sprite[Background].SetLoc(
         ((ScrollX == 0) ? 0 : ScrollX-1), ((ScrollY == 0) ? 0 : ScrollY-1));
    for (unsigned int i = 0; i < Foreground.size(); ++i) {
        Sprite[Foreground[i]].SetLoc(
         ((ScrollX == 0) ? 0 : ScrollX-1), ((ScrollY == 0) ? 0 : ScrollY-1));
    }
}

int AreaClass::CheckCollision(int Ox, int Oy, int Ow, int Oh)
{
    int Count[2] = {0,0};
    for (int x = 0; x < Ow; ++x) {
        for (int y = 0; y < Oh; ++y) {
            if (Tiles[(Oy+y)*Width+(Ox+x)] == 1) ++Count[0];
            if (Tiles[(Oy+y)*Width+(Ox+x)] == 2) ++Count[1];
        }
    }
    if (Count[0] > (Ow*Oh)/8) return 1;
    if (Count[1] > (Ow*Oh)/8) return 2;
    return 0;
}

// Depending on the Z level, set us above or below the foregrounds
void AreaClass::SetZLevel(int SpriteNum, int Level)
{
    if (Foreground.size() == 0 || Level > Foreground.size() || Level < 0) 
        return;

    // If the level is 0, we gotta set it behind all foregrounds
    if (Level == 0) {
        cout << Foreground[0] << ", " << Background << endl;
        ForceSwapSpriteBehind(SpriteNum, Foreground[0]);
        return;
    }

    // Set the sprite in front of the proper foreground 
    ForceSwapSpriteInFrontOf(SpriteNum, Foreground[Level-1]);
}

