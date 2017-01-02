
#class Game
	init = function(self, c)
		self.c = c
		print("Initialized! ", c)
	end

	debug = function(self, a, b)
		print("c * (a + b) = " .. tostring(self.c * (a + b)))
	end
#endclass


#class LocalGame
	init = function(self, name)
		self.name = name
	end

	debug = function(self)
		print("Name: " .. self.name)
	end
#endclass


#class NewerGame
#extends Game
	debug = function(self, a, b)
		print("c + a + b = " .. tostring(self.c + a + b))
	end
#endclass


--local game = Game:new(10)

local game = Game.new(10)
local local_game = LocalGame.new("Jasper")
local newer_game = NewerGame.new(1)


game:debug(1, 2)
local_game:debug()
newer_game:debug(1, 2)









