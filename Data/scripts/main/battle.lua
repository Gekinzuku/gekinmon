local gm = gekinmon
gm.Register("Battle tester", "GeekyLink", "0.1", "GPL3", "Displays battle info")
gm.RegisterEvent("KeyPress", "KeyFunc")

function KeyFunc(Key)
    if Key == 98 then -- b
        gm.Attack(false, 0, true, 0); -- Ally attack Enemy
        print("Number of allies: " .. gm.GetAllyCount())
        print("Number of enemies: " .. gm.GetEnemyCount())
        print("Your HP: " .. gm.GetAllyHP(0))
        print("Enemy HP: " .. gm.GetEnemyHP(0))
    elseif Key == 99 then -- c
        print("Mode: " .. gm.GetGameMode())
    end
end
