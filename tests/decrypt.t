# vim:ft=

use lib '.';
use TestLua;

plan tests => 2 * blocks();

run_tests();

__DATA__

=== TEST 1: decode example2.json file
--- lua
local cjson = require "cjson"
local json_file = io.open("tests/example2.json.enc", "r")
if json_file then
    local json_text = json_file:read("*a")
    json_file:close()
    
    local data = cjson.decode(json_text)
    
    if data and type(data) == "table" then
        print("example2.json decoded successfully:")
        print("ID: " .. data.menu.id)
    else
        print("Failed to decode example2.json")
    end
else
    print("Failed to open example2.json")  
end
--- out
example2.json decoded successfully:
ID: file
