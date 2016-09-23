function collide_diag(self, params) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 ) {
            if( pix[i+1] != 0 && pix[i+2] != 0 ) {
                pix[i] = 255; pix[i+1] = 0; pix[i+2] = 0;
                reg(self);
            }
        } else {
            self.init_k -= params.k_sub;
            self.fun = collide_stepper_splode;
            reg(self, 10);
        }
    }
}

function collide_diag_splode(self) {
    function add(dx,dy) {
        reg({fun:collide_diag, x:self.x + dx, y:self.y + dy, dx:dx,dy:dy, 
             k:self.init_k, init_k:self.init_k})
    }
    add(1,-1);  add(1,1);
    add(-1,-1); add(-1,1);
}

function collide_stepper(self) {
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
            self.fun = collide_diag_splode;
            reg(self);
        }
    }
}

function collide_stepper_splode(self) {
    function add(dx,dy) {
        reg({fun:collide_stepper, x:self.x, y:self.y, dx:dx,dy:dy, 
             k:self.init_k, init_k:self.init_k})
    }
    add(0,1);  add(1,0);
    add(0,-1); add(-1,0);
}

named_patterns.collide = {
    params : {
        init_k : 100, k_sub : 2,
        distx : 20, disty : 44,
    },
    setup : function(params) {
        reg({fun:collide_stepper_splode, x:w/2 + params.distx, y:h/2 + params.disty,
             init_k:params.init_k});
        reg({fun:collide_stepper_splode, x:w/2 - params.distx, y:h/2 - params.disty,
             init_k:params.init_k});
    }
}
