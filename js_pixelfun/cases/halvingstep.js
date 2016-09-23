function halvingstep_stepper(self) {
    if( self.k > 0 ) {
        self.k--;
        self.x += self.dx;
        self.y += self.dy;
        var i = i_of_xy(self.x, self.y);
        if( pix[i] == 255 ) {
            pix[i] = 0; pix[i+1] = 0; pix[i+2] = 0;
            reg(self);
        }
    } else if(self.init_k > 0) {
        reg({fun:halvingstep_stepper,
             dx:-self.dy, dy:+self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k-1});
        reg({fun:halvingstep_stepper,
             dx:+self.dy, dy:-self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k-1});
    }
}

function halvingstep_stepper_splode(self) {
    function add(dx,dy) {
        reg({fun:halvingstep_stepper, x:self.x, y:self.y, dx:dx,dy:dy, 
             k:self.init_k, init_k:self.init_k})
    }
    add(0,1);  add(1,0);
    add(0,-1); add(-1,0);
}

named_patterns.halvingstep = {
    params : { init_k:20 },
    setup : function(params) {
        reg({fun:halvingstep_stepper_splode, x:w/2, y:h/2, init_k:params.init_k});
    }
}
