#!/usr/bin/env lua

-- This is a comment
function titleline(title, signal, length)
   --[[
      This function is for printing titleline.
   ]]
   if title == nil then
      title = "Title Miss."
   end
   if signal == nil then
      signal = "-"
   else if type(signal) == "table" then
	 if signal.length ~= nil then
	    length = signal.length
	 end
	 signal = signal.signal
      end
   end
   if type(length) == "nil" then -- another way for nil
      length = 40
   end
   local left = (length - #title) // 2
   local right = length - left - #title
   local line = ""
   for i=1,left-1 do
      line = line .. signal
   end
   line = line .. " " .. title .. " "
   for i=1,right-1 do
      line = line .. signal
   end
   return line
end

print(titleline("Hello World!"))
print("There's no variable a, and a =", a)
a = nil
print("After execute a = nil, a =", a)
print("<< You can find the default delimiter of print function is tab, and if you assign a variable with nil, the variable is deleted.")
print("Put a = 1 now.")
a = 1
print("If you want to print a = " .. a .. ", you can use .. to connect two data (except nil).")
print()

print(titleline("Data Structure"))

print(titleline("Table","*"))
table1 = {key1 = "val1", key2 = "val2"}
for k, v in pairs(table1) do
   print(k .. " - " .. v)
end
print('If you execute this program again, you may find that the order of table printing changes.')

print(titleline("Boolean", "*"))
if 0 then
   bool_0 = "true"
else
   bool_0 = "false"
end
print("You may need to attention about 0, it's " .. bool_0 .. ".")

print(titleline("Others", {signal="*"}))
print("The index in Lua starts from 1 instead of 0.")
