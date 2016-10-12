

function sink(x,y, k) {
    if( k < 0 ) { return; }
    var dirs = [[0,1],[1,0], [0,-1],[-1,0]];
    var p = pix[i_of_xy(x,y)];
    if( p > 250 ) { return; }
    for( var i in dirs ){
        var tx=x + dirs[i][0], ty = y + dirs[i][1];
        var kt = i_of_xy(tx, ty);
        if( pix[kt] > p+4 ){
            pix[kt] = p + 4;
            pix[kt + 1] = p + 4;
            pix[kt + 2] = p + 4;
            sink(tx,ty, k-1);
        }
    }
}

function rainwater(self, params) {
    self.k -= 1;
    var x = self.x, y = self.y;
    var k = i_of_xy(x,y);
    pix[k] -= 1; pix[k+1] -= 1; pix[k+2] -= 1;
    sink(x,y, 8);
    if( pix[k] == 0 || self.k<0 ) {
        rand_rainwater(params);
    } else {
        var dirs = params.dirs;
        var min_i, min_pix=1256;
        var i=Math.floor(8+8*Math.random()), di=(Math.random()>0.5 ? -1 : +1);
        for( var n=0; n<4 ; n++ ){
            i += di;
            var kt = i_of_xy(x + dirs[i%4][0], y + dirs[i%4][1]);
            if( pix[kt] < min_pix && (((i+2)%4)!=self.prev) ) {
                min_i = i%4;
                min_pix = pix[kt];
            }
        }
        reg({fun:rainwater, k:self.k,
             prev:min_i, x:x + dirs[min_i][0], y:y + dirs[min_i][1]});
    }
}
function rand_rainwater(params) {
    reg({fun:rainwater, k:params.ks,
         x:Math.floor(w*Math.random()), y:Math.floor(w*Math.random())});
}

named_patterns.erosion = {
    params : { ks : 30, dirs:[[0,1],[1,0], [0,-1],[-1,0]] },
    setup : function(params) {
        rand_rainwater(params);
    }
}
