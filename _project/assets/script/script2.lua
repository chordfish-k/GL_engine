-- 获取全局状态表中的对象池
ObjectPool = {}

function Start()
    print(scriptName .. " Start ")
    ObjectPool = GetGlobal("ObjectPool")
end

function Update(dt)
    print(scriptName .. " Update")
    -- 向对象池中添加子弹
--     table.insert(ObjectPool["bullets"], { x = 100, y = 200 })
--     print(ObjectPool["bullets"])
--     for k,v in ipairs(ObjectPool["bullets"]) do
--         print(k,v)
--     end
end
