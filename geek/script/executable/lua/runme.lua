-- file: runme.lua

-- This file illustrates class C++ interface generated
-- by SWIG.

---- importing ----
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	-- lua5.0 doesnt have a nice way to do this
	lib=loadlib('geek_script_wrapper.dll','luaopen_geek_script_wrapper') or loadlib('geek_script_wrapper.so','luaopen_geek_script_wrapper')
	assert(lib)()
else
	-- lua 5.1 does
	require('geek_script_wrapper')
end

----- Object creation -----


component = geek_script_wrapper.component_wrapper("script")
iface = geek_script_wrapper.script_interface_wrapper(component)

iface:hello_world();

script.wait_sec(1);

iface:hello_world();

script.wait_sec(2);

iface:hello_world();
script.wait_sec(1);

iface:hello_world();

script.wait_sec(1);



print "Goodbye"
