## Program Structure
The most important parts of the api are the callbacks

##### cel8.load( )
This function is called exactly once at the beginning of the game.

##### cel8.update( )
Called once per update at 60fps.

##### cel8.draw( )
Called once per frame.

More interactive games can override additional callbacks in order to handle input from the user.

##### cel8.keydown( key )
Called when a key is pressed.

##### cel8.keyup( key )
Called when a key is released.

A simple program that overrides various callback functions might look this:

```lua
function cel8.load(key)
  x, y = 0, 0
end

function cel8.keydown(key)
  if key == "up" then
    y = y - 1
  elseif key == "down" then
    y = y + 1
  elseif key == "left" then
    x = x - 1
  elseif key == "right" then
    x = x + 1
  end
end

function cel8.draw()
  cls(0x00, 0x00)
  color(0x08)
  put(x, y, "@")
end
```