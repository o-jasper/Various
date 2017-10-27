
function dia_city_diag(self, params) {
    var k = i_of_xy(self.x,self.y);
    pix[k] = 0; pix[k + 1] = 0;pix[k + 2] = 0;

    self.x += self.dx; self.y += self.dy;
    self.k--;

    var k = i_of_xy(self.x,self.y);
    if( pix[k] == 0 || pix[k+3] == 0){
        self.ks -= params.k_sub;
    }

    if( self.ks <= params.ks_min ) {
        return;
    } else if( self.k < 0 ){
        self.ks += params.k_regain;

        var dx = self.dx, dy = self.dy;
        reg({ fun:dia_city_diag, x:self.x,y:self.y, dx:-dy,dy:+dx,
              k:self.ks + params.inba, ks:self.ks});
        reg({ fun:dia_city_diag, x:self.x,y:self.y, dx:+dy,dy:-dx,
              k:self.ks - params.inba, ks:self.ks});
    } else{ reg(self); }
}

named_patterns.dia_city = {
    params : { ks:32, k_sub:3, k_regain:0, ks_min:2, inba:0},
    setup : function(params) {
        reg({fun:dia_city_diag, x:w/2, y:h/2, dx:-1, dy:1,
             k:params.ks, ks : params.ks});
    }
}

named_patterns.dia_city_inba = {
    params : { ks:20, k_sub:3, k_regain:1, ks_min:10, inba:6},
    setup : named_patterns.dia_city.setup
}
