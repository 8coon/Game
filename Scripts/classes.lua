
current_class = {}


function extends(class_name, class)
	print(class_name)
	table.insert(current_class.classes, {name = class_name, fields = _G[class_name].fields})
end


function make_env()
	local env = {
		print = print,
		assert = assert,
		error = error,
		getmetatable = getmetatable,
		ipairs = ipairs,
		next = next,
		pairs = pairs,
		pcall = pcall,
		rawequal = rawequal,
		rawget = rawget,
		rawset = rawset,
		select = select,
		setmetatable = setmetatable,
		tonumber = tonumber,
		tostring = tostring,
		type = type,
		unpack = unpack,
		_VERSION = _VERSION,
		xpcall = xpcall,
		coroutine = coroutine,
		string = string,
		table = table,
		math = math,

		_EXTENDS = extends,
	}

	return env
end


function create_class(name, class_func)
	local classes = {}

	local class = {
		classes = classes,
		name = function() return name end,
		fields = {},

		new = function(...)
			local result = {}

			for i, class in ipairs(classes) do
				result[class.name] = {}

				for k, v in pairs(class.fields) do
					result[class.name][k] = v
					result[k] = v
				end

				constructor = class.fields.init or function() end
				constructor(result, ...)

				for k, v in pairs(result[class.name]) do
					--result[k] = v
				end
			end

			return result
		end,
	}

	current_class = class
	local env = make_env()
	local fields = class.fields

	do
		local _ENV = env
		class.fields = class_func()
		table.insert(classes, {name = name, fields = class.fields})
	end

	return class
end
