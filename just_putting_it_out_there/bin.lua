local JPIOT = require "JPIOT"

local jpiot = JPIOT.new()

local fd = io.open(arg[1])

local line = true
while line do
   line = fd:read("l")
   if line then
      jpiot:feed(string.lower(line))
   end
end
fd:close()

local n = tonumber(arg[2]) or 10
while n > 0 do
   print(table.concat(jpiot:produce(tonumber(arg[3]) or 30), " "))
   n = n - 1
end
