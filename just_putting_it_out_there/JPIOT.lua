local string_split = require "string_split"

local JPIOT = {}
JPIOT.__index = JPIOT

function JPIOT.new(self)
   self = self or {}
   self.words = self.words or {}
   return setmetatable(self, JPIOT)
end

function JPIOT:feed_list(list, weight, first, last)
   weight = weight or 1
   if last == nil or last then
      table.insert(list, last or ":last:")
   end
   local prev = first or ":first:"
   if weight <= 0 then -- Negative, Substract.. Needs to take account with disappearance.
      if weight == 0 then return end  -- This is doing nothing with it.
      error("not supported yet")
   else  -- Positive, add.
      for i, el in ipairs(list) do
         local got = self.words[prev]
         -- Note: commented out is some stuff for randomly generating that turned
         --  out unnecessary.
         if not got then
            got = {cnts={}, total=0 } --, list={}}
            self.words[prev] = got
         end
         if not got.cnts[el] then
            got.cnts[el] = weight
            --table.insert(got.list, el)
         else
            got.cnts[el] = got.cnts[el] + weight
         end
         got.total = got.total + weight
         
         prev = el
      end
   end
end

function JPIOT:feed(str, weight, first, last)
   return self:feed_list(string_split(str), weight, first, last)
end

local function next_word(words, cnts, i)
   for new_word, cnt in pairs(cnts) do
      i = i - cnt
      if i <= 0 then
         return new_word
      end
   end
   error("total should be equal to sum, so this shouldnt happen")
end

function JPIOT:produce(n, first, random)
   random = random or math.random
   local result, word = {}, first or ":first:"
   while n > 0 do
      local cur = self.words[word]
      word = next_word(self.words, cur.cnts, random(cur.total))
      if word == ":last:" then return result end
      table.insert(result, word)
      n = n - 1
   end
   return result
end

-- TODO produce backwards? Probably much more cpu intensive,
--  but possible.
--
-- Produce based on a set of provided (lowest-probability?)words?
-- (just generate until you hit one?)
--
-- 

return JPIOT
