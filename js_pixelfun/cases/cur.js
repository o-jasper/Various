function cur_diag(self, params) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 ) {
            if( pix[i+1] != 0 && pix[i+2] != 0 ) {
                var rgb = params.dia_rgb;
                pix[i] = rgb[0]; pix[i+1] = rgb[1]; pix[i+2] = rgb[2];
                reg(self);
            }
        } else if(self.init_k - self.k > params.min_range) {
            self.init_k -= params.k_sub;
            self.fun = cur_stepper_splode;
            reg(self, 10);
        }
    }
}

function cur_diag_splode(self) {
    function add(dx,dy) {
        reg({fun:cur_diag, x:self.x + dx, y:self.y + dy, dx:dx,dy:dy,
             k:self.init_k, init_k:self.init_k})
    }
    add(1,-1);  add(1,1);
    add(-1,-1); add(-1,1);
}

function cur_stepper(self) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 ) {
            pix[i] = 0; pix[i+1] = 0; pix[i+2] = 0;
            reg(self);
        }
        else {
            self.init_k -= 5;
            self.x += self.dx;
            self.y += self.dy;
            self.fun = cur_diag_splode;
            reg(self);
        }
    }
}

function cur_stepper_splode(self) {
    function add(dx,dy) {
        reg({fun:cur_stepper, x:self.x, y:self.y, dx:dx,dy:dy,
             k:self.init_k, init_k:self.init_k})
    }
    add(0,1);  add(1,0);
    add(0,-1); add(-1,0);
}

named_patterns.cur = {
    params:{
        min_range : 0,
        init_k : 80, k_sub : 9,
        distx : 20, disty : 34,
        dia_rgb : [255,0,0]
    },
    setup : function(params) {
        reg({fun:cur_stepper_splode, x:w/2 + params.distx, y:h/2 + params.disty,
             init_k:params.init_k});
        reg({fun:cur_stepper_splode, x:w/2 - params.distx, y:h/2 - params.disty,
             init_k:params.init_k});
    }
}

named_patterns.cur_minr = {
    params: {
        min_range : 2,
        init_k : 120, k_sub : 9,
        distx : 20, disty : 34,
        dia_rgb : [0,0,255]
    },
    setup:named_patterns.cur.setup,
}
