# vim:ft=

use lib '.';
use TestLua;

plan tests => 2 * blocks();

run_tests();

__DATA__

=== TEST 1: decode encrypted json
--- lua
local cjson = require "cjson"
local enc_json_file = io.open("tests/example2.json.enc", "r")
if enc_json_file then
    local json_text = enc_json_file:read("*a")
    enc_json_file:close()

    local data = cjson.decode(json_text)

    if data and type(data) == "table" then
        print(data.menu.popup.menuitem[3].onclick)
    else
        print("Failed to decode example2.json")
    end
else
    print("Failed to open example2.json")  
end
--- out
CloseDoc()



=== TEST 2: decode mixed encrypted and non-encrypted json
--- lua
local cjson = require "cjson"
local enc_json_file = io.open("tests/example2.json.enc", "r")
if enc_json_file then
    local json_text = enc_json_file:read("*a")
    enc_json_file:close()
    
    local data = cjson.decode(json_text)
    
    if data and type(data) == "table" then
        print(data.menu.popup.menuitem[3].onclick)
    else
        print("Failed to decode example2.json")
    end
else
    print("Failed to open example2.json")  
end

local json_file = io.open("tests/example2.json", "r")
if json_file then
    local json_text = json_file:read("*a")
    json_file:close()
    
    local data = cjson.decode(json_text)
    
    if data and type(data) == "table" then
        print(data.menu.popup.menuitem[3].onclick)
    else
        print("Failed to decode example2.json")
    end
else
    print("Failed to open example2.json")  
end
--- out
CloseDoc()
CloseDoc()



=== TEST 3: decode no-encrypted json start with `[`
--- lua
local cjson = require "cjson"
local json_file = io.open("tests/numbers.json", "r")
if json_file then
    local json_text = json_file:read("*a")
    json_file:close()
    
    local data = cjson.decode(json_text)
    
    if data and type(data) == "table" then
        print(data[1])
    else
        print("Failed to decode example2.json")
    end
else
    print("Failed to open example2.json")  
end
--- out
0.110001
