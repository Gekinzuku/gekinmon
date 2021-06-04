local gm = gekinmon
gm.Register("Child Script", "GeekyLink", "0.1", "GPL3", "Just a child plugin to test dynamic loading")
gm.RegisterEvent("KeyPress", "keyp")

function keyp(Key)
    print(Key)
end
