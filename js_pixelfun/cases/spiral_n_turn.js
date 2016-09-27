
function spiral_step(self, params) {
    self.k -= 1;
    var xdirs = params.xdirs, ydirs = params.ydirs;
    self.x += xdirs[(self.ori + self.dir)%xdirs.length];
    self.y += ydirs[(self.ori + self.dir)%ydirs.length];

    var i = i_of_xy(self.x, self.y);
    if( pix[i] < 255 ) {
        var f = (255-pix[i])/256.0;
        self.k  -= params.k_sub*f;
        self.ks -= params.ks_sub*f;
    }
    pix[i] = Math.max(0, 255-(50*self.ks)); pix[i+1] = (10*self.ks)%256; pix[i+2] = (10*self.k)%256;

    if(self.ks <= params.min_k ) {  // Ran out of steam.
        return;
    }
    if(self.k < 0) {  // End of this direction.
        self.dir += 1;
        self.k = self.ks;  // Change direction.
        if( self.dir >= params.dir_cnt ){
            self.ks += params.k_gain;

            self.dir = 0;

            // Second branch.
            reg({fun:spiral_step, ori: self.ori + params.R_turn, dir:0,
                 x:self.x, y:self.y,
                 k:self.ks, ks:self.ks});
            self.ori += params.L_turn;
        }
    }
    reg(self)
}

named_patterns.spiral_n_turn = {
    params : {
        init_k : 15, min_k : 5,
        k_sub : 0, ks_sub : 0.1, k_gain : 2,

        xdirs : [0, 1, 1,  1, 0,  -1, -1, -1],
        ydirs : [1, 1, 0, -1, -1, -1, 0,   1],
        dir_cnt : 5,

        L_turn : 1, R_turn : 2,
    },
    setup : function(params) {
        reg({fun:spiral_step, ori:0, dir:0, x:w/2, y:w/2,
             k:params.init_k, ks:params.init_k});
    }
}
