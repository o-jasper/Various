//  Jasper den Ouden
// Placed in the Public domain.

w = 600, h = 600;
var ctx = ge("work_canvas").getContext("2d");
var imgData = ctx.createImageData(w, h);
function draw_it() { 
    ctx.putImageData(imgData, 10, 10); // No idea what the ten stands for.
}


pix = imgData.data;

function reset_canvas() {
    imgData = ctx.createImageData(w, h);
    pix = imgData.data;
    for (var i = 0; i < pix.length; i ++) { pix[i] = 255; }
}
for (var i = 0; i < pix.length; i ++) { pix[i] = 255; }

//ctx.putImageData(imgData, 10, 10);

function i_of_xy(x,y) { return (4*(x + w * y))%(4*w*h); }

function just_color(self) {
    for( var k = 0 ; k < self.color.length ; k++ ) {
        if(self.i < w*h*4) {
            pix[self.i + k] = self.color[k]%256;
        }
    }
}

var actslist = [], nextlist = [];
var max_cnt = parseInt((ge("max_cnt") || {}).value) || 10000, have_cut = false;
ge("max_cnt").value = max_cnt;

function reg(what, n) {
    n = n || 1;
    if( n==0 ){ 
        if(actslist.length < max_cnt) {
            actslist.push(what);
        } else{ have_cut = true; }
    } else {
        if( nextlist[n - 1] ) {
            if( nextlist[n - 1].length < max_cnt ) {
                nextlist[n - 1].push(what);
            } else{ have_cut = true; }
        } else {
            nextlist[n - 1] = [what];
        }
    }
}

named_patterns = {}, cur_params = null;
function sel_pattern(name, replace, overrides) {
    var got = named_patterns[name]
    if(!got){ alert("Dont have: " + name); return; }
    if(replace){
        actslist = []; nextlist = [];
        cur_params = {}
        for(var k in got.params){ cur_params[k] = got.params[k]; }
    } else {  // Changes the params(including that of the older one, sorry.
        old_params = cur_params;
        cur_params = {}
        for(var k in old_params){ cur_params[k] = old_params[k]; }
        for(var k in got.params){ cur_params[k] = got.params[k]; }
    }
    if(overrides){ for(var k in overrides){ cur_params[k] = parseFloat(overrides[k]); } }
    got.setup(cur_params);

    have_cut = false;
}

var draw_every = 1000;
var cnt = 200000;

function run() {
    while(cnt > 0 ) {
        //actslist.sort(function (a, b) { return a.priority > b.priority });
        var got = actslist.pop();
        got.fun(got);
        
        if(actslist.length == 0) {
            if( nextlist.length > 0 ) {
                actslist = nextlist.pop();
            } else {
                break;
            }
        }
        cnt -= 1;
        if(actslist.length == 0){ break; }
        if( cnt % draw_every == 0 ){ draw_it(); }
    }
    draw_it();
}

function run_continuous() {
    var busy = false, done = false, step_i = 0;
    setInterval(function () {
        if( busy ){ return; }
        ge("cnts").textContent = nextlist.length;
        ge("acts_cnt").textContent = actslist.length;
        ge("step_i").textContent = step_i;
        ge("have_cut").textContent = (have_cut ? "YES" : "no")

        var cnt = 1000;
        while(cnt > 0 ) {
            //actslist.sort(function (a, b) { return a.priority > b.priority });
            var got = actslist.pop();
            if(got) {
                got.fun(got, cur_params);
            } else if( nextlist.length > 0 ) {
                step_i ++;
                actslist = nextlist.shift() || [];
            } else {
                done = true; break;
            }
            cnt -= 1;
        }
        busy = false;
        draw_it();
    }, 20);
}
