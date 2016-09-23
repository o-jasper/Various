var params = {
    init_k : 19,
    leaf_init_k : 4,
    asymmetry : 1
};

function uglytree_raytrace(self) {
    self.y -= 1;
    if( self.y <= 20 ) {
        var i = i_of_xy(self.x, self.from_y);
        pix[i] = 0; pix[i+1] = 200; pix[i+2] = 0;
        if( self.k > 0 ){
            self.k--;
            self.x += self.dx;
            self.y = self.from_y - 1;
            reg(self);
        }
    }
    else {
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 && pix[i+1] == 255 && pix[i+2] == 255 ) {
            pix[i] = 240; pix[i+1] = 240; pix[i+2] = 0;
        }
        reg(self);
    }
}

function uglytree_leafize(self) {
    self.from_y = self.y;
    self.fun = uglytree_raytrace;
    self.k = params.leaf_init_k;
    reg(self);
}

function uglytree_stem(self) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx; self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] != 200 ) {
            for( var k = -1 ; k < 1 ; k++) {
                pix[i + 4*k] = 200; pix[i+1 + 4*k] = 140; pix[i+2 + 4*k] = 0;
            }
            reg(self);
        } else {
            uglytree_leafize(self);
        }
    } else if(self.init_k > 2) {
        self.init_k--;
        if( self.dx == 0 ) {
            self.k = self.init_k;
            reg({fun:uglytree_stem, x:self.x, y:self.y, dx:-1, dy:-1,
                 k:self.k, init_k:self.init_k - params.asymmetry*(self.k%2)});
            reg({fun:uglytree_stem, x:self.x, y:self.y, dx:+1, dy:-1,
                 k:self.k, init_k:self.init_k - params.asymmetry*(self.k%2 - 1)});
        } else {
            self.k = self.init_k;
            self.dx = 0; self.dy = -1;
            reg(self);
        }
    } else { 
        uglytree_leafize(self);
    }
}

named_patterns.uglytree = {
    params : params,
    setup : function(params) {
        reg({fun:uglytree_stem, x:w/2, y:h, dx:0,dy:-1, k:params.init_k,
             init_k:params.init_k});
    }
}
