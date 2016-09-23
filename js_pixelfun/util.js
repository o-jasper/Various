function rect_round(self, d, col) {
    for( var x = self.x - d ; x < self.x + d ; x++ ) {
        for( var y = self.y - d ; y < self.y + d ; y++ ) {
            var i = i_of_xy(x,y);
            for( var k = 0; k < col.length ; k++ ){ pix[i+k] = col[k]; }
        }
    }
}

function split_off(self, set) {
    var a = {dx:-self.dy, dy:+self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k};
    var b = {dx:+self.dy, dy:-self.dx, x:self.x, y:self.y, 
             k:self.init_k, init_k:self.init_k};
    for( k in set){ a[k] = set[k]; b[k] = set[k]; }
    reg(a); reg(b);
}
