-- ��ȡȫ��״̬���еĶ����
ObjectPool = {}

function Start()
    print(scriptName .. " Start ")
    ObjectPool = GetGlobal("ObjectPool")
end

function Update(dt)
    print(scriptName .. " Update")
    -- ������������ӵ�
--     table.insert(ObjectPool["bullets"], { x = 100, y = 200 })
--     print(ObjectPool["bullets"])
--     for k,v in ipairs(ObjectPool["bullets"]) do
--         print(k,v)
--     end
end
