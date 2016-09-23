function fancied_stepper(self, params) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx; self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 ) {
            if( pix[i+1] == 120 ) {
                self.dx = -self.dy
                self.dy = self.dx
                self.init_k += params.spark_plus_init_k;
            }
            pix[i] = 0; pix[i+1] = 0; pix[i+2] = 0;
            reg(self);
        } else {
            self.fun = fancied_splosion;
            reg(self);
        }
    } else if(self.init_k > 0) {
        reg({fun:fancied_stepper,
             dx:-self.dy, dy:+self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k-1});
        reg({fun:fancied_stepper,
             dx:+self.dy, dy:-self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k-1});
    }
}

function fancied_spark(self) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx; self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 0 && pix[i+1] == 0 && pix[i+2] == 0 ) {
            return;
        }
        pix[i] = 255; pix[i+1] = 120; pix[i+2] = 0;
        reg(self);
    }
}

function fancied_splosion(self) {
    rect_round(self, 1, [255,0,0]);
    function add(dx,dy) {
        reg({fun:fancied_spark, x:self.x + dx, y:self.y + dy, dx:dx,dy:dy, 
             k:30});
    }
    if( self.dy != 0 ){
        add(1,self.dy>0 ? +1 : -1);   add(-1, self.dy>0 ? +1 : -1);
    } else {
        add(self.dx>0 ? +1 : -1, -1); add(self.dx>0 ? +1 : -1, +1);
    }
}

function fancied_stepper_splode(self) {
    function add(dx,dy) {
        reg({fun:fancied_stepper, x:self.x, y:self.y, dx:dx,dy:dy, 
             k:self.init_k, init_k:self.init_k})
    }
    add(0,1);  add(1,0);
    add(0,-1); add(-1,0);
}

named_patterns.fancied = {
    params : {
        init_k : 30,
        spark_plus_init_k : 4,
    },
    setup : function(params) {
        reg({fun:fancied_stepper_splode, x:w/2, y:h/2, init_k:params.init_k});
    }
}
