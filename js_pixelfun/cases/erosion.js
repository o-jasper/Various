
function erosion_sink(x,y, N, fi) {
    if( N < 0 ) { return; }
    var dirs = [[0,1],[1,0], [0,-1],[-1,0]];
    var k = i_of_xy(x,y);

    var i=Math.floor(8+8*Math.random()), di=(Math.random()>0.5 ? -1 : +1);
    for( var m=0; m<4 ; m++ ){
        var tx = x + dirs[i%4][0], ty = y + dirs[i%4][1];
        var kt = i_of_xy(tx, ty);
        var p = pix[k];

        if( pix[kt] > p + 2 && i%4 != fi) {
            var n = p + pix[kt], h,l;
            if( n%2 == 0 ) {
                h = n/2 +1;
                l = n/2 -1;
            } else {
                h = (n + 1)/2;
                l = (n - 1)/2;
            }
            pix[k +0] = h; pix[k +1] = h; pix[k +2] = h;
            pix[kt+0] = l; pix[kt+1] = l; pix[kt+2] = l;
            erosion_sink(tx,ty, N-1, (i+2)%4);
        }
        i += di;
    }
}

function erosion_actual_sink(self) {
    var k = i_of_xy(self.x,self.y);
    pix[k] += self.c; pix[k+1] += self.c; pix[k+2] += self.c;

    erosion_sink(self.x, self.y, 32);
}

function erosion_rainwater(self, params) {
    self.k -= 1;
    var x = self.x, y = self.y;
    var k = i_of_xy(x,y);
    if( pix[k] == 0 || self.k<0 ) {
        if( params.redeposit ){
            pix[k] += self.loaded; pix[k+1] += self.loaded;pix[k+2] += self.loaded;
        }
        return;
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
        if( params.not_flat && min_pix >= pix[k] ){
            if( params.redeposit ){
                pix[k] += self.loaded; pix[k+1] += self.loaded;pix[k+2] += self.loaded;
                return;
            }
        }
        var c = (self.loaded > 0 && Math.random() < 0.5 ? -params.take : params.give);
        reg({fun:erosion_actual_sink, x:self.x, y:self.y, c:c},
            32);
        self.loaded += c;

        erosion_rainwater({k:self.k, loaded:self.loaded,
                           prev:min_i, x:x + dirs[min_i][0], y:y + dirs[min_i][1]},
                          params);
    }
}
function erosion_rand_rainwater(self, params) {
    for( var i=0; i<params.sink_rate ; i++ ){ erosion_sink(rand_x(), rand_y(), 32); }

    for( var i=0 ; i<params.rain_rate ; i++ ) {
        erosion_rainwater({k:params.ks, x:rand_x(), y:rand_y(), loaded:0}, params);
    }

    var x= w/2 + Math.floor(10*Math.random()), y= h/2 + Math.floor(10*Math.random());
    var k = i_of_xy(x,y);
    pix[k]=0; pix[k+1]=0; pix[k+2]=0;
    erosion_sink(x,y, 8);

    reg({fun:erosion_rand_rainwater});
}

named_patterns.erosion = {
    params : { ks : 3000, dirs:[[0,1],[1,0], [0,-1],[-1,0]],
               rate:60, sink_rate:1, rain_rate:2, not_flat:true,
               give:1, take:10,

               initial_bottom:127,
             },
    setup : function(params) {
        for( var i = 0 ; i<w ; i++ ) {
            for( var j = 0 ; j<h ; j++ ) {
                var k = 4*(i+w*j);
                var di = i-w/2, dj = j-h/2;

                var c = Math.min(255,
                                 params.initial_bottom + Math.abs(di) + Math.abs(dj));
                if( j < 0.45*h || j > 0.55*h ){ c = 255; }

                var q = (Math.floor((w+h+di+dj)/100) + Math.floor((w+h+di-dj)/100))%2
                c = Math.min(c, Math.max(q==0 ? 255 : 150,
                                         500-(di*di+dj*dj)/100));

                pix[k] = c;pix[k+1] = c;pix[k+2] = c;
            }
        }
        reg({fun:erosion_rand_rainwater});
    }
}
