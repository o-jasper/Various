
function first_red_horsy(self) {
    var n = 5, m=3;
    var x = self.x, y = self.y, dx=self.dx, dy= self.dy;
    for(var k = 0 ; k<n ; k++ ){
        var i = i_of_xy(x + k*dx, y + k*dy);
        if( pix[i][0] == 0 && pix[i][1] == 0 && pix[i][2] == 255 ) {
            self.x = x + k*dx;
            self.y = y + k*dy;
            self.fun = first_horsy_splode;
            reg(self);
            return;
        }
        just_color({i : i, color:[255,0,0,255]});
    }
    for(var k = 0 ; k<m ; k++ ){
        var i = i_of_xy(x + n*dx + dy*k, y + n*dy - dx*k);
        if( pix[i][0] == 0 && pix[i][1] == 0 && pix[i][2] == 255 ) {
            self.x = x + n*dx + dy*k;
            self.y = y + n*dy - dx*k;
            self.fun = first_horsy_splode;
            reg(self);
            return;
        }
        just_color({i : i, color:[255,0,0,255]});
    }

    x += n*dx + m*dy;
    y += n*dy - m*dx;
    for(var k = 0 ; k<2 ; k++ ){
        if(self.k > 0) {
            reg({fun : first_red_horsy,
                 x : x, y : y,
                 dx : (self.k + k)%2,
                 dy : !((self.k + k)%2),
                 k  : self.k - 1
                }, 3);
        }
    }
}

function first_horsy_splode(self) {
    reg({fun:first_red_horsy, x:self.x, y:self.y,
         dx: +self.dy, dy: -self.dx, k:self.k});
    reg({fun:first_red_horsy, x:self.x, y:self.y,
         dx: -self.dx, dy: +self.dx, k:self.k});
}

function first_stepper(self) {
    just_color({i :i_of_xy(self.x, self.y), color:[0,0,255,255]});
    
    // Dont need a copy..(popped)
    self.x += self.dx;
    self.y += self.dy;
    self.k -= 1;
    var i = 4*(self.x + w*self.y);
    if( pix[i] == 255 && pix[i+1] == 0 ) {
        self.fun = first_diag_splode;
        self.k = 20;
    }
    if( pix[i] == 0 && pix[i+1] == 0 && pix[i+2] == 255 ){ return; }
    if( self.k > 0 ){ reg(self); }
}

var s = 1;
function first_diag_splode(self) { 
    reg({fun:first_stepper, x:self.x, y:self.y, dx: s, dy:+s, k:self.k, priority:2});
    reg({fun:first_stepper, x:self.x, y:self.y, dx: s, dy:-s, k:self.k, priority:2});
    reg({fun:first_stepper, x:self.x, y:self.y, dx:-s, dy:-s, k:self.k, priority:2});
    reg({fun:first_stepper, x:self.x, y:self.y, dx:-s, dy:+s, k:self.k, priority:2});
}

named_patterns.first = {
    params:{ init_k1:10, init_k2:20 },

    setup : function(params) {
        reg({fun:first_red_horsy, x:w/2, y:h/2, dx:1,dy:0, k:params.init_k1});
        reg({fun:first_diag_splode, x:w/2, y:h/2, k:params.init_k2});
    }
}
