local gm = gekinmon
gm.Register("Dialog Display", "GeekyLink", "0.1", "GPL3", "Renders dialog to the screen")
gm.RegisterEvent("NewDialog", "TextFunc") 
gm.RegisterEvent("EndDialog", "CompletedFunc") 
gm.RegisterEvent("KeyPress", "KeyFunc")

OptSprites = {}
TextSprites = {}
DialogOn = false
SelectedOption = 0
Subtracter = 0

-- Put the text and all options on the screen
function TextFunc(NPCNum, DNum, BNum)
    SelectedOption, Subtracter = 0, 0
    DialogOn = true;
    ClearSprites()

    if DialogSprite == nil then
        DialogSprite = gm.CreateSprite("Data/dialogbg.png", 255, 0, 255)
        MidBoxPointX = gm.GetWindowWidth() / 2 - gm.GetSpriteWidth(DialogSprite) / 2
        MidBoxPointY = gm.GetWindowHeight() / 2 + gm.GetSpriteHeight(DialogSprite) / 2

        gm.SetSpriteXY(DialogSprite, MidBoxPointX, MidBoxPointY)
        gm.DisplaySprite(DialogSprite, true)
    end

    Text = gm.GetDialog(NPCNum, DNum, BNum)    
    if string.len(Text) <= 42 then
        TextSprites[0] = gm.CreateText(0, Text, MidBoxPointX+16, MidBoxPointY-5, 255, 255, 255)
        TextCount = 1
    else -- We need to split the text across several lines
        SplitPoint = 0
        MaxSize = 42
        TextCount = 0

        -- This will split the text up across several lines
        -- It makes sure that we don't split a word in half as well
        while SplitPoint < string.len(Text) do
            if TextCount == 4 then
                gm.Log("Too much text is bad, mmkay? Truncating any extra text.")
                break
            end
            TempText = string.sub(Text, SplitPoint, SplitPoint + MaxSize)
            if SplitPoint + MaxSize < string.len(Text) then
                Pos = string.find(string.reverse(TempText), " ")
                if Pos > 0 then
                    TempText = string.sub(TempText, 0, -Pos)
                    SplitPoint = SplitPoint - Pos + 2 
                end
            end

            TextSprites[TextCount] = gm.CreateText(0, TempText, MidBoxPointX+16, MidBoxPointY-5+TextCount*16, 255, 255, 255)
            SplitPoint = SplitPoint + MaxSize
            TextCount = TextCount + 1
        end
    end

    -- Display the options
    OptCount = gm.GetDialogOptionCount(NPCNum, DNum, BNum)
    for i = 0, OptCount-1 do
        OptSprites[i] = gm.CreateText(0, gm.GetDialogOption(NPCNum, DNum, BNum, i), MidBoxPointX+32, MidBoxPointY+11+(i+TextCount-1)*16, 255, 255, 255)
        if OptCount + TextCount > 4 then
            gm.DisplaySprite(OptSprites[i], false)
        end
    end

    if OptCount > 0 and CursorSprite == nil then
        CursorSprite = gm.CreateSprite("Data/Cursor.png", MidBoxPointX+16, MidBoxPointY+20+(TextCount-1)*16, 255, 0, 255)
        if OptCount + TextCount > 4 then
            gm.DisplaySprite(CursorSprite, false)
        end
    end
    SetView()
end

-- Clear any remaining sprites from the text or options
function CompletedFunc(NPCNum, DNum)
    DialogOn = false
    ClearSprites()
    if DialogSprite ~= nil then
        gm.DeleteSprite(DialogSprite)
        DialogSprite = nil
    end
end

-- We'll use this to select an option
function KeyFunc(Key)
    if not DialogOn then
        return
    end
    
    if Key == 274 then -- Down arrow
        if OptCount + TextCount > 4 and Subtracter < OptCount + TextCount - 4 then
            Subtracter = Subtracter + 1
        elseif SelectedOption < OptCount - 1 then
            SelectedOption = SelectedOption + 1
        end 
    elseif Key == 273 then -- Up arrow
        if OptCount + TextCount > 4 and Subtracter > 0 and SelectedOption < OptCount - 1 then
            Subtracter = Subtracter - 1
        elseif SelectedOption > 0 then
            SelectedOption = SelectedOption - 1
        end
    -- F key, we want to only return false if they don't have any selected option yet
    elseif Key == 102 and TextCount == 4 and Subtracter == 0 then 
        return false
    end

    gm.SetSpriteY(CursorSprite, MidBoxPointY+20+(SelectedOption+TextCount-1)*16)
    gm.SetDialogOptionID(SelectedOption)
    SetView()
end

function SetView()
    Count = 0
    for i = Subtracter, TextCount do
        gm.SetSpriteY(TextSprites[i], MidBoxPointY-5+(i-Subtracter)*16)
        gm.DisplaySprite(TextSprites[i], true)
        Count = Count + 1
    end
    for i = Subtracter - 1, 0, -1 do
        gm.DisplaySprite(TextSprites[i], false)
    end

    for i = 0, OptCount do
        gm.SetSpriteY(OptSprites[i], MidBoxPointY-5+(Count+i-1)*16)
        gm.DisplaySprite(OptSprites[i], true)
    end
    for i = 4 - Count + 1, OptCount do
        gm.DisplaySprite(OptSprites[i], false)
    end

    if Count <= 4 then
        gm.SetSpriteY(CursorSprite, MidBoxPointY+20+(Count+SelectedOption-2)*16)
        gm.DisplaySprite(CursorSprite, true)
    else
        gm.DisplaySprite(CursorSprite, false)
    end
end

-- Remove all previous option text and the actual text
function ClearSprites()
    for i = # TextSprites, 0, -1 do
        gm.DeleteSprite(TextSprites[i])
        table.remove(TextSprites, i)
    end 
    for i = # OptSprites, 0, -1 do
        gm.DeleteSprite(OptSprites[i])
        table.remove(OptSprites, i)
    end 
    if CursorSprite ~= nil then
        gm.DeleteSprite(CursorSprite)
        CursorSprite = nil
    end
end
