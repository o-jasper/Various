local lousy = require("lousy")
local chrome = require("chrome")

local stylesheet = [==[
]==]

local html = [==[
<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>Items</title>
    <style type="text/css">
        {%stylesheet}
    </style>
</head>
<body>
MAH INFO <span id="info"></span> count: <span id="cnt"></span>
</body>
</html>
]==]

local js = [==[
document.getElementById("info").innerText = "{%info}";
document.getElementById("cnt").innerText = "{%cnt}";
]==]

local chrome_name = "simplechrome"
local chrome_uri = string.format("luakit://%s/", chrome_name)

local mah_info  = "i can display this string"
local cnt = 0

chrome.add(chrome_name, function (view, meta)

    local html = string.gsub(html, "{%%(%w+)}", { stylesheet = stylesheet })
    view:load_string(html, chrome_uri)

    function on_first_visual(_, status)
       print(status, view.uri. chrome_uri)
        -- Wait for new page to be created
        if status ~= "first-visual" then return end

        -- Hack to run-once
        view:remove_signal("load-status", on_first_visual)

        -- Double check that we are where we should be
        if view.uri ~= chrome_uri then return end

        cnt = cnt + 1
        local run_js = string.gsub(js, "{%%(%w+)}", { info = mah_info, cnt = cnt })
        print(run_js)
        local _, err = view:eval_js(run_js, { no_return = true })
        assert(not err, err)
    end

    view:add_signal("load-status", on_first_visual)
end)

local cmd = lousy.bind.cmd
add_cmds({
    cmd(chrome_name, "Open simple chrome page.",
        function (w)  w:new_tab(chrome_uri) end) })
 
