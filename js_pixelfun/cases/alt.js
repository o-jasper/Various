function alt_rect_round(self, d, col) {
    for( var x = self.x - d ; x < self.x + d ; x++ ) {
        for( var y = self.y - d ; y < self.y + d ; y++ ) {
            var i = i_of_xy(x,y);
            for( var k = 0; k < col.length ; k++ ){ pix[i+k] = col[k]; }
        }
    }
}

function alt_split_off(self, set) {
    var a = {dx:-self.dy, dy:+self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k};
    var b = {dx:+self.dy, dy:-self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k};
    for( k in set){ a[k] = set[k]; b[k] = set[k]; }
    reg(a); reg(b);
}

function alt_stepper_r(self) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if(pix[i] == 0 && pix[i+1] == 0 && pix[i+2] == 255 ) {
            //split_off(self, {fun:stepper_r, first:true, init_k:2*self.init_k});
            self.init_k += 3;
            self.fun = alt_stepper_b_splode;
            rect_round(self, 1, [0,0,255]);
            reg(self, 10);
        } else if( pix[i+2] == 255 ) {
            pix[i] = 255; pix[i+1] = 0; pix[i+2] = 0;
            reg(self, 10);
        }
    } else if( self.first ) {
        alt_split_off(self, {fun:alt_stepper_r, first:false});
    }
}
function alt_stepper_b(self) {
    if( self.k > 0 ) {
        self.k--;

        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if(pix[i] == 255 && pix[i+1] == 0 && pix[i+2] == 0 || 
           self.k == self.init_k/2 ) {
            self.fun = alt_stepper_r_splode;
            if( self.k != self.init_k/2 ){ rect_round(self, 1, [255,0,0]); }
            reg(self, 10);
        } else if( pix[i+2] == 255 ) {
            pix[i] = 0; pix[i+1] = 0; pix[i+2] = 255;
            reg(self);
        }
    } else if( self.first ) {
        split_off(self, {fun:alt_stepper_b, first:false});
    }
}

function alt_stepper_r_splode(self) {
    function add(dx,dy) {
        reg({fun:alt_stepper_r, x:self.x, y:self.y, dx:dx,dy:dy, first:true,
             k:self.init_k, init_k:self.init_k})
    }
    pix[i_of_xy(self.x, self.y) + 1] = 128;
    add(0,1);  add(1,0);
    add(0,-1); add(-1,0);
}
function alt_stepper_b_splode(self) {
    function add(dx,dy, wait) {
        reg({fun:alt_stepper_b, x:self.x, y:self.y, dx:dx,dy:dy, first:true,
             k:self.init_k, init_k:self.init_k}, wait);
    }
    pix[i_of_xy(self.x, self.y) + 1] = 128;
    add(1,1);  add(1,-1);
    add(-1,1); add(-1,-1);

    add(1,1, 4);  add(1,-1, 8);
    add(-1,1, 4); add(-1,-1, 8);
}

named_patterns.alt = {
    params : { init_k1:10, init_k2:15},
    setup : function(params) {
        reg({fun:alt_stepper_r_splode, x:w/2, y:h/2, init_k:10});
        reg({fun:alt_stepper_b_splode, x:w/2, y:h/2, init_k:15});
    }
}

