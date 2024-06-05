local ObjectPool = {
    bullets = {}
}


function Start()
    print(scriptName .. " Start")
    SetGlobal("ObjectPool", ObjectPool)
end


function Update(dt)
    print(scriptName .. " Update")

end
