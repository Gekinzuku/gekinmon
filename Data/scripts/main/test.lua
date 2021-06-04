local gm = gekinmon
gm.Register("Tester", "GeekyLink", "0.1", "GPL3", "Loads and unloads a script")
gm.RegisterEvent("KeyPress", "KeyFunc")

function KeyFunc(Key)
    if Key == 116 then -- T
        gm.LoadScript("main/test2.lua");
    end
    if Key == 114 then -- R
        gm.UnloadScript("Child Script")
    end
end
