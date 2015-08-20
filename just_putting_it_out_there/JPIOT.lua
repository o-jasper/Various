local string_split = require "string_split"

local JPIOT = {}

JPIOT.__index = JPIOT

function JPIOT.new(self)
   self = self or {}
   self.words = self.words or {}
   return setmetatable(self, JPIOT)
end

function JPIOT:feed_list(list, first, last)
   if last == nil or last then
      table.insert(list, last or ":last:")
   end
   local prev = first or ":first:"
   for i, el in ipairs(list) do
      local got = self.words[prev]
      -- Note: commented out is some stuff for randomly generating that turned
      --  out unnecessary.
      if not got then
         got = {cnts={}, total=0 } --, list={}}
         self.words[prev] = got
      end
      if not got.cnts[el] then
         got.cnts[el] = 1
         --table.insert(got.list, el)
      else
         got.cnts[el] = got.cnts[el] + 1
      end
      got.total = got.total + 1

      prev = el
   end
end

function JPIOT:feed(str, first, last)
   return self:feed_list(string_split(str), first, last)
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

return JPIOT
