var html = "";
for(k in named_patterns) {
    html += "<button onclick='gui_sel(\"" + k + "\")'>" + k + "</button>";
}

ge("option_list").innerHTML = html;

// https://stackoverflow.com/questions/2090551/parse-query-string-in-javascript
function parse_query(qstr) {
    var query = {};
    var a = qstr.substr(1).split('&');
    for (var i = 0; i < a.length; i++) {
        var b = a[i].split('=');
        query[decodeURIComponent(b[0])] = decodeURIComponent(b[1] || '');
    }
    return query;
}
var qs = parse_query(location.search)
if(qs.max_cnt){ ge("max_cnt").value = qs.max_cnt; }
if(qs.stop_previous){ ge("add_manner").checked = (qs.stop_previous == "true"); }

function gui_sel(name) {
    if(ge("reset").checked) {
        reset_canvas();
    }

    sel_pattern(name, ge("add_manner").checked, qs);
}

gui_sel(location.hash.substr(1) || "fancied");
run_continuous();
