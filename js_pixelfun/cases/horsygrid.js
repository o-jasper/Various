
function horsygrid_red_horsy(self) {
    var n = 5, m=3;
    var x = self.x, y = self.y, dx=self.dx, dy= self.dy;
    for(var k = 0 ; k<n ; k++ ){
        var i = i_of_xy(x + k*dx, y + k*dy);
        just_color({i : i, color:[255,0,0,255]});
    }
    for(var k = 0 ; k<m ; k++ ){
        var i = i_of_xy(x + n*dx + dy*k, y + n*dy - dx*k);
        just_color({i : i, color:[255,0,0,255]});
    }

    x += n*dx + m*dy;
    y += n*dy - m*dx;
    if(self.k > 0) {
        var wait = 3;
        reg({fun : horsygrid_red_horsy,
             x : x, y : y,
             dx : self.dx, dy : self.dy,
             k  : self.k - 1
            }, wait);
        reg({fun : horsygrid_red_horsy,
             x : x, y : y,
             dx : -self.dy, dy : self.dx,
             k  : self.k - 1
            }, wait);
    } else {
        self.x = x; self.y = y;
        self.fun = horsygrid_green_horsy_splode;
        reg(self, 5);
        return;
    }
}

function horsygrid_green_horsy_splode(self) {
    var d = 20, x=self.x, y=self.y;
    for( var i = x - d ; i < x + d ; i++ ) {
        for( var j = x - d ; j < x + d ; j++ ) {
            var index = i_of_xy(i,j);
            if( pix[index] == 0 && pix[index + 1] == 0 && pix[index + 2] == 255 ) {
                horsygrid_horsy_splode(self);
                return;
            }
        }
    }
}

function horsygrid_horsy_splode(self) {
    reg({fun:horsygrid_red_horsy, x:self.x, y:self.y,
         dx: +self.dy, dy: -self.dx, k:self.k});
    reg({fun:horsygrid_red_horsy, x:self.x, y:self.y,
         dx: -self.dy, dy: +self.dx, k:self.k});
}

function horsygrid_stepper(self) {
    just_color({i :i_of_xy(self.x, self.y), color:[0,0,255,255]});
    
    // Dont need a copy..(popped)
    self.x += self.dx;
    self.y += self.dy;
    self.k -= 1;
    //if( pix[i] == 0 && pix[i+1] == 0 && pix[i+2] == 255 ){ return; }
    if( self.k > 0 ){ reg(self); }
    else {
        self.fun = horsygrid_sleep_stepper;
        reg(self, 5);
    }
}

function horsygrid_sleep_stepper(self) {
    var x = self.x, y = self.y;
    var d = 10, any=false, anti=false;
    for( var i = x - d ; i < x + d ; i++ ) {
        for( var j = y - d ; j < y + d ; j++ ) {
            var index = i_of_xy(i,j);
            if( pix[index] == 255 && pix[index + 1] == 0 && pix[index + 1] == 0 ) {
                any=true;
            } else if( pix[index] == 128 && pix[index + 1] == 128 && pix[index + 1] == 0 ) {
                anti = true;
            } else if( pix[index] == 255 && pix[index + 1] == 255 && pix[index + 1] == 255 ) {
                pix[index] = 0; pix[index + 1] = 255; pix[index + 2] = 0;
            }
        }
    }
    if( anti ){ return; }
    if(any){
        pix[index] = 128; pix[index + 1] = 128; pix[index + 1] = 0;
        self.k = 20;
        diag_splode(self, 2);
    } else {
        reg(self, 5);
    }
}

var s = 1;
function horsygrid_diag_splode(self) { 
    reg({fun:horsygrid_stepper, x:self.x, y:self.y, dx: s, dy:+s, k:self.k, priority:2});
    reg({fun:horsygrid_stepper, x:self.x, y:self.y, dx: s, dy:-s, k:self.k, priority:2});
    reg({fun:horsygrid_stepper, x:self.x, y:self.y, dx:-s, dy:-s, k:self.k, priority:2});
    reg({fun:horsygrid_stepper, x:self.x, y:self.y, dx:-s, dy:+s, k:self.k, priority:2});
}

named_patterns.horsygrid = {
    params : { init_k1:5, init_k2:20},
    setup : function(params){
        reg({fun:horsygrid_horsy_splode, x:w/2, y:h/2, dx:1,dy:0, k:params.init_k1});
        actslist.push({fun:horsygrid_diag_splode, x:w/2, y:h/2, k:params.init_k2});
    }
}
