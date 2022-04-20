function cel8.boot()
  -- handler table:
  local function makewrapper(name)
    return function(...)
      if cel8[name] then return cel8[name](...) end
    end
  end
  cel8.handlers = {
    ["quit"] = function() end,
    ["mousepressed"] = makewrapper("mousepressed"),
    ["mousereleased"] = makewrapper("mousereleased"),
    ["mousemoved"] = makewrapper("mousemoved"),
    ["mousescroll"] = makewrapper("mousescroll"),
    ["keypressed"] = makewrapper("keypressed"),
    ["keyreleased"] = makewrapper("keyreleased"),
  }

  -- arguments:
  for i, v in ipairs { cel8.argv[1], cel8.argv[2] } do
    print("argument["..i.."] "..v)
  end

  -- main:
  require("main")

  if cel8.load then cel8.load() end
end

function cel8.frame()
  cel8.step()

  while true do
    local name, a,b,c,d = cel8.poll()
    if not name then
      break
    end
    cel8.handlers[name](a, b, c, d)
  end

  if cel8.update then cel8.update() end
  if cel8.draw then cel8.draw() end
end

function cel8.err(msg)
  -- split the string into chuncks
  local split = function(str, size)
    local s = {}
    for i = 1, #str, size do
      s[#s + 1] = str:sub(i, i + size - 1)
    end
    return s
  end

  local file, err = msg:match("(.-:%d+): (.+)")

  function cel8.frame()
    while true do
      local name, a,b,c,d = cel8.poll()
      if not name then
        break
      end
      if name == "keypressed" and a == "esc" then
        return
      end
    end

    -- bluescreen:
    cel8.cls()

    cel8.color(0x08)
    cel8.put(0, 0, "Error:")

    cel8.color(0x07)
    local sstr = split(msg, 15)
    for i, s in ipairs(sstr) do
      cel8.put(1, i, s)
    end
  end
end

xpcall(cel8.boot, cel8.err)