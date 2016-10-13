
function erosion_sink(x,y, N) {
    if( N < 0 ) { return; }
    var dirs = [[0,1],[1,0], [0,-1],[-1,0]];
    var k = i_of_xy(x,y);
    var p = pix[k];
    for( var i in dirs ){
        var tx=x + dirs[i][0], ty = y + dirs[i][1];
        var kt = i_of_xy(tx, ty);
        if( pix[kt] > p+3 ){
            var n = p + pix[kt]
            var n2 = 2*Math.floor(n/2), h,l;
            if( n2 == n ) {
                h = n/2 +1;
                l = n/2 -1;
            } else {
                h = (n + 1)/2;
                l = (n - 1)/2;
            }
            pix[k +0] = l; pix[k +1] = l; pix[k +2] = l;
            pix[kt+0] = h; pix[kt+1] = h; pix[kt+2] = h;
            erosion_sink(tx,ty, N-1);
        }
    }
}

function erosion_actual_sink(self) {
    var k = i_of_xy(self.x,self.y);
    pix[k] += self.c; pix[k+1] += self.c; pix[k+2] += self.c;

    erosion_sink(self.x, self.y, 64);
}

function erosion_rainwater(self, params) {
    self.k -= 1;
    var x = self.x, y = self.y;
    var k = i_of_xy(x,y);
    if( pix[k] == 0 || self.k<0 ) {
        erosion_rand_rainwater(params);
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
        reg({fun:erosion_rainwater, k:self.k,
             prev:min_i, x:x + dirs[min_i][0], y:y + dirs[min_i][1]});

        if( Math.random() < 0.5 ) {
            reg({fun:erosion_actual_sink, x:self.x, y:self.y, c:(self.loaded ? +1 : -1)},
                1024);
            self.loaded = !self.loaded;
        }
    }
}
function erosion_rand_rainwater(params) {
    for( var i = 1; i<300 ; i++){ sink(rand_x(), rand_y(), 8); }

    reg({fun:erosion_rainwater, k:params.ks, x:rand_x(), y:rand_y()});
}

named_patterns.erosion = {
    params : { ks : 300, dirs:[[0,1],[1,0], [0,-1],[-1,0]] },
    setup : function(params) {
        var k = i_of_xy(w/2,h/2);
        pix[k]=0; pix[k]=0; pix[k]=0;
        erosion_sink(w/2,h/2, 64);

        erosion_rand_rainwater(params);
    }
}
